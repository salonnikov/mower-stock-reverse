# MOWER dashboard — веб-пульт косилки через SWD

Веб-панель на малине: телеметрия + управление приводом/ножом нашей прошивки
chip1 (GD32F305) через mailbox в SRAM, поверх OpenOCD telnet `:4444`.

**Только stdlib Python 3.7+ — никаких pip-зависимостей.** Фронтенд полностью
self-contained (один HTML, без CDN).

```
tools/dashboard/
├── server.py            # backend: telnet :4444 (одна связь + lock), поллер, dead-man, HTTP API
├── static/
│   └── index.html       # frontend: телеметрия, слайдеры, STOP, ARM, BLADE (всё inline)
└── README.md
```

## Деплой на малину (pi@192.168.10.81)

```bash
# с мака (wifi малины ужасный — копировать терпеливо):
scp -r tools/dashboard pi@192.168.10.81:~/dashboard

# на малине:
# 1) убедиться, что openocd поднят и НИКТО другой не сидит на :4444
sudo systemctl status mower-openocd        # должен быть active (poll.cfg)
sudo systemctl stop mower-webctl 2>/dev/null || true   # :4444 — ОДИН клиент!
# bench-скрипты (swd.py и т.п.) тоже должны быть закрыты

# 2) запуск (detached — wifi рвётся, обязательно nohup/tmux):
cd ~/dashboard
nohup python3 server.py > dash.log 2>&1 &

# 3) с телефона/мака в той же сети:
#    http://192.168.10.81:8080
```

Порт меняется через env: `DASH_PORT=9090 python3 server.py`.
Остановка: `pkill -f dashboard/server.py` (при выходе по Ctrl-C сервер сам
делает stop+disarm).

## Как это устроено

- **Одна telnet-связь** с OpenOCD `127.0.0.1:4444` (паттерн `tools/bench/swd.py`).
  Весь доступ сериализован через один `Lock` — поллер, keep-alive и HTTP-команды
  никогда не пишут в сокет одновременно. Каждая команда ~10–150 мс.
- **Поллер** (~4 Гц): один `mdw 0x20014000 9` → декод всех полей → кэш.
  `GET /api/telemetry` отдаёт кэш мгновенно, UI не ждёт SWD.
- Если openocd лежит / чтение вернуло мусор → `alive:false`, реконнект
  с backoff 2 с. Сервер не падает никогда.

## Dead-man / безопасность (двухзвенная цепочка)

1. **Прошивка ↔ бекенд (seq @ +0x08).** Пока armed, поток keep-alive бампает
   `seq` каждые ~150 мс; поллер и каждая команда дополнительно бампают seq
   piggyback'ом под тем же локом. Если бекенд умер/завис — seq замирает,
   прошивка через ~200 мс сама останавливает моторы.
2. **Бекенд ↔ браузер (свежесть /api/drive).** Пока колёса крутятся, фронтенд
   шлёт `/api/drive` каждые 200 мс. Если команды перестали приходить >1 с
   (телефон уснул, wifi отвалился) — бекенд сам обнуляет скорости
   (arm сохраняется, seq продолжает бампаться).
3. **UI:** слайдеры-пружины (отпустил = мгновенно 0), активны только при ARM.
   STOP — всегда: скорости 0 + нож 0 + DISARM. Нож включается только при ARM
   и только через двухшаговое подтверждение (3 с); выключается одним нажатием.
   Уход со страницы (visibilitychange) шлёт beacon `/api/stop`.

**Перед первым запуском:** колёса вывешены, нож ОТСОЕДИНЁН. Проверить STOP и
dead-man (убить сервер во время езды — колёса должны встать за ~200 мс) до
любых тестов на земле.

## HTTP API

| Метод/путь            | Тело                      | Действие |
|-----------------------|---------------------------|----------|
| `GET /api/telemetry`  | —                         | JSON-кэш телеметрии (см. ниже) |
| `POST /api/arm`       | `{"on": true/false}`      | ARM (magic=0x5243414D, скорости 0) / DISARM (скорости 0, нож 0, magic 0) |
| `POST /api/drive`     | `{"left": -7200..7200, "right": ...}` | Скорости колёс (одним mww в +0x0C); если не armed — армит; бампает seq |
| `POST /api/blade`     | `{"on": true/false}`      | Нож. Включение требует ARM (иначе 400) |
| `POST /api/stop`      | `{}`                      | Полный стоп: скорости 0, нож 0, DISARM |

Ответ команд: `{"ok":true}` или `{"ok":false,"error":"..."}` (400 — логика,
503 — SWD/связь).

`GET /api/telemetry` →
```json
{
  "connected": true,          // telnet до openocd жив
  "alive": true,              // heartbeat менялся за ~0.7 с (прошивка живая)
  "heartbeat": 12345678,
  "armed": true,              // что записал бекенд
  "armed_fw": true,           // magic в mailbox == 0x5243414D
  "fw_state": 2, "fw_state_name": "DRIVING",   // 0=DISARMED 1=ARMED-IDLE 2=DRIVING
  "left_speed": 3600, "right_speed": 3600,      // записанные i16
  "blade_on": false,
  "batt_mV": 25100, "batt_pct": 87, "charge": false,
  "ch3_duty": 3590, "ch2_duty": 3612, "ch1_duty": 0,  // факт. PWM: left/right/blade
  "btn": false, "seq": 4242, "speed_max": 7200, "ts": 1751500000.0
}
```

## Контракт mailbox (base = 0x20014000)

CONTROL — пишет дашборд (`mww`/`mwh`):

| Смещение | Тип | Поле | Примечание |
|---|---|---|---|
| +0x04 | u32 | magic | `0x5243414D` = ARM, `0` = disarm |
| +0x08 | u32 | seq | dead-man: должен меняться чаще ~200 мс пока armed |
| +0x0C | i16 | left_speed | −7200..+7200; low16 слова |
| +0x0E | i16 | right_speed | high16 того же слова — пишется одним `mww 0x2001400C` |
| +0x10 | u8 | blade_on | 0/1 (пишем `mwh`) |

TELEMETRY — пишет прошивка (читаем одним `mdw 0x20014000 9`):

| Смещение | Тип | Поле |
|---|---|---|
| +0x00 | u32 | heartbeat (~1 кГц; замер = прошивка висит/выключена) |
| +0x14 | u16 | batt_mV |
| +0x16 | u8 | batt_pct 0..100 |
| +0x17 | u8 | charge 0/1 |
| +0x18 | u16 | ch3_duty (левое колесо, факт PWM) |
| +0x1A | u16 | ch2_duty (правое колесо) |
| +0x1C | u16 | ch1_duty (нож) |
| +0x1E | u8 | btn (кнопка питания) |
| +0x1F | u8 | fw_state (0=disarmed, 1=armed-idle, 2=driving, …) |

## Известные ограничения

- `:4444` — строго один клиент. Пока дашборд работает, bench-скрипты и
  mower-webctl использовать нельзя (и наоборот).
- SWD-команды медленные (до ~150 мс); при жалобах прошивки на dead-man во
  время езды можно поднять `KEEPALIVE_PERIOD`/`POLL_PERIOD` в `server.py`
  (например poll 2 Гц).
- Аутентификации нет — только доверенная локальная сеть.
