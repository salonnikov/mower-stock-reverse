#!/usr/bin/env python3
"""
gen_flash_finish.py — ЗАВЕРШИТЬ прерванную заливку: записать ТОЛЬКО bank1 payload
(0x08090000) + CRC-слово (0x080ffffc). bank0 hook-страница уже записана.
Заморозка watchdog — ПРЯМОЙ mww 0xE0042004 0x300 (DBGMCU_CR=0 по дефолту, robust;
прежний [expr] падал). Метод bank1 = проверенный flash_proof.cfg.
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
def page(data, pa, ps=0x800):
    o=[f"# page 0x{pa:08x}"]
    o.append("mww 0x40022044 0x45670123"); o.append("mww 0x40022044 0xCDEF89AB")
    o.append(f"mww 0x40022050 0x00000002; mww 0x40022054 0x{pa:08x}; mww 0x40022050 0x00000042")
    o.append("sleep 120"); o.append("mww 0x40022050 0x00000000")
    o.append("mww 0x40022050 0x00000001")
    n=0
    for off in range(0,ps,2):
        hw=struct.unpack_from('<H',data,pa-BASE+off)[0]
        if hw!=0xFFFF:
            o.append(f"mwh 0x{pa+off:08x} 0x{hw:04x}; sleep 6"); n+=1
    o.append("mww 0x40022050 0x00000000")
    o.append(f"# {n} halfwords")
    return o
def main():
    data=open(sys.argv[1],'rb').read()
    L=[HDR]
    L+=page(data,0x08090000)
    L+=page(data,0x080ff800)
    L.append("mww 0x40022050 0x00000080  # lock bank1")
    # tail: arg3 = 'resume' (плановый апгрейд живой косилки, без десинка) или 'reset' (recovery). default reset.
    tail = sys.argv[3] if len(sys.argv) > 3 else 'reset'
    L.append("resume" if tail == 'resume' else "reset run")
    L.append("shutdown")
    open(sys.argv[2],'w').write("\n".join(L)+"\n")
    print("finish cfg ->", sys.argv[2], "tail=", tail)
if __name__=='__main__': main()
