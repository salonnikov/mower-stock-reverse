#!/bin/bash
DIR="$(cd "$(dirname "$0")" && pwd)"
LOG=/home/pi/halt_read.log
: > "$LOG"
echo "START halt-read $(date -u +%H:%M:%S) (ДЕРЖИ кнопку)" >> "$LOG"
sudo systemctl stop mower-openocd 2>/dev/null || true
sudo systemctl stop mower-p1-logger 2>/dev/null || true
sudo pkill -9 -f openocd 2>/dev/null || true
sleep 1
for i in $(seq 1 200); do
  OUT=$(sudo timeout 60 openocd -f "$DIR/halt_read_loop.cfg" 2>&1)
  if echo "$OUT" | grep -q "HALTED, ребут"; then
    echo "--- caught attempt $i ---" >> "$LOG"
    echo "$OUT" | grep -E "HALTED|cycle|ERR|done" >> "$LOG"
    break
  fi
  sleep 0.2
done
echo "DONE $(date -u +%H:%M:%S)" >> "$LOG"
chmod 666 "$LOG" 2>/dev/null || true
