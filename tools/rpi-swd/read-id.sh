#!/bin/bash
# ТОЛЬКО ЧТЕНИЕ — опрос GD32F305 по SWD. Ничего не пишет и не стирает.
cd "$(dirname "$0")"
sudo openocd -f gd32-read.cfg
