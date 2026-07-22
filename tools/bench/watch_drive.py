#!/usr/bin/env python3
"""
Monitor of the idle->motion transition (Variant 1): catches how the wheel driver comes up
and PWM/odometry come alive. READ ONLY.
Run:  ./watch_drive.py [seconds]   (default 35)
Prints a line every ~0.3s: time, state, lmdrv, rmdrv, CH2CV, CH3CV,
and the first words of the left driver object (to locate the tick counter).
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
        w = S.mdw(lm + 0x18, 7)  # window where speed/tick fields are likely
        tail = " ".join("%08x" % x for x in w)
    print("%5.1f  %2d  0x%08x  0x%08x  %04x  %04x  | %s"
          % (time.time() - t0, st, lm, rm, ch2, ch3, tail), flush=True)
    time.sleep(0.3)
print("=== monitor done ===", flush=True)
S.close()
