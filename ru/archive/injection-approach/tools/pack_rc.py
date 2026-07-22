#!/usr/bin/env python3
"""
pack_rc.py — врезать собранный RC-payload (payload.bin) в образ приложения chip1
и пересчитать сквозной app-CRC. Выдаёт готовый к заливке образ + 3 региона.

Запуск В КОНТЕЙНЕРЕ (capstone):
  docker run --rm -v "$PWD":/work python:3.12-slim bash -c \
    "pip install -q capstone && python /work/tools/bench/pack_rc.py \
       /work/dist/gd32-mainboard-dump-v1.bin /work/firmware/rc-payload/payload.bin /work/dist/rc_fw.bin"
"""
import sys, struct
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB

BASE         = 0x08000000
PAYLOAD_ADDR = 0x08090000
HOOK_SITE    = 0x08053b0e
HOOK_ORIG    = bytes.fromhex("2ff0bdfd")   # bl FUN_0808368c (до патча)
CRC_START, CRC_END = 0x08018000, 0x080ffffc
PAGE0 = 0x800

def thumb_bl(pc, target):
    off = target - (pc + 4)
    assert -(1<<24) <= off < (1<<24)
    imm = (off >> 1) & 0xFFFFFF
    S=(imm>>23)&1; i1=(imm>>22)&1; i2=(imm>>21)&1
    imm10=(imm>>11)&0x3FF; imm11=imm&0x7FF
    j1=(~(i1^S))&1; j2=(~(i2^S))&1
    return struct.pack('<HH', 0xF000|(S<<10)|imm10, 0xD000|(j1<<13)|(j2<<11)|imm11)

def app_crc(data):
    crc=0xFFFFFFFF
    for (w,) in struct.iter_unpack('<I', data[CRC_START-BASE:CRC_END-BASE]):
        crc^=w
        for _ in range(32):
            crc=((crc<<1)^0x04C11DB7)&0xFFFFFFFF if (crc&0x80000000) else (crc<<1)&0xFFFFFFFF
    return crc

def main():
    dump, payload_bin, out = sys.argv[1], sys.argv[2], sys.argv[3]
    data = bytearray(open(dump,'rb').read())
    payload = open(payload_bin,'rb').read()
    assert len(payload) <= 0x6ffff8, "payload не влезает в свободный регион"
    assert PAYLOAD_ADDR-BASE+len(payload) < CRC_END-BASE, "payload налезает на CRC-слово"

    cur = bytes(data[HOOK_SITE-BASE:HOOK_SITE-BASE+4])
    assert cur == HOOK_ORIG, f"hook-сайт не тот: {cur.hex()} != {HOOK_ORIG.hex()}"

    data[PAYLOAD_ADDR-BASE:PAYLOAD_ADDR-BASE+len(payload)] = payload
    data[HOOK_SITE-BASE:HOOK_SITE-BASE+4] = thumb_bl(HOOK_SITE, PAYLOAD_ADDR)
    crc = app_crc(data)
    struct.pack_into('<I', data, CRC_END-BASE, crc)
    open(out,'wb').write(data)

    md = Cs(CS_ARCH_ARM, CS_MODE_THUMB)
    print("== payload %d байт @0x%08x =="%(len(payload), PAYLOAD_ADDR))
    print("== hook patch @0x%08x =="%HOOK_SITE)
    for ins in md.disasm(bytes(data[HOOK_SITE-BASE:HOOK_SITE-BASE+4]), HOOK_SITE):
        print("   %s %s %s  (было: bl 0x808368c)"%(ins.bytes.hex(), ins.mnemonic, ins.op_str))
    print("== новый app CRC = 0x%08x -> 0x080ffffc =="%crc)
    p0 = HOOK_SITE & ~(PAGE0-1)
    print("\n== РЕГИОНЫ ДЛЯ ЗАЛИВКИ ==")
    print("  bank0 hook-страница: 0x%08x..0x%08x (2КБ erase+program)"%(p0,p0+PAGE0-1))
    print("  bank1 payload:       0x%08x..0x%08x (%d б)"%(PAYLOAD_ADDR,PAYLOAD_ADDR+len(payload)-1,len(payload)))
    print("  CRC-слово:           0x080ffffc = 0x%08x"%crc)

if __name__ == '__main__':
    main()
