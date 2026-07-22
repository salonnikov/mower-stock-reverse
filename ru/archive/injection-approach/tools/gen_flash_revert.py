#!/usr/bin/env python3
"""
gen_flash_revert.py — ОТКАТ на сток: вернуть 3 региона к заводскому образу.
Источник = стоковый дамп gd32-mainboard-dump-v1.bin.
  1. bank0 hook-страница 0x08053800 — стереть + записать СТОКОВОЕ содержимое (вернёт BL 2ff0bdfd)
  2. bank1 payload-страница 0x08090000 — стереть (сток там = 0xFF, программировать нечего)
  3. bank1 CRC-страница 0x080ff800 — стереть + записать стоковый CRC 0x0f69a878 @0x080ffffc
Заморозка wdg — ПРЯМОЙ mww 0xE0042004 0x300. Метод = проверенный flash_proof.cfg.

Запуск: docker run --rm -v "$PWD":/work python:3.12-slim python /work/tools/bench/gen_flash_revert.py \
          /work/dist/gd32-mainboard-dump-v1.bin /work/dist/flash_revert.cfg
"""
import sys, struct
BASE = 0x08000000
HDR = """adapter driver bcm2835gpio
bcm2835gpio_peripheral_base 0x3F000000
bcm2835gpio_speed_coeffs 146203 36
bcm2835gpio_swd_nums 25 24
transport select swd
adapter speed 1000
swd newdap chip cpu -enable
dap create chip.dap -chain-position chip.cpu
target create chip.cpu cortex_m -dap chip.dap
gdb_port disabled
tcl_port disabled
telnet_port disabled
init
halt
mww 0xE0042004 0x00000300
"""

def erase_only(pa, key, ctl, addr):
    return [f"# erase-only page 0x{pa:08x}",
            f"mww 0x{key:08x} 0x45670123", f"mww 0x{key:08x} 0xCDEF89AB",
            f"mww 0x{ctl:08x} 0x00000002; mww 0x{addr:08x} 0x{pa:08x}; mww 0x{ctl:08x} 0x00000042",
            "sleep 120", f"mww 0x{ctl:08x} 0x00000000"]

def erase_prog(data, pa, key, ctl, addr, ps=0x800):
    o = erase_only(pa, key, ctl, addr)
    o[0] = f"# erase+program page 0x{pa:08x}"
    o.append(f"mww 0x{ctl:08x} 0x00000001")
    n = 0
    for off in range(0, ps, 2):
        hw = struct.unpack_from('<H', data, pa - BASE + off)[0]
        if hw != 0xFFFF:
            o.append(f"mwh 0x{pa+off:08x} 0x{hw:04x}; sleep 6"); n += 1
    o.append(f"mww 0x{ctl:08x} 0x00000000")
    o.append(f"# {n} halfwords")
    return o, n

def main():
    data = open(sys.argv[1], 'rb').read()
    L = [HDR]
    # bank0 hook page — restore stock content
    l, n0 = erase_prog(data, 0x08053800, 0x40022004, 0x40022010, 0x40022014); L += l
    L.append("mww 0x40022010 0x00000080  # lock bank0")
    # bank1 payload page — erase only (stock=FF)
    L += erase_only(0x08090000, 0x40022044, 0x40022050, 0x40022054)
    # bank1 CRC page — restore stock CRC 0x0f69a878
    l, nC = erase_prog(data, 0x080ff800, 0x40022044, 0x40022050, 0x40022054); L += l
    L.append("mww 0x40022050 0x00000080  # lock bank1")
    L.append("reset run"); L.append("shutdown")
    open(sys.argv[2], 'w').write("\n".join(L) + "\n")
    # sanity: stock CRC and hook bytes
    crc = struct.unpack_from('<I', data, 0x080ffffc - BASE)[0]
    hook = data[0x08053b0e-BASE:0x08053b0e-BASE+4].hex()
    print(f"revert cfg -> {sys.argv[2]}  (hook {n0} hw, CRC {nC} hw)")
    print(f"stock CRC @0x080ffffc = 0x{crc:08x}; stock hook bytes @0x08053b0e = {hook} (want 2ff0bdfd)")

if __name__ == '__main__':
    main()
