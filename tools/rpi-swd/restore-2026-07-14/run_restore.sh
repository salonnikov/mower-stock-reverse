#!/bin/bash
# run_restore.sh — FACTORY FLASH (3 app pages). RUN ONLY AFTER
# a successful chip1-identify.cfg that confirmed: SP=OURS, PAGE3/hook/payload/CRC=FACTORY.
# Detached: survives WiFi drops, writes a log + DONE marker.
# WARNING: keep the mower powered during flashing (do not let it sleep). After — POWER-CYCLE.
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
# Read-back verification (same SWD, no reset)
sleep 1
sudo pkill -9 -f openocd 2>/dev/null || true
sleep 1
sudo timeout 60 openocd -f "$DIR/verify_after.cfg" >> "$LOG" 2>&1
echo "DONE rc=$? $(date -u +%H:%M:%S)" >> "$LOG"
chmod 666 "$LOG" 2>/dev/null || true
# After DONE: if verify showed the FACTORY references -> physical POWER-CYCLE of the mower.
