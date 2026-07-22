#!/bin/bash
# run_identify_catch.sh — ТОЛЬКО ЧТЕНИЕ. Ловит chip1 в окне питания (ты ДЕРЖИШЬ кнопку).
# Гоняет chip1-identify.cfg в цикле, пока не поймает живой чип (DPIDR != deadbeef).
# Detached: переживает обрыв WiFi. Лог /home/pi/identify.log, маркер CAUGHT/GAVEUP.
DIR="$(cd "$(dirname "$0")" && pwd)"
LOG=/home/pi/identify.log
: > "$LOG"
echo "START catch $(date -u +%H:%M:%S)  (ДЕРЖИ кнопку питания!)" >> "$LOG"
sudo systemctl stop mower-openocd    2>/dev/null || true
sudo systemctl stop mower-p1-logger  2>/dev/null || true
sudo pkill -9 -f openocd 2>/dev/null || true
sleep 1
CAUGHT=0
for i in $(seq 1 120); do          # ~120 попыток x ~1c = до 2 минут удержания
  OUT=$(sudo timeout 6 openocd -f "$DIR/chip1-identify.cfg" 2>&1)
  echo "--- attempt $i $(date -u +%H:%M:%S) ---" >> "$LOG"
  echo "$OUT" >> "$LOG"
  if echo "$OUT" | grep -q "ПРОШИВКА ="; then
    echo "CAUGHT on attempt $i $(date -u +%H:%M:%S)" >> "$LOG"
    CAUGHT=1
    break
  fi
  sleep 0.4
done
[ "$CAUGHT" = 0 ] && echo "GAVEUP after 120 attempts $(date -u +%H:%M:%S)" >> "$LOG"
chmod 666 "$LOG" 2>/dev/null || true
