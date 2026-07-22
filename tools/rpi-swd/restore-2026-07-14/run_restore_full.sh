#!/bin/bash
# run_restore_full.sh — ПОЛНЫЙ откат на завод (4 региона) + авто-verify.
# Держи кнопку питания весь прогон (~40с: флеш ~30-35с + verify). НЕ отпускай до "DONE".
# Ловит связь (retry), при deadbeef просто повторяет (до erase дело не доходит — безопасно).
# Detached, лог /home/pi/restore_full.log.
DIR="$(cd "$(dirname "$0")" && pwd)"
LOG=/home/pi/restore_full.log
: > "$LOG"
echo "START full-restore $(date -u +%H:%M:%S)  (ДЕРЖИ кнопку!)" >> "$LOG"
sudo systemctl stop mower-openocd    2>/dev/null || true
sudo systemctl stop mower-p1-logger  2>/dev/null || true
sudo pkill -9 -f openocd 2>/dev/null || true
sleep 1

# --- FLASH: повторять запуск, пока не поймаем связь и не отработает целиком ---
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
  echo "FLASH FAILED — связь не поймана (держал ли кнопку?)" >> "$LOG"
  echo "DONE rc=1 $(date -u +%H:%M:%S)" >> "$LOG"
  chmod 666 "$LOG" 2>/dev/null || true
  exit 1
fi

# --- VERIFY: те же 4 региона == завод ---
sleep 1; sudo pkill -9 -f openocd 2>/dev/null || true; sleep 1
for i in $(seq 1 30); do
  OUT=$(sudo timeout 30 openocd -f "$DIR/verify_full.cfg" 2>&1)
  if echo "$OUT" | grep -qE "ВСЕ 4 региона = ЗАВОД|не всё совпало"; then
    echo "--- verify $(date -u +%H:%M:%S) ---" >> "$LOG"
    echo "$OUT" | grep -E "SP=|FAIL|OK:|ВНИМАНИЕ" >> "$LOG"
    break
  fi
  sleep 0.3
done
echo "DONE rc=0 $(date -u +%H:%M:%S)" >> "$LOG"
chmod 666 "$LOG" 2>/dev/null || true
