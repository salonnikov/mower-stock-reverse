#!/usr/bin/env python3
"""
Монитор перехода idle->движение (Вариант 1): ловит, как поднимается драйвер колеса
и оживает PWM/одометрия. ТОЛЬКО ЧТЕНИЕ.
Запуск:  ./watch_drive.py [секунд]   (по умолчанию 35)
Печатает строку каждые ~0.3с: время, state, lmdrv, rmdrv, CH2CV, CH3CV,
и первые слова объекта левого драйвера (для поиска тик-счётчика).
"""
import sys
import time
from swd import SWD

S = SWD()
dur = float(sys.argv[1]) if len(sys.argv) > 1 else 35.0
t0 = time.time()
print("  t   st  lmdrv       rmdrv       CH2   CH3   | lmobj[+0x18..+0x30]")
while time.time() - t0 < dur:
    st = (S.mdw(0x200000BC) or [0])[0] & 0xFF
    lm = (S.mdw(0x2000054C) or [0])[0]
    rm = (S.mdw(0x2000057C) or [0])[0]
    ch2 = (S.mdw(0x4000043C) or [0])[0]
    ch3 = (S.mdw(0x40000440) or [0])[0]
    tail = ""
    if lm:
        w = S.mdw(lm + 0x18, 7)  # окно, где вероятны поля скорости/тиков
        tail = " ".join("%08x" % x for x in w)
    print("%5.1f  %2d  0x%08x  0x%08x  %04x  %04x  | %s"
          % (time.time() - t0, st, lm, rm, ch2, ch3, tail), flush=True)
    time.sleep(0.3)
print("=== monitor done ===", flush=True)
S.close()
