#!/usr/bin/env python3
"""
swd.py — thin client to the running OpenOCD (mower-openocd, telnet :4444).
Run on the PI. Reads/writes memory and registers WITHOUT halt (memory-AP).

All the bench Phase scripts import this module. It changes nothing in the services,
does no halt/reset/shutdown. Only mdw/mww.

Precondition: mower-openocd is up, mower-webctl is STOPPED (single telnet client).
Link check:  python3 swd.py
"""
import socket
import time


class SWD:
    def __init__(self, host="127.0.0.1", port=4444, settle=0.2):
        self.s = socket.create_connection((host, port), timeout=5)
        self.s.settimeout(0.2)
        time.sleep(settle)
        self._read_until_prompt(0.6)  # consume the OpenOCD banner up to the '>' prompt

    def _read_until_prompt(self, deadline=0.8):
        """Reads the OpenOCD response up to the '>' prompt or until silence. Fast."""
        buf = b""
        end = time.time() + deadline
        while time.time() < end:
            try:
                chunk = self.s.recv(4096)
                if not chunk:
                    break
                buf += chunk
                if buf.rstrip().endswith(b">"):
                    break
            except socket.timeout:
                if buf:
                    break
        return buf.decode(errors="replace")

    def cmd(self, c):
        self.s.sendall((c + "\n").encode())
        return self._read_until_prompt().strip()

    def mdw(self, addr, count=1):
        """Reads count 32-bit words. Returns a list of ints."""
        out = self.cmd("mdw 0x%08x %d" % (addr, count))
        vals = []
        for line in out.splitlines():
            if ":" in line:
                for tok in line.split(":", 1)[1].split():
                    try:
                        vals.append(int(tok, 16))
                    except ValueError:
                        pass
        return vals

    def mww(self, addr, val):
        """Writes one 32-bit word."""
        return self.cmd("mww 0x%08x 0x%08x" % (addr, val))

    def close(self):
        try:
            self.s.close()
        except Exception:
            pass


if __name__ == "__main__":
    # Link self-check + quick sanity-read.
    try:
        s = SWD()
    except Exception as e:
        print("NO LINK to OpenOCD :4444 — check: is mower-openocd up? "
              "is mower-webctl stopped? (%s)" % e)
        raise SystemExit(1)
    tgt = s.cmd("targets")
    print("targets:\n" + tgt)
    v = s.mdw(0x200000bc)
    if v:
        print("SANITY: state @0x200000bc = 0x%08x (state=%d)" % (v[0], v[0] & 0xFF))
        print("LINK OK.")
    else:
        print("mdw returned no data — target not examined? Run 'targets' manually, "
              "if needed in openocd: 'gdb_memory_map disable'. Do NOT halt.")
    s.close()
