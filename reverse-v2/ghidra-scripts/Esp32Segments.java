// Ghidra preScript: lay out an ESP32 (Xtensa LX6) app image in memory.
//
// The program is imported as raw seg3 (IROM) based at 0x400d0020; this script
// adds the remaining five segments of the esp_image at their real load
// addresses, maps the ROM/peripheral regions, applies the ESP-IDF ROM symbol
// map, and marks the entry point so analysis has somewhere to start.
//
// Args: <segsDir> <romSymsCsv> <entryPointHex>
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.symbol.SourceType;
import java.io.*;
import java.util.*;

public class Esp32Segments extends GhidraScript {

    // name, file, addr, size, r, w, x
    private void addFile(String name, File f, long addr, boolean w, boolean x) throws Exception {
        if (!f.exists()) { println("[seg] MISSING " + f); return; }
        Memory mem = currentProgram.getMemory();
        Address a = toAddr(addr);
        try (FileInputStream fis = new FileInputStream(f)) {
            MemoryBlock b = mem.createInitializedBlock(name, a, fis, f.length(), monitor, false);
            b.setRead(true); b.setWrite(w); b.setExecute(x);
            println(String.format("[seg] %-10s @ 0x%08x  %,d bytes  %s", name, addr, f.length(),
                    (x ? "rx" : (w ? "rw" : "r-"))));
        }
    }

    private void addEmpty(String name, long addr, long size, boolean w, boolean x) throws Exception {
        Memory mem = currentProgram.getMemory();
        MemoryBlock b = mem.createUninitializedBlock(name, toAddr(addr), size, false);
        b.setRead(true); b.setWrite(w); b.setExecute(x);
        println(String.format("[map] %-10s @ 0x%08x  %,d bytes", name, addr, size));
    }

    public void run() throws Exception {
        String[] args = getScriptArgs();
        File segs = new File(args.length > 0 ? args[0] : "/work/segs");
        File romCsv = new File(args.length > 1 ? args[1] : "/work/rom_syms.csv");
        long entry = args.length > 2 ? Long.parseLong(args[2].replace("0x", ""), 16) : 0x400814ecL;

        // Segments of the esp_image other than seg3, which is the imported program.
        addFile("drom0",  new File(segs, "seg0_drom_3f400020.bin"), 0x3f400020L, false, false);
        addFile("dram0",  new File(segs, "seg1_dram_3ffbdb60.bin"), 0x3ffbdb60L, true,  false);
        addFile("iram0",  new File(segs, "seg2_iram_40080000.bin"), 0x40080000L, false, true);
        addFile("iram1",  new File(segs, "seg4_iram_4008309c.bin"), 0x4008309cL, false, true);
        addFile("rtc",    new File(segs, "seg5_rtc_50000000.bin"),  0x50000000L, true,  false);

        // Regions the image references but does not carry.
        addEmpty("rom_code", 0x40000000L, 0x00070000L, false, true);   // ESP32 ROM
        addEmpty("rom_data", 0x3ff90000L, 0x00010000L, false, false);
        addEmpty("dram_bss", 0x3ffc4840L, 0x0003b7c0L, true,  false);  // up to 0x3ffe0000
        addEmpty("periph",   0x3ff40000L, 0x00050000L, true,  false);  // peripheral registers

        // ROM symbol map from the matching ESP-IDF (v4.4.3).
        int applied = 0, skipped = 0;
        if (romCsv.exists()) {
            try (BufferedReader r = new BufferedReader(new FileReader(romCsv))) {
                String line;
                while ((line = r.readLine()) != null) {
                    int c = line.indexOf(',');
                    if (c <= 0) continue;
                    String name = line.substring(0, c).trim();
                    long a;
                    try { a = Long.parseLong(line.substring(c + 1).trim().replace("0x", ""), 16); }
                    catch (Exception e) { continue; }
                    Address ad = toAddr(a);
                    if (currentProgram.getMemory().contains(ad)) {
                        try { createLabel(ad, name, true, SourceType.IMPORTED); applied++; }
                        catch (Exception e) { skipped++; }
                    } else skipped++;
                }
            }
        }
        println("[rom] labels applied=" + applied + " skipped=" + skipped);

        // Entry point — lives in iram0, so it must be marked after that block exists.
        Address ep = toAddr(entry);
        if (currentProgram.getMemory().contains(ep)) {
            addEntryPoint(ep);
            createLabel(ep, "call_start_cpu0_entry", true, SourceType.IMPORTED);
            disassemble(ep);
            println(String.format("[entry] 0x%08x marked and disassembled", entry));
        } else {
            println(String.format("[entry] 0x%08x is outside mapped memory", entry));
        }
    }
}
