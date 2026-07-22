# chip2 — конвейер катушки и состояние (follow_border_check_wave)

Вывод `pdg` — доверяем. `⚑` = интерпретация (`ⓗ`). Имена `✔` — из elog-тегов.

## `follow_border_check_wave` — `fcn.080164bc`  (`✔` elog-тег)

`⚑` Полный конвейер обработки **одной** катушки за цикл: буфер отсчётов → `segment_by_zero` → сопоставление сегментов → `classify_packet` → **код состояния 0..8**.

**Доказательства (`✔`):**
- `fcn.08015dcc(buf, count, 0)` — сегментация (см. 02) `✔`.
- `fcn.080159a0(...)` — классификатор (см. 03); маппинг его `return` в код состояния `✔`:
  - classifier `== 1` → лог `left wave data error, area=%d, str=%d`, state `2` `✔`.
  - classifier `== 0` → state `1` `✔`.
  - classifier `== 4` → state `7` `✔`.
  - classifier `== 3` → state `8` `✔`.
  - classifier `== 2` → (падение сквозь) `✔`.
- `fcn.0801baf8(...,5,...)` в начале — `ⓗ` копирование/подготовка отсчётов (арг `5` ≠ `4` у subwave).
- `fcn.080179f8(...)` — межсегментный матчер/дистанция (тот же, что в классификаторе).

`⚑ ⓗ` Коды состояния (0..8) — это то, что дальше читается билдером JSON как знак/уровень `ls`/`rs`/`lf`. Точная семантика чисел — гипотеза (контекст: 0=нет,1=внутри,2=снаружи, 3/4/6/7/8=промежуточные).

```c
uint fcn.080164bc(seg_in, count, out_state, feat)   // ✔ follow_border_check_wave
{
    if (!seg_in || !out_state) return 2;
    int buf = malloc(count << 1);                   // int16-буфер
    if (!buf) return 2;
    int r = fcn.0801baf8(seg_in, count, count, 5, 0, buf);  // ⓗ prep/copy
    if (r) { if (r==1) return 3; if (r==2) return 4; free_wave(buf); return r; }

    seglist = fcn.08013950(0,0x10);                  // список сегментов
    if (fcn.08015dcc(buf, count, 0) != 0) { free; return 5; }   // ✔ сегментация
    ...
    if (matcher fcn.08014e88(...) == 0) {
        if (coil.count < 3) return 0;                // мало данных
        list_add(feat, seglist[0]);
        if (feat.count > 3) {
            int cls = fcn.080159a0(...);             // ✔ классификатор
            if (cls==1){ log("left wave data error..."); return 2; }  // ✔
            if (cls==0) return 1;                    // ✔
            if (cls==4) return 7;                    // ✔
            if (cls==3) return 8;                    // ✔
        }
    } else { /* объединить перекрывающиеся сегменты, fcn.080179f8 dist */ }
    return 6;
}
```

## `follow_check_subwave` — `fcn.08016784`  (`✔` elog-тег)

`⚑` Вторичный/«search» детектор (питает `sls`/`srs`): та же сегментация `fcn.08015dcc`, но подготовка `fcn.0801baf8(...,4,...)` (арг `4`) и финальный матч по шаблону через `fcn.08015008(seglist, 3000, ...)` с окном **`0x78`=120 или `400`** отсчётов в зависимости от `param_9` (`✔`).

```c
uint fcn.08016784(seg_in, count, c3, ...)   // ✔ follow_check_subwave
{
    int buf = malloc(count<<1); if(!buf) return 2;
    if (fcn.0801baf8(seg_in,count,count,4,0,buf)) { ... return 3/4; }
    seglist = fcn.08013950(0,0x10);
    if (fcn.08015dcc(buf,count,c3)==0) {
        uint win = param_9 ? 0x78 : 400;             // ✔ окно поиска
        int m = fcn.08015008(seglist, 3000, ..., win, ...);   // ✔ шаблон-матч
        return m==0 ? 0 : 6;
    }
    return 5;
}
```

## `deal_lift_sensor` — `fcn.08013970`  (`✔` elog-тег)

`⚑` Дебаунс двух датчиков подъёма (читает два GPIO через vtable, `iVar2`/`iVar3`), считает счётчики до порога **10**, выставляет состояние в `[+0xe]`: **1=left lifted, 2=right/…**, логирует строки `left lifted`/`right lifted`/`both lifted`/`lift none` (`✔` строки на оффсетах `0x13a7f…0x13ad8`).

```c
void fcn.08013970(void)   // ✔ deal_lift_sensor
{
    int L = read_lift_sensor_0();      // vtable +4
    int R = read_lift_sensor_1();      // vtable +8
    obj = get(); // 0x2000... lift state struct
    if (L==1 && R==1) {                // оба
        if (obj[0xf] < 10) { obj[0xf]++; obj[0x10]++; obj[0x11]++; return; }
        if (obj[0xe]==1) return; obj[0xe]=1; ...; log("both lifted");   // ✔
    } else if (L==1) { ... log("left lifted"); }    // ✔
      else if (R==1) { ... log("right lifted"); }   // ✔
      else           { ... log("lift none"); }      // ✔
}
```
