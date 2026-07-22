import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import java.io.*;

public class DecompileAll extends GhidraScript {
    public void run() throws Exception {
        String out = System.getenv("DECOMP_OUT");
        if (out == null) out = "/g/decomp.c";
        PrintWriter w = new PrintWriter(new BufferedWriter(new FileWriter(out)));
        DecompInterface d = new DecompInterface();
        d.openProgram(currentProgram);
        FunctionManager fm = currentProgram.getFunctionManager();
        int n = 0, ok = 0;
        for (Function f : fm.getFunctions(true)) {
            n++;
            w.println("\n// ======== " + f.getName() + " @ " + f.getEntryPoint()
                      + " size=" + f.getBody().getNumAddresses() + " ========");
            try {
                DecompileResults r = d.decompileFunction(f, 60, monitor);
                if (r != null && r.decompileCompleted()) {
                    w.print(r.getDecompiledFunction().getC());
                    ok++;
                } else {
                    w.println("// decompile failed: " + (r==null?"null":r.getErrorMessage()));
                }
            } catch (Exception e) {
                w.println("// exception: " + e);
            }
            w.flush();
        }
        w.close();
        println("DecompileAll: functions=" + n + " decompiled=" + ok + " -> " + out);
    }
}
