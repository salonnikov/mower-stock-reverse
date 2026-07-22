#!/usr/bin/env python3
"""
gen_flash_own.py v2 — залить/откатить СВОЮ прошивку, шьём ТОЛЬКО page 1 (0x08018000).

Обоснование (ревью Fable5): наш app (<2КБ) целиком в одной 2КБ-странице 0x08018000, наш
код НЕ проверяет app-CRC 0x080ffffc, бутлоадер app не верифицирует → достаточно заменить
ТОЛЬКО page 1 (наш vector table+код). Остальной заводской app (pages 2..208 + bank1) не
трогаем → он дремлет (наши векторы к нему не идут), а откат = восстановить 1 страницу.
Brick-окно ~6с вместо ~20с; откат тривиален.

Безопасность (ревью): halt+wdg-freeze с ASSERT DBGMCU==0x300 (иначе abort ДО стирания);
BUSY-poll STAT после стирания; program «PG-раз + mwh + sleep6» (проверенный метод);
READ-BACK VERIFY всей страницы перед reset run (mismatch → shutdown БЕЗ reset = обратимо).

  flash:  gen_flash_own.py flash  <own_fw.bin>              <out.cfg>
  revert: gen_flash_own.py revert <own_fw.bin> <stock_dump.bin> <out.cfg>
          (revert восстанавливает СТОЛЬКО ЖЕ страниц, сколько шьёт flash — только их из дампа)
"""
import sys, struct
BASE=0x08000000
P1=0x08018000          # page 1 (наш vector table + код)
PSIZE=0x800
K0=0x40022004; C0=0x40022010; A0=0x40022014; ST0=0x4002200C

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
halt
"""

def main():
    mode = sys.argv[1]
    if mode == "flash":
        ownfw = open(sys.argv[2],'rb').read(); data = ownfw; out = sys.argv[3]
    else:  # revert: npages из own_fw, ДАННЫЕ из дампа
        ownfw = open(sys.argv[2],'rb').read(); data = open(sys.argv[3],'rb').read(); out = sys.argv[4]
    APP_REGION_END = 0x080ffffc
    # npages = сколько страниц реально занимает НАШ app (по own_fw) — flash и revert трогают ровно их
    last_nonff = P1 - BASE
    for off in range(P1-BASE, APP_REGION_END-BASE):
        if ownfw[off] != 0xFF:
            last_nonff = off
    npages = ((last_nonff - (P1-BASE)) // PSIZE) + 1
    span = npages * PSIZE
    region = data[P1-BASE:P1-BASE+span]
    words = struct.unpack('<%dI'%(span//4), region)   # слова всех N страниц
    hws   = struct.unpack('<%dH'%(span//2), region)   # полуслова
    L=[HDR]
    # --- wdg freeze + ASSERT ---
    # ВАЖНО: freeze ТОЛЬКО прямой записью. Форма `mww 0xE0042004 [expr {$cr(0)|0x300}]`
    # ПАДАЕТ с mww usage-error на этом openocd (mww — нативная команда, не переваривает
    # [expr] как аргумент) → openocd exit rc=1 ДО стирания. Проверено: flash_v2/finish/revert
    # используют прямую запись и проходят. (if/set с [expr] — ок, они Jim-конструкции.)
    L.append("mww 0xE0042004 0x00000300")
    L.append("mem2array wd 32 0xE0042004 1")
    L.append('if {[expr {$wd(0) & 0x300}] != 0x300} { echo "ABORT: wdg NOT frozen (DBGMCU=$wd(0))"; shutdown }')
    # --- unlock bank0 ---
    L.append(f"mww 0x{K0:08x} 0x45670123"); L.append(f"mww 0x{K0:08x} 0xCDEF89AB")
    # --- erase каждой из N страниц + BUSY-poll ---
    for p in range(npages):
        pa = P1 + p*PSIZE
        L.append(f"# erase page 0x{pa:08x}")
        L.append(f"mww 0x{C0:08x} 0x00000002; mww 0x{A0:08x} 0x{pa:08x}; mww 0x{C0:08x} 0x00000042")
        L.append("set busy 1; set n 0")
        L.append(f'while {{$busy && $n < 200}} {{ mem2array s 32 0x{ST0:08x} 1; set busy [expr {{$s(0) & 1}}]; incr n; sleep 1 }}')
        L.append(f'if {{$busy}} {{ echo "ABORT: erase BUSY timeout page 0x{pa:08x}"; mww 0x{C0:08x} 0x00000080; shutdown }}')
        L.append(f"mww 0x{C0:08x} 0x00000000")
    # --- program non-FF halfwords во всём span (PG once + mwh + sleep6) ---
    L.append(f"mww 0x{C0:08x} 0x00000001")
    nprog=0
    for i,hw in enumerate(hws):
        if hw!=0xFFFF:
            L.append(f"mwh 0x{P1+2*i:08x} 0x{hw:04x}; sleep 6"); nprog+=1
    L.append(f"mww 0x{C0:08x} 0x00000000")
    L.append(f"mww 0x{C0:08x} 0x00000080  # lock bank0")
    # --- READ-BACK VERIFY всех N страниц перед reset run ---
    nwords = span // 4
    L.append(f"mem2array rb 32 0x{P1:08x} {nwords}")
    exp=" ".join("0x%08x"%w for w in words)
    L.append(f"set exp {{{exp}}}")
    L.append("set bad 0")
    L.append("for {set i 0} {$i < %d} {incr i} { if {$rb($i) != [lindex $exp $i]} { echo \"VERIFY MISMATCH word $i (got $rb($i))\"; set bad 1 } }" % nwords)
    L.append('if {$bad} { echo "ABORT: verify failed, NOT resetting (recoverable via revert)"; shutdown }')
    L.append(f'echo "VERIFY OK: {npages} page(s) match image"')
    L.append("reset run")
    L.append("shutdown")
    open(out,'w').write("\n".join(L)+"\n")
    print(f"[{mode}] cfg -> {out}")
    print(f"страниц: {npages} (0x{P1:08x}..0x{P1+span-1:08x}), {nprog} non-FF полуслов, verify {nwords} слов")
    print(f"строк: {len(L)}")
    # sanity
    sp=words[0]; rst=words[1]
    print(f"vector SP=0x{sp:08x} Reset=0x{rst:08x}")

if __name__=='__main__': main()
