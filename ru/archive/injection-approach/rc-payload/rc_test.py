#!/usr/bin/env python3
"""
rc_test.py — управление/диагностика RC-payload v2 через SWD-mailbox (poll.cfg :4444). НА МАЛИНЕ.
Колёса ВЫВЕШЕНЫ, нож отключён, косилка на питании, в idle.

Mailbox 0x20014000 (v2): +0 hb, +4 magic, +8 rpc_trigger, +0xC rpc_addr, +0x10..0x1C a0..a3,
                         +0x20 rpc_ret, +0x24 rpc_seq, +0x28 rc_active, +0x29 rc_dir, +0x2C rc_speed.

Команды:
  ./rc_test.py hb                 — heartbeat x5 (растёт = hook жив)
  ./rc_test.py state              — hb/hook/payload/CRC/rpc_seq
  ./rc_test.py arm                — записать magic (включить действия payload)
  ./rc_test.py rpc <addr> [a0..a3]— вызвать функцию addr(a0..) через payload, вернуть ret
  ./rc_test.py init-left          — RPC FUN_0805aee0(0,0,0,0)  (init/energize A4963 левого; ret 0=успех)
  ./rc_test.py enable-left        — RPC FUN_0805f604()         (ENABLE левого — ГИПОТЕЗА недостающего шага)
  ./rc_test.py disable-left       — RPC FUN_0805f5e8()         (disable левого)
  ./rc_test.py setleft <dir> <spd>— RPC FUN_0805f620(dir,spd)  (разовая подача скорости)
  ./rc_test.py drive <spd> [dir]  — rc_active=1: payload КАЖДЫЙ ТИК зовёт set_left(dir,spd)
  ./rc_test.py stop               — set_left(dir,0) один тик → rc_active=0 → disable-left (полный стоп)
  ./rc_test.py ch                 — прочитать CH3CV(L)/CH2CV(R)
Диагностика привода (порядок): arm → init-left (ret 0?) → enable-left → drive 0x600 → смотреть колесо → stop.
"""
import sys, time
from swd import SWD

MB = 0x20014000
MAGIC = 0x5243414D
A_INIT_L   = 0x0805aee1  # FUN_0805aee0
A_ENABLE_L = 0x0805f605  # FUN_0805f604
A_DISABLE_L= 0x0805f5e9  # FUN_0805f5e8
A_SETLEFT  = 0x0805f621  # FUN_0805f620

def r1(S, a):
    r = S.cmd("mdw 0x%08x 1" % a)
    for l in r.splitlines():
        if ": " in l and "0x" in l:
            return int(l.replace(chr(0), "").split(": ")[1].strip().split()[0], 16)
    return None

def w(S, a, v): S.cmd("mww 0x%08x 0x%08x" % (a, v & 0xFFFFFFFF))
def arm(S): w(S, MB + 4, MAGIC)

def rpc(S, addr, a0=0, a1=0, a2=0, a3=0):
    arm(S)
    seq0 = r1(S, MB + 0x24) or 0
    w(S, MB + 0x0C, addr); w(S, MB + 0x10, a0); w(S, MB + 0x14, a1)
    w(S, MB + 0x18, a2); w(S, MB + 0x1C, a3)
    w(S, MB + 0x08, 1)                       # trigger
    for _ in range(20):
        if (r1(S, MB + 0x24) or 0) != seq0:  # seq incremented = call done
            ret = r1(S, MB + 0x20)
            print("RPC 0x%08x(a0=0x%x) -> ret=0x%08x (seq %d->%d)" % (addr, a0, ret, seq0, seq0 + 1))
            return ret
        time.sleep(0.15)
    print("RPC 0x%08x — НЕ выполнился (payload не тикает? magic? hook?)" % addr)
    return None

def main():
    cmd = sys.argv[1] if len(sys.argv) > 1 else "state"
    S = SWD()
    if cmd == "hb":
        for _ in range(5): print("hb=0x%08x" % (r1(S, MB) or 0)); time.sleep(0.6)
    elif cmd == "state":
        print("hb    :0x%08x" % (r1(S, MB) or 0))
        print("hook  :0x%08x (want ..f03c/fa77)" % (r1(S, 0x08053b0c) or 0))
        print("payld :0x%08x (want 4a02b500)" % (r1(S, 0x08090000) or 0))
        print("CRC   :0x%08x (want 613a4a93)" % (r1(S, 0x080ffffc) or 0))
        print("rpcseq:0x%08x" % (r1(S, MB + 0x24) or 0))
    elif cmd == "ch":
        print("CH3CV(L)=0x%x CH2CV(R)=0x%x" % (r1(S, 0x40000440) or 0, r1(S, 0x4000043c) or 0))
    elif cmd == "arm":
        arm(S); print("armed (magic set)")
    elif cmd == "rpc":
        a = [int(x, 0) for x in sys.argv[2:]]
        rpc(S, a[0], *(a[1:] + [0, 0, 0, 0])[:4])
    elif cmd == "init-left":    rpc(S, A_INIT_L, 0, 0, 0, 0)
    elif cmd == "enable-left":  rpc(S, A_ENABLE_L)
    elif cmd == "disable-left": rpc(S, A_DISABLE_L)
    elif cmd == "setleft":
        rpc(S, A_SETLEFT, int(sys.argv[2], 0), int(sys.argv[3], 0))
    elif cmd == "drive":
        spd = int(sys.argv[2], 0); d = int(sys.argv[3], 0) if len(sys.argv) > 3 else 0
        arm(S)
        w(S, MB + 0x2C, spd & 0xFFFF)                 # rc_speed (word ok, payload reads u16)
        S.cmd("mww 0x%08x 0x%08x" % (MB + 0x28, (d << 8) | 1))  # rc_active=1@0x28, rc_dir@0x29
        print("DRIVE armed spd=0x%x dir=%d. CH3CV(L)=0x%x" % (spd, d, r1(S, 0x40000440) or 0))
    elif cmd == "stop":
        # 1) обнулить скорость через set_left(dir,0) чтобы снять залоченный PWM
        d = r1(S, MB + 0x29) or 0
        rpc(S, A_SETLEFT, d & 0xFF, 0)
        # 2) снять rc_active
        w(S, MB + 0x28, 0)
        # 3) disable драйвер
        rpc(S, A_DISABLE_L)
        print("STOPPED. CH3CV(L)=0x%x" % (r1(S, 0x40000440) or 0))
    S.close()

if __name__ == "__main__":
    main()
