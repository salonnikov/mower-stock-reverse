// Dump a disassembly listing for one address range.
// Args: <startHex> <endHex> <outFile>
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.*;
import java.io.*;

public class DumpRange extends GhidraScript {
    public void run() throws Exception {
        String[] a = getScriptArgs();
        Address start = toAddr(Long.parseLong(a[0].replace("0x",""),16));
        Address end   = toAddr(Long.parseLong(a[1].replace("0x",""),16));
        BufferedWriter w = new BufferedWriter(new FileWriter(a[2]));
        Listing l = currentProgram.getListing();
        Address cur = start;
        int n = 0;
        while (cur.compareTo(end) < 0) {
            Instruction ins = l.getInstructionAt(cur);
            if (ins == null) { disassemble(cur); ins = l.getInstructionAt(cur); }
            if (ins == null) {
                Data d = l.getDataAt(cur);
                w.write(cur + "  .data  " + (d != null ? d.toString() : "??") + "\n");
                cur = cur.add(1);
            } else {
                Function f = getFunctionContaining(cur);
                w.write(cur + "  " + ins.toString()
                        + (f != null && f.getEntryPoint().equals(cur) ? "        ; <<< " + f.getName() : "") + "\n");
                cur = cur.add(ins.getLength());
            }
            if (++n > 20000) break;
        }
        w.close();
        println("[dump] " + n + " lines -> " + a[2]);
    }
}
