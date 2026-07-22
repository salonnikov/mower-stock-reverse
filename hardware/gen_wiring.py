# Генератор картинки-распиновки (PNG) подключения ESP32 (mower-link) к J1 косилки.
# Запуск в контейнере (см. историю команд). Шрифт DejaVu — для кириллицы.
from PIL import Image, ImageDraw, ImageFont

def font(sz, bold=True):
    p = "/usr/share/fonts/truetype/dejavu/DejaVuSans%s.ttf" % ("-Bold" if bold else "")
    try: return ImageFont.truetype(p, sz)
    except Exception: return ImageFont.load_default()

W, H = 1180, 780
img = Image.new("RGB", (W, H), "white")
d = ImageDraw.Draw(img)
f_title = font(26); f = font(18); fs = font(15); fb = font(22)

d.text((40, 18), "Подключение ESP32 (mower-link)  →  J1 платы дисплея косилки", font=f_title, fill=(20, 20, 20))

CM = {"yellow": (235, 200, 30), "orange": (240, 140, 40), "cyan": (50, 195, 230), "white": (225, 225, 225)}

# ---- плата ESP32 ----
bx0, by0, bx1, by1 = 250, 95, 470, 690
d.rounded_rectangle([bx0, by0, bx1, by1], radius=18, fill=(28, 33, 60), outline=(90, 100, 150), width=3)
d.text((bx0 + 70, by0 + 14), "ESP32", font=fb, fill=(185, 205, 255))
d.text((bx0 + 52, by0 + 44), "DOIT 30-pin", font=fs, fill=(150, 160, 200))

left = ["EN","VP/36","VN/39","D34","D35","D32","D33","D25","D26","D27","D14","D12","D13","GND","VIN"]
right = ["D23","D22","TX0","RX0","D21","D19","D18","D5","TX2/17","RX2/16","D4","D2","D15","GND","3V3"]
n = 15
ys = [130 + (650 - 130) * i // (n - 1) for i in range(n)]

# индекс правого пина -> цвет (используемые)
used = {7: "yellow", 8: "orange", 9: "cyan", 13: "white"}
pad = 12

for i, lab in enumerate(left):
    y = ys[i]
    d.rectangle([bx0 - pad, y - 7, bx0, y + 7], fill=(130, 130, 130))
    d.text((bx0 - pad - 8 - d.textlength(lab, font=fs), y - 9), lab, font=fs, fill=(70, 70, 70))

rx = []
for i, lab in enumerate(right):
    y = ys[i]
    fillc = CM[used[i]] if i in used else (130, 130, 130)
    d.rectangle([bx1, y - 8, bx1 + pad, y + 8], fill=fillc)
    d.text((bx1 + pad + 8, y - 10), lab, font=(f if i in used else fs), fill=(0, 0, 0) if i in used else (70, 70, 70))
    rx.append((bx1 + pad, y))

cx = (bx0 + bx1) // 2
d.rectangle([cx - 32, by1, cx + 32, by1 + 24], fill=(165, 165, 165))
d.text((cx - 16, by1 + 3), "USB", font=fs, fill=(0, 0, 0))
d.text((cx - 95, by1 + 30), "питание: повербанк/комп", font=fs, fill=(90, 90, 90))

# ---- разъём J1 ----
jx0, jy0, jx1, jy1 = 800, 150, 1060, 470
d.rounded_rectangle([jx0, jy0, jx1, jy1], radius=12, fill=(22, 62, 32), outline=(85, 145, 95), width=3)
d.text((jx0 + 18, jy0 + 10), "J1 (плата дисплея)", font=f, fill=(185, 255, 195))
j1 = [("3V3", None), ("T", "cyan"), ("R", "orange"), ("GND", "white"), ("GND", None), ("P", "yellow")]
jy = [jy0 + 55 + (jy1 - jy0 - 80) * i // (len(j1) - 1) for i in range(len(j1))]
jp = []
for i, (lab, col) in enumerate(j1):
    y = jy[i]
    fillc = CM[col] if col else (120, 120, 120)
    d.rectangle([jx0, y - 9, jx0 + 16, y + 9], fill=fillc)
    d.text((jx0 + 26, y - 11), "J1 " + lab, font=f, fill=(225, 255, 225))
    jp.append((jx0, y))
d.text((jx0 + 90, jy[0] - 11), "— НЕ подключать", font=fs, fill=(255, 150, 150))

# ---- провода J1 -> ESP (изгибы разведены, чтобы не сливались) ----
wire = {1: 9, 2: 8, 5: 7, 3: 13}   # T->16, R->17, P->D5, GND->GND
bend = {1: 560, 2: 600, 5: 640, 3: 685}
for ji, ri in wire.items():
    col = CM[j1[ji][1]]
    x1, y1 = jp[ji]; x2, y2 = rx[ri]
    mx = bend[ji]
    d.line([(x1, y1), (mx, y1), (mx, y2), (x2, y2)], fill=col, width=5)

# ---- легенда (чистая зона: правее проводов, ниже J1) ----
lx, ly = 730, 515
d.text((lx, ly - 30), "Соединения (4 провода):", font=f, fill=(20, 20, 20))
rows = [("J1 T", "RX2 / GPIO16", "cyan"),
        ("J1 R", "TX2 / GPIO17", "orange"),
        ("J1 P", "D5 / GPIO5   (только дамп)", "yellow"),
        ("J1 GND", "GND   (общая земля!)", "white")]
for k, (a, b, c) in enumerate(rows):
    yy = ly + k * 32
    d.rectangle([lx, yy, lx + 28, yy + 20], fill=CM[c], outline=(0, 0, 0))
    d.text((lx + 40, yy - 1), a + "  →  " + b, font=f, fill=(20, 20, 20))
d.text((lx, ly + 4 * 32 + 12), "Питание ESP — свой USB/повербанк.", font=fs, fill=(150, 40, 40))
d.text((lx, ly + 4 * 32 + 32), "J1 3V3 НЕ подключать.", font=fs, fill=(150, 40, 40))

img.save("/out/wiring-esp.png")
print("saved /out/wiring-esp.png")
