#!/bin/bash
# run_identify.sh — READ ONLY. Identify chip1 firmware + check the link.
# Detached style: survives WiFi drops, writes a log + DONE marker.
# Frees SWD (kills openocd/services), then runs chip1-identify.cfg.
DIR="$(cd "$(dirname "$0")" && pwd)"
LOG=/home/pi/identify.log
: > "$LOG"
echo "START $(date -u +%H:%M:%S)" >> "$LOG"
sudo systemctl stop mower-openocd    2>/dev/null || true
sudo systemctl stop mower-p1-logger  2>/dev/null || true
sudo pkill -9 -f openocd 2>/dev/null || true
sleep 1
sudo timeout 45 openocd -f "$DIR/chip1-identify.cfg" >> "$LOG" 2>&1
echo "DONE rc=$? $(date -u +%H:%M:%S)" >> "$LOG"
chmod 666 "$LOG" 2>/dev/null || true
# NB: we do NOT bring the services back up automatically — we leave SWD free
#     in case the rollback runs right away. To bring poll up: sudo systemctl start mower-openocd
