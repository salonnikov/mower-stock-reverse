// Ghidra Java preScript: SVD peripheral map + SRAM block + vector-table seeding.
// Args: <svd-path>
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.symbol.SourceType;
import ghidra.program.model.symbol.SymbolTable;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import org.w3c.dom.*;
import java.io.File;
import java.util.*;

public class Prep extends GhidraScript {

    long parseNum(String s) {
        if (s == null) return -1;
        s = s.trim().toLowerCase();
        try {
            if (s.startsWith("0x")) return Long.parseLong(s.substring(2), 16);
            return Long.parseLong(s);
        } catch (Exception e) { return -1; }
    }

    String childText(Element el, String tag) {
        NodeList nl = el.getElementsByTagName(tag);
        for (int i = 0; i < nl.getLength(); i++) {
            Node n = nl.item(i);
            if (n.getParentNode() == el) return n.getTextContent().trim();
        }
        return null;
    }

    public void run() throws Exception {
        String[] args = getScriptArgs();
        Memory mem = currentProgram.getMemory();
        SymbolTable st = currentProgram.getSymbolTable();

        // --- SRAM block (GD32F305 CL ~96KB) ---
        try {
            mem.createUninitializedBlock("SRAM", toAddr(0x20000000L), 0x18000L, false);
            println("[prep] SRAM block created");
        } catch (Exception e) { println("[prep] SRAM skip: " + e.getMessage()); }

        // --- SVD peripherals ---
        int perBlocks = 0, regLabels = 0;
        if (args.length > 0) {
            File f = new File(args[0]);
            DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
            DocumentBuilder db = dbf.newDocumentBuilder();
            Document doc = db.parse(f);
            NodeList pers = doc.getElementsByTagName("peripheral");
            // index by name for derivedFrom
            Map<String, Element> byName = new HashMap<>();
            for (int i = 0; i < pers.getLength(); i++) {
                Element p = (Element) pers.item(i);
                String nm = childText(p, "name");
                if (nm != null) byName.put(nm, p);
            }
            for (int i = 0; i < pers.getLength(); i++) {
                Element p = (Element) pers.item(i);
                String name = childText(p, "name");
                long base = parseNum(childText(p, "baseAddress"));
                if (name == null || base < 0) continue;

                Element regSrc = p;
                String df = p.getAttribute("derivedFrom");
                NodeList regs = p.getElementsByTagName("register");
                if (regs.getLength() == 0 && df != null && byName.containsKey(df)) {
                    regSrc = byName.get(df);
                    regs = regSrc.getElementsByTagName("register");
                }
                long maxend = 0x40;
                List<long[]> regOffs = new ArrayList<>();
                List<String> regNames = new ArrayList<>();
                for (int r = 0; r < regs.getLength(); r++) {
                    Element reg = (Element) regs.item(r);
                    long roff = parseNum(childText(reg, "addressOffset"));
                    if (roff < 0) continue;
                    long rsz = 4;
                    String szs = childText(reg, "size");
                    if (szs != null) { long b = parseNum(szs); if (b > 0) rsz = b / 8; }
                    String rnm = childText(reg, "name");
                    if (rnm != null) { regOffs.add(new long[]{roff}); regNames.add(rnm); }
                    if (roff + rsz > maxend) maxend = roff + rsz;
                }
                long blen = (maxend + 3) & ~3L;
                try {
                    MemoryBlock b = mem.createUninitializedBlock(name, toAddr(base), blen, false);
                    b.setRead(true); b.setWrite(true); b.setExecute(false); b.setVolatile(true);
                    perBlocks++;
                } catch (Exception e) { /* overlap */ }
                for (int r = 0; r < regNames.size(); r++) {
                    try {
                        st.createLabel(toAddr(base + regOffs.get(r)[0]), name + "_" + regNames.get(r), SourceType.IMPORTED);
                        regLabels++;
                    } catch (Exception e) {}
                }
            }
        }
        println("[prep] SVD: " + perBlocks + " blocks, " + regLabels + " reg labels");

        // --- vector-table seeding ---
        long FLASH_LO = 0x08000000L, FLASH_HI = 0x08100000L;
        int seeded = 0;
        for (long off = 4; off < 0x200; off += 4) {
            long v;
            try { v = ((long) mem.getInt(toAddr(FLASH_LO + off))) & 0xffffffffL; }
            catch (Exception e) { break; }
            if (v == 0 || v == 0xffffffffL) continue;
            long tgt = v & ~1L;
            if (tgt >= FLASH_LO && tgt < FLASH_HI && (v & 1L) == 1L) {
                try {
                    Address a = toAddr(tgt);
                    disassemble(a);
                    if (getFunctionAt(a) == null) createFunction(a, null);
                    seeded++;
                } catch (Exception e) {}
            }
        }
        println("[prep] vector seed: " + seeded + " handlers");
    }
}
