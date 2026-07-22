#!/usr/bin/env python3
"""
gen_perif_dump.py — сгенерить openocd-cfg, который дампит ВСЕ регистры ВСЕЙ периферии
(по списку из GD32F30x_CL.svd) пофайлово в /tmp/perif/<NAME>.bin. Плюс ядро (SCB/NVIC/SysTick).
Standalone openocd 0.10: mdw НЕ печатает в лог (уходит в отключённый telnet), а dump_image пишет
файл — поэтому читаем всё через dump_image. Читаем ТОЛЬКО объявленные в SVD блоки → не задеваем
зарезервированные дыры (которые фолтят шину).

  python gen_perif_dump.py <svd> <out.cfg>          # halt-снимок (для драйв-состояния тоже ок: конфиг-регистры застыли)
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
    # Пропустить потенциально не-замапленные/не-затактированные внешние блоки, чьё
    # чтение может фолтнуть шину и оборвать дамп: EXMC(0xA000..), USBFS(0x5000..),
    # ENET(0x40028..). Всё в 0x40000000..0x40023FFF замаплено; ядро 0xE000.. тоже.
    if 0x40024000 <= b < 0xE0000000:
        continue
    periph.append((name, b, size))

# ядро Cortex-M (нет в SVD-periph): SysTick+NVIC+SCB область 0xE000E000..0xE000F000
periph.append(("CORE_SCS", 0xE000E000, 0x1000))
periph.append(("DBGMCU",   0xE0042000, 0x20))

L = [HDR]
for name, b, size in periph:
    # dump_image берёт длину в байтах; выравниваем до слова
    n = (size + 3) & ~3
    L.append(f'dump_image /tmp/perif/{name}.bin 0x{b:08x} {n}')
L.append("shutdown")
open(out, "w").write("\n".join(L) + "\n")
print(f"периферий: {len(periph)}, строк: {len(L)}")
for name, b, size in periph:
    print(f"  {name:12s} 0x{b:08x} +0x{size:x}")
