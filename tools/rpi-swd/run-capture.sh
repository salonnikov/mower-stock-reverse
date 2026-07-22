#!/bin/bash
DUR=${1:-120}
sudo pkill -f "openocd -f.*poll.cfg" 2>/dev/null; sleep 1
cd ~/mower-swd
sudo openocd -f poll.cfg >/tmp/ocd.log 2>&1 &
sleep 2
python3 poll.py "$DUR" ~/mower-swd/capture.csv
sudo pkill -f "openocd -f.*poll.cfg" 2>/dev/null
echo "openocd log tail:"; tail -3 /tmp/ocd.log
