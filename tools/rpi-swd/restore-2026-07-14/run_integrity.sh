#!/bin/bash
DIR="$(cd "$(dirname "$0")" && pwd)"
LOG=/home/pi/integrity.log
: > "$LOG"
echo "START integrity $(date -u +%H:%M:%S) (HOLD the button)" >> "$LOG"
sudo systemctl stop mower-openocd 2>/dev/null || true
sudo systemctl stop mower-p1-logger 2>/dev/null || true
sudo pkill -9 -f openocd 2>/dev/null || true
sleep 1
for i in $(seq 1 200); do
  OUT=$(sudo timeout 40 openocd -f "$DIR/integrity_check.cfg" 2>&1)
  if echo "$OUT" | grep -q "HALTED, checking"; then
    echo "--- caught attempt $i ---" >> "$LOG"
    echo "$OUT" | grep -E "HALTED|MISMATCH|READ ERR|INTEGRITY|FOUND|done" >> "$LOG"
    break
  fi
  sleep 0.2
done
echo "DONE $(date -u +%H:%M:%S)" >> "$LOG"
chmod 666 "$LOG" 2>/dev/null || true
