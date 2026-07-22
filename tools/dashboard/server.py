#!/usr/bin/env python3
"""
server.py — mower web dashboard via SWD (OpenOCD telnet :4444).
Run on the RASPBERRY PI. Python 3.7+, stdlib ONLY (no pip dependencies).

Architecture:
  - One persistent telnet connection to OpenOCD :4444 (only ONE client
    may sit on :4444 — mower-webctl and bench scripts must be stopped).
  - All SWD accesses are serialized through a single Lock.
  - Poller thread: mdw 0x20014000 9 (~4 Hz) -> decodes telemetry -> cache.
  - Keep-alive thread: while armed, bumps seq (+0x08) ~5 Hz (firmware dead-man
    stops the motors if seq does not change for ~200 ms).
  - Browser watchdog: if armed, speeds != 0 and the client sent no
    /api/drive for longer than DRIVE_TIMEOUT — speeds are zeroed (second
    dead-man link: firmware<->backend by seq, backend<->browser by drive freshness).

Mailbox (base = 0x20014000), contract with the firmware:
  CONTROL (we write):
    +0x04 u32 magic      0x5243414D = ARM, 0 = DISARM
    +0x08 u32 seq        dead-man counter
    +0x0C i16 left_speed  (low16)  | +0x0E i16 right_speed (high16) — in one mww
    +0x10 u8  blade_on
  TELEMETRY (firmware writes):
    +0x00 u32 heartbeat (~1 kHz)
    +0x14 u16 batt_mV | +0x16 u8 batt_pct | +0x17 u8 charge
    +0x18 u16 ch3_duty(left) | +0x1A u16 ch2_duty(right)
    +0x1C u16 ch1_duty(blade) | +0x1E u8 btn | +0x1F u8 fw_state
    +0x20 u16 left_diag | +0x22 u16 right_diag        (A4963 diagnostic words)
    +0x24 u16 blade_diag | +0x26 u16 pad
  -> we read mdw 0x20014000 10 (40 bytes, through +0x27).

A4963 diagnostic-word bit map (datasheet Table 4):
  [15]FF general-fault  [14]POR power-on-reset  [13]SE serial-err
  [11]TW temp-warning   [10]OT overtemp         [9]LOS loss-of-sync
  [7]VS  VBB undervolt  [5]AH/[4]AL/[3]BH/[2]BL/[1]CH/[0]CL per-phase VDS
  (over-current/short). 0xFFFF = SPI read failed.
"""
import json
import os
import socket
import threading
import time
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer

# ---------- config ----------
HTTP_HOST = "0.0.0.0"
HTTP_PORT = int(os.environ.get("DASH_PORT", "8080"))
OPENOCD_HOST = "127.0.0.1"
OPENOCD_PORT = 4444

MBOX = 0x20014000
MAGIC_ARM = 0x5243414D
SPEED_MAX = 7200            # 0x1C20

POLL_PERIOD = 0.25          # telemetry ~4 Hz
KEEPALIVE_PERIOD = 0.15     # bump seq while armed (~5-6 Hz + piggyback)
DRIVE_TIMEOUT = 1.0         # sec without /api/drive at nonzero speeds -> stop
RECONNECT_MIN_INTERVAL = 2.0

STATIC_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), "static")

FW_STATES = {0: "DISARMED", 1: "ARMED-IDLE", 2: "DRIVING"}

# A4963 diagnostic register — (bit, short, long) per the datasheet Table 4.
# Bits 12/8/6 reserved (always 0). Order = from MSB to LSB for display.
A4963_DIAG_BITS = [
    (15, "FF",  "general fault flag"),
    (14, "POR", "power-on reset"),
    (13, "SE",  "serial transmission error"),
    (11, "TW",  "temperature warning"),
    (10, "OT",  "overtemperature (shutdown)"),
    (9,  "LOS", "loss of synchronization"),
    (7,  "VS",  "VBB undervoltage"),
    (5,  "AH",  "phase A high-side VDS (over-current/short)"),
    (4,  "AL",  "phase A low-side VDS"),
    (3,  "BH",  "phase B high-side VDS"),
    (2,  "BL",  "phase B low-side VDS"),
    (1,  "CH",  "phase C high-side VDS"),
    (0,  "CL",  "phase C low-side VDS"),
]


def decode_diag(word):
    """A4963 16-bit diagnostic word -> {raw, hex, ok, valid, flags:[{bit,short,long}]}"""
    word &= 0xFFFF
    # 0xFFFF (all reserved bits set) = SPI read failed / no response.
    valid = (word != 0xFFFF) and ((word & 0x1140) == 0)  # bits 12,8,6 must be 0
    flags = [{"bit": b, "short": s, "long": l}
             for (b, s, l) in A4963_DIAG_BITS if word & (1 << b)]
    return {
        "raw": word,
        "hex": "0x%04X" % word,
        "valid": valid,
        # "ok" = link is up and NO fault bits are set
        "ok": valid and word == 0,
        "flags": flags,
    }


def _i16(v):
    """u16 -> i16"""
    v &= 0xFFFF
    return v - 0x10000 if v & 0x8000 else v


# ---------- thin OpenOCD telnet client (pattern from tools/bench/swd.py) ----------
class SWD:
    def __init__(self, host=OPENOCD_HOST, port=OPENOCD_PORT, settle=0.2):
        self.s = socket.create_connection((host, port), timeout=5)
        self.s.settimeout(0.2)
        time.sleep(settle)
        self._read_until_prompt(0.6)  # consume the banner up to the '>' prompt

    def _read_until_prompt(self, deadline=0.8):
        buf = b""
        end = time.time() + deadline
        while time.time() < end:
            try:
                chunk = self.s.recv(4096)
                if not chunk:
                    raise ConnectionError("openocd closed connection")
                buf += chunk
                if buf.rstrip().endswith(b">"):
                    break
            except socket.timeout:
                if buf:
                    break
        return buf.decode(errors="replace")

    def cmd(self, c):
        self.s.sendall((c + "\n").encode())
        return self._read_until_prompt().strip()

    def mdw(self, addr, count=1):
        out = self.cmd("mdw 0x%08x %d" % (addr, count))
        vals = []
        for line in out.splitlines():
            if ":" in line:
                for tok in line.split(":", 1)[1].split():
                    try:
                        vals.append(int(tok, 16))
                    except ValueError:
                        pass
        return vals

    def mww(self, addr, val):
        return self.cmd("mww 0x%08x 0x%08x" % (addr, val & 0xFFFFFFFF))

    def mwh(self, addr, val):
        return self.cmd("mwh 0x%08x 0x%04x" % (addr, val & 0xFFFF))

    def close(self):
        try:
            self.s.close()
        except Exception:
            pass


# ---------- manager: one connection, one lock, poller, keep-alive ----------
class Mower:
    def __init__(self):
        self.lock = threading.Lock()        # serializes ALL access to :4444
        self.swd = None
        self._last_connect_try = 0.0

        self.seq = 1
        self.armed = False                  # our view (what we wrote)
        self.left = 0
        self.right = 0
        self.blade = False
        self.last_drive_ts = 0.0

        self.telemetry = {"alive": False, "connected": False}
        self._last_hb = None
        self._last_hb_ts = 0.0

        threading.Thread(target=self._poll_loop, daemon=True).start()
        threading.Thread(target=self._keepalive_loop, daemon=True).start()

    # --- connection (call UNDER the lock) ---
    def _ensure_swd(self):
        if self.swd is not None:
            return self.swd
        now = time.time()
        if now - self._last_connect_try < RECONNECT_MIN_INTERVAL:
            raise ConnectionError("openocd: reconnect backoff")
        self._last_connect_try = now
        self.swd = SWD()
        return self.swd

    def _drop_swd(self):
        if self.swd is not None:
            self.swd.close()
            self.swd = None

    def _bump_seq_unlocked(self):
        """Bump the dead-man seq. Call UNDER the lock with a live connection."""
        self.seq = (self.seq + 1) & 0xFFFFFFFF
        self.swd.mww(MBOX + 0x08, self.seq)

    # --- background threads ---
    def _keepalive_loop(self):
        while True:
            time.sleep(KEEPALIVE_PERIOD)
            if not self.armed:
                continue
            # browser watchdog: drive commands stopped arriving -> stop wheels
            stale = ((self.left or self.right)
                     and time.time() - self.last_drive_ts > DRIVE_TIMEOUT)
            try:
                with self.lock:
                    self._ensure_swd()
                    if stale:
                        self.left = self.right = 0
                        self.swd.mww(MBOX + 0x0C, 0)
                    self._bump_seq_unlocked()
            except Exception:
                with self.lock:
                    self._drop_swd()

    def _poll_loop(self):
        while True:
            t0 = time.time()
            try:
                with self.lock:
                    self._ensure_swd()
                    words = self.swd.mdw(MBOX, 10)
                    # piggyback bump: don't let mdw stretch the seq pause
                    if self.armed:
                        self._bump_seq_unlocked()
                if len(words) >= 8:
                    self._decode(words)
                else:
                    self._mark_dead(connected=True)
            except Exception:
                with self.lock:
                    self._drop_swd()
                self._mark_dead(connected=False)
            dt = time.time() - t0
            if dt < POLL_PERIOD:
                time.sleep(POLL_PERIOD - dt)

    def _mark_dead(self, connected):
        t = dict(self.telemetry)
        t["alive"] = False
        t["connected"] = connected
        self.telemetry = t

    def _decode(self, w):
        hb = w[0]
        now = time.time()
        if self._last_hb is None or hb != self._last_hb:
            self._last_hb = hb
            self._last_hb_ts = now
        # alive = heartbeat changed within the last ~0.7 s (2-3 poll cycles)
        alive = (now - self._last_hb_ts) < 0.7
        self.telemetry = {
            "connected": True,
            "alive": alive,
            "heartbeat": hb,
            "magic": w[1],
            "armed_fw": w[1] == MAGIC_ARM,
            "armed": self.armed,
            "seq": w[2],
            "left_speed": _i16(w[3]),
            "right_speed": _i16(w[3] >> 16),
            "blade_on": bool(w[4] & 0xFF),
            "batt_mV": w[5] & 0xFFFF,
            "batt_pct": (w[5] >> 16) & 0xFF,
            "charge": bool((w[5] >> 24) & 0xFF),
            "ch3_duty": w[6] & 0xFFFF,          # left wheel actual PWM
            "ch2_duty": (w[6] >> 16) & 0xFFFF,  # right wheel actual PWM
            "ch1_duty": w[7] & 0xFFFF,          # blade actual PWM
            "btn": bool((w[7] >> 16) & 0xFF),
            "fw_state": (w[7] >> 24) & 0xFF,
            "fw_state_name": FW_STATES.get((w[7] >> 24) & 0xFF,
                                           "?%d" % ((w[7] >> 24) & 0xFF)),
            "speed_max": SPEED_MAX,
            "ts": now,
        }
        # A4963 diagnostic words: +0x20 left|right (w[8]), +0x24 blade (w[9] low)
        # spi_status byte @+0x26 = high-16 low-byte of w[9]: bit0=L,1=R,2=blade
        # SET => that read TIMED OUT (bus not clocking). CLEAR + diag==0xFFFF =>
        # frame completed but driver drove SDO high (silent = hardware/VBB).
        if len(w) >= 10:
            spi = (w[9] >> 16) & 0xFF
            def outcome(bit, diagword):
                if spi & (1 << bit):
                    return "TIMEOUT (SPI bus not clocking = software)"
                if diagword == 0xFFFF:
                    return "exchange completed, driver silent SDO=1 (= hardware/VBB)"
                return "responded"
            self.telemetry["diag"] = {
                "left":  decode_diag(w[8] & 0xFFFF),
                "right": decode_diag((w[8] >> 16) & 0xFFFF),
                "blade": decode_diag(w[9] & 0xFFFF),
            }
            self.telemetry["spi_status"] = {
                "raw": spi,
                "left":  outcome(0, w[8] & 0xFFFF),
                "right": outcome(1, (w[8] >> 16) & 0xFFFF),
                "blade": outcome(2, w[9] & 0xFFFF),
            }

    # --- commands (from HTTP handlers) ---
    def cmd_arm(self, on):
        with self.lock:
            self._ensure_swd()
            if on:
                self.left = self.right = 0
                self.swd.mww(MBOX + 0x0C, 0)                 # speeds 0
                self.swd.mww(MBOX + 0x04, MAGIC_ARM)         # ARM
                self.armed = True
            else:
                self.left = self.right = 0
                self.blade = False
                self.swd.mww(MBOX + 0x0C, 0)
                self.swd.mwh(MBOX + 0x10, 0)                 # blade off
                self.swd.mww(MBOX + 0x04, 0)                 # DISARM
                self.armed = False
            self._bump_seq_unlocked()

    def cmd_drive(self, left, right):
        left = max(-SPEED_MAX, min(SPEED_MAX, int(left)))
        right = max(-SPEED_MAX, min(SPEED_MAX, int(right)))
        packed = ((right & 0xFFFF) << 16) | (left & 0xFFFF)
        with self.lock:
            self._ensure_swd()
            if not self.armed:
                self.swd.mww(MBOX + 0x04, MAGIC_ARM)         # drive implies ARM
                self.armed = True
            self.left, self.right = left, right
            self.last_drive_ts = time.time()
            self.swd.mww(MBOX + 0x0C, packed)
            self._bump_seq_unlocked()

    def cmd_blade(self, on):
        with self.lock:
            self._ensure_swd()
            if on and not self.armed:
                raise ValueError("blade requires ARM first")
            self.blade = bool(on)
            self.swd.mwh(MBOX + 0x10, 1 if on else 0)
            self._bump_seq_unlocked()

    def cmd_stop(self):
        """Full stop: speeds 0, blade 0, DISARM."""
        with self.lock:
            self._ensure_swd()
            self.left = self.right = 0
            self.blade = False
            self.swd.mww(MBOX + 0x0C, 0)
            self.swd.mwh(MBOX + 0x10, 0)
            self.swd.mww(MBOX + 0x04, 0)
            self.armed = False
            self._bump_seq_unlocked()

    def snapshot_gpio(self):
        """HALT -> read every GPIO port (CTL0/CTL1/IDR/ODR) -> RESUME. Reliable
        peripheral snapshot for the motor-rail-enable hunt (diff ours vs factory).
        Peripheral regs read cleanly only while halted; we resume immediately."""
        ports = [("A", 0x40010800), ("B", 0x40010C00), ("C", 0x40011000),
                 ("D", 0x40011400), ("E", 0x40011800)]
        out = {}
        with self.lock:
            self._ensure_swd()
            self.swd.cmd("halt")
            try:
                for name, base in ports:
                    v = self.swd.mdw(base, 4)
                    if len(v) >= 4:
                        out["P" + name] = {"CTL0": "0x%08x" % v[0], "CTL1": "0x%08x" % v[1],
                                           "IDR": "0x%08x" % v[2], "ODR": "0x%08x" % v[3]}
                    else:
                        out["P" + name] = None
            finally:
                self.swd.cmd("resume")
        return out

    def snapshot_gpio_live(self):
        """Read GPIO regs WITHOUT halting (via debug AHB-AP while the CPU runs).
        Non-disruptive — safe to poll during a live motor drive. Returns same
        shape; may read stale/garbage if the AP can't access peripherals while
        running (compare against the halted /api/gpio to validate)."""
        ports = [("A", 0x40010800), ("B", 0x40010C00), ("C", 0x40011000),
                 ("D", 0x40011400), ("E", 0x40011800)]
        out = {}
        with self.lock:
            self._ensure_swd()
            for name, base in ports:
                v = self.swd.mdw(base, 4)
                out["P" + name] = {"CTL0": "0x%08x" % v[0], "CTL1": "0x%08x" % v[1],
                                   "IDR": "0x%08x" % v[2], "ODR": "0x%08x" % v[3]} if len(v) >= 4 else None
            # peripherals (non-halt): find a NON-GPIO difference factory vs ours
            def rd(addr, n):
                w = self.swd.mdw(addr, n)
                return ["0x%08x" % x for x in w] if len(w) >= n else None
            out["AFIO_PCF0"] = rd(0x40010004, 1)          # remap
            out["SPI1"]      = rd(0x40003800, 3)          # CTL0,CTL1,STAT
            out["TIMER2"]    = rd(0x40000400, 21)         # CTL0..CH3CV (0x00..0x50)
            out["RCU_CTL_AHB_APB2_APB1"] = [rd(0x40021000,1), rd(0x40021014,1),
                                            rd(0x40021018,1), rd(0x4002101C,1)]
        return out


MOWER = Mower()


# ---------- HTTP ----------
class Handler(BaseHTTPRequestHandler):
    protocol_version = "HTTP/1.1"

    def log_message(self, fmt, *args):  # quiet log: errors only
        if args and str(args[1] if len(args) > 1 else "").startswith(("4", "5")):
            BaseHTTPRequestHandler.log_message(self, fmt, *args)

    def _json(self, obj, code=200):
        body = json.dumps(obj).encode()
        self.send_response(code)
        self.send_header("Content-Type", "application/json")
        self.send_header("Content-Length", str(len(body)))
        self.send_header("Cache-Control", "no-store")
        self.end_headers()
        self.wfile.write(body)

    def _read_body(self):
        n = int(self.headers.get("Content-Length") or 0)
        if n <= 0:
            return {}
        try:
            return json.loads(self.rfile.read(n).decode())
        except Exception:
            return {}

    def do_GET(self):
        path = self.path.split("?", 1)[0]
        if path == "/api/telemetry":
            return self._json(MOWER.telemetry)
        if path in ("/api/gpio", "/api/gpio_live"):
            try:
                g = (MOWER.snapshot_gpio_live() if path.endswith("_live")
                     else MOWER.snapshot_gpio())
                return self._json({"ok": True, "gpio": g})
            except Exception as e:
                with MOWER.lock:
                    MOWER._drop_swd()
                return self._json({"ok": False, "error": "swd: %s" % e}, 503)
        if path in ("/", "/index.html"):
            return self._static("index.html", "text/html; charset=utf-8")
        return self._json({"ok": False, "error": "not found"}, 404)

    def do_POST(self):
        path = self.path.split("?", 1)[0]
        body = self._read_body()
        try:
            if path == "/api/drive":
                MOWER.cmd_drive(body.get("left", 0), body.get("right", 0))
            elif path == "/api/blade":
                MOWER.cmd_blade(bool(body.get("on", False)))
            elif path == "/api/arm":
                MOWER.cmd_arm(bool(body.get("on", False)))
            elif path == "/api/stop":
                MOWER.cmd_stop()
            else:
                return self._json({"ok": False, "error": "not found"}, 404)
            return self._json({"ok": True})
        except ValueError as e:
            return self._json({"ok": False, "error": str(e)}, 400)
        except Exception as e:
            # openocd connection dropped mid-command — reset and report
            with MOWER.lock:
                MOWER._drop_swd()
            return self._json({"ok": False, "error": "swd: %s" % e}, 503)

    def _static(self, name, ctype):
        fp = os.path.join(STATIC_DIR, name)
        try:
            with open(fp, "rb") as f:
                body = f.read()
        except OSError:
            return self._json({"ok": False, "error": "static file missing"}, 404)
        self.send_response(200)
        self.send_header("Content-Type", ctype)
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)


def main():
    srv = ThreadingHTTPServer((HTTP_HOST, HTTP_PORT), Handler)
    print("mower dashboard: http://%s:%d  (openocd %s:%d, mbox 0x%08x)"
          % (HTTP_HOST, HTTP_PORT, OPENOCD_HOST, OPENOCD_PORT, MBOX))
    try:
        srv.serve_forever()
    except KeyboardInterrupt:
        print("\nstop: disarm + wheels 0")
        try:
            MOWER.cmd_stop()
        except Exception:
            pass


if __name__ == "__main__":
    main()
