#!/usr/bin/env python3
# Инъекция+захват bring-up A4963: форс FSM в ходовое (state=4 departure) по SWD и
# частый опрос драйверов/SPI/reg7/duty, чтобы поймать 1-2сек окно, пока косилка
# не свалилась в error. Запускать НА МАЛИНЕ (логгер перед этим остановить — один
# telnet-клиент). state сам вернётся в error/idle (обратимо).
#
#   ./force_capture.py           # форс state=4, захват ~12с
#
# ⚠️ Инвазивно (пишет state). Форсим 4=departure (НЕ 5=cutting) → нож не должен
# включаться. Всё равно: НОЖ ДОЛЖЕН БЫТЬ ФИЗИЧЕСКИ СНЯТ, колёса вывешены.
import time, sys, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from swd import SWD

FORCE_STATE = int(sys.argv[1]) if len(sys.argv) > 1 else 4
DUR = float(sys.argv[2]) if len(sys.argv) > 2 else 12.0

def main():
    S = SWD()
    S.s.settimeout(0.6)   # быстрее (линк здоров); openocd отвечает мгновенно
    def w(a, n=1):
        v = S.mdw(a, n); return v if len(v) >= n else None

    print("=== FORCE+CAPTURE  state->%d  dur=%.0fs ===" % (FORCE_STATE, DUR))
    print("t     st spiC0    spiDAT   L        R        B        Lreg7 ch2  ch3")

    def line(tag):
        st  = w(0x200000bc); spi = w(0x40003800, 4)
        p   = w(0x20000540, 16); t2 = w(0x4000043c, 2)
        lp = p[3] if p else 0; rp = p[15] if p else 0; bp = p[0] if p else 0
        lr = 0
        if lp and (lp >> 24) == 0x20:
            r = w(lp + 0x3c, 1)
            if r: lr = (r[0] >> 16) & 0xFFFF
        print("%-5s %2d %08x %08x %08x %08x %08x %04x  %04x %04x" % (
            tag, (st[0] & 0xFF) if st else -1,
            spi[0] if spi else 0, spi[3] if spi else 0, lp, rp, bp, lr,
            t2[0] if t2 else 0, t2[1] if t2 else 0), flush=True)

    line("base")                       # до форса
    S.cmd("mww 0x200000bc 0x%x" % FORCE_STATE)   # ★ ИНЪЕКЦИЯ
    t0 = time.time()
    while time.time() - t0 < DUR:
        line("%.1f" % (time.time() - t0))
    print("=== конец захвата (state сам вернётся в idle/error) ===")
    S.close()

if __name__ == "__main__":
    main()
