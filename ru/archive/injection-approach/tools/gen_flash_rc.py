#!/usr/bin/env python3
"""
gen_flash_rc.py — сгенерировать openocd-конфиг заливки RC-payload (flash_rc.cfg)
из готового образа rc_fw.bin. Метод = проверенный flash_proof.cfg (halt + freeze wdg
+ FMC PG-один-раз-на-страницу + mwh + sleep), расширенный на bank0 hook-страницу.

Три страницы (erase + program non-FF полуслов):
  1. bank0 0x08053800 (2КБ) — hook-страница hs-loop (FMC bank0: KEY 0x40022004, CTL 0x40022010, ADDR 0x40022014)
  2. bank1 0x08090000       — payload      (FMC bank1: KEY 0x40022044, CTL 0x40022050, ADDR 0x40022054)
  3. bank1 0x080ff800       — CRC-слово     (bank1)

Запуск: docker run --rm -v "$PWD":/work python:3.12-slim python /work/tools/bench/gen_flash_rc.py \
          /work/dist/rc_fw.bin /work/dist/flash_rc.cfg
"""
import sys, struct

BASE = 0x08000000
PROBE = """adapter driver bcm2835gpio
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
mem2array cr 32 0xE0042004 1
mww 0xE0042004 [expr {$cr(0) | 0x300}]
"""

def prog_page(data, page_addr, key, ctl, addr, page_size=0x800):
    """erase + program non-0xFFFF halfwords страницы page_addr длиной page_size."""
    out = []
    out.append(f"# ==== erase+program page 0x{page_addr:08x} (bank ctl 0x{ctl:08x}) ====")
    out.append(f"mww 0x{key:08x} 0x45670123")
    out.append(f"mww 0x{key:08x} 0xCDEF89AB")
    # erase page
    out.append(f"mww 0x{ctl:08x} 0x00000002; mww 0x{addr:08x} 0x{page_addr:08x}; mww 0x{ctl:08x} 0x00000042")
    out.append("sleep 120")
    out.append(f"mww 0x{ctl:08x} 0x00000000")
    # program
    out.append(f"mww 0x{ctl:08x} 0x00000001")
    n = 0
    for off in range(0, page_size, 2):
        a = page_addr + off
        fo = a - BASE
        hw = struct.unpack_from('<H', data, fo)[0]
        if hw != 0xFFFF:
            out.append(f"mwh 0x{a:08x} 0x{hw:04x}; sleep 6")
            n += 1
    out.append(f"mww 0x{ctl:08x} 0x00000000")
    out.append(f"# programmed {n} halfwords in page 0x{page_addr:08x}")
    return out, n

def main():
    src, outp = sys.argv[1], sys.argv[2]
    data = open(src, 'rb').read()
    lines = [PROBE]
    total = 0
    # 1. bank0 hook page
    l, n = prog_page(data, 0x08053800, 0x40022004, 0x40022010, 0x40022014); lines += l; total += n
    lines.append(f"mww 0x40022010 0x00000080  # lock bank0")
    # 2. bank1 payload page
    l, n = prog_page(data, 0x08090000, 0x40022044, 0x40022050, 0x40022054); lines += l; total += n
    # 3. bank1 CRC page
    l, n = prog_page(data, 0x080ff800, 0x40022044, 0x40022050, 0x40022054); lines += l; total += n
    lines.append(f"mww 0x40022050 0x00000080  # lock bank1")
    lines.append("resume")
    lines.append("shutdown")
    open(outp, 'w').write("\n".join(lines) + "\n")
    print(f"cfg -> {outp}, всего {total} mwh, {len(lines)} строк")

if __name__ == '__main__':
    main()
