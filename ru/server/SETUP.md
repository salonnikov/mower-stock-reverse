# Подключение к существующему Home Assistant

HA у тебя уже есть — новый не разворачиваем. Нужно только связать ESP32 с ним.

## Вариант A (рекомендую) — ESPHome через нативный API, без MQTT

Если у тебя стоит add-on **ESPHome** (или ESPHome Device Builder) — это проще всего:
прошивка `firmware/esphome/mower.yaml` импортируется, ESP32 появляется в HA
автоматически (нативный API, шифрование), MQTT-брокер не нужен **для самого HA**.

> Нюанс: ты хотел «свой бэкенд потом». Нативный API HA — НЕ переносим на чужой бэкенд.
> Поэтому см. вариант B, либо оставь оба: native API для HA + публикация в MQTT
> параллельно (ESPHome умеет и то и другое одновременно).

Шаги:
1. HA → Settings → Add-ons → ESPHome → открыть.
2. New Device → вставить содержимое `firmware/esphome/mower.yaml`, заполнить `secrets`.
3. Install → по USB (первый раз), дальше OTA по Wi-Fi.
4. Устройство `mower-mi302` само добавится в HA со всеми сенсорами/кнопками.

## Вариант B — MQTT (если хочешь переносимость на свой бэкенд)

1. Брокер:
   - HAOS/Supervised → поставь add-on **Mosquitto broker** (Settings → Add-ons).
   - HA в чистом Docker без брокера → `docker compose up -d` в этом каталоге.
2. В HA: Settings → Devices → Integrations → **MQTT** → broker = адрес Mosquitto:1883.
3. В `mower.yaml` оставь блок `mqtt:` (он уже есть), пропиши broker в `secrets.yaml`.
4. ESP32 публикует телеметрию в `mower/mi302/...` (контракт — docs/01-architecture.md).
   Свой бэкенд позже подпишется на те же топики — HA менять не придётся.

## Автоматизации (погода, антиугон, низкий заряд)

Файл `homeassistant/automations-weather.yaml` — скопируй правила в свой HA:
- либо через UI (Settings → Automations → импорт YAML),
- либо добавь `automation: !include automations-weather.yaml` и положи файл в /config.

Проверь имена сущностей под свой HA (`binary_sensor.mower_mi302_rain` и т.д.) —
они зависят от того, как назвал устройство.

## Что от тебя нужно, чтобы двигаться

1. Сказать: вариант **A** или **B** (или оба).
2. Стоит ли уже add-on Mosquitto / ESPHome в твоём HA?
