#!/usr/bin/env python3
"""
АВАРИЙНЫЙ СТОП — обнуляет все CHxCV TIMER2 (duty=0 на всех каналах).
Запуск:  python3 estop.py
Это НЕ заменяет физическое обесточивание — при сомнении рви питание рукой.
"""
from swd import SWD

S = SWD()
for ch in (0x40000434, 0x40000438, 0x4000043C, 0x40000440):  # CH0..3 CV
    S.mww(ch, 0)
print("EMERGENCY: TIMER2 CH0..3 CV = 0 (все duty обнулены).")
S.close()
