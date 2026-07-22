#!/usr/bin/env python3
"""
app_crc.py — сквозной HW-CRC-32 образа приложения chip1 (GD32F305).

Host-side тул (запускать В КОНТЕЙНЕРЕ, не на маке):
  docker run --rm -v "$PWD":/work:ro python:3.12-slim python /work/tools/bench/app_crc.py /work/dist/gd32-mainboard-dump-v1.bin

Алгоритм (MPEG-2 style, ПОДТВЕРЖДЁН на дампе v1 → даёт стоковый 0x0f69a878):
  poly=0x04C11DB7, init=0xFFFFFFFF, слова LE, MSB-first, БЕЗ reflect, БЕЗ финального XOR.
  Диапазон CRC: 0x08018000 .. 0x080ffffc (эксклюзивно). Результат хранится в слове 0x080ffffc.

Приложение при старте сверяет это CRC → любой изменённый байт в диапазоне без обновления слова
0x080ffffc = БРИК. Рецепт заливки payload: записать payload → пересчитать этим тулом → записать
новый CRC в 0x080ffffc.

Использование:
  app_crc.py <flash_image.bin>            — посчитать/сверить CRC текущего образа (1MB, база 0x08000000)
  app_crc.py <image.bin> --set            — записать пересчитанный CRC в слово 0x080ffffc (in place, копия)
"""
import sys, struct

BASE = 0x08000000
CRC_START = 0x08018000
CRC_END   = 0x080ffffc   # эксклюзивно; тут же хранится результат


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
        print(f"образ мал: {len(data)} байт, ожидался >= 0x{CRC_END-BASE+4:x}"); sys.exit(2)
    c = app_crc(data)
    off = CRC_END - BASE
    stored = struct.unpack_from('<I', data, off)[0]
    print(f"computed app CRC = 0x{c:08x}")
    print(f"stored @0x080ffffc = 0x{stored:08x}   match={c == stored}")
    if '--set' in sys.argv:
        struct.pack_into('<I', data, off, c)
        outp = path + '.crcfixed'
        open(outp, 'wb').write(data)
        print(f"записал новый CRC 0x{c:08x} в 0x080ffffc → {outp}")


if __name__ == '__main__':
    main()
