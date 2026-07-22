# Привод колёс / повороты / торможение — декомпиляция (chip1, GD32F305)

Источник: `/opt/mower/dist/gd32-mainboard-dump-v1.bin`, Cortex-M4 Thumb-2.
Инструмент: Ghidra-декомпилятор через radare2 `pdg` (проект `tools/r2out/gd32.r2proj`).

Легенда:
- ✔ — подтверждено строкой/очевидным кодом.
- ⓗ — гипотеза, проверить на живом чипе.
- Блоки ```c — механический вывод `pdg`/`pdf` **verbatim** (доверяемо).
- «не установлено» — назначение неизвестно, не выдумано.

Адреса функций = авто-имена r2 (адрес входа). Регистровый мусор в некоторых
`pdg` (`unaff_rN`, `extraout_rN`, `in_r0`) — артефакт восстановления ABI, не логика.

---

## Карта: как команда доезжает до мотора колеса ✔/ⓗ

Механически прослежена цепочка (по вызовам `axt`/`bl`):

```
команда движения (follow / recover / random / angle)
   │
   ├─ fcn.08020520  angle_to_cycle_tick   (угол → тики энкодера)
   ├─ fcn.0804bb0c  drive-by-angle/ticks  (мьютекс + vtable драйвера)
   │
   ├─ fcn.0805f620  set_wheel_speed #A → расчёт PWM из скорости + передача/направление
   ├─ fcn.08020e0c  set_wheel_speed #B → расчёт PWM из скорости (второй канал)
   │        │
   │        ├─ fcn.0801f4dc  «brake to dir» — смена направления/передачи драйвера (с подтверждением)
   │        └─ fcn.0807f696  запись PWM в регистр канала мотора
```

Два независимых расчётчика PWM (`fcn.0805f620` и `fcn.08020e0c`) работают с
**разными** контекстами мотора в RAM (`0x2000054c` и указатель из `0x0805fb9c`),
оба заканчиваются `fcn.0807f696(ctx_pwm, 3, pwm)`. ⓗ это левое и правое колесо
(либо «привод» и «нож»); статически лево/право по имени не разведено — см. открытые ⓗ.

---

## fcn.08020520 — angle_to_cycle_tick (угол разворота → тики энкодера) ✔ формула

```c
uint32_t fcn.08020520(int param_1)

{
    int iVar1;
    uint32_t uVar2;
    int iVar3;
    
    iVar1 = *(*0x802057c + 4);
    iVar3 = *(iVar1 + 4);
    if (iVar3 == 0) {
        uVar2 = *(iVar1 + 0x32);
    }
    else if (iVar3 == 1) {
        uVar2 = *(iVar1 + 0x3a);
    }
    else if (iVar3 == 3) {
        uVar2 = *(iVar1 + 0x4a);
    }
    else if (iVar3 == 2) {
        uVar2 = *(iVar1 + 0x42);
    }
    else {
        if (iVar3 != 4) {
            fcn.08048adc(1,0x80205e8,0x80205b0,*0x80205ac,0x3df,0x8020580);
            return 0;
        }
        uVar2 = *(iVar1 + 0x52);
    }
    return (param_1 * 0x2b9) / uVar2;
}
```

Доказательства: последняя инструкция — `(param_1 * 0x2b9) / uVar2`. `0x2b9 = 697`.
`iVar3 = *(cfg+4)` — индекс передачи (0..4); по нему выбирается делитель из
таблицы калибровки: gear0→cfg+0x32, gear1→cfg+0x3a, gear2→cfg+0x42, gear3→cfg+0x4a,
gear4→cfg+0x52 (каждое — 2 байта, шаг 8). Ветка «иначе» логирует ошибку через
`fcn.08048adc` (общий логгер, аргументы = формат/имя-функции/файл/строка `0x3df`).

Интерпретация:
- **Формула**: `tick = angle * 697 / cal[gear]` ✔ (арифметика verbatim). `angle`
  — параметр (`param_1`), `cal[gear]` — калибровка «тиков на оборот/градус» для
  текущей передачи. ⓗ 697 — это `angle_full_scale`, физический смысл (полукруг в
  своих единицах?) проверить на чипе.
- Разомкнутый пересчёт: без обратной связи по факту, чистая арифметика. ✔
- Вызывается из `fcn.0804bb0c` (drive-by-angle) и ещё ~6 мест (0x8020c5a, 0x8020d0e,
  0x8020d76, 0x802ff84, 0x8032cf2, 0x8032d44, 0x804bb26 — `axt`). ✔

---

## fcn.0804bb0c — команда «ехать со скоростью arg1 на угол arg2» ⓗ

```c
void fcn.0804bb0c(uint noname_0,int16_t arg1,int16_t arg2)

{
    bool bVar1;
    int iVar2;
    uint uVar3;
    uint uVar4;
    int iVar5;
    uint unaff_r4;
    uint unaff_lr;
    
    uVar3 = fcn.0803c868();
    if (arg2 == -1) {
        uVar4 = 0xffffffff;
    }
    else {
        uVar4 = fcn.08020520(arg2);
    }
    fcn.0808340c();
    iVar2 = *0x804bb70;
    (**(*(*(*0x804bb70 + 4) + 0xc) + 0x10))();
    (**(*(*(iVar2 + 4) + 0xc) + 0x18))(**(*(iVar2 + 4) + 0x54),uVar3);
    (**(*(*(iVar2 + 4) + 0xc) + 0x20))(arg1,*(*(*(iVar2 + 4) + 0x54) + 6),uVar4);
    (**(*(*(iVar2 + 4) + 0xc) + 0x14))(0xffffffff);
    iVar2 = *0x808349c;
    if (*(*0x808349c + 4) == 0) {
        fcn.0801da86(0,0x80834c0,0x80834a0,0x1c0,unaff_r4,unaff_lr);
    }
    iVar5 = *(iVar2 + 4) + -1;
    *(iVar2 + 4) = iVar5;
    if ((iVar5 == 0) && (bVar1 = isCurrentModePrivileged(), bVar1)) {
        setBasePriority(0);
    }
    return;
}
```

Доказательства: при `arg2 != -1` угол пересчитывается в тики через `fcn.08020520`
(`uVar4`); затем через vtable драйвера (`*(iVar2+4)+0xc` = таблица методов)
вызываются методы по смещениям +0x10, +0x18(скорость `uVar3`), +0x20(`arg1`, поле,
тики `uVar4`), +0x14(0xffffffff). Обрамление `fcn.0803c868`/`fcn.0808340c` +
`isCurrentModePrivileged`/`setBasePriority` — вход/выход из критической секции (мьютекс).

Интерпретация:
- ⓗ команда «двигаться на дугу»: `arg1` — скорость/PWM-задание, `arg2` — угол
  (-1 = прямо, без поворота → `uVar4=0xffffffff`), метод +0x20 задаёт целевые тики.
- ⓗ метод vtable +0x18 = задать скорость, +0x20 = задать дистанцию/угол в тиках,
  +0x14 = запуск. Смещения проверить по драйверу на чипе.

---

## fcn.0805f620 — set_wheel_speed (канал A): скорость → PWM + передача/направление («brake to dir») ⓗ

Ключевая функция торможения/переключения. `arg1` — задание скорости; результат —
PWM (`uVar12`) + смена передачи/направления через `fcn.0801f4dc`. Log-строки
внутри: `"...brake to dir=%d, speed =%d over max, pwm=%d"` (@ 0x0805fa1b) и
`"...brake to dir=%d, speed=%d too low, pwm=%d..."` (@ 0x0808cfb0).

```c
void fcn.0805f620(uint noname_0,int16_t arg1)

{
    int *piVar1;
    uint16_t uVar2;
    uint in_r0;
    int iVar3;
    uint uVar4;
    uint uVar5;
    uint32_t uVar6;
    int iVar7;
    int iVar8;
    uint32_t uVar9;
    uint32_t uVar10;
    int iVar11;
    uint32_t uVar12;
    uint32_t uVar13;
    uint32_t uVar14;
    
    piVar1 = *0x805fa18;
    uVar6 = arg1;
    uVar12 = 0;
    iVar8 = **0x805fa18;
    uVar13 = uVar6 * 0x1c20;
    iVar11 = *(iVar8 + 0x10);
    iVar7 = iVar8 + 0x30;
    iVar3 = iVar8 + 0x28;
    if (*(iVar8 + 8) != '\x01') {
        uVar9 = uVar6 - 0x266;
        uVar14 = uVar6 - 0x99;
        uVar10 = uVar6 - 0x132;
        if (iVar11 == 1) {
            if (uVar9 < 0x1598) {
code_r0x0805f8ac:
                uVar2 = uVar13 / 0x17fd;
            }
            else {
                if (0x17fd < uVar6) {
code_r0x0805f976:
                    uVar12 = 0x1c20;
                    goto code_r0x0805f9cc;
                }
                if (0x265 < uVar6) goto code_r0x0805f9cc;
                if (0xacb < uVar10) {
                    if (0x564 < uVar14) {
                        iVar3 = fcn.0801f4dc(iVar3,iVar7,2);
                        if (iVar3 != 0) {
                            fcn.08048adc(2,0x805fab8,0x805fa78,*0x805fa74,0x178,0x805fa58);
                        }
                        *(*piVar1 + 0x10) = 2;
                        goto code_r0x0805f9c8;
                    }
                    iVar3 = fcn.0801f4dc(iVar3,iVar7,2);
                    if (iVar3 != 0) {
                        fcn.08048adc(2,0x805fab8,0x805fa78,*0x805fa74,0x171,0x805fa58);
                    }
                    *(*piVar1 + 0x10) = 2;
                    goto code_r0x0805f9ac;
                }
                iVar3 = fcn.0801f4dc(iVar3,iVar7,0);
                if (iVar3 != 0) {
                    uVar5 = 0x16a;
code_r0x0805fa00:
                    fcn.08048adc(2,0x805fab8,0x805fa78,*0x805fa74,uVar5,0x805fa58);
                }
code_r0x0805fa10:
                *(*piVar1 + 0x10) = 0;
code_r0x0805f93c:
                uVar2 = uVar13 / 0xbfd;
            }
        }
        else {
            if (iVar11 == 0) {
                if (uVar10 < 0xacc) goto code_r0x0805f93c;
                if (0xbfd < uVar6) {
                    iVar3 = fcn.0801f4dc(iVar3,iVar7,1);
                    if (iVar3 != 0) {
                        fcn.08048adc(2,0x805fab8,0x805fa78,*0x805fa74,0x185,0x805fa58);
                    }
                    *(*piVar1 + 0x10) = 1;
                    if (0x1597 < uVar9) goto code_r0x0805f976;
                    goto code_r0x0805f8ac;
                }
                iVar3 = fcn.0801f4dc(iVar3,iVar7,2);
                if (iVar3 != 0) {
                    fcn.08048adc(2,0x805fab8,0x805fa78,*0x805fa74,0x191,0x805fa58);
                }
                *(*piVar1 + 0x10) = 2;
                if (0x564 < uVar14) goto code_r0x0805f9c8;
            }
            else {
                if (iVar11 != 2) goto code_r0x0805f9cc;
                if (0x564 < uVar14) {
                    if (uVar6 < 0x99) {
code_r0x0805f9c8:
                        uVar12 = 0x2ce;
                        goto code_r0x0805f9cc;
                    }
                    if (uVar10 < 0xacc) {
                        iVar3 = fcn.0801f4dc(iVar3,iVar7,0);
                        if (iVar3 != 0) {
                            uVar5 = 0x1a5;
                            goto code_r0x0805fa00;
                        }
                        goto code_r0x0805fa10;
                    }
                    if (0x1597 < uVar9) {
                        iVar3 = fcn.0801f4dc(iVar3,iVar7,1);
                        if (iVar3 != 0) {
                            fcn.08048adc(2,0x805fab8,0x805fa78,*0x805fb2c,0x1b3,0x805fa58);
                        }
                        *(*piVar1 + 0x10) = 1;
                        goto code_r0x0805f976;
                    }
                    iVar3 = fcn.0801f4dc(iVar3,iVar7,1);
                    if (iVar3 != 0) {
                        fcn.08048adc(2,0x805fab8,0x805fa78,*0x805fb2c,0x1ac,0x805fa58);
                    }
                    *(*piVar1 + 0x10) = 1;
                    goto code_r0x0805f8ac;
                }
            }
code_r0x0805f9ac:
            uVar2 = uVar13 / 0x5fd;
        }
        uVar12 = uVar2;
        goto code_r0x0805f9cc;
    }
    uVar12 = uVar6 - 0x5ff;
    uVar9 = uVar6 - 0x2ff;
    uVar10 = uVar6 - 0x17f;
    if (iVar11 == 1) {
        if (uVar12 < 0x11ff) {
            uVar2 = uVar13 / 0x17fd;
            goto code_r0x0805f760;
        }
        if (0x17fd < uVar6) {
            *(iVar8 + 8) = 0;
            uVar5 = 0x805fa1c;
            uVar12 = 0x1c20;
            uVar4 = 0xfc;
code_r0x0805f740:
            fcn.08048adc(2,0x805fab8,0x805fa78,*0x805fa74,uVar4,uVar5);
            goto code_r0x0805f9cc;
        }
        if (uVar9 < 0x8ff) {
            iVar3 = fcn.0801f4dc(iVar3,iVar7,0);
            if (iVar3 != 0) {
                uVar5 = 0xff;
code_r0x0805f830:
                fcn.08048adc(2,0x805fab8,0x805fa78,*0x805fa74,uVar5,0x805fa58);
            }
code_r0x0805f840:
            uVar13 = uVar13 / 0xbfd;
            iVar3 = *piVar1;
            *(iVar3 + 0x10) = 0;
        }
        else {
            if (0x47e < uVar10) {
                iVar3 = fcn.0801f4dc(iVar3,iVar7,2);
                if (iVar3 != 0) {
                    fcn.08048adc(2,0x805fab8,0x805fa78,*0x805fa74,0x10f,0x805fa58);
                }
                uVar5 = *0x805fac0;
                iVar3 = *piVar1;
                uVar12 = 0x706;
                *(iVar3 + 0x10) = 2;
                *(iVar3 + 8) = 0;
                uVar4 = 0x114;
                goto code_r0x0805f740;
            }
            iVar3 = fcn.0801f4dc(iVar3,iVar7,2);
            if (iVar3 != 0) {
                uVar5 = 0x107;
code_r0x0805f7c0:
                fcn.08048adc(2,0x805fab8,0x805fa78,*0x805fa74,uVar5,0x805fa58);
            }
code_r0x0805f7d0:
            uVar13 = uVar13 / 0x5fd;
            iVar3 = *piVar1;
            *(iVar3 + 0x10) = 2;
        }
code_r0x0805f708:
        uVar12 = uVar13 & 0xffff;
    }
    else {
        if (iVar11 != 0) {
            if (uVar10 < 0x47f) {
                uVar2 = uVar13 / 0x5fd;
code_r0x0805f760:
                uVar12 = uVar2;
            }
            else {
                if (0x17e < uVar6) {
                    if (uVar9 < 0x8ff) {
                        iVar3 = fcn.0801f4dc(iVar3,iVar7,0);
                        if (iVar3 != 0) {
                            uVar5 = 0x147;
                            goto code_r0x0805f830;
                        }
                        goto code_r0x0805f840;
                    }
                    if (0x11fe < uVar12) {
                        iVar3 = fcn.0801f4dc(iVar3,iVar7,1);
                        if (iVar3 != 0) {
                            uVar5 = 0x157;
                            goto code_r0x0805f88e;
                        }
                        goto code_r0x0805f79c;
                    }
                    iVar3 = fcn.0801f4dc(iVar3,iVar7,1);
                    if (iVar3 != 0) {
                        uVar5 = 0x14f;
                        goto code_r0x0805f864;
                    }
code_r0x0805f874:
                    uVar13 = uVar13 / 0x17fd;
                    iVar3 = *piVar1;
                    *(iVar3 + 0x10) = 1;
                    goto code_r0x0805f708;
                }
                uVar12 = 0x706;
            }
            *(iVar8 + 8) = 0;
            goto code_r0x0805f9cc;
        }
        if (uVar9 < 0x8ff) {
            uVar2 = uVar13 / 0xbfd;
            goto code_r0x0805f760;
        }
        if (uVar12 < 0x11ff) {
            iVar3 = fcn.0801f4dc(iVar3,iVar7,1);
            if (iVar3 != 0) {
                uVar5 = 0x11d;
code_r0x0805f864:
                fcn.08048adc(2,0x805fab8,0x805fa78,*0x805fa74,uVar5,0x805fa58);
            }
            goto code_r0x0805f874;
        }
        if (uVar6 < 0x17fe) {
            if (uVar10 < 0x47f) {
                iVar3 = fcn.0801f4dc(iVar3,iVar7,2);
                if (iVar3 != 0) {
                    uVar5 = 0x12d;
                    goto code_r0x0805f7c0;
                }
                goto code_r0x0805f7d0;
            }
            iVar3 = fcn.0801f4dc(iVar3,iVar7,2);
            if (iVar3 != 0) {
                fcn.08048adc(2,0x805fab8,0x805fa78,*0x805fa74,0x135,0x805fa58);
            }
            iVar3 = *piVar1;
            uVar12 = 0x706;
            *(iVar3 + 0x10) = 2;
        }
        else {
            iVar3 = fcn.0801f4dc(iVar3,iVar7,1);
            if (iVar3 != 0) {
                uVar5 = 0x125;
code_r0x0805f88e:
                fcn.08048adc(2,0x805fab8,0x805fa78,*0x805fa74,uVar5,0x805fa58);
            }
code_r0x0805f79c:
            iVar3 = *piVar1;
            uVar12 = 0x1c20;
            *(iVar3 + 0x10) = 1;
        }
    }
    *(iVar3 + 8) = 0;
code_r0x0805f9cc:
    fcn.0805f300(in_r0);
    fcn.0807f696(*0x805fac4,3,uVar12);
    *(**0x805fa18 + 2) = uVar12;
    return;
}
```

Доказательства (verbatim):
- Контекст мотора: `piVar1 = *0x805fa18` → указатель в RAM `0x2000054c` (по литералу
  `[0x805fa18:4]=0x2000054c` из дизасма). `iVar11 = *(iVar8+0x10)` — текущая передача
  (0/1/2). `*(iVar8+8)` — флаг режима (0/1).
- `uVar13 = arg1 * 0x1c20`; `0x1c20 = 7200` — максимум PWM (видно как насыщение
  `uVar12 = 0x1c20`).
- Делители передач: `0x17fd`(6141), `0xbfd`(3069), `0x5fd`(1533), плюс минимумы
  `0x2ce`(718)/`0x706`(1798). PWM = `speed*7200 / gear_max`.
- Переключение передачи/направления: `fcn.0801f4dc(iVar3,iVar7,DIR)` с `DIR` 0/1/2,
  после чего `*(ctx+0x10) = DIR` — записывается новая передача. Строка `0x0805fa1c`
  = «max speed=6141, brake to dir=%d...» подтверждает смысл «brake to dir». ✔ строка
- Выход: `fcn.0807f696(*0x805fac4, 3, uVar12)` — запись PWM в регистр канала;
  `*(ctx + 2) = uVar12` — сохранение текущего PWM в контекст.

Интерпретация:
- ✔ (по строкам/делителям) Это «коробка передач» мотора колеса: по заданной скорости
  и порогам (`0x266`, `0x99`, `0x132`, `0x8ff`, `0x11ff` и т.п.) выбирается передача
  (0/1/2), а при её смене драйвер сначала «тормозит к направлению» (`brake to dir`)
  через `fcn.0801f4dc`, затем PWM пересчитывается под новый делитель.
- ⓗ два поддерева (`*(ctx+8) == 1` vs `!= 1`) — два профиля порогов (напр. «мягкий/
  жёсткий» разгон или «вперёд/назад»); точный смысл флага не установлен.
- Насыщение вверх → PWM `0x1c20` (7200), торможение вниз → минимальные PWM
  `0x2ce`/`0x706`. ⓗ значения-минимумы = удержание/ползучий ход.

---

## fcn.08020e0c — set_wheel_speed (канал B, второе колесо/мотор) ⓗ

```c
void fcn.08020e0c(int16_t arg1)

{
    char cVar1;
    int *piVar2;
    uint32_t uVar3;
    int iVar4;
    ushort uVar5;
    uint32_t uVar6;
    
    piVar2 = *0x805fb9c;
    uVar3 = arg1;
    cVar1 = *(**0x8020e24 + 9);
    uVar6 = 0;
    if (uVar3 - 500 < 0xdad) {
        uVar6 = (uVar3 * 0x1c20) / 0x157c & 0xffff;
        *(**0x805fb9c + 0x14) = arg1;
    }
    else {
        uVar5 = 500;
        if (uVar3 < 500) {
            uVar6 = 0x28e;
        }
        else {
            uVar5 = 4000;
            if (uVar3 < 0xfa1) goto code_r0x0805fb76;
            uVar6 = 0x1474;
        }
        *(**0x805fb9c + 0x14) = uVar5;
    }
code_r0x0805fb76:
    fcn.080534f8(*0x805fba0,0x4000);
    fcn.0805f3d8(cVar1 != -1);
    fcn.0807f696(*0x805fba4,3,uVar6);
    iVar4 = *piVar2;
    *(iVar4 + 0x10) = uVar6;
    *(iVar4 + 8) = 0;
    return;
}
```

Доказательства:
- Отдельный контекст `piVar2 = *0x805fb9c` (не `0x2000054c`), отдельный PWM-регистр
  `*0x805fba4`. Диапазон скорости зажат `500..4000` (`0xdad = 3501` ширина окна от 500).
- PWM: `(speed*0x1c20)/0x157c` (`0x157c = 5500`), насыщение вниз `0x28e`(654), вверх
  `0x1474`(5236). Тот же выход `fcn.0807f696(...,3,pwm)`.
- `fcn.0805f3d8(cVar1 != -1)` — установка знака/направления по флагу `*(ctx2+9)`.

Интерпретация:
- ⓗ второй канал привода (второе колесо, либо иная ось): другой делитель `5500` и
  другие клипы, но идентичная схема вывода PWM через `fcn.0807f696` канал 3.
- ⓗ клип 500..4000 = допустимая скорость этого канала; ниже 500 → минимальный PWM,
  выше 4000 → максимум.

---

## fcn.0801f4dc — «brake to dir»: смена направления/передачи драйвера с подтверждением ✔

```c
uint fcn.0801f4dc(uint param_1,int param_2,int param_3)

{
    int iVar1;
    uint32_t uVar2;
    uint16_t uVar3;
    uint8_t uVar4;
    uint32_t uStack_20;
    
    uVar3 = *(param_2 + 10);
    if (param_3 == 1) {
        uVar3 = uVar3 & 0xff8f | 0x30;
    }
    else if (param_3 == 0) {
        uVar3 = uVar3 & 0xff8f | 0x20;
    }
    else {
        if (param_3 != 2) goto code_r0x0801f508;
        uVar3 = uVar3 & 0xff8f | 0x10;
    }
    *(param_2 + 10) = uVar3;
code_r0x0801f508:
    uVar2 = *(param_2 + 10);
    uStack_20 = 0;
    uVar4 = 0;
    while (((iVar1 = fcn.080840d8(param_1,uVar2), iVar1 != 0 ||
            (iVar1 = fcn.0806caac(param_1,uVar2 & 0xe000,&uStack_20), iVar1 != 0)) ||
           ((uVar2 & 0xfff) != (uStack_20 & 0xffff)))) {
        uVar4 = uVar4 + 1;
        if (0x31 < uVar4) {
            return 0xffffffff;
        }
    }
    return 0;
}
```

Доказательства:
- `param_3` (направление/передача): 1→биты `0x30`, 0→`0x20`, 2→`0x10` в поле
  `*(param_2+10)` под маской `0xff8f` (очищаются биты 4..6, ставится код направления).
- Запись драйверу `fcn.080840d8(param_1, reg)`, чтение обратно `fcn.0806caac(...,
  reg & 0xe000, &readback)`, сверка `(reg & 0xfff) == (readback & 0xffff)`; повтор до
  `0x31 = 49` попыток, иначе `return -1` (ошибка). Вызывающий (`fcn.0805f620`) логирует
  ошибку `!= 0`.

Интерпретация:
- ✔ атомарная смена направления вращения драйвера мотора с verify-циклом: пишем код
  направления в теневой регистр, шлём драйверу, читаем назад, подтверждаем. `-1` при
  не-подтверждении за 49 попыток.
- ⓗ `fcn.080840d8`/`fcn.0806caac` — транспорт к драйверу (I2C/SPI/UART к внешней
  микросхеме мотора); тип шины не установлен.

---

## fcn.0807f696 — запись PWM в регистр канала мотора ✔

```c
void fcn.0807f696(int param_1,int param_2,uint param_3)

{
    if (param_2 == 0) {
        *(param_1 + 0x34) = param_3;
        return;
    }
    if (param_2 != 1) {
        if (param_2 != 2) {
            if (param_2 == 3) {
                *(param_1 + 0x40) = param_3;
            }
            return;
        }
        *(param_1 + 0x3c) = param_3;
        return;
    }
    *(param_1 + 0x38) = param_3;
    return;
}
```

Доказательства: `param_2` = номер канала (0→+0x34, 1→+0x38, 2→+0x3c, 3→+0x40),
`param_3` = значение PWM. Оба set_wheel_speed вызывают с `param_2 == 3` → пишут в
`*(ctx+0x40)`.

Интерпретация: ✔ примитив «положить PWM в теневой регистр канала». ⓗ +0x34..+0x40 —
4 канала (2 колеса × 2 направления, либо колёса+нож); привязка канал↔физика не
установлена статически.

---

## fcn.0804f2c8 — rand_range (случайное число в [param_1..param_2]) ✔

```c
int fcn.0804f2c8(int param_1,int param_2)

{
    int iVar1;
    int iVar2;
    
    iVar1 = fcn.0804f560();
    (**(iVar1 + 0xc))();
    fcn.08018528();
    iVar1 = fcn.08018514();
    iVar2 = (param_2 - param_1) + 1;
    return (iVar1 - iVar2 * (iVar1 / iVar2)) + param_1;
}
```

Доказательства: `return (rand % ((param_2-param_1)+1)) + param_1` — классический
`rand_range`. `iVar1 = fcn.08018514()` — сырое ПСЧ; `iVar1 - N*(iVar1/N)` = `iVar1 % N`.

Интерпретация: ✔ равномерное целое в `[param_1, param_2]`. Используется для случайных
углов/дистанций (в follow с диапазонами 0x1e..0x87 = 30..135, 0x2d..0x3c = 45..60 и т.д.).

---

## fcn.08077e10 — random move (действие при потере провода) ⓗ

```c
void fcn.08077e10(int param_1)

{
    int iVar1;
    uint uVar2;
    int iVar3;
    uint uVar4;
    code *pcVar5;
    
    iVar1 = *0x8077f7c;
    iVar3 = *(*0x8077f7c + 4);
    *(iVar3 + 0x130) = param_1;
    if (param_1 == 1) {
        *(iVar3 + 299) = 0xff;
        *(iVar3 + 0x145) = 0xff;
        fcn.0802fa0c(0);
        iVar3 = fcn.0804e3e0();
        if (iVar3 == -1) {
            uVar2 = fcn.0804f2c8(0x3c,0x87);
            uVar4 = 1;
            pcVar5 = *(*(*(iVar1 + 4) + 0xc) + 0x7c);
        }
        else {
            uVar2 = fcn.0804f2c8(0x3c,0x87);
            pcVar5 = *(*(*(iVar1 + 4) + 0xc) + 0x84);
            uVar4 = 2;
        }
        (*pcVar5)(600,uVar4,uVar2);
        (**(*(*(iVar1 + 4) + 0xc) + 0xa8))();
        iVar1 = *(iVar1 + 4);
        *(iVar1 + 0x4e) = 0;
        *(iVar1 + 0x12f) = 0xff;
        *(iVar1 + 0x158) = 0;
        fcn.0802920c();
        uVar4 = 0x8077fb0;
        uVar2 = 0x397;
    }
    else if (param_1 == 2) {
        (**(*(iVar3 + 0xc) + 0x54))();
        iVar1 = *(iVar1 + 4);
        uVar4 = 0x8077f9c;
        *(iVar1 + 0x3c) = 0;
        *(iVar1 + 0x42) = 0;
        uVar2 = 0x386;
    }
    else {
        if (param_1 != 3) {
            return;
        }
        *(iVar3 + 0x36) = 0;
        *(iVar3 + 299) = 0xff;
        fcn.0802fa0c(0);
        (**(*(*(iVar1 + 4) + 0xc) + 0x58))();
        fcn.0802f97c(0);
        fcn.0802c1ac();
        iVar1 = *(iVar1 + 4);
        // ... длинная инициализация полей состояния (сброс) ...
        *(iVar1 + 0x28) = *0x8077f80;
        // ...
        fcn.0802920c();
        uVar4 = 0x8077f88;
        uVar2 = 0x380;
    }
    fcn.08048adc(4,0x8077f94,0x8077fcc,*0x8077fc8,uVar2,uVar4);
    return;
}
```
(средняя ветвь `param_1==3` — большой блок обнуления полей состояния, опущен как
неинформативный; полный текст воспроизводим `pdg @ 0x08077e10`.)

Доказательства:
- `param_1` = под-команда 1/2/3 (enter/stop/reset). При `param_1==1` выбирается
  случайный угол `fcn.0804f2c8(0x3c,0x87)` = 60..135°, и по признаку `fcn.0804e3e0()`
  (== -1 или нет) выбирается один из двух методов драйвера (+0x7c или +0x84) с
  направлением `uVar4` 1 или 2 и запуском `(*pcVar5)(600, dir, angle)`.
- Вызывается из `fcn.0802fa90` (8 раз) и `fcn.08031a34` (диспетчеры поведения) — `axt`.

Интерпретация:
- ⓗ «случайный манёвр»: `param_1==1` — начать (отъезд назад/поворот на случайный
  угол 60..135° со скоростью-заданием 600), `2` — стоп (обнуление PWM-полей +0x3c/+0x42),
  `3` — полный сброс состояния поведения.
- ⓗ `fcn.0804e3e0()` — выбор стороны разворота (лево/право) → метод +0x7c vs +0x84.
- Скорость `600` — задание для set_wheel_speed. ⓗ единицы совпадают с `arg1` каналов A/B.

---

## fcn.0802c900 + fcn.0802c424 — резолвер направления по катушкам (флаг 0/1/2) ✔

Флаг: 0 = нет сигнала (снаружи/потеря), 1 = внутри, 2 = снаружи (задача; коды в
`*param_2`). Катушки: лево `[ctx+0x24]`, право `[ctx+0x2c]`.

### fcn.0802c424 — вычисление флага стороны

```c
void fcn.0802c424(int param_1,uchar *param_2)

{
    uint8_t uVar1;
    uchar uVar2;
    uint uVar3;
    int iVar4;
    int iVar5;
    uint uVar6;
    uint32_t uVar7;
    int iVar8;
    
    iVar5 = *0x802c660;
    if (param_1 != 1) {
        if (param_1 == 3) {
            uVar2 = fcn.0804f2ee();
            *param_2 = uVar2;
            uVar6 = 0x802c67c;
            uVar3 = 0xde3;
        }
        else {
            uVar2 = fcn.0804f2ee();
            *param_2 = uVar2;
            uVar6 = 0x802c608;
            uVar3 = 0xde6;
        }
        goto code_r0x0802c43e;
    }
    iVar4 = *(*0x802c660 + 4);
    uVar1 = *(iVar4 + 0x12a);
    if (uVar1 == 0) {
        iVar5 = *(iVar4 + 0x24);
        iVar4 = *(iVar5 + 0x24);
        if (iVar4 == 0) {
            iVar5 = *(iVar5 + 0x2c);
code_r0x0802c4b8:
            if ((iVar5 == 1) || (iVar5 == 2)) {
code_r0x0802c4bc:
                *param_2 = 1;
                return;
            }
        }
        else if (iVar4 == 1) {
            iVar5 = *(iVar5 + 0x2c);
            if (iVar5 == 0) goto code_r0x0802c4c0;
            if (iVar5 == 1) goto code_r0x0802c530;
            if (iVar5 == 2) goto code_r0x0802c4c0;
        }
        else if (iVar4 == 2) {
            iVar5 = *(iVar5 + 0x2c);
            if (iVar5 == 0) goto code_r0x0802c4c0;
            goto code_r0x0802c4b8;
        }
        uVar3 = 0xd6e;
code_r0x0802c4f2:
        fcn.08048adc(2,0x802c658,0x802c630,*0x802c62c,uVar3,0x802c664);
    }
    else {
        // ... уровни границы uVar1 == 1 и uVar1-1 (мультизона), см. полный pdg ...
    }
code_r0x0802c530:
    uVar2 = fcn.0804f2ee();
code_r0x0802c5f4:
    *param_2 = uVar2;
    return;
}
```
(средние ветви для `uVar1 >= 1` — обработка нескольких зон/петель границы, длинные;
полный текст `pdg @ 0x0802c424`.)

Доказательства:
- Читает две катушки: `iVar4 = *(iVar5+0x24)` (лево) и `iVar5 = *(iVar5+0x2c)` (право),
  каждая = 0/1/2. По их комбинации пишет флаг `*param_2` = 1 (внутри) или 2 (`code_r0x0802c4c0: *param_2 = 2`).
- `fcn.0804f2ee()` — фолбэк (случайный выбор стороны, если сигнала нет). ✔ совпадает
  с адресами полей из задания (`+0x24` лево, `+0x2c` право).

### fcn.0802c900 — использование флага (машина состояний по стороне)

```c
void fcn.0802c900(uint param_1,uint param_2,uint param_3,uint param_4)

{
    int iVar1;
    int iVar2;
    uint uVar3;
    int iVar4;
    uint uVar5;
    int iVar6;
    code *pcVar7;
    uint uStack_10;
    
    iVar2 = *0x802c9d0;
    iVar6 = *(*0x802c9d0 + 4);
    iVar1 = *(iVar6 + 0x24);
    iVar4 = *(iVar1 + 0x24);
    uStack_10 = param_4;
    if (iVar4 == 0) {
        if (*(iVar1 + 0x2c) != 1) {
            return;
        }
        pcVar7 = *(*(iVar6 + 0xc) + 0x88);
        (*pcVar7)(1,0x2d0,pcVar7,param_4,param_2,param_3);
        fcn.0802f97c(1);
        uVar5 = 0x802ca00;
        uVar3 = 0x906;
    }
    else if (iVar4 == 1) {
        if (*(iVar1 + 0x2c) == 0) {
            (**(*(iVar6 + 0xc) + 0x88))(2,0x2d0);
            fcn.0802f97c(1);
            uVar5 = 0x802ca3c;
            uVar3 = 0x90f;
        }
        else {
            if (*(iVar1 + 0x2c) != 1) {
                return;
            }
            fcn.0802c424(1,&uStack_10);
            (**(*(*(iVar2 + 4) + 0xc) + 0x88))(uStack_10,0x2d0);
            fcn.0802f97c(1);
            if (uStack_10 == '\x01') {
                uVar5 = 0x802ca90;
                uVar3 = 0x915;
            }
            else {
                uVar5 = 0x802ca20;
                uVar3 = 0x917;
            }
        }
    }
    else {
        if (iVar4 != 2) {
            return;
        }
        iVar2 = *(iVar1 + 0x2c);
        if (iVar2 == 1) {
            return;
        }
        if (iVar2 == 2) {
            (**(*(iVar6 + 0xc) + 0x88))(1,0x2d0);
            fcn.0802f97c(1);
            uVar5 = 0x802caac;
            uVar3 = 0x924;
        }
        else {
            if (iVar2 != 0) {
                return;
            }
            uVar5 = 0x802c9d4;
            uVar3 = 0x928;
        }
    }
    fcn.08048adc(4,0x802ca88,0x802ca60,*0x802ca5c,uVar3,uVar5);
    return;
}
```

Доказательства:
- `iVar4 = *(iVar1+0x24)` (лево), `*(iVar1+0x2c)` (право) — те же две катушки.
  По их комбинации вызывается метод драйвера `*(vtable+0x88)` с аргументом
  направления (1 или 2) и константой `0x2d0 = 720`.
- В неоднозначном случае (лево=1, право=1) зовёт `fcn.0802c424(1,&uStack_10)` — уточняет
  сторону и рулит по её результату.

Интерпретация:
- ✔ резолвер «с какой стороны провод»: по паре катушек лево/право выбирает направление
  подруливания. Метод vtable +0x88 — команда поворота/подруливания; `0x2d0`(720) —
  ⓗ параметр (скорость/дистанция) манёвра коррекции.
- ⓗ коды направления в `*(vtable+0x88)`: 1 и 2 = «крутить влево»/«вправо».

---

## fcn.0803a7a0 — follow (следование по проводу, bang-bang) — СТРУКТУРА ⓗ

Функция огромная (33032 байт декомпилированного `pdg`, ~92 КБ). Полный дамп
воспроизводится `pdg @ 0x0803a7a0`; здесь — структура (verbatim фрагменты каркаса).

Сигнатура и общий каркас — машина состояний по под-состоянию `*(*piVar17 + 0x68)`
с таблицей переходов на 9 веток:

```c
    //switch table (9 cases) at 0x803b05c
    switch(*(*piVar17 + 0x68)) {
    case 0:      // сброс полей +0x62/+0x81..+0x88; счётчик +0x5c с порогом 0x13
        ...
    case 1:
    case 7:
    case 8:      // проверка *(ctx+0x1c)+0x40 == 2 && ctx+0x77 == 1; вызовы fcn.0805deec, fcn.08032dd8
        ...
    case 2:      // ...
    case 3:
    case 4:
        return uVar32;
    case 5:      uVar13 = 1; break;
    case 6:      uVar13 = 2; break;
    default:     return uVar32;
    }
```

Внутри активно используется `rand_range` для случайных углов манёвров (verbatim
вызовы из тела follow):

```c
    uVar12 = fcn.0804f2c8(0x2d,0x3c);   // 45..60
    uVar12 = fcn.0804f2c8(0x4b,0x87);   // 75..135
    uVar12 = fcn.0804f2c8(0x1e,0x2d);   // 30..45
    uVar12 = fcn.0804f2c8(0x2d,0x73);   // 45..115
    uVar12 = fcn.0804f2c8(0x23,0x41);   // 35..65
    uVar5  = fcn.0804f2c8(0x1e,0x78);   // 30..120
    uVar5  = fcn.0804f2c8(0x4b,0x87);   // 75..135
```

Доказательства:
- 9-веточный switch по `*(*ctx+0x68)` (таблица `0x803b05c`) — под-состояние поведения
  follow. ✔ (таблица переходов явно в дизасме).
- Множественные `fcn.0804f2c8(min,max)` — случайные углы (bang-bang с рандомизацией). ✔
- Единственный вызывающий: `fcn.0806c...` @ 0x8067c34 (`bl fcn.0803a7a0`) — диспетчер
  режимов. ✔ (`axt`).

Интерпретация:
- ⓗ bang-bang по стороне провода: держит сигнал катушки в «коридоре», при выходе —
  доворачивает; при потере — случайный поиск (`rand_range`). Точная логика каждого из
  9 под-состояний не разбиралась из-за размера — см. открытые ⓗ.

---

## fcn.080321d4 — recover (HIT/LIFT/SLOPE/STALL/ultrasonic) — СТРУКТУРА ⓗ

Огромная (диапазон 0x0802cad0..0x08032a58, ~24 КБ). Полный текст — `pdg @ 0x080321d4`.
Голова функции (verbatim, начало):

```c
void fcn.080321d4(void)

{
    // ... объявления ...
    iVar10 = *0x80325a8;
    if (*0x80325ac <= *(*(*0x80325a8 + 4) + 4)) {
        iVar9 = fcn.08051f2c();
        iVar9 = (**(iVar9 + 0x58))();
        if (iVar9 == 1) {
            iVar9 = (**(*(*(iVar10 + 4) + 0xc) + 0x24))();
            if (((iVar9 != 5) && (iVar9 != 6)) || (iVar9 = (**(*(*(iVar10 + 4) + 0xc) + 0x30))(), iVar9 == 0))
            goto code_r0x0803221a;
            iVar9 = fcn.08051f2c();
            (**(iVar9 + 0x54))();
        }
        *(*(iVar10 + 4) + 4) = 0;
    }
code_r0x0803221a:
    iVar9 = fcn.0802bbe0();
    if (iVar9 == 0) {
        fcn.0802bce4(2);
        // ...
    }
    // ...
}
```

Карта веток по строкам-сообщениям (все — в хвосте диапазона функции, т.е. это её
собственные log-строки; адреса из `izz`):

| vaddr строки | сообщение | смысл ветки ⓗ |
|---|---|---|
| 0x0802d070 | `torque cycle hit, blade brake and escape` | HIT по току ножа → тормоз ножа + отъезд |
| 0x0802ebb4 | `torque back hit, blade brake and escape` | HIT при заднем ходе |
| 0x0802f480 | `torque back wheel stall, blade brake and escape` | STALL заднего колеса по току |
| 0x0802f580 | `move brake overtime, continue front` | таймаут торможения → продолжить вперёд |
| 0x0802fde0 | `start blade,but updown, brake and goto error updown` | подъём при старте ножа |
| 0x080326c0 | `start blade,but updown, brake and goto error updown` | (то же, др. состояние) |
| 0x080327c4 | `hit overtime, blade brake and escape` | таймаут HIT |
| 0x080327ec | `lift, blade brake and escape` | LIFT (поднят) → тормоз + отъезд |
| 0x08032a58 | `no border overtime, blade brake and wait` | не найдена граница (таймаут) |
| 0x08032a84 | `overslope, blade brake and escape` | SLOPE (крен) → тормоз + отъезд |
| 0x08032aa8 | `outof border overtime, blade brake and wait` | выход за границу (таймаут) |

Кроме recover, соседние строки-состояния (для контекста, из `izz`):
`brake overtime, refront` (0x0803ec4f), `brake overtime, change to find border`
(0x08042b67), `on border, brake` (0x0804aee0), `check station, brake and change to
docking` (0x08069b98).

Доказательства: строки verbatim из `izz`; лежат внутри/на границе диапазона recover. ✔
Каждая ветка: «blade brake» (тормоз ножа) + «escape/wait» (отъезд/ожидание).

Интерпретация:
- ⓗ recover = обработчик аварий/событий: удар (HIT по току ножа/колеса), подъём (LIFT),
  крен (SLOPE/overslope), застревание (STALL), потеря границы. Общий сценарий: затормозить
  нож, затем отъехать/ждать. Ветвление по под-состоянию (аналогично 9-way switch follow).
- Точное сопоставление код-ветки ↔ строка требует разбора управляющего графа (не сделано
  из-за размера) → открытая ⓗ.

---

## Мотор-контексты и функции-примитивы драйвера

| символ / адрес | назначение | доказательство |
|---|---|---|
| `0x2000054c` | контекст мотора канала A (используется fcn.0805f620) | литерал `[0x805fa18]=0x2000054c` |
| `[0x805fb9c]` → RAM | контекст мотора канала B (fcn.08020e0c) | литерал в fcn.08020e0c |
| `ctx+0x10` | текущая передача/направление (0/1/2) | `*(ctx+0x10)=DIR` после fcn.0801f4dc |
| `ctx+8` | флаг профиля порогов (0/1) | ветвление в fcn.0805f620 |
| `ctx+2` | сохранённый текущий PWM | `*(ctx+2)=uVar12` |
| `driver+0x34..+0x40` | 4 теневых регистра PWM каналов | fcn.0807f696 |
| `driver+10` | теневой регистр направления (биты 4..6) | fcn.0801f4dc |
| `fcn.0807f696` | запись PWM в регистр канала | — |
| `fcn.0801f4dc` | смена направления + verify (49 попыток) | «brake to dir» |
| `fcn.080840d8`/`fcn.0806caac` | транспорт запись/чтение к драйверу ⓗ | вызовы в fcn.0801f4dc |

### Инициализация драйверов и очереди скоростей колёс (aav.0x08055314)

В функции инициализации мотор-подсистемы (диапазон ~0x08054fe4..0x08055730,
r2-имя `aav.0x08055314`) — регистрация «left/right motor driver» и создание очередей:

Строки-доказательства (`izz`):
- `left motor driver init failed`, `right motor driver not register`,
  `right motor driver init failed` — @ ~0x080564xx (дизасм: `adr r1, ...` + `bl fcn.0804f4c4`).
- `create left wheel speed queue failed` @ 0x08056544 (ADR-ссылка из 0x080564a0).
- `create right wheel speed queue failed` @ 0x0805656c (ADR-ссылка из 0x080564a6).

✔ Подтверждает **две очереди скорости колёс — левую и правую** (раздельное управление
левым/правым колесом). ⓗ канал A (0x2000054c) и канал B (0x805fb9c) = эти два колеса,
но привязка A↔лево / B↔право статически НЕ доказана (обе функции читают фиксированные
глобальные контексты, а не параметр «сторона»).

---

## Сводка: команда → мотор колеса ✔/ⓗ

```
Высокоуровневое поведение (follow / recover / random_move / drive-by-angle)
        │  задаёт скорость (unit ~ 0..7200?) и/или угол
        ▼
  angle_to_cycle_tick (fcn.08020520):  tick = angle*697 / cal[gear]      ✔ формула
        ▼
  drive-by-angle (fcn.0804bb0c): мьютекс → vtable драйвера (скорость+тики) ⓗ
        ▼
  set_wheel_speed A/B (fcn.0805f620 / fcn.08020e0c):
        speed → выбор передачи (0/1/2 по порогам)
        при смене передачи: fcn.0801f4dc = «brake to dir» (verify до 49 попыток) ✔
        PWM = speed*7200 / gear_max  (gear_max = 6141/3069/1533; 5500 у канала B)   ✔
        ▼
  fcn.0807f696(driver, ch=3, pwm):  запись PWM в теневой регистр канала        ✔
        ▼
  driver+0x40 (PWM) / driver+10 (направление) → внешний драйвер мотора (транспорт ⓗ)
```

Торможение в этой прошивке — это НЕ отдельный «стоп», а часть смены передачи/направления:
`fcn.0801f4dc` («brake to dir=%d») сначала приводит драйвер к нужному направлению
(с подтверждением по обратному чтению), и только затем меняется PWM. Полная остановка
= PWM 0 (ветки `uVar12 = 0` / обнуление `ctx+0x3c`,`+0x42` в fcn.08020e0c). «blade brake»
в recover — торможение ножа (отдельная подсистема), не колёс.

---

## Открытые гипотезы (проверить на живом чипе)

1. ⓗ **Лево/право**: канал A (`0x2000054c`, fcn.0805f620) vs канал B (`[0x805fb9c]`,
   fcn.08020e0c) — которое левое, которое правое? Очереди «left/right wheel speed»
   доказывают ДВА колеса, но привязка контекст↔сторона статически не разведена.
2. ⓗ **Единицы скорости**: `arg1` в set_wheel_speed. Максимум PWM `0x1c20 = 7200`;
   `speed*7200/gear_max`. Диапазон `arg1` (по клипам канала B: 500..4000). Физику
   (мм/с? % ?) проверить.
3. ⓗ **Смысл 697** (`0x2b9`) в angle→tick и делителей передач 6141/3069/1533/5500.
   Это калибровки «тиков на оборот/градус»; подтвердить на энкодере.
4. ⓗ **Флаг `ctx+8`** в fcn.0805f620 — два дерева порогов (вперёд/назад? мягкий/жёсткий?).
5. ⓗ **Транспорт к драйверу** (`fcn.080840d8`/`fcn.0806caac`/`fcn.08084...`): тип шины
   (I2C/SPI/UART) и адрес внешней микросхемы мотора — не установлено.
6. ⓗ **Метод vtable +0x88** (резолвер fcn.0802c900) и +0x18/+0x20 (drive-by-angle) —
   точное соответствие смещений командам драйвера.
7. ⓗ **Сопоставление веток recover/follow ↔ строки** — из-за размера функций (24/33 КБ)
   полный управляющий граф не разбирался; карта веток дана по log-строкам, не по CFG.
8. ⓗ **RAM `0x2000b250`** — кандидат «мотор-значение» из задания. Статических xref НЕ
   найдено (`axt`/`/v4` пусто); связь с приводом НЕ доказана. Основные мотор-данные
   лежат в контекстах `0x2000054c` и `[0x805fb9c]`.
