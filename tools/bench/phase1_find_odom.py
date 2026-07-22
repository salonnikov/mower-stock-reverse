#!/usr/bin/env python3
"""
ФАЗА 1 — калибровка одометрии (руками, БЕЗ моторов).
Находит живой счётчик тиков колеса как поле в объекте драйвера (malloc), диффом
до/после ручной прокрутки колеса. Даёт абсолютный адрес тика для Фазы 3.

Порядок:
  1) python3 phase1_find_odom.py save   # снимок №1
  2) руками провернуть ОБА колеса (напр. по 10 оборотов), сказать «готово»
  3) python3 phase1_find_odom.py diff    # найдёт поля-кандидаты тиков
"""
import sys
import json
from swd import SWD

WORDS = 0x40  # сколько слов объекта дампить
SNAP = "/tmp/odom_snap.json"
S = SWD()


def dump(slot):
    base = S.mdw(slot)
    base = base[0] if base else 0
    if base == 0:
        return 0, []
    return base, S.mdw(base, WORDS)


lb, lv = dump(0x2000054C)  # left driver object
rb, rv = dump(0x2000057C)  # right driver object
snap = {"lb": lb, "lv": lv, "rb": rb, "rv": rv}

mode = sys.argv[1] if len(sys.argv) > 1 else "save"

if mode == "save":
    if lb == 0 or rb == 0:
        print("ВНИМАНИЕ: объект драйвера пуст (lmdrv=0x%08x rmdrv=0x%08x). "
              "Сначала подними движ-сервис (см. Фаза 0)." % (lb, rb))
    json.dump(snap, open(SNAP, "w"))
    print("Снимок №1 сохранён (%s)." % SNAP)
    print("Теперь ПРОВЕРНИ оба колеса рукой (напр. по 10 оборотов), затем:")
    print("   python3 phase1_find_odom.py diff")
else:
    old = json.load(open(SNAP))
    found = []
    for tag, base, new, oldv in (("LEFT", lb, lv, old["lv"]), ("RIGHT", rb, rv, old["rv"])):
        print("--- %s obj @0x%08x ---" % (tag, base))
        for i, (a, b) in enumerate(zip(oldv, new)):
            if a != b:
                d = (b - a) & 0xFFFFFFFF
                d = d if d < 0x80000000 else d - 0x100000000
                addr = base + i * 4
                print("  +0x%03x @0x%08x: 0x%08x -> 0x%08x  (Δ=%d)  <= КАНДИДАТ тик"
                      % (i * 4, addr, a, b, d))
                found.append((tag, addr, d))
    print("\nВыбери поле с монотонным приростом ~= числу оборотов*тик_на_оборот.")
    print("Его АДРЕС подставишь в Фазу 3 как odom-адрес:")
    for tag, addr, d in found:
        print("   %s: 0x%08x (Δ=%d)" % (tag, addr, d))
    if not found:
        print("   ничего не изменилось — колесо не крутили, объект пуст, или тик в др. структуре")
        print("   fallback: смотреть TIMER3 CNT/capture (0x40000824) или расширить WORDS.")
S.close()
