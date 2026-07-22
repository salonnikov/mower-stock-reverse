# Подсистема НОЖ (blade / cutting motor) — декомпиляция

Прошивка chip1, GD32F305 (Cortex-M4, Thumb-2). Дамп: `/opt/mower/dist/gd32-mainboard-dump-v1.bin`.
Виртуальный адрес = fileoffset + `0x08000000`. Загрузка проекта r2:
`sudo docker run --rm -u root -v /opt/mower:/work -w /work radare/radare2 r2 -q -p /work/tools/r2out/gd32.r2proj -c '...'`.

Обозначения: `✔` — подтверждено строкой/дизасмом; `ⓗ` — гипотеза, проверить на живом чипе; verbatim-блоки — механический вывод Ghidra-декомпилятора (`pdg`) / дизасма (`pdf`), доверяемо.

Исходные модули (из строк-путей в бинаре):
- `..\src\framework\service_blade\service_blade.c` (сервис ножа)
- `..\src\platform\driver\driver_motor\driver_blade_motor_snk_v2.c` (драйвер мотора ножа, вариант SNK v2)
- `..\src\app\process\process_cutting.c` (процесс кошения / регулировка скорости)

---

## fcn.08022938 — force_blade forward/backward (service_blade.c) ✔

Верхняя сервис-функция «принудительно крутить нож вперёд/назад с заданной скоростью».

```c
void fcn.08022938(void)

{
    ushort uVar1;
    int iVar2;
    uint uVar3;
    int iVar4;
    int iVar5;
    uint uVar6;
    code *pcVar7;

    iVar2 = *0x8022a00;
    iVar4 = *(*0x8022a00 + 4);
    if ((*(iVar4 + 4) != 0) && (*(iVar4 + 4) != 1)) {
        return;
    }
    if (*(iVar4 + 0x26) == '\x01') {
        if (*(iVar4 + 0x25) == '\0') {
            (**(*(iVar4 + 0x10) + 0x14))(*(iVar4 + 8));
            iVar4 = *(iVar2 + 4);
            uVar6 = 0x8022a04;
            uVar3 = 0x21b;
            *(iVar4 + 0x27) = 0;
            uVar1 = *(iVar4 + 8);
        }
        else {
            if (*(iVar4 + 0x25) != '\x01') {
                iVar4 = fcn.0804cd64();
                iVar5 = *(*(iVar2 + 4) + 0x10);
                if (iVar4 == 0) {
                    (**(iVar5 + 0x14))(*(*(iVar2 + 4) + 8));
                    *(*(iVar2 + 4) + 0x27) = 0;
                }
                else {
                    (**(iVar5 + 0x18))();
                    *(*(iVar2 + 4) + 0x27) = 1;
                }
                goto code_r0x080229f8;
            }
            (**(*(iVar4 + 0x10) + 0x18))(*(iVar4 + 8));
            iVar4 = *(iVar2 + 4);
            uVar6 = 0x8022a24;
            uVar3 = 0x21f;
            *(iVar4 + 0x27) = 1;
            uVar1 = *(iVar4 + 8);
        }
        fcn.08048adc(3,0x8022a78,0x8022a48,*0x8022a44,uVar3,uVar6,uVar1);
        goto code_r0x080229f8;
    }
    if (*(iVar4 + 0x28) == '\0') {
        *(iVar4 + 0x28) = 0xff;
        if (*(iVar4 + 0x27) != '\0') {
            *(iVar4 + 0x27) = 0;
            goto code_r0x080229e4;
        }
        *(iVar4 + 0x27) = 1;
code_r0x080229f0:
        uVar1 = *(iVar4 + 8);
        pcVar7 = *(*(iVar4 + 0x10) + 0x18);
    }
    else {
        if (*(iVar4 + 0x27) != '\0') goto code_r0x080229f0;
code_r0x080229e4:
        uVar1 = *(iVar4 + 8);
        pcVar7 = *(*(iVar4 + 0x10) + 0x14);
    }
    (*pcVar7)(uVar1);
code_r0x080229f8:
    *(*(iVar2 + 4) + 0x30) = 0;
    return;
}
```

Доказательства:
- Лог-строки: `0x08022a04` → " force blade foreward, speed=%d" (@0x08022a03), `0x08022a24` → "force blade backward, speed=%d". Путь `0x08022a48` = "…\service_blade\service_blade.c", тег `0x08022a78` = "blade". Номера строк исходника: `0x21b`=539 (fwd), `0x21f`=543 (bwd).
- `fcn.08048adc(level, path, tag, hash, line, fmt, ...)` — общий логгер (уровень 3 = info).

Интерпретация ⓗ:
- `*0x8022a00` — указатель на объект сервиса ножа; поле `+4` = «private data» (blade context), далее в нём: `+8` = **заданная скорость** (передаётся в драйвер), `+0x10` = **vtable драйвера мотора** (ops), `+0x25` = запрошенное направление (0=вперёд, 1=назад), `+0x26` = флаг «force-режим», `+0x27` = текущее направление, `+0x28` = флаг «нужна инверсия/реверс». ⓗ (по смещениям).
- **Точка управления:** `ops+0x14` = `set_forward(speed)`, `ops+0x18` = `set_backward(speed)` — это и есть driver-обёртки «крутить вперёд/назад». Скорость берётся из `ctx+8`. ✔ (вызовы + лог-строки со `speed=%d`).

---

## fcn.08022a80 — blade_direction (driver_blade_motor_snk_v2.c) ✔

Драйвер задания направления вращения ножа; логирует "set blade direction failed".

```c
void fcn.08022a80(int param_1,uint param_2,uint param_3,uint param_4)

{
    int *piVar1;
    int iVar2;
    uint uVar3;

    piVar1 = *0x8022ae4;
    iVar2 = **0x8022ae4;
    if (*(iVar2 + 0xc) == 0) {
        if (param_1 != 1) {
            return;
        }
        iVar2 = fcn.0801f4c6(iVar2 + 0x20,iVar2 + 0x28,1,0,param_3,param_4);
        if (iVar2 != 0) {
            fcn.08048adc(2,0x8022b48,0x8022b08,*0x8022b04,0x134,0x8022ae8);
        }
        uVar3 = 1;
    }
    else {
        if (*(iVar2 + 0xc) != 1) {
            return;
        }
        if (param_1 != 0) {
            return;
        }
        iVar2 = fcn.0801f4c6(iVar2 + 0x20,iVar2 + 0x28,0,1,param_3,param_4);
        if (iVar2 != 0) {
            fcn.08048adc(2,0x8022b48,0x8022b08,*0x8022b04,0x139,0x8022ae8);
        }
        uVar3 = 0;
    }
    *(*piVar1 + 0xc) = uVar3;
    return;
}
```

Доказательства:
- `0x08022ae8` = "set blade direction failed" (уровень лога 2 = warning), путь `0x08022b08` = "…driver_blade_motor_snk_v2.c", строки исходника `0x134`=308 / `0x139`=313.
- Символ функции: "blade_direction" (строка @ `0x08088770`, таблица имён драйвера).

Интерпретация ⓗ:
- `*0x8022ae4` → private-data драйвера SNK v2; `+0xc` = **текущее направление** (0/1), меняется только если новое ≠ текущего (защита от дребезга). `param_1` = запрошенное направление.
- Реальная запись направления делает `fcn.0801f4c6(handle=+0x20, ctrl_reg=+0x28, dir_bit, …)` — см. ниже (бит направления в управляющем регистре внешнего контроллера мотора). ✔

---

## fcn.080239c8 — brake_blade_motor_snk_v2 (торможение) ✔

Драйвер торможения ножа; логирует "set blade brake failed".

```c
void fcn.080239c8(uint param_1,uint param_2,uint param_3,uint param_4)

{
    int *piVar1;
    int iVar2;

    piVar1 = *0x8023a0c;
    if (*(**0x8023a0c + 8) == '\0') {
        fcn.0807f696(*0x8023a10,1,0,param_4,param_3,param_4);
        iVar2 = *piVar1;
        iVar2 = fcn.0801f4b8(iVar2 + 0x20,iVar2 + 0x28,1);
        if (iVar2 != 0) {
            fcn.08048adc(2,0x8023a70,0x8023a30,*0x8023a2c,0x110,0x8023a14);
        }
        *(*piVar1 + 8) = 1;
    }
    return;
}
```

Дизасм ключевого места (verbatim):

```
0x080239c8      push {r2, r3, r4, lr}
0x080239ca      ldr r4, [0x08023a0c]        ; [0x8023a0c]=0x20000540  (drv priv data)
0x080239ce      ldrb r0, [r0, 8]            ; brake_flag
0x080239d2      bne 0x8023a0a               ; уже заторможен -> выход
0x080239d8      ldr r0, [0x08023a10]        ; [0x8023a10]=0x40000400  (TIMER2 base)
0x080239da      bl fcn.0807f696             ; TIMER2 CH1CV = 0  (PWM duty -> 0)
0x080239e8      bl fcn.0801f4b8             ; сброс бит скорости в ctrl-регистре (brake)
0x080239ee      adr r1, "set blade brake failed"
0x08023a00      bl fcn.08048adc             ; лог при ошибке
0x08023a08      strb r0, [r1, 8]            ; brake_flag = 1
```

Доказательства:
- `0x08023a14` = "set blade brake failed" (уровень 2), путь `0x08023a30` = "…driver_blade_motor_snk_v2.c", символ `0x08023a2c` → "brake_blade_motor_snk_v2" (@0x08088757), строка исходника `0x110`=272.
- `[0x08023a10]` = `0x40000400` — база **TIMER2** ⓗ (GD32F30x: TIMER2 = 0x40000400).

Интерпретация ⓗ:
- Торможение = два действия: (1) `fcn.0807f696(TIMER2, ch=1, 0)` — обнулить PWM-заполнение канала (offset +0x38 = CH1CV) → снять привод; (2) `fcn.0801f4b8(handle, ctrl_reg, 1)` — сбросить биты скорости (маска `0xfffffe0f`, биты 4–8) в управляющем регистре внешнего контроллера → тормоз. `priv+8` = флаг «заторможен».
- **Идемпотентно:** повторный вызов при уже установленном `brake_flag` ничего не делает.

---

## fcn.08023994 — enable/release brake (driver SNK v2) ⓗ

Соседняя driver-функция (символ "release"/enable ⓗ): включает выход через GPIO + таймер. Вызывается перед стартом ножа.

```c
void fcn.08023994(void)

{
    int *piVar1;

    piVar1 = *0x80239bc;
    if (*(**0x80239bc + 4) == '\0') {
        fcn.080534fc(*0x80239c0,0x8000);
        fcn.0807f696(*0x80239c4,1,0);
        *(*piVar1 + 4) = 1;
    }
    return;
}
```

Доказательства (дизасм): `[0x080239c0]` = `0x40010c00` (**GPIOC** база), `fcn.080534fc(port, 0x8000)` пишет в `port+0x10` = **BOP** (set bit) → выставляет **GPIOC pin15 = 1**. `[0x080239c4]` = `0x40000400` (TIMER2), `fcn.0807f696(TIMER2,1,0)`. Флаг `priv+4`.

Интерпретация ⓗ: включение силового каскада ножа (enable-пин на GPIOC15 + сброс канала таймера). Парная к brake. Проверить назначение GPIOC15 на живом чипе.

---

## fcn.0801f4c6 — low-level: запись НАПРАВЛЕНИЯ в ctrl-регистр внешнего контроллера ✔

```c
uint fcn.0801f4c6(uint param_1,int param_2,int param_3)

{
    int iVar1;
    uint32_t uVar2;
    uint16_t uVar3;
    uint8_t uVar4;
    uint32_t uStack_20;

    if (param_3 == 0) {
        uVar3 = *(param_2 + 0xe) & 0xfffd;   // сброс бита 1 (dir)
    }
    else {
        uVar3 = *(param_2 + 0xe) | 2;        // установка бита 1 (dir)
    }
    *(param_2 + 0xe) = uVar3;
    uVar2 = uVar3;
    uStack_20 = 0;
    uVar4 = 0;
    while (((iVar1 = fcn.080840d8(param_1,uVar2), iVar1 != 0 ||
            (iVar1 = fcn.0806caac(param_1,uVar2 & 0xe000,&uStack_20), iVar1 != 0)) ||
           ((uVar2 & 0xfff) != (uStack_20 & 0xffff)))) {
        uVar4 = uVar4 + 1;
        if (0x31 < uVar4) {
            return 0xffffffff;   // 49 попыток -> ошибка
        }
    }
    return 0;
}
```

Интерпретация ⓗ:
- `param_2+0xe` — теневая копия 16-битного управляющего регистра мотор-контроллера. **Бит 1 (0x2) = направление вращения**. ✔ (по коду dir-драйвера).
- `fcn.080840d8` = запись регистра по последовательной шине (см. ниже), `fcn.0806caac` = чтение-обратно; цикл — write→read-verify с ретраями (до 0x31=49). Возврат `0xffffffff` = "set blade direction failed".

---

## fcn.0801f4b8 — low-level: сброс битов СКОРОСТИ (brake) в ctrl-регистре ✔

```c
uint fcn.0801f4b8(uint param_1,int param_2,uint param_3,uint32_t param_4)

{
    int iVar1;
    uint32_t uVar2;
    uint8_t uVar3;
    uint32_t uStack_20;

    uVar2 = *(param_2 + 0xe);
    *(param_2 + 0xe) = uVar2 & 0xfffffe0f;   // сброс битов 4..8 (0x1f0) = скорость -> 0
    uStack_20 = param_4 & 0xffff0000;
    uVar3 = 0;
    while (((iVar1 = fcn.080840d8(param_1,uVar2 & 0xfffffe0f), iVar1 != 0 ||
            (iVar1 = fcn.0806caac(param_1,uVar2 & 0xe000,&uStack_20), iVar1 != 0)) ||
           ((uVar2 & 0xe0f) != (uStack_20 & 0xffff)))) {
        uVar3 = uVar3 + 1;
        if (0x31 < uVar3) {
            return 0xffffffff;
        }
    }
    return 0;
}
```

Интерпретация ⓗ: биты 4..8 (`0x1f0`) управляющего регистра = **уставка скорости**; торможение = обнуление этих битов + write-verify. Возврат `0xffffffff` = "set blade brake failed". ✔

---

## fcn.0807f696 — low-level: запись канала TIMER (PWM compare) ✔

```c
void fcn.0807f696(int param_1,int param_2,uint param_3)

{
    if (param_2 == 0) { *(param_1 + 0x34) = param_3; return; }   // CH0CV
    if (param_2 != 1) {
        if (param_2 != 2) {
            if (param_2 == 3) { *(param_1 + 0x40) = param_3; }   // CH3CV
            return;
        }
        *(param_1 + 0x3c) = param_3; return;                     // CH2CV
    }
    *(param_1 + 0x38) = param_3; return;                         // CH1CV
}
```

Интерпретация ✔/ⓗ: `set_timer_channel_compare(timer_base, channel, value)`. Смещения `0x34/0x38/0x3c/0x40` = CH0CV/CH1CV/CH2CV/CH3CV регистры сравнения GD32-таймера (значение = PWM-заполнение). База `0x40000400` = TIMER2 ⓗ. brake использует канал 1 → PWM ножа = **TIMER2_CH1** ⓗ (проверить на чипе).

---

## fcn.080840d8 — low-level: запись регистра по последовательной шине ⓗ

```c
uint fcn.080840d8(uint *param_1,uint32_t param_2)

{
    int iVar1;
    uint uVar2;
    int iVar3;
    int iVar4;
    int iVar5;

    iVar1 = *0x8084164;
    iVar5 = *0x8084160;
    fcn.080843f4(*(*0x8084164 + 4),0,0xffffffff);   // lock (mutex/крит.секция)
    (**param_1)();                                  // assert CS / start
    uVar2 = *0x8084168;                             // база периферии
    iVar4 = iVar5;
    while (iVar3 = fcn.0807ddd8(uVar2,2), iVar3 == 0) {   // ждать флаг готовности
        if (iVar4 == 0) goto code_r0x08084150;
        iVar4 = iVar4 + -1;
    }
    if (iVar4 != 0) {
        fcn.0807dd98(uVar2,param_2 | 0x1000);       // записать данные (+ управляющий бит 0x1000)
        while (iVar4 = fcn.0807ddd8(uVar2,1), iVar4 == 0) {
            if (iVar5 == 0) goto code_r0x08084150;
            iVar5 = iVar5 + -1;
        }
        if (iVar5 != 0) {
            fcn.0807dd92(uVar2);                     // завершить
            (*param_1[1])();                         // deassert CS / stop
            fcn.08084644(*(iVar1 + 4),0,0);          // unlock
            return 0;
        }
    }
code_r0x08084150:
    fcn.08084644(*(iVar1 + 4),0,0);
    return 0xffffffff;
}
```

Интерпретация ⓗ: обмен с внешним контроллером мотора ножа по последовательной шине (CS assert/deassert через `param_1[0]/[1]`, poll статус-флагов, mutex-lock). Тип шины (SPI/I2C/UART) точно **не установлено** — проверить по инициализации периферии `*0x8084168` на живом чипе.

---

## fcn.0802fa90 — машина запуска ножа "start blade" (service_blade.c) ✔

Обрабатывает попытку запуска ножа с проверкой всех блокировок (станция, подъём/lift, наклон/updown, удар/hit, граница/border, застревание/stall).

```c
void fcn.0802fa90(void)

{
    int16_t iVar1;
    int iVar2;
    uint32_t uVar3;
    uint uVar4;
    uint uVar5;
    int iVar6;
    bool bVar7;
    uint uVar8;

    iVar6 = *0x802fd08;
    if (*(*(*0x802fd08 + 4) + 0x161) == '\x01') {
        fcn.08077e10(3);
        uVar8 = 0x802fd50;
        *(*(iVar6 + 4) + 0x148) = 0;
        uVar5 = 0xbe6;
    }
    else {
        iVar2 = fcn.08050c6c();
        iVar2 = (**(iVar2 + 0xc))();
        if (iVar2 == 0) {
            fcn.0802bce4(0x4fffffff);
            uVar8 = 0x802fd70;
            uVar5 = 0xbec;
code_r0x0802fb70:
            uVar4 = 2;
            goto code_r0x0802fc62;
        }
        iVar2 = *(*(iVar6 + 4) + 0x24);
        if (*(iVar2 + 0x34) == 1) {
            fcn.08077e10(3);
            (**(*(*(iVar6 + 4) + 0x10) + 0x34))();
            iVar6 = *(iVar6 + 4);
            *(iVar6 + 0x146) = 0;
            *(iVar6 + 0x138) = 2;
            uVar8 = 0x802fd9c;
            uVar5 = 0xbf5;
        }
        else {
            if (*(iVar2 + 0x3c) == 6) {
                fcn.0802bce4(1);
                uVar8 = 0x802fde0;
                uVar5 = 0xbfb;
                goto code_r0x0802fb70;
            }
            if (*(iVar2 + 0x38) == 3) {
                fcn.08077e10(3);
                (**(*(*(iVar6 + 4) + 0x10) + 0x34))();
                iVar6 = *(iVar6 + 4);
                *(iVar6 + 0x146) = 0;
                *(iVar6 + 0x138) = 1;
                uVar8 = 0x802fe14;
                uVar5 = 0xc04;
            }
            else {
                iVar2 = fcn.0805deb8();
                if (iVar2 != 0) {
                    iVar2 = *(*(*(iVar6 + 4) + 0x24) + 0x3c);
                    if (((iVar2 == 4) || (iVar2 == 2)) || (iVar2 == 5)) {
                        fcn.08077e10(3);
                        (**(*(*(iVar6 + 4) + 0x10) + 0x34))();
                        iVar2 = *(iVar6 + 4);
                        *(iVar2 + 0x146) = 0;
                        *(iVar2 + 0x138) = 3;
                        iVar2 = fcn.08051f2c();
                        iVar2 = (**(iVar2 + 0x28))();
                        iVar1 = *(iVar2 + 2);
                        iVar2 = fcn.08051f2c();
                        iVar2 = (**(iVar2 + 0x28))();
                        fcn.08048adc(3,0x802fdd8,0x802fd28,*0x802fd24,0xc18,*0x802fe8c,
                                     *(*(*(iVar6 + 4) + 0x24) + 0x3c),*(iVar2 + 4),iVar1);
                        return;
                    }
                    uVar3 = (**(*(*(iVar6 + 4) + 0x10) + 0x14))();
                    if (uVar3 == 0) {
                        iVar6 = *(iVar6 + 4);
                        if (*(iVar6 + 0x42) < 0x14) {
                            *(iVar6 + 0x42) = *(iVar6 + 0x42) + 1;
                            return;
                        }
                        (**(*(iVar6 + 0x10) + 0x28))();
                        uVar8 = 0x802fe90;
                        uVar5 = 0xc21;
                    }
                    else {
                        if (uVar3 == 1) {
                            *(*(iVar6 + 4) + 0x42) = 0;
                            return;
                        }
                        if (uVar3 == 2) {
                            fcn.08077e10(3);
                            uVar8 = 0x802fe9c;
                            *(*(iVar6 + 4) + 0x148) = 0;
                            uVar5 = 0xc2c;
                            goto code_r0x0802fb56;
                        }
                        bVar7 = uVar3 == 3;
                        while (!bVar7) {
                            bVar7 = true;
                            if (uVar3 != 4) {
                                fcn.08077e10(3);
                                uVar8 = 0xc41;
                                uVar5 = 0x802fd0c;
code_r0x0802fca0:
                                uVar4 = 2;
code_r0x0802fd02:
                                fcn.08048adc(uVar4,0x802fdd8,0x802fd28,*0x802fd24,uVar8,uVar5,uVar3);
                                return;
                            }
                        }
                        iVar2 = *(iVar6 + 4);
                        *(iVar2 + 0x148) = *(iVar2 + 0x148) + '\x01';
                        (**(*(iVar2 + 0x10) + 0x34))();
                        uVar3 = *(*(iVar6 + 4) + 0x148);
                        if (0xe < uVar3) {
                            fcn.0802bce4(0x100000);
                            uVar3 = *(*(iVar6 + 4) + 0x148);
                            uVar8 = 0xc32;
                            uVar5 = *0x802fecc;
                            goto code_r0x0802fca0;
                        }
                        if (uVar3 % 3 == 0) {
                            fcn.08077e10(3);
                            iVar6 = *(iVar6 + 4);
                            *(iVar6 + 0x146) = 0;
                            *(iVar6 + 0x138) = 6;
                            uVar3 = *(iVar6 + 0x148);
                            uVar8 = 0xc38;
                            uVar4 = 3;
                            uVar5 = *0x802ff00;
                            goto code_r0x0802fd02;
                        }
                        (**(*(*(iVar6 + 4) + 0x10) + 0x34))();
                        (**(*(*(iVar6 + 4) + 0x10) + 0x38))();
                        uVar8 = 0x802fed0;
                        *(*(iVar6 + 4) + 0x42) = 0;
                        uVar5 = 0xc3d;
                    }
                    uVar4 = 4;
                    goto code_r0x0802fc62;
                }
                fcn.08077e10(3);
                (**(*(*(iVar6 + 4) + 0x10) + 0x34))();
                iVar6 = *(iVar6 + 4);
                *(iVar6 + 0x146) = 0;
                *(iVar6 + 0x138) = 4;
                uVar8 = 0x802fe4c;
                uVar5 = 0xc0d;
            }
        }
    }
code_r0x0802fb56:
    uVar4 = 3;
code_r0x0802fc62:
    fcn.08048adc(uVar4,0x802fdd8,0x802fd28,*0x802fd24,uVar5,uVar8);
    return;
}
```

Доказательства (лог-строки, все из service_blade.c, путь `0x802fd28`, тег/hash `0x802fd24`):
- `0x802fd0c` = "blade state error=%d" (ветка default, `uVar3` неизвестного состояния)
- `0x802fd50` = "exhibition mode, disable blade" (флаг `priv+0x161` — демо-режим)
- `0x802fd70` = "start blade but in station, change to error" (`fcn.08050c6c()->[0xc]()==0`: робот в станции)
- `0x802fd9c` = "start blade but lift, turn to step random move to escape" (`+0x34==1`: подъём/lift)
- `0x802fde0` = "start blade,but updown, brake and goto error updown" (`+0x3c==6`: наклон)
- `0x802fe14` = "start blade but hit, turn to step random move to escape" (`+0x38==3`: удар)
- `0x802fe4c` = "start blade but no border, turn to..." (`fcn.0805deb8()==0`: нет границы)
- `0x802fe90` = "start blade" / `0x802fe9c` = "start blade finish, turn to step random move" (`uVar3==2`)
- `0x802fecc` = "start blade but stall, reverse blade direction" (`uVar3==4`, застревание → реверс)

Интерпретация ⓗ:
- `uVar3 = (**(ops+0x14))()` — опрос состояния мотора ножа (0=разгон/ждать, 1=ОК, 2=finish, 3/4=stall). ✔ (ветвление + строки).
- `(**(ops+0x34))()` = **brake** (тормоз, вызывается во всех аварийных ветках), `(**(ops+0x38))()` = **reverse direction** (реверс при stall). ✔ (по строкам "brake"/"reverse blade direction").
- `+0x138` = следующее состояние машины движения (1=lift, 2=updown/error, 3=slope, 4=no-border, 6=stall-random), `+0x148` = счётчик попыток stall (после 0xe=14 попыток → `blade block`/error), `+0x42` = таймер-счётчик ожидания разгона (порог 0x14=20).

---

## fcn.080321d4 (регион 0x080301ae–0x08030234) — регулировка скорости по нагрузке (process_cutting.c) ✔

Внутри большой задачи кошения `fcn.080321d4` (0x0802cad0–0x08032a58, ~24 КБ). Регулятор выбирает профиль скорости по нагрузке ножа и наклону. Дизасм (verbatim):

```
; --- highload-детектор + выбор скорости ---
0x08030186  ldr r0, [r0, 0xc]        ; process obj
0x08030188  ldr r0, [r0, 0x20]       ; slope-состояние getter
0x0803018a  blx r0
0x0803018c  cmp r0, 3                 ; slope==normal ?
0x08030190  bl fcn.08027bf4           ; highload-детектор (см. ниже)
0x0803019a  ldr r1, [r0, 0x1c]        ; <-- SPEED SETTER (process+0xc -> +0x1c)
0x0803019c  beq 0x80301aa
0x0803019e  movs r0, 0                ; режим 0 = normal
0x080301a0  blx r1                    ; set_speed(0)
0x080301a2  adr r1, "slope normal, change to normal speed"
;--
0x080301aa  movs r0, 2                ; режим 2 = highload
0x080301ac  blx r1                    ; set_speed(2)
0x080301ae  adr r1, "slope normal but blade highload, change to highload speed"
;--
0x080301c6  movs r0, 3                ; режим 3 = slope
0x080301c8  blx r1                    ; set_speed(3)
0x080301ca  adr r1, "slope down, change to slope speed"
;--
0x08030206  ldr r1, [r0, 0x1c]
0x0803020c  movs r0, 2                ; режим 2 = highload
0x0803020e  blx r1                    ; set_speed(2)
0x08030210  adr r1, "blade highload, change to highload speed"
;--
0x08030228  ldr r1, [r0, 0x1c]
0x0803022a  movs r0, 0                ; режим 0 = normal
0x0803022c  blx r1                    ; set_speed(0)
0x0803022e  adr r1, "blade normal, change to normal speed"
0x08030216  ... bl fcn.08048adc      ; лог (путь "..\src\app\process\process_cutting.c")
```

Highload-детектор:

```c
int fcn.08027bf4(void)

{
    int iVar1;
    int iVar2;

    iVar1 = *0x8027c44;
    iVar2 = (**(*(*(*0x8027c44 + 4) + 0x10) + 0x14))();   // опрос состояния мотора
    if (iVar2 == 4) {                                     // состояние 4 = перегруз/stall
        iVar2 = *(iVar1 + 4);
        if (*(iVar2 + 0x4c) < 0x3d) {                     // счётчик до 0x3d=61
            *(iVar2 + 0x4c) = *(iVar2 + 0x4c) + 1;
        }
        else {
            *(iVar2 + 0x12e) = 0;
        }
    }
    else {
        iVar2 = *(iVar1 + 4);
        if (*(iVar2 + 0x4c) < 3) {
            *(iVar2 + 0x4c) = 0;
            *(iVar2 + 0x12e) = 0xff;                       // флаг highload
        }
        else {
            *(iVar2 + 0x4c) = *(iVar2 + 0x4c) - 2;
        }
    }
    return *(*(iVar1 + 4) + 0x12e);
}
```

Доказательства: строки `0x08030528` "slope normal, change to normal speed", `0x08030550` "slope normal but blade highload, change to highload speed", `0x0803058c` "slope down, change to slope speed", `0x080305b0` "blade highload, change to highload speed", `0x08030610` "blade normal, change to normal speed"; путь `process_cutting.c` @ `0x080305e0`.

Интерпретация ⓗ:
- **Точка регулировки скорости:** `(*(*(process+0xc) + 0x1c))(mode)` — единый сеттер профиля скорости ножа. `mode`: **0 = normal, 2 = highload, 3 = slope**. ✔ (movs r0,#; blx + лог-строки).
- `fcn.08027bf4` — гистерезисный детектор перегруза: при состоянии мотора `4` (stall/перегруз) наращивает счётчик `+0x4c`; иначе — уменьшает на 2 и при <3 сбрасывает флаг `+0x12e`. Возврат ≠0 → «нож в перегрузе» → переключение на highload-скорость. ⓗ (пороги 0x3d/3, состояние 4).

---

## Итог: точки управления ножом (куда «бить»)

Все действия идут через **vtable драйвера мотора** `ops = ctx->0x10` (сервис) и через сеттер скорости `process->0xc->0x1c` (процесс кошения):

| Действие | Точка в коде | Доказательство |
|---|---|---|
| Крутить ВПЕРЁД, скорость | `ops+0x14 (speed)` в `fcn.08022938` | "force blade foreward, speed=%d" ✔ |
| Крутить НАЗАД, скорость | `ops+0x18 (speed)` в `fcn.08022938` | "force blade backward, speed=%d" ✔ |
| Задать НАПРАВЛЕНИЕ | `fcn.08022a80` → `fcn.0801f4c6` (бит 1 ctrl-регистра) | "set blade direction failed" ✔ |
| ТОРМОЗ | `ops+0x34` → `fcn.080239c8` → TIMER2 CH1CV=0 + `fcn.0801f4b8` (сброс битов скорости) | "set blade brake failed" ✔ |
| РЕВЕРС при stall | `ops+0x38` в машине start | "reverse blade direction" ✔ |
| ВКЛ силовой каскад | `fcn.08023994` → GPIOC pin15 (BOP) + TIMER2 | дизасм GPIOC/BOP ⓗ |
| Профиль СКОРОСТИ по нагрузке | `process->0xc->0x1c (mode)`; mode 0/2/3 | "…change to normal/highload/slope speed" ✔ |
| Запуск с проверками блокировок | `fcn.0802fa90` (station/lift/updown/hit/border/stall) | серия "start blade…" ✔ |

**Как выключить нож:** вызвать тормоз `fcn.080239c8` (или `ops+0x34`) — обнуляет PWM (TIMER2_CH1) и биты скорости в управляющем регистре внешнего контроллера, ставит `brake_flag`.
**Как включить:** `fcn.08023994` (enable) → задать направление `fcn.08022a80` → задать скорость через `ops+0x14/0x18` (force) или профиль `process->0xc->0x1c`.

## Открытые ⓗ (проверить на живом чипе)

1. **База таймера `0x40000400`** — предположительно TIMER2; **канал 1 (CH1CV, offset 0x38)** — PWM ножа. Проверить осциллографом какой пин таймера идёт на затвор/enable мотора.
2. **`0x40010c00` = GPIOC, pin15** (через BOP, offset 0x10) — enable силового каскада ножа. Проверить трассировку пина.
3. **Тип шины `fcn.080840d8`/`fcn.0806caac`** к внешнему контроллеру мотора (периферия `*0x8084168`) — SPI/I2C/UART не установлено. Управляющий регистр (теневая копия по смещению `+0xe` priv-данных): **бит 1 = направление**, **биты 4..8 (0x1f0) = уставка скорости**. Проверить, что это за микросхема-контроллер BLDC.
4. Пороги highload-детектора `fcn.08027bf4` (счётчик до 0x3d=61, состояние мотора `4`=перегруз) — проверить на реальной нагрузке.
5. Смещения полей контекста (`+8` speed, `+0x25/0x26/0x27` направление/флаги, `+0x138` next-state, `+0x148` stall-счётчик, `+0x42` таймер разгона) — выведены по коду, не по символам; подтвердить трассировкой.
