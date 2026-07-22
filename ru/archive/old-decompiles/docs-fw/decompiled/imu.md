# IMU / КУРС / НАКЛОН (MEMS) — декомпиляция

Бинарь: `/opt/mower/dist/gd32-mainboard-dump-v1.bin` (chip1, GD32F305, Cortex-M4 Thumb-2).
База отображения: `vaddr = fileoffset + 0x08000000`. Инструмент: radare2 `pdg`/`pdf` (Ghidra-декомпилятор).

Легенда: `✔` — подтверждено строкой/регистром/вызовом; `ⓗ` — гипотеза, проверить на живом чипе.

---

## Краткая карта подсистемы

Прошивка несёт ДВА драйвера MEMS и выбирает один по модели платы в рантайме:
- Bosch **BNO055** — `driver_mems_snk_v13.c` (строки `get_mems_value_snk_v13`, `reset_mems_v13`). Инициализация `fcn.0802a3d0` — набор регистров и chip-id совпадают с BNO055 один-в-один ✔.
- TDK InvenSense **ICM-42688** — `driver_mems_tdk42688_v1.c` (строки `init_mems_tdk42688_snk_v1/v2`, `get_mems_value_tdk42688_snk_v1`).

Единый объект модуля MEMS лежит по указателю **`0x2000059c`** (драйверный слой), а обработанные углы (pitch/roll/heading + усреднённые) — в объекте service-слоя по указателю **`0x2000039c`**, поле `[+4]`.

Поток данных (verbatim-функции ниже):
```
fcn.0803a198 (service update, периодический)
   ├─ читает euler из активного драйвера (vtable[+8] через fcn.0804e754 → [0x2000059c+4])
   ├─ пишет мгновенные: heading→[obj+0x18], roll→[obj+0x1a], pitch→[obj+0x1c]   (obj = [0x2000039c+4])
   ├─ fcn.0802b868  calculate_mems_average_value → усреднённые [obj+0x1e/0x20/0x22]
   └─ fcn.08039fc4  slope-классификатор → использует ТОЛЬКО pitch(0x1c)+roll(0x1a)
```

---

## fcn.0802a3d0 — init BNO055 (driver_mems_snk_v13) ✔

Владелец строк `read mems id failed` @ `0x0802a5f8`, `set mems opr/page/acc cfg/gry cfg/axis map/axis sign/power mode failed` @ `0x0802a60c..0x0802a6a4` (xref `axt @ 0x0802a5f8` → `fcn.0802a3d0`).

```c
uint fcn.0802a3d0(void)
{
    int iVar1;
    uint uVar2;
    uint uVar3;
    int16_t iVar4;
    uchar auStack_20 [4];
    uint uStack_1c;
    uchar auStack_18 [4];

    iVar4 = 0x96;
    auStack_20[0] = 0;
    uStack_1c = 0;
    while ((((uStack_1c != -0x60 || (uStack_1c._1_1_ != -5)) || (uStack_1c._2_1_ != '2')) || (uStack_1c._3_1_ != '\x0f')
           )) {
        if (iVar4 == 0) goto code_r0x0802a440;
        fcn.08062980(0,&uStack_1c,4);          // I2C read reg 0x00, 4 байта (ID-блок)
        iVar4 = iVar4 + -1;
        fcn.08083610(5);                        // delay 5 мс
    }
    // ... далее последовательная запись регистров через fcn.0806299a(reg, &val, 1):
    // reg 0x3D (OPR_MODE), 0x07 (PAGE_ID), 0x08, 0x0A, 0x41 (AXIS_MAP_CONFIG),
    //     0x42 (AXIS_MAP_SIGN), 0x3E (PWR_MODE) ...  (полный листинг в pdg)
code_r0x0802a5e2:
    fcn.08048adc(2,0x802a700,0x802a6c4,*0x802a6c0,uVar2,uVar3);  // лог "set mems ... failed"
    return 0xffffffff;
}
```

Доказательства (это именно BNO055) ✔:
- Chip-ID блок читается по I2C reg `0x00`, 4 байта, и сверяется с константой байтов `{0xA0, 0xFB, 0x32, 0x0F}` (в коде `-0x60, -5, '2', '\x0f'`). Это ровно BNO055: `CHIP_ID=0xA0`, `ACC_ID=0xFB`, `MAG_ID=0x32`, `GYR_ID=0x0F` по регистрам 0x00..0x03.
- Записываемые регистры — карта BNO055: `0x3D OPR_MODE`, `0x3E PWR_MODE`, `0x41 AXIS_MAP_CONFIG`, `0x42 AXIS_MAP_SIGN`, `0x07 PAGE_ID`.
- `fcn.08062980` = I2C read `(reg, buf, len)`; `fcn.0806299a` = I2C write `(reg, buf, len)`; `fcn.08083610` = delay(ms). ⓗ (по сигнатурам вызовов).

Вызывается из reconfig-пути `get_mems_value_snk_v13` (`0x0804e93a`, строки `read mems opr mode =%d, reconfig bno055 failed/success`) и из `fcn.0806ed94` (`0x0806f05c`).

---

## fcn.080477c4 — выбор активного драйвера MEMS по модели платы ⓗ

Диспетчер bring-up драйверов. Ключ — код модели `iVar4 = (**(iVar3 + 0x24))()`. Для MEMS ветка `iVar5 = iVar4 - 0x50dc`:

```c
    iVar5 = iVar4 + -0x50dc;
    if (iVar5 == 0) {
        puVar6 = fcn.0806e5a4();          // конструктор MEMS-драйвера (модель 0x50dc)
    }
    else {
        bVar9 = iVar5 == 0x708;
        do {
            if (bVar9) {
                iVar5 = (**(iVar3 + 0x54))();
                if (iVar5 == 4) { puVar6 = fcn.0806e66c(); }
                else            { puVar6 = fcn.0806e608(); }
                goto code_r0x08047836;
            }
            ...
```

`fcn.0806e5a4` конструирует объект по `0x2000059c`, прописывая методы vtable в `[obj+4]`: `+0xc, +0x10, +0x14, +0x2c`. Метод `[+0x2c] = 0x806efad` (thumb `0x0806efac`) — это BNO055-reconfig (`fcn.0806ed94`, тот же, что дёргает `fcn.0802a3d0`). Значит модель `0x50dc` → BNO055 ⓗ.

**Вывод: активный датчик определяется кодом модели платы, читаемым в рантайме — статически однозначно НЕ фиксируется (ⓗ). Полностью реализован и увязан с пайплайном наклона именно BNO055 (snk_v13); TDK ICM-42688 присутствует как альтернатива для других моделей.**

---

## fcn.0804e754 — аксессор объекта активного драйвера ✔

```c
// return *(0x2000059c + 4);   (указатель на объект драйвера с vtable)
```
Соседние stub-геттеры (тот же приём — достать объект и tail-call по vtable):
- `0x0804e760`: `obj = [0x2000059c+4]; tailcall obj->[+0x30]`
- `0x0804e77c`: `obj = [0x2000059c+4]; tailcall obj->[+0x10]`
- `0x0804e1f0`: `return (int8_t)*([[0x2000059c]] + 0x18)` — читает байт статуса, не угол.

---

## fcn.0803a198 — периодический service-update MEMS ⓗ (имя гипотеза)

Начало по `push {r2,r3,r4,r5,r6,lr}` @ `0x0803a198`; вызывает `fcn.0804e754` (драйвер), `fcn.0802b868` (усреднение), `fcn.08039fc4` (наклон); рядом строка `reset mems failed` @ `0x0803a2af` и `mems service error`-класс лог.

```c
void fcn.0803a198(uint param_1,uint param_2,uint param_3,uint param_4)
{
    ...
    iVar6 = *0x803a2ac;                 // = 0x2000039c
    iVar2 = *(*0x803a2ac + 4);          // obj = [0x2000039c+4]
    if (*(iVar2 + 8) != '\0') { return; }
    if (*(iVar2 + 0x48) != -1) {                 // ветка (ре)конфигурации датчика
        iVar2 = fcn.0804e754();
        iVar2 = (**(iVar2 + 0x2c))();            // vtable[+0x2c]: проверка/reconfig (BNO055)
        if (iVar2 == 0) { ... *(iVar6+4)[0x48] = 0xff; }   // успех
        else { ... счётчик до 5, потом флаг ошибки 4->6 (mems service error) }
        return;
    }
    if (*(iVar2 + 0x46) == '\x01') {             // датчик валиден
        iVar2 = fcn.0804e754();
        puVar5 = (**(iVar2 + 8))();              // vtable[+8]: получить euler-буфер
        uVar1 = *(puVar5 + 1);                   // halfword @ +2  -> pitch
        iVar2 = *(iVar6 + 4);
        // heading = нормализация в [0..359]:  (0x168 - euler[0]) + offset[obj+0x3e]
        for (param_3 = (0x168 - *puVar5) + *(iVar2 + 0x3e); 0x167 < param_3; param_3 -= 0x168) {}
        for (; param_3 < 0; param_3 += 0x168) {}
        *(iVar2 + 0x18) = param_3;               // -> heading
        *(iVar2 + 0x1c) = uVar1;                 // -> pitch
    }
    else {                                       // датчик невалиден: тестовые дефолты
        *(iVar2 + 0x18) = 1;                     // heading=1
        *(iVar2 + 0x1c) = 0xb3;                  // pitch=179
        *(iVar2 + 0x1a) = 0;                     // roll=0
    }
    fcn.0802b868(*(iVar6 + 4) + 0x18);           // усреднение (передаёт &[heading,roll,pitch])
    fcn.08039fc4(*(iVar6 + 4) + 0x18);           // slope-классификатор
    iVar2 = fcn.0804e754();
    (**(iVar2 + 0x24))(0);                        // vtable[+0x24]: ack/clear
    // затем до 3 колбэков-подписчиков из [obj + 0xc + i*4]
    // и vtable[+0x14]() -> статус в [obj+0x45]
}
```

Разбор ⓗ/✔:
- Мгновенные углы кладутся в `obj = [0x2000039c+4]`: `heading→[+0x18]`, `roll→[+0x1a]`, `pitch→[+0x1c]` ✔ (совпадает с валидатором и усреднением ниже).
- Heading пересчитывается через `0x168 = 360` c калибровочным смещением `[obj+0x3e]` и заворачивается в диапазон `0..359` ✔. Значит heading хранится в **целых градусах** ✔.
- `puVar5` — euler-буфер драйвера: слово 0 = сырое heading, halfword +2 = pitch. Roll (`+0x1a`) в этой ветке отдельным полем не переписывается из буфера в приведённом коде — берётся драйвером/усреднением (ⓗ, требует уточнения по vtable[+8]).
- Ветка «датчик невалиден» пишет фиктивные `pitch=179, heading=1, roll=0` — это индикатор сбоя, не реальные данные ⓗ.

---

## fcn.0802b868 — calculate_mems_average_value ✔

Владелец строк `calculate_mems_average_value` (`0x08087811`) и `set_heading_value` (`0x08087845`) — грузятся из пула `*0x0802bb80`. Вход `param_1` = `&[heading, roll, pitch]` (три `int16_t`: `param_1[0]`=heading, `param_1[1]`=roll, `param_1[2]`=pitch).

```c
void fcn.0802b868(int16_t *param_1)
{
    ...
    iVar4 = *0x802bb3c;                 // = 0x2000039c
    iVar2 = *(*0x802bb3c + 4);          // obj
    fVar9 = *(iVar2 + 0x24);           // float-аккумулятор pitch
    iVar8 = param_1[2];               // мгновенный pitch
    // усреднение pitch с обработкой перехода через ±180 (константы 90.0, 360.0):
    ...  fVar9 = ((fVar10 + fVar9 * 24.0) - 360.0) / 25.0;  // взвеш. 24:1 (~96% старого)
    *(iVar2 + 0x24) = fVar9;                 // float avg pitch
    iVar5 = fVar9 + 0.5;
    *(iVar2 + 0x22) = iVar5;                 // int16 avg pitch  -> [obj+0x22]
    ...
    // roll: без усреднения, просто копия мгновенного
    *(iVar8 + 0x28) = (float)param_1[1];     // float roll        -> [obj+0x28]
    *(iVar8 + 0x20) = param_1[1];            // int16 avg roll   -> [obj+0x20]
    // heading: простое среднее двух отсчётов (*0.5) c заворотом через 360:
    fVar9 = (fVar9 + (float)*param_1) * 0.5;
    *(iVar8 + 0x2c) = fVar9;                 // float avg heading -> [obj+0x2c]
    *(iVar8 + 0x1e) = (int16_t)(fVar9+0.5);  // int16 avg heading -> [obj+0x1e]
}
```

Доказательства/разбор:
- `*0x802bb3c = 0x2000039c` ✔ (проверено `pxw`), `*0x802bb4c=90.0`, `*0x802bb50=360.0`, `*0x802bb40=-90.0`, `*0x802bbbc=360.0` — пороги заворота и предел градусов ✔.
- Усреднение: **pitch** — экспоненциальное 24:1 (сильное сглаживание) ✔; **heading** — среднее двух `(old+new)/2` ✔; **roll** — БЕЗ усреднения, avg = мгновенное ✔.
- Все результаты округляются `+0.5` в `int16` → усреднённые значения тоже в **целых градусах** ✔.
- Записывает `[obj+0x1e]` avgHeading, `[obj+0x20]` avgRoll, `[obj+0x22]` avgPitch, плюс float-теневые `+0x2c/+0x28/+0x24` ✔.

---

## fcn.08039fc4 — slope-классификатор (service_slope) ✔ — heading НЕ используется

Вызывается из `fcn.0803a198` c `param_1 = obj+0x18`. Владеет логами `robot updown, pitch=%d, roll=%d` (`0x08039cb4`), `robot slope over` (`0x08039ce4`).

```c
void fcn.08039fc4(int param_1)
{
    ...
    iVar2 = *(param_1 + 4);            // offset 0x1c -> PITCH
    iVar1 = *(*0x803a194 + 4);         // конфиг порогов наклона
    if (((iVar2 < *(iVar1 + 0x43)) || (0xb4 < iVar2)) && (0xf < iVar2 + 0xb4U)) {
        ...
        iVar3 = *(param_1 + 2);        // offset 0x1a -> ROLL
        // серия сравнений pitch(iVar2) и roll(iVar3) с порогами [iVar1+0x40..0x44],
        // с гистерезисом-счётчиками -> состояние наклона *(iVar1+4) = 1/2/3/5
        ...
    }
}
```

Доказательство ✔: функция читает **только** `param_1+4` (pitch @ 0x1c) и `param_1+2` (roll @ 0x1a). Поле `param_1+0` (heading @ 0x18) в теле классификатора **не читается**. Классифицирует наклон в состояния 1/2/3/5 по порогам pitch/roll с гистерезисом (счётчики `[iVar1+0x30..0x3a]`). Это и есть детект SLOPE, используемый в recover.
(листинг выше — начало функции; тело длинное, механика сравнений одинаковая по веткам.)

---

## fcn.08028c70 — check_slope_value_correct (валидатор диапазонов) ✔

Владелец строк `mems pitch/roll/heading value=%d error` (`0x08028d67/dc0/ddc`) и `mems avg pitch/roll/heading value=%d error` (`0x08028df8/e18/e38`). Файл `service_slope.c` (строка `check_slope_value_correct` в пуле).

```c
void fcn.08028c70(void)
{
    int iVar1; int iVar2;
    iVar2 = *0x8028d64;                       // = 0x2000039c
    iVar1 = *(*(*0x8028d64 + 4) + 0x1c);      // pitch
    if (0x168 < iVar1 + 0xb4U)   fcn.08048adc(...,0x8028d68,iVar1);   // |pitch| > 180
    iVar1 = *(*(iVar2 + 4) + 0x1a);           // roll
    if (0xb4  < iVar1 + 0x5aU)   fcn.08048adc(...,0x8028dc0,iVar1);   // |roll|  > 90
    iVar1 = *(*(iVar2 + 4) + 0x18);           // heading
    if (0x2d0 < iVar1 + 0x168U)  fcn.08048adc(...,0x8028ddc,iVar1);   // |head|  > 360
    iVar1 = *(*(iVar2 + 4) + 0x22);           // avg pitch  (>180)
    ...  0x8028df8
    iVar1 = *(*(iVar2 + 4) + 0x20);           // avg roll   (>90)
    ...  0x8028e18
    iVar2 = *(*(iVar2 + 4) + 0x1e);           // avg heading(>360)
    ...  0x8028e38
    return;
}
```

Доказательства/разбор ✔:
- `*0x8028d64 = 0x2000039c`; поля читаются из `[+4]`: `0x18 heading`, `0x1a roll`, `0x1c pitch`, `0x1e avgHeading`, `0x20 avgRoll`, `0x22 avgPitch` ✔ — согласовано со всеми функциями выше.
- Пороги валидности задают единицы = **целые градусы** ✔: pitch/avgPitch `±180` (`0xb4`), roll/avgRoll `±90` (`0x5a`), heading/avgHeading `±360` (`0x168`).

---

## fcn.08060f24 — декодер флагов ошибок MEMS-сервиса ⓗ

Строка `mems service error` (`0x0806145c`) выводится на бите `param_1 << 0xc` (строка 0x736). Функция — большой bit-decode: `param_1` (битовая маска ошибок) → для каждого бита свой лог `fcn.08048adc(sev,...)`. Диагностика/телеметрия, не влияет на углы.

```c
void fcn.08060f24(uint param_1)
{
    if ((param_1 & 1) != 0) fcn.08048adc(1,...,0x806126c);
    ...
    if (param_1 << 0xc < 0)  fcn.08048adc(1,...,0x806145c);   // "mems service error"
    ...
    if (param_1 < 0)         fcn.08048adc(1,...,0x8061588);
}
```

---

## Карта RAM (обработанные углы MEMS)

Указатель service-объекта: `0x2000039c`; данные в `obj = *(0x2000039c + 4)`.

| Смещение в obj | Поле | Тип | Единицы ✔ | Кто пишет |
|---|---|---|---|---|
| `+0x18` | heading (мгновенный) | int16 | градусы, 0..359 | `fcn.0803a198` |
| `+0x1a` | roll (мгновенный) | int16 | градусы, ±90 | драйвер/`fcn.0803a198` |
| `+0x1c` | pitch (мгновенный) | int16 | градусы, ±180 | `fcn.0803a198` |
| `+0x1e` | avg heading | int16 | градусы, ±360 | `fcn.0802b868` |
| `+0x20` | avg roll | int16 | градусы, ±90 | `fcn.0802b868` (=мгновенный) |
| `+0x22` | avg pitch | int16 | градусы, ±180 | `fcn.0802b868` (сглаж. 24:1) |
| `+0x24` | avg pitch (float shadow) | float | градусы | `fcn.0802b868` |
| `+0x28` | roll (float shadow) | float | градусы | `fcn.0802b868` |
| `+0x2c` | avg heading (float shadow) | float | градусы | `fcn.0802b868` |
| `+0x3e` | калибр. смещение heading | int16 | градусы | конфиг |
| `+0x46` | «датчик валиден» флаг | u8 | — | сервис |
| `+0x48` | состояние (ре)конфигурации | u8 | — | сервис |

Драйверный объект: `0x2000059c`, `[+4]` = объект с vtable активного драйвера (методы `+8` read-euler, `+0x14` статус, `+0x24` ack, `+0x2c` reconfig/проверка).

Единицы ✔: **все pitch/roll/heading, хранимые в RAM, — целые градусы** (доказано порогами `0x168=360 / 0xb4=180 / 0x5a=90` и заворотом через 360.0). Сырьё сенсора BNO055 euler = 1/16° — пересчёт в градусы делается внутри драйвера до записи в `obj` (ⓗ, конкретный код деления на 16 не выделен).

---

## Курс (heading) в обратной связи разворота

- **SLOPE-детект (`fcn.08039fc4`) heading НЕ использует — только pitch(0x1c)+roll(0x1a)** ✔. Это доказывает, что триггер наклона и связанное с ним поведение в recover не завязаны на курс.
- Heading при этом активно используется как **навигационный** вход: логи `record map ... heading=%d`, `cycle in border, heading=%d`, `back find on border ... heading=%d`, строка-геттер `set_heading_value`. То есть курс — не «мёртвое» поле.
- Участвует ли heading в **замкнутой обратной связи самого физического разворота/реверса** в service_recover — по декомпилированным здесь функциям НЕ подтверждено. Оставляю `ⓗ`: разворот в recover запускается по состоянию наклона (pitch/roll), heading идёт в карту/навигацию; закрывает ли он контур поворота — проверить в recover.c/service_recover.

---

## Открытые ⓗ

1. Какой датчик физически распаян на конкретном юните — зависит от кода модели платы `(**(iVar3+0x24))()` в `fcn.080477c4`; статически = ⓗ. Полный slope-пайплайн реализован под BNO055 (snk_v13) ✔.
2. Точный источник `roll` (`obj+0x1a`) в валидной ветке `fcn.0803a198` — берётся ли он из euler-буфера драйвера (vtable[+8]) или пишется отдельным геттером. ⓗ.
3. Пересчёт сырых 1/16° BNO055 → целые градусы: где именно делится на 16 (внутри vtable[+8]/ISR драйвера). ⓗ.
4. Сигнатуры I2C-примитивов `fcn.08062980` (read), `fcn.0806299a` (write), `fcn.08083610` (delay ms) — по контексту вызовов, не по строкам. ⓗ.
5. Замкнут ли контур физического разворота/реверса по heading в service_recover. ⓗ (в slope-детекте — точно нет ✔).
6. Значение калибровочного смещения heading `[obj+0x3e]` и где оно задаётся/сохраняется. ⓗ.
