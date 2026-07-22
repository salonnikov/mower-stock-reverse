#!/usr/bin/env python3
import sys
from swd import SWD
S=SWD()
addr=int(sys.argv[1],0); cnt=int(sys.argv[2]) if len(sys.argv)>2 else 4
for i,v in enumerate(S.mdw(addr,cnt)):
    print("0x%08x: 0x%08x"%(addr+i*4,v))
S.close()
