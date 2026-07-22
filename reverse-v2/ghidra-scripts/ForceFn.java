// Ghidra Java postScript: force-disassemble + decompile a list of addresses that
// auto-analysis missed. Args: <outdir> <addr1,addr2,...>  (addrs hex, no 0x needed)
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.*;
import java.io.*;

public class ForceFn extends GhidraScript {
    public void run() throws Exception {
        String[] args = getScriptArgs();
        String outdir = args.length > 0 ? args[0] : "/out";
        new File(outdir).mkdirs();
        String[] addrs = args.length > 1 ? args[1].split(",") : new String[0];

        DecompInterface di = new DecompInterface();
        di.setOptions(new DecompileOptions());
        di.toggleCCode(true);
        di.openProgram(currentProgram);

        BufferedWriter out = new BufferedWriter(new FileWriter(new File(outdir, "forced_functions.c")));

        for (String a : addrs) {
            a = a.trim().replace("0x", "");
            long addr = Long.parseLong(a, 16);
            Address ep = toAddr(addr);
            out.write("\n/* ===== FORCE @ 0x" + a + " ===== */\n");
            println("[force] " + a);
            try {
                // clear a generous window then disassemble at entry
                clearListing(ep, toAddr(addr + 0x700));
            } catch (Exception e) { out.write("/* clear failed: " + e.getMessage() + " */\n"); }
            try {
                disassemble(ep);
            } catch (Exception e) { out.write("/* disasm failed: " + e.getMessage() + " */\n"); }
            Function f = getFunctionAt(ep);
            if (f == null) {
                try { f = createFunction(ep, null); } catch (Exception e) { out.write("/* createFunction failed: " + e.getMessage() + " */\n"); }
            }
            if (f == null) {
                out.write("/* no function created; raw disassembly follows */\n");
                Address cur = ep;
                Listing lst = currentProgram.getListing();
                for (int i = 0; i < 120; i++) {
                    Instruction ins = lst.getInstructionAt(cur);
                    if (ins == null) break;
                    out.write(cur + ":  " + ins.toString() + "\n");
                    cur = ins.getMaxAddress().next();
                }
                continue;
            }
            out.write("/* function: " + f.getName() + " size=" + f.getBody().getNumAddresses() + " */\n");
            DecompileResults res = di.decompileFunction(f, 90, monitor);
            if (res != null && res.decompileCompleted()) {
                out.write(res.getDecompiledFunction().getC());
            } else {
                out.write("/* decompile failed; raw disassembly */\n");
                Address cur = ep;
                Listing lst = currentProgram.getListing();
                for (int i = 0; i < 200; i++) {
                    Instruction ins = lst.getInstructionAt(cur);
                    if (ins == null) break;
                    out.write(cur + ":  " + ins.toString() + "\n");
                    cur = ins.getMaxAddress().next();
                }
            }
        }
        out.close();
        println("[force] DONE -> " + outdir + "/forced_functions.c");
    }
}
