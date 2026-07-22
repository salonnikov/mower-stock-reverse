#!/usr/bin/env python3
"""
PHASE 3 — direct duty write to CH3CV (THE MAIN test: does PWM turn the wheel).
WRITES to the register! Preconditions: wheels off the ground, blade disconnected, hand on the kill switch,
Phase 0 = GO, Phase 2 understood.

Run (one wheel at a time, small duty):
  python3 phase3_drive.py L 500 0x2000beXX     # side duty odom_addr(from Phase 1)
  python3 phase3_drive.py L 500 0x2000beXX --loop   # if Phase 2 showed overwriting

Arguments:
  side : L (CH3CV 0x40000440) | R (CH2CV 0x4000043c — ⓗ confirm the channel via Phase 0)
  duty : 0..7200 (CAR period ~7200). Start with 300-500.
  odom : tick address from Phase 1 (for an objective rotation check). May be omitted.
  --loop : hold duty ~3s, rewriting every 20ms (against overwriting by the firmware).
Always writes 0 at the end (stop/brake).
"""
import sys
import time
from swd import SWD

CH = {"L": 0x40000440, "R": 0x4000043C}

side = (sys.argv[1].upper() if len(sys.argv) > 1 else "L")
duty = int(sys.argv[2]) if len(sys.argv) > 2 else 500
odom = int(sys.argv[3], 0) if len(sys.argv) > 3 and not sys.argv[3].startswith("--") else None
loop = "--loop" in sys.argv
ch = CH.get(side, CH["L"])

if duty < 0 or duty > 7200:
    print("duty out of [0..7200]"); raise SystemExit(1)

S = SWD()
print("=== PHASE 3 — %s wheel, CH=0x%08x, duty=%d, %s ==="
      % (side, ch, duty, "LOOP 3s" if loop else "once 1s"))
print("CONFIRM out loud: wheels off the ground, blade disconnected, hand on the kill switch.")
try:
    input("Enter — start (Ctrl-C — cancel)... ")
except KeyboardInterrupt:
    print("\ncancelled"); S.close(); raise SystemExit(0)

o0 = S.mdw(odom)[0] if odom else None
if o0 is not None:
    print("odom BEFORE = %d" % o0)

try:
    if loop:
        t_end = time.time() + 3.0
        while time.time() < t_end:
            S.mww(ch, duty)
            time.sleep(0.02)
    else:
        S.mww(ch, duty)
        print("wrote CH<-%d, holding 1s..." % duty)
        time.sleep(1.0)
finally:
    S.mww(ch, 0)  # ALWAYS stop
    print("wrote CH<-0 (stop/brake)")

o1 = S.mdw(odom)[0] if odom else None
if o1 is not None:
    print("odom AFTER = %d  (Δ=%d)" % (o1, o1 - o0))
    if o1 != o0:
        print(">>> TICKS RISING + watch with your eyes: if the wheel turned — PWM duty REALLY drives it.")
    else:
        print(">>> no ticks: duty alone does NOT turn it -> Phase 4 (A4963 RUN/direction).")
else:
    print(">>> watch with your eyes: did the wheel turn. (odom address not given)")
print("If something goes wrong — run:  python3 estop.py")
S.close()
