# coil-scope — an "oscilloscope out of an ESP32" for the perimeter-wire signal

Goal of the first step (phase 0.5): **without opening the mower**, see the native wire
signal — whether it exists, at what frequency, what it looks like, and **whether it is coded**.
There is no oscilloscope — its role is played by the ESP32 itself (a web UI with waveform + spectrum).

## Coil from a twisted pair (0 ₽)

1. Unravel a twisted pair, take **one** wire (5–10 m long).
2. Wind **~150–200 turns** onto a cylinder Ø ~6–8 cm (a bottle, tape roll, can).
   More turns = more signal. No core is needed (an air coil is fine).
3. The coil ends are the two leads. Fix the winding with tape.

## Connection to the ESP32 (minimum, no components)

| Coil | ESP32 |
|---------|-------|
| lead A | **GPIO36 (VP)** |
| lead B | **GND** |

Power the ESP32 — **a power bank over USB**. Nothing else is needed for the first test.

> Attention: GPIO36 is input-only (ADC1), it is the correct pin. Do not confuse it with 5V/3.3V.

## Firmware (board in the Mac, binary built on Linux)

Without home Wi-Fi and without settings: the ESP brings up its own access point, you watch straight from the phone.
Ready binary: `build/coil-scope-full.bin` (a single file, flash at address **0x0**).

### Step 1. Flash the binary from the browser (no installs on the Mac)
1. Plug the ESP32 into the Mac over USB.
2. Open in **Chrome/Edge**: `https://espressif.github.io/esptool-js/`
3. Baudrate 921600 → **Connect** → choose the board's port.
4. Add file → `coil-scope-full.bin`, offset **0x0** → **Program**.
   (If it does not connect — hold BOOT on the board while plugging in.)

### Sensor connection
- **Coil:** one end → GPIO36 (VP), the other → GND.
- **IMU LSM6DS3:** VIN→3V3, GND→GND, SCL→GPIO22, SDA→GPIO21, **CS→3V3** (I2C mode).
  Do not connect the other pins (SAD/OCS/SCX/SDX/INT). The page will show
  "IMU (0x69)" if found, otherwise "NOT FOUND".

### Step 2. Watch from the phone (AP mode)
1. Connect the phone to the Wi-Fi network **`coil-scope`** (password `mower1234`).
2. Open in the browser **`http://192.168.4.1/`**.
3. You see: the wire signal (waveform/spectrum/frequency) + IMU (tilt, angular rate).

> There is no MQTT/Home Assistant in this version (not needed for the field). If logging
> into HA is needed — I will add STA mode (connecting to the home network) as a separate build.

## Rebuilding (I do this, on Linux, in a container)
```
cd firmware/coil-scope
docker run --rm -v $PWD:/p -w /p -v /root/.pio-core:/root/.platformio \
  python:3.12-slim bash -lc "pip install -q platformio && pio run"
# then merge into one file build/coil-scope-full.bin (see the command history)
```

## How to use

- On the page — the **peak frequency**, the **waveform** and the **spectrum** (updates ~3 times/s).
- Bring the coil close to the perimeter wire (or to the charging base/station — the sender is there).
- Move the coil and rotate it: we look for a **clear peak** in the spectrum and a maximum of the amplitude.
- Frequency/amplitude are also mirrored to MQTT (`mower/mi302/recon/wire_freq`, `/wire_mag`) —
  visible in Home Assistant.

## What this gives us

- The **frequency** of the native signal → we will tune the reception in the brain to it.
- The **waveform/spectrum** → to understand whether it is a simple tone (a single peak) or a complex code (a wide
  spectrum/comb). That determines whether the sender needs changing (docs/06).
- If the signal is weak without amplification — we will add a Vcc/2 bias (2 resistors) or
  an op-amp. First we try it as-is.
