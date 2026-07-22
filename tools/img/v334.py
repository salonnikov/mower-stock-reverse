from PIL import Image, ImageDraw, ImageFont
src="/img/1782670334_AQADnx9rG3KxCUp-.jpg"
im=Image.open(src).convert("RGB")
d=ImageDraw.Draw(im)
def F(s,b=True):
    return ImageFont.truetype("/usr/share/fonts/truetype/dejavu/DejaVuSans%s.ttf"%("-Bold" if b else ""),s)
pads={"GND":(245,180),"RES":(245,212),"JTDO":(245,245),"CLK":(245,278),"DIO":(245,310),"3V3":(243,340)}
for n,(x,y) in pads.items():
    d.ellipse([x-15,y-15,x+15,y+15],outline=(255,0,255),width=3); d.text((x+16,y-7),n,fill=(255,255,0),font=F(16))
box=(120,140,420,400);sc=2.0
im.crop(box).resize((int((box[2]-box[0])*sc),int((box[3]-box[1])*sc))).save("/work/tools/img/_v334.jpg",quality=92)
print("ok")
