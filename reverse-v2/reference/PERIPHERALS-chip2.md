# CHIP2 (GD32F30x, border coil board) — peripheral and IO map

> Chip: **chip2** = the border coil/sensor board (border/coil sensor board). GD32F30x.
> Dump: `dist/gd32-mainboard-chip2-dump-v1.bin`, base `0x08000000`.
> Decompilation: `reverse-v2/chip2/decompiled_all.c`, `symbols.txt`, `memory_map.txt`, `functions_index.csv`.
> Trust marks: **[V]**=verified (offset/code line/literal in the dump), **[I]**=inferred, **[U]**=unknown.
>
> ⚠️ EVERYTHING below concerns **chip2**. Do not confuse with chip1 (the main GD32F305). chip2 has its OWN USART2.

## 0. Which peripherals are ACTUALLY used (method: searching for base-address literals in the dump)

Byte search for little-endian base addresses in `gd32-mainboard-chip2-dump-v1.bin`:

| Peripheral | Base | Literals in dump | Conclusion |
|---|---|---|---|
| ADC0 | 0x40012400 | 4 | **[V] used** (coils + self-test) |
| ADC1 | 0x40012800 | 1 | **[V] used** (dual-ADC coils) |
| ADC2 | 0x40013c00 | 1 | **[V]** IEC60730 self-test only (not coils) |
| DAC | 0x40007400 | 0 | **[V] NOT used** (coil excitation NOT via DAC) |
| SPI0/1/2 | — | 0 / 0 / 0 | **[V] NOT used** |
| I2C0/I2C1 | — | 0 / 0 | **[V] NOT used** |
| TIMER1 | 0x40000000 | (literal `0x40000000` in bdsensor_init code) | **[V] coil excitation** |
| TIMER0 | 0x40012c00 | 2 | **[I]** used (timebase, not bound) |
| TIMER2 | 0x40000400 | 1 | **[I]** used |
| USART2 | 0x40004800 | 5 | **[V] link to chip1** |
| DMA0 | 0x40020000 | (literal in FUN_08019d34) | **[V]** ADC + USART2 |
| GPIOA | 0x40010800 | 5 | **[V]** (PA5/PA7 coil analog) |
| GPIOB | 0x40010c00 | 8 | **[V]** (PB1 analog, PB10/PB11 USART2) |
| GPIOC/D/E | — | 0 | **[V] NOT used** (important: the link is NOT remapped) |

**Bottom line: the truly live peripherals of chip2 = ADC0+ADC1 (coils), TIMER1 (excitation), USART2+DMA0 (link to chip1), GPIOA+GPIOB. That's all. No SPI, no I2C, no DAC, no GPIOC/D/E.**

---

## 1. Coil ADC path — dual/sync ADC0 + ADC1

Initialization: **`FUN_08019bf4` @ 0x08019bf4** (dual-ADC init) and **`FUN_08019d34` @ 0x08019d34** (DMA), called from **`FUN_08012b6c` @ 0x08012b6c = `bdsensor_init`** (file `..\src\platform\driver\driver_board_sensor\driver_bdsensor.c`, string `driver_bdsensor.c` @0x08012ca0).

Base literals confirmed by reading the dump:
- `DAT_08019cf0 = 0x40012400` = **ADC0** [V]
- `DAT_08019cf4 = 0x40012800` = **ADC1** [V]
- `DAT_08019da0 = 0x40020000` = **DMA0** [V]
- `DAT_08019da4 = 0x4001244c` = **ADC0_RDATA** (ADC0+0x4C, regular data register) [V]

### Channels (decoded from `FUN_08012a04` = adc_regular_channel_config(base, rank, channel, sample_time)):
- `FUN_08012a04(ADC0, 0, 5, 7)` → **ADC0, rank0, channel 5, sample-time index 7** [V] (decompiled_all.c:14341)
- `FUN_08012a04(ADC1, 0, 9, 7)` → **ADC1, rank0, channel 9, sample-time index 7** [V] (decompiled_all.c:14342)

Mechanics of `FUN_08012a04` (dc:6165): writes the 5-bit channel field into RSQ2/RSQ1/RSQ0 (off 0x34/0x30/0x2c) and the 3-bit sample-time field into SAMPT1/SAMPT0 (off 0x10/0xc). Verified — standard GD32 semantics. **Sample-time index 7 = 239.5 ADC cycles** (the maximum) [V] — i.e. the maximally "slow"/averaged sampling (a coil = high-impedance source).

### Analog pins (from `bdsensor_init`, GPIO in analog mode, `FUN_08016ef0(port,0,4,mask)`):
- `FUN_08016ef0(GPIOA, 0,4, 0x20)` → **PA5 = ADC0_CH5** [V] (mask 0x20 = bit5)
- `FUN_08016ef0(GPIOB, 0,4, 0x02)` → **PB1 = ADC1_CH9** [V] (mask 0x02 = bit1; GD32: ch9=PB1)
- `FUN_08016ef0(GPIOA, 0,4, 0x80)` → **PA7** also in analog mode [I] (mask 0x80 = bit7 = ADC_CH7; NOT configured in the regular sequence — reserve/injection/monitoring, purpose [U])

### Sync/dual mode and DMA:
- `FUN_080129ec(0x20000)` in init — sets the ADC sync mode (dual regular parallel). In GD32 dual-sync the result of ADC0 (low 16 bits) + ADC1 (high 16 bits) is placed as a single word into **ADC0_RDATA (0x4001244c)** [V/I].
- `*(ADC0+8) |= 0x100` (dc:14353) — enabling the DMA bit in ADC0_CTL1 [V].
- **`FUN_08019d34`**: `dma_deinit(DMA0, ch0)` → `dma_init(DMA0, ch0, cfg)` where cfg.periph_addr = `0x4001244c` (ADC0_RDATA), circular, mem-increment, 16-bit, high-priority; `dma_channel_enable(DMA0,0)` + NVIC (`FUN_08017bc4(0xB,5,0)` → IRQ 0x0B = **DMA0_Channel0_IRQn**) [V]. **DMA0 Channel 0** reads ADC0_RDATA in a loop [V].

### Sample buffer and baseline:
- The waveform buffer is allocated dynamically in **`FUN_08019e34` @ 0x08019e34**: the length field (`+0x92`) = **800** samples, mallocs 3 buffers: `+0x48` (800×2 B), `+0x68` (800×2 B), `+0x70` (800×4 B) [V]. Matches the strings "left wave data", "disturb wave", "enqueue wave to disturb wave".
- The current single coil sample: `FUN_08016a90` returns `*(0x2000004c)` (16-bit) [V].

### 🔧 Baseline / normalization (IMPORTANT for recalibration to new coils):
In `bdsensor_init` (dc:6349-6362) at startup an **automatic baseline measurement** is performed: the coil ADC is read 5 times, the average `sum/5` is taken and written to `*_DAT_08012c88 = 0x20000094` (16-bit **base voltage**) [V].
- If the measurement fails → default **2048** (0x800, 12-bit midscale) — string "base voltage =%d error, set default 2048" @0x080174ef [V].
- The coil signal is then interpreted relative to this baseline (DC-offset subtraction).
- **There is NO explicit digital gain multiplier in the path.** The "gain" is set by: (a) the TIMER1 excitation parameters (see §4), (b) the external analog front-end (outside the MCU). The digital normalization = only subtraction of `base_voltage` (0x20000094).

### Left/right (which channel is which side):
- **[U]** — the code does NOT mark ADC0-ch5 / ADC1-ch9 as "left"/"right" directly. Only this is known: PA5=ADC0_CH5 and PB1=ADC1_CH9 — the two sides of the coil. The left/right split happens in software from the dual-word (low half = ADC0, high half = ADC1), but the correspondence of a half to a specific side is **not proven** ([I]: ADC0/low = one side, ADC1/high = the other).

---

## 2. USART2 (0x40004800) — link to chip1

Init: **`FUN_080004d4` @ 0x080004d4**, called from `FUN_080005e0`.

- **Baud 115200** [V]: `FUN_08001d2c(&USART2_STAT0, 0x1c200)` — 0x1c200 = 115200 (dc:497).
- Frame 8 bits, config via `FUN_08001e4c(&USART2,0x304)` and a family of setters (dc:498-504) — 8 data bits, standard (8N1 [I]).
- **NVIC**: `FUN_08001a68(0x27,3,0)` → IRQ **0x27 = 39 = USART2_IRQn** (GD32F30x), priority 3 [V].
- **Pins (GPIOB, no remap — GPIOC/D absent from the dump)**:
  - `FUN_080014d8(&GPIOB_CTL0, 0x18, 4, 0x400)` → **PB10 = USART2_TX** (mask 0x400 = bit10, AF push-pull) [V]
  - `FUN_080014d8(&GPIOB_CTL0, 4, 4, 0x800)` → **PB11 = USART2_RX** (mask 0x800 = bit11) [V]
  - (The default USART2 GD32 layout = PB10/PB11, remap not applied [V])
- **DMA (DMA0)**:
  - **TX = DMA0 Channel 1** [V/I]: in send `FUN_08019a18` (send_string) → `dma...(_DAT_08019b3c, 1, len)` (dc:14295-14296). Matches GD32 USART2_TX→DMA0_CH1.
  - **RX = DMA0 Channel 2** [V/I]: `FUN_080008d0(&DMA0_INTF, 2, &DAT_20000078)` + count `0x100`=256 (dc:474-475, 603-604). RX buffer **0x20000078**, size 256 B. Matches GD32 USART2_RX→DMA0_CH2.

### Protocol frame (chip2↔chip1) — what is confirmed:
- **Header = 0xA5** [V]: `if (DAT_20000078 == -0x5b ...)` (dc:467), -0x5b as a byte = 0xA5. The first byte of the RX buffer.
- **Command byte** = RX[1], range 1..0x15, dispatch via a jump table (dc:467-470) [V].
- **Payload XOR 0x5B** [V]: `*(param_1+uVar2) ^= 0x5b` (dc:7630, 7674) — scrambling/descrambling of the frame body with the key 0x5B.
- **End marker '#' = 0x23** [V]: `FUN_0801b6d8(&local_c, 0x23)` (dc:7400).
- Frame checksum: **[I]** present (the task mentions CRC8) — the exact algorithm is in software, no explicit CRC8 polynomial isolated in the decompilation; the hardware **CRC (0x40023000)** block is used NOT for the frame but for the **flash CRC32 self-test** (IEC60730, dc:638-666, `CRC_DATA`/`CRC_CTL`). Do not confuse.
- Port driver: `..\src\platform\driver\driver_port\driver_mboard_port_snk_v2.c` (string @0x08019adc), log tag "mport drv".

---

## 3. GPIO — lift limit switches + other

### Lift limit switches (2 of them):
Handler: **`FUN_08013970` @ 0x08013970** (`deal_lift_sensor`, file `process_comm.c`) [V].
- Reads **2 digital inputs** via the lift driver's method pointers: `read_ch1()` (struct+8→+4) and `read_ch2()` (struct+8→+8) (dc:7509-7510) [V].
- State logic: **both lifted / left lifted / right lifted / lift none** (strings @0x08013abc/0x08013a7f/0x08013ac8/0x08013ad8) [V].
- **Debounce**: a counter with a threshold of **10** consecutive ticks before a state change (dc:7515-7566) [V].
- **Limit-switch pins: [U]** — read through the lift-driver abstraction (function pointers), the specific GPIOs are set in its init (not localized exactly). The port — GPIOA or GPIOB (no other ports in the dump) [I].
- Init strings: "init lift driver failed" @0x080172a8.

### Other GPIO (confirmed):
| Pin | Purpose | Mark |
|---|---|---|
| PA5 | ADC0_CH5 (coil, analog) | [V] |
| PA7 | analog (ADC_CH7, purpose unclear) | [I]/[U] |
| PB1 | ADC1_CH9 (coil, analog) | [V] |
| PB10 | USART2_TX (link to chip1) | [V] |
| PB11 | USART2_RX (link to chip1) | [V] |

GPIO helpers: `FUN_080014d8` (config CTL0/CTL1 by nibbles), `FUN_08016ef0` (analog/mode by mask).

---

## 4. TIMER — coil excitation (TIMER1 CH1 PWM)

From `bdsensor_init` (dc:6335-6345), base = **`0x40000000` = TIMER1** [V]:
- `FUN_0801af7c(0x40000000, &cfg)` = timer_init; cfg: **prescaler = 0x0B (11)**, **period/CAR = 199**, everything else 0 (dc:6335-6339) [V].
- `FUN_0801ac58(0x40000000, 1, {mode=1})` = timer_channel_output config, **channel 1 (CH1)** [V].
- `FUN_0801ae7a(0x40000000, 1, 100)` = **pulse/CH1CV = 100** (≈50% of 199) [V].
- `FUN_0801ae2c(0x40000000, 1, 0x70)` = output mode = **PWM** (0x70 = TIMER_OC PWM) [V].
- `FUN_0801ae98(0x40000000, 1, 0)` = additional config (dc:6345).

**Bottom line: coil excitation = TIMER1 CH1, PWM ~50% (pulse 100 / period 199), prescaler 12 (0x0B+1).**
- Frequency [I]: at a timer clock of 120 MHz → 120e6/12/200 ≈ **50 kHz** coil excitation (plausible for an inductive border sensor). The exact clock APB1×2 not re-verified → [I].

### 🔧 For recalibration to new coils, tweak here:
- **prescaler (0x0B) and period (199)** → coil excitation frequency (dc:6335-6336).
- **pulse (100)** → excitation duty/energy (dc:6343).
Plus the baseline from §1 (0x20000094) — the digital offset.

### TIMER0 / TIMER2:
- **[I]** used (base literals in the dump: TIMER0 ×2, TIMER2 ×1), purpose not localized — probably timebase/scheduler tick or sampling synchronization. Binding to the coil **[U]**.

---

## 5. DMA and other peripherals

### DMA0 (0x40020000) — the only DMA controller used [V]:
| Channel | Purpose | Evidence |
|---|---|---|
| CH0 | ADC0_RDATA → coil waveform buffer (circular) | [V] FUN_08019d34 |
| CH1 | USART2 TX | [V/I] FUN_08019a18 |
| CH2 | USART2 RX (buffer 0x20000078, 256 B) | [V] FUN_080004d4/FUN_08000694 |

DMA1 (0x40020400): the code only has checks `param==&DMA1_INTF` in generic helpers (dc:711…827) — **DMA1 itself is not actively configured** [I].

### CRC (0x40023000):
- The hardware CRC32 — **only the IEC60730 flash self-test** (dc:638-666), "FLASH CRC32 Test" strings @0x0801a768. NOT for the USART2 frame [V].

### Other:
- **FMC** (0x40022000, 357 accesses) — flash operations/self-test [V].
- **PMU / RCU / NVIC** — system-level (clocking, power, interrupts).
- **IEC60730 safety self-test** active: CPU/FLASH-CRC/Clock/PC/RAM/ADC tests (strings @0x0801a52c…, `adc test(PreRun)` @0x0801a910 uses **ADC2**) [V].
- Logging: EasyLogger V2.2.99 (`elog.c`), tags "hr_bd", "mport drv", "init_bd".
- OS: FreeRTOS (`tasks.c`/`queue.c`/`heap_4.c`/`port.c`).

### Clocking (RCU clock-enable, `FUN_08019148`/`FUN_08001bf8` with enum):
In `bdsensor_init` the clocks are enabled with enums 0x602,0x603,0x609,0x60a,0x500,0x700; in system-init — 0x300,0x400,0x500,0x600,0x602-0x606,0x700,0x712. The exact enum→peripheral decoding was not done, but it is consistent with the literal evidence: GPIOA/GPIOB, DMA0, ADC0/ADC1, TIMER1 are clocked [I].

---

## Key RAM addresses (chip2)
| Address | Contents | Mark |
|---|---|---|
| 0x2000004c | current coil sample (16-bit) | [V] |
| 0x20000078 | USART2 RX buffer (256 B), [0]=0xA5 hdr, [1]=cmd | [V] |
| 0x20000094 | **base voltage** of the coil (16-bit, default 2048) | [V] |

## Main [U] gaps
1. **Coil left/right** — which of ADC0_CH5(PA5)/ADC1_CH9(PB1) is which side: not proven.
2. **Lift limit-switch pins** — the exact GPIOs (port A/B) are hidden behind the lift-driver abstraction.
3. **Purpose of PA7** (analog, ADC_CH7) — configured, but not in the regular sequence.
4. **TIMER0/TIMER2** — used, but the role is not localized.
5. **Frame CRC8** — presence likely, the exact algorithm/polynomial not isolated (the hardware CRC is occupied by the flash self-test).
