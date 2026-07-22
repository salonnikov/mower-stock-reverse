from PIL import Image, ImageDraw, ImageFont
src="/img/1782670345_AQADpx9rG3KxCUp-.jpg"
im=Image.open(src).convert("RGB")
d=ImageDraw.Draw(im)
pads={"3V3":(614,534),"DIO":(676,540),"CLK":(739,546),"JTDO":(798,475),"RES":(854,486),"GND":(811,350)}
for name,(x,y) in pads.items():
    d.ellipse([x-22,y-22,x+22,y+22],outline=(255,0,255),width=3)
    d.text((x-10,y-8),name,fill=(255,255,0))
box=(520,260,960,640); sc=1.8
c=im.crop(box).resize((int((box[2]-box[0])*sc),int((box[3]-box[1])*sc)))
c.save("/work/tools/img/_verify.jpg",quality=92)
print("ok")
