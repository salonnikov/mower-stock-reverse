#!/bin/bash
# run_identify.sh — ТОЛЬКО ЧТЕНИЕ. Опознать прошивку chip1 + проверить связь.
# Detached-стиль: переживает обрыв WiFi, пишет лог + маркер DONE.
# Освобождает SWD (глушит openocd/сервисы), затем гоняет chip1-identify.cfg.
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
# NB: сервисы НЕ поднимаем обратно автоматически — SWD оставляем свободным
#     на случай, если сразу пойдёт откат. Поднять poll: sudo systemctl start mower-openocd
