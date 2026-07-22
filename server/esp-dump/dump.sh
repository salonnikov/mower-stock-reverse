#!/usr/bin/env bash
# Автоматический дамп прошивки ESP32 косилки через мост mower-link (BRIDGE).
# Только чтение — косилку не ломает. Запуск см. docker-compose.yml.
set -u
pip install -q esptool
PORT="socket://${BRIDGE_IP}:${BRIDGE_PORT}"
COMMON=(--before no_reset --after no_reset --chip esp32 --port "$PORT")

echo "=============================================================="
echo " Мост: $PORT"
echo " СЕЙЧАС: на странице mower-link включи режим BRIDGE,"
echo "         затем ПЕРЕДЁРНИ питание косилки (ESP уйдёт в загрузчик)."
echo "=============================================================="

ok=0
for i in $(seq 1 80); do
  if esptool "${COMMON[@]}" flash_id >/tmp/fid 2>&1; then
    echo "+++ Поймал ESP косилки!"; grep -iE 'Chip is|Flash size|MAC' /tmp/fid || true; ok=1; break
  fi
  echo "...нет синка (попытка $i/80) — передёрни питание косилки. Жду 3с"; sleep 3
done
[ "$ok" = 1 ] || { echo "!!! Не поймал ESP в загрузчике. BRIDGE включён? Питание передёрнул?"; exit 1; }

TS=$(date +%Y%m%d-%H%M%S); OUT="/out/mower-esp-$TS.bin"
echo ">>> Читаю флеш 4 МБ в $OUT (на 115200 это ~5 минут)..."
esptool "${COMMON[@]}" read_flash 0 0x400000 "$OUT" || { echo "!!! Чтение не удалось"; exit 1; }
ls -la "$OUT"

echo ">>> Проверка содержимого:"
python3 - "$OUT" <<'PY'
import sys,re
d=open(sys.argv[1],'rb').read(); n=len(d)
ff=d.count(0xff)*100//n; zero=d.count(0)*100//n
strings=len(re.findall(rb'[ -~]{6,}', d))
print(f"  размер={n}  0xFF={ff}%  0x00={zero}%  ASCII-строк(>=6)={strings}")
if strings>300 and ff<90:
    print("  ВЕРДИКТ: похоже на НОРМАЛЬНУЮ прошивку — внутри есть протокол, можно разбирать. 🎯")
elif ff>95:
    print("  ВЕРДИКТ: почти пусто (0xFF) — не туда читали или флеш пуст.")
else:
    print("  ВЕРДИКТ: мало строк / высокая энтропия — ВОЗМОЖНО ЗАШИФРОВАН. Идём по сниффу протокола.")
PY
echo "Готово. Файл в ./out/. Верни mower-link в SNIFF и передёрни питание косилки (вернётся к норме)."
