// Ghidra preScript: define the functions that are only ever reached through a
// pointer.
//
// The application calls FreeRTOS task entries, IDF callbacks and its own
// dispatch tables indirectly: the address sits in an l32r literal and is called
// as (*(code *)PTR_FUN_xxxx)(). Ghidra's flow analysis never reaches those
// bodies, so they end up folded into whatever function precedes them.
//
// This walks every 4-byte word of the loaded image, and where a word holds an
// address inside an executable block whose first byte is the Xtensa windowed
// prologue `entry` (opcode 0x36), it disassembles and creates a function there.
// The prologue check is what keeps this from inventing functions out of data.
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.mem.MemoryBlock;

public class Esp32IndirectFuncs extends GhidraScript {

    private boolean executable(Address a) {
        MemoryBlock b = currentProgram.getMemory().getBlock(a);
        return b != null && b.isExecute() && b.isInitialized();
    }

    public void run() throws Exception {
        Memory mem = currentProgram.getMemory();
        int candidates = 0, created = 0, already = 0;

        for (MemoryBlock blk : mem.getBlocks()) {
            if (!blk.isInitialized()) continue;
            Address a = blk.getStart();
            long off = a.getOffset();
            if ((off & 3) != 0) a = a.add(4 - (off & 3));

            while (a.compareTo(blk.getEnd().subtract(3)) < 0) {
                if (monitor.isCancelled()) break;
                int word;
                try { word = mem.getInt(a); } catch (Exception e) { break; }
                long v = word & 0xffffffffL;
                a = a.add(4);

                // Must look like a code address, 4-byte aligned.
                if ((v & 3) != 0) continue;
                if (v < 0x40000000L || v > 0x401b4267L) continue;

                Address t;
                try { t = toAddr(v); } catch (Exception e) { continue; }
                if (!executable(t)) continue;

                // Xtensa windowed prologue: `entry aX, N` — opcode byte 0x36.
                int first;
                try { first = mem.getByte(t) & 0xff; } catch (Exception e) { continue; }
                if (first != 0x36) continue;
                candidates++;

                Function f = getFunctionAt(t);
                if (f != null) { already++; continue; }

                try {
                    disassemble(t);
                    f = createFunction(t, null);
                    if (f != null) created++;
                } catch (Exception e) { /* skip */ }
            }
        }
        println("[indirect] prologue-matching pointers=" + candidates
                + " already functions=" + already
                + " newly created=" + created);
    }
}
