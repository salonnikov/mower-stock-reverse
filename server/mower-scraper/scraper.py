#!/usr/bin/env python3
# Скрапер: опрашивает локальную страницу ESP /data и сохраняет ВСЮ сессию.
# Прошивку не трогаем. Дедуп по содержимому строки (в каждом кадре уникальный ms).
import urllib.request, json, time, os, re
from collections import deque

ESP = os.environ.get("ESP", "http://192.168.10.76/data")
OUT = os.environ.get("OUT", "/logs/mower-session.log")
os.makedirs(os.path.dirname(OUT), exist_ok=True)
f = open(OUT, "a", buffering=1)
recent = deque(maxlen=6000)

def decode(parts):
    out = []
    for h in parts:
        try: c = int(h, 16)
        except Exception: continue
        out.append(chr(c) if 32 <= c < 127 else ".")
    return "".join(out)

print("scraper ->", ESP, "out:", OUT, flush=True)
errs = 0
while True:
    try:
        with urllib.request.urlopen(ESP, timeout=5) as r:
            d = json.loads(r.read().decode("utf-8", "replace"))
        errs = 0
        for ln in d.get("lines", []):
            if ln in recent:
                continue
            recent.append(ln)
            p = ln.split()
            if p and p[0] in ("A", "B"):
                ms = p[2] if len(p) > 2 else "?"
                txt = decode(p[3:])
                f.write("%s %s | %s\n" % (p[0], ms, txt))
            elif p and p[0] == "S":
                f.write(ln + "\n")
    except Exception as e:
        errs += 1
        if errs <= 3 or errs % 30 == 0:
            print("err:", e, flush=True)
    time.sleep(0.4)
