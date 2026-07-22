# Ghidra Jython headless postScript: export decompiled C for all functions + metadata.
# Usage: -postScript export_decomp.py <outdir>
import os
from ghidra.app.decompiler import DecompInterface
from ghidra.util.task import ConsoleTaskMonitor

args = getScriptArgs()
outdir = args[0] if args else "/out"
if not os.path.exists(outdir):
    os.makedirs(outdir)

prog = currentProgram
fm = prog.getFunctionManager()
monitor = ConsoleTaskMonitor()

decomp = DecompInterface()
decomp.toggleCCode(True)
decomp.toggleSyntaxTree(True)
decomp.setSimplificationStyle("decompile")
decomp.openProgram(prog)

funcs = list(fm.getFunctions(True))
print("[export] %d functions" % len(funcs))

# 1) combined decompiled C + per-function index
c_all = open(os.path.join(outdir, "decompiled_all.c"), "w")
idx = open(os.path.join(outdir, "functions_index.csv"), "w")
idx.write("addr,name,size,decomp_ok\n")

ok = 0
fail = 0
for i, f in enumerate(funcs):
    ep = f.getEntryPoint()
    name = f.getName()
    size = f.getBody().getNumAddresses()
    res = None
    try:
        res = decomp.decompileFunction(f, 60, monitor)
    except Exception as e:
        res = None
    good = res is not None and res.decompileCompleted()
    idx.write("%s,%s,%d,%d\n" % (ep, name, size, 1 if good else 0))
    c_all.write("\n/* ==== %s @ %s  size=%d ==== */\n" % (name, ep, size))
    if good:
        try:
            c_all.write(res.getDecompiledFunction().getC())
            ok += 1
        except:
            c_all.write("/* getC failed */\n")
            fail += 1
    else:
        c_all.write("/* decompile failed */\n")
        fail += 1
    if i % 200 == 0:
        print("[export] %d/%d (ok=%d fail=%d)" % (i, len(funcs), ok, fail))

c_all.close()
idx.close()
print("[export] decompiled ok=%d fail=%d" % (ok, fail))

# 2) symbols
st = prog.getSymbolTable()
fsym = open(os.path.join(outdir, "symbols.txt"), "w")
for s in st.getAllSymbols(True):
    try:
        fsym.write("%s %s %s\n" % (s.getAddress(), s.getSymbolType(), s.getName()))
    except:
        pass
fsym.close()

# 3) strings (defined data of string type)
from ghidra.program.util import DefinedDataIterator
fstr = open(os.path.join(outdir, "strings.txt"), "w")
try:
    for d in DefinedDataIterator.definedStrings(prog):
        try:
            fstr.write("%s\t%s\n" % (d.getAddress(), d.getValue()))
        except:
            pass
except Exception as e:
    print("[export] strings err %s" % e)
fstr.close()

# 4) memory map
fmm = open(os.path.join(outdir, "memory_map.txt"), "w")
for b in prog.getMemory().getBlocks():
    fmm.write("%s - %s  %s  r=%s w=%s x=%s vol=%s\n" % (
        b.getStart(), b.getEnd(), b.getName(),
        b.isRead(), b.isWrite(), b.isExecute(), b.isVolatile()))
fmm.close()

print("[export] DONE -> %s" % outdir)
