#!/usr/bin/env python3
"""
pack_own.py — build a FULL flash image with OUR firmware in place of the factory app.
The bootloader (0x08000000..0x08017FFF) is taken from the stock dump (untouched); the app region
(0x08018000..0x080FFFFB) is erased to 0xFF and our app.bin is placed at 0x08018000; we compute
the end-to-end app-CRC over 0x08018000..0x080FFFFC and write it to 0x080FFFFC.

  docker run --rm -v /Users/denis/MOWER:/work python:3.12-slim python /work/tools/bench/pack_own.py \
    /work/dist/gd32-mainboard-dump-v1.bin /work/firmware/mower-own/build/app.bin /work/dist/own_fw.bin
"""
import sys, struct
BASE=0x08000000
APP_START=0x08018000
CRC_END=0x080ffffc      # exclusive; the CRC word is here
APP_REGION_END=0x080ffffb

def app_crc(data):
    crc=0xFFFFFFFF
    for (w,) in struct.iter_unpack('<I', data[APP_START-BASE:CRC_END-BASE]):
        crc^=w
        for _ in range(32):
            crc=((crc<<1)^0x04C11DB7)&0xFFFFFFFF if (crc&0x80000000) else (crc<<1)&0xFFFFFFFF
    return crc

def main():
    dump,app,out=sys.argv[1],sys.argv[2],sys.argv[3]
    data=bytearray(open(dump,'rb').read())
    appb=open(app,'rb').read()
    assert APP_START-BASE+len(appb) < CRC_END-BASE, "app does not fit"
    # erase the app region to 0xFF (except the CRC word, which we'll overwrite)
    for i in range(APP_START-BASE, CRC_END-BASE):
        data[i]=0xFF
    # place the app
    data[APP_START-BASE:APP_START-BASE+len(appb)]=appb
    # CRC
    crc=app_crc(data)
    struct.pack_into('<I', data, CRC_END-BASE, crc)
    open(out,'wb').write(data)
    # verify
    sp=struct.unpack_from('<I',data,APP_START-BASE)[0]
    reset=struct.unpack_from('<I',data,APP_START-BASE+4)[0]
    stored=struct.unpack_from('<I',data,CRC_END-BASE)[0]
    print("app.bin: %d bytes @0x%08x"%(len(appb),APP_START))
    print("vector[0] SP    = 0x%08x (expect 0x20018000)"%sp)
    print("vector[1] Reset = 0x%08x (thumb, in the app region)"%reset)
    print("app CRC = 0x%08x -> 0x080ffffc (stored 0x%08x, match=%s)"%(crc,stored,crc==stored))
    # regions to flash: the whole app region changed vs stock → flash bank0(0x08018000..0x0807ffff)+bank1
    print("image: %s (1MB). Bootloader 0x08000000..0x08017fff = stock (do not touch)."%out)

if __name__=='__main__': main()
