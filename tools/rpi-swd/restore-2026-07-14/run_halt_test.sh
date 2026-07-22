#!/bin/bash
# run_halt_test.sh — ловит ребутящееся ядро chip1 и проверяет стабильность halt.
# НЕ пишет во флеш. Повторяет запуск, пока не поймает halt; поймав — даёт тесту
# отработать 15с. Detached, лог /home/pi/halt.log.
DIR="$(cd "$(dirname "$0")" && pwd)"
LOG=/home/pi/halt.log
: > "$LOG"
echo "START halt-test $(date -u +%H:%M:%S)  (ДЕРЖИ кнопку)" >> "$LOG"
sudo systemctl stop mower-openocd    2>/dev/null || true
sudo systemctl stop mower-p1-logger  2>/dev/null || true
sudo pkill -9 -f openocd 2>/dev/null || true
sleep 1
CAUGHT=0
for i in $(seq 1 200); do
  OUT=$(sudo timeout 25 openocd -f "$DIR/halt_stability_test.cfg" 2>&1)
  if echo "$OUT" | grep -q "CAUGHT+HALTED"; then
    echo "--- caught on attempt $i $(date -u +%H:%M:%S) ---" >> "$LOG"
    echo "$OUT" | grep -E "CAUGHT|t=|СТАБИЛЬНО|НЕСТАБИЛЬНО|LOST" >> "$LOG"
    CAUGHT=1
    break
  fi
  sleep 0.2
done
[ "$CAUGHT" = 0 ] && echo "GAVEUP: не поймал halt за 200 попыток $(date -u +%H:%M:%S)" >> "$LOG"
echo "DONE $(date -u +%H:%M:%S)" >> "$LOG"
chmod 666 "$LOG" 2>/dev/null || true
