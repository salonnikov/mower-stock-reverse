#!/usr/bin/env python3
"""
gen_flash_own.py v2 — flash/revert OUR firmware, writing ONLY page 1 (0x08018000).

Rationale (Fable5 review): our app (<2KB) fits entirely in one 2KB page 0x08018000, our
code does NOT check the app-CRC 0x080ffffc, the app bootloader doesn't verify → it suffices to replace
ONLY page 1 (our vector table+code). The rest of the factory app (pages 2..208 + bank1) we don't
touch → it lies dormant (our vectors don't point to it), and revert = restore 1 page.
Brick window ~6s instead of ~20s; revert is trivial.

Safety (review): halt+wdg-freeze with ASSERT DBGMCU==0x300 (otherwise abort BEFORE erase);
BUSY-poll STAT after erase; program "PG-once + mwh + sleep6" (proven method);
READ-BACK VERIFY of the whole page before reset run (mismatch → shutdown WITHOUT reset = reversible).

  flash:  gen_flash_own.py flash  <own_fw.bin>              <out.cfg>
  revert: gen_flash_own.py revert <own_fw.bin> <stock_dump.bin> <out.cfg>
          (revert restores THE SAME NUMBER of pages as flash writes — just those, from the dump)
"""
import sys, struct
BASE=0x08000000
P1=0x08018000          # page 1 (our vector table + code)
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
    else:  # revert: npages from own_fw, DATA from the dump
        ownfw = open(sys.argv[2],'rb').read(); data = open(sys.argv[3],'rb').read(); out = sys.argv[4]
    APP_REGION_END = 0x080ffffc
    # npages = how many pages OUR app actually occupies (per own_fw) — flash and revert touch exactly those
    last_nonff = P1 - BASE
    for off in range(P1-BASE, APP_REGION_END-BASE):
        if ownfw[off] != 0xFF:
            last_nonff = off
    npages = ((last_nonff - (P1-BASE)) // PSIZE) + 1
    span = npages * PSIZE
    region = data[P1-BASE:P1-BASE+span]
    words = struct.unpack('<%dI'%(span//4), region)   # words of all N pages
    hws   = struct.unpack('<%dH'%(span//2), region)   # halfwords
    L=[HDR]
    # --- wdg freeze + ASSERT ---
    # IMPORTANT: freeze ONLY via a direct write. The form `mww 0xE0042004 [expr {$cr(0)|0x300}]`
    # FAILS with an mww usage-error on this openocd (mww is a native command, does not digest
    # [expr] as an argument) → openocd exit rc=1 BEFORE erase. Verified: flash_v2/finish/revert
    # use a direct write and pass. (if/set with [expr] is fine, those are Jim constructs.)
    L.append("mww 0xE0042004 0x00000300")
    L.append("mem2array wd 32 0xE0042004 1")
    L.append('if {[expr {$wd(0) & 0x300}] != 0x300} { echo "ABORT: wdg NOT frozen (DBGMCU=$wd(0))"; shutdown }')
    # --- unlock bank0 ---
    L.append(f"mww 0x{K0:08x} 0x45670123"); L.append(f"mww 0x{K0:08x} 0xCDEF89AB")
    # --- erase each of the N pages + BUSY-poll ---
    for p in range(npages):
        pa = P1 + p*PSIZE
        L.append(f"# erase page 0x{pa:08x}")
        L.append(f"mww 0x{C0:08x} 0x00000002; mww 0x{A0:08x} 0x{pa:08x}; mww 0x{C0:08x} 0x00000042")
        L.append("set busy 1; set n 0")
        L.append(f'while {{$busy && $n < 200}} {{ mem2array s 32 0x{ST0:08x} 1; set busy [expr {{$s(0) & 1}}]; incr n; sleep 1 }}')
        L.append(f'if {{$busy}} {{ echo "ABORT: erase BUSY timeout page 0x{pa:08x}"; mww 0x{C0:08x} 0x00000080; shutdown }}')
        L.append(f"mww 0x{C0:08x} 0x00000000")
    # --- program non-FF halfwords across the whole span (PG once + mwh + sleep6) ---
    L.append(f"mww 0x{C0:08x} 0x00000001")
    nprog=0
    for i,hw in enumerate(hws):
        if hw!=0xFFFF:
            L.append(f"mwh 0x{P1+2*i:08x} 0x{hw:04x}; sleep 6"); nprog+=1
    L.append(f"mww 0x{C0:08x} 0x00000000")
    L.append(f"mww 0x{C0:08x} 0x00000080  # lock bank0")
    # --- READ-BACK VERIFY of all N pages before reset run ---
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
    print(f"pages: {npages} (0x{P1:08x}..0x{P1+span-1:08x}), {nprog} non-FF halfwords, verify {nwords} words")
    print(f"lines: {len(L)}")
    # sanity
    sp=words[0]; rst=words[1]
    print(f"vector SP=0x{sp:08x} Reset=0x{rst:08x}")

if __name__=='__main__': main()
