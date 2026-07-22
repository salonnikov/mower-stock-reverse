#!/usr/bin/env python3
# Быстрый семплер ходового состояния (chip1 factory, SWD non-halt).
# Ловит ПЕРЕХОД idle→drive: когда завод создаёт A4963-драйверы, включает SPI1
# и оживляет мотор-тракт. Одна SWD-сессия, ~1 сэмпл/сек, каждая строка с t+state.
#
#   ./p1_drive_sampler.py 90 > /tmp/p1_drive_loop.txt 2>&1     # семплить 90 сек
#
# Читает только (быстро): state, указатели драйверов, live reg7 левого, SPI1
# CTL0/STAT/DATA(diag), TIMER2 CH2/CH3 duty, RCU APB1EN (SPI1EN бит14), GPIOD OCTL.
# НЕ halt. Смотреть строки где state!=3 (4=departure/5=cutting/6=findborder).
import sys, time
from swd import SWD

DUR = float(sys.argv[1]) if len(sys.argv) > 1 else 90.0

def w(S, a, n=1):
    v = S.mdw(a, n)
    return v if len(v) >= n else None

def main():
    try:
        sys.stdout.reconfigure(line_buffering=True)   # писать каждую строку сразу (переживает kill)
    except Exception:
        pass
    S = SWD()
    print("=== DRIVE SAMPLER  dur=%.0fs  (SWD non-halt, chip1 factory) ===" % DUR)
    print("t     st  apb1EN   spiCTL0  spiSTAT spiDATA  Lptr     Rptr     Bptr     Lreg7 ch2  ch3  gpioDoctl")
    t0 = time.time()
    n = 0
    while time.time() - t0 < DUR:
        t = time.time() - t0
        st   = w(S, 0x200000bc)
        apb1 = w(S, 0x4002101c)                 # RCU APB1EN (bit14=SPI1EN)
        spi  = w(S, 0x40003800, 4)              # CTL0, CTL1, STAT, DATA
        ptrs = w(S, 0x20000540, 16)             # [0]=blade [3]=left [15]=right
        t2   = w(S, 0x4000043c, 2)              # CH2CV, CH3CV
        god  = w(S, 0x4001140c)                 # GPIOD OCTL (CS биты3/4/5)
        stv  = st[0] if st else 0
        ap   = apb1[0] if apb1 else 0
        c0   = spi[0] if spi else 0
        ss   = spi[2] if spi else 0
        sd   = spi[3] if spi else 0
        bp   = ptrs[0] if ptrs else 0
        lp   = ptrs[3] if ptrs else 0
        rp   = ptrs[15] if ptrs else 0
        ch2  = t2[0] if t2 else 0
        ch3  = t2[1] if t2 else 0
        gd   = god[0] if god else 0
        lreg7 = 0
        if lp and (lp >> 24) == 0x20:           # left driver создан → live reg7
            r = w(S, lp + 0x3e - 2, 1)          # слово, содержащее reg7 (halfword @+0x3e)
            if r:
                lreg7 = (r[0] >> 16) & 0xFFFF
        print("%5.1f %2d  %08x %08x %08x %08x %08x %08x %08x %04x  %04x %04x %08x" %
              (t, stv & 0xFF, ap, c0, ss, sd, lp, rp, bp, lreg7, ch2, ch3, gd))
        n += 1
        time.sleep(0.4)
    print("=== %d сэмплов за %.0fс ===" % (n, DUR))
    S.close()

if __name__ == "__main__":
    main()
