#!/bin/bash
# A3 detached-раннер: подменяет poll.cfg на flash-конфиг, гоняет erase/program/verify,
# возвращает poll.cfg. Всё логируется в /tmp/a3.log. Обрыв ssh не прерывает (nohup).
LOG=/tmp/a3.log
{
  echo "=== A3 START $(date) ==="
  echo ">> stop mower-openocd"
  sudo systemctl stop mower-openocd
  sleep 1
  echo ">> run flash openocd"
  sudo openocd -f "$HOME/mower-swd/flash_a3_test.cfg"
  echo ">> openocd rc=$?"
  sleep 1
  echo ">> restart mower-openocd (poll.cfg)"
  sudo systemctl start mower-openocd
  sleep 2
  echo -n ">> poll :4444 back: "
  (ss -ltn 2>/dev/null || netstat -ltn) | grep -q :4444 && echo UP || echo DOWN
  echo "=== A3 DONE $(date) ==="
} > "$LOG" 2>&1
