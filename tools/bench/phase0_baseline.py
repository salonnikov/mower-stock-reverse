#!/usr/bin/env python3
"""
PHASE 0 — passive reconnaissance (READ ONLY, not a single write).
Run:  python3 phase0_baseline.py
Gives a GO/NO-GO verdict: is the left wheel's PWM path active in the current state.
"""
import time
from swd import SWD

S = SWD()


def rd(name, addr, note=""):
    v = S.mdw(addr)
    v = v[0] if v else -1
    print("  %-26s @0x%08x = 0x%08x %s" % (name, addr, v & 0xFFFFFFFF, note))
    return v


print("=== PHASE 0 — passive reconnaissance (read only) ===")
state = rd("state", 0x200000BC)
print("      -> state = %d  (3idle 4departure 5cutting 6findbd 7dock 8charge 9err 10off)"
      % (state & 0xFF))
mb = rd("movebase singleton", 0x20000338, "(0 = drive service NOT up)")
lm = rd("lmdrv obj slot", 0x2000054C, "(0 = left driver NOT up)")
rm = rd("rmdrv obj slot", 0x2000057C)
ctl0 = rd("TIMER2 CTL0", 0x40000400)
print("      -> CEN(bit0) = %d  (1 = timer counter running)" % (ctl0 & 1))
chctl2 = rd("TIMER2 CHCTL2", 0x40000420)
print("      -> CH2EN = %d  CH3EN = %d" % ((chctl2 >> 8) & 1, (chctl2 >> 12) & 1))
car = rd("TIMER2 CAR (period)", 0x4000042C)
c1 = S.mdw(0x40000424)
time.sleep(0.2)
c2 = S.mdw(0x40000424)
c1 = c1[0] if c1 else 0
c2 = c2[0] if c2 else 0
print("  %-26s 0x%08x -> 0x%08x  (%s)"
      % ("TIMER2 CNT (2 snapshots)", c1, c2, "running" if c1 != c2 else "STOPPED"))
ch2 = rd("TIMER2 CH2CV", 0x4000043C, "(right? to confirm)")
ch3 = rd("TIMER2 CH3CV", 0x40000440, "(LEFT wheel, duty) <- baseline")

print("\n=== VERDICT ===")
problems = []
if mb == 0:
    problems.append("drive service not up (movebase=0) — A4963 not configured; PWM is meaningless")
if lm == 0:
    problems.append("left driver not up (lmdrv=0)")
if (ctl0 & 1) == 0:
    problems.append("TIMER2 stopped (CEN=0) — writing CH3CV has no effect")
if ((chctl2 >> 12) & 1) == 0:
    problems.append("CH3 not enabled (CH3EN=0) — PWM output inactive")
if c1 == c2:
    problems.append("CNT not running — timer stopped")

if not problems:
    print("GO: the left wheel's PWM path is ACTIVE. Proceed to Phase 1 (odometry) and Phase 2/3.")
    print("     Remember baseline CH3CV = 0x%08x, CAR = 0x%08x" % (ch3 & 0xFFFFFFFF, car & 0xFFFFFFFF))
else:
    print("NO-GO for a direct PWM test. Reasons:")
    for p in problems:
        print("   - " + p)
    print("   -> first put the mower into a state where the motor loop is up "
          "(drive service active) but NOT in state 5/6 (there border navigation commands by itself).")
S.close()
