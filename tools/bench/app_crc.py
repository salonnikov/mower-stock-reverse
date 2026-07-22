#!/usr/bin/env python3
"""
app_crc.py — end-to-end HW-CRC-32 of the chip1 (GD32F305) application image.

Host-side tool (run IN A CONTAINER, not on the Mac):
  docker run --rm -v "$PWD":/work:ro python:3.12-slim python /work/tools/bench/app_crc.py /work/dist/gd32-mainboard-dump-v1.bin

Algorithm (MPEG-2 style, CONFIRMED on dump v1 → yields the stock 0x0f69a878):
  poly=0x04C11DB7, init=0xFFFFFFFF, LE words, MSB-first, NO reflect, NO final XOR.
  CRC range: 0x08018000 .. 0x080ffffc (exclusive). Result is stored in the word 0x080ffffc.

The application checks this CRC at startup → any changed byte in the range without updating the word
0x080ffffc = BRICK. Payload-flashing recipe: write the payload → recompute with this tool → write
the new CRC to 0x080ffffc.

Usage:
  app_crc.py <flash_image.bin>            — compute/verify the CRC of the current image (1MB, base 0x08000000)
  app_crc.py <image.bin> --set            — write the recomputed CRC to the word 0x080ffffc (in place, copy)
"""
import sys, struct

BASE = 0x08000000
CRC_START = 0x08018000
CRC_END   = 0x080ffffc   # exclusive; the result is stored here too


def app_crc(data):
    buf = data[CRC_START - BASE: CRC_END - BASE]
    assert len(buf) % 4 == 0, len(buf)
    crc = 0xFFFFFFFF
    for (w,) in struct.iter_unpack('<I', buf):
        crc ^= w
        for _ in range(32):
            crc = ((crc << 1) ^ 0x04C11DB7) & 0xFFFFFFFF if (crc & 0x80000000) else (crc << 1) & 0xFFFFFFFF
    return crc


def main():
    if len(sys.argv) < 2:
        print(__doc__); sys.exit(1)
    path = sys.argv[1]
    data = bytearray(open(path, 'rb').read())
    if len(data) < (CRC_END - BASE + 4):
        print(f"image too small: {len(data)} bytes, expected >= 0x{CRC_END-BASE+4:x}"); sys.exit(2)
    c = app_crc(data)
    off = CRC_END - BASE
    stored = struct.unpack_from('<I', data, off)[0]
    print(f"computed app CRC = 0x{c:08x}")
    print(f"stored @0x080ffffc = 0x{stored:08x}   match={c == stored}")
    if '--set' in sys.argv:
        struct.pack_into('<I', data, off, c)
        outp = path + '.crcfixed'
        open(outp, 'wb').write(data)
        print(f"wrote new CRC 0x{c:08x} to 0x080ffffc → {outp}")


if __name__ == '__main__':
    main()
