#!/usr/bin/env python3
"""Снять образ флеша через работающий openocd (:4444, dump_image), БЕЗ halt.
Usage: ./flash_dump.py <outfile> <addr> <size>   напр. /tmp/cur.bin 0x08000000 0x100000
Только чтение памяти. Ждёт завершения до 300с."""
import sys
import time
import socket

path, addr, size = sys.argv[1], sys.argv[2], sys.argv[3]
s = socket.create_connection(("127.0.0.1", 4444), timeout=5)
s.settimeout(2)
time.sleep(0.3)
try:
    while True:
        if not s.recv(4096):
            break
except socket.timeout:
    pass
s.sendall(("dump_image %s %s %s\n" % (path, addr, size)).encode())
buf = b""
end = time.time() + 300
s.settimeout(3)
while time.time() < end:
    try:
        c = s.recv(4096)
        if not c:
            break
        buf += c
        if buf.rstrip().endswith(b">"):
            break
    except socket.timeout:
        pass
print(buf.decode(errors="replace").strip())
