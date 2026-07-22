#!/bin/bash
# run_restore.sh — ЗАЛИВКА ЗАВОДА (3 страницы app). ЗАПУСКАТЬ ТОЛЬКО ПОСЛЕ
# успешного chip1-identify.cfg, подтвердившего: SP=НАШ, PAGE3/hook/payload/CRC=ЗАВОД.
# Detached: переживает обрыв WiFi, пишет лог + маркер DONE.
# ⚠️ Держать питание косилки во время заливки (не дать уснуть). После — POWER-CYCLE.
DIR="$(cd "$(dirname "$0")" && pwd)"
LOG=/home/pi/restore.log
: > "$LOG"
echo "START $(date -u +%H:%M:%S)" >> "$LOG"
sudo systemctl stop mower-openocd    2>/dev/null || true
sudo systemctl stop mower-p1-logger  2>/dev/null || true
sudo pkill -9 -f openocd 2>/dev/null || true
sleep 1
sudo timeout 300 openocd -f "$DIR/flash_factory_restore_3page.cfg" >> "$LOG" 2>&1
echo "FLASH rc=$? $(date -u +%H:%M:%S)" >> "$LOG"
# Верификация read-back (тот же SWD, без reset)
sleep 1
sudo pkill -9 -f openocd 2>/dev/null || true
sleep 1
sudo timeout 60 openocd -f "$DIR/verify_after.cfg" >> "$LOG" 2>&1
echo "DONE rc=$? $(date -u +%H:%M:%S)" >> "$LOG"
chmod 666 "$LOG" 2>/dev/null || true
# После DONE: если verify показал ЗАВОД-эталоны -> физический POWER-CYCLE косилки.
