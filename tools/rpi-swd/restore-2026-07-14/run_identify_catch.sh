#!/bin/bash
# run_identify_catch.sh — READ ONLY. Catches chip1 in the power window (you HOLD the button).
# Runs chip1-identify.cfg in a loop until it catches a live chip (DPIDR != deadbeef).
# Detached: survives WiFi drops. Log /home/pi/identify.log, CAUGHT/GAVEUP marker.
DIR="$(cd "$(dirname "$0")" && pwd)"
LOG=/home/pi/identify.log
: > "$LOG"
echo "START catch $(date -u +%H:%M:%S)  (HOLD the power button!)" >> "$LOG"
sudo systemctl stop mower-openocd    2>/dev/null || true
sudo systemctl stop mower-p1-logger  2>/dev/null || true
sudo pkill -9 -f openocd 2>/dev/null || true
sleep 1
CAUGHT=0
for i in $(seq 1 120); do          # ~120 attempts x ~1s = up to 2 minutes of holding
  OUT=$(sudo timeout 6 openocd -f "$DIR/chip1-identify.cfg" 2>&1)
  echo "--- attempt $i $(date -u +%H:%M:%S) ---" >> "$LOG"
  echo "$OUT" >> "$LOG"
  if echo "$OUT" | grep -q "FIRMWARE ="; then
    echo "CAUGHT on attempt $i $(date -u +%H:%M:%S)" >> "$LOG"
    CAUGHT=1
    break
  fi
  sleep 0.4
done
[ "$CAUGHT" = 0 ] && echo "GAVEUP after 120 attempts $(date -u +%H:%M:%S)" >> "$LOG"
chmod 666 "$LOG" 2>/dev/null || true
