from PIL import Image, ImageDraw
src="/img/1782670345_AQADpx9rG3KxCUp-.jpg"
im=Image.open(src).convert("RGB")
box=(430,400,880,720); sc=2
c=im.crop(box).resize(((box[2]-box[0])*sc,(box[3]-box[1])*sc))
d=ImageDraw.Draw(c)
ox,oy=box[0],box[1]
for X in range(box[0],box[2]+1,20):
    px=(X-ox)*sc; d.line([(px,0),(px,c.size[1])],fill=(255,0,0),width=1)
    if X%40==0: d.text((px+1,1),str(X),fill=(255,255,0))
for Y in range(box[1],box[3]+1,20):
    py=(Y-oy)*sc; d.line([(0,py),(c.size[0],py)],fill=(255,0,0),width=1)
    if Y%40==0: d.text((1,py+1),str(Y),fill=(0,255,255))
c.save("/work/tools/img/_zoom2.jpg",quality=92)
print("ok",c.size)
