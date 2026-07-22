# Ghidra Jython headless script: apply SVD peripheral memory map + register labels.
# Usage (headless): -preScript svd_load.py <path-to.svd>
# Creates volatile uninitialized memory blocks for each peripheral and labels registers.
import xml.etree.ElementTree as ET
from ghidra.program.model.symbol import SourceType

args = getScriptArgs()
if not args:
    print("[svd] no SVD path arg, skipping")
else:
    svd_path = args[0]
    print("[svd] loading %s" % svd_path)
    tree = ET.parse(svd_path)
    root = tree.getroot()

    def txt(el, tag, default=None):
        c = el.find(tag)
        return c.text.strip() if c is not None and c.text else default

    def num(s):
        if s is None: return None
        s = s.strip().lower()
        try:
            if s.startswith("0x"): return int(s, 16)
            return int(s)
        except: return None

    mem = currentProgram.getMemory()
    symtab = currentProgram.getSymbolTable()

    # SRAM block so RAM globals get tracked (GD32F305 CL: ~96KB @ 0x20000000)
    try:
        mem.createUninitializedBlock("SRAM", toAddr(0x20000000), 0x18000, False)
        print("[svd] SRAM block created")
    except Exception as e:
        print("[svd] SRAM block skip: %s" % e)

    # default register/peripheral sizes
    dev_size = num(txt(root, "size")) or 32

    per_count = 0
    reg_count = 0
    # resolve derivedFrom peripherals
    peripherals = {}
    for p in root.iter("peripheral"):
        nm = txt(p, "name")
        peripherals[nm] = p

    for p in root.iter("peripheral"):
        name = txt(p, "name")
        base = num(txt(p, "baseAddress"))
        if base is None:
            continue
        # collect registers, following derivedFrom
        src = p
        df = p.get("derivedFrom")
        regs_el = p.find("registers")
        if regs_el is None and df and df in peripherals:
            src = peripherals[df]
            regs_el = src.find("registers")
        regs = []
        maxend = 0x40  # min block size
        if regs_el is not None:
            for r in regs_el.iter("register"):
                roff = num(txt(r, "addressOffset")) or 0
                rsz = (num(txt(r, "size")) or dev_size) / 8
                rnm = txt(r, "name")
                if rnm:
                    regs.append((roff, rnm))
                if roff + rsz > maxend:
                    maxend = roff + rsz
        # addressBlock size if present
        for ab in p.iter("addressBlock"):
            sz = num(txt(ab, "size"))
            off = num(txt(ab, "offset")) or 0
            if sz and off + sz > maxend:
                maxend = off + sz
        # round up to 4
        blen = (maxend + 3) & ~3
        start = toAddr(base)
        try:
            blk = mem.createUninitializedBlock(name, start, blen, False)
            blk.setRead(True); blk.setWrite(True); blk.setExecute(False)
            blk.setVolatile(True)
            per_count += 1
        except Exception as e:
            # overlap or exists; still try to label
            pass
        for (roff, rnm) in regs:
            try:
                symtab.createLabel(toAddr(base + roff), "%s_%s" % (name, rnm), True, SourceType.IMPORTED)
                reg_count += 1
            except:
                pass
    print("[svd] done: %d peripheral blocks, %d register labels" % (per_count, reg_count))
