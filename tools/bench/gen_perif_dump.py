#!/usr/bin/env python3
"""
gen_perif_dump.py — generate an openocd-cfg that dumps ALL registers of ALL peripherals
(from the list in GD32F30x_CL.svd) file-by-file into /tmp/perif/<NAME>.bin. Plus the core (SCB/NVIC/SysTick).
Standalone openocd 0.10: mdw does NOT print to the log (goes to the disconnected telnet), whereas dump_image writes
a file — so we read everything via dump_image. We read ONLY the blocks declared in the SVD → we don't touch
reserved holes (which fault the bus).

  python gen_perif_dump.py <svd> <out.cfg>          # halt snapshot (fine for the drive state too: config registers are frozen)
"""
import sys, re
svd = open(sys.argv[1], errors='ignore').read()
out = sys.argv[2]

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
poll off
halt"""

periph = []
for m in re.finditer(r"<peripheral\b[^>]*>(.*?)</peripheral>", svd, re.S):
    body = m.group(1)
    nm = re.search(r"<name>([^<]+)</name>", body)
    base = re.search(r"<baseAddress>([^<]+)</baseAddress>", body)
    szs = re.findall(r"<addressBlock>.*?<size>([^<]+)</size>.*?</addressBlock>", body, re.S)
    if not (nm and base):
        continue
    name = nm.group(1).strip()
    b = int(base.group(1), 0)
    size = max((int(s, 0) for s in szs), default=0x100)
    if size == 0:
        size = 0x100
    # Skip potentially unmapped/unclocked external blocks whose
    # read may fault the bus and abort the dump: EXMC(0xA000..), USBFS(0x5000..),
    # ENET(0x40028..). Everything in 0x40000000..0x40023FFF is mapped; the core 0xE000.. too.
    if 0x40024000 <= b < 0xE0000000:
        continue
    periph.append((name, b, size))

# Cortex-M core (not in SVD peripherals): SysTick+NVIC+SCB region 0xE000E000..0xE000F000
periph.append(("CORE_SCS", 0xE000E000, 0x1000))
periph.append(("DBGMCU",   0xE0042000, 0x20))

L = [HDR]
for name, b, size in periph:
    # dump_image takes the length in bytes; align to a word
    n = (size + 3) & ~3
    L.append(f'dump_image /tmp/perif/{name}.bin 0x{b:08x} {n}')
L.append("shutdown")
open(out, "w").write("\n".join(L) + "\n")
print(f"peripherals: {len(periph)}, lines: {len(L)}")
for name, b, size in periph:
    print(f"  {name:12s} 0x{b:08x} +0x{size:x}")
