#!/usr/bin/env python3
"""
ФАЗА 2 — переписывает ли прошивка CH3CV сама (наблюдение, БЕЗ записи).
Определяет метод инъекции для Фазы 3.
Запуск:  python3 phase2_watch_pwm.py
"""
import time
from swd import SWD

S = SWD()
CH3CV = 0x40000440
N = 20
print("=== ФАЗА 2 — наблюдение CH3CV (%d раз, ~%.0fс) ===" % (N, N * 0.5))
vals = []
for i in range(N):
    v = S.mdw(CH3CV)
    v = v[0] if v else -1
    vals.append(v)
    print("  t=%4.1fс  CH3CV=0x%08x" % (i * 0.5, v & 0xFFFFFFFF))
    time.sleep(0.5)

uniq = set(vals)
print("\n=== ВЕРДИКТ ===")
if len(uniq) == 1:
    print("CH3CV СТАБИЛЕН -> прошивка его сейчас не трогает.")
    print("   => Фаза 3: можно писать duty напрямую, запись удержится.")
else:
    print("CH3CV ДЁРГАЕТСЯ сам (%d разных значений) -> мотор-контур активно командует." % len(uniq))
    print("   => прямая запись будет затираться ~каждые 10мс. Варианты:")
    print("      - писать duty в ЦИКЛе (Фаза 3 с --loop), или")
    print("      - перевести косилку в состояние, где контур НЕ командует скорость.")
S.close()
