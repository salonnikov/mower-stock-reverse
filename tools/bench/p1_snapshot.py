#!/usr/bin/env python3
# P1 snapshot of the chip1 factory firmware state over SWD (WITHOUT halt).
# Uses the proven swd.py module (SWD class, single telnet connection to
# openocd :4444, memory-AP, no halt). Run ON THE PI from ~/mower-bench.
#
#   ./p1_snapshot.py idle      > /tmp/p1_idle.txt 2>&1
#   ./p1_snapshot.py driving   > /tmp/p1_drive.txt 2>&1
#
# Argument = state label (idle/departure/driving/error) — for the header.
# All reads are idempotent/non-invasive, do NOT halt (halt=reset chip1).
# Field meanings: reverse-v2/reports/MEASUREMENT-PLAN-factory-drive.md +
# reference/A4963-DRIVER-*.md.
import sys, time
from swd import SWD

# (label, address, count of 32-bit words)
FIXED = [
    ("state(0x200000bc)",         0x200000bc, 1),
    ("RCU AHBEN/APB2EN/APB1EN",   0x40021014, 3),   # +14 AHBEN, +18 APB2EN, +1c APB1EN
    ("AFIO_PCF0",                 0x40010004, 1),    # expect factory 0x00001C30
    ("SPI1 CTL0/CTL1/STAT",       0x40003800, 3),    # +0 CTL0, +4 CTL1, +8 STAT
    ("SPI1_DATA(diag A4963)",     0x4000380c, 1),    # high bits = diag of the live A4963
    ("TIMER2 CTL0..CH3CV",        0x40000400, 20),   # incl CHCTL0/1/2, CAR, CH0..3CV
    ("GPIOA CTL0/CTL1/ISTAT/OCTL", 0x40010800, 4),
    ("GPIOB CTL0/CTL1/ISTAT/OCTL", 0x40010c00, 4),   # OCTL bit12 = NSS/enable PB12
    ("GPIOC CTL0/CTL1/ISTAT/OCTL", 0x40011000, 4),
    ("GPIOD CTL0/CTL1/ISTAT/OCTL", 0x40011400, 4),   # OCTL bits3/4/5 = CS blade/right/left
    ("GPIOE CTL0/CTL1/ISTAT/OCTL", 0x40011800, 4),
]

# pointers to A4963 driver objects → deref → shadow reg blocks
DRIVERS = [
    ("BLADE", 0x20000540, 0x28),   # reg0..reg7 = obj+0x28..0x36
    ("LEFT",  0x2000054c, 0x30),   # reg0..reg7 = obj+0x30..0x3e
    ("RIGHT", 0x2000057c, 0x30),
]

def rd(S, addr, count=1, tries=5):
    for _ in range(tries):
        v = S.mdw(addr, count)
        if len(v) >= count:
            return v[:count]
        time.sleep(0.2)
    return None

def main():
    label = sys.argv[1] if len(sys.argv) > 1 else "?"
    try:
        S = SWD()
    except Exception as e:
        print("NO LINK to openocd :4444 (%s). Is the mower asleep? Is mower-webctl not stopped?" % e)
        raise SystemExit(1)

    print("=== P1 SNAPSHOT  state=%s  (SWD non-halt, chip1 factory) ===" % label)
    print("(no halt done; values are live)\n")

    for name, addr, cnt in FIXED:
        w = rd(S, addr, cnt)
        if w is None:
            print("%-30s @0x%08x : <not read>" % (name, addr))
        else:
            print("%-30s @0x%08x : %s" % (name, addr, " ".join("%08x" % x for x in w)))

    print("\n--- A4963 driver shadow regs (deref of pointers) ---")
    for name, pptr, blk in DRIVERS:
        p = rd(S, pptr, 1)
        if not p or p[0] == 0 or (p[0] >> 24) != 0x20:
            print("%-6s ptr@0x%08x = %s  (object not created / not in SRAM)" %
                  (name, pptr, ("0x%08x" % p[0]) if p else "<none>"))
            continue
        obj = p[0]
        regs = rd(S, obj + blk, 4)   # reg0..reg7 = 8 halfwords = 4 words
        if regs is None:
            print("%-6s obj@0x%08x  reg-block@+0x%02x: <not read>" % (name, obj, blk))
            continue
        hw = []
        for word in regs:
            hw.append(word & 0xFFFF)
            hw.append((word >> 16) & 0xFFFF)
        print("%-6s obj@0x%08x  reg-block@+0x%02x: reg0..7 = %s   [reg7=%04x]" %
              (name, obj, blk, " ".join("%04x" % r for r in hw[:8]), hw[7]))

    S.close()

if __name__ == "__main__":
    main()
