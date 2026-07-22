from PIL import Image, ImageDraw, ImageFont
src="/img/1782670334_AQADnx9rG3KxCUp-.jpg"
im=Image.open(src).convert("RGB"); W,H=im.size; print("SIZE",W,H)
# увеличим в 2x для читаемости подписей
S=2; im=im.resize((W*S,H*S)); d=ImageDraw.Draw(im)
def F(s,b=True): return ImageFont.truetype("/usr/share/fonts/truetype/dejavu/DejaVuSans%s.ttf"%("-Bold" if b else ""),s)
GREEN=(40,230,80);GREY=(150,150,150);RED=(255,60,60);BLK=(0,0,0);YEL=(255,230,40);WHT=(255,255,255)
pads={"GND":(252,180,True),"RES":(252,212,False),"JTDO":(252,245,False),
      "CLK":(252,278,True),"DIO":(252,310,True),"3V3":(250,340,False)}
for n,(x,y,use) in pads.items():
    x*=S;y*=S;col=GREEN if use else GREY
    d.ellipse([x-16,y-16,x+16,y+16],outline=col,width=4)
    if not use:
        d.line([x-10,y-10,x+10,y+10],fill=RED,width=3);d.line([x-10,y+10,x+10,y-10],fill=RED,width=3)
# рамка вокруг разъёма
bx=(228*S,160*S,300*S,360*S)
d.rectangle(bx,outline=YEL,width=3)
d.text((bx[0]-2,bx[1]-26),"SWD-разъём (отладка GD32F305)",fill=YEL,font=F(20))
# стрелка-указатель из общего вида
d.line([(470*S,70*S),(305*S,200*S)],fill=YEL,width=4)
d.polygon([(305*S,200*S),(322*S,190*S),(318*S,208*S)],fill=YEL)
d.text((472*S,55*S),"ВОТ ЭТОТ",fill=YEL,font=F(22))
# выноски к зелёным пятакам -> правый блок
def call(px,py,tx,ty,t):
    d.line([(px+16,py),(tx-6,ty+14)],fill=GREEN,width=3)
    f=F(19);w=d.textlength(t,font=f)
    d.rectangle([tx-6,ty-3,tx+w+6,ty+26],fill=BLK,outline=GREEN,width=2)
    d.text((tx,ty),t,fill=GREEN,font=f)
call(252*S,180*S,360*S,150*S,"GND → RPi GND (пин 6)")
call(252*S,278*S,360*S,250*S,"CLK → RPi GPIO25 (пин 22)")
call(252*S,310*S,360*S,330*S,"DIO → RPi GPIO24 (пин 18)")
# заголовок
d.rectangle([0,0,W*S,34],fill=BLK)
d.text((8,6),"Косилка GD32F305 · SWD к Raspberry Pi: паять ТОЛЬКО 3 зелёных (GND/CLK/DIO). Красные ✗ — НЕ трогать.",fill=YEL,font=F(17))
im.save("/work/dist/wiring-mainboard-swd-to-rpi-v2.jpg",quality=90)
print("saved")
