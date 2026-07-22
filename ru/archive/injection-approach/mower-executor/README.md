# mower-executor — тонкий исполнитель RC-управления для chip1 (Вариант 1)

Заменяет «мозг» родной прошивки мейнборда (GD32F305) на внешнее управление
(RPi/ESP32), **переиспользуя родные драйверы** (A4963/PWM/датчики/RTOS/watchdog).
Доставляется как патч стокового образа через brick-safe USB-канал (`SNK_MB_*.bin`).

## Архитектура
```
RPi/ESP32 --UART(бинарный кадр &..#, CRC-8)--> [chip1: наш блоб]
   команды DRIVE/BLADE/ESTOP/TELEM            hook -> mx_executor_tick
                                               |-> safety (failsafe/e-stop)
                                               |-> mixer (кламп/слю/дедбенд)
                                               |-> mx_vendor.drive -> A4963/PWM (родное)
                                               |-> телеметрия <- датчики (родные vtable)
                                               |-> feed watchdog
```
- Блоб живёт в свободной флеш app-региона; при загрузке конструктор
  process-manager получает наш указатель (патч флеш-константы) → наш tick
  вызывается вместо родной навигации. RTOS/драйверы/watchdog не трогаются.

## Слои
| Модуль | Роль | Тестируемость |
|---|---|---|
| `mx_crc8` | CRC-8 Dallas/Maxim (== родной `FUN_080241d0`) | хост-тест (эталон 0xA1) |
| `mx_proto` | кадрирование `&|LEN|CMD|payload|CRC|#`, RX-автомат | хост-тест (roundtrip/CRC/ресинх/overflow) |
| `mx_mixer` | RC→скорости колёс: кламп 0..6141, дедбенд, слю-рейт | хост-тест |
| `mx_safety` | failsafe по таймауту, e-stop латч (lift/hit) | хост-тест |
| `mx_executor` | склейка: команды→safety→mixer→привод→телеметрия | хост-тест (моки) |
| `mx_vendor` | обёртки родного драйвера по адресам (ARM) | обзор кода |
| `mx_hook` | перехват tick process-manager, UART I/O (ARM) | обзор кода |

Портируемое ядро (crc/proto/mixer/safety/executor) **полностью покрыто** хостовыми
тестами под ASan/UBSan + cppcheck + clang-tidy + clang analyzer (0 предупреждений).

## Сборка (только в docker, образ mower-fwbuild)
```
docker build -f Dockerfile.build -t mower-fwbuild .
docker run --rm -v $PWD:/w mower-fwbuild make -C /w test     # юнит-тесты
docker run --rm -v $PWD:/w mower-fwbuild make -C /w asan     # ASan+UBSan
docker run --rm -v $PWD:/w mower-fwbuild make -C /w lint     # cppcheck+clang-tidy
docker run --rm -v $PWD:/w mower-fwbuild make -C /w analyze  # clang static analyzer
docker run --rm -v $PWD:/w mower-fwbuild make -C /w fw       # ARM-блоб executor.bin
```
Упаковка образа обновления (не заливает!):
```
python3 tools/pack_snk_mb.py --dump ../../dist/gd32-mainboard-dump-v1.bin \
   --blob build/executor.bin --version <BIG> --out-dir /out
# -> SNK_MB_<BIG>.bin (версия ДОЛЖНА быть > установленной)
```

## ⚠️ Что подтвердить на живом чипе перед ЛЮБОЙ заливкой ([H] в mx_vendor.h)
1. Точный вход `right_motor_rotation` и слот `rmdrv` (левый — `0x2000054c` [OK]).
2. Что патчим именно ПЕРИОДИЧЕСКИЙ tick process-manager (`0x0804f088`→`0x080549f0`),
   а не одноразовый init; иначе выбрать метод1 (`0x0804f08c`).
3. Свободность области `0x080F0000` (упаковщик проверяет 0xFF — сейчас свободна).
4. Свободность RAM-региона блоба (`ld/executor.ld`, `0x2001C000`).
5. Адреса UART наружу (`MX_ADDR_UART_*`), тика (`now_ms`), watchdog feed.
6. Офсеты vtable датчиков (IMU/ток/дождь/кнопки) — телеметрия дополнится.

Пока любой [H]=0 — соответствующий вызов безопасно пропускается (guard на NULL).
Откат всегда: полный дамп `dist/gd32-mainboard-dump-v1.bin` (SWD) или SNK_MB из стока.
