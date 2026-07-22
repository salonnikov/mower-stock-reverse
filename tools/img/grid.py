from PIL import Image, ImageDraw, ImageFont
src="/img/1782670345_AQADpx9rG3KxCUp-.jpg"
im=Image.open(src).convert("RGB")
W,H=im.size
print("SIZE",W,H)
d=ImageDraw.Draw(im)
for x in range(0,W,100):
    d.line([(x,0),(x,H)],fill=(255,0,0),width=1)
    d.text((x+2,2),str(x),fill=(255,255,0))
    d.text((x+2,H-14),str(x),fill=(255,255,0))
for y in range(0,H,100):
    d.line([(0,y),(W,y)],fill=(255,0,0),width=1)
    d.text((2,y+2),str(y),fill=(0,255,255))
    d.text((W-40,y+2),str(y),fill=(0,255,255))
im.save("/work/tools/img/_grid.jpg",quality=90)
print("ok")
