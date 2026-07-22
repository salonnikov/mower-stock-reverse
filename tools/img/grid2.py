from PIL import Image, ImageDraw
for tag,src in [("334","/img/1782670334_AQADih1rG3KxCUp-.jpg"),("336","/img/1782670336_AQADoB9rG3KxCUp-.jpg")]:
    im=Image.open(src).convert("RGB"); W,H=im.size
    d=ImageDraw.Draw(im)
    for x in range(0,W,50):
        d.line([(x,0),(x,H)],fill=(255,0,0),width=1)
        if x%100==0: d.text((x+1,1),str(x),fill=(255,255,0))
    for y in range(0,H,50):
        d.line([(0,y),(W,y)],fill=(255,0,0),width=1)
        if y%100==0: d.text((1,y+1),str(y),fill=(0,255,255))
    im.save("/work/tools/img/_g%s.jpg"%tag,quality=88); print("ok",tag,W,H)
