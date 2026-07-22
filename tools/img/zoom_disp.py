from PIL import Image, ImageDraw
src="/img/1782761417_AQADoRZrG7-pGEp-.jpg"
im=Image.open(src).convert("RGB"); W,H=im.size; print("SIZE",W,H)
# J1 — bottom of the board; ESP — center. Take two zones enlarged.
def crop(name,box,sc):
    c=im.crop(box).resize((int((box[2]-box[0])*sc),int((box[3]-box[1])*sc)))
    c.save("/work/tools/img/%s.jpg"%name,quality=92); print(name,c.size)
crop("_disp_j1",(120,690,520,810),2.4)   # J1 header at the bottom
crop("_disp_esp",(230,440,520,700),2.0)  # ESP module and its edge
