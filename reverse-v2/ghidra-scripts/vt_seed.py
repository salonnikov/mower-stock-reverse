# Ghidra Jython preScript: seed disassembly/functions from the Cortex-M vector table.
# Reads vectors at base (0x08000000) and disassembles every handler that points into flash.
from ghidra.program.model.address import AddressSet

mem = currentProgram.getMemory()
FLASH_LO = 0x08000000
FLASH_HI = 0x08100000
base = toAddr(FLASH_LO)

def rd32(off):
    return mem.getInt(toAddr(FLASH_LO + off)) & 0xffffffff

seeded = 0
# vector table: word0 = SP, then handlers. Scan first 0x200 bytes (128 entries).
for off in range(4, 0x200, 4):
    try:
        v = rd32(off)
    except:
        break
    if v == 0 or v == 0xffffffff:
        continue
    if FLASH_LO <= (v & ~1) < FLASH_HI and (v & 1) == 1:  # thumb handler
        tgt = toAddr(v & ~1)
        try:
            disassemble(tgt)
            f = getFunctionAt(tgt)
            if f is None:
                createFunction(tgt, None)
            seeded += 1
        except Exception as e:
            pass

print("[vt_seed] seeded %d handlers from vector table" % seeded)
