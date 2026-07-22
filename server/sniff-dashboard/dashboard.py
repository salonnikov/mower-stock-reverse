#!/usr/bin/env python3
# Дашборд сниффера НА СЕРВЕРЕ. Принимает поток от uart-sniffer (:9000),
# показывает состояние ESP (RSSI/режим/ошибки) + живые расшифрованные кадры (:8080).
# Только stdlib.
import socket, threading, json, time, os
from collections import deque
from http.server import ThreadingHTTPServer, BaseHTTPRequestHandler

DATA_PORT = int(os.environ.get("DATA_PORT", "9000"))
WEB_PORT  = int(os.environ.get("WEB_PORT", "8080"))
OUT = os.environ.get("OUT", "/out"); os.makedirs(OUT, exist_ok=True)

ST = {"connected": False, "last": 0.0, "rssi": 0, "ssid": "—", "ip": "—", "baud": 0,
      "a": 0, "b": 0, "session": "—", "buf": 0, "drop": 0}
FA, FB = deque(maxlen=25), deque(maxlen=25)   # A=↑(GPIO17), B=↓(GPIO16)

def decode(parts):
    out = []
    for h in parts:
        try: c = int(h, 16)
        except Exception: continue
        out.append(chr(c) if 32 <= c < 127 else ".")
    return "".join(out)

def reader(conn):
    sess = time.strftime("session-%Y%m%d-%H%M%S")   # новая сессия на каждое подключение ESP
    ST["connected"] = True; ST["session"] = sess; ST["a"] = 0; ST["b"] = 0
    FA.clear(); FB.clear()
    f = open(OUT + "/" + sess + ".log", "ab", buffering=0)
    print("новая сессия:", sess, flush=True)
    buf = b""; conn.settimeout(10)
    while True:
        try: d = conn.recv(4096)
        except Exception: break
        if not d: break
        ST["last"] = time.time(); buf += d
        while b"\n" in buf:
            line, buf = buf.split(b"\n", 1)
            f.write(line + b"\n")
            s = line.decode("latin1").strip()
            if not s: continue
            if s[0] == "S":
                try:
                    j = json.loads(s[1:].strip())
                    ST.update(rssi=j.get("rssi", 0), ssid=j.get("ssid", "—"), ip=j.get("ip", "—"),
                              baud=j.get("baud", 0), a=j.get("a", 0), b=j.get("b", 0),
                              buf=j.get("buf", 0), drop=j.get("drop", 0))
                except Exception: pass
            elif s[0] in ("A", "B"):
                p = s.split()
                rec = (p[2] if len(p) > 2 else "") + ": " + decode(p[3:])
                (FA if s[0] == "A" else FB).append(rec)
    ST["connected"] = False; f.close()

def accept_loop():
    ls = socket.socket(); ls.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    ls.bind(("0.0.0.0", DATA_PORT)); ls.listen(1)
    while True:
        c, _ = ls.accept(); reader(c)

PAGE = """<!doctype html><meta charset=utf-8><meta name=viewport content="width=device-width,initial-scale=1">
<title>mower-sniff</title><style>
body{font-family:system-ui,sans-serif;background:#0e1116;color:#cdd9e5;margin:0;padding:14px}
h2{margin:0 0 12px}.row{display:flex;gap:10px;flex-wrap:wrap}
.card{background:#171c24;border:1px solid #2a313c;border-radius:10px;padding:12px;flex:1;min-width:130px}
.k{color:#7a869a;font-size:12px}.v{font-size:20px;font-weight:600;margin-top:4px}
.ok{color:#3fb950}.no{color:#f85149}.warn{color:#d29922}
pre{background:#0a0d12;border:1px solid #2a313c;border-radius:8px;padding:10px;max-height:240px;overflow:auto;font-size:12px;white-space:pre-wrap;word-break:break-all}
.A{color:#6cf}.B{color:#fc6}</style>
<h2>&#128225; mower-sniff &mdash; состояние (сервер)</h2>
<div style="color:#7a869a;margin-bottom:8px">Сессия: <b id=sess>&mdash;</b> &middot; буфер ESP: <span id=buf>0</span> Б &middot; потеряно: <span id=drop>0</span> Б</div>
<div class=row>
 <div class=card><div class=k>ESP</div><div class=v id=esp>&mdash;</div></div>
 <div class=card><div class=k>Wi-Fi (<span id=ssid>&mdash;</span>)</div><div class=v id=rssi>&mdash;</div></div>
 <div class=card><div class=k>Режим</div><div class=v>Сниффер @<span id=bd>&mdash;</span></div></div>
 <div class=card><div class=k>Ошибки</div><div class=v id=err>&mdash;</div></div>
</div>
<div class=row style=margin-top:10px>
 <div class=card><div class=k>Канал &#8593; (A, GPIO17)</div><div class=v id=ca>0</div></div>
 <div class=card><div class=k>Канал &#8595; (B, GPIO16)</div><div class=v id=cb>0</div></div>
</div>
<div class=row style=margin-top:10px>
 <div class=card style=flex:1><div class=k>&#8593; A (расшифровано)</div><pre id=fa></pre></div>
 <div class=card style=flex:1><div class=k>&#8595; B (расшифровано)</div><pre id=fb></pre></div>
</div>
<script>
async function t(){try{let s=await(await fetch('/state')).json();
 document.getElementById('esp').innerHTML=s.online?'<span class=ok>онлайн</span>':'<span class=no>офлайн</span>';
 document.getElementById('ssid').textContent=s.ssid;
 document.getElementById('rssi').innerHTML=s.online?('<span class="'+(s.rssi>-67?'ok':s.rssi>-80?'warn':'no')+'">'+s.rssi+' dBm</span>'):'<span class=no>&mdash;</span>';
 document.getElementById('bd').textContent=s.baud;
 document.getElementById('sess').textContent=s.session;
 document.getElementById('buf').textContent=s.buf;
 document.getElementById('drop').textContent=s.drop;
 document.getElementById('ca').textContent=s.a.toLocaleString();
 document.getElementById('cb').textContent=s.b.toLocaleString();
 let e=[]; if(!s.online)e.push('ESP офлайн'); if(s.online&&s.a==0&&s.b==0)e.push('нет данных от косилки');
 document.getElementById('err').innerHTML=e.length?'<span class=warn>'+e.join(', ')+'</span>':'<span class=ok>нет</span>';
 document.getElementById('fa').textContent=s.fa.join('\\n');
 document.getElementById('fb').textContent=s.fb.join('\\n');
}catch(e){}}
setInterval(t,500);t();
</script>""".encode("utf-8")

class H(BaseHTTPRequestHandler):
    def log_message(self, *a): pass
    def do_GET(self):
        if self.path.startswith("/state"):
            online = ST["connected"] and (time.time() - ST["last"] < 8)
            b = json.dumps({**ST, "online": online, "fa": list(FA), "fb": list(FB)}).encode()
            self.send_response(200); self.send_header("Content-Type", "application/json")
            self.send_header("Content-Length", str(len(b))); self.end_headers(); self.wfile.write(b)
        else:
            self.send_response(200); self.send_header("Content-Type", "text/html; charset=utf-8")
            self.send_header("Content-Length", str(len(PAGE))); self.end_headers(); self.wfile.write(PAGE)

threading.Thread(target=accept_loop, daemon=True).start()
print("dashboard: data:%d web:%d" % (DATA_PORT, WEB_PORT), flush=True)
ThreadingHTTPServer(("0.0.0.0", WEB_PORT), H).serve_forever()
