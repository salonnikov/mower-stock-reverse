#!/usr/bin/env python3
"""
pack_snk_mb.py — собрать образ обновления SNK_MB_<ver>.bin для мейнборда chip1.

Модель (Вариант 1, brick-safe канал USB — см. docs/fw/usb-update-format.md):
  * файл на флешке = СЫРОЙ образ приложения (без заголовка/CRC), CRC считает
    загрузчик; версия берётся из ИМЕНИ файла и должна быть > установленной;
  * мы берём стоковый app-регион из полного дампа, вклеиваем блоб-исполнитель
    в свободную (0xFF) область флеша и патчим указатель метода process-manager
    на вход блоба (mx_hook_entry) — при загрузке конструктор поставит наш хук.

ВНИМАНИЕ: базовый адрес блоба и адрес патча помечены [H] в mx_vendor.h/ld и
должны быть подтверждены на живом чипе (SWD) ДО заливки. Скрипт делает строгие
проверки (регион свободен, попадание в app-регион, размеры) и НИЧЕГО не заливает.
"""
import argparse
import struct
import sys

FLASH_BASE = 0x08000000
APP_BASE   = 0x08018000            # начало приложения
APP_MAX    = 0x000E8000            # макс. размер app (928К)
APP_END    = APP_BASE + APP_MAX    # 0x08100000


def fail(msg):
    print("ОШИБКА: " + msg, file=sys.stderr)
    sys.exit(1)


def main():
    ap = argparse.ArgumentParser(description="Сборка SNK_MB для chip1")
    ap.add_argument("--dump", required=True, help="полный дамп 1МБ (стоковый образ)")
    ap.add_argument("--blob", required=True, help="executor.bin (блоб-исполнитель)")
    ap.add_argument("--version", type=int, required=True,
                    help="версия для имени файла (должна быть > установленной)")
    ap.add_argument("--blob-base", type=lambda x: int(x, 0), default=0x080F0000,
                    help="[H] адрес размещения блоба во флеше (свободная область)")
    ap.add_argument("--pmgr-ptr", type=lambda x: int(x, 0), default=0x0804F088,
                    help="[H] адрес флеш-константы указателя метода process-manager")
    ap.add_argument("--out-dir", default=".", help="куда положить SNK_MB_<ver>.bin")
    args = ap.parse_args()

    dump = bytearray(open(args.dump, "rb").read())
    blob = open(args.blob, "rb").read()
    if len(dump) != 0x100000:
        fail(f"дамп ожидается 1МБ, получено {len(dump)}")

    # --- проверки размещения блоба ---
    if not (APP_BASE <= args.blob_base < APP_END):
        fail(f"blob-base 0x{args.blob_base:08x} вне app-региона "
             f"[0x{APP_BASE:08x},0x{APP_END:08x})")
    blob_end = args.blob_base + len(blob)
    if blob_end > APP_END:
        fail(f"блоб не влезает: конец 0x{blob_end:08x} > 0x{APP_END:08x}")

    off = args.blob_base - FLASH_BASE
    region = dump[off:off + len(blob)]
    if any(b != 0xFF for b in region):
        fail(f"область блоба 0x{args.blob_base:08x}+{len(blob)} НЕ свободна "
             f"(есть не-0xFF байты) — выберите другой --blob-base")

    # --- проверка адреса патча указателя ---
    if not (APP_BASE <= args.pmgr_ptr < APP_END - 4):
        fail(f"pmgr-ptr 0x{args.pmgr_ptr:08x} вне app-региона")
    poff = args.pmgr_ptr - FLASH_BASE
    old_ptr = struct.unpack("<I", dump[poff:poff + 4])[0]
    new_ptr = args.blob_base | 1  # thumb-бит

    # --- вклейка блоба и патч указателя ---
    dump[off:off + len(blob)] = blob
    dump[poff:poff + 4] = struct.pack("<I", new_ptr)

    # --- вырезать app-регион как файл обновления ---
    app_img = dump[APP_BASE - FLASH_BASE:APP_END - FLASH_BASE]
    out_name = f"{args.out_dir}/SNK_MB_{args.version}.bin"
    with open(out_name, "wb") as f:
        f.write(app_img)

    print(f"OK: {out_name}")
    print(f"  размер app-образа : {len(app_img)} байт (<= 0x{APP_MAX:x})")
    print(f"  блоб @ 0x{args.blob_base:08x} : {len(blob)} байт (был свободен)")
    print(f"  патч указателя @ 0x{args.pmgr_ptr:08x} : "
          f"0x{old_ptr:08x} -> 0x{new_ptr:08x}")
    print(f"  ВЕРСИЯ в имени = {args.version} (должна быть > установленной!)")
    print("  [H] blob-base/pmgr-ptr подтвердить по SWD перед заливкой.")


if __name__ == "__main__":
    main()
