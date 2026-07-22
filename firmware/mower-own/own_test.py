#!/usr/bin/env python3
"""
own_test.py — управление/проверка СВОЕЙ прошивки mower-own по SWD-mailbox (poll.cfg :4444).
Запуск НА МАЛИНЕ. Колёса ВЫВЕШЕНЫ, нож НЕ включать без нужды, косилка на питании.

Mailbox 0x20014000 (mower-own):
  +0x00 u32 heartbeat  (прошивка++)   +0x04 u32 magic(0x5243414D)   +0x08 u32 seq(dead-man, мы++)
  +0x0C i16 left_speed  +0x0E i16 right_speed  +0x10 u8 blade_on

Команды:
  ./own_test.py hb                 — heartbeat x5 (растёт = наша прошивка жива)
  ./own_test.py state              — heartbeat/CH3CV(L)/CH2CV(R)/CH1CV(blade)
  ./own_test.py drive <L> <R>      — задать скорости (signed, |v|<=0x1c20), взвести, дёргать seq
  ./own_test.py fwd <spd>          — оба вперёд
  ./own_test.py stop               — скорости 0, снять magic
  ./own_test.py blade <0|1>        — нож вкл/выкл (ОСОЗНАННО!)
  ./own_test.py hold               — держать seq живым ~10с (dead-man), чтобы колёса не встали
"""
import sys, time
from swd import SWD
MB=0x20014000; MAGIC=0x5243414D

def r1(S,a):
    r=S.cmd("mdw 0x%08x 1"%a)
    for l in r.splitlines():
        if ": " in l and "0x" in l: return int(l.replace(chr(0),"").split(": ")[1].strip().split()[0],16)
    return None
def w(S,a,v): S.cmd("mww 0x%08x 0x%08x"%(a,v&0xffffffff))
def bump_seq(S): w(S,MB+8,((r1(S,MB+8) or 0)+1)&0xffffffff)
def set_speeds(S,l,r):
    # left@0xC, right@0xE как i16 в одном слове 0xC
    lo=l&0xffff; hi=r&0xffff
    w(S,MB+0xC,(hi<<16)|lo)

def main():
    cmd=sys.argv[1] if len(sys.argv)>1 else "state"
    S=SWD()
    if cmd=="hb":
        for _ in range(5): print("hb=0x%08x"%(r1(S,MB) or 0)); time.sleep(0.6)
    elif cmd=="state":
        print("hb    =0x%08x"%(r1(S,MB) or 0))
        print("CH3(L)=0x%x CH2(R)=0x%x CH1(blade)=0x%x"%(r1(S,0x40000440) or 0,r1(S,0x4000043c) or 0,r1(S,0x40000438) or 0))
    elif cmd in ("drive","fwd"):
        if cmd=="fwd": l=r=int(sys.argv[2],0)
        else: l=int(sys.argv[2],0); r=int(sys.argv[3],0)
        set_speeds(S,l,r); w(S,MB+4,MAGIC); bump_seq(S)
        print("DRIVE L=%d R=%d armed. CH3(L)=0x%x CH2(R)=0x%x"%(l,r,r1(S,0x40000440) or 0,r1(S,0x4000043c) or 0))
    elif cmd=="stop":
        set_speeds(S,0,0); bump_seq(S); time.sleep(0.2); w(S,MB+4,0)  # скорости 0, снять magic
        print("STOP. CH3(L)=0x%x CH2(R)=0x%x"%(r1(S,0x40000440) or 0,r1(S,0x4000043c) or 0))
    elif cmd=="blade":
        on=int(sys.argv[2],0); w(S,MB+0x10,on); w(S,MB+4,MAGIC); bump_seq(S)
        print("BLADE=%d. CH1(blade)=0x%x"%(on,r1(S,0x40000438) or 0))
    elif cmd=="hold":
        for _ in range(20): bump_seq(S); time.sleep(0.5)
        print("held seq ~10s")
    S.close()

if __name__=="__main__": main()
