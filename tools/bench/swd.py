#!/usr/bin/env python3
"""
swd.py — тонкий клиент к работающему OpenOCD (mower-openocd, telnet :4444).
Запускать на МАЛИНЕ. Читает/пишет память и регистры БЕЗ halt (memory-AP).

Все bench-скрипты Фаз импортируют этот модуль. Ничего не меняет в сервисах,
не делает halt/reset/shutdown. Только mdw/mww.

Пред-условие: mower-openocd поднят, mower-webctl ОСТАНОВЛЕН (один telnet-клиент).
Проверка связи:  python3 swd.py
"""
import socket
import time


class SWD:
    def __init__(self, host="127.0.0.1", port=4444, settle=0.2):
        self.s = socket.create_connection((host, port), timeout=5)
        self.s.settimeout(0.2)
        time.sleep(settle)
        self._read_until_prompt(0.6)  # съесть баннер OpenOCD до приглашения '>'

    def _read_until_prompt(self, deadline=0.8):
        """Читает ответ OpenOCD до приглашения '>' или до тишины. Быстро."""
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
        """Читает count 32-битных слов. Возвращает список int."""
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
        """Пишет одно 32-битное слово."""
        return self.cmd("mww 0x%08x 0x%08x" % (addr, val))

    def close(self):
        try:
            self.s.close()
        except Exception:
            pass


if __name__ == "__main__":
    # Самопроверка связи + быстрый sanity-read.
    try:
        s = SWD()
    except Exception as e:
        print("НЕТ СВЯЗИ с OpenOCD :4444 — проверь: mower-openocd поднят? "
              "mower-webctl остановлен? (%s)" % e)
        raise SystemExit(1)
    tgt = s.cmd("targets")
    print("targets:\n" + tgt)
    v = s.mdw(0x200000bc)
    if v:
        print("SANITY: state @0x200000bc = 0x%08x (состояние=%d)" % (v[0], v[0] & 0xFF))
        print("СВЯЗЬ ОК.")
    else:
        print("mdw не вернул данных — target не examined? Запусти 'targets' вручную, "
              "при необходимости в openocd: 'gdb_memory_map disable'. НЕ делай halt.")
    s.close()
