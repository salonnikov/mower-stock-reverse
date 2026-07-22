# MOWER — reverse engineering of the VILLARTEC MI 302 robot lawn mower

## Introduction

This is a from-scratch reverse engineering of the **stock control board** of a robot lawn
mower. The board is not unique to one product: the same base design (a GD32 main MCU + a GD32
boundary-sensor MCU + an ESP32 display + Fortior FU6832N BLDC drivers) is shared across **many
rebranded mowers** on the market, so the findings here should apply well beyond the specific
VILLARTEC MI 302 unit we worked on.

The work covers both factory firmwares decompiled from scratch, a complete architecture /
peripheral / subsystem map, a working SWD flash / dump / restore toolchain, and a custom chip1
firmware that runs on the real hardware. This repository is the consolidated knowledge base of
everything we found.

**It is not finished.** The core reverse engineering is solid and verified on real hardware, but
the project never reached its final goal — see *[Main unsolved problem](#-main-unsolved-problem--the-motors-never-spun-on-our-firmware)*
below. **If you want to take it further, please do** — everything you need to continue is here.

> **Why it stopped:** our physical board **died** — rain got into the mower and the **power
> section failed**. So the work is frozen where it is; this repo is published so the knowledge
> isn't lost and anyone with the same base board can pick it up.

> ⚠️ **Blade + 20 V Li-ion.** Any work involving the blade — only with the battery removed /
> key pulled. There is no official schematic (closed OEM); everything here was recovered by
> reverse engineering the two factory GD32 firmwares.

> **Language:** the primary layer of this repo is **English**. The original Russian working
> notes are preserved verbatim under [`ru/`](ru/) (active docs mirror the primary tree;
> historical / superseded material is under [`ru/archive/`](ru/archive/)).

---

## The hardware (established by reverse engineering)

The MI 302 board = **three MCUs + three BLDC controllers**:

| Node | Chip | Role |
|---|---|---|
| chip1 (main) | **GD32F305 (GD32F30x_CL)** | mower FSM, drive (SPI to the BLDCs), BMS link (USART2), link to chip2 and the display |
| chip2 (boundary sensor) | **GD32F4xx** | perimeter-coil acquisition (ADC+DMA), wave/area detection, lift sensors → chip1 over UART/cJSON |
| display | **ESP32-WROOM-32UE** (SNK DISPLAY CP V11) | buttons / LEDs, ledport protocol |
| drive (×3: L/R wheels + blade) | **Fortior FU6832N** | smart BLDC controller (8051 + FOC, own 16 kB firmware), listens on SPI as an A4963-compatible slave |

> **Important correction (2026-07-13):** early docs called the drivers "Allegro A4963" — by
> board photos they are physically absent; they are **FU6832N** parts emulating the A4963
> protocol. The SPI reverse from the chip1 side stays valid; conclusions taken from the A4963
> datasheet (POR defaults, RUN=1, VBB behaviour) are **void**.

---

## Summary of findings

### 1. Firmware & decompilation
- **Both factory firmwares decompiled from scratch** (Ghidra, canonical pass) — see
  [`reverse-v2/chip1`](reverse-v2/chip1) and [`reverse-v2/chip2`](reverse-v2/chip2)
  (`decompiled_all.c`, symbols, strings). The old `docs/fw/` and the first iteration are **not
  trustworthy** (kept only as history under `ru/archive/`).
- **Architecture / peripheral / subsystem map** — [`reverse-v2/ARCHITECTURE.md`](reverse-v2/ARCHITECTURE.md),
  [`reverse-v2/reports/subsystem-findings.md`](reverse-v2/reports/subsystem-findings.md),
  [`reverse-v2/factory-map/`](reverse-v2/factory-map/) (function-by-function walkthrough,
  start at `00-INDEX.md`).
- **Function & peripheral references** — [`reverse-v2/reference/`](reverse-v2/reference/)
  (`FUNCTIONS-chip1/2`, `PERIPHERALS-chip1/2`, `MOWING-ALGORITHM`, bring-up RAM, register maps).

### 2. Boot, power and security
- **Boot / power-on sequence and the power-latch** — [`reverse-v2/factory-map/01-boot-poweron.md`](reverse-v2/factory-map/01-boot-poweron.md).
  A key operational fact: a `deadbeef` DPIDR on chip1 means **the chip is powered off**
  (power-latch open), *not* a broken SWD wire.
- **PIN / pairing security** — [`reverse-v2/factory-map/02-pin-security.md`](reverse-v2/factory-map/02-pin-security.md).

### 3. Mow / home state machine
- **FSM (mow ↔ home ↔ idle) and the mowing algorithm** —
  [`reverse-v2/factory-map/03-fsm-mow-home.md`](reverse-v2/factory-map/03-fsm-mow-home.md),
  [`reverse-v2/reference/MOWING-ALGORITHM.md`](reverse-v2/reference/MOWING-ALGORITHM.md).

### 4. Drive (BLDC / FU6832N over SPI)
- **The SPI drive chain is decompiled byte-for-byte** —
  [`reverse-v2/factory-map/04-motor-drivers.md`](reverse-v2/factory-map/04-motor-drivers.md),
  [`reverse-v2/reports/drive-chain.md`](reverse-v2/reports/drive-chain.md).
  Chip-selects: left = PD5, right = PD4, blade = PD3; SPI1; PWM on TIMER2 ch3. There is **no
  GPIO "enable"** line to the drivers (arming = SPI register writes + power rail).
- **SPI write into the BLDC is confirmed** — the wheel channels arm over SPI, and the **blade
  pulsed RUN=1/0 on command**. So the SPI path itself works end-to-end.

### 5. BMS / battery-pack link (USART2)
- **The pack link is established** — USART2, 19200 8N1, half-duplex on PD8/PD9; frame
  `3A A3 len … C1 … CRC8` (CRC-8/MAXIM); connect handshake ×4; the pack answers (`connected=1`).
  Details: [`reverse-v2/factory-map/05-bms-pack.md`](reverse-v2/factory-map/05-bms-pack.md).

### 6. Boundary detection (chip2)
- **Border detection is dissected** — perimeter coils via ADC+DMA, wave/area classification;
  the sensitivity knob is the `|sample| > 2500` threshold in `FUN_0801baf8`, the base voltage
  auto-calibrates, and the whole image is guarded by an **IEC60730 FLASH-CRC32** (so patching
  flash breaks the CRC). How to adjust it for new coils:
  [`reverse-v2/factory-map/06-chip2.md`](reverse-v2/factory-map/06-chip2.md),
  [`reverse-v2/reports/CHIP2-BORDER-SENSITIVITY-2026-07-13.md`](reverse-v2/reports/CHIP2-BORDER-SENSITIVITY-2026-07-13.md).

### 7. Custom firmware + SWD toolchain
- **Custom chip1 firmware is flashed and runs live** — [`firmware/mower-own/`](firmware/mower-own/):
  heartbeat, battery read, normal idle. Flashing is **CRC-safe** (the bootloader does not check
  the app CRC).
- **SWD infrastructure on Raspberry Pi** — flash / dump / restore via openocd bitbang; the full
  brick → recovery cycle was verified on the bench. chip1 = GPIO25/24, chip2 = GPIO8/7.
  Scripts: [`tools/bench/`](tools/bench/), [`tools/rpi-swd/`](tools/rpi-swd/);
  procedure: [`reverse-v2/reports/flash-procedure.md`](reverse-v2/reports/flash-procedure.md).
- **Factory restore verified** — chip1 was rolled back to a bit-exact factory image
  (SP `0x20017ff8`, CRC `0x0f69a878`); tooling in [`tools/rpi-swd/restore-2026-07-14/`](tools/rpi-swd/restore-2026-07-14/).
- **Dumps, firmware images and flashers** — [`dist/`](dist/), see [`dist/FLASHERS.md`](dist/FLASHERS.md)
  (includes the factory chip1 dump `gd32-mainboard-dump-v1.bin`, the chip2 dump, and the full
  `factory-full.asm` disassembly).

---

## ⛔ Main unsolved problem — the motors never spun on our firmware

**Under our own firmware the wheels never physically turned**, even though SPI, PWM and GPIO are
driven correctly and the **blade is controllable**. Something was **stalling them**, and the
cause was never reduced to a single proven fact. On the bench the wheel produced **total
silence** (3-phase BLDC, no movement at all), which points at the drive controller **not being
powered / not being armed** rather than at the SPI/PWM/GPIO layer (all of which match the
factory firmware).

Open hypotheses and the plan to close them —
[`reverse-v2/reports/WHEELS-ROOT-CAUSE-AUDIT-2026-07-10.md`](reverse-v2/reports/WHEELS-ROOT-CAUSE-AUDIT-2026-07-10.md):

- selection of the FU6832 **fast/slow config** for the wheel channel,
- byte-exact **SPI CTL0/CTL1** vs. factory,
- full **TIMER2** match vs. factory,
- possible **gating of the power rail** (a BMS command, or a hardware power switch / VBB) that
  the factory enables and we do not.

The next single SWD capture against the factory firmware is designed to record everything needed
to settle this at once — [`reverse-v2/reports/SWD-CAPTURE-PACKAGE-2026-07-13.md`](reverse-v2/reports/SWD-CAPTURE-PACKAGE-2026-07-13.md).

---

## Repository layout

```
reverse-v2/            ★ CORE: clean re-teardown of both firmwares
  ARCHITECTURE.md      unified robot map (chip1 / chip2 / display / drive)
  chip1/ chip2/        canonical decompile (decompiled_all.c, symbols, strings)
  factory-map/         full factory-firmware walkthrough by function (00-INDEX)
  reference/           references (bring-up RAM, drivers, registers, mowing algorithm)
  reports/             current reports and plans — see reports/README.md
  analysis/ measurements/ ghidra-scripts/   supporting analysis
firmware/
  mower-own/           ★ custom chip1 firmware (flashed, runs; drive still under debug)
  coil-scope/          coil work (ESP32)
  esphome/mower.yaml   telemetry bench for the display ESP32 (read-only)
tools/
  bench/               flash-script generators, CRC, packer, recovery.cfg
  rpi-swd/             SWD configs on the Pi (chip1 25/24, chip2 8/7) + restore-2026-07-14/
  dashboard/ img/      supporting
dist/                  dumps, firmware images, flashers — see dist/FLASHERS.md
server/                HA/MQTT glue, sniffers, scraper (telemetry)
hardware/              signal-map, SWD/ESP wiring diagrams
ru/                    Russian originals (verbatim); ru/archive/ = historical / superseded
```

## Historical: the "ESP32 brain-transplant" approach

The project originally ran as an ESP32-S3 overlay on top of the stock board (Ardumower port,
matched-filter wire reception). That approach was **abandoned** in favour of directly controlling
the stock GD32 with our own firmware. The planning docs and the old ESP32 brain firmware are kept
under [`ru/archive/old-docs/planning-esp32/`](ru/archive/old-docs/planning-esp32/) and
[`ru/archive/`](ru/archive/) (see `ru/archive/README.md`).
