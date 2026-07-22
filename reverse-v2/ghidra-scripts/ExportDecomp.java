// Ghidra Java postScript: export decompiled C for all functions + metadata.
// Args: <outdir>
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.listing.Data;
import java.io.*;
import java.util.*;

public class ExportDecomp extends GhidraScript {
    public void run() throws Exception {
        String[] args = getScriptArgs();
        String outdir = args.length > 0 ? args[0] : "/out";
        new File(outdir).mkdirs();

        Program prog = currentProgram;
        FunctionManager fm = prog.getFunctionManager();

        DecompInterface di = new DecompInterface();
        DecompileOptions opts = new DecompileOptions();
        di.setOptions(opts);
        di.toggleCCode(true);
        di.toggleSyntaxTree(true);
        di.setSimplificationStyle("decompile");
        di.openProgram(prog);

        List<Function> funcs = new ArrayList<>();
        for (Function f : fm.getFunctions(true)) funcs.add(f);
        println("[export] functions: " + funcs.size());

        BufferedWriter cAll = new BufferedWriter(new FileWriter(new File(outdir, "decompiled_all.c")));
        BufferedWriter idx = new BufferedWriter(new FileWriter(new File(outdir, "functions_index.csv")));
        idx.write("addr,name,size,decomp_ok\n");

        int ok = 0, fail = 0, i = 0;
        for (Function f : funcs) {
            i++;
            String name = f.getName();
            String ep = f.getEntryPoint().toString();
            long size = f.getBody().getNumAddresses();
            DecompileResults res = null;
            boolean good = false;
            try {
                res = di.decompileFunction(f, 60, monitor);
                good = res != null && res.decompileCompleted();
            } catch (Exception e) { good = false; }
            idx.write(ep + "," + name.replace(",", "_") + "," + size + "," + (good ? 1 : 0) + "\n");
            cAll.write("\n/* ==== " + name + " @ " + ep + "  size=" + size + " ==== */\n");
            if (good) {
                try { cAll.write(res.getDecompiledFunction().getC()); ok++; }
                catch (Exception e) { cAll.write("/* getC failed */\n"); fail++; }
            } else { cAll.write("/* decompile failed */\n"); fail++; }
            if (i % 200 == 0) println("[export] " + i + "/" + funcs.size() + " ok=" + ok + " fail=" + fail);
        }
        cAll.close(); idx.close();
        println("[export] decompiled ok=" + ok + " fail=" + fail);

        // symbols
        BufferedWriter sym = new BufferedWriter(new FileWriter(new File(outdir, "symbols.txt")));
        for (Symbol s : prog.getSymbolTable().getAllSymbols(true)) {
            try { sym.write(s.getAddress() + " " + s.getSymbolType() + " " + s.getName() + "\n"); } catch (Exception e) {}
        }
        sym.close();

        // strings (defined data whose value is a String)
        BufferedWriter str = new BufferedWriter(new FileWriter(new File(outdir, "strings.txt")));
        try {
            for (Data d : prog.getListing().getDefinedData(true)) {
                try {
                    Object val = d.getValue();
                    if (val instanceof String) {
                        str.write(d.getAddress() + "\t" + ((String) val).replace("\n", " ").replace("\r", " ") + "\n");
                    }
                } catch (Exception e) {}
            }
        } catch (Exception e) { println("[export] strings err " + e.getMessage()); }
        str.close();

        // memory map
        BufferedWriter mm = new BufferedWriter(new FileWriter(new File(outdir, "memory_map.txt")));
        for (MemoryBlock b : prog.getMemory().getBlocks()) {
            mm.write(b.getStart() + " - " + b.getEnd() + "  " + b.getName()
                + "  r=" + b.isRead() + " w=" + b.isWrite() + " x=" + b.isExecute() + " vol=" + b.isVolatile() + "\n");
        }
        mm.close();
        println("[export] DONE -> " + outdir);
    }
}
