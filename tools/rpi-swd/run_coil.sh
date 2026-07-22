#!/bin/bash
# Standalone coil-measurement runner on the Pi. All redirects are inside — no
# nested quoting through ssh. Writes a log and a DONE marker, survives WiFi drops.
LOG=/home/pi/coil.log
: > "$LOG"
echo "START $(date -u +%H:%M:%S)" >> "$LOG"
pkill -9 -f openocd 2>/dev/null
sleep 1
timeout 30 openocd -f /home/pi/mower-swd/chip2-coil-read.cfg >> "$LOG" 2>&1
echo "DONE rc=$? $(date -u +%H:%M:%S)" >> "$LOG"
chmod 666 "$LOG" 2>/dev/null
