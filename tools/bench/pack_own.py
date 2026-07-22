#!/usr/bin/env python3
"""
pack_own.py — собрать ПОЛНЫЙ флеш-образ со СВОЕЙ прошивкой на месте заводского app.
Бутлоадер (0x08000000..0x08017FFF) берём из стокового дампа (не трогаем); app-регион
(0x08018000..0x080FFFFB) стираем в 0xFF и кладём наш app.bin в 0x08018000; считаем
сквозной app-CRC над 0x08018000..0x080FFFFC и пишем в 0x080FFFFC.

  docker run --rm -v /Users/denis/MOWER:/work python:3.12-slim python /work/tools/bench/pack_own.py \
    /work/dist/gd32-mainboard-dump-v1.bin /work/firmware/mower-own/build/app.bin /work/dist/own_fw.bin
"""
import sys, struct
BASE=0x08000000
APP_START=0x08018000
CRC_END=0x080ffffc      # эксклюзивно; тут CRC-слово
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
    assert APP_START-BASE+len(appb) < CRC_END-BASE, "app не влезает"
    # стереть app-регион в 0xFF (кроме CRC-слова, его перезапишем)
    for i in range(APP_START-BASE, CRC_END-BASE):
        data[i]=0xFF
    # положить app
    data[APP_START-BASE:APP_START-BASE+len(appb)]=appb
    # CRC
    crc=app_crc(data)
    struct.pack_into('<I', data, CRC_END-BASE, crc)
    open(out,'wb').write(data)
    # verify
    sp=struct.unpack_from('<I',data,APP_START-BASE)[0]
    reset=struct.unpack_from('<I',data,APP_START-BASE+4)[0]
    stored=struct.unpack_from('<I',data,CRC_END-BASE)[0]
    print("app.bin: %d байт @0x%08x"%(len(appb),APP_START))
    print("vector[0] SP    = 0x%08x (ждём 0x20018000)"%sp)
    print("vector[1] Reset = 0x%08x (thumb, в app-регионе)"%reset)
    print("app CRC = 0x%08x -> 0x080ffffc (stored 0x%08x, match=%s)"%(crc,stored,crc==stored))
    # регионы под заливку: весь app-регион изменился vs стока → флешить bank0(0x08018000..0x0807ffff)+bank1
    print("образ: %s (1MB). Бутлоадер 0x08000000..0x08017fff = сток (не трогать)."%out)

if __name__=='__main__': main()
