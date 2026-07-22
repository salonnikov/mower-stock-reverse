#!/bin/bash
sudo systemctl stop mower-openocd 2>/dev/null
sudo pkill -9 openocd 2>/dev/null; sleep 1
echo "RECLOOP START $(date)" > /tmp/recloop.log
for i in $(seq 1 4000); do
  out=$(sudo openocd -f /tmp/recovery.cfg 2>&1)
  if echo "$out" | grep -q "RECOVERY-FIX-DONE"; then
    echo "=== CAUGHT+FIXED i=$i $(date) ===" >> /tmp/recloop.log
    echo "$out" | tail -8 >> /tmp/recloop.log
    break
  fi
done
echo "RECLOOP END $(date)" >> /tmp/recloop.log
sudo systemctl start mower-openocd 2>/dev/null
