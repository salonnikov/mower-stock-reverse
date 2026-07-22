#!/usr/bin/env python3
"""
ФАЗА 0 — пассивная разведка (ТОЛЬКО чтение, ни одной записи).
Запуск:  python3 phase0_baseline.py
Даёт вердикт GO/NO-GO: активен ли PWM-тракт левого колеса в текущем состоянии.
"""
import time
from swd import SWD

S = SWD()


def rd(name, addr, note=""):
    v = S.mdw(addr)
    v = v[0] if v else -1
    print("  %-26s @0x%08x = 0x%08x %s" % (name, addr, v & 0xFFFFFFFF, note))
    return v


print("=== ФАЗА 0 — пассивная разведка (только чтение) ===")
state = rd("state", 0x200000BC)
print("      -> состояние = %d  (3idle 4departure 5cutting 6findbd 7dock 8charge 9err 10off)"
      % (state & 0xFF))
mb = rd("movebase singleton", 0x20000338, "(0 = движ-сервис НЕ поднят)")
lm = rd("lmdrv obj slot", 0x2000054C, "(0 = левый драйвер НЕ поднят)")
rm = rd("rmdrv obj slot", 0x2000057C)
ctl0 = rd("TIMER2 CTL0", 0x40000400)
print("      -> CEN(бит0) = %d  (1 = счётчик таймера идёт)" % (ctl0 & 1))
chctl2 = rd("TIMER2 CHCTL2", 0x40000420)
print("      -> CH2EN = %d  CH3EN = %d" % ((chctl2 >> 8) & 1, (chctl2 >> 12) & 1))
car = rd("TIMER2 CAR (период)", 0x4000042C)
c1 = S.mdw(0x40000424)
time.sleep(0.2)
c2 = S.mdw(0x40000424)
c1 = c1[0] if c1 else 0
c2 = c2[0] if c2 else 0
print("  %-26s 0x%08x -> 0x%08x  (%s)"
      % ("TIMER2 CNT (2 снимка)", c1, c2, "бежит" if c1 != c2 else "СТОИТ"))
ch2 = rd("TIMER2 CH2CV", 0x4000043C, "(правый? уточнить)")
ch3 = rd("TIMER2 CH3CV", 0x40000440, "(ЛЕВОЕ колесо, duty) <- baseline")

print("\n=== ВЕРДИКТ ===")
problems = []
if mb == 0:
    problems.append("движ-сервис не поднят (movebase=0) — A4963 не сконфигурен; PWM смысла не имеет")
if lm == 0:
    problems.append("левый драйвер не поднят (lmdrv=0)")
if (ctl0 & 1) == 0:
    problems.append("TIMER2 остановлен (CEN=0) — запись CH3CV не даст эффекта")
if ((chctl2 >> 12) & 1) == 0:
    problems.append("CH3 не включён (CH3EN=0) — выход PWM неактивен")
if c1 == c2:
    problems.append("CNT не бежит — таймер стоит")

if not problems:
    print("GO: PWM-тракт левого колеса АКТИВЕН. Можно к Фазе 1 (одометрия) и Фазе 2/3.")
    print("     Запомни baseline CH3CV = 0x%08x, CAR = 0x%08x" % (ch3 & 0xFFFFFFFF, car & 0xFFFFFFFF))
else:
    print("NO-GO для прямого PWM-теста. Причины:")
    for p in problems:
        print("   - " + p)
    print("   -> сперва перевести косилку в состояние, где мотор-контур поднят "
          "(движ-сервис активен), но НЕ в state 5/6 (там border-навигация командует сама).")
S.close()
