# MOWER dashboard — web control panel for the mower over SWD

Web panel on the Raspberry Pi: telemetry + drive/blade control of our firmware on
chip1 (GD32F305) via a mailbox in SRAM, on top of OpenOCD telnet `:4444`.

**stdlib Python 3.7+ only — no pip dependencies.** The frontend is fully
self-contained (a single HTML, no CDN).

```
tools/dashboard/
├── server.py            # backend: telnet :4444 (single connection + lock), poller, dead-man, HTTP API
├── static/
│   └── index.html       # frontend: telemetry, sliders, STOP, ARM, BLADE (all inline)
└── README.md
```

## Deploy to the Pi (pi@192.168.10.81)

```bash
# from the Mac (the Pi wifi is terrible — copy patiently):
scp -r tools/dashboard pi@192.168.10.81:~/dashboard

# on the Pi:
# 1) make sure openocd is up and NOBODY else is sitting on :4444
sudo systemctl status mower-openocd        # should be active (poll.cfg)
sudo systemctl stop mower-webctl 2>/dev/null || true   # :4444 — ONE client!
# bench scripts (swd.py etc.) must also be closed

# 2) start (detached — wifi drops, nohup/tmux mandatory):
cd ~/dashboard
nohup python3 server.py > dash.log 2>&1 &

# 3) from a phone/Mac on the same network:
#    http://192.168.10.81:8080
```

The port is changed via env: `DASH_PORT=9090 python3 server.py`.
Stopping: `pkill -f dashboard/server.py` (on exit via Ctrl-C the server itself
does stop+disarm).

## How it works

- **A single telnet connection** to OpenOCD `127.0.0.1:4444` (the `tools/bench/swd.py` pattern).
  All access is serialized through one `Lock` — the poller, keep-alive and HTTP commands
  never write to the socket simultaneously. Each command ~10–150 ms.
- **Poller** (~4 Hz): one `mdw 0x20014000 9` → decode all fields → cache.
  `GET /api/telemetry` returns the cache instantly, the UI does not wait for SWD.
- If openocd is down / the read returned garbage → `alive:false`, reconnect
  with a 2 s backoff. The server never crashes.

## Dead-man / safety (two-link chain)

1. **Firmware ↔ backend (seq @ +0x08).** While armed, the keep-alive thread bumps
   `seq` every ~150 ms; the poller and each command additionally bump seq
   piggyback under the same lock. If the backend died/hung — seq freezes,
   the firmware stops the motors itself after ~200 ms.
2. **Backend ↔ browser (freshness of /api/drive).** While the wheels are turning, the frontend
   sends `/api/drive` every 200 ms. If commands stop arriving for >1 s
   (the phone went to sleep, wifi dropped) — the backend itself zeroes the speeds
   (arm is preserved, seq keeps being bumped).
3. **UI:** spring sliders (release = instantly 0), active only when ARMED.
   STOP — always: speeds 0 + blade 0 + DISARM. The blade turns on only when ARMED
   and only via a two-step confirmation (3 s); it turns off with a single press.
   Leaving the page (visibilitychange) sends a `/api/stop` beacon.

**Before the first run:** wheels suspended, blade DISCONNECTED. Verify STOP and
the dead-man (kill the server while driving — the wheels must stop within ~200 ms) before
any tests on the ground.

## HTTP API

| Method/path            | Body                      | Action |
|-----------------------|---------------------------|----------|
| `GET /api/telemetry`  | —                         | JSON cache of telemetry (see below) |
| `POST /api/arm`       | `{"on": true/false}`      | ARM (magic=0x5243414D, speeds 0) / DISARM (speeds 0, blade 0, magic 0) |
| `POST /api/drive`     | `{"left": -7200..7200, "right": ...}` | Wheel speeds (single mww to +0x0C); if not armed — arms; bumps seq |
| `POST /api/blade`     | `{"on": true/false}`      | Blade. Turning on requires ARM (otherwise 400) |
| `POST /api/stop`      | `{}`                      | Full stop: speeds 0, blade 0, DISARM |

Command response: `{"ok":true}` or `{"ok":false,"error":"..."}` (400 — logic,
503 — SWD/link).

`GET /api/telemetry` →
```json
{
  "connected": true,          // telnet to openocd alive
  "alive": true,              // heartbeat changed within ~0.7 s (firmware alive)
  "heartbeat": 12345678,
  "armed": true,              // what the backend wrote
  "armed_fw": true,           // magic in mailbox == 0x5243414D
  "fw_state": 2, "fw_state_name": "DRIVING",   // 0=DISARMED 1=ARMED-IDLE 2=DRIVING
  "left_speed": 3600, "right_speed": 3600,      // written i16
  "blade_on": false,
  "batt_mV": 25100, "batt_pct": 87, "charge": false,
  "ch3_duty": 3590, "ch2_duty": 3612, "ch1_duty": 0,  // actual PWM: left/right/blade
  "btn": false, "seq": 4242, "speed_max": 7200, "ts": 1751500000.0
}
```

## Mailbox contract (base = 0x20014000)

CONTROL — written by the dashboard (`mww`/`mwh`):

| Offset | Type | Field | Note |
|---|---|---|---|
| +0x04 | u32 | magic | `0x5243414D` = ARM, `0` = disarm |
| +0x08 | u32 | seq | dead-man: must change more often than ~200 ms while armed |
| +0x0C | i16 | left_speed | −7200..+7200; low16 of the word |
| +0x0E | i16 | right_speed | high16 of the same word — written with a single `mww 0x2001400C` |
| +0x10 | u8 | blade_on | 0/1 (written with `mwh`) |

TELEMETRY — written by the firmware (read with a single `mdw 0x20014000 9`):

| Offset | Type | Field |
|---|---|---|
| +0x00 | u32 | heartbeat (~1 kHz; frozen = firmware hung/off) |
| +0x14 | u16 | batt_mV |
| +0x16 | u8 | batt_pct 0..100 |
| +0x17 | u8 | charge 0/1 |
| +0x18 | u16 | ch3_duty (left wheel, actual PWM) |
| +0x1A | u16 | ch2_duty (right wheel) |
| +0x1C | u16 | ch1_duty (blade) |
| +0x1E | u8 | btn (power button) |
| +0x1F | u8 | fw_state (0=disarmed, 1=armed-idle, 2=driving, …) |

## Known limitations

- `:4444` — strictly one client. While the dashboard is running, the bench scripts and
  mower-webctl cannot be used (and vice versa).
- SWD commands are slow (up to ~150 ms); if the firmware complains about the dead-man
  while driving, you can raise `KEEPALIVE_PERIOD`/`POLL_PERIOD` in `server.py`
  (e.g. poll 2 Hz).
- There is no authentication — only a trusted local network.
