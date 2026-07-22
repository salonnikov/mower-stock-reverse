#!/usr/bin/env python3
"""
ФАЗА 3 — прямая запись duty в CH3CV (ГЛАВНЫЙ тест: крутит ли PWM колесо).
ПИШЕТ в регистр! Пред-условия: колёса вывешены, нож отключён, рука на обесточке,
Фаза 0 = GO, Фаза 2 понятна.

Запуск (по одному колесу, малым duty):
  python3 phase3_drive.py L 500 0x2000beXX     # side duty odom_addr(из Фазы1)
  python3 phase3_drive.py L 500 0x2000beXX --loop   # если Фаза2 показала перезапись

Аргументы:
  side : L (CH3CV 0x40000440) | R (CH2CV 0x4000043c — ⓗ уточни канал по Фазе0)
  duty : 0..7200 (CAR период ~7200). Начинай с 300-500.
  odom : адрес тика из Фазы 1 (для объективной проверки вращения). Можно опустить.
  --loop : держать duty ~3с, переписывая каждые 20мс (против перезаписи прошивкой).
Всегда в конце пишет 0 (стоп/тормоз).
"""
import sys
import time
from swd import SWD

CH = {"L": 0x40000440, "R": 0x4000043C}

side = (sys.argv[1].upper() if len(sys.argv) > 1 else "L")
duty = int(sys.argv[2]) if len(sys.argv) > 2 else 500
odom = int(sys.argv[3], 0) if len(sys.argv) > 3 and not sys.argv[3].startswith("--") else None
loop = "--loop" in sys.argv
ch = CH.get(side, CH["L"])

if duty < 0 or duty > 7200:
    print("duty вне [0..7200]"); raise SystemExit(1)

S = SWD()
print("=== ФАЗА 3 — %s колесо, CH=0x%08x, duty=%d, %s ==="
      % (side, ch, duty, "ЦИКЛ 3с" if loop else "разово 1с"))
print("ПОДТВЕРДИ голосом: колёса вывешены, нож отключён, рука на обесточивании.")
try:
    input("Enter — пуск (Ctrl-C — отмена)... ")
except KeyboardInterrupt:
    print("\nотмена"); S.close(); raise SystemExit(0)

o0 = S.mdw(odom)[0] if odom else None
if o0 is not None:
    print("odom ДО  = %d" % o0)

try:
    if loop:
        t_end = time.time() + 3.0
        while time.time() < t_end:
            S.mww(ch, duty)
            time.sleep(0.02)
    else:
        S.mww(ch, duty)
        print("записал CH<-%d, держу 1с..." % duty)
        time.sleep(1.0)
finally:
    S.mww(ch, 0)  # ВСЕГДА стоп
    print("записал CH<-0 (стоп/тормоз)")

o1 = S.mdw(odom)[0] if odom else None
if o1 is not None:
    print("odom ПОСЛЕ = %d  (Δ=%d)" % (o1, o1 - o0))
    if o1 != o0:
        print(">>> ТИКИ РАСТУТ + смотри глазами: если колесо крутилось — PWM-duty РЕАЛЬНО вращает.")
    else:
        print(">>> тиков нет: duty сам по себе НЕ крутит -> Фаза 4 (A4963 RUN/направление).")
else:
    print(">>> смотри глазами: крутилось ли колесо. (odom-адрес не задан)")
print("Если что-то не так — запусти:  python3 estop.py")
S.close()
