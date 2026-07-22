# Connecting to an existing Home Assistant

You already have HA — we do not deploy a new one. We only need to link the ESP32 to it.

## Option A (recommended) — ESPHome via the native API, without MQTT

If you have the **ESPHome** add-on installed (or ESPHome Device Builder) — this is the easiest:
the firmware `firmware/esphome/mower.yaml` is imported, the ESP32 appears in HA
automatically (native API, encryption), an MQTT broker is not needed **for HA itself**.

> Caveat: you wanted "my own backend later". The HA native API is NOT portable to a third-party backend.
> So see option B, or keep both: the native API for HA + publishing to MQTT
> in parallel (ESPHome can do both at once).

Steps:
1. HA → Settings → Add-ons → ESPHome → open.
2. New Device → paste the contents of `firmware/esphome/mower.yaml`, fill in the `secrets`.
3. Install → over USB (the first time), then OTA over Wi-Fi.
4. The device `mower-mi302` adds itself to HA with all sensors/buttons.

## Option B — MQTT (if you want portability to your own backend)

1. Broker:
   - HAOS/Supervised → install the **Mosquitto broker** add-on (Settings → Add-ons).
   - HA in plain Docker without a broker → `docker compose up -d` in this directory.
2. In HA: Settings → Devices → Integrations → **MQTT** → broker = the Mosquitto:1883 address.
3. In `mower.yaml` keep the `mqtt:` block (it is already there), set the broker in `secrets.yaml`.
4. The ESP32 publishes telemetry to `mower/mi302/...` (the contract — docs/01-architecture.md).
   Your own backend will later subscribe to the same topics — HA will not need changing.

## Automations (weather, anti-theft, low charge)

The file `homeassistant/automations-weather.yaml` — copy the rules into your HA:
- either via the UI (Settings → Automations → import YAML),
- or add `automation: !include automations-weather.yaml` and put the file in /config.

Check the entity names for your HA (`binary_sensor.mower_mi302_rain` etc.) —
they depend on how you named the device.

## What is needed from you to move forward

1. Say: option **A** or **B** (or both).
2. Do you already have the Mosquitto / ESPHome add-on in your HA?
