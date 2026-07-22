# 05 — BMS / smart pack over USART2 (chip1, factory fw)

Full analysis of the factory USART2 ↔ battery pack protocol. Sources: `chip1/decompiled_all.c`,
`strings.txt`, `symbols.txt`, dump `dist/gd32-mainboard-dump-v1.bin`. All frames are extracted from
the unpacked `.data` (SL-Type2 decompressor `FUN_080197b4`, descriptor src=0x0808DA24 → dst=0x20000070,
size=0x6D0) and **verified against CRC-8/MAXIM live in this session** (see §2). The connect frame matched a previously
confirmed SWD read of `0x200005E0` — the unpacking is correct.

> ★ MAIN CONCLUSION (see §5): a separate command "enable discharge / feed power to the motor tap"
> does **NOT** exist in the protocol. All commands to the pack are telemetry, link-wake, and the **charge** mode.
> Every string containing the word "discharger" is a decision by the ROBOT (safety), not a command to the pack.

---

## 1. FUNCTION MAP

### Transport / low level (driver_battery_snk_v*.c)
| Function | Role |
|---|---|
| `FUN_080213c0` / `FUN_08021468` | init USART2 (baud/CTL, RCU, GPIO, NVIC IRQ39) |
| `FUN_080211f0(dir)` / `FUN_080212d8(dir)` | half-duplex switch of PD8: 1=TX, 0=RX (v1/v2) |
| `FUN_0807e00c` / `FUN_0807e050` | **TX connect frame** (6 bytes, template BASE+4) [v1/v2] |
| `FUN_08074da0` / `FUN_08074de4` | **TX "CE-wake" frame** (12 bytes, template BASE+0x36) [v1/v2] |
| `FUN_08058de0` / `FUN_0805929c` | **telemetry poll loop**: sends C3 (pack msg, +0x0A) and 0x53 (cell msg, +0x2E), parses |
| `FUN_0802ab98` | **CRC-8/MAXIM** (poly refl 0x8C, init 0). Table @dump 0x88B3C. Covers LEN-1 bytes (opcode+args) |
| `FUN_0802440c` / `FUN_08024788` | **response parser** for C1 (marker 0xC1), recompute V/I/T, status |
| `FUN_08020e54` / `FUN_08021020` | **connect**: 4 attempts to TX C1, on timeout → CE-wake, parse, compare V vs OCV |
| `FUN_08083178(base,bit)` | poll a STAT bit (7=TBE, 6=TC, 5=RBNE, 4=IDLE) |
| `FUN_08082e3c(base,byte)` | write a byte into DATA (base+0x04) |
| driver-init (v1 @≈0x080599xx, calls connect@69558→poll@69560) | malloc ctx (magic 0x55AA@ctx+0x11w), RCU/GPIO/USART/NVIC, connect, initial poll |

### Service (service_bms.c)
| Function | Role |
|---|---|
| `FUN_08056…` ("create bms task failed" @0x08056c9c) | enable handler: validates enable(0/1)→ctx+0xa5, creates bms-task (prio 0x200), calls vtable +0x1c/+0x20 |
| `FUN_08072948` | **bms-task tick / command dispatcher**: by the bits of ctx+0xa6 calls vtable +0x30..+0x40 (into/exit charge, state) |
| "set bms enable charge current=%d" @0x080492c4, "set disenable…success" @0x08049323 | sets the **charging** current (charge, not discharge); body — outside the decompiled subset |
| strings @0x08072a37 `send cmd into charge`, @0x08072a84 `send cmd exit charge` | dispatcher log |

**ctx structure (BASE = 0x200005DC, v1):** [0]=ctx-ptr(malloc), +0x54=connect status(0/0xFF),
+0x55=RX-done flag(0xFF=waiting), +0x56=TX-len, +0x57=TX-idx, +0x58=TX pointer to the template,
+0x5C=retry-cnt, +0x5D…=RX buffer (0x40 bytes), +0xA5=task-enable(0/1), +0xA6=command bitmask.

---

## 2. COMMAND TABLE (host → pack) — all templates from .data, CRCs verified

Request header **`1C A1`**; the pack's response — **`3A A3`**. Format: `1C A1 <LEN> <opcode> <args…> <CRC8>`,
where **LEN = number of bytes after LEN** (opcode+args+crc), **CRC8 = CRC-8/MAXIM over opcode+args**.

| Address (RAM) | Off | Frame (hex) | Opcode | Purpose | Who sends / when |
|---|---|---|---|---|---|
| 0x200005E0 | +0x04 | `1C A1 03 C1 01 2E` | **C1** | connect / read telemetry (V,I,T) | `FUN_0807e00c` — connect ×4 at init |
| 0x200005E6 | +0x0A | `1C A1 03 C3 01 BF` | **C3** | "pack message" (type/capacity/status) | `FUN_08058de0` — poll loop |
| 0x200005EC | +0x10 | `1C A1 03 B0 11 C1` | **B0** (arg 0x11) | mode command (charge/state) | vtable method (not decompiled) |
| 0x200005F2 | +0x16 | `1C A1 03 B1 00 C6` | **B1** (arg 0x00) | mode command | vtable method |
| 0x200005F8 | +0x1C | `1C A1 03 B2 00 93` | **B2** (arg 0x00) | mode command | vtable method |
| 0x200005FE | +0x22 | `1C A1 03 B3 55 B3` | **B3** (arg 0x55) | mode command | vtable method |
| 0x20000604 | +0x28 | `1C A1 03 B4 0F 78` | **B4** (arg 0x0F) | mode command | vtable method |
| 0x2000060A | +0x2E | `1C A1 05 53 00 02 1A 22` | **0x53 'S'** | "cell message" (bank voltages) | `FUN_0805929c`/`FUN_08058de0` — poll loop |
| 0x20000612 | +0x36 | `1C A1 09 CE 55 55 55 55 55 55 55 6E` | **CE** | **link-wake / resync** (7×0x55) | `FUN_08074da0` — init, connect-retry, CRC-mismatch, poll-timeout |

All 9 CRCs matched (CRC-8/MAXIM over opcode+args). Dispatcher `FUN_08072948` (vtable @ (svc+4)+0x20):
- bit0 of ctx+0xa6 → method **+0x30 = "into charge"**
- bit1 → method **+0x34 = "exit charge"**
- bit2 → method **+0x38** (state), bit3 → **+0x3C** (state), bit4 → **+0x40** (state, param=0x10)

The five vtable methods almost certainly correspond to sending B0…B4 (the exact opcode↔method mapping is not proven —
the method bodies are outside the decompiled subset; but this is the only set of 5 "command" templates in the table).

### pack → host response (parsers FUN_0802440c / FUN_08024788)
Validity: `buf[+0x5D]==0x3A && buf[+0x5E]==0xA3 && buf[+0x60]==0xC1(=-0x3F) && buf[+0x67]==CRC`.
Pulls: `+0x62`→status byte (@ctx+0x42), `+0x63/64`→u16 (@+0x48, current), `+0x65/66`→u16 (@+0x46, voltage).
Compare: V<0x9C5, I<0x1068 → status ctx+0x4C (0=ok/1/2). **There is NO "discharge on / FET open" flag in the parser.**
Live factory response (SWD): `1C 3A A3 08 C1 01 19 0E` — C1-echo + telemetry.

---

## 3. BMS TIMELINE

```
POWER-ON / platform-init
        │
        ▼
driver_battery init (FUN_080599xx)
   ├─ malloc ctx (0xA8), magic 0x55AA
   ├─ RCU(USART2/GPIOD/AFIO), GPIO PD8/PD9 remap, USART2 19200 8N1, NVIC IRQ39
   ├─ FUN_08074da0()               ← CE-wake (1C A1 09 CE 55×7 6E)   ["wake up" the link]
   ├─ FUN_08020e54()  connect ×4:
   │     TX C1 → wait RX 25×2ms → on timeout CE-wake+retry → parse, compare V/OCV
   │     success → ctx+0x54=0 ; failure → 0xFF + "battery connect failed"
   └─ FUN_08058de0()  initial poll: TX C3 (pack msg) → TX 0x53 (cell msg) → parse type/capacity
        │
        ▼
bms-service enable (FUN_08056…, enable=1 → ctx+0xa5=1) → the bms-task is created (prio 0x200)
        │
        ▼
bms-task loop (FUN_08072948 + poll):
   ├─ periodically poll telemetry (C3 + 0x53), update V/I/T/banks  ← keep-alive traffic
   ├─ on frame loss/CRC-mismatch → CE-wake + retry
   └─ by the bits of ctx+0xa6 (sets the charge/departure FSM):
         into charge  (dock) → method +0x30 (sends B-frame)
         exit charge  (leaving) → method +0x34 (sends B-frame)
         state cmds          → methods +0x38/+0x3C/+0x40

DOCK/CHARGE (deal_charge / check_recharge):
   is_reach_charger_station → "send cmd into charge" → the pack accepts charging current
   "set bms enable charge current=%d" → set the charge current
DEPARTURE/MOW:
   "send cmd exit charge" → exit charge mode
   the decision to DRIVE/MOW — on the ROBOT side (deal_safety), no discharge command is sent to the pack
```

---

## 4. KEY CODE (annotated)

### connect TX primitive (FUN_0807e00c)
```c
FUN_080211f0(1);                       // PD8 → TX (half-duplex)
iVar3 = *DAT_0807e048;                 // ctx = *BASE
*(u8*)(iVar3+0x56)=6;                   // window LEN = 6 bytes
*(u8*)(iVar3+0x57)=0;                   // idx
*(int**)(iVar3+0x58)=piVar1+1;          // template = BASE+4 = 1C A1 03 C1 01 2E
do {} while(!FUN_08083178(usart,7));    // wait for TBE
FUN_08082e3c(usart, first_byte);        // 1st byte; the rest — in the IRQ on TBE
```

### CE-wake / resync (FUN_08074da0) — template BASE+0x36
```c
FUN_080211f0(1);
*(u8*)(ctx+0x56)=0xc;                    // window LEN = 12
*(int*)(ctx+0x58)=(int)BASE+0x36;        // 1C A1 09 CE 55 55 55 55 55 55 55 6E
... TBE, send ...
```
Called at init, connect-retry (`FUN_08020e54` LAB_08020eb8), CRC-mismatch (`FUN_0802440c` @34371),
poll-timeout (`FUN_08058de0` @68770/68830). ⇒ this is a **link-level "wake/resync"**, not a power command.

### CRC-8/MAXIM (FUN_0802ab98)
```c
uint crc=0;
for (i=0; i<len; i++) crc = table[(byte)(buf[i]^crc)];   // table @ dump 0x88B3C
```

### bms-task command dispatcher (FUN_08072948)
```c
iVar6=*(svc+4);
if (*(char*)(iVar6+0xa5)==1) {           // task enabled
  m=*(byte*)(iVar6+0xa6);                 // request bitmask
  if (m&1)  { (*vtbl[+0x30])(); log("send cmd into charge"); }  // → B-frame
  if (m&2)  { (*vtbl[+0x34])(); log("send cmd exit charge"); }  // → B-frame
  if (m&4)  { (*vtbl[+0x38])(); m&=~4; }                        // state
  if (m&8)  { (*vtbl[+0x3c])(); m&=~8; }                        // state
  if (m&16) { (*vtbl[+0x40])(); *param=0x10; m&=~16; }          // state
}
```
The +0xa6 bits are set by the upper level (charge/dock FSM). There is NO "enable discharge/motor" branch.

---

## 5. ★ IS THERE A COMMAND TO ENABLE POWER DISCHARGE (wheel motor tap)? — **NO**

**Direct answer: in the factory USART2↔pack protocol there is NO command "enable discharge / feed power to
the motor tap".** Evidence:

1. **The full command table is decoded and CRC-valid** (§2): C1, C3, 0x53, CE, B0–B4. This is the exhaustive
   set of request templates in `.data` (adjacent addresses 0x200005E0…0x20000612, no garbage between them).
   Semantics: telemetry (C1/C3/0x53), link-wake (CE), **charge** mode/state (into/exit charge + 3 state).
2. **The response parser contains no discharge-FET flag** — only V/I/T/status (§4). The pack does not report
   "discharge on/off", and the host does not request it.
3. **All "discharger" strings are decisions by the ROBOT, not commands to the pack**:
   `battery singal protect, can not discharger`, `battery health, can not discharger`,
   `cutting but bat health can not discharger` → these are `deal_safety`/FSM returning "cannot drive"
   (the robot itself withholds drive), not a frame on USART2. `enable auto reconnect battery discharge temp` /
   `recover discharge temp` — thermal auto-reconnect logic (by temperature), also host-side.
4. **"set bms enable charge current" = CHARGING current** (to accept charge from the base), not discharge.
5. **Live test (2026-07-06/07)**: the full C1-handshake was established (connected=1, the pack responds with
   telemetry), but the wheel VBB did NOT come up. ⇒ polling C1 does not enable discharge.

### However — what the factory sends that our mower-own does NOT (the only unclosed gap)
Live test v2 sent **only C1** (connect + re-poll). The factory additionally and continuously sends:
- **CE-wake** `1C A1 09 CE 55×7 6E` — at init and on every link error;
- **C3** (pack msg) and **0x53** (cell msg) — cyclic keep-alive polling from bms-task.

If the pack holds the discharge FET on a **link watchdog timer** and drops it when the host "goes silent",
then the missing link is not a special command but a **steady keep-alive with exactly these frames** (CE + C3 + 0x53),
which the C1-only test did not provide. This is the only "the pack gates the wheels" hypothesis not closed by hardware-bot.

### Recommendation for mower-own
Implement the full sequence like the factory and measure the wheel VBB:
1. `CE-wake` (12 bytes) → 2. `connect C1` ×4 (on timeout retry CE) → 3. periodic keep-alive
`C3` + `0x53` (plus `CE` on CRC-fail) with a short period. All bytes and CRCs are in §2 (ready to hardcode).
If VBB does not come up even with full keep-alive — the "the pack gates the wheels" hypothesis is closed definitively,
and only the hardware gate of the 20U key (Q10/Q17/Q14) remains.

---
_Compiled by the reverse-engineering agent 2026-07-08. Frames unpacked from .data and verified against CRC-8/MAXIM in a container._
