#!/bin/bash
# READ ONLY — poll GD32F305 over SWD. Writes nothing and erases nothing.
cd "$(dirname "$0")"
sudo openocd -f gd32-read.cfg
