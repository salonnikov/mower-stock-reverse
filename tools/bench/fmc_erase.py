#!/usr/bin/env python3
# Стереть страницу bank1 0x080F0000 через держащий openocd (:4444), вернуть 0xFF, reset run.
import sys, time
from swd import SWD
S=SWD()
def w(a,v): S.cmd("mww 0x%08x 0x%08x"%(a,v))
cur=(S.mdw(0xE0042004) or [0])[0]
w(0xE0042004, cur|0x300)                 # freeze watchdog
w(0x40022044,0x45670123); w(0x40022044,0xCDEF89AB)  # unlock bank1
w(0x40022050,0x00000002); w(0x40022054,0x080F0000); w(0x40022050,0x00000042)  # PER+START
time.sleep(0.3)
w(0x40022050,0x00000000); w(0x40022050,0x00000080)  # clear PER, lock
print("0x080F0000 после стирания:", ["0x%08x"%x for x in (S.mdw(0x080F0000,4) or [])])
if "--reset" in sys.argv:
    print("reset run:", S.cmd("reset run"))
S.close()
