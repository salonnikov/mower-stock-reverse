#!/bin/bash
DIR="$(cd "$(dirname "$0")" && pwd)"
LOG=/home/pi/fsm_batt.log
: > "$LOG"
echo "START fsm/batt $(date -u +%H:%M:%S) (HOLD the button)" >> "$LOG"
sudo systemctl stop mower-openocd 2>/dev/null || true
sudo systemctl stop mower-p1-logger 2>/dev/null || true
sudo pkill -9 -f openocd 2>/dev/null || true
sleep 1
for i in $(seq 1 200); do
  OUT=$(sudo timeout 40 openocd -f "$DIR/read_fsm_batt.cfg" 2>&1)
  if echo "$OUT" | grep -q "HALTED, sampling"; then
    echo "--- caught attempt $i ---" >> "$LOG"
    echo "$OUT" | grep -E "HALTED|s[0-9]:|halt fail|stopped|done" >> "$LOG"
    break
  fi
  sleep 0.2
done
echo "DONE $(date -u +%H:%M:%S)" >> "$LOG"
chmod 666 "$LOG" 2>/dev/null || true
