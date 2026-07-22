#!/usr/bin/env python3
"""
EMERGENCY STOP — zeros all TIMER2 CHxCV (duty=0 on all channels).
Run:  python3 estop.py
This does NOT replace physical power-off — when in doubt, pull the power by hand.
"""
from swd import SWD

S = SWD()
for ch in (0x40000434, 0x40000438, 0x4000043C, 0x40000440):  # CH0..3 CV
    S.mww(ch, 0)
print("EMERGENCY: TIMER2 CH0..3 CV = 0 (all duty zeroed).")
S.close()
