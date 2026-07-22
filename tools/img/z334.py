from PIL import Image, ImageDraw
src="/img/1782670334_AQADnx9rG3KxCUp-.jpg"
im=Image.open(src).convert("RGB")
box=(120,150,320,400); sc=2.6
c=im.crop(box).resize((int((box[2]-box[0])*sc),int((box[3]-box[1])*sc)))
d=ImageDraw.Draw(c); ox,oy=box[0],box[1]
for X in range(box[0],box[2]+1,10):
    px=int((X-ox)*sc); col=(255,140,0) if X%50==0 else (255,0,0)
    d.line([(px,0),(px,c.size[1])],fill=col,width=1)
    if X%50==0: d.text((px+1,1),str(X),fill=(255,255,0))
for Y in range(box[1],box[3]+1,10):
    py=int((Y-oy)*sc); col=(255,140,0) if Y%50==0 else (255,0,0)
    d.line([(0,py),(c.size[0],py)],fill=col,width=1)
    if Y%50==0: d.text((1,py+1),str(Y),fill=(0,255,255))
c.save("/work/tools/img/_z334.jpg",quality=92); print("ok",c.size)
