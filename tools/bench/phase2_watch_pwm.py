#!/usr/bin/env python3
"""
PHASE 2 — does the firmware rewrite CH3CV by itself (observation, WITHOUT writing).
Determines the injection method for Phase 3.
Run:  python3 phase2_watch_pwm.py
"""
import time
from swd import SWD

S = SWD()
CH3CV = 0x40000440
N = 20
print("=== PHASE 2 — observing CH3CV (%d times, ~%.0fs) ===" % (N, N * 0.5))
vals = []
for i in range(N):
    v = S.mdw(CH3CV)
    v = v[0] if v else -1
    vals.append(v)
    print("  t=%4.1fs  CH3CV=0x%08x" % (i * 0.5, v & 0xFFFFFFFF))
    time.sleep(0.5)

uniq = set(vals)
print("\n=== VERDICT ===")
if len(uniq) == 1:
    print("CH3CV is STABLE -> the firmware isn't touching it right now.")
    print("   => Phase 3: you can write duty directly, the write will hold.")
else:
    print("CH3CV MOVES by itself (%d distinct values) -> the motor loop is actively commanding." % len(uniq))
    print("   => a direct write will be overwritten ~every 10ms. Options:")
    print("      - write duty in a LOOP (Phase 3 with --loop), or")
    print("      - put the mower into a state where the loop does NOT command speed.")
S.close()
