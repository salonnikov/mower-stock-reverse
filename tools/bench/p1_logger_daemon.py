#!/usr/bin/env python3
# Непрерывный логгер ходового состояния chip1 (SWD non-halt) — ДЕМОН.
# systemd-сервис mower-p1-logger. Пишет построчно (timestamp) в ПЕРСИСТЕНТНЫЙ файл.
# Автономен: не зависит от ssh/wifi (пишет локально); Restart=always + enable →
# переживает краш и РЕБУТ малины (сам встаёт; openocd тоже enabled).
# САМОЛЕЧЕНИЕ: если mem-AP отдаёт нули («плохое окно», часто сразу после буста) —
# делает `chip.cpu arp_examine` (как catchread.py) и продолжает, не залипая на нулях.
import time, os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))  # найти swd.py рядом
from swd import SWD

LOG    = "/home/pi/mower-swd/p1_log.txt"
PERIOD = 0.6   # пауза между сэмплами (плюс read-latency swd.py; дефолт-таймаут 1.2с надёжен)

def ts():
    return time.strftime("%Y-%m-%d %H:%M:%S")

def logline(s):
    with open(LOG, "a", buffering=1) as f:
        f.write("%s  %s\n" % (ts(), s))
    print(ts(), s, flush=True)   # дублируем в journald

def reexamine(S):
    try:
        S.cmd("chip.cpu arp_examine")   # ре-экзамен таргета (цель "chip.cpu", из catchread.py)
        time.sleep(0.2)
    except Exception:
        pass

def sample(S):
    """Возвращает (kind, line). kind: OK / BAD(нули, нужен re-examine) / FAIL(пустое чтение)."""
    def w(a, n=1):
        v = S.mdw(a, n)
        return v if len(v) >= n else None
    apb1 = w(0x4002101c)                 # RCU APB1EN — КАНАРЕЙКА: на живом чипе всегда 0x184e000f
    if apb1 is None:
        return ("FAIL", "READ-FAIL (пустой mdw; линк/таймаут)")
    if apb1[0] == 0:
        return ("BAD", "LINK-BAD (apb1=0, mem-AP нули) → re-examine")
    st   = w(0x200000bc)                 # FSM state
    spi  = w(0x40003800, 4)              # CTL0, CTL1, STAT, DATA(diag A4963)
    ptrs = w(0x20000540, 16)             # [0]=blade [3]=left [15]=right (создан?)
    t2   = w(0x4000043c, 2)              # CH2CV(прав), CH3CV(лев) — duty
    lp = ptrs[3]  if ptrs else 0
    rp = ptrs[15] if ptrs else 0
    bp = ptrs[0]  if ptrs else 0
    lreg7 = 0
    if lp and (lp >> 24) == 0x20:        # left драйвер создан → живой reg7
        r = w(lp + 0x3c, 1)              # слово: [0x3c]=reg6(low)|[0x3e]=reg7(high)
        if r:
            lreg7 = (r[0] >> 16) & 0xFFFF
    line = ("st=%d apb1=%08x spiC0=%08x spiST=%08x spiDAT=%08x "
            "L=%08x R=%08x B=%08x Lreg7=%04x ch2=%04x ch3=%04x") % (
        (st[0] & 0xFF) if st else -1, apb1[0],
        spi[0] if spi else 0, spi[2] if spi else 0, spi[3] if spi else 0,
        lp, rp, bp, lreg7,
        t2[0] if t2 else 0, t2[1] if t2 else 0)
    return ("OK", line)

def main():
    os.makedirs(os.path.dirname(LOG), exist_ok=True)
    logline("=== p1_logger daemon START ===")
    while True:
        try:
            S = SWD()
        except Exception as e:
            logline("NO-OPENOCD (%s) retry" % e)
            time.sleep(3); continue
        try:
            while True:
                kind, line = sample(S)
                logline(line)
                if kind == "BAD":
                    reexamine(S)        # лечим mem-AP-нули, следующий сэмпл уже валиден
                    time.sleep(0.3)
                elif kind == "FAIL":
                    time.sleep(0.5)
                else:
                    time.sleep(PERIOD)
        except Exception as e:
            logline("SWD-ERR (%s) reconnect" % e)
            try: S.close()
            except Exception: pass
            time.sleep(2)

if __name__ == "__main__":
    main()
