#!/usr/bin/env bash
# Automatic dump of the mower's ESP32 firmware via the mower-link bridge (BRIDGE).
# Read-only — does not break the mower. For how to run it, see docker-compose.yml.
set -u
pip install -q esptool
PORT="socket://${BRIDGE_IP}:${BRIDGE_PORT}"
COMMON=(--before no_reset --after no_reset --chip esp32 --port "$PORT")

echo "=============================================================="
echo " Bridge: $PORT"
echo " NOW: on the mower-link page enable BRIDGE mode,"
echo "         then POWER-CYCLE the mower (the ESP will enter the bootloader)."
echo "=============================================================="

ok=0
for i in $(seq 1 80); do
  if esptool "${COMMON[@]}" flash_id >/tmp/fid 2>&1; then
    echo "+++ Caught the mower's ESP!"; grep -iE 'Chip is|Flash size|MAC' /tmp/fid || true; ok=1; break
  fi
  echo "...no sync (attempt $i/80) — power-cycle the mower. Waiting 3s"; sleep 3
done
[ "$ok" = 1 ] || { echo "!!! Did not catch the ESP in the bootloader. Is BRIDGE enabled? Did you power-cycle?"; exit 1; }

TS=$(date +%Y%m%d-%H%M%S); OUT="/out/mower-esp-$TS.bin"
echo ">>> Reading 4 MB flash into $OUT (at 115200 this takes ~5 minutes)..."
esptool "${COMMON[@]}" read_flash 0 0x400000 "$OUT" || { echo "!!! Read failed"; exit 1; }
ls -la "$OUT"

echo ">>> Checking contents:"
python3 - "$OUT" <<'PY'
import sys,re
d=open(sys.argv[1],'rb').read(); n=len(d)
ff=d.count(0xff)*100//n; zero=d.count(0)*100//n
strings=len(re.findall(rb'[ -~]{6,}', d))
print(f"  size={n}  0xFF={ff}%  0x00={zero}%  ASCII-strings(>=6)={strings}")
if strings>300 and ff<90:
    print("  VERDICT: looks like NORMAL firmware — there is a protocol inside, we can disassemble it. 🎯")
elif ff>95:
    print("  VERDICT: almost empty (0xFF) — wrong location read or the flash is empty.")
else:
    print("  VERDICT: few strings / high entropy — POSSIBLY ENCRYPTED. Proceeding via protocol sniffing.")
PY
echo "Done. File is in ./out/. Return mower-link to SNIFF and power-cycle the mower (it will return to normal)."
