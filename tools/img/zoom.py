from PIL import Image, ImageDraw
src="/img/1782670345_AQADpx9rG3KxCUp-.jpg"
im=Image.open(src).convert("RGB")
# обрезаем зону гребёнки и увеличиваем x3
box=(450,330,900,680)
c=im.crop(box).resize(((900-450)*2,(680-330)*2))
d=ImageDraw.Draw(c)
ox,oy=box[0],box[1]; sc=2
for X in range(box[0],box[2]+1,25):
    px=(X-ox)*sc; d.line([(px,0),(px,c.size[1])],fill=(255,0,0),width=1)
    if X%50==0: d.text((px+1,1),str(X),fill=(255,255,0))
for Y in range(box[1],box[3]+1,25):
    py=(Y-oy)*sc; d.line([(0,py),(c.size[0],py)],fill=(255,0,0),width=1)
    if Y%50==0: d.text((1,py+1),str(Y),fill=(0,255,255))
c.save("/work/tools/img/_zoom.jpg",quality=92)
print("ok",c.size)
