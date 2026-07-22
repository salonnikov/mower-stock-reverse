from PIL import Image, ImageDraw, ImageFont
src="/img/1782670334_AQADnx9rG3KxCUp-.jpg"
im=Image.open(src).convert("RGB"); W,H=im.size; print("SIZE",W,H)
# enlarge 2x for readable labels
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
# frame around the connector
bx=(228*S,160*S,300*S,360*S)
d.rectangle(bx,outline=YEL,width=3)
d.text((bx[0]-2,bx[1]-26),"SWD connector (GD32F305 debug)",fill=YEL,font=F(20))
# pointer arrow from the overview
d.line([(470*S,70*S),(305*S,200*S)],fill=YEL,width=4)
d.polygon([(305*S,200*S),(322*S,190*S),(318*S,208*S)],fill=YEL)
d.text((472*S,55*S),"THIS ONE",fill=YEL,font=F(22))
# callouts to the green pads -> right block
def call(px,py,tx,ty,t):
    d.line([(px+16,py),(tx-6,ty+14)],fill=GREEN,width=3)
    f=F(19);w=d.textlength(t,font=f)
    d.rectangle([tx-6,ty-3,tx+w+6,ty+26],fill=BLK,outline=GREEN,width=2)
    d.text((tx,ty),t,fill=GREEN,font=f)
call(252*S,180*S,360*S,150*S,"GND → RPi GND (pin 6)")
call(252*S,278*S,360*S,250*S,"CLK → RPi GPIO25 (pin 22)")
call(252*S,310*S,360*S,330*S,"DIO → RPi GPIO24 (pin 18)")
# title
d.rectangle([0,0,W*S,34],fill=BLK)
d.text((8,6),"GD32F305 mower · SWD to Raspberry Pi: solder ONLY the 3 green (GND/CLK/DIO). Red ✗ — do NOT touch.",fill=YEL,font=F(17))
im.save("/work/dist/wiring-mainboard-swd-to-rpi-v2.jpg",quality=90)
print("saved")
