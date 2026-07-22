#!/usr/bin/env python3
# Оркестратор + ЖИВОЙ веб-дашборд.
#   data:7777 / ctrl:7778 — соединения от mower-auto;  web:8080 — статус в браузере.
# Сам снимает дамп прошивки (esptool через мост), затем переходит в сниф.
import socket, os, pty, select, threading, subprocess, time, re, json
from collections import deque
from http.server import ThreadingHTTPServer, BaseHTTPRequestHandler

DATA_PORT = int(os.environ.get("DATA_PORT", "7777"))
CTRL_PORT = int(os.environ.get("CTRL_PORT", "7778"))
WEB_PORT  = int(os.environ.get("WEB_PORT", "8080"))
OUT = os.environ.get("OUT", "/out")
os.makedirs(OUT, exist_ok=True)

ST = {"phase": "Жду подключения ESP", "data": False, "ctrl": False, "rx": 0, "tx": 0,
      "alive": False, "dumping": False, "dump_size": 0, "dump_total": 4194304,
      "verdict": "", "dump_file": "", "sniff_last": ""}
LOG = deque(maxlen=50)

def log(*a):
    m = " ".join(str(x) for x in a); LOG.append("%s %s" % (time.strftime("%H:%M:%S"), m)); print("[orch]", m, flush=True)

PAGE = """<!doctype html><meta charset=utf-8><meta name=viewport content="width=device-width,initial-scale=1">
<title>mower-auto</title><style>
body{font-family:system-ui,sans-serif;background:#0e1116;color:#cdd9e5;margin:0;padding:16px}
h2{margin:0 0 12px}.row{display:flex;gap:12px;flex-wrap:wrap}
.card{background:#171c24;border:1px solid #2a313c;border-radius:10px;padding:14px;flex:1;min-width:150px}
.k{color:#7a869a;font-size:13px}.v{font-size:22px;font-weight:600;margin-top:4px}
.ok{color:#3fb950}.no{color:#f85149}.warn{color:#d29922}
.bar{height:14px;background:#22272e;border-radius:7px;overflow:hidden;margin-top:6px}
.bar>i{display:block;height:100%;background:#388bfd;width:0}
.dot{display:inline-block;width:10px;height:10px;border-radius:50%;margin-right:6px;vertical-align:middle}
pre{background:#0a0d12;border:1px solid #2a313c;border-radius:8px;padding:10px;max-height:240px;overflow:auto;font-size:12px}
.big{font-size:26px}</style>
<h2>🤖 mower-auto — живой статус</h2>
<div class=row>
 <div class=card><div class=k>ESP (наша плата)</div><div class=v id=esp>—</div></div>
 <div class=card><div class=k>Косилка</div><div class=v id=alive>—</div></div>
 <div class=card><div class=k>Сигнал от косилки</div><div class=v id=rx>0 Б</div></div>
 <div class=card><div class=k>Фаза</div><div class=v id=phase>—</div></div>
</div>
<div class=card style=margin-top:12px>
 <div class=k>Дамп прошивки</div>
 <div class="v big" id=dump>—</div>
 <div class=bar><i id=bar></i></div>
 <div class=v id=verdict style=margin-top:8px></div>
</div>
<div class=card style=margin-top:12px><div class=k>Лог</div><pre id=log></pre></div>
<div class=card style=margin-top:12px><div class=k>Последний сниф (hex)</div><pre id=sniff></pre></div>
<script>
let lastRx=0,aliveTick=0;
async function t(){try{let s=await(await fetch('/state')).json();
 document.getElementById('esp').innerHTML=(s.data?'<span class="dot" style="background:#3fb950"></span>':'<span class="dot" style="background:#f85149"></span>')+(s.data?'подключена':'нет связи');
 let mov=s.rx>lastRx; lastRx=s.rx; if(mov)aliveTick=6; if(aliveTick>0)aliveTick--;
 document.getElementById('alive').innerHTML=(aliveTick>0?'<span class="ok">● ожила, шлёт</span>':(s.rx>0?'<span class="warn">○ молчит</span>':'<span class="no">— тишина</span>'));
 document.getElementById('rx').textContent=s.rx.toLocaleString()+' Б';
 document.getElementById('phase').textContent=s.phase;
 let pct=s.dump_total? Math.min(100,Math.round(s.dump_size*100/s.dump_total)):0;
 document.getElementById('dump').textContent=s.dumping?('идёт... '+pct+'%'):(s.dump_file?'готов':'—');
 document.getElementById('bar').style.width=pct+'%';
 document.getElementById('verdict').innerHTML=s.verdict?('<span class="'+(s.verdict.includes('НОРМ')?'ok':'warn')+'">'+s.verdict+'</span>'):'';
 document.getElementById('log').textContent=s.log.join('\\n');
 document.getElementById('sniff').textContent=s.sniff_last;
}catch(e){}}
setInterval(t,500);t();
</script>"""

class H(BaseHTTPRequestHandler):
    def log_message(self, *a): pass
    def do_GET(self):
        if self.path.startswith("/state"):
            b = json.dumps({**ST, "log": list(LOG)}).encode()
            self.send_response(200); self.send_header("Content-Type", "application/json")
            self.send_header("Content-Length", str(len(b))); self.end_headers(); self.wfile.write(b)
        else:
            b = PAGE.encode("utf-8")
            self.send_response(200); self.send_header("Content-Type", "text/html; charset=utf-8")
            self.send_header("Content-Length", str(len(b))); self.end_headers(); self.wfile.write(b)

threading.Thread(target=lambda: ThreadingHTTPServer(("0.0.0.0", WEB_PORT), H).serve_forever(), daemon=True).start()
log("веб-дашборд на :%d" % WEB_PORT)

def listen(port):
    s = socket.socket(); s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.bind(("0.0.0.0", port)); s.listen(1); return s

ls_data, ls_ctrl = listen(DATA_PORT), listen(CTRL_PORT)
log("жду ESP: data:%d ctrl:%d" % (DATA_PORT, CTRL_PORT))

def handle():
    ST.update(phase="Жду подключения ESP", data=False, ctrl=False, alive=False, dumping=False,
              dump_size=0, verdict="", dump_file="", sniff_last="")
    data, _ = ls_data.accept(); ST["data"] = True; log("ESP data подключилась")
    ctrl, _ = ls_ctrl.accept(); ST["ctrl"] = True; log("ESP ctrl подключилась")
    mfd, sfd = pty.openpty(); slave = os.ttyname(sfd)
    stop = threading.Event(); sniff = threading.Event()
    sf = open(f"{OUT}/sniff.log", "ab", buffering=0)

    def bridge():
        data.setblocking(False)
        while not stop.is_set():
            try: r, _, _ = select.select([data, mfd], [], [], 0.2)
            except Exception: break
            if data in r:                       # ОТ косилки (mower -> мы)
                try: d = data.recv(4096)
                except Exception: d = b''
                if d == b'': break
                if d:
                    os.write(mfd, d); ST["rx"] += len(d)
                    ST["sniff_last"] = " ".join("%02X" % b for b in d[-32:])   # сырьё от косилки, всегда
                    if sniff.is_set():
                        sf.write(("%d %s\n" % (int(time.time()*1000), " ".join("%02X" % b for b in d))).encode())
            if mfd in r:                        # В косилку (esptool -> mower)
                try: d = os.read(mfd, 4096)
                except OSError: d = b''
                if d:
                    ST["tx"] += len(d)
                    try: data.sendall(d)
                    except Exception: break
        stop.set()
    threading.Thread(target=bridge, daemon=True).start()

    ctrl.sendall(b'0'); ST["phase"] = "Готов к дампу — ПЕРЕДЁРНИ ПИТАНИЕ КОСИЛКИ"
    log("P->LOW. >>> ПЕРЕДЁРНИ ПИТАНИЕ КОСИЛКИ <<<")
    ts = time.strftime("%Y%m%d-%H%M%S"); dump = f"{OUT}/mower-esp-{ts}.bin"; ok = False

    def watch_size():
        while not stop.is_set() and ST["dumping"]:
            try: ST["dump_size"] = os.path.getsize(dump)
            except OSError: pass
            time.sleep(0.5)

    for i in range(40):
        if stop.is_set(): break
        log("esptool sync %d/40" % (i+1))
        ST["dumping"] = True; ST["dump_size"] = 0
        threading.Thread(target=watch_size, daemon=True).start()
        r = subprocess.run(["esptool", "--before", "no_reset", "--after", "no_reset", "--chip", "esp32",
                            "--connect-attempts", "2", "--port", slave,
                            "read_flash", "0", "0x400000", dump], capture_output=True, text=True)
        ST["dumping"] = False
        if r.returncode == 0 and os.path.exists(dump) and os.path.getsize(dump) > 1_000_000:
            ok = True; ST["dump_file"] = os.path.basename(dump); ST["dump_size"] = os.path.getsize(dump)
            log("*** ДАМП СНЯТ:", dump); break
        ST["phase"] = "Жду загрузчик (передёрни питание косилки)"; time.sleep(2)

    if ok:
        d = open(dump, "rb").read(); n = len(d); ff = d.count(0xff)*100//n
        stc = len(re.findall(rb'[ -~]{6,}', d))
        v = "НОРМАЛЬНАЯ прошивка — есть протокол!" if (stc > 300 and ff < 90) else ("почти пусто" if ff > 95 else "ВОЗМОЖНО ЗАШИФРОВАНА")
        ST["verdict"] = v; log("вердикт:", v, "(size=%d FF=%d%% strings=%d)" % (n, ff, stc))
    else:
        log("дамп не снят — перехожу в сниф.")

    ctrl.sendall(b'1'); ST["phase"] = "СНИФ (пишу sniff.log)"; sniff.set()
    log("P release. Сниф. (для норм. работы косилки передёрни питание ещё раз)")
    while not stop.is_set(): time.sleep(1)
    log("ESP отключилась.")
    for x in (data, ctrl, sf):
        try: x.close()
        except Exception: pass

while True:
    try: handle()
    except Exception as e: log("ошибка:", e); time.sleep(2)
