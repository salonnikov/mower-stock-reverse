# Signal map of the SNK_MAINBOARD_CP_V11 board (LIVING document)

This is our de-facto "schematic". Filled in during phase 0
(see [../docs/00-reverse-engineering.md](../docs/00-reverse-engineering.md)).
Do not delete empty rows — this is a checklist of what still needs to be measured.

## FINDINGS FROM THE TEARDOWN (2026-06) — OEM = "SNK" (probably Sunseeker)

Two boards + a shared UART. The silkscreen is LABELED — reversing is easy.

**Mainboard `SNK_MAINBOARD_CP_V11` (PN 80102372-01):**
- Main MCU: ARM Cortex-M, by the look **MM32 / GD32** (an STM32 clone), LQFP.
- **SWD/JTAG header (back side):** `3V3 / DIO / CLK / JTDO / RES / GND`.
- Connectors: `J9 = LEFT / RIGHT (+GND)` wheel motors; `J8 = GND / START / OK / +5V / ON`;
  `J7 = GND…`; signals `BLADE_1` (blade), `B_5V`, `B_AGND` (battery current measurement), `STOP`.
- USB port = firmware update from a FAT32 flash drive (shows "USb" on the screen). NOT a control channel.
- Powered from a 5S 18650 pack (~20 V).

**Display board `SNK_DISPLAY_CP_V11` (PN 80102373-01) = essentially an ESP32 module:**
- **`ESP32-WROOM-32UE`** (dual-core, Wi-Fi+BT, external U.FL antenna; FCC 2AC7Z-ESPWROOM32UE).
  It is THIS that drives the display/buttons/buzzer/Wi-Fi and talks to the mainboard over UART.
- 7-segment LED drivers (SOIC), buzzer BUZ1, buttons ON/START/HOME/OK.
- **Flashing header on the edge: `3V3 / T / R / GND / GND / P`** (T/R=UART0, **P=IO0/boot**) →
  you can dump and reflash the ESP32 in place.
- It has its own SWD header too.
- The white `J2` ribbon to the mainboard — power (5V/GND) + **inter-board UART** (telemetry+commands).

**PLAN (refined):** B — hijack the stock ESP32:
  0) `esptool read_flash` of the stock firmware (backup + the mainboard protocol is baked into it);
  1) sniff the inter-board UART for confirmation;
  2) reflash the ESP32 with our code → Wi-Fi/MQTT + control over the native UART, keeping display/buttons.

**Sensors:** odometry — Hall sensors `KCD KD-002`, labeled `LEFT`/`RIGHT` (potted in compound).

**PLAN (paths to the goal):**
- A. Sniff the inter-board UART (mainboard↔display ribbon) — the native protocol. ← start.
- B. Reflash the stock display ESP with our code (ready-made Wi-Fi + UART to the brain).
- C. SWD dump of the main MCU via the header.
- D. Direct transplant: drive J9 LEFT/RIGHT + BLADE, read the Halls/perimeter.

---

## Summary (fill in with conclusions)

- MCU logic level: **___ V** (3.3 / 5) ← CRITICAL for connecting the ESP32
- Main MCU: ___ (marking / "blob")
- Is there a debug UART: ___ (yes/no, pins, baud rate)
- Architecture conclusion: **companion / replace**, because ___

## Integrated circuits (IC)

| Ref | Package marking | What it is | Datasheet / notes |
|-----|-----------------------|---------|-------------------|
| U1  |                       | main MCU |               |
| U2  |                       | motor driver L |          |
| U3  |                       | motor driver R |          |
| U4  |                       | blade driver |              |
| U5  |                       | op-amp/comparator of the perimeter receiver | |
| U6  |                       | DC-DC (20V→5V) |            |
| U7  |                       | LDO (5V→3.3V) |             |

## Power

| Node | Voltage | Where measured | Notes |
|------|-----------|------------|---------|
| VBAT |  ~20 V    |            | 5S Li-ion |
| +5V  |           |            |         |
| +3V3 |           |            |         |
| GND  |    0      |            | reference ground |

## Motors

| Motor | Type (BDC/BLDC) | Control signals | MCU pin | PWM freq/duty | Halls |
|-------|----------------|--------------------|---------|------------------------|-------|
| Wheel L |             | PWM + DIR / 2×PWM |        |                        |       |
| Wheel R |             |                    |        |                        |       |
| Blade   |             |                    |        |                        |       |

## Perimeter signal (boundary wire)

| Parameter | Value | Notes |
|----------|----------|---------|
| Number of coils | 1 / 2 |  |
| Signal frequency |  | kHz, from the oscilloscope |
| Shape | sine/pulses |  |
| Comparator output → MCU pin |  | logic "I see the wire" |

## Sensors

| Sensor | Type | Active level | MCU pin | Notes |
|--------|-----|------------------|---------|---------|
| Rain  | comb+comparator |        |        |         |
| Lift | microswitch/accelerometer |     |        |         |
| Tilt | ball/accelerometer  |     |        |         |
| Bump (bumper) | microswitches         |     |        |         |
| Wheel odometry | Hall/encoder  |     |        |         |
| Battery current | shunt+op-amp → ADC      |     |        |         |
| Battery voltage | divider → ADC |     |        |         |

## Charging station

| Signal | Notes |
|--------|---------|
| Charge contacts (V+, V−) |  |
| "Found the station" signal |  search by a second wire/IR |

## Connectors (pinout)

| Connector | Purpose | Pins (wire color → signal) |
|--------|------------|------------------------------|
| J1     |            |                              |
| J2     |            |                              |
