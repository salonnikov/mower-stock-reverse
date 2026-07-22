#!/bin/bash
# Universal detached runner for flash operations. $1 = name of the .cfg in ~/mower-swd/.
# Swaps poll.cfg for the flash-cfg, runs it, restores poll.cfg. Log /tmp/a3.log.
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
