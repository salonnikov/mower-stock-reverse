#!/bin/bash
# Автономный раннер замера катушки на малине. Все redirect'ы внутри — никакого
# вложенного квотирования через ssh. Пишет лог и маркер DONE, переживает обрыв WiFi.
LOG=/home/pi/coil.log
: > "$LOG"
echo "START $(date -u +%H:%M:%S)" >> "$LOG"
pkill -9 -f openocd 2>/dev/null
sleep 1
timeout 30 openocd -f /home/pi/mower-swd/chip2-coil-read.cfg >> "$LOG" 2>&1
echo "DONE rc=$? $(date -u +%H:%M:%S)" >> "$LOG"
chmod 666 "$LOG" 2>/dev/null
