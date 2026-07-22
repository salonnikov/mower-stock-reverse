
/* ===== FORCE @ 0x08070a94 ===== */
/* function: FUN_08070a94 size=86 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_08070a94(void)

{
  int iVar1;
  uint uVar2;
  undefined4 uVar3;
  
  iVar1 = _DAT_08070aec;
  if (*(int *)(_DAT_08070aec + 4) == 0) {
    if ((code *)*_DAT_08070af0 == (code *)0x0) {
      FUN_08048adc(0,0x8070b64,0x8070b2c,_DAT_08070af4,0x1db,0x8070b08,0x8070af8,_DAT_08070af4,0x1db
                  );
      do {
                    /* WARNING: Do nothing block with infinite loop */
      } while( true );
    }
    (*(code *)*_DAT_08070af0)(0x8070af8,_DAT_08070af4);
  }
  uVar2 = **(uint **)(iVar1 + 4);
  if ((uVar2 & 1) == 0) {
    uVar3 = 0xffffffff;
  }
  else {
    if (-1 < (int)(uVar2 << 0x1e)) {
      **(uint **)(iVar1 + 4) = uVar2 | 2;
    }
    uVar3 = 0;
  }
  return uVar3;
}


/* ===== FORCE @ 0x0805dc24 ===== */
/* function: FUN_0805dc24 size=10 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_0805dc24(void)

{
  return (int)*(char *)(*(int *)(_DAT_0805dc30 + 4) + 0xa0);
}

