#!/bin/bash
# Ставит P1-логгер как systemd-сервис на малину. Запускать С ХОСТА, когда линк есть.
# Идемпотентно: повторный запуск просто обновляет файлы и перезапускает сервис.
# Требует sudo на малине (nopasswd) — ставит unit в /etc/systemd/system.
set -e
PI=pi@192.168.10.81
SSH="ssh -o ConnectTimeout=8 -o BatchMode=yes"
HERE="$(cd "$(dirname "$0")" && pwd)"

scp -o ConnectTimeout=8 "$HERE/p1_logger_daemon.py"    "$PI:/home/pi/mower-bench/"
scp -o ConnectTimeout=8 "$HERE/mower-p1-logger.service" "$PI:/tmp/"

$SSH $PI '
  set -e
  chmod +x /home/pi/mower-bench/p1_logger_daemon.py
  mkdir -p /home/pi/mower-swd
  sudo mv /tmp/mower-p1-logger.service /etc/systemd/system/mower-p1-logger.service
  sudo systemctl daemon-reload
  sudo systemctl enable mower-p1-logger.service
  sudo systemctl restart mower-p1-logger.service
  sleep 3
  echo "active=$(systemctl is-active mower-p1-logger.service) enabled=$(systemctl is-enabled mower-p1-logger.service)"
  echo "--- last log lines ---"
  tail -4 /home/pi/mower-swd/p1_log.txt 2>/dev/null || echo "(лог ещё пуст)"
'
