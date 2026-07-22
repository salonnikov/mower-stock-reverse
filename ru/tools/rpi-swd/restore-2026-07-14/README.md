# chip1: опознать прошивку + откат на завод (2026-07-14)

Готовый пакет под малину (SWD bitbang к chip1 GD32F305). Две итерации.
Пины chip1: **SWCLK=GPIO25 (pin22), SWDIO=GPIO24 (pin18), GND (pin6)** → `swd_nums 25 24`.

## Что установлено (анализ на хосте)
- Наши флешеры пишут app с `0x08018000`. Наш app SP = **0x20018000**, заводской = **0x20017ff8** —
  одно слово надёжно отличает «наше» от «завода».
- **Последний билд (testO, 10.07) стирал 3 страницы: 0x18000, 0x18800, 0x19000** и писал до 0x080192f6.
  Все старые `flash_revert*.cfg` откатывают только **2** страницы (0x18000..0x18ffe) → **страница 3
  осталась бы не-заводской → CRC всего app (@0x080ffffc) не сойдётся → брик-луп.** Поэтому здесь — свой
  корректный **3-страничный** откат, байт-в-байт из `dist/gd32-mainboard-dump-v1.bin`.

## Файлы
| файл | что | пишет? |
|---|---|---|
| `chip1-identify.cfg` | опознать прошивку + связь + проверить высокие регионы | нет (read-only) |
| `run_identify.sh` | detached-раннер identify, лог `/home/pi/identify.log` | нет |
| `flash_factory_restore_3page.cfg` | откат 3 страниц app на завод (3071 полуслов) | **ДА** |
| `run_restore.sh` | detached-раннер отката + авто-verify, лог `/home/pi/restore.log` | **ДА** |
| `verify_after.cfg` | read-back проверка, что 3 стр. = завод | нет |

## Заброс на малину
```bash
scp -r tools/rpi-swd/restore-2026-07-14 pi@<малина>:/home/pi/mower-swd/restore/
ssh pi@<малина> 'chmod +x /home/pi/mower-swd/restore/*.sh'
```

## ИТЕРАЦИЯ 1 — ОПОЗНАТЬ (read-only, безопасно)
Косилка должна быть **под питанием** (не спать), иначе chip1 не виден (DPIDR=deadbeef/нули).
```bash
ssh pi@<малина> 'nohup /home/pi/mower-swd/restore/run_identify.sh >/dev/null 2>&1 &'
sleep 8
ssh pi@<малина> 'cat /home/pi/identify.log'
```
Читаем вывод:
- **LINK: CPUID/DBG_ID** ненулевые → связь есть. Всё deadbeef/0 → косилка спит/нет питания.
- **`>>> ПРОШИВКА = ...`** → завод или наш own_fw.
- Строки PAGE3 / HOOK / PAYLOAD / CRC: если все = **ЗАВОД**, а SP = **НАШ** — тронуты только 3 младшие
  страницы → 3-страничный откат = точный и достаточный.
- Если HOOK/PAYLOAD/CRC помечены `!!!` — тронуты высокие регионы (bank1/инъекция) → **не заливать**
  3-стр. откат, сказать мне: сгенерю расширенный (нужен `dist/gd32-mainboard-dump-v1.bin` на малине).

## ИТЕРАЦИЯ 2 — ОТКАТ НА ЗАВОД (по отдельной команде)
Только после того, как identify подтвердил «тронуты только 3 стр.». **Держать питание косилки.**
```bash
ssh pi@<малина> 'nohup /home/pi/mower-swd/restore/run_restore.sh >/dev/null 2>&1 &'
sleep 60
ssh pi@<малина> 'cat /home/pi/restore.log'
```
Ждём в логе `FLASH rc=0`, затем verify: `>>> OK: 3 страницы = ЗАВОД`.
Затем — **физический POWER-CYCLE косилки** (не `reset run` — он рассинхронит платы → E88).

## Если заливка оборвётся (WiFi/питание)
Флеш-обрыв обратим (проверено): catch-and-hold ловушка в окне питания (юзер держит кнопку) —
см. `reverse-v2/reports/flash-procedure.md` §5. Перезапуск `run_restore.sh` идемпотентен
(erase+write тех же 3 страниц).
