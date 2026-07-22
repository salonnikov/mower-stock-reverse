#!/usr/bin/env python3
"""
gen_flash_flat.py — сгенерить "плоский" (flat) openocd-cfg для заливки СВОЕГО app,
совместимый с openocd 0.10 на малине (без Tcl if/while/for/mem2array/[expr]).

Формат = как рабочий flash_own_flat.cfg (2026-07-07): HDR+`poll off`+halt →
wdg-freeze прямой mww → unlock bank0 → erase N страниц (mww SER/AR/START + sleep 100)
→ program (PG; per non-FF полуслово `mwh; sleep 6`; PG off; lock) → shutdown.
БЕЗ reset run (после флеша — физический power-cycle) и БЕЗ in-cfg verify
(верификация отдельным read-дампом).

  docker run --rm -v /Users/denis/MOWER:/work python:3.12-slim \
    python /work/tools/bench/gen_flash_flat.py /work/firmware/mower-own/test-images/app-XXX.bin /work/<out>.cfg
"""
import sys
BASE=0x08000000
P1=0x08018000
PSIZE=0x800
K0=0x40022004; C0=0x40022010; A0=0x40022014   # FMC KEY / CTL / ADDR

HDR="""adapter driver bcm2835gpio
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

def main():
    app = open(sys.argv[1],'rb').read()
    out = sys.argv[2]
    npages = (len(app) + PSIZE - 1)//PSIZE
    span = npages*PSIZE
    region = app + b'\xff'*(span-len(app))    # pad to page boundary with erased 0xFF
    L=[HDR]
    L.append("mww 0xE0042004 0x00000300")   # DBGMCU_CR: freeze watchdogs on halt
    L.append(f"mww 0x{K0:08x} 0x45670123")
    L.append(f"mww 0x{K0:08x} 0xCDEF89AB")
    for p in range(npages):
        pa=P1+p*PSIZE
        L.append(f"mww 0x{C0:08x} 0x00000002")   # SER
        L.append(f"mww 0x{A0:08x} 0x{pa:08x}")   # page addr
        L.append(f"mww 0x{C0:08x} 0x00000042")   # SER|START
        L.append("sleep 100")
        L.append(f"mww 0x{C0:08x} 0x00000000")
    L.append(f"mww 0x{C0:08x} 0x00000001")       # PG
    nprog=0
    for i in range(0,span,2):
        hw=region[i]|(region[i+1]<<8)
        if hw!=0xFFFF:
            L.append(f"mwh 0x{P1+i:08x} 0x{hw:04x}; sleep 6"); nprog+=1
    L.append(f"mww 0x{C0:08x} 0x00000000")
    # NB: NO inline comment here — openocd 0.10/Jim treats a trailing `# ...` as
    # extra mww args => usage error (killed the whole run at the last line).
    L.append(f"mww 0x{C0:08x} 0x00000080")   # lock bank0 (comment kept OUT of the emitted line)
    L.append("shutdown")
    open(out,'w').write("\n".join(L)+"\n")
    sp=int.from_bytes(app[0:4],'little'); rst=int.from_bytes(app[4:8],'little')
    print(f"[flat] {out}: {npages} page(s) 0x{P1:08x}..0x{P1+span-1:08x}, {nprog} non-FF полуслов, {len(L)} строк")
    print(f"vector SP=0x{sp:08x} Reset=0x{rst:08x}")

if __name__=='__main__': main()
