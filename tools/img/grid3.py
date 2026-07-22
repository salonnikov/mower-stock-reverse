from PIL import Image, ImageDraw
imgs={"334":"1782670334_AQADnx9rG3KxCUp-.jpg","336":"1782670336_AQADoB9rG3KxCUp-.jpg",
      "344":"1782670344_AQADph9rG3KxCUp-.jpg","337":"1782670337_AQADoR9rG3KxCUp-.jpg"}
for tag,fn in imgs.items():
    im=Image.open("/img/"+fn).convert("RGB"); W,H=im.size
    d=ImageDraw.Draw(im)
    for x in range(0,W,50):
        d.line([(x,0),(x,H)],fill=(255,0,0),width=1)
        if x%100==0: d.text((x+1,1),str(x),fill=(255,255,0))
    for y in range(0,H,50):
        d.line([(0,y),(W,y)],fill=(255,0,0),width=1)
        if y%100==0: d.text((1,y+1),str(y),fill=(0,255,255))
    im.save("/work/tools/img/_g%s.jpg"%tag,quality=85); print("ok",tag)
