#!/usr/bin/env python3
"""
build_proof_payload.py — собрать МИНИМАЛЬНЫЙ tick-proof payload для chip1 и врезать
его в образ приложения (из стокового дампа), пересчитав сквозной app-CRC.

Что делает payload: каждые 10мс (через hook в hs-loop) инкрементит u32-счётчик в
mailbox 0x20014000. Это доказывает end-to-end: наш код размещён во флеше, реально
вызывается родным кодом каждый тик (в т.ч. в idle), CRC сходится, косилка грузится —
БЕЗ риска моторами.

Механизм:
  - payload (veneer+tick) кладётся в свободный bank1 0x08090000;
  - hook: единственный BL внутри hs-loop FUN_08053af0 по адресу 0x08053b0e
    (`bl 0x0808368c`, байты 2ff0bdfd) заменяется на `bl 0x08090000`;
  - veneer зовёт оригинал FUN_0808368c (10мс-пейсинг, r0/r1 от caller целы) + наш tick,
    возврат на 0x08053b12;
  - пересчёт app-CRC (0x08018000..0x080ffffc) -> слово 0x080ffffc.

Запуск В КОНТЕЙНЕРЕ (keystone+capstone):
  docker run --rm -v "$PWD":/work python:3.12-slim bash -c \
    "pip install -q keystone-engine capstone && python /work/tools/bench/build_proof_payload.py /work/dist/gd32-mainboard-dump-v1.bin /work/dist/proof_fw.bin"

Выход: <out.bin> (полный образ) + печать 3 регионов под заливку (bank0 hook-страница,
bank1 payload, CRC-слово).
"""
import sys, struct
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB

def thumb_bl(pc, target):
    """32-бит Thumb BL (T1). pc = адрес инструкции BL. Возвращает 4 байта LE."""
    off = target - (pc + 4)
    assert -(1<<24) <= off < (1<<24), "BL вне диапазона"
    imm = (off >> 1) & 0xFFFFFF
    S  = (imm >> 23) & 1
    i1 = (imm >> 22) & 1
    i2 = (imm >> 21) & 1
    imm10 = (imm >> 11) & 0x3FF
    imm11 = imm & 0x7FF
    j1 = (~(i1 ^ S)) & 1
    j2 = (~(i2 ^ S)) & 1
    hw1 = 0xF000 | (S << 10) | imm10
    hw2 = 0xD000 | (j1 << 13) | (j2 << 11) | imm11
    return struct.pack('<HH', hw1, hw2)

BASE        = 0x08000000
PAYLOAD_ADDR= 0x08090000
HOOK_SITE   = 0x08053b0e   # BL FUN_0808368c внутри hs-loop
HOOK_ORIG   = bytes.fromhex("2ff0bdfd")  # ожидаемые байты до патча
FUN_PACE    = 0x0808368c   # FUN_0808368c (thumb)
MAILBOX     = 0x20014000
RET_ADDR    = 0x08053b12   # возврат после оригинального BL
CRC_START, CRC_END = 0x08018000, 0x080ffffc
PAGE0_SIZE  = 0x800

def app_crc(data):
    buf = data[CRC_START-BASE:CRC_END-BASE]
    crc = 0xFFFFFFFF
    for (w,) in struct.iter_unpack('<I', buf):
        crc ^= w
        for _ in range(32):
            crc = ((crc<<1)^0x04C11DB7)&0xFFFFFFFF if (crc&0x80000000) else (crc<<1)&0xFFFFFFFF
    return crc

def main():
    src, out = sys.argv[1], sys.argv[2]
    data = bytearray(open(src,'rb').read())

    # --- veneer + proof_tick по фиксированной раскладке от PAYLOAD_ADDR ---
    # 0x00 veneer:  push {lr}            B500
    # 0x02          bl FUN_0808368c      (4)
    # 0x06          bl proof_tick(0x0C)  (4)
    # 0x0A          pop {pc}             BD00
    # 0x0C proof_tick: ldr r3,[pc,#8]    4B02   (литерал @0x18)
    # 0x0E          ldr r2,[r3]          681A
    # 0x10          adds r2,#1           3201
    # 0x12          str r2,[r3]          601A
    # 0x14          bx lr                4770
    # 0x16          nop (align)          BF00
    # 0x18          .word MAILBOX
    A = PAYLOAD_ADDR
    payload  = bytes.fromhex("00b5")                     # push {lr}
    payload += thumb_bl(A+0x02, FUN_PACE)                # bl 0x0808368c
    payload += thumb_bl(A+0x06, A+0x0C)                  # bl proof_tick
    payload += bytes.fromhex("00bd")                     # pop {pc}
    payload += bytes.fromhex("024b1a680132")             # ldr r3,[pc,#8]; ldr r2,[r3]; adds r2,#1
    payload += bytes.fromhex("1a607047")                 # str r2,[r3]; bx lr
    payload += bytes.fromhex("00bf")                     # nop (align to 0x18)
    payload += struct.pack('<I', MAILBOX)                # literal @0x18
    assert len(payload) == 0x1C, hex(len(payload))

    # --- патч hook-сайта: bl 0x08090000 по адресу HOOK_SITE ---
    cur = bytes(data[HOOK_SITE-BASE:HOOK_SITE-BASE+4])
    assert cur == HOOK_ORIG, f"hook-сайт не совпал: {cur.hex()} != {HOOK_ORIG.hex()}"
    patch = thumb_bl(HOOK_SITE, PAYLOAD_ADDR)
    assert len(patch) == 4, f"патч не 4 байта: {patch.hex()}"

    # --- врезаем ---
    data[PAYLOAD_ADDR-BASE:PAYLOAD_ADDR-BASE+len(payload)] = payload
    data[HOOK_SITE-BASE:HOOK_SITE-BASE+4] = patch

    # --- пересчёт CRC ---
    crc = app_crc(data)
    struct.pack_into('<I', data, CRC_END-BASE, crc)

    open(out,'wb').write(data)

    # --- верификация дизасмом ---
    md = Cs(CS_ARCH_ARM, CS_MODE_THUMB)
    print("== payload @0x%08x (%d bytes) =="%(PAYLOAD_ADDR,len(payload)))
    for ins in md.disasm(payload, PAYLOAD_ADDR):
        print("   0x%08x: %-10s %s %s"%(ins.address, ins.bytes.hex(), ins.mnemonic, ins.op_str))
    print("== hook patch @0x%08x =="%HOOK_SITE)
    for ins in md.disasm(patch, HOOK_SITE):
        print("   0x%08x: %-10s %s %s (было: bl 0x808368c)"%(ins.address, ins.bytes.hex(), ins.mnemonic, ins.op_str))
    print("== new app CRC = 0x%08x -> 0x080ffffc =="%crc)

    # --- регионы под заливку ---
    p0 = HOOK_SITE & ~(PAGE0_SIZE-1)
    print("\n== РЕГИОНЫ ДЛЯ ЗАЛИВКИ ==")
    print("  bank0 hook-страница: 0x%08x..0x%08x (2КБ, erase+program целиком)"%(p0, p0+PAGE0_SIZE-1))
    print("  bank1 payload:       0x%08x..0x%08x (%d байт)"%(PAYLOAD_ADDR, PAYLOAD_ADDR+len(payload)-1, len(payload)))
    print("  CRC-слово:           0x080ffffc = 0x%08x"%crc)
    print("  проверка счётчика после буста: mdw 0x%08x — должен РАСТИ"%MAILBOX)

if __name__ == '__main__':
    main()
