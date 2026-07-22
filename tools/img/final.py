from PIL import Image, ImageDraw, ImageFont
src="/img/1782670345_AQADpx9rG3KxCUp-.jpg"
im=Image.open(src).convert("RGB")
d=ImageDraw.Draw(im)
def F(s,b=True):
    p="/usr/share/fonts/truetype/dejavu/DejaVuSans%s.ttf"%("-Bold" if b else "")
    return ImageFont.truetype(p,s)
GREEN=(40,230,80); GREY=(150,150,150); RED=(255,60,60); BLK=(0,0,0); WHT=(255,255,255); YEL=(255,230,40)
pads={"3V3":(658,615,False),"DIO":(731,624,True),"CLK":(804,641,True),
      "JTDO":(883,658,False),"RES":(956,673,False),"GND":(1037,685,True)}
# кружки
for name,(x,y,use) in pads.items():
    col=GREEN if use else GREY
    d.ellipse([x-24,y-24,x+24,y+24],outline=col,width=5)
    if not use:
        d.line([x-15,y-15,x+15,y+15],fill=RED,width=4); d.line([x-15,y+15,x+15,y-15],fill=RED,width=4)
# выноски для нужных
def callout(x,y,tx,ty,text):
    d.line([x,y,tx,ty],fill=GREEN,width=4)
    f=F(34); w=d.textlength(text,font=f)
    d.rectangle([tx-8,ty-6,tx+w+8,ty+44],fill=BLK,outline=GREEN,width=3)
    d.text((tx,ty),text,fill=GREEN,font=f)
callout(731,624,120,150,"DIO  →  RPi GPIO24 (пин 18)")
callout(804,641,120,230,"CLK  →  RPi GPIO25 (пин 22)")
callout(1037,685,120,310,"GND  →  RPi GND (пин 6)")
# подписи не-используемых
d.text((640,560),"3V3 ✗",fill=RED,font=F(24)); d.text((905,610),"JTDO ✗",fill=RED,font=F(24)); d.text((980,628),"RES ✗",fill=RED,font=F(24))
# заголовок
title="SWD мейнборда GD32F305  —  паять ТОЛЬКО к 3 зелёным; красные ✗ не трогать"
f=F(28); d.rectangle([0,0,im.size[0],46],fill=BLK); d.text((12,9),title,fill=YEL,font=f)
im.save("/work/dist/wiring-mainboard-swd-to-rpi-v1.jpg",quality=92)
# превью-кроп для проверки
box=(560,520,1140,740);sc=1.6
im.crop(box).resize((int((box[2]-box[0])*sc),int((box[3]-box[1])*sc))).save("/work/tools/img/_final_check.jpg",quality=92)
print("ok")
