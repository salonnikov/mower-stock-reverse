#!/usr/bin/env bash
#
# Decompile the display-board ESP32 firmware with Ghidra, headless, in Docker.
#
#   ./run.sh [<dump.bin>] [<outdir>]
#
# Defaults: dist/esp32-display-dump-v1.bin -> reverse-v2/esp32-display/
#
# The image is linux/amd64 ON PURPOSE: the Ghidra release ships native
# decompiler binaries for linux_x86_64 / mac_* / win_x86_64 but NOT for
# linux_arm_64, so an arm64 image would start without a decompiler. On Apple
# Silicon this needs Docker Desktop's Rosetta enabled (Settings -> General ->
# "Use Rosetta for x86_64/amd64 emulation"), not qemu.
#
# Ghidra 12.1.2 carries an Xtensa processor module of its own — no third-party
# extension is needed.

set -euo pipefail

HERE="$(cd "$(dirname "$0")" && pwd)"
REPO="$(cd "$HERE/../.." && pwd)"
DUMP="${1:-$REPO/dist/esp32-display-dump-v1.bin}"
OUT="${2:-$REPO/reverse-v2/esp32-display}"
IMAGE="ghidra-esp32:12.1.2"
WORK="$(mktemp -d)"

command -v docker >/dev/null || { echo "docker not found" >&2; exit 1; }
[ -f "$DUMP" ] || { echo "dump not found: $DUMP" >&2; exit 1; }

if ! docker image inspect "$IMAGE" >/dev/null 2>&1; then
    echo "Image $IMAGE is missing. Build it first:"
    echo "  curl -fsSL -o ghidra.zip https://github.com/NationalSecurityAgency/ghidra/releases/download/Ghidra_12.1.2_build/ghidra_12.1.2_PUBLIC_20260605.zip"
    echo "  docker build --platform linux/amd64 -t $IMAGE -f $HERE/Dockerfile ."
    exit 1
fi

mkdir -p "$WORK"/{segs,scripts,out,proj}
cp "$REPO/reverse-v2/ghidra-scripts/Esp32Segments.java" "$WORK/scripts/"
cp "$REPO/reverse-v2/ghidra-scripts/ExportDecomp.java"  "$WORK/scripts/"

# ota_0 (the live app) starts at 0x10000 and is 0x170000 long.
dd if="$DUMP" of="$WORK/ota0.bin" bs=4096 skip=16 count=368 status=none

# esp_image segments: file offset, length, load address (from `esptool image-info`).
split_seg() { dd if="$WORK/ota0.bin" of="$WORK/segs/$1" bs=1 skip="$2" count="$3" status=none; }
split_seg seg0_drom_3f400020.bin $((0x18))     $((0x26270))
split_seg seg1_dram_3ffbdb60.bin $((0x26290))  $((0x6cdc))
split_seg seg2_iram_40080000.bin $((0x2cf74))  $((0x309c))
split_seg seg3_irom_400d0020.bin $((0x30018))  $((0xe4248))
split_seg seg4_iram_4008309c.bin $((0x114268)) $((0x191bc))
split_seg seg5_rtc_50000000.bin  $((0x12d42c)) $((0x10))

# ROM symbol map from the ESP-IDF release the firmware was built with (v4.4.3).
curl -fsSL "https://raw.githubusercontent.com/espressif/esp-idf/v4.4.3/components/esp_rom/esp32/ld/esp32.rom.ld" \
  | grep -oE '^ *PROVIDE *\( *[A-Za-z_][A-Za-z0-9_]* *= *0x[0-9a-fA-F]+' \
  | sed -E 's/ *PROVIDE *\( *//; s/ *= */,/' > "$WORK/rom_syms.csv"
echo "ROM symbols: $(wc -l < "$WORK/rom_syms.csv")"

docker run --rm --platform linux/amd64 -v "$WORK:/work" "$IMAGE" \
    /work/proj esp32disp \
    -import /work/segs/seg3_irom_400d0020.bin \
    -processor "Xtensa:LE:32:default" \
    -loader BinaryLoader \
    -loader-baseAddr 0x400d0020 \
    -scriptPath /work/scripts \
    -preScript Esp32Segments.java /work/segs /work/rom_syms.csv 0x400814ec \
    -postScript ExportDecomp.java /work/out

mkdir -p "$OUT"
cp "$WORK/out/"* "$OUT/"
echo "Done -> $OUT"
rm -rf "$WORK"
