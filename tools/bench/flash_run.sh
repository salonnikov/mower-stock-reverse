#!/bin/bash
# Универсальный detached-раннер flash-операций. $1 = имя .cfg в ~/mower-swd/.
# Подменяет poll.cfg на flash-cfg, гоняет, возвращает poll.cfg. Лог /tmp/a3.log.
CFG="$1"
LOG=/tmp/a3.log
{
  echo "=== FLASH-RUN $CFG $(date) ==="
  echo ">> stop mower-openocd"; sudo systemctl stop mower-openocd; sleep 1
  echo ">> run flash openocd ($CFG)"
  sudo openocd -f "$HOME/mower-swd/$CFG"
  echo ">> openocd rc=$?"
  sleep 1; echo ">> restart mower-openocd"; sudo systemctl start mower-openocd; sleep 2
  echo -n ">> :4444 back: "; (ss -ltn 2>/dev/null || netstat -ltn) | grep -q :4444 && echo UP || echo DOWN
  echo "=== A3 DONE $(date) ==="
} > "$LOG" 2>&1
