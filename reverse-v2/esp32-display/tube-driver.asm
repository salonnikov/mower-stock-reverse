400dff00  ssr a8
400dff03  sra a12,a11
400dff06  bnone 0x400dff18,a12,a10,
400dff09  l32r a9,0x400d140c
400dff0c  slli a8,a8,0x1
400dff0f  add.n a8,a9,a8
400dff11  s16i a10,a8,0x0
400dff14  movi.n a2,0x0        ; <<< FUN_400dff14
400dff16  retw.n
400dff18  addi.n a8,a8,0x1        ; <<< FUN_400dff18
400dff1a  j 0x400dff14
400dff1d  ill
400dff20  entry a1,0x30
400dff23  l32r a2,0x400d1410
400dff26  movi a3,0x0
400dff29  l32i a2,a2,0x0
400dff2c  s16i a3,a1,0x0
400dff2f  beqz a2,0x400dff61
400dff32  l8ui a8,a2,0x10
400dff35  bnez a8,0x400dffd8
400dff38  movi.n a8,0xff
400dff3a  s8i a8,a2,0x10
400dff3d  l8ui a8,a2,0x4
400dff40  bnez.n a8,0x400dff64
400dff42  l16ui a8,a2,0x6
400dff45  l32r a3,0x400d1414
400dff48  addi.n a10,a2,0x6
400dff4a  and a8,a8,a3
400dff4d  l32r a3,0x400d039c
400dff50  or a8,a8,a3
400dff53  s16i a8,a2,0x6
400dff56  call8 0x400dfef0
400dff59  l8ui a3,a2,0x4
400dff5c  addi.n a3,a3,0x1
400dff5e  s8i a3,a2,0x4
400dff61  retw.n
400dff63  .data  ?? 00h
400dff64  bnei a8,0x1,0x400dff80
400dff67  l16ui a8,a2,0x8
400dff6a  l32r a3,0x400d1414
400dff6d  addi.n a10,a2,0x8
400dff6f  and a8,a8,a3
400dff72  l32r a3,0x400d056c
400dff75  or a8,a8,a3
400dff78  s16i a8,a2,0x8
400dff7b  j 0x400dff56
400dff7e  .data  ?? 00h
400dff7f  .data  ?? 00h
400dff80  bnei a8,0x2,0x400dff9c
400dff83  l16ui a8,a2,0xa
400dff86  l32r a3,0x400d1414
400dff89  addi.n a10,a2,0xa
400dff8b  and a8,a8,a3
400dff8e  l32r a3,0x400d0974
400dff91  or a8,a8,a3
400dff94  s16i a8,a2,0xa
400dff97  j 0x400dff56
400dff9a  .data  ?? 00h
400dff9b  .data  ?? 00h
400dff9c  bnei a8,0x3,0x400dffb8
400dff9f  l16ui a8,a2,0xc
400dffa2  l32r a3,0x400d1414
400dffa5  addi.n a10,a2,0xc
400dffa7  and a8,a8,a3
400dffaa  movi a3,0x400
400dffad  or a8,a8,a3
400dffb0  s16i a8,a2,0xc
400dffb3  j 0x400dff56
400dffb6  .data  ?? 00h
400dffb7  .data  ?? 00h
400dffb8  bnei a8,0x4,0x400dff61
400dffbb  l16ui a8,a2,0xe
400dffbe  l32r a9,0x400d1414
400dffc1  addi.n a10,a2,0xe
400dffc3  and a8,a8,a9
400dffc6  movi a9,0x200
400dffc9  or a8,a8,a9
400dffcc  s16i a8,a2,0xe
400dffcf  call8 0x400dfef0
400dffd2  j 0x400dff5e
400dffd5  ill
400dffd8  mov.n a10,a1
400dffda  call8 0x400dfef0
400dffdd  s8i a3,a2,0x10
400dffe0  j 0x400dff61
400dffe3  srli a3,a0,0x6
400dffe6  .data  ?? 00h
400dffe7  .data  ?? 81h
400dffe8  add.n a12,a5,a0        ; <<< FUN_400dffe8
400dffea  l32i.n a8,a8,0x0
400dffec  l32i.n a2,a8,0x0
400dffee  retw.n
400dfff0  entry a1,0x20        ; <<< FUN_400dfff0
400dfff3  l32r a14,0x400d1418
400dfff6  l32r a10,0x400d141c
400dfff9  movi.n a13,0x0
400dfffb  movi.n a12,0x1
400dfffd  movi a11,0x2bc
400e0000  call8 0x40094dc8
400e0003  mov.n a2,a10
400e0005  call8 0x40093484
400e0008  mov.n a12,a10
400e000a  movi.n a14,0xff
400e000c  mov a10,a2
400e000f  movi a13,0x0
400e0012  movi a11,0x1
400e0015  call8 0x40094e04
400e0018  movi.n a2,0x0
400e001a  l32r a3,0x400d140c
400e001d  slli a8,a2,0x1
400e0020  add.n a8,a3,a8
400e0022  l16ui a10,a8,0x0
400e0025  addi.n a2,a2,0x1
400e0027  extui a2,a2,0x0,0x8
400e002a  call8 0x400e6f54
400e002d  bltui a2,0x4,0x400e0032
400e0030  movi.n a2,0x0
400e0032  movi.n a10,0x2
400e0034  call8 0x4009417c        ; <<< FUN_400e0034
400e0037  j 0x400e001a
400e003a  cust0 {op2=0x3,r=a0,s=a0,t=a0}
400e003d  l32r a8,0x400a8440
400e0040  l32i.n a15,a4,0x30
400e0042  l32i a2,a2,0x0
400e0045  l32i.n a3,a2,0x34
400e0047  l32i a2,a3,0x0
400e004a  extui a2,a2,0x0,0x1
400e004d  bnez.n a2,0x400e0078
400e004f  call8 0x400e6fb0
400e0052  addi a10,a1,0x10
400e0055  s16i a2,a1,0x10
400e0058  call8 0x400dfef0
400e005b  movi.n a4,0x1
400e005d  l32r a12,0x400d0974
400e0060  l32r a11,0x400d1424
400e0063  l32r a10,0x400d1428
400e0066  mov.n a15,a2
400e0068  mov.n a13,a2
400e006a  s32i.n a4,a1,0x0
400e006c  movi.n a14,0x19
400e006e  call8 0x400933f8
400e0071  l32i.n a2,a3,0x0
400e0073  .data  ?? 40h    @
400e0074  l32i a2,a0,0xa4        ; <<< FUN_400e0074
400e0077  lsi f0,a12,0x8
400e007a  retw.n
400e007c  entry a1,0x20        ; <<< FUN_400e007c
400e007f  sext a2,a2,7
400e0082  l32r a8,0x400d142c
400e0085  bnei a2,0x2,0x400e008e
400e0088  movi.n a2,0x1
400e008a  s32i.n a2,a8,0x0
400e008c  retw.n
400e008e  movi.n a9,0x0
400e0090  s32i.n a9,a8,0x0
400e0092  l32r a8,0x400d1410
400e0095  l32i.n a9,a8,0x0
400e0097  l16ui a8,a9,0x6
400e009a  bnei a2,0x1,0x400e00a9
400e009d  movi a2,0x100
400e00a0  or a8,a8,a2
400e00a3  s16i a8,a9,0x6
400e00a6  j 0x400e008c
400e00a9  movi a2,-0x101
400e00ac  and a8,a8,a2        ; <<< FUN_400e00ac
400e00af  j 0x400e00a3
400e00b2  cust0 {op2=0x3,r=a0,s=a0,t=a0}
400e00b5  l32r a4,0x400a80b8
400e00b8  xor a7,a4,a2
400e00bb  cust0 {op2=0xf,r=a7,s=a4,t=a3}
400e00be  l32i a5,a0,0x2c
400e00c1  .data  ?? 22h    "
400e00c2  movi a8,0xff
400e00c5  extui a2,a2,0x0,0x8
400e00c8  bne a3,a8,0x400e00e4
400e00cb  l32r a3,0x400d1410
400e00ce  slli a2,a2,0x1
400e00d1  l32i.n a9,a3,0x0
400e00d3  movi a8,-0x80
400e00d6  add.n a2,a9,a2
400e00d8  l16ui a3,a2,0x6
400e00db  and a3,a3,a8
400e00de  s16i a3,a2,0x6
400e00e1  retw.n
400e00e3  .data  ?? 00h
400e00e4  movi.n a10,0x9
400e00e6  bltu a10,a3,0x400e00e1
400e00e9  l32r a8,0x400d1410
400e00ec  slli a2,a2,0x1
400e00ef  l32i.n a9,a8,0x0
400e00f1  add.n a9,a9,a2
400e00f3  l16ui a8,a9,0x6
400e00f6  beqi a3,0x5,0x400e0164
400e00f9  bgeui a3,0x6,0x400e0125
400e00fc  beqi a3,0x2,0x400e014b
400e00ff  bgeui a3,0x3,0x400e0114
400e0102  beqi a3,0x1,0x400e0140
400e0105  movi a2,-0x41
400e0108  and a8,a8,a2
400e010b  movi.n a2,0x3f
400e010d  or a8,a8,a2
400e0110  j 0x400e0185
400e0113  .data  ?? 00h
400e0114  beqi a3,0x3,0x400e0158
400e0117  bnei a3,0x4,0x400e0105
400e011a  movi.n a2,0xe6
400e011c  and a8,a8,a2
400e011f  movi a2,0x66
400e0122  j 0x400e010d
400e0125  beqi a3,0x7,0x400e017c
400e0128  bltui a3,0x7,0x400e0170
400e012b  movi a2,0x7f
400e012e  beqi a3,0x8,0x400e010d
400e0131  bne a3,a10,0x400e0105
400e0134  movi.n a2,0xef
400e0136  and a8,a8,a2
400e0139  movi a2,0x6f
400e013c  j 0x400e010d
400e013f  .data  ?? 00h
400e0140  movi a2,-0x7a
400e0143  and a8,a8,a2
400e0146  movi.n a2,0x6
400e0148  j 0x400e010d
400e014b  movi a2,-0x25
400e014e  and a8,a8,a2
400e0151  movi.n a2,0x5b
400e0153  j 0x400e010d
400e0156  .data  ?? 00h
400e0157  .data  ?? 00h
400e0158  movi a2,-0x31
400e015b  and a8,a8,a2
400e015e  movi.n a2,0x4f
400e0160  j 0x400e010d
400e0163  .data  ?? 00h
400e0164  movi.n a2,0xed
400e0166  and a8,a8,a2
400e0169  movi a2,0x6d
400e016c  j 0x400e010d
400e016f  .data  ?? 00h
400e0170  movi.n a2,0xfd
400e0172  and a8,a8,a2
400e0175  movi a2,0x7d
400e0178  j 0x400e010d
400e017b  .data  ?? 00h
400e017c  movi a2,-0x79
400e017f  and a8,a8,a2
400e0182  or a8,a3,a8
400e0185  s16i a8,a9,0x6
400e0188  j 0x400e00e1
400e018b  srli a3,a0,0x6
400e018e  mov a2,a0
400e0191  .data  ?? 74h    t
400e0192  extui a3,a3,0x0,0x8
400e0195  bltui a2,0x5,0x400e019b
400e0198  j 0x400e03f4
400e019b  l32r a8,0x400d1410
400e019e  addi.n a2,a2,-0x1
400e01a0  extui a2,a2,0x0,0x8
400e01a3  l32i.n a8,a8,0x0
400e01a5  slli a2,a2,0x1
400e01a8  add.n a8,a8,a2
400e01aa  movi a2,0x67
400e01ad  l16ui a9,a8,0x6
400e01b0  bne a3,a2,0x400e01b6
400e01b3  j 0x400e0394
400e01b6  bgeu a2,a3,0x400e01bc
400e01b9  j 0x400e0290
400e01bc  movi.n a2,0x4f
400e01be  bne a3,a2,0x400e01c4
400e01c1  j 0x400e0434
400e01c4  bltu a2,a3,0x400e021c
400e01c7  movi.n a2,0x44
400e01c9  bne a3,a2,0x400e01cf
400e01cc  j 0x400e044c
400e01cf  bltu a2,a3,0x400e01f5
400e01d2  movi.n a2,0x23
400e01d4  beq a3,a2,0x400e01ec
400e01d7  bgeu a2,a3,0x400e01ec
400e01da  movi a2,0x2b
400e01dd  bne a3,a2,0x400e01e3
400e01e0  j 0x400e047c
400e01e3  movi a2,0x3d
400e01e6  bne a3,a2,0x400e01ec
400e01e9  j 0x400e0470
400e01ec  movi a2,-0x80
400e01ef  and a9,a9,a2
400e01f2  j 0x400e025d
400e01f5  movi.n a2,0x46
400e01f7  bne a3,a2,0x400e01fd
400e01fa  j 0x400e0285
400e01fd  bgeu a3,a2,0x400e0203
