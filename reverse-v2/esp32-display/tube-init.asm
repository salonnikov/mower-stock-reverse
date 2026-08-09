400e6f8c  entry a1,0x20        ; <<< FUN_400e6f8c
400e6f8f  movi a11,0x1
400e6f92  movi.n a10,0x2
400e6f94  call8 0x4012cf40
400e6f97  l32r a2,0x400d1ca8
400e6f9a  l32i.n a2,a2,0x0
400e6f9c  call8 0x40093484
400e6f9f  mov.n a12,a10
400e6fa1  movi.n a14,0xff
400e6fa3  movi.n a13,0x0
400e6fa5  movi.n a11,0x2
400e6fa7  mov.n a10,a2
400e6fa9  call8 0x40094e04
400e6fac  retw.n
400e6fae  .data  ?? 00h
400e6faf  .data  ?? 00h
400e6fb0  entry a1,0x80
400e6fb3  movi.n a11,0x2
400e6fb5  movi a10,0x1a
400e6fb8  call8 0x4012d0e8
400e6fbb  movi.n a11,0x2
400e6fbd  mov.n a10,a11
400e6fbf  call8 0x4012d0e8
400e6fc2  movi.n a11,0x1
400e6fc4  movi.n a10,0x1a
400e6fc6  call8 0x4012cf40
400e6fc9  movi.n a12,0x1c
400e6fcb  movi.n a11,0x0
400e6fcd  movi.n a2,0x19
400e6fcf  addi a10,a1,0x14
400e6fd2  l32r a8,0x400d0190
400e6fd5  callx8 a8
400e6fd8  s32i.n a2,a1,0x0
400e6fda  movi.n a2,0xff
400e6fdc  movi.n a8,0x21
400e6fde  s32i.n a2,a1,0x4
400e6fe0  s32i.n a2,a1,0xc
400e6fe2  s32i.n a2,a1,0x10
400e6fe4  movi.n a12,0x28
400e6fe6  movi a2,0x80
400e6fe9  movi.n a11,0x0
400e6feb  addi a10,a1,0x30
400e6fee  s32i.n a8,a1,0x8
400e6ff0  s32i.n a2,a1,0x24
400e6ff2  l32r a8,0x400d0190
400e6ff5  callx8 a8
400e6ff8  l32r a2,0x400d1c74
400e6ffb  movi.n a12,0x1
400e6ffd  s32i.n a2,a1,0x3c
400e6fff  movi.n a2,0x20
400e7001  s32i a2,a1,0x44
400e7004  mov.n a11,a1
400e7006  movi.n a2,0x7
400e7008  mov.n a10,a12
400e700a  s32i a2,a1,0x4c
400e700d  call8 0x4012e7b0
400e7010  mov.n a2,a10
400e7012  movi.n a13,0x0
400e7014  l32r a14,0x400d1cac
400e7017  l32r a11,0x400d1cb0
400e701a  l32r a10,0x400d1cb4
400e701d  mov.n a12,a13
400e701f  call8 0x40094dc8
400e7022  l32r a8,0x400d1ca8
400e7025  s32i.n a10,a8,0x0
400e7027  call8 0x400e6f8c
400e702a  beqz.n a2,0x400e703c
400e702c  l32r a14,0x400d1cb8
400e702f  l32r a13,0x400d1cbc
400e7032  movi.n a12,0x43
400e7034  l32r a11,0x400d1cc0
400e7037  mov.n a10,a2
400e7039  call8 0x40091c08
400e703c  l32r a12,0x400d1ca4
400e703f  addi a11,a1,0x30
400e7042  movi a10,0x1
400e7045  call8 0x4012f01c
400e7048  mov.n a2,a10
400e704a  beqz.n a10,0x400e7058
400e704c  l32r a14,0x400d1cb8
400e704f  l32r a13,0x400d1cbc
400e7052  movi.n a12,0x45
400e7054  j 0x400e7034
400e7057  .data  ?? 00h
400e7058  call8 0x400e6f54
400e705b  mov a11,a2
400e705e  movi.n a10,0x1a
400e7060  call8 0x4012cf40
400e7063  .data  ?? ADh
400e7064  s32c1i a0,a5,0x3b8        ; <<< FUN_400e7064
400e7067  .data  ?? FFh
400e7068  retw.n
400e706a  cust0 {op2=0x3,r=a0,s=a0,t=a0}
400e706d  l32r a12,0x400af8a8
