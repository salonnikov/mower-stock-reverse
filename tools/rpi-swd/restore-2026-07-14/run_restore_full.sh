#!/bin/bash
# run_restore_full.sh — FULL rollback to factory (4 regions) + auto-verify.
# Hold the power button for the whole run (~40s: flash ~30-35s + verify). Do NOT release until "DONE".
# Catches the link (retry); on deadbeef it simply retries (it never reaches erase — safe).
# Detached, log /home/pi/restore_full.log.
DIR="$(cd "$(dirname "$0")" && pwd)"
LOG=/home/pi/restore_full.log
: > "$LOG"
echo "START full-restore $(date -u +%H:%M:%S)  (HOLD the button!)" >> "$LOG"
sudo systemctl stop mower-openocd    2>/dev/null || true
sudo systemctl stop mower-p1-logger  2>/dev/null || true
sudo pkill -9 -f openocd 2>/dev/null || true
sleep 1

# --- FLASH: retry until we catch the link and it runs to completion ---
FLASHED=0
for i in $(seq 1 60); do
  OUT=$(sudo timeout 130 openocd -f "$DIR/flash_factory_restore_FULL.cfg" 2>&1)
  echo "--- flash attempt $i $(date -u +%H:%M:%S) ---" >> "$LOG"
  echo "$OUT" | tail -8 >> "$LOG"
  if ! echo "$OUT" | grep -qE "deadbeef|DAP init failed|Error:"; then
    echo "FLASH OK on attempt $i $(date -u +%H:%M:%S)" >> "$LOG"
    FLASHED=1
    break
  fi
  sleep 0.3
done
if [ "$FLASHED" = 0 ]; then
  echo "FLASH FAILED — link not caught (did you hold the button?)" >> "$LOG"
  echo "DONE rc=1 $(date -u +%H:%M:%S)" >> "$LOG"
  chmod 666 "$LOG" 2>/dev/null || true
  exit 1
fi

# --- VERIFY: the same 4 regions == factory ---
sleep 1; sudo pkill -9 -f openocd 2>/dev/null || true; sleep 1
for i in $(seq 1 30); do
  OUT=$(sudo timeout 30 openocd -f "$DIR/verify_full.cfg" 2>&1)
  if echo "$OUT" | grep -qE "ALL 4 regions = FACTORY|not everything matched"; then
    echo "--- verify $(date -u +%H:%M:%S) ---" >> "$LOG"
    echo "$OUT" | grep -E "SP=|FAIL|OK:|WARNING" >> "$LOG"
    break
  fi
  sleep 0.3
done
echo "DONE rc=0 $(date -u +%H:%M:%S)" >> "$LOG"
chmod 666 "$LOG" 2>/dev/null || true
