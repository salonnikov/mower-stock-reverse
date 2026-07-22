from PIL import Image, ImageDraw
src="/img/1782670345_AQADpx9rG3KxCUp-.jpg"
im=Image.open(src).convert("RGB")
box=(820,300,1180,720); sc=1.8
c=im.crop(box).resize((int((box[2]-box[0])*sc),int((box[3]-box[1])*sc)))
d=ImageDraw.Draw(c); ox,oy=box[0],box[1]
for X in range(box[0],box[2]+1,20):
    px=int((X-ox)*sc); col=(255,140,0) if X%100==0 else (255,0,0)
    d.line([(px,0),(px,c.size[1])],fill=col,width=1)
    if X%40==0: d.text((px+1,1),str(X),fill=(255,255,0))
for Y in range(box[1],box[3]+1,20):
    py=int((Y-oy)*sc); col=(255,140,0) if Y%100==0 else (255,0,0)
    d.line([(0,py),(c.size[0],py)],fill=col,width=1)
    if Y%40==0: d.text((1,py+1),str(Y),fill=(0,255,255))
c.save("/work/tools/img/_zoom5.jpg",quality=92)
print("ok",c.size)
