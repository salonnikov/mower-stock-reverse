#!/bin/bash
# run_latch_hold_test.sh — ловит chip1 в окне удержания кнопки и запускает
# latch_hold_test.cfg (выставляет защёлку по SWD + мониторит 25с).
# Повторяет запуск openocd, пока не поймает живой чип; поймав — даёт тест-сеансу
# отработать целиком (не убивает). Detached, лог /home/pi/latch.log.
DIR="$(cd "$(dirname "$0")" && pwd)"
LOG=/home/pi/latch.log
: > "$LOG"
echo "START latch-test $(date -u +%H:%M:%S)  (ДЕРЖИ кнопку пока не увидишь ASSERTED)" >> "$LOG"
sudo systemctl stop mower-openocd    2>/dev/null || true
sudo systemctl stop mower-p1-logger  2>/dev/null || true
sudo pkill -9 -f openocd 2>/dev/null || true
sleep 1
CAUGHT=0
for i in $(seq 1 120); do
  OUT=$(sudo timeout 40 openocd -f "$DIR/latch_hold_test.cfg" 2>&1)
  echo "--- attempt $i $(date -u +%H:%M:%S) ---" >> "$LOG"
  echo "$OUT" >> "$LOG"
  if echo "$OUT" | grep -q "ASSERTED"; then
    echo "CAUGHT+ASSERTED on attempt $i $(date -u +%H:%M:%S)" >> "$LOG"
    CAUGHT=1
    break
  fi
  sleep 0.3
done
[ "$CAUGHT" = 0 ] && echo "GAVEUP after 120 attempts $(date -u +%H:%M:%S)" >> "$LOG"
chmod 666 "$LOG" 2>/dev/null || true
