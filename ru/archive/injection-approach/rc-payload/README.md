# rc-payload — RC-исполнитель chip1

> ★ **АКТУАЛЬНО: payload v2** (`payload.c`) = heartbeat + **RPC** (вызов функции по адресу из mailbox — для
> живой диагностики энергизации) + RC-привод левого. Причина v2: изолированный `FUN_0805aee0` НЕ крутит
> колесо (энергизация неполная); RPC позволяет вживую подтвердить недостающий шаг (гипотеза `FUN_0805f604`).
> Актуальный порядок работы и mailbox v2 — в `reverse-v2/reports/hw-session-runbook.md` (секция V2-СЕССИЯ) и
> в шапке `payload.c`. Заливка апгрейда: `dist/flash_v2.cfg` (bank1 only + resume). Тест: `rc_test.py`
> (arm/rpc/init-left/enable-left/drive/stop). Ниже — исходное описание v1 (hook/CRC/заливка актуальны).

---

## (v1, историческое описание — механика hook/CRC/заливки та же)

Минимальный собственный код, врезаемый в заводскую прошивку косилки MI 302 (chip1 GD32F305).
Даёт ручное управление левым колесом по SWD-mailbox. Правое — follow-up (энергайз правого драйвера
ещё не запинчен, см. SESSION-HANDOFF). Всё опирается на проверенный реверс `reverse-v2/`.

## Как работает
- **Hook** (аддитивный): единственный `BL FUN_0808368c` в 10мс-диспетчере hs-loop `FUN_08053af0`
  по адресу `0x08053b0e` заменён на `BL mx_veneer`. Veneer зовёт оригинал (пейсинг) + `mx_tick`,
  возврат на `0x08053b12`. Родная FSM/watchdog работают штатно — мы лишь добавляем свой тик.
- **mx_tick** каждые 10мс: инкрементит heartbeat; если mailbox взведён magic — при `rc_active`
  одноразово энергизирует левый драйвер (`FUN_0805aee0`) и гонит `set_left(dir,speed)` (CH3CV).
- Payload живёт в свободном bank1 `0x08090000` (92 б). Образ приложения под сквозным app-CRC —
  после врезки пересчитан в `0x080ffffc`.

## Mailbox — SRAM `0x20014000` (подтверждён свободным живьём)
| off | тип | поле | смысл |
|----|-----|------|-------|
|0x00|u32|heartbeat|payload инкрементит каждый тик (проверка что hook жив)|
|0x04|u8 |rc_active|1=рулим левым, 0=инертны|
|0x05|u8 |left_dir|0/1 (какое вперёд — уточнить живьём)|
|0x06|u8 |need_energize|хост=1 → энергизация левого ОДИН раз, payload чистит в 0|
|0x08|u16|left_speed|\|скорость\| в единицах set_left (0..~0x1800)|
|0x0C|u32|magic|**ВЗВОД**: привод активен только если == `0x5243414D`|

⚠ На буте mailbox = мусор RAM. Пока хост не записал magic — payload ТОЛЬКО тикает heartbeat, моторы
не трогает (мусорный rc_active движения не даст). Это защита от неконтролируемого пуска.

## Сборка
```
# payload (ARM):
docker run --rm -v "$PWD/firmware/rc-payload":/w mower-fwbuild bash -c \
 'cd /w && arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -std=c11 -Os -ffreestanding \
   -fno-common -ffunction-sections -fdata-sections -Wall -Wextra -Werror -T payload.ld -nostartfiles \
   -Wl,--gc-sections payload.c -o payload.elf && arm-none-eabi-objcopy -O binary payload.elf payload.bin'
# врезка + CRC -> dist/rc_fw.bin:
docker run --rm -v "$PWD":/work python:3.12-slim bash -c \
 'pip install -q capstone && python /work/tools/bench/pack_rc.py \
   /work/dist/gd32-mainboard-dump-v1.bin /work/firmware/rc-payload/payload.bin /work/dist/rc_fw.bin'
```
Тулчейн-образ: `firmware/mower-executor/Dockerfile.build.local` (`docker build -t mower-fwbuild ...`).

## Заливка (3 региона, brick-риск — держать питание!)
Текущий CRC-образа = `0xfd0f0f91`. Регионы (transform current→rc_fw.bin):
1. **bank0 hook-страница** `0x08053800..0x08053fff` (2КБ) — erase+program (меняются 4 байта BL @0x08053b0e).
2. **bank1 payload** `0x08090000` (92 б) — метод bank1 (flash_proof.cfg: PG раз на страницу + mwh + sleep);
   стереть страницу (там старый proof-payload) + записать.
3. **CRC-слово** `0x080ffffc` = `0xfd0f0f91`.
Откат: восстановить `dist/gd32-mainboard-dump-v1.bin`. Сеть восстановления — recovery.cfg + catch-and-hold.

## Управление после заливки (по SWD :4444, mww)
```
# 0) проверить hook: mdw 0x20014000 несколько раз — heartbeat РАСТЁТ = наш код в тике
# 1) взвести и поехать левым вперёд на скорости S (напр. 0x600), колёса ВЫВЕШЕНЫ:
mww 0x20014008 0x00000600      # left_speed=0x600
mww 0x20014004 0x00010001      # rc_active=1, left_dir=0, need_energize=1
mww 0x2001400c 0x5243414d      # magic = ВЗВОД (последним!)
# 2) стоп:
mww 0x2001400c 0x00000000      # снять взвод  (или mww 0x20014008 0 = скорость 0)
```
Тест ТОЛЬКО на вывешенных колёсах, нож отключён, косилка на питании.
