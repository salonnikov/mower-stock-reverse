# MI 302 factory firmware — module map + timelines (chip1 + chip2)

Thoroughly: what the factory firmware does — by modules and by event branches, down to the
precision of function / pin / bus. The main question: **what does the factory use to enable the 20U wheel power rail** (the blade on
the live rail spins with our fw too, the wheels are dead). Analysis is offline (decompilation + dump), no guessing.

Chip: **GD32F30x_CL** (F1 peripherals). GPIOA=0x40010800, RCU=0x40021000 (APB1EN=+0x1C/APB2EN=+0x18/AHBEN=+0x14),
GPIO F1-style (BOP=+0x10 set, BC=+0x14 clear, OCTL=+0x0c). Timers: TIMER2=0x40000400 (motor PWM),
TIMER3=0x40000800 (tacho). SPI1=0x40003800 (A4963). USART2=0x40004800 (BMS). Our `gd32_regs.h` — these same bases `[VERIFIED]`.

## Analysis status — DONE (2026-07-08, Fable 5 agents)
- [x] 01 boot  - [x] 02 pin  - [x] 03 fsm  - [x] 04 motors  - [x] 05 bms  - [x] 06 chip2  - [x] synthesis (this file)

## Map files
| File | Content | Key conclusion |
|---|---|---|
| `01-boot-poweron.md` | reset→main→RCU→FreeRTOS→rw_init. Latches, driver order. | Power = PE12+PE7+PB0 (we have all of them). There is NO wheel rail-enable on the boot path. |
| `02-pin-security.md` | button, latches, cJSON display protocol, PIN check, FSM unlock. | The branch does not touch power GPIO/A4963/20U. |
| `03-fsm-mow-home.md` | FSM 3→4→5, "home", movebase (wheel motion). | Driving = PWM+direction(SPI)+run(PD7/PD4); there is NO separate "enable rail" in software. ★ raised the SPI/A4963 lead. |
| `04-motor-drivers.md` | blade/left/right + A4963, SPI configs, TIMER2/3. | There is no firmware POWER asymmetry blade-vs-wheels; only PD7/PD4 (tested) + TIMER3-tacho. |
| `05-bms-pack.md` | service_bms + USART2-pack, the whole command table. | There is NO special discharge-enable command for the wheels. Gap: keep-alive CE+C3+53 (watchdog?). |
| `06-chip2.md` | second MCU = border sensors + lift, UART/cJSON to chip1. | No direct relation to motor power (indirectly: lift-stop). |

---

## UNIFIED TIMELINE (by branches)

### A. Power-on (01)
`reset` → bootloader `boot_sys_init FUN_08000f38`: clocks GPIOA-E+AFIO → **PE12=HIGH (main latch)** →
**PE7=HIGH (aux)** → comms pins (USART0/UART3/SPI0, UART-DE LOW), PE0/1/5=OUT-OD LOW → `FUN_08001b4c`
on power-on-reset → **PB0=HIGH (secondary latch)** → teardown `FUN_08000e48` (kills IRQ/TIMER0/DMA/SPI0,
does not touch the latches) → **app** `main FUN_08061714` → `FUN_0807f010` (holds the latch ports' clocks + flash self-check)
→ FreeRTOS `FUN_0806bbd8` (task "initial" @0x0805bc28) → `rw_init FUN_080477c4` creates the drivers
**tilt→battery→BLADE→RIGHT→LEFT** (vtable) → FSM **state 3 (idle)**, `fsm_set FUN_0807976c` writes @0x200000bc.

### B. PIN code / unlock (02)
Button `FUN_0800d034` (input bit10, debounce) → `FUN_0800cf50` logs `key_press_power_on`, flag |0x40.
The display (ESP32) sends over UART a cJSON frame `0x26 | payload | CRC8(poly 0x8C) | 0x23` (opt. XOR 0x5b):
`dp_receive FUN_0804721c`→de-XOR `FUN_0803953c`→validation `FUN_0806e938`→cJSON `FUN_080470f0`
(dispatch by a 16-entry table, key `cmd&0xFF000000`). Class `0x30…`→userset `FUN_08044c80` (switch):
**case 09 = PIN check** (json vs `DAT_08060d38`; ok→`result=true`+reset counter; no→increment NVM
"run_param"+0xb8, >9→lock+event 0x20; reply `FUN_08072e08`). Unlock = request/response;
**state 3 sets up the process layer** `FUN_08069848`/`FUN_08049d14(3)`. No power writes.

### C. "Mow" (03) — state 3→4→5
`idle FUN_080270a8` (process_wait): safety `FUN_0804f758` + schedule gate `FUN_0805deec` → `fsm(4)`
(on the transition — only a state write, NO GPIO/TIMER/SPI). **departure** → `FUN_08077e10(1)` (movebase):
speed `FUN_0804f2c8(0x3c,0x87)` → vtable to the wheels `(600, dir, speed)` → vtable "enable" @+0xa8. The hardware behind the
vtable: **TIMER2 CH2CV(right)/CH3CV(left)=PWM**, **direction=SPI-write A4963** (`FUN_0801f4c6→FUN_0808408c`,
`word|0x1000`), **run/coast=PD7(left)/PD4(right)**. Departure distance passed → `fsm(5)` cutting → blade
`FUN_0802bce4`. **There is NO separate "enable the 20U rail" step.**

### D. "Home" / dock (03) — the same movebase path; charge-init (0x08085xxx) is outside the decompile.

### E. Motors (04)
3× A4963 on the common SPI1, vtable objects. Config: `FUN_0808408c` write+readback (reg-addr=bits15:13, WR=0x1000
in the transfer `FUN_080840d8`). Direction for all — `FUN_0801f4c6` (bit 0x2 reg7). A4963 left reg0-7 normal
`03E8,22DF,4753,6721,8735,A736,C000,EE0D`; slow `01A8,2ADF,47D3,6721,8745,A736,C000,EE0D`. reg7=`EE0D`=RUN.
Wheel-only: PD7/PD4 run/coast + TIMER3-tacho. The blade needs no discrete enable.

### F. BMS / pack (05)
USART2 19200 8N1 half-duplex PD8/PD9. Frame `1C A1 <LEN> <op> <args> <CRC8/MAXIM>`, response `3A A3`:
`C1`=connect/telemetry (×4), `C3`=poll the pack, `0x53`=poll the cells, `CE`(55×7)=link-wake/resync,
`B0..B4`=charge mode/state (into/exit charge, dispatcher `FUN_08072948`, sends only when enable==1 @+0xa5).
Factory cycle: init→CE→connect→initial-poll→keep-alive C3+53→charge per the dock. **There is NO motor-FET discharge-enable command.**

### G. chip2 (06)
GD32F4xx of the BORDER sensor board: ADC+DMA of the perimeter (`bdsensor_init`), wave detection, lift sensors.
The link with chip1 = UART/cJSON (chip1 master, tag bdport). Not directly involved in motor power.

---

## ★★ CONCLUSION: what enables the 20U wheel rail — ranked plan

On the entire factory software path (boot / pin / mow / home / motors / bms) there is **NO separate action "enable
the wheel power rail"** beyond what we already reproduce (PWM + direction-SPI + run PD7/PD4 +
latches PE12/PE7/PB0). The analysis produced **two concrete firmware tests** before any hardware:

### 1. ★★ Fix SPI1 → get the A4963 config to actually land on the WHEEL chips
Our `verify=0xFFFF` even on the live blade ⇒ **SPI1 read does not work, probably the write is not applied either.**
Hypothesis: the blade spins on the A4963 POR default, while for the wheel chips the default = coast/standby, they MUST have
the SPI config landed (reg0..7 incl. `EE0D` + direction `FUN_0801f4c6`). Test: fix SPI1 read/write,
check `verify != 0xFFFF` on the wheel A4963, make sure reg0-7 got written → spin the wheels.
(Partially revises the earlier "A4963 auto-run, SPI not needed" — true for the blade, NOT proven for the wheels.)

### 2. ★ BMS keep-alive (pack link watchdog)
We sent the pack only `C1`; the factory constantly sends `CE`-wake + keep-alive `C3`+`0x53`. If the pack drops
the power FET on a link watchdog — a robust periodicity of these frames is needed (not a special command). Test: implement
`CE→connect→periodic C3+53`, measure the wheel VBB. If it does not come up — the hypothesis "the pack gates the wheels" is closed.

### 3. Hardware high-side switch of the 20U rail (Q10/Q17/Q14)
The default outcome if (1) and (2) miss. Ring out the gate factory-driving vs ours. By user decision — low priority.

### Method caveats
Parts outside the Ghidra decompile (blade/right motor init 0805a858/0805b820, the security service,
charge-init 0x08085xxx, the RX task) were extracted indirectly/by dump disasm — the connections are traced, but not byte-for-byte.
