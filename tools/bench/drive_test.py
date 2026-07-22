#!/usr/bin/env python3
"""Software drive+override test: forces motion (state=6), waits for PWM, zeros
the left channel CH3CV (checks loop takeover), then stops. BENCH ONLY."""
import time
from swd import SWD
S = SWD()
ST, CH2, CH3 = 0x200000BC, 0x4000043C, 0x40000440
rd = lambda a: (S.mdw(a) or [-1])[0]
print("baseline: state=%d CH2=%#06x CH3=%#06x" % (rd(ST) & 0xFF, rd(CH2), rd(CH3)))
print(">> force state=6 (go)")
S.mww(ST, 6)
drv = False
for i in range(15):
    st, c2, c3 = rd(ST) & 0xFF, rd(CH2), rd(CH3)
    print("  t%-2d state=%d CH2=%#06x CH3=%#06x" % (i, st, c2, c3))
    if c2 > 0x400 or c3 > 0x400:
        drv = True
        break
    time.sleep(0.2)
if not drv:
    print("Did NOT move with forced state=6. Stop.")
    S.mww(ST, 3)
    S.close()
    raise SystemExit
print(">> MOVING. Watch the LEFT wheel. Zeroing CH3CV in 1s.")
time.sleep(1.0)
print(">> CH3CV <- 0 (holding against the loop)")
for i in range(12):
    S.mww(CH3, 0)
    c2, c3 = rd(CH2), rd(CH3)
    print("  hold%-2d CH2=%#06x CH3=%#06x" % (i, c2, c3))
print(">> STOP (state=9, both channels 0)")
S.mww(ST, 9)
S.mww(CH2, 0)
S.mww(CH3, 0)
print("=== QUESTION: did the left wheel SLOW/STOP while the right kept turning? ===")
S.close()
