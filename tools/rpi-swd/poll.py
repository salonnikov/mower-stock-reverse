import socket, struct, time, sys
HOST,PORT="127.0.0.1",4444
DUR=float(sys.argv[1]) if len(sys.argv)>1 else 120.0
OUT=sys.argv[2] if len(sys.argv)>2 else "/home/pi/mower-swd/capture.csv"

def conn():
    s=socket.create_connection((HOST,PORT),5); s.settimeout(5)
    _until(s); return s
def _until(s):
    buf=b""
    while not buf.rstrip().endswith(b">"):
        d=s.recv(4096)
        if not d: break
        buf+=d
    return buf
def cmd(s,c):
    s.sendall((c+"\n").encode()); return _until(s).decode("latin1")
def words(s,addr,n):
    out=cmd(s,"mdw 0x%08x %d"%(addr,n)); vals=[]
    for line in out.splitlines():
        if ":" in line and "0x" in line.split(":")[0]:
            for t in line.split(":")[1].split():
                try: vals.append(int(t,16))
                except: pass
    return vals
def s16(w,hi=False):
    v=(w>>16)&0xffff if hi else w&0xffff
    return v-0x10000 if v>0x7fff else v
def s32(w): return w-0x100000000 if w>0x7fffffff else w
def f64(lo,hi): return struct.unpack("<d",struct.pack("<II",lo,hi))[0]
def ok(p): return 0x20000000<=p<0x20020000

s=conn()
# защёлкиваем указатели
p_pose=words(s,0x20000090,1)[0]
p_head=words(s,0x2000039c,1)[0]
a=words(s,0x200003a8,1)[0]
p_bd=words(s,a+4,1)[0] if ok(a) else 0
print("ptr pose=%08x head=%08x bd_outer=%08x bd=%08x"%(p_pose,p_head,a,p_bd))
print("state=",words(s,0x200000bc,1)[0])
if not(ok(p_pose) and ok(p_head)):
    print("!!! указатели не защёлкнулись (косилка под питанием? чтение без halt идёт?) — стоп"); sys.exit(1)
print("OK, пишу",OUT,"в течение",DUR,"с")
f=open(OUT,"w",buffering=1)
f.write("t,state,x,y,heading,dist,ls,rs,sls,srs,lf\n")
t0=time.time()
while time.time()-t0<DUR:
    try:
        A=words(s,p_pose+0x34,12)          # 0x34..0x60
        dist=A[0]; x=f64(A[3],A[4]); y=f64(A[5],A[6]); hcopy=s16(A[10],hi=True)
        B=words(s,p_bd+0x0c,8) if p_bd else [0]*8   # 0x0c..0x28
        ls=s32(B[0]); rs=s32(B[2]); lf=s16(B[4]); sls=s32(B[6]); srs=s32(B[7])
        hd=s16(words(s,p_head+0x18,1)[0])
        st=words(s,0x200000bc,1)[0]
        f.write("%.3f,%d,%.4f,%.4f,%d,%d,%d,%d,%d,%d,%d\n"%(time.time(),st,x,y,hd,dist,ls,rs,sls,srs,lf))
    except Exception as e:
        print("read err",e); time.sleep(0.2)
    time.sleep(0.03)
print("done, rows in",OUT)
