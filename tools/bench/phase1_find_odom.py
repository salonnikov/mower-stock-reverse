#!/usr/bin/env python3
"""
PHASE 1 — odometry calibration (by hand, WITHOUT motors).
Finds the live wheel-tick counter as a field in the driver object (malloc), by diffing
before/after manually rotating the wheel. Yields the absolute tick address for Phase 3.

Procedure:
  1) python3 phase1_find_odom.py save   # snapshot #1
  2) manually turn BOTH wheels (e.g. 10 turns each), then say "done"
  3) python3 phase1_find_odom.py diff    # finds candidate tick fields
"""
import sys
import json
from swd import SWD

WORDS = 0x40  # how many words of the object to dump
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
        print("WARNING: driver object is empty (lmdrv=0x%08x rmdrv=0x%08x). "
              "First bring up the drive service (see Phase 0)." % (lb, rb))
    json.dump(snap, open(SNAP, "w"))
    print("Snapshot #1 saved (%s)." % SNAP)
    print("Now TURN both wheels by hand (e.g. 10 turns each), then:")
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
                print("  +0x%03x @0x%08x: 0x%08x -> 0x%08x  (Δ=%d)  <= tick CANDIDATE"
                      % (i * 4, addr, a, b, d))
                found.append((tag, addr, d))
    print("\nPick the field with a monotonic increase ~= turns*ticks_per_turn.")
    print("Plug its ADDRESS into Phase 3 as the odom address:")
    for tag, addr, d in found:
        print("   %s: 0x%08x (Δ=%d)" % (tag, addr, d))
    if not found:
        print("   nothing changed — the wheel wasn't turned, the object is empty, or the tick is in another structure")
        print("   fallback: look at TIMER3 CNT/capture (0x40000824) or widen WORDS.")
S.close()
