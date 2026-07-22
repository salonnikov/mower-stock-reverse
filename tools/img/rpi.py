from PIL import Image, ImageDraw, ImageFont
def F(s,b=True): return ImageFont.truetype("/usr/share/fonts/truetype/dejavu/DejaVuSans%s.ttf"%("-Bold" if b else ""),s)
W,H=1340,820
im=Image.new("RGB",(W,H),(20,24,30)); d=ImageDraw.Draw(im)
BLK=(0,0,0);GRN=(40,230,80);YEL=(255,225,40);WHT=(230,235,240);GRY=(120,128,140);ORN=(255,170,40)
names={1:"3V3",2:"5V",3:"GPIO2 SDA",4:"5V",5:"GPIO3 SCL",6:"GND",7:"GPIO4",8:"GPIO14 TXD",
9:"GND",10:"GPIO15 RXD",11:"GPIO17",12:"GPIO18",13:"GPIO27",14:"GND",15:"GPIO22",16:"GPIO23",
17:"3V3",18:"GPIO24",19:"GPIO10 MOSI",20:"GND",21:"GPIO9 MISO",22:"GPIO25",23:"GPIO11 SCLK",
24:"GPIO8 CE0",25:"GND",26:"GPIO7 CE1"}
target={6,18,22}
d.text((20,16),"Raspberry Pi 1 — GPIO (26 pin, P1)   подключение SWD к мейнборду косилки",fill=YEL,font=F(22))
d.text((20,48),"Паять только 3 провода. Питание (3V3/5V) НЕ подключать — у косилки своё.",fill=WHT,font=F(16))
d.text((20,74),"ОРИЕНТАЦИЯ: пин 1 = единственный КВАДРАТНЫЙ пятак на плате (остальные круглые). От него весь отсчёт.",fill=ORN,font=F(16))
lx,rx,y0,dy,bw,bh=380,580,140,46,34,34
def pin(num,cx,cy,left):
    box=(cx-bw//2,cy-bh//2,cx+bw//2,cy+bh//2); hl=num in target
    if num==1:   d.rectangle(box,fill=(60,55,20),outline=YEL,width=3)
    elif hl:     d.ellipse(box,fill=(20,60,30),outline=GRN,width=3)
    else:        d.ellipse(box,fill=(45,55,68),outline=GRY,width=1)
    d.text((cx-7 if num<10 else cx-13,cy-9),str(num),fill=WHT,font=F(15))
    nm=names[num]; f=F(14); w=d.textlength(nm,font=f); col=GRN if hl else GRY
    if left: d.text((cx-bw//2-12-w,cy-8),nm,fill=col,font=f)
    else:    d.text((cx+bw//2+12,cy-8),nm,fill=col,font=f)
for r in range(13):
    cy=y0+r*dy; pin(2*r+1,lx,cy,True); pin(2*r+2,rx,cy,False)
# указатель на пин 1
d.line([(250,y0),(lx-bw//2-4,y0)],fill=YEL,width=3)
d.polygon([(lx-bw//2-4,y0),(lx-bw//2-20,y0-8),(lx-bw//2-20,y0+8)],fill=YEL)
d.text((40,y0-12),"ПИН 1 = квадрат",fill=YEL,font=F(18))
# верх/низ
d.text((lx-40,y0-44),"▲ ВЕРХ (угол/край платы)",fill=WHT,font=F(15))
d.text((lx-10,y0+12*dy+30),"▼ НИЗ",fill=WHT,font=F(15))
# выноски
cal={6:200,18:520,22:612}
txts={6:"GND → SWD GND",18:"GPIO24 → SWD DIO (SWDIO)",22:"GPIO25 → SWD CLK (SWCLK)"}
for num in sorted(target):
    r=(num-2)//2; cy=y0+r*dy; ty=cal[num]; txt=txts[num]
    d.line([(rx+bw//2+150,cy),(770,ty)],fill=GRN,width=3)
    f=F(19); w=d.textlength(txt,font=f)
    d.rectangle([762,ty-6,772+w+8,ty+28],fill=BLK,outline=GRN,width=2)
    d.text((770,ty),txt,fill=GRN,font=f)
im.save("/work/dist/wiring-rpi1-gpio-swd-v2.png")
print("saved",W,H)
