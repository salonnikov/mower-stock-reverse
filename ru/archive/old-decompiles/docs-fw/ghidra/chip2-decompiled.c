
// ======== FUN_08000138 @ 08000138 size=44 ========

void FUN_08000138(void)

{
  int iVar1;
  code *UNRECOVERED_JUMPTABLE;
  int iVar2;
  undefined4 *puVar3;
  
  iVar1 = DAT_08000164;
  puVar3 = (undefined4 *)((int)&DAT_08000164 + DAT_08000164);
  iVar2 = DAT_08000164 + 0x8000163;
  if (puVar3 == (undefined4 *)((int)&DAT_08000164 + DAT_08000168)) {
    FUN_08000194();
  }
  UNRECOVERED_JUMPTABLE = *(code **)(iVar1 + 0x8000170);
  if (((uint)UNRECOVERED_JUMPTABLE & 1) != 0) {
    UNRECOVERED_JUMPTABLE = (code *)(iVar2 - (int)UNRECOVERED_JUMPTABLE);
  }
                    /* WARNING: Could not recover jumptable at 0x08000162. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*UNRECOVERED_JUMPTABLE)
            (*puVar3,*(undefined4 *)((int)&DAT_08000168 + iVar1),
             *(undefined4 *)(&LAB_0800016c + iVar1));
  return;
}


// ======== FUN_08000188 @ 08000188 size=8 ========

undefined8 FUN_08000188(undefined4 param_1,undefined4 param_2)

{
  FUN_08001ed0();
  return CONCAT44(param_2,param_1);
}


// ======== FUN_08000190 @ 08000190 size=4 ========

void FUN_08000190(void)

{
  return;
}


// ======== FUN_08000194 @ 08000194 size=48 ========

void FUN_08000194(void)

{
  undefined4 uVar1;
  undefined4 extraout_r2;
  undefined8 uVar2;
  
  uVar1 = FUN_080001fa();
  FUN_08000188(uVar1,extraout_r2);
  FUN_08001a34();
  uVar2 = FUN_08000244();
  FUN_08000190();
  FUN_08000260((int)uVar2,(int)((ulonglong)uVar2 >> 0x20));
  (*DAT_080001dc)();
                    /* WARNING: Could not recover jumptable at 0x080001ba. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*DAT_080001e0)();
  return;
}


// ======== FUN_080001d0 @ 080001d0 size=10 ========

undefined8 FUN_080001d0(void)

{
  return CONCAT44(DAT_080001e8,DAT_080001e4);
}


// ======== FUN_080001fa @ 080001fa size=74 ========

void FUN_080001fa(void)

{
  uint uVar1;
  undefined4 *puVar2;
  undefined4 unaff_lr;
  uint *puVar3;
  
  uVar1 = FUN_08000258();
  *(undefined4 *)((uVar1 & 0xfffffff8) + 0x5c) = unaff_lr;
  puVar3 = (uint *)((uVar1 & 0xfffffff8) + 0x58);
  *puVar3 = uVar1;
  FUN_080001d0();
  puVar2 = (undefined4 *)*puVar3;
  *puVar2 = 0;
  puVar2[1] = 0;
  puVar2[2] = 0;
  puVar2[3] = 0;
  puVar2[4] = 0;
  puVar2[5] = 0;
  puVar2[6] = 0;
  puVar2[7] = 0;
  puVar2[8] = 0;
  puVar2[9] = 0;
  puVar2[10] = 0;
  puVar2[0xb] = 0;
  puVar2[0xc] = 0;
  puVar2[0xd] = 0;
  puVar2[0xe] = 0;
  puVar2[0xf] = 0;
  return;
}


// ======== FUN_08000244 @ 08000244 size=18 ========

void FUN_08000244(undefined4 param_1,undefined4 param_2)

{
  FUN_08000190();
  FUN_08000260(param_1,param_2);
  (*DAT_080001dc)();
                    /* WARNING: Could not recover jumptable at 0x080001ba. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*DAT_080001e0)();
  return;
}


// ======== FUN_08000258 @ 08000258 size=4 ========

undefined4 FUN_08000258(void)

{
  return DAT_0800025c;
}


// ======== FUN_08000260 @ 08000260 size=8 ========

void FUN_08000260(void)

{
  software_bkpt(0xab);
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}


// ======== FUN_08000270 @ 08000270 size=50 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_08000270(void)

{
  FUN_08001da8(0x40004800);
  FUN_08001a48(0x27);
  FUN_08000820(0x40020000,2);
  _DAT_e000e010 = 0;
  _DAT_e000e018 = 0;
  _DAT_e000e014 = 0;
  return;
}


// ======== FUN_080002b8 @ 080002b8 size=484 ========

/* WARNING: Removing unreachable block (ram,0x08000498) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_080002b8(void)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  uint uVar4;
  uint local_4;
  
  local_4 = 0;
  do {
    if (((19999 < local_4 + 1) || (19999 < local_4 + 2)) || (19999 < local_4 + 3)) break;
    local_4 = local_4 + 4;
  } while (local_4 < 20000);
  uVar2 = _DAT_40021000 & 0xfef2ffff | 0x10001;
  iVar3 = 0xfffc;
  do {
    uVar4 = uVar2 << 0xe;
    uVar1 = uVar4;
    if (-1 < (int)uVar4) {
      uVar1 = uVar2;
    }
  } while (((-1 < (int)uVar4 && -1 < (int)(uVar1 << 0xe)) && (iVar3 != 0)) &&
          (((_DAT_40021000 & 0x20000) == 0 && (iVar3 = iVar3 + -4, -1 < (int)(uVar2 << 0xe)))));
  if (-1 < (int)(uVar2 << 0xe)) {
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  uVar2 = _DAT_40021000 & 0xfef2ffff | 0x1010001;
  do {
    if (0x7fffffff < uVar2 << 6) break;
  } while (-1 < (int)(uVar2 << 6));
  do {
    if (0x7fffffff < (uint)(_DAT_40007004 << 0xf)) break;
  } while (-1 < _DAT_40007004 << 0xf);
  do {
    if (0x7fffffff < (uint)(_DAT_40007004 << 0xe)) break;
  } while (-1 < _DAT_40007004 << 0xe);
  do {
  } while( true );
}


// ======== FUN_0800049c @ 0800049c size=54 ========

void FUN_0800049c(void)

{
  int iVar1;
  
  iVar1 = FUN_08001e64(0x40004800,0x40304);
  if (iVar1 == 0) {
    return;
  }
  FUN_08001d98(0x40004800);
  FUN_080007b4(0x40020000,2);
  FUN_08001c70();
  return;
}


// ======== FUN_080004d4 @ 080004d4 size=200 ========

void FUN_080004d4(void)

{
  undefined4 local_2c;
  undefined4 uStack_28;
  undefined4 local_24;
  undefined4 uStack_20;
  undefined4 local_1c;
  undefined4 local_18;
  undefined2 local_14;
  undefined1 local_12;
  
  FUN_08001bf8(0x712);
  FUN_08001da8(0x40004800);
  FUN_08001d2c(0x40004800,0x1c200);
  FUN_08001e88(0x40004800,0);
  FUN_08001ec0(0x40004800,0);
  FUN_08001ea4(0x40004800,0);
  FUN_08001eb4(0x40004800,8);
  FUN_08001e98(0x40004800,4);
  FUN_08001e24(0x40004800,0x40);
  FUN_08001e4c(0x40004800,0x304);
  FUN_08001a68(0x27,3,0);
  FUN_08001e30(0x40004800);
  local_2c = 0x40004804;
  uStack_28 = 0;
  local_24 = FUN_08001964();
  uStack_20 = 0;
  local_1c = 0x208;
  local_18 = 0x3000;
  local_14 = 0x100;
  local_12 = 0;
  FUN_08001bf8(0x500);
  FUN_08000820(0x40020000,2);
  FUN_0800085c(0x40020000,2,&local_2c);
  FUN_080007fc(0x40020000,2);
  FUN_080007d8(0x40020000,2);
  return;
}


// ======== FUN_080005a0 @ 080005a0 size=62 ========

void FUN_080005a0(void)

{
  bool bVar1;
  
  if (DAT_20000060 == '\0') {
    return;
  }
  if (((DAT_08010000 & 0x2ffe0000) == 0x20000000) && (&DAT_08010000 < PTR_FUN_0801a5f0_1_08010004))
  {
    FUN_08000270();
    bVar1 = (bool)isCurrentModePrivileged();
    if (bVar1) {
      setMainStackPointer(DAT_08010000);
    }
    (*(code *)PTR_FUN_0801a5f0_1_08010004)(PTR_FUN_0801a5f0_1_08010004,DAT_08010000);
  }
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}


// ======== FUN_080005e0 @ 080005e0 size=180 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_080005e0(void)

{
  uint uVar1;
  
  FUN_08001b10(0x300);
  FUN_08001c58(4);
  uVar1 = FUN_08001b24(1);
  uVar1 = uVar1 / 1000 - 1;
  if (uVar1 >> 0x18 == 0) {
    _DAT_e000e018 = 0;
    _DAT_e000e010 = 7;
    _DAT_e000e014 = uVar1;
  }
  DAT_e000ed23 = 0;
  FUN_08001bf8(0x602);
  FUN_08001bf8(0x606);
  FUN_08001bf8(0x603);
  FUN_08001bf8(0x604);
  FUN_08001bf8(0x605);
  FUN_08001bf8(0x600);
  FUN_080014d8(0x40010c00,0x18,4,0x400);
  FUN_080014d8(0x40010c00,4,4,0x800);
  FUN_080004d4();
  FUN_08001bf8(0x506);
  return;
}


// ======== FUN_08000694 @ 08000694 size=92 ========

void FUN_08000694(void)

{
  int iVar1;
  int iVar2;
  
  FUN_08000768(5);
  FUN_080008d0(0x40020000,2,&DAT_20000078);
  FUN_080008f0(0x40020000,2,0x100);
  FUN_080007d8(0x40020000,2);
  iVar1 = FUN_080014cc();
  do {
    if (DAT_20000068 != '\0') {
      return;
    }
    iVar2 = FUN_080014cc();
  } while ((uint)(iVar2 - iVar1) < 0xc9);
  DAT_20000060 = 1;
  return;
}


// ======== FUN_080006f0 @ 080006f0 size=100 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_080006f0(int param_1,uint param_2)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  
  if (param_2 != 0) {
    uVar3 = param_2 & 3;
    if (param_2 - 1 < 3) {
      uVar1 = 0;
    }
    else {
      uVar1 = 0;
      iVar2 = param_1 + -0x10;
      do {
        uVar1 = uVar1 + 4;
        _DAT_40023000 = *(undefined4 *)(iVar2 + 0x1c);
        iVar2 = iVar2 + 0x10;
      } while ((param_2 & 0xfffffffc) != uVar1);
    }
    if ((uVar3 != 0) && (_DAT_40023000 = *(undefined4 *)(param_1 + uVar1 * 4), uVar3 != 1)) {
      param_1 = param_1 + uVar1 * 4;
      _DAT_40023000 = *(undefined4 *)(param_1 + 4);
      if (uVar3 != 2) {
        _DAT_40023000 = *(undefined4 *)(param_1 + 8);
      }
    }
  }
  return _DAT_40023000;
}


// ======== FUN_08000754 @ 08000754 size=18 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_08000754(void)

{
  _DAT_40023008 = _DAT_40023008 | 1;
  return;
}


// ======== FUN_08000768 @ 08000768 size=36 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_08000768(int param_1)

{
  _DAT_20000064 = param_1;
  do {
    if (param_1 == 0) {
      return;
    }
    if (param_1 == 0) {
      return;
    }
    if (param_1 == 0) {
      return;
    }
  } while (param_1 != 0);
  return;
}


// ======== FUN_080007b4 @ 080007b4 size=36 ========

void FUN_080007b4(int param_1,uint param_2)

{
  if ((param_1 == 0x40020400) && (4 < param_2)) {
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  param_1 = param_1 + param_2 * 0x14;
  *(uint *)(param_1 + 8) = *(uint *)(param_1 + 8) & 0xfffffffe;
  return;
}


// ======== FUN_080007d8 @ 080007d8 size=36 ========

void FUN_080007d8(int param_1,uint param_2)

{
  if ((param_1 == 0x40020400) && (4 < param_2)) {
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  param_1 = param_1 + param_2 * 0x14;
  *(uint *)(param_1 + 8) = *(uint *)(param_1 + 8) | 1;
  return;
}


// ======== FUN_080007fc @ 080007fc size=36 ========

void FUN_080007fc(int param_1,uint param_2)

{
  if ((param_1 == 0x40020400) && (4 < param_2)) {
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  param_1 = param_1 + param_2 * 0x14;
  *(uint *)(param_1 + 8) = *(uint *)(param_1 + 8) & 0xffffffdf;
  return;
}


// ======== FUN_08000820 @ 08000820 size=60 ========

void FUN_08000820(int param_1,uint param_2)

{
  int iVar1;
  
  if ((param_1 == 0x40020400) && (4 < param_2)) {
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  iVar1 = param_1 + param_2 * 0x14;
  *(uint *)(iVar1 + 8) = *(uint *)(iVar1 + 8) & 0xfffffffe;
  *(undefined4 *)(iVar1 + 8) = 0;
  *(undefined4 *)(iVar1 + 0xc) = 0;
  *(undefined4 *)(iVar1 + 0x10) = 0;
  *(undefined4 *)(iVar1 + 0x14) = 0;
  *(uint *)(param_1 + 4) = 0xf << ((param_2 & 0x3f) << 2) | *(uint *)(param_1 + 4);
  return;
}


// ======== FUN_0800085c @ 0800085c size=116 ========

void FUN_0800085c(int param_1,uint param_2,undefined4 *param_3)

{
  uint uVar1;
  
  if ((param_1 == 0x40020400) && (4 < param_2)) {
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  param_1 = param_1 + param_2 * 0x14;
  *(undefined4 *)(param_1 + 0x10) = *param_3;
  *(undefined4 *)(param_1 + 0x14) = param_3[2];
  *(uint *)(param_1 + 0xc) = (uint)*(ushort *)(param_3 + 4);
  *(uint *)(param_1 + 8) =
       param_3[3] | param_3[1] | *(uint *)(param_1 + 8) & 0xffffc0ff | param_3[5];
  uVar1 = *(uint *)(param_1 + 8) & 0xffffffbf;
  if (*(char *)(param_3 + 6) == '\x01') {
    uVar1 = uVar1 + 0x40;
  }
  *(uint *)(param_1 + 8) = uVar1;
  uVar1 = *(uint *)(param_1 + 8) & 0xffffff7f;
  if (*(char *)((int)param_3 + 0x19) == '\x01') {
    uVar1 = uVar1 + 0x80;
  }
  *(uint *)(param_1 + 8) = uVar1;
  uVar1 = *(uint *)(param_1 + 8) & 0xffffffef;
  if (*(char *)((int)param_3 + 0x1a) != '\0') {
    uVar1 = uVar1 + 0x10;
  }
  *(uint *)(param_1 + 8) = uVar1;
  return;
}


// ======== FUN_080008d0 @ 080008d0 size=30 ========

void FUN_080008d0(int param_1,uint param_2,undefined4 param_3)

{
  if ((param_1 == 0x40020400) && (4 < param_2)) {
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  *(undefined4 *)(param_1 + param_2 * 0x14 + 0x14) = param_3;
  return;
}


// ======== FUN_080008f0 @ 080008f0 size=32 ========

void FUN_080008f0(int param_1,uint param_2,ushort param_3)

{
  if ((param_1 == 0x40020400) && (4 < param_2)) {
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  *(uint *)(param_1 + param_2 * 0x14 + 0xc) = (uint)param_3;
  return;
}


// ======== FUN_08000910 @ 08000910 size=44 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_08000910(void)

{
  _DAT_40022010 = _DAT_40022010 | 0x80;
  if (_DAT_1ffff7e0 < 0x201) {
    return;
  }
  _DAT_40022050 = _DAT_40022050 | 0x80;
  return;
}


// ======== FUN_0800093c @ 0800093c size=1450 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint FUN_0800093c(uint param_1)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  bool bVar4;
  
  if (_DAT_1ffff7e0 < 0x201) {
    iVar3 = -0xf0000;
    iVar2 = iVar3;
    do {
      bVar4 = (_DAT_4002200c & 1) == 0;
      uVar1 = 0;
      if (!bVar4) {
        uVar1 = _DAT_4002200c;
      }
      if (bVar4 || (uVar1 & 1) == 0) {
LAB_08000afa:
        if ((int)(_DAT_4002200c << 0x1b) < 0) {
          return 3;
        }
        uVar1 = _DAT_4002200c >> 1 & 2;
        if (uVar1 != 0) {
          return uVar1;
        }
        goto LAB_08000b22;
      }
      bVar4 = (_DAT_4002200c & 1) == 0;
      uVar1 = 0;
      if (!bVar4) {
        uVar1 = _DAT_4002200c;
      }
      if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000afa;
      bVar4 = (_DAT_4002200c & 1) == 0;
      uVar1 = 0;
      if (!bVar4) {
        uVar1 = _DAT_4002200c;
      }
      if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000afa;
      bVar4 = (_DAT_4002200c & 1) == 0;
      uVar1 = 0;
      if (!bVar4) {
        uVar1 = _DAT_4002200c;
      }
      if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000afa;
      bVar4 = (_DAT_4002200c & 1) == 0;
      uVar1 = 0;
      if (!bVar4) {
        uVar1 = _DAT_4002200c;
      }
      if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000afa;
      bVar4 = (_DAT_4002200c & 1) == 0;
      uVar1 = 0;
      if (!bVar4) {
        uVar1 = _DAT_4002200c;
      }
      if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000afa;
      bVar4 = (_DAT_4002200c & 1) == 0;
      uVar1 = 0;
      if (!bVar4) {
        uVar1 = _DAT_4002200c;
      }
      if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000afa;
      bVar4 = (_DAT_4002200c & 1) == 0;
      uVar1 = 0;
      if (!bVar4) {
        uVar1 = _DAT_4002200c;
      }
      if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000afa;
      bVar4 = (_DAT_4002200c & 1) == 0;
      uVar1 = 0;
      if (!bVar4) {
        uVar1 = _DAT_4002200c;
      }
      if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000afa;
      bVar4 = (_DAT_4002200c & 1) == 0;
      uVar1 = 0;
      if (!bVar4) {
        uVar1 = _DAT_4002200c;
      }
      if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000afa;
      bVar4 = (_DAT_4002200c & 1) == 0;
      uVar1 = 0;
      if (!bVar4) {
        uVar1 = _DAT_4002200c;
      }
      if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000afa;
      bVar4 = (_DAT_4002200c & 1) == 0;
      uVar1 = 0;
      if (!bVar4) {
        uVar1 = _DAT_4002200c;
      }
      if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000afa;
      iVar2 = iVar2 + 0x18;
    } while (iVar2 != 0);
  }
  else {
    iVar3 = -0xf0000;
    iVar2 = iVar3;
    if (param_1 < 0x807ffff) {
      do {
        bVar4 = (_DAT_4002200c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002200c;
        }
        if (bVar4 || (uVar1 & 1) == 0) {
LAB_08000cb4:
          if ((int)(_DAT_4002200c << 0x1b) < 0) {
            return 3;
          }
          uVar1 = _DAT_4002200c >> 1 & 2;
          if (uVar1 != 0) {
            return uVar1;
          }
          _DAT_40022014 = param_1;
          goto LAB_08000cdc;
        }
        bVar4 = (_DAT_4002200c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002200c;
        }
        if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000cb4;
        bVar4 = (_DAT_4002200c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002200c;
        }
        if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000cb4;
        bVar4 = (_DAT_4002200c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002200c;
        }
        if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000cb4;
        bVar4 = (_DAT_4002200c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002200c;
        }
        if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000cb4;
        bVar4 = (_DAT_4002200c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002200c;
        }
        if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000cb4;
        bVar4 = (_DAT_4002200c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002200c;
        }
        if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000cb4;
        bVar4 = (_DAT_4002200c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002200c;
        }
        if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000cb4;
        bVar4 = (_DAT_4002200c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002200c;
        }
        if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000cb4;
        bVar4 = (_DAT_4002200c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002200c;
        }
        if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000cb4;
        bVar4 = (_DAT_4002200c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002200c;
        }
        if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000cb4;
        bVar4 = (_DAT_4002200c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002200c;
        }
        if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000cb4;
        iVar2 = iVar2 + 0x18;
      } while (iVar2 != 0);
    }
    else {
      do {
        bVar4 = (_DAT_4002204c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002204c;
        }
        if (bVar4 || (uVar1 & 1) == 0) {
LAB_08000da6:
          if ((int)(_DAT_4002204c << 0x1b) < 0) {
            return 3;
          }
          uVar1 = _DAT_4002204c >> 1 & 2;
          if (uVar1 != 0) {
            return uVar1;
          }
          _DAT_40022054 = param_1;
          if (_DAT_4002201c << 0x1e < 0) {
            _DAT_40022014 = param_1;
          }
          goto LAB_08000dd4;
        }
        bVar4 = (_DAT_4002204c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002204c;
        }
        if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000da6;
        bVar4 = (_DAT_4002204c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002204c;
        }
        if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000da6;
        bVar4 = (_DAT_4002204c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002204c;
        }
        if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000da6;
        bVar4 = (_DAT_4002204c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002204c;
        }
        if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000da6;
        bVar4 = (_DAT_4002204c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002204c;
        }
        if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000da6;
        bVar4 = (_DAT_4002204c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002204c;
        }
        if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000da6;
        bVar4 = (_DAT_4002204c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002204c;
        }
        if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000da6;
        bVar4 = (_DAT_4002204c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002204c;
        }
        if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000da6;
        bVar4 = (_DAT_4002204c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002204c;
        }
        if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000da6;
        bVar4 = (_DAT_4002204c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002204c;
        }
        if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000da6;
        bVar4 = (_DAT_4002204c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002204c;
        }
        if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000da6;
        iVar2 = iVar2 + 0x18;
      } while (iVar2 != 0);
    }
  }
  return 4;
  while( true ) {
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000e8c;
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000e8c;
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000e8c;
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000e8c;
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000e8c;
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000e8c;
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000e8c;
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000e8c;
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000e8c;
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000e8c;
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000e8c;
    iVar3 = iVar3 + 0x18;
    if (iVar3 == 0) break;
LAB_08000b22:
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) {
LAB_08000e8c:
      if ((int)(_DAT_4002200c << 0x1b) < 0) {
        uVar1 = 3;
      }
      else {
        uVar1 = _DAT_4002200c >> 1 & 2;
      }
      goto LAB_08000ebe;
    }
  }
  uVar1 = 4;
LAB_08000ebe:
  _DAT_40022010 = _DAT_40022010 & 0xfffffffd | 0x40;
  _DAT_40022014 = param_1;
  return uVar1;
  while( true ) {
    bVar4 = (_DAT_4002204c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002204c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000eac;
    bVar4 = (_DAT_4002204c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002204c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000eac;
    bVar4 = (_DAT_4002204c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002204c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000eac;
    bVar4 = (_DAT_4002204c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002204c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000eac;
    bVar4 = (_DAT_4002204c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002204c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000eac;
    bVar4 = (_DAT_4002204c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002204c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000eac;
    bVar4 = (_DAT_4002204c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002204c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000eac;
    bVar4 = (_DAT_4002204c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002204c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000eac;
    bVar4 = (_DAT_4002204c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002204c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000eac;
    bVar4 = (_DAT_4002204c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002204c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000eac;
    bVar4 = (_DAT_4002204c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002204c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000eac;
    iVar3 = iVar3 + 0x18;
    if (iVar3 == 0) break;
LAB_08000dd4:
    bVar4 = (_DAT_4002204c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002204c;
    }
    if (bVar4 || (uVar1 & 1) == 0) {
LAB_08000eac:
      if ((int)(_DAT_4002204c << 0x1b) < 0) {
        uVar1 = 3;
      }
      else {
        uVar1 = _DAT_4002204c >> 1 & 2;
      }
      goto LAB_08000eda;
    }
  }
  uVar1 = 4;
LAB_08000eda:
  _DAT_40022050 = _DAT_40022050 & 0xfffffffd | 0x40;
  return uVar1;
  while( true ) {
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000e9c;
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000e9c;
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000e9c;
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000e9c;
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000e9c;
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000e9c;
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000e9c;
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000e9c;
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000e9c;
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000e9c;
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08000e9c;
    iVar3 = iVar3 + 0x18;
    if (iVar3 == 0) break;
LAB_08000cdc:
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) {
LAB_08000e9c:
      if ((int)(_DAT_4002200c << 0x1b) < 0) {
        uVar1 = 3;
      }
      else {
        uVar1 = _DAT_4002200c >> 1 & 2;
      }
      goto LAB_08000ecc;
    }
  }
  uVar1 = 4;
LAB_08000ecc:
  _DAT_40022010 = _DAT_40022010 & 0xfffffffd | 0x40;
  return uVar1;
}


// ======== FUN_08000ee8 @ 08000ee8 size=80 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_08000ee8(void)

{
  if (_DAT_40022010 << 0x18 < 0) {
    _DAT_40022004 = 0xcdef89ab;
  }
  if (0x200 < _DAT_1ffff7e0) {
    if (-1 < _DAT_40022050 << 0x18) {
      return;
    }
    _DAT_40022044 = 0xcdef89ab;
  }
  return;
}


// ======== FUN_08000f38 @ 08000f38 size=1426 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint FUN_08000f38(undefined4 *param_1,undefined4 param_2)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  bool bVar4;
  
  if (_DAT_1ffff7e0 < 0x201) {
    iVar2 = -0xf0000;
    iVar3 = iVar2;
    do {
      bVar4 = (_DAT_4002200c & 1) == 0;
      uVar1 = 0;
      if (!bVar4) {
        uVar1 = _DAT_4002200c;
      }
      if (bVar4 || (uVar1 & 1) == 0) {
LAB_080010fc:
        if ((int)(_DAT_4002200c << 0x1b) < 0) {
          return 3;
        }
        uVar1 = _DAT_4002200c >> 1 & 2;
        if (uVar1 != 0) {
          return uVar1;
        }
        *param_1 = param_2;
        goto LAB_0800111e;
      }
      bVar4 = (_DAT_4002200c & 1) == 0;
      uVar1 = 0;
      if (!bVar4) {
        uVar1 = _DAT_4002200c;
      }
      if (bVar4 || (uVar1 & 1) == 0) goto LAB_080010fc;
      bVar4 = (_DAT_4002200c & 1) == 0;
      uVar1 = 0;
      if (!bVar4) {
        uVar1 = _DAT_4002200c;
      }
      if (bVar4 || (uVar1 & 1) == 0) goto LAB_080010fc;
      bVar4 = (_DAT_4002200c & 1) == 0;
      uVar1 = 0;
      if (!bVar4) {
        uVar1 = _DAT_4002200c;
      }
      if (bVar4 || (uVar1 & 1) == 0) goto LAB_080010fc;
      bVar4 = (_DAT_4002200c & 1) == 0;
      uVar1 = 0;
      if (!bVar4) {
        uVar1 = _DAT_4002200c;
      }
      if (bVar4 || (uVar1 & 1) == 0) goto LAB_080010fc;
      bVar4 = (_DAT_4002200c & 1) == 0;
      uVar1 = 0;
      if (!bVar4) {
        uVar1 = _DAT_4002200c;
      }
      if (bVar4 || (uVar1 & 1) == 0) goto LAB_080010fc;
      bVar4 = (_DAT_4002200c & 1) == 0;
      uVar1 = 0;
      if (!bVar4) {
        uVar1 = _DAT_4002200c;
      }
      if (bVar4 || (uVar1 & 1) == 0) goto LAB_080010fc;
      bVar4 = (_DAT_4002200c & 1) == 0;
      uVar1 = 0;
      if (!bVar4) {
        uVar1 = _DAT_4002200c;
      }
      if (bVar4 || (uVar1 & 1) == 0) goto LAB_080010fc;
      bVar4 = (_DAT_4002200c & 1) == 0;
      uVar1 = 0;
      if (!bVar4) {
        uVar1 = _DAT_4002200c;
      }
      if (bVar4 || (uVar1 & 1) == 0) goto LAB_080010fc;
      bVar4 = (_DAT_4002200c & 1) == 0;
      uVar1 = 0;
      if (!bVar4) {
        uVar1 = _DAT_4002200c;
      }
      if (bVar4 || (uVar1 & 1) == 0) goto LAB_080010fc;
      bVar4 = (_DAT_4002200c & 1) == 0;
      uVar1 = 0;
      if (!bVar4) {
        uVar1 = _DAT_4002200c;
      }
      if (bVar4 || (uVar1 & 1) == 0) goto LAB_080010fc;
      bVar4 = (_DAT_4002200c & 1) == 0;
      uVar1 = 0;
      if (!bVar4) {
        uVar1 = _DAT_4002200c;
      }
      if (bVar4 || (uVar1 & 1) == 0) goto LAB_080010fc;
      iVar3 = iVar3 + 0x18;
    } while (iVar3 != 0);
  }
  else {
    iVar2 = -0xf0000;
    iVar3 = iVar2;
    if (param_1 < (undefined4 *)0x807ffff) {
      do {
        bVar4 = (_DAT_4002200c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002200c;
        }
        if (bVar4 || (uVar1 & 1) == 0) {
LAB_080012b2:
          if ((int)(_DAT_4002200c << 0x1b) < 0) {
            return 3;
          }
          uVar1 = _DAT_4002200c >> 1 & 2;
          if (uVar1 != 0) {
            return uVar1;
          }
          *param_1 = param_2;
          goto LAB_080012d4;
        }
        bVar4 = (_DAT_4002200c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002200c;
        }
        if (bVar4 || (uVar1 & 1) == 0) goto LAB_080012b2;
        bVar4 = (_DAT_4002200c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002200c;
        }
        if (bVar4 || (uVar1 & 1) == 0) goto LAB_080012b2;
        bVar4 = (_DAT_4002200c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002200c;
        }
        if (bVar4 || (uVar1 & 1) == 0) goto LAB_080012b2;
        bVar4 = (_DAT_4002200c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002200c;
        }
        if (bVar4 || (uVar1 & 1) == 0) goto LAB_080012b2;
        bVar4 = (_DAT_4002200c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002200c;
        }
        if (bVar4 || (uVar1 & 1) == 0) goto LAB_080012b2;
        bVar4 = (_DAT_4002200c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002200c;
        }
        if (bVar4 || (uVar1 & 1) == 0) goto LAB_080012b2;
        bVar4 = (_DAT_4002200c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002200c;
        }
        if (bVar4 || (uVar1 & 1) == 0) goto LAB_080012b2;
        bVar4 = (_DAT_4002200c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002200c;
        }
        if (bVar4 || (uVar1 & 1) == 0) goto LAB_080012b2;
        bVar4 = (_DAT_4002200c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002200c;
        }
        if (bVar4 || (uVar1 & 1) == 0) goto LAB_080012b2;
        bVar4 = (_DAT_4002200c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002200c;
        }
        if (bVar4 || (uVar1 & 1) == 0) goto LAB_080012b2;
        bVar4 = (_DAT_4002200c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002200c;
        }
        if (bVar4 || (uVar1 & 1) == 0) goto LAB_080012b2;
        iVar3 = iVar3 + 0x18;
      } while (iVar3 != 0);
    }
    else {
      do {
        bVar4 = (_DAT_4002204c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002204c;
        }
        if (bVar4 || (uVar1 & 1) == 0) {
LAB_0800139a:
          if ((int)(_DAT_4002204c << 0x1b) < 0) {
            return 3;
          }
          uVar1 = _DAT_4002204c >> 1 & 2;
          if (uVar1 != 0) {
            return uVar1;
          }
          *param_1 = param_2;
          goto LAB_080013b8;
        }
        bVar4 = (_DAT_4002204c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002204c;
        }
        if (bVar4 || (uVar1 & 1) == 0) goto LAB_0800139a;
        bVar4 = (_DAT_4002204c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002204c;
        }
        if (bVar4 || (uVar1 & 1) == 0) goto LAB_0800139a;
        bVar4 = (_DAT_4002204c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002204c;
        }
        if (bVar4 || (uVar1 & 1) == 0) goto LAB_0800139a;
        bVar4 = (_DAT_4002204c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002204c;
        }
        if (bVar4 || (uVar1 & 1) == 0) goto LAB_0800139a;
        bVar4 = (_DAT_4002204c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002204c;
        }
        if (bVar4 || (uVar1 & 1) == 0) goto LAB_0800139a;
        bVar4 = (_DAT_4002204c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002204c;
        }
        if (bVar4 || (uVar1 & 1) == 0) goto LAB_0800139a;
        bVar4 = (_DAT_4002204c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002204c;
        }
        if (bVar4 || (uVar1 & 1) == 0) goto LAB_0800139a;
        bVar4 = (_DAT_4002204c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002204c;
        }
        if (bVar4 || (uVar1 & 1) == 0) goto LAB_0800139a;
        bVar4 = (_DAT_4002204c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002204c;
        }
        if (bVar4 || (uVar1 & 1) == 0) goto LAB_0800139a;
        bVar4 = (_DAT_4002204c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002204c;
        }
        if (bVar4 || (uVar1 & 1) == 0) goto LAB_0800139a;
        bVar4 = (_DAT_4002204c & 1) == 0;
        uVar1 = 0;
        if (!bVar4) {
          uVar1 = _DAT_4002204c;
        }
        if (bVar4 || (uVar1 & 1) == 0) goto LAB_0800139a;
        iVar3 = iVar3 + 0x18;
      } while (iVar3 != 0);
    }
  }
  return 4;
  while( true ) {
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08001470;
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08001470;
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08001470;
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08001470;
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08001470;
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08001470;
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08001470;
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08001470;
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08001470;
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08001470;
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08001470;
    iVar2 = iVar2 + 0x18;
    if (iVar2 == 0) break;
LAB_0800111e:
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) {
LAB_08001470:
      if ((int)(_DAT_4002200c << 0x1b) < 0) {
        uVar1 = 3;
      }
      else {
        uVar1 = _DAT_4002200c >> 1 & 2;
      }
      goto LAB_080014a2;
    }
  }
  uVar1 = 4;
LAB_080014a2:
  _DAT_40022010 = _DAT_40022010 & 0xfffffffe;
  return uVar1;
  while( true ) {
    bVar4 = (_DAT_4002204c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002204c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08001490;
    bVar4 = (_DAT_4002204c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002204c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08001490;
    bVar4 = (_DAT_4002204c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002204c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08001490;
    bVar4 = (_DAT_4002204c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002204c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08001490;
    bVar4 = (_DAT_4002204c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002204c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08001490;
    bVar4 = (_DAT_4002204c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002204c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08001490;
    bVar4 = (_DAT_4002204c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002204c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08001490;
    bVar4 = (_DAT_4002204c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002204c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08001490;
    bVar4 = (_DAT_4002204c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002204c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08001490;
    bVar4 = (_DAT_4002204c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002204c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08001490;
    bVar4 = (_DAT_4002204c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002204c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08001490;
    iVar2 = iVar2 + 0x18;
    if (iVar2 == 0) break;
LAB_080013b8:
    bVar4 = (_DAT_4002204c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002204c;
    }
    if (bVar4 || (uVar1 & 1) == 0) {
LAB_08001490:
      if ((int)(_DAT_4002204c << 0x1b) < 0) {
        uVar1 = 3;
      }
      else {
        uVar1 = _DAT_4002204c >> 1 & 2;
      }
      goto LAB_080014be;
    }
  }
  uVar1 = 4;
LAB_080014be:
  _DAT_40022050 = _DAT_40022050 & 0xfffffffe;
  return uVar1;
  while( true ) {
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08001480;
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08001480;
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08001480;
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08001480;
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08001480;
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08001480;
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08001480;
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08001480;
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08001480;
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08001480;
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) goto LAB_08001480;
    iVar2 = iVar2 + 0x18;
    if (iVar2 == 0) break;
LAB_080012d4:
    bVar4 = (_DAT_4002200c & 1) == 0;
    uVar1 = 0;
    if (!bVar4) {
      uVar1 = _DAT_4002200c;
    }
    if (bVar4 || (uVar1 & 1) == 0) {
LAB_08001480:
      if ((int)(_DAT_4002200c << 0x1b) < 0) {
        uVar1 = 3;
      }
      else {
        uVar1 = _DAT_4002200c >> 1 & 2;
      }
      goto LAB_080014b0;
    }
  }
  uVar1 = 4;
LAB_080014b0:
  _DAT_40022010 = _DAT_40022010 & 0xfffffffe;
  return uVar1;
}


// ======== FUN_080014cc @ 080014cc size=12 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_080014cc(void)

{
  return _DAT_20000180;
}


// ======== FUN_080014d8 @ 080014d8 size=660 ========

/* WARNING: Type propagation algorithm not settling */

void FUN_080014d8(uint *param_1,uint param_2,uint param_3,uint param_4)

{
  uint *puVar1;
  uint uVar2;
  uint *puVar3;
  uint uVar4;
  uint *puVar5;
  
  uVar4 = param_2 & 0xf;
  if ((int)(param_2 << 0x1b) < 0) {
    if (param_3 == 4) {
      uVar4 = uVar4 | 3;
      param_1[0xf] = param_1[0xf] | param_4;
    }
    else {
      uVar4 = uVar4 | param_3;
    }
  }
  puVar1 = param_1 + 4;
  puVar5 = param_1 + 5;
  if ((param_4 & 1) != 0) {
    uVar2 = *param_1;
    puVar3 = puVar5;
    if ((param_2 == 0x28) || (puVar3 = puVar1, param_2 == 0x48)) {
      *puVar3 = param_4 & 1;
    }
    *param_1 = uVar2 & 0xfffffff0 | uVar4;
  }
  if ((param_4 & 2) != 0) {
    uVar2 = *param_1;
    puVar3 = puVar1;
    if ((param_2 == 0x48) || (puVar3 = puVar5, param_2 == 0x28)) {
      *puVar3 = param_4 & 2;
    }
    *param_1 = uVar2 & 0xffffff0f | uVar4 << 4;
  }
  if ((param_4 & 4) != 0) {
    uVar2 = *param_1;
    puVar3 = puVar1;
    if ((param_2 == 0x48) || (puVar3 = puVar5, param_2 == 0x28)) {
      *puVar3 = param_4 & 4;
    }
    *param_1 = uVar2 & 0xfffff0ff | uVar4 << 8;
  }
  if ((param_4 & 8) != 0) {
    uVar2 = *param_1;
    puVar3 = puVar1;
    if ((param_2 == 0x48) || (puVar3 = puVar5, param_2 == 0x28)) {
      *puVar3 = param_4 & 8;
    }
    *param_1 = uVar2 & 0xffff0fff | uVar4 << 0xc;
  }
  if ((param_4 & 0x10) != 0) {
    uVar2 = *param_1;
    puVar3 = puVar1;
    if ((param_2 == 0x48) || (puVar3 = puVar5, param_2 == 0x28)) {
      *puVar3 = param_4 & 0x10;
    }
    *param_1 = uVar2 & 0xfff0ffff | uVar4 << 0x10;
  }
  if ((param_4 & 0x20) != 0) {
    uVar2 = *param_1;
    puVar3 = puVar1;
    if ((param_2 == 0x48) || (puVar3 = puVar5, param_2 == 0x28)) {
      *puVar3 = param_4 & 0x20;
    }
    *param_1 = uVar2 & 0xff0fffff | uVar4 << 0x14;
  }
  if ((param_4 & 0x40) != 0) {
    uVar2 = *param_1;
    puVar3 = puVar1;
    if ((param_2 == 0x48) || (puVar3 = puVar5, param_2 == 0x28)) {
      *puVar3 = param_4 & 0x40;
    }
    *param_1 = uVar2 & 0xf0ffffff | uVar4 << 0x18;
  }
  if ((param_4 & 0x80) != 0) {
    uVar2 = *param_1;
    puVar3 = puVar1;
    if ((param_2 == 0x48) || (puVar3 = puVar5, param_2 == 0x28)) {
      *puVar3 = param_4 & 0x80;
    }
    *param_1 = uVar2 & 0xfffffff | uVar4 << 0x1c;
  }
  if ((param_4 & 0x100) != 0) {
    uVar2 = param_1[1];
    puVar3 = puVar5;
    if ((param_2 == 0x28) || (puVar3 = puVar1, param_2 == 0x48)) {
      *puVar3 = param_4 & 0x100;
    }
    param_1[1] = uVar2 & 0xfffffff0 | uVar4;
  }
  if ((param_4 & 0x200) != 0) {
    uVar2 = param_1[1];
    puVar3 = puVar1;
    if ((param_2 == 0x48) || (puVar3 = puVar5, param_2 == 0x28)) {
      *puVar3 = param_4 & 0x200;
    }
    param_1[1] = uVar2 & 0xffffff0f | uVar4 << 4;
  }
  if ((param_4 & 0x400) != 0) {
    uVar2 = param_1[1];
    puVar3 = puVar1;
    if ((param_2 == 0x48) || (puVar3 = puVar5, param_2 == 0x28)) {
      *puVar3 = param_4 & 0x400;
    }
    param_1[1] = uVar2 & 0xfffff0ff | uVar4 << 8;
  }
  if ((param_4 & 0x800) != 0) {
    uVar2 = param_1[1];
    puVar3 = puVar1;
    if ((param_2 == 0x48) || (puVar3 = puVar5, param_2 == 0x28)) {
      *puVar3 = param_4 & 0x800;
    }
    param_1[1] = uVar2 & 0xffff0fff | uVar4 << 0xc;
  }
  if ((param_4 & 0x1000) != 0) {
    uVar2 = param_1[1];
    puVar3 = puVar1;
    if ((param_2 == 0x48) || (puVar3 = puVar5, param_2 == 0x28)) {
      *puVar3 = param_4 & 0x1000;
    }
    param_1[1] = uVar2 & 0xfff0ffff | uVar4 << 0x10;
  }
  if ((param_4 & 0x2000) != 0) {
    uVar2 = param_1[1];
    puVar3 = puVar1;
    if ((param_2 == 0x48) || (puVar3 = puVar5, param_2 == 0x28)) {
      *puVar3 = param_4 & 0x2000;
    }
    param_1[1] = uVar2 & 0xff0fffff | uVar4 << 0x14;
  }
  if ((param_4 & 0x4000) != 0) {
    uVar2 = param_1[1];
    puVar3 = puVar1;
    if ((param_2 == 0x48) || (puVar3 = puVar5, param_2 == 0x28)) {
      *puVar3 = param_4 & 0x4000;
    }
    param_1[1] = uVar2 & 0xf0ffffff | uVar4 << 0x18;
  }
  if ((param_4 & 0x8000) == 0) {
    return;
  }
  uVar2 = param_1[1];
  if ((param_2 == 0x48) || (puVar1 = puVar5, param_2 == 0x28)) {
    *puVar1 = param_4 & 0x8000;
  }
  param_1[1] = uVar2 & 0xfffffff | uVar4 << 0x1c;
  return;
}


// ======== FUN_0800176c @ 0800176c size=94 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0800176c(void)

{
  int iVar1;
  
  _DAT_20000178 = &DAT_080017cc;
  do {
    iVar1 = FUN_08001e3c(0x40004800,7);
  } while (iVar1 == 0);
  FUN_08001da0(0x40004800,0x62);
  do {
    iVar1 = FUN_08001e3c(0x40004800,7);
  } while (iVar1 == 0);
  FUN_08001da0(0x40004800,0x74);
  do {
    iVar1 = FUN_08001e3c(0x40004800,7);
  } while (iVar1 == 0);
  FUN_08001da0(0x40004800,0x6c);
  FUN_08000768(2);
  return;
}


// ======== FUN_08001964 @ 08001964 size=12 ========

undefined1 * FUN_08001964(void)

{
  return &DAT_20000078;
}


// ======== FUN_08001970 @ 08001970 size=26 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_08001970(void)

{
  if (_DAT_2000006c == (code *)0x0) {
    return;
  }
  (*_DAT_2000006c)();
  _DAT_2000006c = (code *)0x0;
  return;
}


// ======== FUN_0800198c @ 0800198c size=162 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0800198c(void)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  
  uVar3 = (_DAT_2000007a & 0xff) << 8 | (uint)(_DAT_2000007a >> 8);
  FUN_08000ee8();
  if (uVar3 != 0) {
    iVar2 = 0x10;
    do {
      FUN_08000f38(_DAT_20000070 + _DAT_2000017c + iVar2 + -0x10,
                   *(undefined4 *)(&DAT_2000006c + iVar2));
      uVar1 = iVar2 - 0xc;
      iVar2 = iVar2 + 4;
    } while (uVar1 < uVar3);
  }
  FUN_08000910();
  _DAT_2000017c = _DAT_2000017c + uVar3;
  _DAT_20000178 = &DAT_08001ef2;
  do {
    iVar2 = FUN_08001e3c(0x40004800,7);
  } while (iVar2 == 0);
  FUN_08001da0(0x40004800,0x6f);
  do {
    iVar2 = FUN_08001e3c(0x40004800,7);
  } while (iVar2 == 0);
  FUN_08001da0(0x40004800,0x6b);
  do {
    iVar2 = FUN_08001e3c(0x40004800,7);
  } while (iVar2 == 0);
  FUN_08001da0(0x40004800,0x21);
  return;
}


// ======== FUN_08001a34 @ 08001a34 size=18 ========

void FUN_08001a34(void)

{
  FUN_080005e0();
  FUN_08000694();
  do {
    FUN_08001970();
    FUN_080005a0();
  } while( true );
}


// ======== FUN_08001a48 @ 08001a48 size=30 ========

void FUN_08001a48(uint param_1)

{
  *(int *)(param_1 >> 3 & 0xfffffffc | 0xe000e180) = 1 << (param_1 & 0x1f);
  return;
}


// ======== FUN_08001a68 @ 08001a68 size=168 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_08001a68(uint param_1,int param_2,uint param_3)

{
  int iVar1;
  
  if ((_DAT_e000ed0c & 0x700) == 0x700) {
    iVar1 = 4;
  }
  else if ((_DAT_e000ed0c & 0x700) == 0x600) {
    iVar1 = 3;
  }
  else if ((_DAT_e000ed0c & 0x700) == 0x500) {
    iVar1 = 2;
  }
  else if ((_DAT_e000ed0c & 0x700) == 0x400) {
    iVar1 = 1;
  }
  else if ((_DAT_e000ed0c & 0x700) == 0x300) {
    iVar1 = 0;
  }
  else {
    _DAT_e000ed0c = 0x5fa0500;
    iVar1 = 2;
  }
  *(char *)((param_1 | 0xe000e100) + 0x300) =
       (char)((0xfU >> (4U - iVar1 & 0xff) & param_3 | param_2 << iVar1) << 4);
  *(int *)((param_1 >> 3 & 0x1c) + 0xe000e100) = 1 << (param_1 & 0x1f);
  return;
}


// ======== FUN_08001b10 @ 08001b10 size=20 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_08001b10(uint param_1)

{
  _DAT_e000ed0c = param_1 | 0x5fa0000;
  return;
}


// ======== FUN_08001b24 @ 08001b24 size=192 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint FUN_08001b24(uint param_1)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  
  uVar1 = 8000000;
  if ((_DAT_40021004 & 0xc) == 8) {
    uVar3 = 4000000;
    if ((int)(_DAT_40021004 << 0xf) < 0) {
      uVar2 = 48000000;
      if (-1 < _DAT_4002102c << 1) {
        uVar2 = uVar1;
      }
      if (-1 < (int)(_DAT_40021004 << 0xe)) {
        uVar3 = uVar2;
      }
    }
    uVar2 = (_DAT_40021004 >> 0x19 & 0x20) +
            ((_DAT_40021004 & 0x3fffff) >> 0x12) + (_DAT_40021004 >> 0x17 & 0x10);
    uVar1 = uVar2;
    if (uVar2 != 0x3f) {
      uVar1 = uVar2 + 1;
    }
    if (uVar2 < 0xf) {
      uVar1 = uVar2 + 2;
    }
    uVar1 = uVar1 * uVar3;
  }
  if (param_1 < 4) {
    uVar3 = uVar1 >> (&DAT_08001be8)[(_DAT_40021004 & 0xff) >> 4];
    switch(param_1) {
    case 1:
      return uVar3;
    case 2:
      return uVar3 >> (&DAT_08001eea)[(_DAT_40021004 & 0x7ff) >> 8];
    case 3:
      uVar1 = uVar3 >> (&DAT_08001eea)[(_DAT_40021004 & 0x3fff) >> 0xb];
    }
    return uVar1;
  }
  return 0;
}


// ======== FUN_08001bf8 @ 08001bf8 size=30 ========

void FUN_08001bf8(uint param_1)

{
  *(uint *)(&DAT_40021000 + (param_1 >> 6)) =
       1 << (param_1 & 0x1f) | *(uint *)(&DAT_40021000 + (param_1 >> 6));
  return;
}


// ======== FUN_08001c38 @ 08001c38 size=30 ========

void FUN_08001c38(uint param_1)

{
  *(uint *)(&DAT_40021000 + (param_1 >> 6)) =
       1 << (param_1 & 0x1f) | *(uint *)(&DAT_40021000 + (param_1 >> 6));
  return;
}


// ======== FUN_08001c58 @ 08001c58 size=24 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_08001c58(int param_1)

{
  _DAT_e000e010 = _DAT_e000e010 & 0xfffffffb;
  if (param_1 == 4) {
    _DAT_e000e010 = _DAT_e000e010 + 4;
  }
  return;
}


// ======== FUN_08001c70 @ 08001c70 size=164 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_08001c70(void)

{
  if (DAT_20000078 == -0x5b) {
    _DAT_2000006c = &DAT_08001a31;
    switch(DAT_20000079) {
    case 1:
      DAT_20000068 = 1;
      _DAT_2000006c = (undefined1 *)0x800176d;
      break;
    default:
      _DAT_2000006c = &DAT_08001961;
      break;
    case 0x11:
      break;
    case 0x12:
      _DAT_20000074 =
           _DAT_2000007c << 0x18 | (_DAT_2000007c >> 8 & 0xff) << 0x10 |
           (_DAT_2000007c >> 0x10 & 0xff) << 8 | _DAT_2000007c >> 0x18;
      _DAT_2000006c = &LAB_080018d8_1;
      break;
    case 0x13:
      _DAT_2000006c = (undefined1 *)0x800198d;
      break;
    case 0x14:
      _DAT_20000070 =
           _DAT_2000007c << 0x18 | (_DAT_2000007c >> 8 & 0xff) << 0x10 |
           (_DAT_2000007c >> 0x10 & 0xff) << 8 | _DAT_2000007c >> 0x18;
      _DAT_2000006c = &DAT_08001961;
      break;
    case 0x15:
      _DAT_2000006c = &LAB_080017d0_1;
    }
  }
  else {
    _DAT_2000006c = &DAT_08001961;
  }
  FUN_080008d0(0x40020000,2,&DAT_20000078);
  FUN_080008f0(0x40020000,2,0x100);
  FUN_080007d8(0x40020000,2);
  return;
}


// ======== FUN_08001d2c @ 08001d2c size=106 ========

void FUN_08001d2c(int param_1,uint param_2)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = 0;
  if (param_1 < 0x40004c00) {
    if ((param_1 != 0x40004400) && (param_1 != 0x40004800)) goto LAB_08001d88;
LAB_08001d82:
    uVar2 = 2;
  }
  else {
    if ((param_1 == 0x40004c00) || (param_1 == 0x40005000)) goto LAB_08001d82;
    if (param_1 != 0x40013800) goto LAB_08001d88;
    uVar2 = 3;
  }
  iVar1 = FUN_08001b24(uVar2);
LAB_08001d88:
  *(uint *)(param_1 + 8) = (iVar1 + (param_2 >> 1)) / param_2 & 0xffff;
  return;
}


// ======== FUN_08001d98 @ 08001d98 size=8 ========

uint FUN_08001d98(int param_1)

{
  return *(uint *)(param_1 + 4) & 0x1ff;
}


// ======== FUN_08001da0 @ 08001da0 size=8 ========

void FUN_08001da0(int param_1,uint param_2)

{
  *(uint *)(param_1 + 4) = param_2 & 0x1ff;
  return;
}


// ======== FUN_08001da8 @ 08001da8 size=154 ========

void FUN_08001da8(int param_1)

{
  uint uVar1;
  
  if (param_1 < 0x40004c00) {
    if (param_1 == 0x40004400) {
      uVar1 = 0x411;
    }
    else {
      if (param_1 != 0x40004800) {
        return;
      }
      uVar1 = 0x412;
    }
  }
  else if (param_1 == 0x40004c00) {
    uVar1 = 0x413;
  }
  else if (param_1 == 0x40005000) {
    uVar1 = 0x414;
  }
  else {
    if (param_1 != 0x40013800) {
      return;
    }
    uVar1 = 0x30e;
  }
  FUN_08001c38(uVar1);
  *(uint *)(&DAT_40021000 + (uVar1 >> 6)) =
       *(uint *)(&DAT_40021000 + (uVar1 >> 6)) & ~(1 << (uVar1 & 0x1f));
  return;
}


// ======== FUN_08001e24 @ 08001e24 size=12 ========

void FUN_08001e24(int param_1,uint param_2)

{
  *(uint *)(param_1 + 0x14) = param_2 | *(uint *)(param_1 + 0x14) & 0xffffffbf;
  return;
}


// ======== FUN_08001e30 @ 08001e30 size=10 ========

void FUN_08001e30(int param_1)

{
  *(uint *)(param_1 + 0xc) = *(uint *)(param_1 + 0xc) | 0x2000;
  return;
}


// ======== FUN_08001e3c @ 08001e3c size=16 ========

uint FUN_08001e3c(int param_1,uint param_2)

{
  return *(uint *)((param_2 >> 6) + param_1) >> (param_2 & 0x1f) & 1;
}


// ======== FUN_08001e4c @ 08001e4c size=22 ========

void FUN_08001e4c(int param_1,uint param_2)

{
  *(uint *)((param_2 >> 6) + param_1) = 1 << (param_2 & 0x1f) | *(uint *)((param_2 >> 6) + param_1);
  return;
}


// ======== FUN_08001e64 @ 08001e64 size=34 ========

uint FUN_08001e64(int param_1,uint param_2)

{
  return *(uint *)((param_2 >> 0x16) + param_1) >> ((param_2 & 0x1fffff) >> 0x10) &
         *(uint *)(((param_2 & 0xffff) >> 6) + param_1) >> (param_2 & 0x1f) & 1;
}


// ======== FUN_08001e88 @ 08001e88 size=16 ========

void FUN_08001e88(int param_1,uint param_2)

{
  *(uint *)(param_1 + 0xc) = *(uint *)(param_1 + 0xc) & 0xfffff9ff;
  *(uint *)(param_1 + 0xc) = param_2 | *(uint *)(param_1 + 0xc);
  return;
}


// ======== FUN_08001e98 @ 08001e98 size=12 ========

void FUN_08001e98(int param_1,uint param_2)

{
  *(uint *)(param_1 + 0xc) = param_2 | *(uint *)(param_1 + 0xc) & 0xfffffffb;
  return;
}


// ======== FUN_08001ea4 @ 08001ea4 size=16 ========

void FUN_08001ea4(int param_1,uint param_2)

{
  *(uint *)(param_1 + 0x10) = *(uint *)(param_1 + 0x10) & 0xffffcfff;
  *(uint *)(param_1 + 0x10) = param_2 | *(uint *)(param_1 + 0x10);
  return;
}


// ======== FUN_08001eb4 @ 08001eb4 size=12 ========

void FUN_08001eb4(int param_1,uint param_2)

{
  *(uint *)(param_1 + 0xc) = param_2 | *(uint *)(param_1 + 0xc) & 0xfffffff7;
  return;
}


// ======== FUN_08001ec0 @ 08001ec0 size=16 ========

void FUN_08001ec0(int param_1,uint param_2)

{
  *(uint *)(param_1 + 0xc) = *(uint *)(param_1 + 0xc) & 0xffffefff;
  *(uint *)(param_1 + 0xc) = param_2 | *(uint *)(param_1 + 0xc);
  return;
}


// ======== FUN_08001ed0 @ 08001ed0 size=26 ========

uint FUN_08001ed0(void)

{
  uint in_fpscr;
  
  return in_fpscr & 0xfc3f0000 | 0x3000000;
}


// ======== FUN_08010138 @ 08010138 size=14 ========

void FUN_08010138(void)

{
                    /* WARNING: Could not recover jumptable at 0x0801013a. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*DAT_0801013c)();
  return;
}


// ======== FUN_08010164 @ 08010164 size=32 ========

void FUN_08010164(void)

{
  bool bVar1;
  undefined4 unaff_r8;
  undefined4 in_cr14;
  
  bVar1 = (bool)isCurrentModePrivileged();
  if (bVar1) {
    setMainStackPointer(*(undefined4 *)*puRam08010180);
  }
  enableIRQinterrupts();
  enableFIQinterrupts();
  DataSynchronizationBarrier(0xf);
  InstructionSynchronizationBarrier(0xf);
  software_interrupt(0);
  coprocessor_store(0,in_cr14,unaff_r8);
  *DAT_08010194 = *DAT_08010194 | 0xf00000;
  return;
}


// ======== FUN_08010184 @ 08010184 size=14 ========

void FUN_08010184(void)

{
  *DAT_08010194 = *DAT_08010194 | 0xf00000;
  return;
}


// ======== FUN_080101f4 @ 080101f4 size=6 ========

uint FUN_080101f4(void)

{
  bool bVar1;
  uint uVar2;
  
  uVar2 = 0;
  bVar1 = (bool)isCurrentModePrivileged();
  if (bVar1) {
    uVar2 = getCurrentExceptionNumber();
    uVar2 = uVar2 & 0x1f;
  }
  return uVar2;
}


// ======== FUN_08010268 @ 08010268 size=214 ========

undefined4 FUN_08010268(int *param_1,int *param_2,int param_3)

{
  int iVar1;
  int *piVar2;
  int *piVar3;
  
  iVar1 = -1 - param_3;
  for (piVar3 = param_1; piVar2 = param_1, piVar3 <= param_2; piVar3 = piVar3 + 1) {
    *piVar3 = param_3;
  }
  while( true ) {
    piVar3 = param_1;
    if (param_2 < piVar2) {
      while( true ) {
        if (param_2 < piVar3) {
          piVar3 = (int *)((int)param_2 - 0xf);
          while( true ) {
            if (piVar3 < param_1) {
              piVar3 = (int *)((int)param_2 - 0xf);
              while( true ) {
                if (piVar3 < param_1) {
                  while( true ) {
                    if (param_2 < param_1) {
                      return 1;
                    }
                    if (*param_1 != param_3) break;
                    param_1 = param_1 + 1;
                  }
                  return 0;
                }
                if (piVar3[3] != iVar1) {
                  return 0;
                }
                piVar3[3] = param_3;
                if (piVar3[2] != iVar1) break;
                piVar3[2] = param_3;
                if (piVar3[1] != iVar1) {
                  return 0;
                }
                piVar3[1] = param_3;
                if (*piVar3 != iVar1) {
                  return 0;
                }
                *piVar3 = param_3;
                piVar3 = piVar3 + -4;
              }
              return 0;
            }
            if (piVar3[3] != param_3) {
              return 0;
            }
            piVar3[3] = iVar1;
            if (piVar3[2] != param_3) {
              return 0;
            }
            piVar3[2] = iVar1;
            if (piVar3[1] != param_3) {
              return 0;
            }
            piVar3[1] = iVar1;
            if (*piVar3 != param_3) break;
            *piVar3 = iVar1;
            piVar3 = piVar3 + -4;
          }
          return 0;
        }
        if (*piVar3 != iVar1) {
          return 0;
        }
        *piVar3 = param_3;
        if (piVar3[1] != iVar1) {
          return 0;
        }
        piVar3[1] = param_3;
        if (piVar3[2] != iVar1) {
          return 0;
        }
        piVar3[2] = param_3;
        if (piVar3[3] != iVar1) break;
        piVar3[3] = param_3;
        piVar3 = piVar3 + 4;
      }
      return 0;
    }
    if (*piVar2 != param_3) {
      return 0;
    }
    *piVar2 = iVar1;
    if (piVar2[1] != param_3) {
      return 0;
    }
    piVar2[1] = iVar1;
    if (piVar2[2] != param_3) {
      return 0;
    }
    piVar2[2] = iVar1;
    if (piVar2[3] != param_3) break;
    piVar2[3] = iVar1;
    piVar2 = piVar2 + 4;
  }
  return 0;
}


// ======== FUN_0801033e @ 0801033e size=292 ========

undefined4 FUN_0801033e(int param_1,int param_2,int param_3)

{
  int *piVar1;
  undefined4 *puVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  
  *DAT_08010464 = *DAT_08010464 + 0xb;
  iVar3 = DAT_08010468;
  iVar4 = -1 - param_3;
  if (param_1 == param_2) {
    iVar5 = 4;
    do {
      *(int *)(param_1 + *(int *)(iVar3 + iVar5)) = param_3;
      iVar5 = iVar5 + 4;
    } while (iVar5 < 0x21);
    iVar5 = 4;
    do {
      if (*(int *)(param_1 + *(int *)(iVar3 + iVar5)) != param_3) {
        return 0;
      }
      *(int *)(param_1 + *(int *)(iVar3 + iVar5)) = iVar4;
      iVar5 = iVar5 + 4;
    } while (iVar5 < 0x21);
    iVar5 = 4;
    do {
      if (*(int *)(param_1 + *(int *)(iVar3 + iVar5)) != iVar4) {
        return 0;
      }
      *(int *)(param_1 + *(int *)(iVar3 + iVar5)) = param_3;
      iVar5 = iVar5 + 4;
    } while (iVar5 < 0x21);
    uVar6 = 0x24;
    do {
      uVar6 = uVar6 - 4;
      if (*(int *)(param_1 + *(int *)(iVar3 + uVar6)) != param_3) {
        return 0;
      }
      *(int *)(param_1 + *(int *)(iVar3 + uVar6)) = iVar4;
    } while (4 < uVar6);
    uVar6 = 0x24;
    do {
      uVar6 = uVar6 - 4;
      if (*(int *)(param_1 + *(int *)(iVar3 + uVar6)) != iVar4) {
        return 0;
      }
      *(int *)(param_1 + *(int *)(iVar3 + uVar6)) = param_3;
    } while (4 < uVar6);
    iVar4 = 4;
    do {
      if (*(int *)(param_1 + *(int *)(iVar3 + iVar4)) != param_3) {
        return 0;
      }
      iVar4 = iVar4 + 4;
    } while (iVar4 < 0x21);
  }
  else {
    iVar5 = 0;
    do {
      piVar1 = (int *)(iVar3 + iVar5);
      iVar5 = iVar5 + 4;
      *(undefined4 *)(param_2 + iVar5) = *(undefined4 *)(param_1 + *piVar1);
    } while (iVar5 < 0x15);
    iVar5 = 0;
    do {
      *(int *)(param_1 + *(int *)(iVar3 + iVar5)) = param_3;
      iVar5 = iVar5 + 4;
    } while (iVar5 < 0x15);
    iVar5 = 0;
    do {
      if (*(int *)(param_1 + *(int *)(iVar3 + iVar5)) != param_3) {
        return 0;
      }
      *(int *)(param_1 + *(int *)(iVar3 + iVar5)) = iVar4;
      iVar5 = iVar5 + 4;
    } while (iVar5 < 0x15);
    iVar5 = 0;
    do {
      if (*(int *)(param_1 + *(int *)(iVar3 + iVar5)) != iVar4) {
        return 0;
      }
      *(int *)(param_1 + *(int *)(iVar3 + iVar5)) = param_3;
      iVar5 = iVar5 + 4;
    } while (iVar5 < 0x15);
    iVar5 = 0x18;
    do {
      iVar5 = iVar5 + -4;
      if (*(int *)(param_1 + *(int *)(iVar3 + iVar5)) != param_3) {
        return 0;
      }
      *(int *)(param_1 + *(int *)(iVar3 + iVar5)) = iVar4;
    } while (iVar5 != 0);
    iVar5 = 0x18;
    do {
      iVar5 = iVar5 + -4;
      if (*(int *)(param_1 + *(int *)(iVar3 + iVar5)) != iVar4) {
        return 0;
      }
      *(int *)(param_1 + *(int *)(iVar3 + iVar5)) = param_3;
    } while (iVar5 != 0);
    iVar4 = 0;
    do {
      if (*(int *)(param_1 + *(int *)(iVar3 + iVar4)) != param_3) {
        return 0;
      }
      iVar4 = iVar4 + 4;
    } while (iVar4 < 0x15);
    iVar4 = 0x18;
    do {
      puVar2 = (undefined4 *)(param_2 + iVar4);
      iVar4 = iVar4 + -4;
      *(undefined4 *)(param_1 + *(int *)(iVar3 + iVar4)) = *puVar2;
    } while (iVar4 != 0);
  }
  *DAT_0801046c = *DAT_0801046c + -0xb;
  return 1;
}


// ======== FUN_08010470 @ 08010470 size=604 ========

undefined8 FUN_08010470(void)

{
  bool bVar1;
  undefined4 *puVar2;
  undefined4 uVar3;
  uint extraout_r1;
  int iVar4;
  uint uVar5;
  byte bVar6;
  
  uVar3 = getProcessStackPointer();
  bVar1 = (bool)isCurrentModePrivileged();
  if (bVar1) {
    setProcessStackPointer(DAT_080106bc);
  }
  iVar4 = getProcessStackPointer();
  if (iVar4 == DAT_080106bc) {
    bVar1 = (bool)isCurrentModePrivileged();
    if (bVar1) {
      setProcessStackPointer(DAT_080106c0);
    }
    iVar4 = getProcessStackPointer();
    if (iVar4 == DAT_080106c0) {
      bVar1 = (bool)isCurrentModePrivileged();
      if (bVar1) {
        setProcessStackPointer(uVar3);
      }
      uVar3 = UnsignedSaturate(0xffffffff,10);
      bVar6 = UnsignedDoesSaturate(0xffffffff,10);
      if ((bVar6 & 0x1f | 0x16) == 0x17) {
        return CONCAT44(uVar3,1);
      }
    }
  }
  *puRam080138a0 = 1;
  FUN_0801a510();
  puVar2 = DAT_080138bc;
  for (uVar5 = 0; uVar5 < extraout_r1; uVar5 = uVar5 + 1) {
    *puVar2 = *(undefined4 *)(uVar5 * 4);
  }
  return CONCAT44(extraout_r1,*puVar2);
}


// ======== FUN_080106c4 @ 080106c4 size=488 ========

/* WARNING: Removing unreachable block (ram,0x08019578) */

undefined8 FUN_080106c4(undefined4 param_1,undefined4 param_2)

{
  return CONCAT44(param_2,1);
}


// ======== FUN_080108a4 @ 080108a4 size=98 ========

longlong FUN_080108a4(uint param_1,int param_2,uint param_3,uint param_4)

{
  longlong lVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  bool bVar5;
  undefined8 uVar6;
  longlong lVar7;
  
  lVar1 = 0;
  iVar3 = 0x40;
  while (iVar4 = iVar3 + -1, 0 < iVar3) {
    uVar6 = FUN_08010f76(param_1,param_2,iVar4);
    uVar2 = (uint)((ulonglong)uVar6 >> 0x20);
    iVar3 = iVar4;
    if (param_4 < uVar2 || uVar2 - param_4 < (uint)(param_3 <= (uint)uVar6)) {
      uVar6 = FUN_08010f58(param_3,param_4,iVar4);
      bVar5 = param_1 < (uint)uVar6;
      param_1 = param_1 - (uint)uVar6;
      param_2 = (param_2 - (int)((ulonglong)uVar6 >> 0x20)) - (uint)bVar5;
      lVar7 = FUN_08010f58(1,0,iVar4);
      lVar1 = lVar7 + lVar1;
    }
  }
  return lVar1;
}


// ======== FUN_08010906 @ 08010906 size=12 ========

int FUN_08010906(int param_1)

{
  if (param_1 - 0x41U < 0x1a) {
    param_1 = param_1 + 0x20;
  }
  return param_1;
}


// ======== FUN_08010912 @ 08010912 size=40 ========

void FUN_08010912(undefined4 *param_1,undefined4 *param_2,uint param_3)

{
  undefined4 uVar1;
  bool bVar2;
  
  if ((((uint)param_1 | (uint)param_2) & 3) == 0) {
    for (; 3 < param_3; param_3 = param_3 - 4) {
      uVar1 = *param_2;
      param_2 = param_2 + 1;
      *param_1 = uVar1;
      param_1 = param_1 + 1;
    }
  }
  while (bVar2 = param_3 != 0, param_3 = param_3 - 1, bVar2) {
    *(undefined1 *)param_1 = *(undefined1 *)param_2;
    param_2 = (undefined4 *)((int)param_2 + 1);
    param_1 = (undefined4 *)((int)param_1 + 1);
  }
  return;
}


// ======== FUN_0801093a @ 0801093a size=14 ========

void FUN_0801093a(undefined1 *param_1,int param_2,undefined1 param_3)

{
  bool bVar1;
  
  while (bVar1 = param_2 != 0, param_2 = param_2 + -1, bVar1) {
    *param_1 = param_3;
    param_1 = param_1 + 1;
  }
  return;
}


// ======== FUN_08010948 @ 08010948 size=4 ========

void FUN_08010948(undefined4 param_1,undefined4 param_2)

{
  FUN_0801093a(param_1,param_2,0);
  return;
}


// ======== FUN_0801095e @ 0801095e size=36 ========

char * FUN_0801095e(char *param_1,char *param_2)

{
  char cVar1;
  char cVar2;
  char *pcVar3;
  char *pcVar4;
  
  pcVar3 = param_2;
  pcVar4 = param_1;
LAB_08010966:
  cVar1 = *param_1;
  cVar2 = *pcVar3;
  if (cVar1 != '\0') goto code_r0x08010970;
  goto LAB_08010974;
code_r0x08010970:
  param_1 = param_1 + 1;
  pcVar3 = pcVar3 + 1;
  if (cVar1 != cVar2) {
LAB_08010974:
    pcVar3 = pcVar4;
    if ((cVar2 == '\0') || (pcVar3 = (char *)0x0, cVar1 == '\0')) {
      return pcVar3;
    }
    param_1 = pcVar4 + 1;
    pcVar3 = param_2;
    pcVar4 = param_1;
  }
  goto LAB_08010966;
}


// ======== FUN_08010982 @ 08010982 size=14 ========

int FUN_08010982(char *param_1)

{
  char cVar1;
  char *pcVar2;
  char *pcVar3;
  
  pcVar3 = param_1;
  do {
    pcVar2 = pcVar3 + 1;
    cVar1 = *pcVar3;
    pcVar3 = pcVar2;
  } while (cVar1 != '\0');
  return (int)pcVar2 - (int)(param_1 + 1);
}


// ======== FUN_08010990 @ 08010990 size=28 ========

int FUN_08010990(int param_1,int param_2)

{
  byte bVar1;
  int iVar2;
  
  iVar2 = 0;
  while( true ) {
    bVar1 = *(byte *)(param_1 + iVar2);
    if ((bVar1 != *(byte *)(param_2 + iVar2)) || (bVar1 == 0)) break;
    iVar2 = iVar2 + 1;
  }
  return (uint)bVar1 - (uint)*(byte *)(param_2 + iVar2);
}


// ======== FUN_080109ac @ 080109ac size=18 ========

char * FUN_080109ac(char *param_1,char *param_2)

{
  char cVar1;
  char *pcVar2;
  
  pcVar2 = param_1;
  do {
    cVar1 = *param_2;
    *pcVar2 = cVar1;
    pcVar2 = pcVar2 + 1;
    param_2 = param_2 + 1;
  } while (cVar1 != '\0');
  return param_1;
}


// ======== FUN_080109be @ 080109be size=30 ========

void FUN_080109be(int param_1,int param_2,uint param_3)

{
  uint uVar1;
  
  for (uVar1 = 0;
      ((uVar1 < param_3 && (*(char *)(param_1 + uVar1) == *(char *)(param_2 + uVar1))) &&
      (*(char *)(param_1 + uVar1) != '\0')); uVar1 = uVar1 + 1) {
  }
  return;
}


// ======== FUN_080109dc @ 080109dc size=48 ========

void FUN_080109dc(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 *local_48 [6];
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 uStack_8;
  undefined4 uStack_4;
  
  local_48[0] = &uStack_8;
  local_20 = 0xffffffff;
  local_18 = 0;
  local_30 = DAT_08010a0c;
  local_2c = DAT_08010a10;
  local_24 = param_1;
  local_1c = param_1;
  uStack_8 = param_3;
  uStack_4 = param_4;
  FUN_08010fa4(&local_24,param_2,local_48);
  return;
}


// ======== FUN_08010a14 @ 08010a14 size=296 ========

void FUN_08010a14(undefined4 *param_1,byte *param_2,byte *param_3,int param_4)

{
  byte bVar1;
  ulonglong uVar2;
  int iVar3;
  uint uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  int iVar8;
  uint uVar9;
  bool bVar10;
  undefined8 uVar11;
  undefined8 uVar12;
  
  iVar8 = DAT_08010d6c;
  uVar6 = DAT_08010d68;
  bVar1 = *param_2;
  if (bVar1 == 0xff) {
    iVar3 = 0;
  }
  else {
    iVar3 = 0;
    while( true ) {
      param_2 = param_2 + 1;
      if (*param_2 == 0xff) break;
      iVar3 = (uint)*param_2 + iVar3 * 10;
    }
    if (bVar1 == 0x2d) {
      iVar3 = -iVar3;
    }
  }
  bVar10 = false;
  bVar1 = *param_3;
  uVar2 = 0;
  if ((bVar1 == 0x2d) || (bVar1 == 0x2b)) {
    param_3 = param_3 + 1;
    bVar10 = bVar1 == 0x2d;
  }
  while( true ) {
    bVar1 = *param_3;
    if (bVar1 == 0xff) break;
    param_3 = param_3 + 1;
    uVar2 = (uVar2 & 0xffffffff) * 10 + CONCAT44((int)(uVar2 >> 0x20) * 10,(uint)bVar1);
  }
  uVar9 = param_4 + iVar3;
  if ((uVar2 == 0) || ((int)uVar9 < -400)) {
    *param_1 = 0;
    param_1[1] = 0;
  }
  else {
    uVar5 = 0;
    uVar7 = 0;
    uVar11 = FUN_08011388((int)uVar2);
    uVar4 = uVar9;
    if ((int)uVar9 < 0) {
      uVar4 = -uVar9;
    }
    iVar3 = DAT_08010d6c + uVar4 * 0x100000;
    for (; uVar12 = CONCAT44(iVar8,uVar7), uVar4 != 0; uVar4 = (int)uVar4 >> 1) {
      if ((uVar4 & 1) != 0) {
        uVar12 = FUN_080112a4(uVar7,iVar8,uVar5,uVar6);
      }
      iVar8 = (int)((ulonglong)uVar12 >> 0x20);
      uVar7 = (undefined4)uVar12;
      uVar12 = FUN_080112a4(uVar5,uVar6);
      uVar6 = (undefined4)((ulonglong)uVar12 >> 0x20);
      uVar5 = (undefined4)uVar12;
    }
    if ((int)uVar9 < 0) {
      uVar11 = FUN_08010d74();
      uVar11 = FUN_08010d74((int)uVar11,(int)((ulonglong)uVar11 >> 0x20),uVar7,iVar8);
    }
    else {
      uVar11 = FUN_080112a4((int)uVar11,(int)((ulonglong)uVar11 >> 0x20),0,iVar3);
      uVar11 = FUN_080112a4((int)uVar11,(int)((ulonglong)uVar11 >> 0x20),uVar7,iVar8);
    }
    uVar9 = (uint)((ulonglong)uVar11 >> 0x20);
    if (bVar10) {
      uVar9 = uVar9 ^ 0x80000000;
    }
    *param_1 = (int)uVar11;
    param_1[1] = uVar9;
  }
  return;
}


// ======== FUN_08010b3c @ 08010b3c size=556 ========

int FUN_08010b3c(undefined4 param_1,undefined4 param_2,int *param_3,int *param_4)

{
  undefined1 *puVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  undefined4 *puVar5;
  undefined4 uVar6;
  uint uVar7;
  uint uVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  undefined1 *puVar12;
  int iVar13;
  char *pcVar14;
  char *pcVar15;
  undefined1 local_68;
  char local_67 [23];
  undefined4 local_50;
  undefined4 local_4c;
  undefined1 local_48 [12];
  undefined1 *local_3c;
  undefined4 uStack_34;
  undefined4 local_30;
  int *local_2c;
  int *piStack_28;
  
  uVar7 = param_4[1];
  iVar10 = param_4[2];
  iVar13 = 0;
  local_3c = (undefined1 *)0x2e;
  iVar9 = -1;
  uStack_34 = param_1;
  local_30 = param_2;
  local_2c = param_3;
  piStack_28 = param_4;
  do {
    iVar3 = iVar9;
    iVar9 = iVar3 + 1;
    puVar1 = (undefined1 *)(*(code *)param_4[6])(local_30);
    iVar2 = (*(code *)param_4[8])();
  } while (iVar2 != 0);
  if (puVar1 == (undefined1 *)0xffffffff) {
    return -1;
  }
  uVar7 = uVar7 & 0xfffff97f;
  if (0 < iVar10) {
    if (puVar1 != (undefined1 *)0x2b) {
      if (puVar1 != (undefined1 *)0x2d) goto LAB_08010b9a;
      uVar7 = uVar7 | 0x400;
    }
    iVar9 = iVar3 + 2;
    puVar1 = (undefined1 *)(*(code *)param_4[6])(local_30);
    iVar10 = iVar10 + -1;
  }
LAB_08010b9a:
  if ((int)(uVar7 << 0x15) < 0) {
    local_68 = 0x2d;
  }
  else {
    local_68 = 0x2b;
  }
  pcVar15 = local_67;
  puVar12 = local_48;
  while ((0 < iVar10 && (puVar1 == &Reserved5))) {
    iVar9 = iVar9 + 1;
    puVar1 = (undefined1 *)(*(code *)param_4[6])(local_30);
    uVar7 = uVar7 | 0x200;
    iVar10 = iVar10 + -1;
    *local_2c = iVar9;
  }
  if (puVar1 == local_3c) {
    uVar7 = uVar7 | 0x80;
    iVar2 = iVar9;
    while( true ) {
      iVar9 = iVar2 + 1;
      iVar10 = iVar10 + -1;
      puVar1 = (undefined1 *)(*(code *)param_4[6])(local_30);
      if (puVar1 != &Reserved5) break;
      iVar13 = iVar13 + -1;
      uVar7 = uVar7 | 0x200;
      *local_2c = iVar2 + 2;
      iVar2 = iVar9;
    }
  }
  do {
    if (iVar10 < 1) {
LAB_08010d04:
      (*(code *)param_4[7])(local_30);
      *pcVar15 = -1;
      *puVar12 = 0xff;
      local_50 = 0;
      local_4c = 0;
      FUN_08010a14(&local_50,local_48,&local_68,iVar13);
      if (-1 < (int)(uVar7 << 0x16)) {
        return -2;
      }
      if ((uVar7 & 0x24) != 0) {
        if ((uVar7 & 1) != 0) {
          return iVar9;
        }
        piVar4 = (int *)*param_4;
        *param_4 = (int)(piVar4 + 1);
        puVar5 = (undefined4 *)*piVar4;
        *puVar5 = local_50;
        puVar5[1] = local_4c;
        return iVar9;
      }
      uVar6 = FUN_080113d0(local_50,local_4c);
      if ((uVar7 & 1) != 0) {
        return iVar9;
      }
      puVar5 = (undefined4 *)*param_4;
      *param_4 = (int)(puVar5 + 1);
      *(undefined4 *)*puVar5 = uVar6;
      return iVar9;
    }
    if ((puVar1 == local_3c) && (-1 < (int)(uVar7 << 0x18))) {
      uVar8 = uVar7 | 0x80;
    }
    else {
      iVar2 = FUN_080120ac(puVar1);
      if (iVar2 == 0) {
        if ((0 < iVar10) &&
           (((puVar1 == (undefined1 *)0x65 || (puVar1 == (undefined1 *)0x45)) &&
            ((int)(uVar7 << 0x16) < 0)))) {
          uVar7 = uVar7 & 0xfffffcff;
          iVar11 = iVar10 + -1;
          iVar3 = (*(code *)param_4[6])(local_30);
          iVar2 = iVar9 + 1;
          if (0 < iVar11) {
            if (iVar3 != 0x2b) {
              if (iVar3 != 0x2d) goto LAB_08010c9e;
              uVar7 = uVar7 | 0x100;
            }
            iVar3 = (*(code *)param_4[6])(local_30);
            iVar11 = iVar10 + -2;
            iVar2 = iVar9 + 2;
          }
LAB_08010c9e:
          iVar9 = iVar2;
          if ((int)(uVar7 << 0x17) < 0) {
            local_48[0] = 0x2d;
          }
          else {
            local_48[0] = 0x2b;
          }
          puVar12 = local_48 + 1;
          local_3c = puVar12;
          while ((0 < iVar11 && (iVar10 = FUN_080120ac(iVar3), iVar10 != 0))) {
            iVar11 = iVar11 + -1;
            if (puVar12 < local_48 + 9) {
              *puVar12 = (char)(iVar3 - 0x30U);
              if (((iVar3 - 0x30U & 0xff) != 0) || (local_3c < puVar12)) {
                puVar12 = puVar12 + 1;
              }
            }
            else {
              iVar13 = DAT_08010d70;
              if (-1 < (int)(uVar7 << 0x17)) {
                iVar13 = 9999;
              }
            }
            iVar9 = iVar9 + 1;
            iVar3 = (*(code *)param_4[6])(local_30);
            *local_2c = iVar9;
            uVar7 = uVar7 | 0x200;
          }
        }
        goto LAB_08010d04;
      }
      uVar8 = uVar7 | 0x200;
      if (pcVar15 < local_67 + 0x12) {
        pcVar14 = pcVar15 + 1;
        *pcVar15 = (char)puVar1 + -0x30;
        pcVar15 = pcVar14;
        if ((int)(uVar7 << 0x18) < 0) {
          iVar13 = iVar13 + -1;
        }
      }
      else if (-1 < (int)(uVar7 << 0x18)) {
        iVar13 = iVar13 + 1;
      }
    }
    iVar10 = iVar10 + -1;
    if ((int)(uVar8 << 0x16) < 0) {
      *local_2c = iVar9 + 1;
    }
    iVar9 = iVar9 + 1;
    puVar1 = (undefined1 *)(*(code *)param_4[6])(local_30);
    uVar7 = uVar8;
  } while( true );
}


// ======== FUN_08010d74 @ 08010d74 size=252 ========

undefined8 FUN_08010d74(uint param_1,uint param_2,uint param_3,uint param_4)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  int iVar7;
  int iVar8;
  uint uVar9;
  bool bVar10;
  
  if ((param_1 != 0 || (param_2 & 0x7fffffff) != 0) && (param_3 != 0 || (param_4 & 0x7fffffff) != 0)
     ) {
    uVar5 = param_4 & 0xfffff | 0x100000;
    uVar4 = param_2 & 0xfffff | 0x100000;
    iVar7 = ((param_2 & 0x7fffffff) >> 0x14) - ((param_4 & 0x7fffffff) >> 0x14);
    iVar8 = iVar7 + 0x3fd;
    if (uVar5 < uVar4 || uVar4 - uVar5 < (uint)(param_3 <= param_1)) {
      iVar8 = iVar7 + 0x3fe;
    }
    else {
      bVar10 = CARRY4(param_1,param_1);
      param_1 = param_1 * 2;
      uVar4 = uVar4 * 2 + (uint)bVar10;
    }
    if (-1 < iVar8) {
      uVar3 = 0x100000;
      uVar6 = 0;
      uVar9 = 0;
      for (uVar2 = 0; uVar2 != 0 || uVar3 != 0; uVar2 = (uint)(uVar1 != 0) << 0x1f | uVar2 >> 1) {
        if (uVar5 < uVar4 || uVar4 - uVar5 < (uint)(param_3 <= param_1)) {
          bVar10 = param_1 < param_3;
          param_1 = param_1 - param_3;
          uVar4 = (uVar4 - uVar5) - (uint)bVar10;
          uVar6 = uVar6 | uVar2;
          uVar9 = uVar9 | uVar3;
        }
        uVar1 = uVar3 & 1;
        uVar3 = uVar3 >> 1;
        bVar10 = CARRY4(param_1,param_1);
        param_1 = param_1 * 2;
        uVar4 = uVar4 * 2 + (uint)bVar10;
      }
      if (param_1 != 0 || uVar4 != 0) {
        if (param_1 == param_3 && uVar4 == uVar5) {
          param_1 = 0;
          uVar4 = 0x80000000;
        }
        else if (uVar5 < uVar4 || uVar4 - uVar5 < (uint)(param_3 <= param_1)) {
          param_1 = 0xfffffffe;
          uVar4 = 0xffffffff;
        }
        else {
          param_1 = 1;
          uVar4 = 0;
        }
      }
      iVar7 = uVar9 + iVar8 * 0x100000 + ((param_2 ^ param_4) & 0x80000000);
      if ((int)uVar4 < 0) {
        bVar10 = 0xfffffffe < uVar6;
        uVar6 = uVar6 + 1;
        iVar7 = iVar7 + (uint)bVar10;
        if ((param_1 & 0x7fffffff) == 0 && uVar4 * 2 + (uint)CARRY4(param_1,param_1) == 0) {
          uVar6 = uVar6 & 0xfffffffe;
        }
      }
      return CONCAT44(iVar7,uVar6);
    }
  }
  return 0;
}


// ======== FUN_08010e52 @ 08010e52 size=34 ========

void FUN_08010e52(uint param_1)

{
  uint uVar1;
  
  uVar1 = (int)param_1 >> 0x1f;
  FUN_080110ba((param_1 ^ uVar1) - uVar1,0,0,0,0,uVar1 * -0x80000000,0x433);
  return;
}


// ======== FUN_08010e74 @ 08010e74 size=26 ========

void FUN_08010e74(undefined4 param_1)

{
  FUN_080110ba(param_1,0,0,0,0,0,0x433);
  return;
}


// ======== FUN_08010e8e @ 08010e8e size=62 ========

int FUN_08010e8e(int param_1,uint param_2)

{
  uint uVar1;
  
  uVar1 = (param_2 & 0x7fffffff) >> 0x14;
  if (uVar1 < 0x3ff) {
    param_1 = 0;
  }
  else {
    if (uVar1 < 0x434) {
      param_1 = FUN_08010f76(param_1,param_2 & 0xfffff | 0x100000,-(uVar1 - 0x433));
    }
    else {
      param_1 = param_1 << (uVar1 - 0x433 & 0xff);
    }
    if ((param_2 & 0x80000000) != 0) {
      return -param_1;
    }
  }
  return param_1;
}


// ======== FUN_08010ecc @ 08010ecc size=48 ========

int FUN_08010ecc(int param_1,int param_2)

{
  if (-1 < param_2) {
    param_1 = -param_1;
  }
  return param_1;
}


// ======== FUN_08010efc @ 08010efc size=48 ========

int FUN_08010efc(int param_1,int param_2)

{
  if (-1 < param_2) {
    param_1 = -param_1;
  }
  return param_1;
}


// ======== FUN_08010f2c @ 08010f2c size=44 ========

int FUN_08010f2c(uint param_1,uint param_2)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  
  iVar1 = 0;
  uVar2 = 0x20;
  while (uVar3 = uVar2 - 1, 0 < (int)uVar2) {
    uVar2 = uVar3;
    if (param_2 <= param_1 >> (uVar3 & 0xff)) {
      param_1 = param_1 - (param_2 << (uVar3 & 0xff));
      iVar1 = iVar1 + (1 << (uVar3 & 0xff));
    }
  }
  return iVar1;
}


// ======== FUN_08010f58 @ 08010f58 size=30 ========

longlong FUN_08010f58(uint param_1,int param_2,uint param_3)

{
  if (0x1f < (int)param_3) {
    return (ulonglong)(param_1 << (param_3 - 0x20 & 0xff)) << 0x20;
  }
  return CONCAT44(param_2 << (param_3 & 0xff) | param_1 >> (0x20 - param_3 & 0xff),
                  param_1 << (param_3 & 0xff));
}


// ======== FUN_08010f76 @ 08010f76 size=32 ========

ulonglong FUN_08010f76(uint param_1,uint param_2,uint param_3)

{
  if (0x1f < (int)param_3) {
    return (ulonglong)(param_2 >> (param_3 - 0x20 & 0xff));
  }
  return CONCAT44(param_2 >> (param_3 & 0xff),
                  param_1 >> (param_3 & 0xff) | param_2 << (0x20 - param_3 & 0xff));
}


// ======== FUN_08010f98 @ 08010f98 size=12 ========

undefined1 FUN_08010f98(undefined4 *param_1,int param_2)

{
  undefined1 uVar1;
  
  uVar1 = *(undefined1 *)*param_1;
  *param_1 = (undefined1 *)*param_1 + param_2;
  return uVar1;
}


// ======== FUN_08010fa4 @ 08010fa4 size=828 ========

/* WARNING: Type propagation algorithm not settling */

int FUN_08010fa4(undefined4 param_1,undefined4 param_2,undefined4 *param_3)

{
  bool bVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined4 *puVar5;
  int *piVar6;
  uint uVar7;
  int iVar8;
  undefined4 uVar9;
  int iVar10;
  bool bVar11;
  undefined4 local_58;
  undefined4 *local_54;
  uint local_50 [9];
  undefined4 local_2c;
  undefined4 *puStack_28;
  
  param_3[3] = param_2;
  param_3[5] = DAT_08010fb8;
  param_3[8] = DAT_08010fbc;
  param_3[4] = 0;
  iVar10 = 0;
  local_54 = param_3 + 3;
  bVar1 = true;
  iVar8 = 0;
  local_2c = param_1;
  puStack_28 = param_3;
LAB_08011478:
  do {
    while( true ) {
      iVar3 = (*(code *)param_3[5])(local_54,1);
      if (iVar3 == 0) {
        return iVar10;
      }
      if (iVar3 == 0x25) break;
      iVar4 = (*(code *)param_3[8])();
      if (iVar4 == 0) {
        iVar4 = (*(code *)param_3[6])(local_2c);
        if (iVar4 != iVar3) {
          (*(code *)param_3[7])(local_2c);
          goto joined_r0x080114d8;
        }
LAB_080114cc:
        iVar8 = iVar8 + 1;
      }
      else {
        do {
          (*(code *)param_3[5])(local_54,1);
          iVar3 = (*(code *)param_3[8])();
        } while (iVar3 != 0);
        (*(code *)param_3[5])(local_54,0xffffffff);
        while( true ) {
          (*(code *)param_3[6])(local_2c);
          iVar3 = (*(code *)param_3[8])();
          if (iVar3 == 0) break;
          iVar8 = iVar8 + 1;
        }
        (*(code *)param_3[7])(local_2c);
      }
    }
    iVar4 = 0;
    iVar3 = (*(code *)param_3[5])(local_54,0);
    if (iVar3 == 0x2a) {
      (*(code *)param_3[5])(local_54,1);
    }
    iVar2 = DAT_08011784;
    uVar7 = (uint)(iVar3 == 0x2a);
    while (iVar3 = (*(code *)param_3[5])(local_54,1), iVar3 - 0x30U < 10) {
      if (iVar2 < iVar4) {
        return iVar10;
      }
      iVar4 = iVar3 + iVar4 * 10 + -0x30;
      if (iVar4 < 0) {
        return iVar10;
      }
      uVar7 = uVar7 | 0x10;
    }
    if (-1 < (int)(uVar7 << 0x1b)) {
      iVar4 = 0x7fffffff;
    }
    if (iVar3 == 0x6c) {
      iVar3 = (*(code *)param_3[5])(local_54,1);
      if (iVar3 != 0x6c) {
        uVar7 = uVar7 | 4;
        goto LAB_08011584;
      }
LAB_0801155a:
      uVar7 = uVar7 | 2;
LAB_0801157c:
      iVar3 = (*(code *)param_3[5])(local_54,1);
    }
    else {
      if (iVar3 == 0x4c) {
        uVar7 = uVar7 | 0x20;
        goto LAB_0801157c;
      }
      if (iVar3 == 0x68) {
        iVar3 = (*(code *)param_3[5])(local_54,1);
        if (iVar3 == 0x68) {
          uVar7 = uVar7 | 0x800;
          goto LAB_0801157c;
        }
        uVar7 = uVar7 | 8;
      }
      else {
        if (iVar3 == 0x6a) goto LAB_0801155a;
        if ((iVar3 == 0x74) || (iVar3 == 0x7a)) goto LAB_0801157c;
      }
    }
LAB_08011584:
    param_3[1] = uVar7;
    param_3[2] = iVar4;
    if (iVar3 == 0x65) goto LAB_08011608;
    if (iVar3 < 0x66) {
      if (iVar3 == 0x58) {
LAB_08011688:
        param_3[1] = uVar7 | 0x40;
joined_r0x08011690:
        if ((int)(uVar7 << 0x1e) < 0) {
          iVar3 = -2;
        }
        else {
LAB_080116a8:
          iVar3 = -2;
        }
      }
      else if (iVar3 < 0x59) {
        if (iVar3 != 0x45) {
          if (iVar3 < 0x46) {
            if (iVar3 == 0x25) {
              iVar4 = (*(code *)param_3[6])(local_2c);
              if (iVar4 != 0x25) {
                (*(code *)param_3[7])(local_2c);
joined_r0x080114d8:
                if (iVar4 != -1) {
                  return iVar10;
                }
                if (iVar10 == 0) {
                  return -1;
                }
                return iVar10;
              }
              goto LAB_080114cc;
            }
            if (iVar3 != 0x41) {
              return iVar10;
            }
          }
          else if ((iVar3 != 0x46) && (iVar3 != 0x47)) {
            return iVar10;
          }
        }
LAB_08011608:
        iVar3 = FUN_08010b3c(0xfffffffe,local_2c,&local_58,param_3);
      }
      else {
        if (iVar3 != 0x5b) {
          if (iVar3 == 0x61) goto LAB_08011608;
          if (iVar3 != 99) {
            if (iVar3 != 100) {
              return iVar10;
            }
LAB_08011676:
            param_3[1] = uVar7 | 0x40;
            goto joined_r0x08011690;
          }
        }
LAB_080116b4:
        uVar9 = 0;
        if (iVar3 == 99) {
          if (-1 < (int)(uVar7 << 0x1b)) {
            param_3[2] = 1;
          }
          uVar9 = 1;
        }
        else if (iVar3 == 0x5b) {
          iVar3 = (*(code *)param_3[5])(local_54,1,(code *)param_3[5],0);
          bVar11 = iVar3 == 0x5e;
          if (bVar11) {
            iVar3 = (*(code *)param_3[5])(local_54,1);
          }
          if (param_3[4] == 0) {
            iVar4 = 0;
            do {
              local_50[iVar4] = 0;
              iVar4 = iVar4 + 1;
            } while (iVar4 < 8);
          }
          do {
            if (iVar3 == 0) {
              return iVar10;
            }
            if (param_3[4] == 0) {
              local_50[(int)(iVar3 + ((uint)(iVar3 >> 0x1f) >> 0x1b)) >> 5] =
                   local_50[(int)(iVar3 + ((uint)(iVar3 >> 0x1f) >> 0x1b)) >> 5] |
                   1 << (iVar3 % 0x20 & 0xffU);
            }
            iVar3 = (*(code *)param_3[5])(local_54,1);
          } while (iVar3 != 0x5d);
          if (bVar11) {
            iVar3 = 0;
            do {
              local_50[iVar3] = ~local_50[iVar3];
              iVar3 = iVar3 + 1;
            } while (iVar3 < 8);
          }
        }
        iVar3 = -2;
        local_58 = uVar9;
      }
      if (iVar3 < 0) {
        if (iVar3 != -1) {
          return iVar10;
        }
        if (bVar1) {
          return -1;
        }
        return iVar10;
      }
      if ((uVar7 & 1) == 0) {
        iVar10 = iVar10 + 1;
      }
      iVar8 = iVar8 + iVar3;
      bVar1 = false;
      goto LAB_08011478;
    }
    if (iVar3 == 0x6f) {
      param_3[1] = uVar7 | 0x40;
      goto joined_r0x08011690;
    }
    if (0x6f < iVar3) {
      if (iVar3 != 0x70) {
        if (iVar3 == 0x73) goto LAB_080116b4;
        if (iVar3 == 0x75) goto LAB_08011676;
        if (iVar3 != 0x78) {
          return iVar10;
        }
        goto LAB_08011688;
      }
      param_3[1] = uVar7 & 0xfffff7f1;
      goto LAB_080116a8;
    }
    if ((iVar3 == 0x66) || (iVar3 == 0x67)) goto LAB_08011608;
    if (iVar3 == 0x69) {
      param_3[1] = uVar7 | 0x40;
      goto joined_r0x08011690;
    }
    if (iVar3 != 0x6e) {
      return iVar10;
    }
    if ((uVar7 & 1) == 0) {
      puVar5 = (undefined4 *)*param_3;
      *param_3 = puVar5 + 1;
      piVar6 = (int *)*puVar5;
      if ((int)(uVar7 << 0x14) < 0) {
        *(char *)piVar6 = (char)iVar8;
      }
      else if ((int)(uVar7 << 0x1c) < 0) {
        *(short *)piVar6 = (short)iVar8;
      }
      else if ((int)(uVar7 << 0x1e) < 0) {
        *piVar6 = iVar8;
        piVar6[1] = iVar8 >> 0x1f;
      }
      else {
        *piVar6 = iVar8;
      }
    }
  } while( true );
}


// ======== FUN_08011000 @ 08011000 size=54 ========

void FUN_08011000(undefined4 param_1,int param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 *local_38;
  undefined4 local_34;
  undefined4 uStack_30;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 uStack_8;
  undefined4 uStack_4;
  
  local_38 = &uStack_8;
  *(undefined4 *)(param_2 + 0xc) = 0;
  local_20 = DAT_08011090;
  local_1c = DAT_08011094;
  local_34 = 4;
  uStack_30 = 0x7fffffff;
  local_18 = DAT_08011098;
  uStack_8 = param_3;
  uStack_4 = param_4;
  FUN_08010b3c(0,param_2,param_1,&local_38);
  return;
}


// ======== FUN_08011036 @ 08011036 size=90 ========

void FUN_08011036(undefined4 param_1,int param_2,int *param_3)

{
  int iVar1;
  int iVar2;
  int local_28;
  int local_24;
  int iStack_20;
  int local_18 [2];
  
  local_24 = 0xffffffff;
  local_18[0] = 0;
  local_28 = param_2;
  iStack_20 = param_2;
  iVar1 = FUN_08011000(local_18,&local_28,param_1);
  local_24 = param_2 + local_18[0];
  if (param_3 != (int *)0x0) {
    iVar2 = param_2;
    if (local_18[0] != 0) {
      iVar2 = local_24;
    }
    *param_3 = iVar2;
  }
  if ((local_18[0] != 0) && ((iVar1 < 1 || (local_28 != local_24)))) {
    local_24 = local_24 - param_2;
    local_28 = param_2;
    iStack_20 = param_2;
    FUN_08011000(local_18,&local_28,param_1);
  }
  return;
}


// ======== FUN_080110ba @ 080110ba size=156 ========

ulonglong FUN_080110ba(undefined4 param_1,int param_2,int param_3,int param_4,uint param_5,
                      int param_6,int param_7)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  bool bVar4;
  ulonglong uVar5;
  undefined8 uVar6;
  longlong lVar7;
  
  if (param_2 == 0) {
    iVar2 = LZCOUNT(param_1) + 0x20;
  }
  else {
    iVar2 = LZCOUNT(param_2);
  }
  uVar5 = FUN_08010f58();
  uVar3 = (uint)(uVar5 >> 0x20);
  if (((uint)uVar5 != 0 || param_3 != 0) || (uVar3 != 0 || param_4 != 0)) {
    if (param_3 != 0 || param_4 != 0) {
      uVar6 = FUN_08010f76(param_3,param_4,0x40 - iVar2);
      lVar7 = FUN_08010f58(param_3,param_4,iVar2);
      uVar5 = CONCAT44(uVar3 | (uint)((ulonglong)uVar6 >> 0x20),
                       (uint)uVar5 | (uint)uVar6 | (uint)(lVar7 != 0));
    }
    uVar3 = (uint)(uVar5 >> 0xb);
    iVar2 = (param_7 - iVar2) + 10;
    if (-1 < iVar2) {
      uVar1 = uVar3 + param_5;
      iVar2 = iVar2 * 0x100000 + (int)((uVar5 >> 0xb) >> 0x20) + param_6 +
              (uint)CARRY4(uVar3,param_5);
      if ((int)uVar5 * 0x200000 < 0) {
        bVar4 = 0xfffffffe < uVar1;
        uVar1 = uVar1 + 1;
        iVar2 = iVar2 + (uint)bVar4;
        if ((uVar5 & 0x3ff) == 0) {
          uVar1 = uVar1 & 0xfffffffe;
        }
      }
      return CONCAT44(iVar2,uVar1);
    }
    uVar5 = 0;
  }
  return uVar5;
}


// ======== FUN_08011156 @ 08011156 size=322 ========

uint FUN_08011156(uint param_1,uint param_2,uint param_3,uint param_4)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  bool bVar10;
  ulonglong uVar11;
  longlong lVar12;
  
  iVar1 = (int)(param_2 ^ param_4) >> 0x1f;
  iVar2 = -iVar1;
  uVar7 = param_3;
  uVar9 = param_4;
  if ((param_2 & 0x7fffffff) <= (param_4 & 0x7fffffff) &&
      (uint)(param_3 <= param_1) <= (param_2 & 0x7fffffff) - (param_4 & 0x7fffffff)) {
    uVar7 = param_1;
    uVar9 = param_2;
    param_2 = param_4;
    param_1 = param_3;
  }
  if ((uVar9 & 0x7fffffff) != 0 || uVar7 != 0) {
    uVar8 = param_2 >> 0x14;
    uVar3 = uVar8 & 0x7ff;
    iVar4 = uVar3 - ((uVar9 & 0x7fffffff) >> 0x14);
    if (iVar4 < 0x40) {
      uVar9 = uVar9 & 0xfffff | 0x100000;
      if (iVar2 != 0) {
        bVar10 = uVar7 != 0;
        uVar7 = -uVar7;
        uVar9 = -(uint)bVar10 - uVar9;
      }
      uVar11 = FUN_08010f58(uVar7,uVar9,0x40 - iVar4);
      uVar6 = (uint)(uVar11 >> 0x20);
      uVar5 = (uint)uVar11;
      lVar12 = FUN_0801142c(uVar7,uVar9,iVar4);
      lVar12 = lVar12 + CONCAT44(param_2,param_1);
      param_1 = (uint)lVar12;
      uVar7 = (uint)((ulonglong)lVar12 >> 0x20);
      if ((param_2 ^ uVar7) >> 0x14 != 0) {
        if (iVar2 == 0) {
          uVar11 = CONCAT44(uVar6 >> 1 | param_1 * -0x80000000,
                            (uint)((uVar11 & 0x100000000) != 0) << 0x1f | uVar5 >> 1);
          param_1 = (uint)((uVar7 + 0x100000 & 1) != 0) << 0x1f | param_1 >> 1;
        }
        else {
          if (iVar4 < 2) {
            uVar7 = FUN_080110ba(param_1,uVar7 + uVar8 * -0x100000 + 0x100000,uVar5,uVar6,0,
                                 (uVar8 & 0x800) << 0x14,uVar3);
            return uVar7;
          }
          param_1 = param_1 * 2 | uVar6 >> 0x1f;
          uVar11 = CONCAT44(uVar6 * 2 + (uint)CARRY4(uVar5,uVar5),uVar5 * 2);
        }
      }
    }
    else {
      uVar11 = CONCAT44(-(uint)(1 < (uint)(iVar1 * -2)),iVar1 * 2 + 1);
      param_1 = param_1 + iVar1;
    }
    if (((longlong)uVar11 < 0) &&
       (param_1 = param_1 + 1,
       (uVar11 & 0x7fffffff) == 0 &&
       (int)(uVar11 >> 0x20) * 2 + (uint)CARRY4((uint)uVar11,(uint)uVar11) == 0)) {
      param_1 = param_1 & 0xfffffffe;
    }
    return param_1;
  }
  return param_1;
}


// ======== FUN_080112a4 @ 080112a4 size=228 ========

void FUN_080112a4(uint param_1,uint param_2,uint param_3,uint param_4)

{
  longlong lVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  
  if ((param_1 != 0 || (param_2 & 0x7fffffff) != 0) && (param_3 != 0 || (param_4 & 0x7fffffff) != 0)
     ) {
    uVar6 = (uint)((ulonglong)param_1 * (ulonglong)param_3);
    uVar2 = param_2 & 0xfffff | 0x100000;
    uVar3 = param_4 & 0xfffff | 0x100000;
    uVar4 = param_1 >> 10 | param_2 << 0x16;
    uVar8 = param_3 >> 10 | param_4 << 0x16;
    lVar1 = (ulonglong)uVar4 * (ulonglong)uVar8;
    uVar7 = (uint)lVar1;
    iVar5 = uVar4 * (uVar3 >> 10) + (uVar2 >> 10) * uVar8 + (int)((ulonglong)lVar1 >> 0x20);
    uVar8 = ((param_1 * uVar3 +
             uVar2 * param_3 + (int)((ulonglong)param_1 * (ulonglong)param_3 >> 0x20)) -
            (iVar5 * 0x100000 | uVar7 >> 0xc)) - (uint)(uVar6 < uVar7 * 0x100000);
    uVar4 = param_1 >> 0x1a | uVar2 << 6;
    uVar2 = param_3 >> 0x1a | uVar3 << 6;
    uVar3 = uVar6 + uVar7 * -0x100000 >> 0x14 | uVar8 * 0x1000;
    lVar1 = (ulonglong)uVar4 * (ulonglong)uVar2 +
            (ulonglong)((iVar5 - uVar4 * uVar2) + (uVar8 >> 0x14) + (uint)CARRY4(uVar3,uVar7));
    FUN_080110ba((int)lVar1,(int)((ulonglong)lVar1 >> 0x20),uVar6 * 0x1000,uVar3 + uVar7,0,
                 (param_2 ^ param_4) & 0x80000000,
                 ((param_4 & 0x7fffffff) >> 0x14) + ((param_2 & 0x7fffffff) >> 0x14) + -0x3ff);
  }
  return;
}


// ======== FUN_08011388 @ 08011388 size=24 ========

void FUN_08011388(void)

{
  FUN_080110ba();
  return;
}


// ======== FUN_080113a0 @ 080113a0 size=48 ========

void FUN_080113a0(undefined4 param_1,uint param_2)

{
  uint uVar1;
  uint uVar2;
  
  uVar2 = (param_2 & 0x7fffffff) >> 0x14;
  uVar1 = param_2 & 0xfffff | 0x100000;
  if (uVar2 < 0x3ff) {
    return;
  }
  if (uVar2 < 0x434) {
    FUN_08010f76(param_1,uVar1,-(uVar2 - 0x433));
    return;
  }
  FUN_08010f58(param_1,uVar1);
  return;
}


// ======== FUN_080113d0 @ 080113d0 size=74 ========

uint FUN_080113d0(uint param_1,uint param_2)

{
  uint uVar1;
  int iVar2;
  
  if (param_1 != 0 || (param_2 & 0x7fffffff) != 0) {
    iVar2 = ((param_2 & 0x7fffffff) >> 0x14) - 0x380;
    if (0 < iVar2) {
      uVar1 = (param_1 >> 0x1d | (param_2 & 0xfffff) << 3) + (param_2 & 0x80000000) +
              iVar2 * 0x800000;
      if (-1 < (int)(param_1 * 8)) {
        return uVar1;
      }
      uVar1 = uVar1 + 1;
      if ((param_1 & 0xfffffff) == 0) {
        uVar1 = uVar1 & 0xfffffffe;
      }
      return uVar1;
    }
  }
  return 0;
}


// ======== FUN_08011408 @ 08011408 size=36 ========

undefined8 FUN_08011408(void)

{
  undefined4 *puVar1;
  uint uVar2;
  uint extraout_r2;
  int iVar3;
  undefined4 *puVar4;
  
  puVar1 = puRam08011428;
  for (puVar4 = puRam08011424; puVar4 < puVar1; puVar4 = puVar4 + 4) {
    (*(code *)puVar4[3])(*puVar4,puVar4[1],puVar4[2]);
  }
  uVar2 = FUN_08010138();
  if ((int)extraout_r2 < 0x20) {
    iVar3 = 0 >> (extraout_r2 & 0xff);
    uVar2 = uVar2 >> (extraout_r2 & 0xff) | 0 << (0x20 - extraout_r2 & 0xff);
  }
  else {
    uVar2 = 0 >> (extraout_r2 - 0x20 & 0xff);
    iVar3 = (int)uVar2 >> 0x1f;
  }
  return CONCAT44(iVar3,uVar2);
}


// ======== FUN_0801142c @ 0801142c size=36 ========

undefined8 FUN_0801142c(uint param_1,uint param_2,uint param_3)

{
  uint uVar1;
  int iVar2;
  
  if ((int)param_3 < 0x20) {
    iVar2 = (int)param_2 >> (param_3 & 0xff);
    uVar1 = param_1 >> (param_3 & 0xff) | param_2 << (0x20 - param_3 & 0xff);
  }
  else {
    uVar1 = (int)param_2 >> (param_3 - 0x20 & 0xff);
    iVar2 = (int)(param_2 | uVar1) >> 0x1f;
  }
  return CONCAT44(iVar2,uVar1);
}


// ======== FUN_08011450 @ 08011450 size=44 ========

uint FUN_08011450(byte param_1)

{
  byte bVar1;
  uint uVar2;
  
  if (param_1 < 0x80) {
    bVar1 = *(byte *)(DAT_0801181c + (uint)(param_1 >> 1));
    if ((param_1 & 1) == 0) {
      bVar1 = bVar1 >> 4;
    }
    uVar2 = bVar1 & 0xf;
  }
  else {
    uVar2 = 0;
  }
  return 1 << uVar2 & 0x1e00;
}


// ======== FUN_08011920 @ 08011920 size=34 ========

void FUN_08011920(void)

{
  undefined4 uVar1;
  int iVar2;
  
  uVar1 = DAT_08011944;
  iVar2 = FUN_08013ef0(DAT_08011944,1,8);
  if (iVar2 == 1) {
    FUN_08013ee4(uVar1,1,8);
    return;
  }
  return;
}


// ======== FUN_0801196c @ 0801196c size=50 ========

void FUN_0801196c(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  bool bVar1;
  byte bVar2;
  undefined4 uVar3;
  
  if (*DAT_080119a0 == '\0') {
    FUN_08011c6c();
  }
  bVar2 = 0;
  bVar1 = (bool)isCurrentModePrivileged();
  if (bVar1) {
    uVar3 = getBasePriority();
    bVar2 = (byte)uVar3;
  }
  bVar1 = (bool)isCurrentModePrivileged();
  if (bVar1) {
    setBasePriority(0x20);
  }
  FUN_080119a4(param_1,param_2,param_3);
  bVar1 = (bool)isCurrentModePrivileged();
  if (bVar1) {
    setBasePriority((uint)bVar2);
  }
  return;
}


// ======== FUN_080119a4 @ 080119a4 size=86 ========

uint FUN_080119a4(int param_1,undefined4 param_2,uint param_3)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  
  iVar3 = DAT_080119fc + param_1 * 0x18;
  iVar1 = *(int *)(iVar3 + 0x2c);
  iVar3 = iVar3 + 0x18;
  if (iVar1 == 0) {
    uVar2 = FUN_08011cd4(iVar3);
    if (param_3 <= uVar2) goto LAB_080119d2;
  }
  else {
    if (iVar1 == 1) {
      uVar2 = FUN_08011cd4(iVar3);
      if (uVar2 < param_3) {
        param_3 = uVar2;
      }
LAB_080119d2:
      FUN_08011d44(iVar3,param_2,param_3);
      return param_3;
    }
    if (iVar1 == 2) {
      uVar2 = FUN_08011cea(iVar3,param_2);
      return uVar2;
    }
  }
  return 0;
}


// ======== FUN_08011a00 @ 08011a00 size=26 ========

void FUN_08011a00(undefined4 param_1,undefined4 param_2)

{
  undefined4 uVar1;
  
  uVar1 = FUN_08010982(param_2);
  FUN_0801196c(param_1,param_2,uVar1);
  return;
}


// ======== FUN_08011a1a @ 08011a1a size=48 ========

undefined4 FUN_08011a1a(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 uVar1;
  undefined1 auStack_120 [260];
  undefined4 uStack_8;
  undefined4 uStack_4;
  
  uStack_8 = param_3;
  uStack_4 = param_4;
  FUN_08010948(auStack_120,0x100);
  uVar1 = FUN_08011e0c(auStack_120,param_2,&uStack_8);
  FUN_08011a00(param_1,auStack_120);
  return uVar1;
}


// ======== FUN_08011a4c @ 08011a4c size=96 ========

undefined4 FUN_08011a4c(void)

{
  uint *puVar1;
  uint *puVar2;
  uint uVar3;
  int iVar4;
  undefined4 uVar5;
  
  puVar2 = DAT_08011ab0;
  puVar1 = DAT_08011aac;
  uVar5 = 0;
  uVar3 = *DAT_08011aac;
  if ((*DAT_08011ab0 ^ uVar3) == 0xffffffff) {
    if (uVar3 < DAT_08011ab4) {
      if (DAT_08011ab4 - 0x10 < uVar3) {
        *DAT_08011aac = DAT_08011ab4 - 0x10;
      }
      iVar4 = FUN_0801033e(*puVar1,DAT_08011ab8 + 4,0x55555555);
      if (iVar4 != 1) goto LAB_08011a9a;
      uVar3 = *puVar1 + 0x10;
      *puVar1 = uVar3;
    }
    else {
      iVar4 = FUN_0801033e(DAT_08011ab8,DAT_08011ab8,0x55555555);
      uVar3 = DAT_08011abc;
      if (iVar4 == 1) {
        uVar5 = 2;
      }
      else {
        uVar5 = 1;
      }
      *puVar1 = DAT_08011abc;
    }
    *puVar2 = ~uVar3;
  }
  else {
LAB_08011a9a:
    uVar5 = 1;
  }
  return uVar5;
}


// ======== FUN_08011ac0 @ 08011ac0 size=14 ========

void FUN_08011ac0(void)

{
  uint uVar1;
  
  uVar1 = DAT_08011ad0;
  *DAT_08011ad4 = DAT_08011ad0;
  *DAT_08011ad8 = ~uVar1;
  return;
}


// ======== FUN_08011af0 @ 08011af0 size=80 ========

void FUN_08011af0(void)

{
  uint *puVar1;
  
  *DAT_08011b34 = *DAT_08011b34 | 0xf00000;
  puVar1 = DAT_08011b38;
  *DAT_08011b38 = *DAT_08011b38 | 1;
  puVar1[1] = 0;
  puVar1[0xb] = 0;
  *puVar1 = *puVar1 & DAT_08011b3c;
  puVar1[2] = 0x9f0000;
  *puVar1 = *puVar1 & 0xfffbffff;
  FUN_0801a07c();
  *DAT_08017c7c = DAT_08017c78 & 0x10000 | 0x8000000;
  return;
}


// ======== FUN_08011b40 @ 08011b40 size=36 ========

void FUN_08011b40(void)

{
  undefined4 uVar1;
  
  uVar1 = DAT_08011b64;
  FUN_0801b020(DAT_08011b64,1);
  FUN_0801b010(uVar1,1);
  if (*(code **)(DAT_08011b68 + 4) != (code *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x08011b60. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(DAT_08011b68 + 4))();
    return;
  }
  return;
}


// ======== FUN_08011bac @ 08011bac size=168 ========

void FUN_08011bac(void)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  undefined4 uVar3;
  undefined1 uVar4;
  int iVar5;
  int iVar6;
  
  uVar1 = DAT_08011c54;
  iVar5 = FUN_0801b246(DAT_08011c54,3);
  puVar2 = DAT_08011c58;
  if (iVar5 != 0) {
    FUN_0801b120(uVar1);
    FUN_0801b232(uVar1,3);
    puVar2[1] = puVar2[1] + 1;
  }
  iVar5 = FUN_0801b284(uVar1,0x308);
  if (iVar5 != 0) {
    FUN_0801b270(uVar1,0x308);
  }
  iVar6 = FUN_0801b284(uVar1,DAT_08011c5c);
  iVar5 = DAT_08011c60;
  if (iVar6 != 0) {
    uVar4 = FUN_0801b120(uVar1);
    if (*(uint *)(iVar5 + 0x80) < 0x80) {
      *(undefined1 *)(iVar5 + *(uint *)(iVar5 + 0x80)) = uVar4;
      *(int *)(iVar5 + 0x80) = *(int *)(iVar5 + 0x80) + 1;
    }
  }
  uVar3 = DAT_08011c64;
  iVar6 = FUN_0801b284(uVar1,DAT_08011c64);
  if (iVar6 != 0) {
    FUN_0801b120(uVar1);
    if (5 < *(uint *)(iVar5 + 0x80)) {
      if ((code *)*puVar2 != (code *)0x0) {
        (*(code *)*puVar2)(DAT_08011c60);
      }
      if (puVar2[1] != 0) {
        puVar2[1] = puVar2[1] + -1;
      }
    }
    FUN_0801b270(uVar1,uVar3);
    FUN_08010948(DAT_08011c60,0x80);
    *(undefined4 *)(iVar5 + 0x80) = 0;
  }
  return;
}


// ======== FUN_08011c6c @ 08011c6c size=76 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_08011c6c(void)

{
  int iVar1;
  
  iVar1 = _DAT_08011cb8;
  *(undefined4 *)(_DAT_08011cb8 + 0x10) = 2;
  *(undefined4 *)(iVar1 + 0x14) = 2;
  *(char **)(iVar1 + 0x18) = s_Terminal_08011cbb + 1;
  *(int *)(iVar1 + 0x1c) = iVar1 + 0x78;
  *(undefined4 *)(iVar1 + 0x20) = 0x100;
  *(undefined4 *)(iVar1 + 0x28) = 0;
  *(undefined4 *)(iVar1 + 0x24) = 0;
  *(char **)(iVar1 + 0x48) = s_Terminal_08011cbb + 1;
  *(undefined4 *)(iVar1 + 0x2c) = 0;
  *(int *)(iVar1 + 0x4c) = iVar1 + 0x178;
  *(undefined4 *)(iVar1 + 0x58) = 0;
  *(undefined4 *)(iVar1 + 0x50) = 0x10;
  *(undefined4 *)(iVar1 + 0x54) = 0;
  *(undefined4 *)(iVar1 + 0x5c) = 0;
  FUN_080109ac(iVar1 + 7,&DAT_08011cc8);
  FUN_080109ac(iVar1,s_SEGGER_08011ccc);
  *(undefined1 *)(iVar1 + 6) = 0x20;
  return;
}


// ======== FUN_08011cd4 @ 08011cd4 size=22 ========

int FUN_08011cd4(int param_1)

{
  int iVar1;
  uint uVar2;
  
  uVar2 = *(uint *)(param_1 + 0xc);
  if (uVar2 < *(uint *)(param_1 + 0x10)) {
    iVar1 = -uVar2;
  }
  else {
    iVar1 = *(int *)(param_1 + 8) - uVar2;
  }
  return iVar1 + *(uint *)(param_1 + 0x10) + -1;
}


// ======== FUN_08011cea @ 08011cea size=90 ========

int FUN_08011cea(int param_1,int param_2,uint param_3)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  
  uVar3 = *(uint *)(param_1 + 0xc);
  iVar5 = 0;
  do {
    uVar1 = *(uint *)(param_1 + 0x10);
    if (uVar3 < uVar1) {
      iVar4 = uVar1 - uVar3;
    }
    else {
      iVar4 = *(int *)(param_1 + 8) - (uVar3 - uVar1);
    }
    uVar2 = *(int *)(param_1 + 8) - uVar3;
    uVar1 = iVar4 - 1U;
    if (uVar2 <= iVar4 - 1U) {
      uVar1 = uVar2;
    }
    if (param_3 <= uVar1) {
      uVar1 = param_3;
    }
    FUN_08010912(*(int *)(param_1 + 4) + uVar3,param_2,uVar1);
    param_3 = param_3 - uVar1;
    uVar3 = uVar3 + uVar1;
    iVar5 = iVar5 + uVar1;
    param_2 = param_2 + uVar1;
    if (*(uint *)(param_1 + 8) == uVar3) {
      uVar3 = 0;
    }
    *(uint *)(param_1 + 0xc) = uVar3;
  } while (param_3 != 0);
  return iVar5;
}


// ======== FUN_08011d44 @ 08011d44 size=66 ========

void FUN_08011d44(int param_1,int param_2,uint param_3)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  
  iVar3 = *(int *)(param_1 + 0xc);
  uVar2 = *(int *)(param_1 + 8) - iVar3;
  iVar1 = *(int *)(param_1 + 4) + *(int *)(param_1 + 0xc);
  if (param_3 < uVar2) {
    FUN_08010912(iVar1,param_2);
    *(uint *)(param_1 + 0xc) = iVar3 + param_3;
  }
  else {
    FUN_08010912(iVar1,param_2,uVar2);
    FUN_08010912(*(undefined4 *)(param_1 + 4),param_2 + uVar2,param_3 - uVar2);
    *(uint *)(param_1 + 0xc) = param_3 - uVar2;
  }
  return;
}


// ======== FUN_08011d88 @ 08011d88 size=44 ========

undefined4 FUN_08011d88(undefined1 *param_1,int param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 uVar1;
  undefined1 *local_20;
  int iStack_1c;
  undefined4 uStack_4;
  
  uVar1 = 0;
  if (param_2 != 0) {
    iStack_1c = param_2 + -1;
    local_20 = param_1;
    uStack_4 = param_4;
    uVar1 = FUN_080120bc(param_3,&uStack_4,&local_20,DAT_08011db4);
    *local_20 = 0;
  }
  return uVar1;
}


// ======== FUN_08011db8 @ 08011db8 size=34 ========

undefined4 FUN_08011db8(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 uVar1;
  undefined4 uStack_10;
  undefined4 local_c;
  undefined4 uStack_8;
  undefined4 uStack_4;
  
  uStack_10 = param_1;
  local_c = param_2;
  uStack_8 = param_3;
  uStack_4 = param_4;
  uVar1 = FUN_080120bc(param_2,&uStack_8,&uStack_10,DAT_08011ddc);
  FUN_08012800(0,&uStack_10);
  return uVar1;
}


// ======== FUN_08011de0 @ 08011de0 size=40 ========

undefined4 FUN_08011de0(undefined1 *param_1,int param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 uVar1;
  undefined1 *local_18;
  int iStack_14;
  
  uVar1 = 0;
  if (param_2 != 0) {
    iStack_14 = param_2 + -1;
    local_18 = param_1;
    uVar1 = FUN_080120bc(param_3,param_4,&local_18,DAT_08011e08);
    *local_18 = 0;
  }
  return uVar1;
}


// ======== FUN_08011e0c @ 08011e0c size=30 ========

undefined8 FUN_08011e0c(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 uVar1;
  undefined4 uStack_18;
  undefined4 uStack_14;
  undefined4 uStack_10;
  
  uStack_18 = param_1;
  uStack_14 = param_2;
  uStack_10 = param_3;
  uVar1 = FUN_080120bc(param_2,param_3,&uStack_18,DAT_08011e2c);
  FUN_08012800(0,&uStack_18);
  return CONCAT44(uStack_18,uVar1);
}


// ======== FUN_08011e30 @ 08011e30 size=88 ========

undefined8 FUN_08011e30(int param_1,undefined4 param_2)

{
  int *piVar1;
  int *piVar2;
  int *piVar3;
  int *piVar4;
  
  if (param_1 == 0) {
    return CONCAT44(param_2,param_2);
  }
  piVar4 = (int *)(param_1 - 4);
  piVar2 = (int *)*DAT_08011e88;
  piVar1 = (int *)0x0;
  for (piVar3 = piVar2; (piVar3 != (int *)0x0 && (piVar3 <= piVar4)); piVar3 = (int *)piVar3[1]) {
    piVar1 = piVar3;
  }
  if (piVar1 == (int *)0x0) {
    *DAT_08011e88 = (uint)piVar4;
  }
  else {
    piVar2 = (int *)piVar1[1];
    if ((int)piVar4 - (int)piVar1 == *piVar1) {
      *piVar1 = *piVar1 + *piVar4;
      piVar4 = piVar1;
    }
    else {
      piVar1[1] = (int)piVar4;
    }
  }
  if ((piVar3 != (int *)0x0) && ((int)piVar3 - (int)piVar4 == *piVar4)) {
    *piVar4 = *piVar4 + *piVar3;
    piVar3 = (int *)piVar3[1];
  }
  piVar4[1] = (int)piVar3;
  return CONCAT44(piVar2,piVar1);
}


// ======== FUN_08011e90 @ 08011e90 size=30 ========

undefined4 FUN_08011e90(undefined4 param_1,undefined4 param_2)

{
  undefined8 local_10;
  
  local_10 = DAT_08011eb0;
  FUN_08011036(&local_10,param_1,param_2);
  return (int)local_10;
}


// ======== FUN_08011eb8 @ 08011eb8 size=64 ========

int FUN_08011eb8(undefined4 param_1)

{
  int *piVar1;
  uint *puVar2;
  int *piVar3;
  uint uVar4;
  int iVar5;
  
  piVar3 = DAT_08011f00;
  puVar2 = DAT_08011efc;
  piVar1 = DAT_08011ef8;
  uVar4 = DAT_08011f04 - (int)DAT_08011efc;
  while( true ) {
    iVar5 = FUN_080177fc(param_1,0);
    if (iVar5 != 0) {
      return iVar5;
    }
    if (*piVar1 != 0) break;
    *piVar3 = (int)puVar2;
    *puVar2 = uVar4 & 0xfffffff8;
    puVar2[1] = 0;
    *piVar1 = 1;
  }
  return 0;
}


// ======== thunk_FUN_08011f10 @ 08011f08 size=2 ========

void thunk_FUN_08011f10(undefined4 *param_1,undefined4 *param_2,int param_3)

{
  undefined4 uVar1;
  
  for (; param_3 != 0; param_3 = param_3 + -4) {
    uVar1 = *param_1;
    param_1 = param_1 + 1;
    *param_2 = uVar1;
    param_2 = param_2 + 1;
  }
  return;
}


// ======== FUN_08011f10 @ 08011f10 size=12 ========

void FUN_08011f10(undefined4 *param_1,undefined4 *param_2,int param_3)

{
  undefined4 uVar1;
  
  for (; param_3 != 0; param_3 = param_3 + -4) {
    uVar1 = *param_1;
    param_1 = param_1 + 1;
    *param_2 = uVar1;
    param_2 = param_2 + 1;
  }
  return;
}


// ======== FUN_08011f28 @ 08011f28 size=366 ========

void FUN_08011f28(int *param_1,int param_2,int param_3,uint param_4,uint param_5,int param_6)

{
  int iVar1;
  uint uVar2;
  char extraout_r2;
  int iVar3;
  uint uVar4;
  int iVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  bool bVar9;
  undefined1 uVar10;
  longlong lVar11;
  undefined8 uVar12;
  longlong lVar13;
  
  uVar2 = 0;
  if (param_3 == 0 && param_4 == 0) {
    if (param_6 == 1) {
      uVar2 = ~param_5;
    }
    *param_1 = (int)&DAT_080120a0;
    param_1[1] = uVar2;
    param_1[2] = 1;
    param_1[3] = param_6;
  }
  else {
    iVar5 = (int)(((param_4 >> 0x14) - 0x3ff) * 0x4d10) >> 0x10;
    do {
      while( true ) {
        if (param_6 == 1) {
          uVar2 = -param_5;
        }
        else {
          uVar2 = (iVar5 - param_5) + 1;
        }
        lVar11 = (ulonglong)DAT_08012098 << 0x20;
        lVar13 = (ulonglong)DAT_0801209c << 0x20;
        uVar4 = uVar2;
        if ((int)uVar2 < 0) {
          uVar4 = -uVar2;
        }
        iVar1 = DAT_0801209c + uVar4 * 0x100000;
        while( true ) {
          uVar6 = (undefined4)((ulonglong)lVar11 >> 0x20);
          uVar8 = (undefined4)((ulonglong)lVar13 >> 0x20);
          uVar7 = (undefined4)lVar13;
          if (uVar4 == 0) break;
          if ((uVar4 & 1) != 0) {
            lVar13 = FUN_080112a4(uVar7,uVar8,(int)lVar11,uVar6);
          }
          lVar11 = FUN_080112a4((int)lVar11,uVar6);
          uVar4 = (int)uVar4 >> 1;
        }
        uVar10 = 1;
        if ((int)uVar2 < 0) {
          uVar12 = FUN_080112a4();
          uVar12 = FUN_080112a4((int)uVar12,(int)((ulonglong)uVar12 >> 0x20),uVar7,uVar8);
        }
        else {
          uVar12 = FUN_08010d74(param_3,param_4,0,iVar1);
          uVar12 = FUN_08010d74((int)uVar12,(int)((ulonglong)uVar12 >> 0x20),uVar7,uVar8);
        }
        bVar9 = true;
        FUN_08010efc();
        if ((bool)uVar10 && !bVar9) {
          FUN_08011156((int)uVar12,(int)((ulonglong)uVar12 >> 0x20),0,DAT_080120a8);
          lVar13 = FUN_080113a0();
        }
        else {
          lVar13 = -1;
        }
        iVar1 = 0x10;
        while( true ) {
          if ((lVar13 == 0) || (iVar1 < 0)) break;
          lVar13 = FUN_080108a4((int)lVar13,(int)((ulonglong)lVar13 >> 0x20),10,0);
          *(char *)(param_2 + iVar1) = extraout_r2 + '0';
          iVar1 = iVar1 + -1;
        }
        iVar3 = 0x11 - (iVar1 + 1);
        if (param_6 != 1) break;
        if (lVar13 == 0) {
          iVar5 = (iVar3 - param_5) + -1;
          goto LAB_08012088;
        }
        param_5 = 0x11;
        param_6 = 0;
      }
      bVar9 = true;
      if ((lVar13 == 0) && (iVar3 <= (int)param_5)) {
        if (iVar3 < (int)param_5) {
          bVar9 = false;
          iVar5 = iVar5 + -1;
        }
      }
      else {
        bVar9 = false;
        iVar5 = iVar5 + 1;
      }
    } while (!bVar9);
LAB_08012088:
    param_1[2] = iVar3;
    param_1[3] = param_6;
    *param_1 = iVar1 + 1 + param_2;
    param_1[1] = iVar5;
  }
  return;
}


// ======== FUN_080120ac @ 080120ac size=14 ========

undefined4 FUN_080120ac(int param_1)

{
  if (param_1 - 0x30U < 10) {
    return 1;
  }
  return 0;
}


// ======== FUN_080120bc @ 080120bc size=1696 ========

int FUN_080120bc(byte *param_1,uint *param_2,undefined4 param_3,code *param_4)

{
  byte bVar1;
  char cVar2;
  int iVar3;
  uint uVar4;
  int iVar5;
  char extraout_r1;
  int *piVar6;
  uint uVar7;
  char *pcVar8;
  undefined1 uVar9;
  uint *puVar10;
  int extraout_r2;
  undefined4 uVar11;
  uint uVar12;
  uint uVar13;
  int iVar14;
  byte *pbVar15;
  char *pcVar16;
  byte *pbVar17;
  uint *puVar18;
  char **unaff_r10;
  uint uVar19;
  undefined4 *puVar20;
  bool bVar21;
  bool bVar22;
  bool bVar23;
  longlong lVar24;
  char *local_88;
  undefined4 local_84;
  char **local_80;
  char *local_7c;
  char **local_78;
  uint local_74;
  undefined1 auStack_70 [32];
  char *local_50;
  char **local_4c;
  char *pcStack_48;
  int local_44;
  byte local_3b [3];
  char *local_38;
  byte *pbStack_34;
  uint *puStack_30;
  undefined4 local_2c;
  code *pcStack_28;
  
  pcStack_28 = param_4;
  local_2c = param_3;
  puStack_30 = param_2;
  pbStack_34 = param_1;
  iVar14 = 0;
LAB_080124ec:
  uVar4 = (uint)*param_1;
  if (uVar4 == 0) {
    return iVar14;
  }
  if (uVar4 == 0x25) {
    uVar13 = 0;
    pcVar16 = (char *)0x0;
    local_74 = 0;
    while( true ) {
      pbVar15 = param_1 + 1;
      uVar4 = 1 << (*pbVar15 - 0x20 & 0xff);
      if ((uVar4 & DAT_080124b8) == 0) break;
      uVar13 = uVar13 | uVar4;
      param_1 = pbVar15;
    }
    if (*pbVar15 == 0x2a) {
      puVar10 = param_2 + 1;
      local_74 = *param_2;
      if ((int)local_74 < 0) {
        uVar13 = uVar13 | 0x2000;
        local_74 = -local_74;
      }
      uVar13 = uVar13 | 2;
      pbVar15 = param_1 + 2;
    }
    else {
      for (; puVar10 = param_2, *pbVar15 - 0x30 < 10; pbVar15 = pbVar15 + 1) {
        uVar13 = uVar13 | 2;
        local_74 = (uint)*pbVar15 + local_74 * 10 + -0x30;
      }
    }
    param_1 = pbVar15;
    puVar18 = puVar10;
    if (*pbVar15 == 0x2e) {
      param_1 = pbVar15 + 1;
      uVar13 = uVar13 | 4;
      if (*param_1 == 0x2a) {
        puVar18 = puVar10 + 1;
        pcVar16 = (char *)*puVar10;
        param_1 = pbVar15 + 2;
      }
      else {
        for (; *param_1 - 0x30 < 10; param_1 = param_1 + 1) {
          pcVar16 = (char *)((uint)*param_1 + (int)pcVar16 * 10 + -0x30);
        }
      }
    }
    bVar1 = *param_1;
    if (bVar1 == 0x6c) {
      uVar13 = uVar13 | 0x100000;
LAB_08012192:
      if (param_1[1] == bVar1) {
        uVar13 = uVar13 + 0x100000;
        param_1 = param_1 + 1;
      }
    }
    else if (bVar1 < 0x6d) {
      if (bVar1 != 0x4c) {
        if (bVar1 == 0x68) {
          uVar13 = uVar13 | 0x300000;
          goto LAB_08012192;
        }
        if (bVar1 != 0x6a) goto LAB_080121a0;
        uVar13 = uVar13 | 0x200000;
      }
    }
    else if ((bVar1 != 0x74) && (bVar1 != 0x7a)) goto LAB_080121a0;
    param_1 = param_1 + 1;
LAB_080121a0:
    uVar4 = (uint)*param_1;
    bVar23 = SBORROW4(uVar4,0x66);
    bVar21 = (int)(uVar4 - 0x66) < 0;
    bVar22 = uVar4 == 0x66;
LAB_080121a4:
    while (!bVar22) {
      param_2 = puVar18;
      if (bVar21 == bVar23) {
        if (uVar4 == 0x70) {
          uVar13 = uVar13 | 4;
          pcVar16 = &NMI;
          local_88 = (char *)0x10;
        }
        else if (uVar4 < 0x71) {
          if (uVar4 == 0x67) break;
          if (uVar4 == 0x69) goto LAB_080122be;
          if (uVar4 == 0x6e) {
            uVar4 = (uVar13 & 0x7fffff) >> 0x14;
            if (uVar4 == 2) {
              piVar6 = (int *)*puVar18;
              *piVar6 = iVar14;
              piVar6[1] = iVar14 >> 0x1f;
            }
            else if (uVar4 == 3) {
              *(short *)*puVar18 = (short)iVar14;
            }
            else if (uVar4 == 4) {
              *(char *)*puVar18 = (char)iVar14;
            }
            else {
              *(int *)*puVar18 = iVar14;
            }
            param_2 = puVar18 + 1;
            goto LAB_080124ea;
          }
          if (uVar4 != 0x6f) goto LAB_080121f4;
          local_88 = (char *)0x8;
        }
        else {
          if (uVar4 == 0x73) {
            unaff_r10 = (char **)*puVar18;
            iVar5 = -1;
            goto LAB_0801224e;
          }
          if (uVar4 == 0x75) {
            local_88 = (char *)0xa;
          }
          else {
            if (uVar4 != 0x78) goto LAB_080121f4;
            local_88 = (char *)0x10;
          }
        }
LAB_0801235c:
        uVar12 = (uVar13 & 0x7fffff) >> 0x14;
        if (uVar12 == 2) {
          puVar10 = (uint *)((uint)((int)puVar18 + 7) & 0xfffffff8);
          param_2 = puVar10 + 2;
          uVar19 = *puVar10;
          uVar7 = puVar10[1];
        }
        else {
          param_2 = puVar18 + 1;
          uVar19 = *puVar18;
          uVar7 = 0;
          if (uVar12 == 3) {
            uVar19 = uVar19 & 0xffff;
          }
          if (uVar12 == 4) {
            uVar19 = uVar19 & 0xff;
          }
        }
        iVar5 = 0;
        if ((int)(uVar13 << 0x1c) < 0) {
          if (uVar4 == 0x70) {
            local_84 = (char *)CONCAT31(local_84._1_3_,0x40);
            iVar5 = 1;
          }
          else if ((local_88 == (char *)0x10) && (uVar19 != 0 || uVar7 != 0)) {
            local_84._0_2_ = CONCAT11(*param_1,0x30);
            iVar5 = 2;
          }
          if ((local_88 == (char *)0x8) &&
             ((uVar19 != 0 || uVar7 != 0 || ((int)(uVar13 << 0x1d) < 0)))) {
            local_84 = (char *)CONCAT31(local_84._1_3_,0x30);
            iVar5 = 1;
            pcVar16 = pcVar16 + -1;
          }
        }
LAB_080123e6:
        lVar24 = CONCAT44(uVar7,uVar19);
        if (uVar4 == 0x58) {
          local_7c = s_0123456789ABCDEF_080124d0;
        }
        else {
          local_7c = s_0123456789abcdef_080124bc;
        }
        local_80 = &local_50;
        while( true ) {
          if (lVar24 == 0) break;
          lVar24 = FUN_080108a4((int)lVar24,(int)((ulonglong)lVar24 >> 0x20),local_88,0);
          local_80 = (char **)((int)local_80 + -1);
          *(char *)local_80 = local_7c[extraout_r2];
        }
        pcVar8 = (char *)((int)&local_50 - (int)local_80);
        if ((int)(uVar13 << 0x1d) < 0) {
          uVar13 = uVar13 & 0xfffeffff;
        }
        else {
          pcVar16 = (char *)0x1;
        }
        if ((int)pcVar8 < (int)pcVar16) {
          local_88 = pcVar16 + -(int)pcVar8;
        }
        else {
          local_88 = (char *)0x0;
        }
        local_74 = local_74 - (int)(pcVar8 + (int)(local_88 + iVar5));
        if (-1 < (int)(uVar13 << 0xf)) {
          iVar3 = FUN_080127bc(local_74,uVar13,local_2c,param_4);
          iVar14 = iVar14 + iVar3;
        }
        for (iVar3 = 0; iVar3 < iVar5; iVar3 = iVar3 + 1) {
          (*param_4)(*(undefined1 *)((int)&local_84 + iVar3),local_2c);
          iVar14 = iVar14 + 1;
        }
        if ((int)(uVar13 << 0xf) < 0) {
          iVar5 = FUN_080127bc(local_74,uVar13,local_2c,param_4);
          iVar14 = iVar14 + iVar5;
        }
        while (pcVar16 = local_88 + -1, bVar22 = 0 < (int)local_88, local_88 = pcVar16, bVar22) {
          (*param_4)(0x30,local_2c);
          iVar14 = iVar14 + 1;
        }
        while (unaff_r10 = (char **)(pcVar8 + -1), uVar4 = local_74, 0 < (int)pcVar8) {
          cVar2 = *(char *)local_80;
          local_80 = (char **)((int)local_80 + 1);
          (*param_4)(cVar2,local_2c);
          iVar14 = iVar14 + 1;
          pcVar8 = (char *)unaff_r10;
        }
        goto LAB_080124e4;
      }
      if (uVar4 == 0x58) {
        local_88 = (char *)0x10;
        goto LAB_0801235c;
      }
      if (0x58 < uVar4) {
        if (uVar4 == 99) {
          local_88._0_2_ = (ushort)(byte)*puVar18;
          iVar5 = 1;
          unaff_r10 = &local_88;
LAB_0801224e:
          param_2 = puVar18 + 1;
          iVar3 = 0;
          if ((int)(uVar13 << 0x1d) < 0) {
            for (; (iVar3 < (int)pcVar16 &&
                   ((iVar3 < iVar5 || (*(char *)((int)unaff_r10 + iVar3) != '\0'))));
                iVar3 = iVar3 + 1) {
            }
          }
          else {
            for (; (iVar3 < iVar5 || (*(char *)((int)unaff_r10 + iVar3) != '\0')); iVar3 = iVar3 + 1
                ) {
            }
          }
          uVar4 = local_74 - iVar3;
          iVar5 = FUN_080127bc(uVar4,uVar13,local_2c,param_4);
          iVar14 = iVar5 + iVar14 + iVar3;
          while (bVar22 = iVar3 != 0, iVar3 = iVar3 + -1, bVar22) {
            (*param_4)(*(char *)unaff_r10,local_2c);
            unaff_r10 = (char **)((int)unaff_r10 + 1);
          }
          goto LAB_080124e4;
        }
        if (uVar4 == 100) {
LAB_080122be:
          local_88 = (char *)0xa;
          uVar12 = (uVar13 & 0x7fffff) >> 0x14;
          if (uVar12 == 2) {
            puVar10 = (uint *)((uint)((int)puVar18 + 7) & 0xfffffff8);
            param_2 = puVar10 + 2;
            uVar19 = *puVar10;
            uVar7 = puVar10[1];
          }
          else {
            param_2 = puVar18 + 1;
            uVar19 = *puVar18;
            if (uVar12 == 3) {
              uVar19 = (uint)(short)uVar19;
            }
            uVar7 = (int)uVar19 >> 0x1f;
            if (uVar12 == 4) {
              uVar19 = (uint)(char)uVar19;
              uVar7 = (int)uVar19 >> 0x1f;
            }
          }
          if ((int)uVar7 < 0) {
            bVar22 = uVar19 != 0;
            uVar19 = -uVar19;
            uVar7 = -(uint)bVar22 - uVar7;
            uVar9 = 0x2d;
LAB_0801231e:
            local_84 = (char *)CONCAT31(local_84._1_3_,uVar9);
            iVar5 = 1;
          }
          else {
            if ((int)(uVar13 << 0x14) < 0) {
              uVar9 = 0x2b;
              goto LAB_0801231e;
            }
            iVar5 = 0;
            if ((uVar13 & 1) != 0) {
              uVar9 = 0x20;
              goto LAB_0801231e;
            }
          }
          goto LAB_080123e6;
        }
        if (uVar4 == 0x65) break;
        goto LAB_080121f4;
      }
      if (uVar4 == 0) {
        return iVar14;
      }
      bVar23 = SBORROW4(uVar4,0x45);
      bVar21 = (int)(uVar4 - 0x45) < 0;
      bVar22 = uVar4 == 0x45;
      if (!bVar22) goto code_r0x080121b6;
    }
    goto LAB_080124fc;
  }
  goto LAB_080121f4;
code_r0x080121b6:
  bVar23 = SBORROW4(uVar4,0x46);
  bVar21 = (int)(uVar4 - 0x46) < 0;
  bVar22 = uVar4 == 0x46;
  if (!bVar22) goto code_r0x080121ba;
  goto LAB_080121a4;
code_r0x080121ba:
  if (uVar4 != 0x47) {
LAB_080121f4:
    (*param_4)(uVar4,local_2c);
    iVar14 = iVar14 + 1;
    goto LAB_080124ea;
  }
LAB_080124fc:
  if (-1 < (int)(uVar13 << 0x1d)) {
    pcVar16 = (char *)0x6;
  }
  puVar20 = (undefined4 *)((uint)((int)puVar18 + 7) & 0xfffffff8);
  param_2 = puVar20 + 2;
  uVar11 = *puVar20;
  if ((puVar20[1] & 0x80000000) == 0) {
    if ((int)(uVar13 << 0x14) < 0) {
      local_38 = &DAT_08012790;
    }
    else if ((uVar13 & 1) == 0) {
      local_38 = s_0123456789abcdef_080124bc + 0x10;
    }
    else {
      local_38 = &DAT_08012794;
    }
  }
  else {
    local_38 = &DAT_0801278c;
  }
  uVar12 = puVar20[1] & 0x7fffffff;
  if (uVar4 == 0x65) {
LAB_08012566:
    if ((int)pcVar16 < 0x11) {
      local_88 = pcVar16 + 1;
    }
    else {
      local_88 = (char *)0x11;
    }
    local_84 = (char *)0x0;
    FUN_08011f28(&local_50,auStack_70,uVar11,uVar12);
    local_7c = pcStack_48;
    local_88 = local_50;
    unaff_r10 = (char **)(pcVar16 + 1);
    local_78 = (char **)0x0;
LAB_0801262e:
    local_78 = (char **)0x0;
    local_84 = (char *)0x1;
    local_80 = local_4c;
    local_88 = local_50;
    local_7c = pcStack_48;
  }
  else if (uVar4 < 0x66) {
    if (uVar4 == 0x45) goto LAB_08012566;
    if (uVar4 == 0x46) goto LAB_08012592;
    if (uVar4 == 0x47) goto LAB_080125d8;
  }
  else if (uVar4 == 0x66) {
LAB_08012592:
    local_84 = (char *)0x1;
    local_80 = (char **)0x80000000;
    local_88 = pcVar16;
    FUN_08011f28(&local_50,auStack_70,uVar11,uVar12);
    local_7c = pcStack_48;
    local_88 = local_50;
    local_78 = (char **)0x0;
    unaff_r10 = (char **)pcStack_48;
    if (local_44 == 0) {
      unaff_r10 = (char **)((int)local_4c + (int)(pcVar16 + 1));
    }
    iVar5 = (int)pcVar16 - (int)unaff_r10;
    if (-1 < iVar5) {
      unaff_r10 = (char **)(pcVar16 + 1);
      local_78 = (char **)(-1 - iVar5);
    }
    local_84 = (char *)((int)unaff_r10 + -(int)pcVar16);
  }
  else {
    if (uVar4 != 0x67) goto LAB_08012662;
LAB_080125d8:
    if ((int)pcVar16 < 1) {
      pcVar16 = (char *)0x1;
    }
    local_88 = pcVar16;
    if (0x11 < (int)pcVar16) {
      local_88 = (char *)0x11;
    }
    local_84 = (char *)0x0;
    FUN_08011f28(&local_50,auStack_70,uVar11,uVar12);
    local_7c = pcStack_48;
    local_78 = (char **)0x0;
    local_88 = local_50;
    unaff_r10 = (char **)pcVar16;
    if (-1 < (int)(uVar13 << 0x1c)) {
      pcVar8 = pcStack_48;
      if ((int)pcVar16 <= (int)pcStack_48) goto LAB_08012612;
      while( true ) {
        unaff_r10 = (char **)pcVar8;
LAB_08012612:
        if (((int)unaff_r10 < 2) || (*(char *)((int)unaff_r10 + (int)(local_50 + -1)) != '0'))
        break;
        pcVar8 = (char *)((int)unaff_r10 + -1);
      }
    }
    if (((int)pcVar16 <= (int)local_4c) || ((int)local_4c < -4)) goto LAB_0801262e;
    if ((int)local_4c < 1) {
      local_78 = local_4c;
      pcVar16 = (char *)((int)unaff_r10 + -(int)local_4c);
LAB_08012652:
      unaff_r10 = (char **)pcVar16;
    }
    else {
      pcVar16 = (char *)((int)local_4c + 1);
      if ((int)unaff_r10 < (int)pcVar16) goto LAB_08012652;
    }
    local_84 = (char *)((int)local_4c + (1 - (int)local_78));
    local_80 = (char **)0x80000000;
  }
LAB_08012662:
  if ((-1 < (int)(uVar13 << 0x1c)) && ((int)unaff_r10 <= (int)local_84)) {
    local_84 = (char *)0xffffffff;
  }
  local_3b[2] = 0;
  pbVar15 = local_3b + 2;
  if (local_80 != (char **)0x80000000) {
    local_50 = (char *)0x2b;
    iVar5 = 2;
    if ((int)local_80 < 0) {
      local_80 = (char **)-(int)local_80;
      local_50 = (char *)0x2d;
    }
    while ((0 < iVar5 || (local_80 != (char **)0x0))) {
      local_80 = (char **)FUN_08010f2c(local_80,10);
      pbVar15 = pbVar15 + -1;
      *pbVar15 = extraout_r1 + 0x30;
      iVar5 = iVar5 + -1;
    }
    pbVar15[-1] = (byte)local_50;
    pbVar15 = pbVar15 + -2;
    *pbVar15 = *param_1 & 0x20 | 0x45;
  }
  pbVar17 = local_3b + (2 - (int)pbVar15);
  local_74 = (local_74 -
             (int)((int)unaff_r10 +
                  (int)(pbVar17 + ((int)local_84 >> 0x1f) + (uint)(*local_38 != '\0')))) - 1;
  if (-1 < (int)(uVar13 << 0xf)) {
    iVar5 = FUN_080127bc(local_74,uVar13,local_2c,param_4);
    iVar14 = iVar14 + iVar5;
  }
  if (*local_38 != '\0') {
    (*param_4)(*local_38,local_2c);
    iVar14 = iVar14 + 1;
  }
  iVar5 = iVar14;
  pcVar16 = (char *)unaff_r10;
  if ((int)(uVar13 << 0xf) < 0) {
    iVar5 = FUN_080127bc(local_74,uVar13,local_2c,param_4);
    iVar5 = iVar14 + iVar5;
  }
  while (iVar14 = iVar5, unaff_r10 = (char **)(pcVar16 + -1), 0 < (int)pcVar16) {
    if (((int)local_78 < 0) || ((int)local_7c <= (int)local_78)) {
      cVar2 = '0';
    }
    else {
      cVar2 = local_88[(int)local_78];
    }
    (*param_4)(cVar2,local_2c);
    local_78 = (char **)((int)local_78 + 1);
    local_84 = local_84 + -1;
    iVar5 = iVar14 + 1;
    pcVar16 = (char *)unaff_r10;
    if (local_84 == (char *)0x0) {
      (*param_4)(0x2e,local_2c);
      iVar5 = iVar14 + 2;
    }
  }
  while (uVar4 = local_74, 0 < (int)pbVar17) {
    (*param_4)(*pbVar15,local_2c);
    iVar14 = iVar14 + 1;
    pbVar15 = pbVar15 + 1;
    pbVar17 = pbVar17 + -1;
  }
LAB_080124e4:
  iVar5 = FUN_08012798(uVar4,uVar13,local_2c,param_4);
  iVar14 = iVar14 + iVar5;
LAB_080124ea:
  param_1 = param_1 + 1;
  goto LAB_080124ec;
}


// ======== FUN_08012798 @ 08012798 size=36 ========

int FUN_08012798(int param_1,int param_2,undefined4 param_3,code *param_4)

{
  int iVar1;
  
  iVar1 = 0;
  if (param_2 << 0x12 < 0) {
    while (param_1 = param_1 + -1, -1 < param_1) {
      (*param_4)(0x20,param_3);
      iVar1 = iVar1 + 1;
    }
  }
  return iVar1;
}


// ======== FUN_080127bc @ 080127bc size=46 ========

int FUN_080127bc(int param_1,int param_2,undefined4 param_3,code *param_4)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = 0;
  if (param_2 << 0xf < 0) {
    uVar2 = 0x30;
  }
  else {
    uVar2 = 0x20;
  }
  if (-1 < param_2 << 0x12) {
    while (param_1 = param_1 + -1, -1 < param_1) {
      (*param_4)(uVar2,param_3);
      iVar1 = iVar1 + 1;
    }
  }
  return iVar1;
}


// ======== FUN_08012800 @ 08012800 size=10 ========

void FUN_08012800(undefined1 param_1,undefined4 *param_2)

{
  undefined1 *puVar1;
  
  puVar1 = (undefined1 *)*param_2;
  *param_2 = puVar1 + 1;
  *puVar1 = param_1;
  return;
}


// ======== FUN_0801280a @ 0801280a size=30 ========

void FUN_0801280a(int param_1)

{
  *(uint *)(param_1 + 8) = *(uint *)(param_1 + 8) | 8;
  do {
  } while (*(int *)(param_1 + 8) << 0x1c < 0);
  *(uint *)(param_1 + 8) = *(uint *)(param_1 + 8) | 4;
  do {
  } while (*(int *)(param_1 + 8) << 0x1d < 0);
  return;
}


// ======== FUN_08012828 @ 08012828 size=54 ========

void FUN_08012828(int param_1,int param_2,int param_3)

{
  uint uVar1;
  
  uVar1 = param_3 * 0x100000 - 0x100000;
  if (param_2 != 1) {
    if (param_2 == 2) {
      *(uint *)(param_1 + 0x38) = *(uint *)(param_1 + 0x38) & 0xffcfffff;
      *(uint *)(param_1 + 0x38) = uVar1 & 0x300000 | *(uint *)(param_1 + 0x38);
    }
    return;
  }
  *(uint *)(param_1 + 0x2c) = *(uint *)(param_1 + 0x2c) & 0xff0fffff;
  *(uint *)(param_1 + 0x2c) = uVar1 & 0xf00000 | *(uint *)(param_1 + 0x2c);
  return;
}


// ======== FUN_0801285e @ 0801285e size=20 ========

void FUN_0801285e(int param_1,int param_2)

{
  uint uVar1;
  
  if (param_2 == 0) {
    uVar1 = *(uint *)(param_1 + 8) & 0xfffff7ff;
  }
  else {
    uVar1 = *(uint *)(param_1 + 8) | 0x800;
  }
  *(uint *)(param_1 + 8) = uVar1;
  return;
}


// ======== FUN_08012874 @ 08012874 size=76 ========

void FUN_08012874(int param_1)

{
  uint *puVar1;
  uint uVar2;
  
  param_1 = param_1 + DAT_080128ac;
  if (param_1 == 0) {
    uVar2 = 0x309;
  }
  else if (param_1 == 0x400) {
    uVar2 = 0x30a;
  }
  else {
    if (param_1 != 0x1800) {
      return;
    }
    uVar2 = 0x30f;
  }
  FUN_08019180(uVar2);
  puVar1 = (uint *)(DAT_0801917c + (uVar2 >> 6));
  *puVar1 = *puVar1 & ~(1 << (uVar2 & 0x1f));
  return;
}


// ======== FUN_080128ba @ 080128ba size=16 ========

void FUN_080128ba(int param_1)

{
  if ((*(uint *)(param_1 + 8) & 1) == 0) {
    *(uint *)(param_1 + 8) = *(uint *)(param_1 + 8) | 1;
  }
  return;
}


// ======== FUN_080128ca @ 080128ca size=54 ========

void FUN_080128ca(int param_1,int param_2,int param_3)

{
  uint uVar1;
  
  if (param_3 == 0) {
    if (param_2 << 0x1f != 0) {
      *(uint *)(param_1 + 8) = *(uint *)(param_1 + 8) & 0xffefffff;
    }
    if (-1 < param_2 << 0x1e) {
      return;
    }
    uVar1 = *(uint *)(param_1 + 8) & 0xffff7fff;
  }
  else {
    if (param_2 << 0x1f != 0) {
      *(uint *)(param_1 + 8) = *(uint *)(param_1 + 8) | 0x100000;
    }
    if (-1 < param_2 << 0x1e) {
      return;
    }
    uVar1 = *(uint *)(param_1 + 8) | 0x8000;
  }
  *(uint *)(param_1 + 8) = uVar1;
  return;
}


// ======== FUN_08012900 @ 08012900 size=32 ========

void FUN_08012900(int param_1,int param_2,uint param_3)

{
  uint uVar1;
  
  if (param_2 == 1) {
    uVar1 = *(uint *)(param_1 + 8) & 0xfff1ffff;
  }
  else {
    if (param_2 != 2) {
      return;
    }
    uVar1 = *(uint *)(param_1 + 8) & 0xffff8fff;
  }
  *(uint *)(param_1 + 8) = uVar1;
  *(uint *)(param_1 + 8) = *(uint *)(param_1 + 8) | param_3;
  return;
}


// ======== FUN_08012920 @ 08012920 size=14 ========

bool FUN_08012920(uint *param_1,uint param_2)

{
  return (*param_1 & param_2) != 0;
}


// ======== FUN_0801292e @ 0801292e size=80 ========

void FUN_0801292e(int param_1,int param_2,uint param_3,int param_4)

{
  uint uVar1;
  
  uVar1 = (param_2 - ((*(uint *)(param_1 + 0x38) & 0x3fffff) >> 0x14)) * 5 + 0xf;
  *(uint *)(param_1 + 0x38) =
       param_3 << (uVar1 & 0xff) | *(uint *)(param_1 + 0x38) & ~(0x1f << (uVar1 & 0xff));
  if (param_3 < 10) {
    *(uint *)(param_1 + 0x10) =
         param_4 << (param_3 * 3 & 0xff) | *(uint *)(param_1 + 0x10) & ~(7 << (param_3 * 3 & 0xff));
  }
  else if (param_3 < 0x12) {
    uVar1 = (param_3 - 10) * 3;
    *(uint *)(param_1 + 0xc) =
         param_4 << (uVar1 & 0xff) | *(uint *)(param_1 + 0xc) & ~(7 << (uVar1 & 0xff));
    return;
  }
  return;
}


// ======== FUN_0801297e @ 0801297e size=38 ========

void FUN_0801297e(int param_1,uint param_2)

{
  if ((param_2 & 1) != 0) {
    *(uint *)(param_1 + 4) = *(uint *)(param_1 + 4) & 0xffffffbf;
  }
  if ((int)(param_2 << 0x1e) < 0) {
    *(uint *)(param_1 + 4) = *(uint *)(param_1 + 4) & 0xffffffdf;
  }
  if ((int)(param_2 << 0x1d) < 0) {
    *(uint *)(param_1 + 4) = *(uint *)(param_1 + 4) & 0xffffff7f;
  }
  return;
}


// ======== FUN_080129a4 @ 080129a4 size=8 ========

void FUN_080129a4(uint *param_1,uint param_2)

{
  *param_1 = *param_1 & ~param_2;
  return;
}


// ======== FUN_080129ac @ 080129ac size=62 ========

undefined4 FUN_080129ac(uint *param_1,int param_2)

{
  int iVar1;
  uint uVar2;
  undefined4 uVar3;
  
  uVar3 = 0;
  if (param_2 == 1) {
    uVar2 = *param_1 & 1;
    iVar1 = param_1[1] << 0x19;
  }
  else if (param_2 == 2) {
    uVar2 = *param_1 & 2;
    iVar1 = param_1[1] << 0x1a;
  }
  else {
    if (param_2 != 4) {
      return 0;
    }
    uVar2 = *param_1 & 4;
    iVar1 = param_1[1] << 0x18;
  }
  if ((iVar1 < 0) && (uVar2 != 0)) {
    uVar3 = 1;
  }
  return uVar3;
}


// ======== FUN_080129ec @ 080129ec size=18 ========

void FUN_080129ec(uint param_1)

{
  uint *puVar1;
  
  puVar1 = DAT_08012a00;
  *DAT_08012a00 = *DAT_08012a00 & 0xfff0ffff;
  *puVar1 = *puVar1 | param_1;
  return;
}


// ======== FUN_08012a04 @ 08012a04 size=126 ========

void FUN_08012a04(int param_1,uint param_2,uint param_3,int param_4)

{
  uint uVar1;
  
  if (param_2 < 6) {
    *(uint *)(param_1 + 0x34) =
         param_3 << (param_2 * 5 & 0xff) |
         *(uint *)(param_1 + 0x34) & ~(0x1f << (param_2 * 5 & 0xff));
  }
  else if (param_2 < 0xc) {
    uVar1 = (param_2 - 6) * 5;
    *(uint *)(param_1 + 0x30) =
         param_3 << (uVar1 & 0xff) | *(uint *)(param_1 + 0x30) & ~(0x1f << (uVar1 & 0xff));
  }
  else if (param_2 < 0x10) {
    uVar1 = (param_2 - 0xc) * 5;
    *(uint *)(param_1 + 0x2c) =
         param_3 << (uVar1 & 0xff) | *(uint *)(param_1 + 0x2c) & ~(0x1f << (uVar1 & 0xff));
  }
  if (param_3 < 10) {
    *(uint *)(param_1 + 0x10) =
         param_4 << (param_3 * 3 & 0xff) | *(uint *)(param_1 + 0x10) & ~(7 << (param_3 * 3 & 0xff));
  }
  else if (param_3 < 0x12) {
    uVar1 = (param_3 - 10) * 3;
    *(uint *)(param_1 + 0xc) =
         param_4 << (uVar1 & 0xff) | *(uint *)(param_1 + 0xc) & ~(7 << (uVar1 & 0xff));
    return;
  }
  return;
}


// ======== FUN_08012a82 @ 08012a82 size=6 ========

uint FUN_08012a82(int param_1)

{
  return *(uint *)(param_1 + 0x4c) & 0xffff;
}


// ======== FUN_08012a88 @ 08012a88 size=26 ========

void FUN_08012a88(int param_1,uint param_2)

{
  if ((param_2 & 1) != 0) {
    *(uint *)(param_1 + 8) = *(uint *)(param_1 + 8) | 0x400000;
  }
  if ((int)(param_2 << 0x1e) < 0) {
    *(uint *)(param_1 + 8) = *(uint *)(param_1 + 8) | 0x200000;
  }
  return;
}


// ======== FUN_08012aa2 @ 08012aa2 size=82 ========

void FUN_08012aa2(int param_1,int param_2,int param_3)

{
  uint uVar1;
  
  if (param_3 == 0) {
    if (param_2 * 0x800000 < 0) {
      *(uint *)(param_1 + 4) = *(uint *)(param_1 + 4) & 0xfffffeff;
    }
    if (param_2 << 0x15 < 0) {
      *(uint *)(param_1 + 4) = *(uint *)(param_1 + 4) & 0xfffffbff;
    }
    if (-1 < param_2 << 0x1e) {
      return;
    }
    uVar1 = *(uint *)(param_1 + 8) & 0xfffffffd;
  }
  else {
    if (param_2 * 0x800000 < 0) {
      *(uint *)(param_1 + 4) = *(uint *)(param_1 + 4) | 0x100;
    }
    if (param_2 << 0x15 < 0) {
      *(uint *)(param_1 + 4) = *(uint *)(param_1 + 4) | 0x400;
    }
    if (-1 < param_2 << 0x1e) {
      return;
    }
    uVar1 = *(uint *)(param_1 + 8) | 2;
  }
  *(uint *)(param_1 + 8) = uVar1;
  return;
}


// ======== FUN_08012b16 @ 08012b16 size=120 ========

undefined4 FUN_08012b16(int param_1,int param_2,int param_3,int param_4,int param_5)

{
  int *piVar1;
  int *piVar2;
  uint uVar3;
  
  if (((param_1 == 0) || (param_2 == 0)) || (param_3 == 0)) {
    return 0;
  }
  if (param_5 == 0) {
    param_2 = FUN_08012fe0(param_2,param_4);
    if (param_2 == 0) {
      return 0;
    }
    uVar3 = *(uint *)(param_3 + 0xc) & 0xfffffdff;
  }
  else {
    uVar3 = *(uint *)(param_3 + 0xc) | 0x200;
  }
  if ((-1 < (int)((uint)*(ushort *)(param_3 + 0xc) << 0x16)) && (*(int *)(param_3 + 0x20) != 0)) {
    (**(code **)(param_4 + 4))();
  }
  *(uint *)(param_3 + 0xc) = uVar3;
  *(int *)(param_3 + 0x20) = param_2;
  if ((param_3 != 0) && (param_1 != 0)) {
    piVar1 = *(int **)(param_1 + 8);
    if (*(int **)(param_1 + 8) == (int *)0x0) {
      *(int *)(param_1 + 8) = param_3;
    }
    else {
      do {
        piVar2 = piVar1;
        piVar1 = (int *)*piVar2;
      } while ((int *)*piVar2 != (int *)0x0);
      *piVar2 = param_3;
      *(int **)(param_3 + 4) = piVar2;
    }
    return 1;
  }
  return 0;
}


// ======== FUN_08012b6c @ 08012b6c size=276 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_08012b6c(void)

{
  undefined4 uVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  undefined4 local_28;
  char *local_24;
  uint local_20;
  undefined1 local_1c;
  undefined2 local_18 [2];
  undefined2 local_14;
  
  FUN_08019148(0x602);
  FUN_08019148(0x603);
  FUN_08019148(0x609);
  FUN_08019148(0x60a);
  FUN_08019148(0x500);
  FUN_08019148(0x700);
  FUN_08018e7c(1);
  uVar1 = DAT_08012c80;
  FUN_08016ef0(DAT_08012c80,0,4,0x20);
  FUN_08016ef0(DAT_08012c84,0,4,2);
  FUN_08016ef0(uVar1,0,4,0x80);
  uVar3 = 0;
  local_20 = 199;
  local_28 = 0xb;
  local_24 = (char *)0x0;
  local_1c = 0;
  FUN_0801af7c(0x40000000,&local_28);
  local_18[0] = 1;
  local_14 = 0;
  FUN_0801ac58(0x40000000,1,local_18);
  FUN_0801ae7a(0x40000000,1,100);
  FUN_0801ae2c(0x40000000,1,0x70);
  FUN_0801ae98(0x40000000,1,0);
  FUN_08019d34();
  FUN_08019bf4();
  FUN_0801b65c(100);
  iVar4 = 0;
  do {
    iVar2 = thunk_FUN_0801a1f8();
    if (iVar2 != 1) {
      return 0xffffffff;
    }
    iVar2 = FUN_08016a90();
    uVar3 = iVar2 + uVar3 & 0xffff;
    FUN_0801b65c(0x32);
    uVar1 = DAT_08012c9c;
    iVar4 = iVar4 + 1;
  } while (iVar4 < 5);
  local_20 = uVar3 / 5;
  *_DAT_08012c88 = (short)local_20;
  local_28 = 0x5b;
  local_24 = s_base_voltage__d_08012c8b + 1;
  FUN_080141e8(3,s_hr_bd_08012ce0,s____src_platform_driver_driver_bo_08012ca0,uVar1);
  return 0;
}


// ======== FUN_08012ce8 @ 08012ce8 size=14 ========

void FUN_08012ce8(void)

{
  FUN_08018ef0();
  FUN_08018ee0();
  return;
}


// ======== FUN_08012cf8 @ 08012cf8 size=38 ========

undefined2 FUN_08012cf8(int param_1)

{
  undefined2 uVar1;
  int iVar2;
  
  uVar1 = 0;
  iVar2 = DAT_08012d20 + param_1 * 4;
  if (param_1 - 0xbU < 0x20) {
    uVar1 = *(undefined2 *)(iVar2 + 0xc14);
  }
  else if (param_1 - 1U < 10) {
    uVar1 = *(undefined2 *)(iVar2 + 0xc00);
  }
  return uVar1;
}


// ======== FUN_08012d24 @ 08012d24 size=34 ========

void FUN_08012d24(int param_1,undefined2 param_2)

{
  int iVar1;
  
  iVar1 = DAT_08012d48 + param_1 * 4;
  if (param_1 - 0xbU < 0x20) {
    *(undefined2 *)(iVar1 + 0xc14) = param_2;
  }
  else if (param_1 - 1U < 10) {
    *(undefined2 *)(iVar1 + 0xc00) = param_2;
    return;
  }
  return;
}


// ======== FUN_08012d4c @ 08012d4c size=44 ========

int * FUN_08012d4c(int *param_1)

{
  uint uVar1;
  
  if ((param_1 == (int *)0x0) || (*param_1 == 0)) {
    return (int *)0x0;
  }
  do {
    uVar1 = param_1[2];
    if (((uint)param_1[1] <= uVar1) || (0x20 < *(byte *)(*param_1 + uVar1))) break;
    uVar1 = uVar1 + 1;
    param_1[2] = uVar1;
  } while (param_1 != (int *)0x0);
  if (uVar1 == param_1[1]) {
    param_1[2] = uVar1 - 1;
  }
  return param_1;
}


// ======== FUN_08012d78 @ 08012d78 size=42 ========

undefined4 FUN_08012d78(undefined4 param_1,undefined4 param_2)

{
  undefined4 uVar1;
  int iVar2;
  
  uVar1 = FUN_08012da8();
  iVar2 = FUN_08012b16(param_1,param_2,uVar1,DAT_08012da4,0);
  if (iVar2 == 0) {
    FUN_08012e38(uVar1);
    uVar1 = 0;
  }
  return uVar1;
}


// ======== FUN_08012da8 @ 08012da8 size=98 ========

int FUN_08012da8(void)

{
  int iVar1;
  undefined4 uVar2;
  undefined1 in_CY;
  bool bVar3;
  undefined8 in_d0;
  
  iVar1 = FUN_08012ed0(DAT_08012e0c);
  if (iVar1 != 0) {
    bVar3 = false;
    *(undefined4 *)(iVar1 + 0xc) = 8;
    *(undefined8 *)(iVar1 + 0x18) = in_d0;
    FUN_08010efc();
    if ((bool)in_CY && !bVar3) {
      FUN_08010ecc();
      if ((bool)in_CY && !bVar3) {
        uVar2 = FUN_08010e8e();
      }
      else {
        uVar2 = 0x80000000;
      }
    }
    else {
      uVar2 = 0x7fffffff;
    }
    *(undefined4 *)(iVar1 + 0x14) = uVar2;
  }
  return iVar1;
}


// ======== FUN_08012e20 @ 08012e20 size=18 ========

void FUN_08012e20(void)

{
  int iVar1;
  
  iVar1 = FUN_08012ed0(DAT_08012e34);
  if (iVar1 != 0) {
    *(undefined4 *)(iVar1 + 0xc) = 0x40;
  }
  return;
}


// ======== FUN_08012e38 @ 08012e38 size=66 ========

void FUN_08012e38(int *param_1)

{
  int iVar1;
  int iVar2;
  
  iVar1 = DAT_08012e7c;
  while (param_1 != (int *)0x0) {
    iVar2 = *param_1;
    if ((-1 < (int)((uint)*(ushort *)(param_1 + 3) << 0x17)) &&
       (((param_1[2] == 0 || (FUN_08012e38(), -1 < (int)((uint)*(ushort *)(param_1 + 3) << 0x17)))
        && (param_1[4] != 0)))) {
      (**(code **)(iVar1 + 4))();
    }
    if ((-1 < (int)((uint)*(ushort *)(param_1 + 3) << 0x16)) && (param_1[8] != 0)) {
      (**(code **)(iVar1 + 4))();
    }
    (**(code **)(iVar1 + 4))(param_1);
    param_1 = (int *)iVar2;
  }
  return;
}


// ======== FUN_08012e80 @ 08012e80 size=128 ========

/* WARNING: Removing unreachable block (ram,0x08016cba) */
/* WARNING: Removing unreachable block (ram,0x08016cbc) */
/* WARNING: Removing unreachable block (ram,0x08016cc0) */
/* WARNING: Removing unreachable block (ram,0x08016cca) */
/* WARNING: Removing unreachable block (ram,0x08016cb8) */

undefined4 * FUN_08012e80(int param_1,char *param_2)

{
  int iVar1;
  int iVar2;
  undefined4 *puVar3;
  char *pcVar4;
  char *pcVar5;
  
  if ((param_1 != 0) && (param_2 != (char *)0x0)) {
    for (puVar3 = *(undefined4 **)(param_1 + 8); puVar3 != (undefined4 *)0x0;
        puVar3 = (undefined4 *)*puVar3) {
      pcVar5 = (char *)puVar3[8];
      if ((param_2 != (char *)0x0) && (pcVar5 != (char *)0x0)) {
        pcVar4 = param_2;
        if (param_2 == pcVar5) {
          return puVar3;
        }
        while( true ) {
          iVar1 = FUN_08010906(*pcVar4);
          iVar2 = FUN_08010906(*pcVar5);
          if (iVar1 != iVar2) break;
          if (*pcVar4 == '\0') goto LAB_08016d0c;
          pcVar5 = pcVar5 + 1;
          pcVar4 = pcVar4 + 1;
        }
        iVar1 = FUN_08010906();
        iVar2 = FUN_08010906(*pcVar5);
        if (iVar1 == iVar2) {
LAB_08016d0c:
          if (puVar3 == (undefined4 *)0x0) {
            return (undefined4 *)0x0;
          }
          if (puVar3[8] == 0) {
            return (undefined4 *)0x0;
          }
          return puVar3;
        }
      }
    }
  }
  return (undefined4 *)0x0;
}


// ======== FUN_08012e88 @ 08012e88 size=54 ========

void FUN_08012e88(int *param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  
  piVar4 = DAT_08012ecc;
  iVar3 = DAT_08012ec8;
  iVar2 = DAT_08012ec4;
  iVar1 = DAT_08012ec0;
  if (param_1 != (int *)0x0) {
    *DAT_08012ecc = DAT_08012ec4;
    if (*param_1 != 0) {
      *piVar4 = *param_1;
    }
    piVar4[1] = iVar3;
    if (param_1[1] != 0) {
      piVar4[1] = param_1[1];
    }
    piVar4[2] = 0;
    if ((*piVar4 == iVar2) && (piVar4[1] == iVar3)) {
      piVar4[2] = iVar1;
    }
    return;
  }
  *DAT_08012ecc = DAT_08012ec4;
  piVar4[1] = iVar3;
  piVar4[2] = iVar1;
  return;
}


// ======== FUN_08012ed0 @ 08012ed0 size=22 ========

int FUN_08012ed0(undefined4 *param_1)

{
  int iVar1;
  
  iVar1 = (*(code *)*param_1)(0x28);
  if (iVar1 != 0) {
    FUN_08010948(iVar1,0x28);
  }
  return iVar1;
}


// ======== FUN_08012ee6 @ 08012ee6 size=226 ========

/* WARNING: Removing unreachable block (ram,0x08012fa6) */
/* WARNING: Removing unreachable block (ram,0x08012f7a) */
/* WARNING: Removing unreachable block (ram,0x08012f88) */
/* WARNING: Removing unreachable block (ram,0x08012fbe) */

int FUN_08012ee6(int param_1)

{
  int *piVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  int *piVar5;
  int local_40;
  uint local_3c;
  uint local_38;
  int local_30;
  int local_2c;
  int iStack_28;
  
  FUN_08010948(&local_40,0x1c);
  piVar1 = DAT_08012fcc;
  *DAT_08012fcc = 0;
  piVar1[1] = 0;
  if (param_1 != 0) {
    local_40 = param_1;
    iVar2 = FUN_08010982(param_1);
    piVar1 = DAT_08012fcc;
    local_3c = iVar2 + 1;
    local_38 = 0;
    local_30 = DAT_08012fcc[2];
    local_2c = DAT_08012fcc[3];
    iStack_28 = DAT_08012fcc[4];
    iVar2 = FUN_08012ed0(DAT_08012fcc + 2);
    if (iVar2 != 0) {
      if ((local_40 == 0) || (local_38 != 0)) {
        piVar5 = (int *)(undefined1 *)0x0;
      }
      else {
        piVar5 = &local_40;
        if ((4 < local_3c) &&
           (iVar4 = FUN_080109be(local_40,&DAT_08012fd0,3), piVar5 = &local_40, iVar4 == 0)) {
          local_38 = local_38 + 3;
          piVar5 = &local_40;
        }
      }
      uVar3 = FUN_08012d4c(piVar5);
      iVar4 = FUN_080180a0(iVar2,uVar3);
      if (iVar4 != 0) {
        return iVar2;
      }
      FUN_08012e38(iVar2);
    }
    if ((local_3c <= local_38) && (local_38 = 0, local_3c != 0)) {
      local_38 = local_3c - 1;
    }
    *piVar1 = param_1;
    piVar1[1] = local_38;
  }
  return 0;
}


// ======== FUN_08012fd4 @ 08012fd4 size=160 ========

int FUN_08012fd4(undefined4 param_1)

{
  undefined4 *puVar1;
  int iVar2;
  uint uVar3;
  int local_38;
  uint local_34;
  int local_30;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 uStack_1c;
  undefined4 uStack_18;
  
  puVar1 = DAT_08012fdc;
  FUN_08010948(&local_38,0x24);
  local_38 = (*(code *)*puVar1)(0x100);
  local_24 = 0;
  local_34 = 0x100;
  local_20 = *puVar1;
  uStack_1c = puVar1[1];
  uStack_18 = puVar1[2];
  if (local_38 != 0) {
    iVar2 = FUN_08018758(param_1,&local_38);
    if (iVar2 != 0) {
      FUN_0801b028(&local_38);
      if ((code *)puVar1[2] == (code *)0x0) {
        iVar2 = (*(code *)*puVar1)(local_30 + 1);
        if (iVar2 != 0) {
          uVar3 = local_34;
          if (local_30 + 1U <= local_34) {
            uVar3 = local_30 + 1;
          }
          FUN_08010912(iVar2,local_38,uVar3);
          *(undefined1 *)(iVar2 + local_30) = 0;
          (*(code *)puVar1[1])(local_38);
          return iVar2;
        }
      }
      else {
        iVar2 = (*(code *)puVar1[2])(local_38,local_30 + 1);
        if (iVar2 != 0) {
          return iVar2;
        }
      }
    }
    if (local_38 != 0) {
      (*(code *)puVar1[1])();
    }
  }
  return 0;
}


// ======== FUN_08012fe0 @ 08012fe0 size=40 ========

int FUN_08012fe0(int param_1,undefined4 *param_2)

{
  int iVar1;
  int iVar2;
  
  if (param_1 != 0) {
    iVar1 = FUN_08010982();
    iVar2 = (*(code *)*param_2)();
    if (iVar2 != 0) {
      FUN_08010912(iVar2,param_1,iVar1 + 1);
      return iVar2;
    }
  }
  return 0;
}


// ======== FUN_08013008 @ 08013008 size=44 ========

void FUN_08013008(byte *param_1,int param_2)

{
  uint uVar1;
  byte *pbVar2;
  byte bVar3;
  bool bVar4;
  
  uVar1 = 0;
  while (bVar4 = param_2 != 0, param_2 = param_2 + -1, bVar4) {
    pbVar2 = param_1 + 1;
    uVar1 = uVar1 ^ *param_1;
    bVar3 = 0;
    do {
      if ((uVar1 & 1) == 0) {
        uVar1 = uVar1 >> 1;
      }
      else {
        uVar1 = uVar1 >> 1 ^ 0x8c;
      }
      bVar3 = bVar3 + 1;
      param_1 = pbVar2;
    } while (bVar3 < 8);
  }
  return;
}


// ======== FUN_08013036 @ 08013036 size=202 ========

void FUN_08013036(uint param_1,int param_2,int *param_3)

{
  undefined4 uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  int *piVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  
  if (8 < param_1) {
    return;
  }
  if (param_2 != 0) {
    if (param_3 == (int *)0x0) {
      return;
    }
    if (param_1 != 0) {
      if ((param_1 == 7) || (param_1 == 8)) {
        if (*(int *)(*param_3 + 4) == 0) {
          return;
        }
        iVar6 = **(int **)(*param_3 + 0xc);
        uVar1 = *(undefined4 *)(iVar6 + 0xc);
        *(undefined4 *)(param_2 + 8) = *(undefined4 *)(iVar6 + 8);
        *(undefined4 *)(param_2 + 0xc) = uVar1;
      }
      else if (param_1 != 1) {
        if (((param_1 != 3) && (param_1 != 4)) && (param_1 != 2)) {
          return;
        }
        if (2 < *(byte *)((int)param_3 + 0x1d)) {
          *(undefined1 *)((int)param_3 + 9) = 2;
          param_3[3] = 0;
          return;
        }
        *(byte *)((int)param_3 + 0x1d) = *(byte *)((int)param_3 + 0x1d) + 1;
        return;
      }
    }
    FUN_0801499e(*param_3);
    uVar2 = 0;
    iVar6 = 0;
    uVar3 = 0;
    iVar8 = 0;
    piVar5 = *(int **)(*param_3 + 0x10);
    for (uVar4 = 0; uVar4 < *(uint *)(*param_3 + 4); uVar4 = (uint)(char)((char)uVar4 + '\x01')) {
      iVar7 = *piVar5;
      if (*(char *)(iVar7 + 1) == '\0') {
        uVar2 = uVar2 + 1 & 0xff;
        iVar6 = iVar6 + *(int *)(iVar7 + 4);
      }
      else if (*(char *)(iVar7 + 1) == '\x01') {
        uVar3 = uVar3 + 1 & 0xff;
        iVar8 = iVar8 + *(int *)(iVar7 + 4);
      }
      piVar5 = (int *)piVar5[2];
    }
    if (uVar2 < uVar3) {
      *(undefined1 *)((int)param_3 + 9) = 1;
      iVar6 = iVar8 / (int)uVar3;
    }
    else {
      iVar6 = iVar6 / (int)uVar2;
      *(undefined1 *)((int)param_3 + 9) = 0;
    }
    param_3[3] = iVar6;
    if (*(char *)((int)param_3 + 0x1d) != '\0') {
      *(char *)((int)param_3 + 0x1d) = *(char *)((int)param_3 + 0x1d) + -1;
    }
    return;
  }
  return;
}


// ======== FUN_08013100 @ 08013100 size=494 ========

void FUN_08013100(uint param_1,int param_2,int param_3,byte *param_4,byte *param_5,int param_6)

{
  byte bVar1;
  byte bVar2;
  uint uVar3;
  int *piVar4;
  byte bVar5;
  byte bVar6;
  int iVar7;
  int iVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 local_30;
  undefined4 local_2c [2];
  
  if (8 < param_1) {
    return;
  }
  if (param_2 == 0) {
    return;
  }
  if (param_6 == 0) {
    return;
  }
  bVar6 = *param_4;
  bVar5 = *param_5;
  if (param_1 == 0) {
    if (*(int *)(param_2 + 4) == 0) {
      iVar7 = FUN_0801499e(param_2,param_3);
      if (iVar7 != 0) {
        *(undefined1 *)(param_6 + 1) = 2;
        *(undefined4 *)(param_6 + 4) = 0;
LAB_08013202:
        bVar6 = 0;
        goto LAB_080131ee;
      }
    }
    else {
      iVar8 = **(int **)(param_2 + 0xc);
      local_30 = 0;
      local_2c[0] = 0;
      iVar7 = FUN_080179f8(*(undefined4 *)(iVar8 + 8),*(undefined4 *)(iVar8 + 0xc),
                           *(undefined4 *)(param_3 + 8),*(undefined4 *)(param_3 + 0xc),
                           *(int *)(*(int *)(DAT_080132f4 + 4) + 0x78) << 2,100,&local_30,local_2c);
      if ((((iVar7 != 0) &&
           (iVar7 = FUN_080179f8(*(undefined4 *)(iVar8 + 8),*(undefined4 *)(iVar8 + 0xc),
                                 *(undefined4 *)(param_3 + 8),*(undefined4 *)(param_3 + 0xc),
                                 *(int *)(*(int *)(DAT_080132f4 + 4) + 0x78) << 3,0x96,&local_30,
                                 local_2c), iVar7 != 0)) &&
          (iVar7 = FUN_080179f8(*(undefined4 *)(iVar8 + 8),*(undefined4 *)(iVar8 + 0xc),
                                *(undefined4 *)(param_3 + 8),*(undefined4 *)(param_3 + 0xc),
                                *(int *)(*(int *)(DAT_080132f4 + 4) + 0x78) * 3,100,&local_30,
                                local_2c), iVar7 != 0)) &&
         (iVar7 = FUN_080179f8(*(undefined4 *)(iVar8 + 8),*(undefined4 *)(iVar8 + 0xc),
                               *(undefined4 *)(param_3 + 8),*(undefined4 *)(param_3 + 0xc),
                               *(int *)(*(int *)(DAT_080132f4 + 4) + 0x78) * 7,0x96,&local_30,
                               local_2c), iVar7 != 0)) {
        if (bVar5 < 2) goto LAB_08013206;
        FUN_0801364c(param_2);
        goto LAB_080131ee;
      }
      iVar7 = FUN_0801499e(param_2,param_3);
      if (iVar7 != 0) goto LAB_08013202;
    }
LAB_0801327c:
    if (2 < *(uint *)(param_2 + 4)) {
      bVar1 = 0;
      bVar2 = 0;
      uVar10 = 0;
      uVar9 = 0;
      piVar4 = *(int **)(param_2 + 0x10);
      for (uVar3 = 0; uVar3 < *(uint *)(param_2 + 4); uVar3 = (uint)(char)((char)uVar3 + '\x01')) {
        iVar7 = *piVar4;
        if (*(char *)(iVar7 + 1) == '\0') {
          bVar1 = bVar1 + 1;
          uVar9 = *(undefined4 *)(iVar7 + 4);
        }
        else if (*(char *)(iVar7 + 1) == '\x01') {
          bVar2 = bVar2 + 1;
          uVar10 = *(undefined4 *)(iVar7 + 4);
        }
        piVar4 = (int *)piVar4[2];
      }
      if (bVar1 < bVar2) {
        *(undefined1 *)(param_6 + 1) = 1;
        *(undefined4 *)(param_6 + 4) = uVar10;
      }
      else {
        *(undefined1 *)(param_6 + 1) = 0;
        *(undefined4 *)(param_6 + 4) = uVar9;
      }
      goto LAB_080132e4;
    }
  }
  else {
    if (param_1 == 3) {
      if (bVar6 < 9) {
        bVar6 = bVar6 + 1;
      }
      else {
        FUN_0801364c(param_2);
        *(undefined1 *)(param_6 + 1) = 2;
        *(undefined4 *)(param_6 + 4) = 0;
LAB_080131ee:
        bVar5 = 0;
      }
      goto LAB_0801327c;
    }
    if (param_1 == 2) {
      *(undefined1 *)(param_6 + 1) = 2;
      *(undefined4 *)(param_6 + 4) = 0;
      *param_4 = 0;
      *param_5 = 0;
      FUN_0801364c(param_2);
      return;
    }
    if (*(int *)(param_2 + 4) != 0) {
      if (bVar5 < 8) {
LAB_08013206:
        bVar5 = bVar5 + 1;
      }
      else {
        FUN_0801364c(param_2);
        bVar5 = 0;
        *(undefined1 *)(param_6 + 1) = 2;
        *(undefined4 *)(param_6 + 4) = 0;
        *param_4 = 0;
        *param_5 = 0;
      }
      goto LAB_0801327c;
    }
  }
  *(undefined1 *)(param_6 + 1) = 2;
  *(undefined4 *)(param_6 + 4) = 0;
LAB_080132e4:
  *param_4 = bVar6;
  *param_5 = bVar5;
  return;
}


// ======== FUN_080132f8 @ 080132f8 size=24 ========

void FUN_080132f8(void)

{
  int iVar1;
  
  iVar1 = *(int *)(DAT_08013310 + 4);
  *(undefined1 *)(iVar1 + 0x39) = 2;
  *(undefined4 *)(iVar1 + 0x3c) = 0;
  *(undefined1 *)(iVar1 + 0x59) = 2;
  *(undefined4 *)(iVar1 + 0x5c) = 0;
  FUN_08013314();
  return;
}


// ======== FUN_08013314 @ 08013314 size=100 ========

void FUN_08013314(void)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  
  iVar3 = DAT_08013378;
  iVar1 = *(int *)(DAT_08013378 + 4);
  *(undefined4 *)(iVar1 + 0x88) = 0;
  *(undefined4 *)(iVar1 + 0x8c) = 0;
  if (*(int *)(iVar1 + 0x30) != 0) {
    FUN_08013d7c();
  }
  if (*(int *)(*(int *)(iVar3 + 4) + 0x50) != 0) {
    FUN_08013d7c();
  }
  uVar2 = FUN_08013950(0x28,0x10);
  *(undefined4 *)(*(int *)(iVar3 + 4) + 0x30) = uVar2;
  uVar2 = FUN_08013950(0x28,0x10);
  iVar3 = *(int *)(iVar3 + 4);
  *(undefined4 *)(iVar3 + 0x50) = uVar2;
  *(undefined1 *)(iVar3 + 0x11) = 2;
  *(undefined4 *)(iVar3 + 0x14) = 0;
  *(undefined1 *)(iVar3 + 0x21) = 2;
  *(undefined4 *)(iVar3 + 0x24) = 0;
  *(undefined1 *)(iVar3 + 0xa8) = 0;
  *(undefined1 *)(iVar3 + 0xa9) = 0;
  *(undefined1 *)(iVar3 + 0xac) = 0;
  *(undefined1 *)(iVar3 + 0xad) = 0;
  *(undefined1 *)(iVar3 + 0xa1) = 0;
  FUN_08019e34();
  FUN_08019f84();
  return;
}


// ======== FUN_0801337c @ 0801337c size=438 ========

undefined4 FUN_0801337c(void)

{
  longlong lVar1;
  undefined4 *puVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  uint uVar10;
  uint uVar11;
  bool bVar12;
  longlong lVar13;
  
  iVar3 = DAT_08013534;
  if (*(uint *)(*(int *)(DAT_08013534 + 4) + 0x78) < 1000) {
    return 0xffffffff;
  }
  if (*(int *)(*(int *)(DAT_08013534 + 4) + 0x50) != 0) {
    FUN_08013d7c();
  }
  if (*(int *)(*(int *)(iVar3 + 4) + 0x30) != 0) {
    FUN_08013d7c();
  }
  FUN_08019eb8();
  FUN_0801ba58();
  lVar13 = (**(code **)(*(int *)(*(int *)(iVar3 + 4) + 4) + 0x10))();
  puVar2 = DAT_08013538;
  uVar8 = (uint)((ulonglong)lVar13 >> 0x20);
  iVar5 = *(int *)(iVar3 + 4);
  if (uVar8 <= *(uint *)(iVar5 + 0x8c) &&
      (uint)(*(uint *)(iVar5 + 0x88) <= (uint)lVar13) <= uVar8 - *(uint *)(iVar5 + 0x8c)) {
    lVar13 = (**(code **)(*(int *)(iVar5 + 4) + 0x10))();
    uVar11 = (uint)((ulonglong)lVar13 >> 0x20);
    uVar8 = (uint)lVar13;
    uVar7 = *(uint *)(*(int *)(iVar3 + 4) + 0x88);
    uVar6 = *(uint *)(*(int *)(iVar3 + 4) + 0x8c);
    if (uVar11 <= uVar6 && (uint)(uVar7 <= uVar8) <= uVar11 - uVar6) {
      FUN_08019e34(uVar8,uVar11,(uVar11 - uVar6) - (uint)(uVar7 > uVar8),uVar8 - uVar7);
      FUN_08019f84();
      goto LAB_08013426;
    }
  }
  iVar5 = *(int *)(iVar3 + 4);
  uVar8 = 2;
  uVar6 = *(uint *)(iVar5 + 0x88);
  uVar11 = (uint)((ulonglong)(lVar13 + 2000) >> 0x20);
  while (lVar1 = (ulonglong)uVar8 * (ulonglong)*(uint *)(iVar5 + 0x78) + *(longlong *)(iVar5 + 0x88)
        , uVar7 = (uint)((ulonglong)lVar1 >> 0x20),
        uVar7 <= uVar11 && (uint)((uint)(lVar13 + 2000) <= (uint)lVar1) <= uVar7 - uVar11) {
    uVar8 = uVar8 + 1;
  }
  if (uVar8 < 0x10) {
    uVar8 = uVar8 * *(int *)(iVar5 + 0x78);
    uVar9 = uVar6 + uVar8;
    uVar11 = uVar9 - (uint)lVar13;
    uVar10 = *(uint *)(iVar5 + 0x78);
    uVar7 = uVar11 + DAT_0801353c;
    for (uVar8 = (((*(int *)(iVar5 + 0x8c) + (uint)CARRY4(uVar6,uVar8)) -
                  (int)((ulonglong)lVar13 >> 0x20)) - (uint)(uVar9 < (uint)lVar13)) + -1 +
                 (uint)CARRY4(uVar11,DAT_0801353c); uVar8 != 0 || uVar8 < (uVar10 << 1 <= uVar7);
        uVar8 = uVar8 - bVar12) {
      bVar12 = uVar7 < uVar10;
      uVar7 = uVar7 - uVar10;
    }
    (**(code **)(*(int *)(iVar5 + 4) + 8))();
    iVar5 = FUN_0801c818();
    if (iVar5 == 0) {
      *puVar2 = 0x10000000;
      DataSynchronizationBarrier(0xf);
      InstructionSynchronizationBarrier(0xf);
    }
    iVar5 = *(int *)(iVar3 + 4);
    *(undefined2 *)(iVar5 + 0x90) = 0;
    *(undefined1 *)(iVar5 + 0xa2) = 0;
    *(undefined2 *)(iVar5 + 0x94) = 0;
    *(undefined1 *)(iVar5 + 0xa3) = 0xff;
    *(undefined1 *)(iVar5 + 0xa4) = 0;
    if (*(char *)(iVar5 + 0x39) != '\x02') {
      *(undefined1 *)(iVar5 + 0x4d) = 0;
    }
    if (*(char *)(iVar5 + 0x59) != '\x02') {
      *(undefined1 *)(iVar5 + 0x6d) = 0;
    }
    *(undefined1 *)(iVar5 + 0x11) = 2;
    *(undefined4 *)(iVar5 + 0x14) = 0;
    *(undefined1 *)(iVar5 + 0x21) = 2;
    *(undefined4 *)(iVar5 + 0x24) = 0;
    *(undefined1 *)(iVar5 + 0xa5) = 0;
    *(undefined1 *)(iVar5 + 0xa6) = 0;
    *(undefined2 *)(iVar5 + 0x96) = 0;
    *(undefined2 *)(iVar5 + 0x98) = 0;
    *(undefined2 *)(iVar5 + 0x9a) = 0;
    *(undefined1 *)(iVar5 + 0xaa) = 0;
    *(undefined1 *)(iVar5 + 0xab) = 0;
    *(undefined1 *)(iVar5 + 0xae) = 0;
    *(undefined1 *)(iVar5 + 0xad) = 0;
    iVar5 = FUN_08013950(5,0x10);
    *(int *)(*(int *)(iVar3 + 4) + 0x30) = iVar5;
    if (iVar5 != 0) {
      iVar5 = FUN_08013950(5,0x10);
      iVar4 = *(int *)(iVar3 + 4);
      *(int *)(iVar4 + 0x50) = iVar5;
      if (iVar5 != 0) {
        FUN_0801364c(*(undefined4 *)(iVar4 + 0x30));
        FUN_0801364c(*(undefined4 *)(*(int *)(iVar3 + 4) + 0x50));
        FUN_0801364c(*(undefined4 *)(*(int *)(iVar3 + 4) + 8));
        FUN_0801364c(*(undefined4 *)(*(int *)(iVar3 + 4) + 0xc));
        FUN_0801364c(*(undefined4 *)(*(int *)(iVar3 + 4) + 0x34));
        FUN_0801364c(*(undefined4 *)(*(int *)(iVar3 + 4) + 0x54));
        return 0;
      }
      return 0xffffffff;
    }
    return 0xffffffff;
  }
  FUN_08019e34();
LAB_08013426:
  iVar3 = FUN_0801c818();
  if (iVar3 == 0) {
    *puVar2 = 0x10000000;
    DataSynchronizationBarrier(0xf);
    InstructionSynchronizationBarrier(0xf);
  }
  return 0xffffffff;
}


// ======== FUN_08013540 @ 08013540 size=260 ========

undefined8 FUN_08013540(int param_1,int param_2)

{
  undefined4 uVar1;
  uint uVar2;
  uint uVar3;
  char cVar4;
  undefined4 uVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  
  uVar3 = *(uint *)(param_2 + 4);
  uVar2 = *(uint *)(param_1 + 4);
  uVar5 = (undefined4)DAT_08013644;
  uVar1 = (undefined4)((ulonglong)DAT_08013644 >> 0x20);
  if (*(char *)(param_1 + 1) == *(char *)(param_2 + 1)) {
    if ((int)uVar2 < 0) {
      uVar2 = -uVar2;
    }
    if ((int)uVar3 < 0) {
      uVar3 = -uVar3;
    }
    if ((uVar3 != 0) && (uVar2 != 0)) {
      cVar4 = uVar3 <= uVar2;
      if ((int)uVar2 < (int)uVar3) {
        uVar6 = FUN_08010e52(uVar3);
        uVar7 = FUN_08010e52(uVar2);
        uVar6 = FUN_08010d74((int)uVar7,(int)((ulonglong)uVar7 >> 0x20),(int)uVar6,
                             (int)((ulonglong)uVar6 >> 0x20));
        FUN_08010efc((int)uVar6,(int)((ulonglong)uVar6 >> 0x20),uVar5,uVar1);
        if (cVar4 != '\0') goto LAB_08013638;
      }
      else {
        uVar6 = FUN_08010e52(uVar2);
        uVar7 = FUN_08010e52(uVar3);
        uVar6 = FUN_08010d74((int)uVar7,(int)((ulonglong)uVar7 >> 0x20),(int)uVar6,
                             (int)((ulonglong)uVar6 >> 0x20));
        FUN_08010efc((int)uVar6,(int)((ulonglong)uVar6 >> 0x20),uVar5,uVar1);
        if (cVar4 != '\0') goto LAB_0801363e;
      }
      uVar2 = *(int *)(param_1 + 0xc) + *(int *)(param_2 + 0xc) +
              (uint)CARRY4(*(uint *)(param_1 + 8),*(uint *)(param_2 + 8));
      uVar3 = uVar2 >> 1;
      uVar2 = (uint)((uVar2 & 1) != 0) << 0x1f |
              *(uint *)(param_1 + 8) + *(uint *)(param_2 + 8) >> 1;
      goto LAB_08013630;
    }
  }
  else {
    if ((int)uVar2 < 0) {
      uVar2 = -uVar2;
    }
    if ((int)uVar3 < 0) {
      uVar3 = -uVar3;
    }
    if ((uVar3 != 0) && (uVar2 != 0)) {
      cVar4 = uVar3 <= uVar2;
      if ((int)uVar2 < (int)uVar3) {
        uVar6 = FUN_08010e52(uVar3);
        uVar7 = FUN_08010e52(uVar2);
        uVar6 = FUN_08010d74((int)uVar7,(int)((ulonglong)uVar7 >> 0x20),(int)uVar6,
                             (int)((ulonglong)uVar6 >> 0x20));
        FUN_08010efc((int)uVar6,(int)((ulonglong)uVar6 >> 0x20),uVar5,uVar1);
        if (cVar4 == '\0') goto LAB_08013596;
LAB_08013638:
        uVar2 = *(uint *)(param_2 + 8);
        uVar3 = *(uint *)(param_2 + 0xc);
        goto LAB_08013630;
      }
      uVar6 = FUN_08010e52(uVar2);
      uVar7 = FUN_08010e52(uVar3);
      uVar6 = FUN_08010d74((int)uVar7,(int)((ulonglong)uVar7 >> 0x20),(int)uVar6,
                           (int)((ulonglong)uVar6 >> 0x20));
      FUN_08010efc((int)uVar6,(int)((ulonglong)uVar6 >> 0x20),uVar5,uVar1);
      if (cVar4 == '\0') {
LAB_08013596:
        if (*(char *)(param_1 + 1) != '\0') goto LAB_08013638;
      }
    }
  }
LAB_0801363e:
  uVar2 = *(uint *)(param_1 + 8);
  uVar3 = *(uint *)(param_1 + 0xc);
LAB_08013630:
  return CONCAT44(uVar3,uVar2);
}


// ======== FUN_0801364c @ 0801364c size=52 ========

undefined4 FUN_0801364c(int param_1)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  
  if (param_1 == 0) {
    uVar1 = 0xffffffff;
  }
  else {
    uVar1 = 0;
    if (*(int *)(param_1 + 4) != 0) {
      puVar2 = *(undefined4 **)(param_1 + 0xc);
      while (puVar2 != (undefined4 *)0x0) {
        FUN_080195ac(*puVar2);
        puVar3 = (undefined4 *)puVar2[1];
        FUN_080195ac(puVar2);
        puVar2 = puVar3;
      }
      *(undefined4 *)(param_1 + 0x10) = 0;
      *(undefined4 *)(param_1 + 0xc) = 0;
      *(undefined4 *)(param_1 + 4) = 0;
      return 0;
    }
  }
  return uVar1;
}


// ======== FUN_08013680 @ 08013680 size=160 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_08013680(undefined4 param_1,char *param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  int iVar2;
  int iVar3;
  uint local_10;
  undefined4 local_c;
  
  local_10 = 0;
  local_c = param_4;
  iVar1 = FUN_08016ab4();
  if (iVar1 == 0) {
    param_1 = 0xe4;
    param_2 = s_get_factory_driver_failed_08013720;
    FUN_080141e8(2,&DAT_08013764,s____src_application_process_comm__08013740,DAT_0801373c,0xe4,
                 s_get_factory_driver_failed_08013720);
  }
  iVar3 = -1;
  local_c = FUN_0801c510();
  iVar1 = _DAT_0801376c;
  do {
    while (*(char *)(*(int *)(iVar1 + 4) + 0xd) != '\0') {
      iVar2 = FUN_0801c618(0,0xffffffff,&local_10,0xffffffff,param_1,param_2);
      if (iVar2 == 1) {
        if ((local_10 & 1) != 0) {
          iVar2 = *(int *)(iVar1 + 4);
          *(undefined1 *)(iVar2 + 0xd) = 0;
          *(byte *)(iVar2 + 0xc) = *(byte *)(iVar2 + 0xc) & 0xfb | 2;
          local_c = FUN_0801c510();
        }
      }
      else {
        param_1 = 0x105;
        param_2 = s_comm_task_init_bd_task_wait_fail_0801376f + 1;
        FUN_080141e8(2,&DAT_08013764,s____src_application_process_comm__08013740,DAT_0801373c);
      }
    }
    if (iVar3 == -1) {
      iVar2 = FUN_08017884();
      if (iVar2 == 0) {
        iVar3 = 0;
      }
    }
    else {
      FUN_08014ac8();
    }
    FUN_08013970();
    FUN_08019914();
    FUN_0801b6d8(&local_c,0x23);
  } while( true );
}


// ======== FUN_080138a4 @ 080138a4 size=24 ========

undefined4 FUN_080138a4(int param_1,uint param_2)

{
  undefined4 *puVar1;
  uint uVar2;
  
  puVar1 = DAT_080138bc;
  for (uVar2 = 0; uVar2 < param_2; uVar2 = uVar2 + 1) {
    *puVar1 = *(undefined4 *)(param_1 + uVar2 * 4);
  }
  return *puVar1;
}


// ======== FUN_080138c0 @ 080138c0 size=6 ========

undefined4 FUN_080138c0(void)

{
  return *DAT_080138c8;
}


// ======== FUN_080138dc @ 080138dc size=86 ========

undefined4 * FUN_080138dc(void)

{
  int *piVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  
  piVar1 = DAT_08013934;
  puVar3 = (undefined4 *)*DAT_08013934;
  if ((undefined4 *)*DAT_08013934 == (undefined4 *)0x0) {
    iVar2 = FUN_080195b8(0x1c);
    *piVar1 = iVar2;
    puVar3 = (undefined4 *)0x0;
    if (iVar2 != 0) {
      iVar2 = FUN_080195b8(0x2c);
      puVar4 = (undefined4 *)*piVar1;
      puVar4[6] = iVar2;
      puVar3 = (undefined4 *)0x0;
      if (iVar2 != 0) {
        *puVar4 = DAT_08013938;
        puVar4[1] = DAT_0801393c;
        puVar4[5] = DAT_08013940;
        puVar4[4] = DAT_08013944;
        puVar4[2] = DAT_08013948;
        puVar4[3] = DAT_0801394c;
        piVar1[1] = iVar2;
        *(undefined1 *)(iVar2 + 0x10) = 0;
        *(undefined1 *)(iVar2 + 0x20) = 1;
        *(undefined4 *)(iVar2 + 0x14) = 0;
        *(undefined4 *)(iVar2 + 0x18) = 0;
        *(undefined4 *)(iVar2 + 0x1c) = 0;
        *(undefined4 *)(iVar2 + 0x24) = 0;
        *(undefined1 *)(iVar2 + 0x28) = 1;
        puVar3 = puVar4;
      }
    }
  }
  return puVar3;
}


// ======== FUN_08013950 @ 08013950 size=30 ========

void FUN_08013950(undefined4 param_1,undefined4 param_2)

{
  undefined4 *puVar1;
  
  puVar1 = (undefined4 *)FUN_080195b8(0x14);
  if (puVar1 != (undefined4 *)0x0) {
    puVar1[4] = 0;
    puVar1[2] = param_2;
    puVar1[3] = 0;
    *puVar1 = param_1;
    puVar1[1] = 0;
  }
  return;
}


// ======== FUN_08013970 @ 08013970 size=266 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_08013970(void)

{
  char cVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  char *pcVar6;
  char cVar7;
  
  iVar4 = _DAT_08013a7c;
  iVar2 = (**(code **)(*(int *)(*(int *)(_DAT_08013a7c + 4) + 8) + 4))();
  iVar3 = (**(code **)(*(int *)(*(int *)(iVar4 + 4) + 8) + 8))();
  cVar7 = '\0';
  if (iVar2 == 1) {
    iVar4 = *(int *)(iVar4 + 4);
    if (iVar3 == 1) {
      if (*(byte *)(iVar4 + 0xf) < 10) {
        *(byte *)(iVar4 + 0xf) = *(byte *)(iVar4 + 0xf) + 1;
        *(char *)(iVar4 + 0x10) = *(char *)(iVar4 + 0x10) + '\x01';
        cVar7 = *(char *)(iVar4 + 0x11) + '\x01';
LAB_080139a6:
        *(char *)(iVar4 + 0x11) = cVar7;
        return;
      }
      if (*(char *)(iVar4 + 0xe) == '\x01') {
        return;
      }
      *(undefined1 *)(iVar4 + 0xe) = 1;
      *(undefined1 *)(iVar4 + 0x10) = 0;
      *(undefined1 *)(iVar4 + 0x11) = 0;
      pcVar6 = s_both_lifted_08013abc;
      uVar5 = 0x115;
    }
    else {
      if (*(byte *)(iVar4 + 0x10) < 10) {
        cVar7 = *(byte *)(iVar4 + 0x10) + 1;
LAB_08013a4c:
        *(char *)(iVar4 + 0x10) = cVar7;
        return;
      }
      if (*(char *)(iVar4 + 0xe) == '\x02') {
        return;
      }
      *(undefined1 *)(iVar4 + 0xe) = 2;
      *(undefined1 *)(iVar4 + 0xf) = 0;
      *(undefined1 *)(iVar4 + 0x11) = 0;
      pcVar6 = s_left_lifted_08013a7f + 1;
      uVar5 = 0x123;
    }
  }
  else {
    iVar4 = *(int *)(iVar4 + 4);
    if (iVar3 == 1) {
      if (*(byte *)(iVar4 + 0x11) < 10) {
        cVar7 = *(byte *)(iVar4 + 0x11) + 1;
LAB_08013a66:
        *(char *)(iVar4 + 0x11) = cVar7;
        return;
      }
      if (*(char *)(iVar4 + 0xe) == '\x03') {
        return;
      }
      *(undefined1 *)(iVar4 + 0xe) = 3;
      *(undefined1 *)(iVar4 + 0xf) = 0;
      *(undefined1 *)(iVar4 + 0x10) = 0;
      pcVar6 = s_right_lifted_08013ac8;
      uVar5 = 0x12f;
    }
    else {
      cVar1 = *(char *)(iVar4 + 0xe);
      if (cVar1 == '\x01') {
        if (*(char *)(iVar4 + 0xf) != '\0') {
          *(char *)(iVar4 + 0xf) = *(char *)(iVar4 + 0xf) + -1;
          return;
        }
        *(undefined1 *)(iVar4 + 0xe) = 0;
        *(undefined1 *)(iVar4 + 0xf) = 0;
        *(undefined1 *)(iVar4 + 0x10) = 0;
        *(undefined1 *)(iVar4 + 0x11) = 0;
        pcVar6 = s_lift_none_08013ad8;
        uVar5 = 0x13e;
      }
      else if (cVar1 == '\x02') {
        if (*(char *)(iVar4 + 0x10) != '\0') {
          cVar7 = *(char *)(iVar4 + 0x10) + -1;
          goto LAB_08013a4c;
        }
        *(undefined1 *)(iVar4 + 0xe) = 0;
        *(undefined1 *)(iVar4 + 0xf) = 0;
        *(undefined1 *)(iVar4 + 0x10) = 0;
        *(undefined1 *)(iVar4 + 0x11) = 0;
        pcVar6 = s_lift_none_08013ad8;
        uVar5 = 0x148;
      }
      else {
        if (cVar1 != '\x03') {
          *(undefined1 *)(iVar4 + 0xe) = 0;
          *(undefined1 *)(iVar4 + 0xf) = 0;
          *(undefined1 *)(iVar4 + 0x10) = 0;
          goto LAB_080139a6;
        }
        if (*(char *)(iVar4 + 0x11) != '\0') {
          cVar7 = *(char *)(iVar4 + 0x11) + -1;
          goto LAB_08013a66;
        }
        *(undefined1 *)(iVar4 + 0xe) = 0;
        *(undefined1 *)(iVar4 + 0xf) = 0;
        *(undefined1 *)(iVar4 + 0x10) = 0;
        *(undefined1 *)(iVar4 + 0x11) = 0;
        pcVar6 = s_lift_none_08013ad8;
        uVar5 = 0x152;
      }
    }
  }
  FUN_080141e8(3,&DAT_08013ab4,s____src_application_process_comm__08013a90,DAT_08013a8c,uVar5,pcVar6
              );
  return;
}


// ======== FUN_08013ae4 @ 08013ae4 size=74 ========

undefined4 FUN_08013ae4(int param_1,uint *param_2,undefined4 param_3,undefined4 param_4,int param_5)

{
  int iVar1;
  uint uVar2;
  
  if ((3 < *param_2) && (*param_2 < 0x81)) {
    if ((param_5 == 1) && ((param_1 != 0 && (param_2 != (uint *)0x0)))) {
      for (uVar2 = 1; uVar2 < *param_2 - 1; uVar2 = uVar2 + 1) {
        *(byte *)(param_1 + uVar2) = *(byte *)(param_1 + uVar2) ^ 0x5b;
      }
    }
    iVar1 = FUN_08019388();
    if (iVar1 == 0) {
      return 0;
    }
  }
  return 0xffffffff;
}


// ======== FUN_08013b30 @ 08013b30 size=138 ========

undefined4
FUN_08013b30(undefined4 param_1,uint param_2,undefined1 *param_3,uint *param_4,int param_5,
            int param_6)

{
  undefined1 uVar1;
  int iVar2;
  uint uVar3;
  undefined4 uVar4;
  
  if ((((param_2 == 0) || (0x80 < param_2)) || (param_4 == (uint *)0x0)) ||
     (iVar2 = FUN_08010912(param_3 + 1,param_1,param_2), iVar2 == 0)) {
    uVar4 = 0xffffffff;
  }
  else {
    if (param_6 == 1) {
      uVar1 = FUN_08013008(param_1,param_2);
      *param_3 = 0x26;
      param_3[param_2 + 2] = 0x23;
      param_3[param_2 + 1] = uVar1;
      param_2 = param_2 + 3;
    }
    else {
      *param_3 = 0x26;
      param_3[param_2 + 1] = 0x23;
      param_2 = param_2 + 2;
    }
    *param_4 = param_2;
    if (((param_5 == 1) && (param_3 != (undefined1 *)0x0)) && (2 < param_2)) {
      for (uVar3 = 1; uVar3 < *param_4 - 1; uVar3 = uVar3 + 1) {
        param_3[uVar3] = param_3[uVar3] ^ 0x5b;
      }
    }
    uVar4 = 0;
  }
  return uVar4;
}


// ======== FUN_08013bbc @ 08013bbc size=274 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_08013bbc(void)

{
  undefined4 *puVar1;
  int iVar2;
  code *pcVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  uint uVar10;
  short sVar11;
  bool bVar12;
  longlong lVar13;
  
  iVar5 = _DAT_08013cd0;
  if (*(int *)(*(int *)(_DAT_08013cd0 + 4) + 0x48) == 0) {
    iVar2 = FUN_080195b8((uint)*(ushort *)(*(int *)(_DAT_08013cd0 + 4) + 0x92) << 1);
    *(int *)(*(int *)(iVar5 + 4) + 0x48) = iVar2;
    if (iVar2 == 0) {
      return 0xffffffff;
    }
  }
  if (*(int *)(*(int *)(iVar5 + 4) + 0x68) == 0) {
    iVar2 = FUN_080195b8((uint)*(ushort *)(*(int *)(iVar5 + 4) + 0x92) << 1);
    *(int *)(*(int *)(iVar5 + 4) + 0x68) = iVar2;
    if (iVar2 == 0) {
      return 0xffffffff;
    }
  }
  if (*(int *)(*(int *)(iVar5 + 4) + 0x70) == 0) {
    iVar2 = FUN_080195b8((uint)*(ushort *)(*(int *)(iVar5 + 4) + 0x92) << 2);
    iVar6 = *(int *)(iVar5 + 4);
    *(int *)(iVar6 + 0x70) = iVar2;
    if (iVar2 == 0) {
      FUN_080141e8(1,s_init_bd_08013d20,s____src_application_process_deal__08013cf8,DAT_08013cf4,
                   0xefa,s_malloc_all_sensor_data_failed_08013cd3 + 1);
    }
    else {
      (**(code **)(*(int *)(iVar6 + 0x74) + 4))(iVar2,*(undefined2 *)(iVar6 + 0x92));
    }
  }
  FUN_0801ba58();
  lVar13 = (**(code **)(*(int *)(*(int *)(iVar5 + 4) + 4) + 0x10))();
  puVar1 = DAT_08013d28;
  sVar11 = 10;
  do {
    uVar8 = (uint)((ulonglong)lVar13 >> 0x20);
    uVar4 = (uint)lVar13;
    iVar2 = *(int *)(iVar5 + 4);
    uVar7 = *(uint *)(iVar2 + 0x8c);
    bVar12 = *(uint *)(iVar2 + 0x88) <= uVar4;
    if (uVar7 < uVar8 || uVar8 - uVar7 < (uint)bVar12) {
      if (sVar11 != 0) {
        uVar8 = *(uint *)(iVar2 + 0x78) + DAT_08013d2c;
        uVar9 = *(uint *)(iVar2 + 0x88) + uVar8;
        uVar7 = *(int *)(iVar2 + 0x8c) + (CARRY4(*(uint *)(iVar2 + 0x78),DAT_08013d2c) - 1) +
                (uint)CARRY4(*(uint *)(iVar2 + 0x88),uVar8);
        uVar10 = (uint)(lVar13 + 100);
        uVar8 = (uint)((ulonglong)(lVar13 + 100) >> 0x20);
        if (uVar7 < uVar8 || uVar8 - uVar7 < (uint)(uVar9 <= uVar10)) {
          for (; uVar7 <= uVar8 && (uint)(uVar10 <= uVar9) <= uVar7 - uVar8; uVar7 = uVar7 + bVar12)
          {
            bVar12 = CARRY4(*(uint *)(iVar2 + 0x78),uVar9);
            uVar9 = *(uint *)(iVar2 + 0x78) + uVar9;
          }
        }
        (**(code **)(*(int *)(iVar2 + 4) + 8))(uVar9 - uVar4);
        iVar5 = FUN_0801c818();
        if (iVar5 == 0) {
          *puVar1 = 0x10000000;
          DataSynchronizationBarrier(0xf);
          InstructionSynchronizationBarrier(0xf);
        }
        return 0;
      }
      break;
    }
    pcVar3 = *(code **)(*(int *)(iVar2 + 4) + 0x10);
    lVar13 = (*pcVar3)(pcVar3,uVar8,(uVar8 - uVar7) - (uint)!bVar12);
    sVar11 = sVar11 + -1;
  } while (sVar11 != 0);
  iVar5 = FUN_0801c818();
  if (iVar5 == 0) {
    *puVar1 = 0x10000000;
    DataSynchronizationBarrier(0xf);
    InstructionSynchronizationBarrier(0xf);
  }
  return 0xffffffff;
}


// ======== FUN_08013d30 @ 08013d30 size=76 ========

undefined4 FUN_08013d30(int param_1,int param_2)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  
  uVar1 = 0xffffffff;
  if ((param_1 != 0) && (*(int *)(param_1 + 4) != 0)) {
    puVar3 = *(undefined4 **)(param_1 + 0x10);
    if (param_2 != 0) {
      FUN_08010912(param_2,*puVar3,*(undefined4 *)(param_1 + 8));
    }
    if (*(int *)(param_1 + 4) == 1) {
      *(undefined4 *)(param_1 + 0xc) = 0;
      *(undefined4 *)(param_1 + 0x10) = 0;
    }
    else {
      iVar2 = *(int *)(*(int *)(param_1 + 0x10) + 8);
      *(int *)(param_1 + 0x10) = iVar2;
      *(undefined4 *)(iVar2 + 4) = 0;
    }
    FUN_080195ac(*puVar3);
    FUN_080195ac(puVar3);
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + -1;
    uVar1 = 0;
  }
  return uVar1;
}


// ======== FUN_08013d7c @ 08013d7c size=26 ========

undefined4 FUN_08013d7c(int param_1)

{
  if (param_1 != 0) {
    FUN_0801364c();
    FUN_080195ac(param_1);
    return 0;
  }
  return 0xffffffff;
}


// ======== FUN_08013d96 @ 08013d96 size=32 ========

void FUN_08013d96(int param_1,int param_2)

{
  int iVar1;
  
  iVar1 = FUN_08013f5c();
  if (iVar1 != 0) {
    param_1 = param_1 + param_2 * 0x14;
    *(uint *)(param_1 + 8) = *(uint *)(param_1 + 8) & 0xfffffffe;
    return;
  }
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}


// ======== FUN_08013db6 @ 08013db6 size=32 ========

void FUN_08013db6(int param_1,int param_2)

{
  int iVar1;
  
  iVar1 = FUN_08013f5c();
  if (iVar1 != 0) {
    param_1 = param_1 + param_2 * 0x14;
    *(uint *)(param_1 + 8) = *(uint *)(param_1 + 8) | 1;
    return;
  }
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}


// ======== FUN_08013dd6 @ 08013dd6 size=32 ========

void FUN_08013dd6(int param_1,int param_2)

{
  int iVar1;
  
  iVar1 = FUN_08013f5c();
  if (iVar1 != 0) {
    param_1 = param_1 + param_2 * 0x14;
    *(uint *)(param_1 + 8) = *(uint *)(param_1 + 8) & 0xffffffdf;
    return;
  }
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}


// ======== FUN_08013df6 @ 08013df6 size=32 ========

void FUN_08013df6(int param_1,int param_2)

{
  int iVar1;
  
  iVar1 = FUN_08013f5c();
  if (iVar1 != 0) {
    param_1 = param_1 + param_2 * 0x14;
    *(uint *)(param_1 + 8) = *(uint *)(param_1 + 8) | 0x20;
    return;
  }
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}


// ======== FUN_08013e16 @ 08013e16 size=54 ========

void FUN_08013e16(int param_1,uint param_2)

{
  int iVar1;
  
  iVar1 = FUN_08013f5c();
  if (iVar1 != 0) {
    iVar1 = param_1 + param_2 * 0x14;
    *(uint *)(iVar1 + 8) = *(uint *)(iVar1 + 8) & 0xfffffffe;
    *(undefined4 *)(iVar1 + 8) = 0;
    *(undefined4 *)(iVar1 + 0xc) = 0;
    *(undefined4 *)(iVar1 + 0x10) = 0;
    *(undefined4 *)(iVar1 + 0x14) = 0;
    *(uint *)(param_1 + 4) = *(uint *)(param_1 + 4) | 0xf << ((param_2 & 0x3f) << 2);
    return;
  }
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}


// ======== FUN_08013e4c @ 08013e4c size=120 ========

void FUN_08013e4c(int param_1,int param_2,undefined4 *param_3)

{
  int iVar1;
  uint uVar2;
  
  iVar1 = FUN_08013f5c();
  if (iVar1 != 0) {
    param_1 = param_1 + param_2 * 0x14;
    *(undefined4 *)(param_1 + 0x10) = *param_3;
    *(undefined4 *)(param_1 + 0x14) = param_3[2];
    *(uint *)(param_1 + 0xc) = (uint)*(ushort *)(param_3 + 4);
    *(uint *)(param_1 + 8) =
         param_3[1] | param_3[3] | param_3[5] | *(uint *)(param_1 + 8) & 0xffffc0ff;
    if (*(char *)(param_3 + 6) == '\x01') {
      uVar2 = *(uint *)(param_1 + 8) | 0x40;
    }
    else {
      uVar2 = *(uint *)(param_1 + 8) & 0xffffffbf;
    }
    *(uint *)(param_1 + 8) = uVar2;
    if (*(char *)((int)param_3 + 0x19) == '\x01') {
      uVar2 = *(uint *)(param_1 + 8) | 0x80;
    }
    else {
      uVar2 = *(uint *)(param_1 + 8) & 0xffffff7f;
    }
    *(uint *)(param_1 + 8) = uVar2;
    if (*(char *)((int)param_3 + 0x1a) == '\0') {
      uVar2 = *(uint *)(param_1 + 8) & 0xffffffef;
    }
    else {
      uVar2 = *(uint *)(param_1 + 8) | 0x10;
    }
    *(uint *)(param_1 + 8) = uVar2;
    return;
  }
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}


// ======== FUN_08013ec4 @ 08013ec4 size=32 ========

void FUN_08013ec4(int param_1,int param_2,uint param_3)

{
  int iVar1;
  
  iVar1 = FUN_08013f5c();
  if (iVar1 != 0) {
    param_1 = param_1 + param_2 * 0x14;
    *(uint *)(param_1 + 8) = *(uint *)(param_1 + 8) | param_3;
    return;
  }
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}


// ======== FUN_08013ee4 @ 08013ee4 size=12 ========

void FUN_08013ee4(int param_1,uint param_2,int param_3)

{
  *(uint *)(param_1 + 4) = *(uint *)(param_1 + 4) | param_3 << ((param_2 & 0x3f) << 2);
  return;
}


// ======== FUN_08013ef0 @ 08013ef0 size=74 ========

undefined4 FUN_08013ef0(uint *param_1,uint param_2,int param_3)

{
  uint uVar1;
  uint uVar2;
  
  uVar2 = param_3 << ((param_2 & 0x3f) << 2);
  if (param_3 == 2) {
    uVar2 = *param_1 & uVar2;
    uVar1 = param_1[param_2 * 5 + 2] & 2;
  }
  else if (param_3 == 4) {
    uVar2 = *param_1 & uVar2;
    uVar1 = param_1[param_2 * 5 + 2] & 4;
  }
  else {
    if (param_3 != 8) {
      do {
                    /* WARNING: Do nothing block with infinite loop */
      } while( true );
    }
    uVar2 = *param_1 & uVar2;
    uVar1 = param_1[param_2 * 5 + 2] & 8;
  }
  if ((uVar2 != 0) && (uVar1 != 0)) {
    return 1;
  }
  return 0;
}


// ======== FUN_08013f3a @ 08013f3a size=32 ========

void FUN_08013f3a(int param_1,int param_2)

{
  int iVar1;
  
  iVar1 = FUN_08013f5c();
  if (iVar1 != 0) {
    param_1 = param_1 + param_2 * 0x14;
    *(uint *)(param_1 + 8) = *(uint *)(param_1 + 8) & 0xffffbfff;
    return;
  }
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}


// ======== FUN_08013f5c @ 08013f5c size=18 ========

undefined4 FUN_08013f5c(int param_1,uint param_2)

{
  undefined4 uVar1;
  
  uVar1 = 1;
  if ((param_1 == DAT_08013f70) && (4 < param_2)) {
    uVar1 = 0;
  }
  return uVar1;
}


// ======== FUN_08013f74 @ 08013f74 size=30 ========

void FUN_08013f74(int param_1,int param_2,uint param_3)

{
  int iVar1;
  
  iVar1 = FUN_08013f5c();
  if (iVar1 != 0) {
    *(uint *)(param_1 + param_2 * 0x14 + 0xc) = param_3 & 0xffff;
    return;
  }
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}


// ======== FUN_08013fc8 @ 08013fc8 size=8 ========

void FUN_08013fc8(void)

{
  FUN_08017c58(0x300);
  return;
}


// ======== FUN_08013fd0 @ 08013fd0 size=170 ========

undefined4 FUN_08013fd0(undefined4 param_1,undefined4 param_2,undefined4 param_3,uint param_4)

{
  undefined4 uVar1;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  uint local_14;
  
  uVar1 = DAT_0801407c;
  local_20 = param_1;
  local_1c = param_2;
  local_18 = param_3;
  local_14 = param_4;
  FUN_0801aef0(DAT_0801407c);
  FUN_08019148(0x703);
  local_18 = 999;
  local_20 = 0x77;
  local_1c = 0;
  local_14 = local_14 & 0xffffff00;
  FUN_0801af7c(uVar1,&local_20);
  FUN_08017bc4(0x32,3,0);
  FUN_0801b020(uVar1,1);
  FUN_0801b018(uVar1,1);
  FUN_0801af72(uVar1);
  uVar1 = DAT_08014080;
  FUN_0801aef0(DAT_08014080);
  FUN_08019148(0x704);
  local_18 = 49999;
  local_20 = 0x77;
  local_1c = 0;
  local_14 = local_14 & 0xffffff00;
  FUN_0801af7c(uVar1,&local_20);
  FUN_08017bc4(0x36,1,0);
  FUN_0801b020(uVar1,1);
  FUN_0801b018(uVar1,1);
  return 0;
}


// ======== FUN_08014084 @ 08014084 size=6 ========

void FUN_08014084(undefined4 param_1)

{
  *DAT_0801408c = param_1;
  return;
}


// ======== FUN_08014090 @ 08014090 size=140 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined1 FUN_08014090(int param_1)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  undefined1 uVar4;
  
  if (param_1 == 0) {
    if ((code *)*DAT_0801411c == (code *)0x0) {
      FUN_080141e8(0,&LAB_08014184,s____src_platform_easylogger_elog__08014160,DAT_08014120,0x1ab,
                   s___s__has_assert_failed_at__s__ld_0801413b + 1,s_tag______void___0__08014124,
                   DAT_08014120,0x1ab);
      do {
                    /* WARNING: Do nothing block with infinite loop */
      } while( true );
    }
    (*(code *)*DAT_0801411c)(s_tag______void___0__08014124,DAT_08014120);
  }
  iVar1 = _DAT_08014138;
  uVar3 = 0;
  uVar4 = 5;
  if (*(char *)(_DAT_08014138 + 100) != '\0') {
    FUN_080145d0();
    do {
      iVar2 = uVar3 * 0xb + iVar1;
      if ((*(char *)(iVar2 + 0x1d) == '\x01') &&
         (iVar2 = FUN_080109be(param_1,iVar2 + 0x14,8), iVar2 == 0)) {
        uVar4 = *(undefined1 *)(uVar3 * 0xb + iVar1 + 0x13);
        break;
      }
      uVar3 = uVar3 + 1 & 0xff;
    } while (uVar3 < 5);
    FUN_08014564();
  }
  return uVar4;
}


// ======== FUN_0801418c @ 0801418c size=86 ========

int FUN_0801418c(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  
  iVar1 = DAT_080141e4;
  iVar2 = 0;
  if ((*(char *)(DAT_080141e4 + 100) != '\x01') && (iVar2 = FUN_0801459c(0), iVar2 == 0)) {
    FUN_08014538(1);
    *(undefined1 *)(iVar1 + 0x67) = 0;
    *(undefined1 *)(iVar1 + 0x68) = 0;
    FUN_080145f8(5);
    uVar4 = 0;
    do {
      iVar3 = uVar4 * 0xb + iVar1;
      *(undefined4 *)(iVar3 + 0x14) = 0;
      *(undefined4 *)(iVar3 + 0x18) = 0;
      *(undefined1 *)(iVar3 + 0x1c) = 0;
      *(undefined1 *)(iVar3 + 0x13) = 0;
      uVar4 = uVar4 + 1 & 0xff;
      *(undefined1 *)(iVar3 + 0x1d) = 0;
    } while (uVar4 < 5);
    *(undefined1 *)(iVar1 + 100) = 1;
  }
  return iVar2;
}


// ======== FUN_080141e8 @ 080141e8 size=658 ========

void FUN_080141e8(uint param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                 undefined4 param_5,undefined4 param_6)

{
  byte *pbVar1;
  byte *pbVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  undefined4 uVar6;
  undefined *puVar7;
  int iVar8;
  byte *pbVar9;
  undefined4 local_44;
  undefined4 local_40;
  undefined4 local_3c;
  undefined4 local_38;
  
  uVar3 = FUN_08010982(param_2);
  iVar8 = 0;
  local_44 = 0;
  local_40 = 0;
  local_3c = 0;
  local_38 = 0;
  if (5 < param_1) {
    if ((code *)*DAT_0801447c == (code *)0x0) {
      FUN_080141e8(0,&DAT_080144fc,s____src_platform_easylogger_elog__080144d8,DAT_08014480,0x209,
                   s___s__has_assert_failed_at__s__ld_080144b4,s_level_<__ELOG_LVL_VERBOSE_08014484,
                   DAT_08014480,0x209);
      do {
                    /* WARNING: Do nothing block with infinite loop */
      } while( true );
    }
    (*(code *)*DAT_0801447c)(s_level_<__ELOG_LVL_VERBOSE_08014484,DAT_08014480);
  }
  pbVar1 = DAT_080144a0;
  if (DAT_080144a0[0x65] == 0) {
    return;
  }
  if (*DAT_080144a0 < param_1) {
    return;
  }
  uVar4 = FUN_08014090(param_2);
  if (uVar4 < param_1) {
    return;
  }
  iVar5 = FUN_0801095e(param_2,pbVar1 + 1);
  if (iVar5 == 0) {
    return;
  }
  FUN_08014518();
  iVar5 = FUN_08016ae4(param_1,1);
  pbVar9 = pbVar1 + -0x100;
  if (iVar5 != 0) {
    iVar8 = FUN_080147f8(0,pbVar9,DAT_0801447c[param_1 + 1]);
  }
  iVar5 = FUN_08016ae4(param_1,2);
  if (iVar5 != 0) {
    iVar5 = FUN_080147f8(iVar8,pbVar9 + iVar8,param_2);
    iVar8 = iVar8 + iVar5;
    if (uVar3 < 5) {
      FUN_0801093a(&local_3c,4 - uVar3,0x20);
      iVar5 = FUN_080147f8(iVar8,pbVar9 + iVar8,&local_3c);
      iVar8 = iVar8 + iVar5;
    }
    iVar5 = FUN_080147f8(iVar8,pbVar9 + iVar8,&DAT_080144a4);
    iVar8 = iVar8 + iVar5;
  }
  iVar5 = FUN_08016ae4(param_1,0x1c);
  if (iVar5 != 0) {
    iVar5 = FUN_080147f8(iVar8,pbVar9 + iVar8,&DAT_080144a8);
    iVar8 = iVar8 + iVar5;
    iVar5 = FUN_08016ae4(param_1,4);
    if (iVar5 != 0) {
      uVar6 = FUN_08014594();
      iVar5 = FUN_080147f8(iVar8,pbVar9 + iVar8,uVar6);
      iVar8 = iVar8 + iVar5;
      iVar5 = FUN_08016ae4(param_1,0x18);
      if (iVar5 != 0) {
        iVar5 = FUN_080147f8(iVar8,pbVar9 + iVar8,&DAT_080144a4);
        iVar8 = iVar8 + iVar5;
      }
    }
    iVar5 = FUN_08016ae4(param_1,8);
    if (iVar5 != 0) {
      uVar6 = FUN_08014584();
      iVar5 = FUN_080147f8(iVar8,pbVar9 + iVar8,uVar6);
      iVar8 = iVar8 + iVar5;
      iVar5 = FUN_08016ae4(param_1,0x10);
      if (iVar5 != 0) {
        iVar5 = FUN_080147f8(iVar8,pbVar9 + iVar8,&DAT_080144a4);
        iVar8 = iVar8 + iVar5;
      }
    }
    iVar5 = FUN_08016ae4(param_1,0x10);
    if (iVar5 != 0) {
      uVar6 = FUN_0801458c();
      iVar5 = FUN_080147f8(iVar8,pbVar9 + iVar8,uVar6);
      iVar8 = iVar8 + iVar5;
    }
    iVar5 = FUN_080147f8(iVar8,pbVar9 + iVar8,&DAT_080144ac);
    iVar8 = iVar8 + iVar5;
  }
  iVar5 = FUN_08016ae4(param_1,0xe0);
  if (iVar5 != 0) {
    iVar5 = FUN_080147f8(iVar8,pbVar9 + iVar8,&DAT_080144b0);
    iVar8 = iVar8 + iVar5;
    iVar5 = FUN_08016ae4(param_1,0x20);
    if (iVar5 != 0) {
      iVar5 = FUN_080147f8(iVar8,pbVar9 + iVar8,param_3);
      iVar8 = iVar8 + iVar5;
      iVar5 = FUN_08016ae4(param_1,0x40);
      if (iVar5 == 0) {
        iVar5 = FUN_08016ae4(param_1,0x80);
        if (iVar5 == 0) goto LAB_080143c4;
        puVar7 = &DAT_08014504;
      }
      else {
        puVar7 = &DAT_080144a4;
      }
      iVar5 = FUN_080147f8(iVar8,pbVar9 + iVar8,puVar7);
      iVar8 = iVar8 + iVar5;
    }
LAB_080143c4:
    iVar5 = FUN_08016ae4(param_1,0x40);
    if (iVar5 != 0) {
      iVar5 = FUN_080147f8(iVar8,pbVar9 + iVar8,param_4);
      iVar8 = iVar8 + iVar5;
      iVar5 = FUN_08016ae4(param_1,0x80);
      if (iVar5 != 0) {
        iVar5 = FUN_080147f8(iVar8,pbVar9 + iVar8,&DAT_08014504);
        iVar8 = iVar8 + iVar5;
      }
    }
    iVar5 = FUN_08016ae4(param_1,0x80);
    if (iVar5 != 0) {
      FUN_08011d88(&local_44,5,&DAT_08014508,param_5);
      iVar5 = FUN_080147f8(iVar8,pbVar9 + iVar8,&local_44);
      iVar8 = iVar8 + iVar5;
    }
    iVar5 = FUN_080147f8(iVar8,pbVar9 + iVar8,&DAT_0801450c);
    iVar8 = iVar8 + iVar5;
  }
  iVar5 = FUN_08011de0(pbVar9 + iVar8,0x100 - iVar8,param_6,&stack0x00000008);
  pbVar2 = DAT_080144a0;
  uVar3 = iVar8 + iVar5;
  if (((0x100 < uVar3) || (iVar5 < 0)) || (0x100 < uVar3 + 2)) {
    uVar3 = 0xfe;
  }
  if (pbVar1[10] != 0) {
    pbVar9[uVar3] = 0;
    iVar8 = FUN_0801095e(pbVar2 + -0x100);
    if (iVar8 == 0) goto LAB_0801446c;
  }
  iVar8 = FUN_080147f8(uVar3,pbVar9 + uVar3,&DAT_08014510);
  FUN_080145c4(DAT_08014514,iVar8 + uVar3);
LAB_0801446c:
  FUN_08014564();
  return;
}


// ======== FUN_08014518 @ 08014518 size=28 ========

void FUN_08014518(void)

{
  int iVar1;
  
  iVar1 = DAT_08014534;
  if (*(char *)(DAT_08014534 + 0x66) != '\0') {
    FUN_080145d0();
    *(undefined1 *)(iVar1 + 0x68) = 1;
    return;
  }
  *(undefined1 *)(DAT_08014534 + 0x67) = 1;
  return;
}


// ======== FUN_08014538 @ 08014538 size=40 ========

void FUN_08014538(int param_1)

{
  int iVar1;
  
  iVar1 = DAT_08014560;
  *(char *)(DAT_08014560 + 0x66) = (char)param_1;
  if (param_1 != 0) {
    if (*(char *)(iVar1 + 0x68) == '\0') {
      if (*(char *)(DAT_08014560 + 0x67) != '\0') {
        FUN_080145d0();
        return;
      }
    }
    else if (*(char *)(DAT_08014560 + 0x67) == '\0') {
      FUN_080145e4();
      return;
    }
  }
  return;
}


// ======== FUN_08014564 @ 08014564 size=28 ========

void FUN_08014564(void)

{
  int iVar1;
  
  iVar1 = DAT_08014580;
  if (*(char *)(DAT_08014580 + 0x66) != '\0') {
    FUN_080145e4();
    *(undefined1 *)(iVar1 + 0x68) = 0;
    return;
  }
  *(undefined1 *)(DAT_08014580 + 0x67) = 0;
  return;
}


// ======== FUN_08014584 @ 08014584 size=4 ========

undefined * FUN_08014584(void)

{
  return &DAT_08014588;
}


// ======== FUN_0801458c @ 0801458c size=4 ========

undefined * FUN_0801458c(void)

{
  return &DAT_08014590;
}


// ======== FUN_08014594 @ 08014594 size=4 ========

undefined1 * FUN_08014594(void)

{
  return &LAB_08014598;
}


// ======== FUN_0801459c @ 0801459c size=30 ========

undefined4 FUN_0801459c(void)

{
  int iVar1;
  
  iVar1 = FUN_0801bc8c(1);
  *DAT_080145bc = iVar1;
  if (iVar1 != 0) {
    FUN_08014084(DAT_080145c0);
    return 0;
  }
  return 1;
}


// ======== FUN_080145c4 @ 080145c4 size=10 ========

void FUN_080145c4(undefined4 param_1,undefined4 param_2)

{
  FUN_0801196c(0,param_1,param_2);
  return;
}


// ======== FUN_080145d0 @ 080145d0 size=14 ========

void FUN_080145d0(void)

{
  FUN_0801bd28(*DAT_080145e0,0,0xffffffff);
  return;
}


// ======== FUN_080145e4 @ 080145e4 size=14 ========

void FUN_080145e4(void)

{
  FUN_0801bf78(*DAT_080145f4,0,0);
  return;
}


// ======== FUN_080145f8 @ 080145f8 size=64 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_080145f8(uint param_1)

{
  if (5 < param_1) {
    if ((code *)*DAT_08014638 == (code *)0x0) {
      FUN_080141e8(0,&DAT_080146a8,s____src_platform_easylogger_elog__08014684,DAT_0801463c,0x125,
                   s___s__has_assert_failed_at__s__ld_0801465f + 1,
                   s_level_<__ELOG_LVL_VERBOSE_08014640,DAT_0801463c,0x125);
      do {
                    /* WARNING: Do nothing block with infinite loop */
      } while( true );
    }
    (*(code *)*DAT_08014638)(s_level_<__ELOG_LVL_VERBOSE_08014640,DAT_0801463c);
  }
  *_DAT_0801465c = (char)param_1;
  return;
}


// ======== FUN_080146b0 @ 080146b0 size=70 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_080146b0(uint param_1,undefined4 param_2)

{
  if (5 < param_1) {
    if ((code *)*DAT_080146f8 == (code *)0x0) {
      FUN_080141e8(0,&DAT_08014768,s____src_platform_easylogger_elog__08014744,DAT_080146fc,0x10b,
                   s___s__has_assert_failed_at__s__ld_0801471f + 1,
                   s_level_<__ELOG_LVL_VERBOSE_08014700,DAT_080146fc,0x10b);
      do {
                    /* WARNING: Do nothing block with infinite loop */
      } while( true );
    }
    (*(code *)*DAT_080146f8)(s_level_<__ELOG_LVL_VERBOSE_08014700,DAT_080146fc);
  }
  *(undefined4 *)(_DAT_0801471c + param_1 * 4 + 0x4c) = param_2;
  return;
}


// ======== FUN_080147f8 @ 080147f8 size=146 ========

int FUN_080147f8(uint param_1,char *param_2,char *param_3)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  char *pcVar3;
  
  puVar2 = DAT_08014890;
  uVar1 = DAT_0801488c;
  if (param_2 == (char *)0x0) {
    if ((code *)*DAT_08014890 == (code *)0x0) {
      FUN_080141e8(0,&DAT_080148e4,s____src_platform_easylogger_elog__080148bc,DAT_0801488c,0x2c,
                   s___s__has_assert_failed_at__s__ld_08014898,&DAT_08014894,DAT_0801488c,0x2c);
      do {
                    /* WARNING: Do nothing block with infinite loop */
      } while( true );
    }
    (*(code *)*DAT_08014890)(&DAT_08014894,DAT_0801488c,0x2c);
  }
  pcVar3 = param_3;
  if (param_3 == (char *)0x0) {
    if ((code *)*puVar2 == (code *)0x0) {
      FUN_080141e8(0,&DAT_080148e4,s____src_platform_easylogger_elog__080148bc,DAT_0801488c,0x2d,
                   s___s__has_assert_failed_at__s__ld_08014898,&LAB_080148ec,uVar1,0x2d);
      do {
                    /* WARNING: Do nothing block with infinite loop */
      } while( true );
    }
    (*(code *)*puVar2)(&LAB_080148ec,DAT_0801488c,0x2d);
  }
  while ((*pcVar3 != '\0' && (param_1 < 0x100))) {
    *param_2 = *pcVar3;
    pcVar3 = pcVar3 + 1;
    param_2 = param_2 + 1;
    param_1 = param_1 + 1;
  }
  return (int)pcVar3 - (int)param_3;
}


// ======== FUN_0801499c @ 0801499c size=2 ========

void FUN_0801499c(void)

{
  return;
}


// ======== FUN_0801499e @ 0801499e size=100 ========

int * FUN_0801499e(int *param_1,int param_2)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  
  if ((param_1 != (int *)0x0) && (param_2 != 0)) {
    if ((param_1[1] == *param_1) && (*param_1 != 0)) {
      FUN_08013d30(param_1,0);
    }
    piVar1 = (int *)FUN_080195b8(0xc);
    if (piVar1 != (int *)0x0) {
      iVar2 = FUN_080195b8(param_1[2]);
      *piVar1 = iVar2;
      FUN_08010912(iVar2,param_2,param_1[2]);
      if (*piVar1 != 0) {
        iVar2 = param_1[1];
        iVar3 = param_1[3];
        piVar1[1] = iVar3;
        if (iVar2 == 0) {
          param_1[3] = (int)piVar1;
          param_1[4] = (int)piVar1;
        }
        else {
          *(int **)(iVar3 + 8) = piVar1;
          param_1[3] = (int)piVar1;
        }
        param_1[1] = param_1[1] + 1;
        return piVar1;
      }
      FUN_080195ac(piVar1);
    }
  }
  return (int *)0x0;
}


// ======== FUN_08014a02 @ 08014a02 size=138 ========

int FUN_08014a02(int *param_1,uint param_2)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  
  if ((((param_1 != (int *)0x0) && (iVar2 = *param_1, iVar2 != 0)) &&
      ((uVar3 = param_1[1], uVar3 == 0 || ((uint)param_1[2] < uVar3)))) && (param_2 < 0x80000000)) {
    uVar1 = param_1[2] + param_2 + 1;
    if (uVar1 <= uVar3) {
      return iVar2 + param_1[2];
    }
    if (param_1[4] == 0) {
      if (uVar1 < 0x40000000) {
        iVar4 = uVar1 * 2;
      }
      else {
        if (0x7fffffff < uVar1) {
          return 0;
        }
        iVar4 = 0x7fffffff;
      }
      if ((code *)param_1[8] == (code *)0x0) {
        iVar2 = (*(code *)param_1[6])(iVar4);
        if (iVar2 != 0) {
          FUN_08010912(iVar2,*param_1,param_1[2] + 1);
          (*(code *)param_1[7])(*param_1);
          goto LAB_08014a76;
        }
      }
      else {
        iVar2 = (*(code *)param_1[8])(iVar2,iVar4);
        if (iVar2 != 0) {
LAB_08014a76:
          *param_1 = iVar2;
          param_1[1] = iVar4;
          return param_1[2] + iVar2;
        }
      }
      (*(code *)param_1[7])(*param_1);
      param_1[1] = 0;
      *param_1 = 0;
    }
  }
  return 0;
}


// ======== FUN_08014ac8 @ 08014ac8 size=158 ========

void FUN_08014ac8(void)

{
  char cVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  code *UNRECOVERED_JUMPTABLE;
  byte bVar5;
  int iVar6;
  int iVar7;
  
  iVar3 = FUN_08016ba8();
  iVar3 = (**(code **)(iVar3 + 8))();
  cVar1 = *(char *)(iVar3 + 1);
  iVar3 = FUN_08016ba8();
  iVar3 = (**(code **)(iVar3 + 8))();
  iVar7 = *(int *)(iVar3 + 4);
  iVar3 = FUN_08016ba8();
  iVar3 = (**(code **)(iVar3 + 0xc))();
  cVar2 = *(char *)(iVar3 + 1);
  iVar3 = FUN_08016ba8();
  iVar3 = (**(code **)(iVar3 + 0xc))();
  iVar6 = *(int *)(iVar3 + 4);
  iVar4 = FUN_08016ab4();
  iVar3 = DAT_08014b68;
  if ((cVar1 == '\x03') || (cVar2 == '\x03')) {
    UNRECOVERED_JUMPTABLE = *(code **)(iVar4 + 0xc);
LAB_08014b2e:
    (*UNRECOVERED_JUMPTABLE)();
    *(undefined1 *)(*(int *)(iVar3 + 4) + 0x17) = 0;
    return;
  }
  if ((cVar1 == '\x01') && (cVar2 == '\0')) {
    if ((29000 < iVar7) && (iVar6 < -29000)) {
      UNRECOVERED_JUMPTABLE = *(code **)(iVar4 + 8);
      goto LAB_08014b2e;
    }
    iVar3 = *(int *)(DAT_08014b68 + 4);
    bVar5 = *(char *)(iVar3 + 0x17) + 1;
    *(byte *)(iVar3 + 0x17) = bVar5;
    if (bVar5 < 0x28) goto LAB_08014b46;
    if (0x4f < bVar5) {
      *(undefined1 *)(iVar3 + 0x17) = 0;
      return;
    }
  }
  else {
    iVar3 = *(int *)(DAT_08014b68 + 4);
    bVar5 = *(char *)(iVar3 + 0x17) + 1;
    *(byte *)(iVar3 + 0x17) = bVar5;
    if (bVar5 < 0x14) {
LAB_08014b46:
      UNRECOVERED_JUMPTABLE = *(code **)(iVar4 + 8);
      goto LAB_08014b54;
    }
    if (0x27 < bVar5) {
      *(undefined1 *)(iVar3 + 0x17) = 0;
      return;
    }
  }
  UNRECOVERED_JUMPTABLE = *(code **)(iVar4 + 0xc);
LAB_08014b54:
                    /* WARNING: Could not recover jumptable at 0x08014b58. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*UNRECOVERED_JUMPTABLE)();
  return;
}


// ======== FUN_08014b6c @ 08014b6c size=788 ========

int FUN_08014b6c(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  int iVar5;
  byte bVar6;
  uint uVar7;
  int iVar8;
  uint uVar9;
  int *piVar10;
  undefined4 local_68;
  undefined4 uStack_64;
  undefined4 uStack_60;
  undefined4 uStack_5c;
  undefined4 local_58;
  undefined4 local_54 [4];
  undefined4 local_44;
  undefined4 local_40;
  undefined4 uStack_3c;
  
  iVar5 = DAT_08014e84;
  if (*(int *)(*(int *)(DAT_08014e84 + 4) + 0x30) == 0) {
    iVar1 = FUN_08013950(0x28,0x10);
    *(int *)(*(int *)(iVar5 + 4) + 0x30) = iVar1;
    if (iVar1 == 0) {
      return -1;
    }
  }
  if (*(int *)(*(int *)(iVar5 + 4) + 0x50) == 0) {
    iVar1 = FUN_08013950(0x28,0x10);
    iVar2 = *(int *)(iVar5 + 4);
    *(int *)(iVar2 + 0x50) = iVar1;
    if (iVar1 == 0) {
      FUN_08013d7c(*(undefined4 *)(iVar2 + 0x30));
      return -1;
    }
  }
  uVar9 = 0;
  local_54[0] = 0;
  local_58 = 0;
  iVar2 = *(int *)(iVar5 + 4);
  iVar1 = -1;
  iVar8 = 0;
  iVar2 = FUN_08015fa8(*(undefined4 *)(iVar2 + 0x48),*(undefined2 *)(iVar2 + 0x92),local_54,
                       *(undefined4 *)(iVar2 + 0x30));
  if (iVar2 == 0) {
    iVar2 = *(int *)(*(int *)(iVar5 + 4) + 0x30);
    if (2 < *(uint *)(iVar2 + 4)) {
      piVar10 = *(int **)(iVar2 + 0x10);
      for (uVar7 = 0; uVar7 < *(int *)(iVar2 + 4) - 1U; uVar7 = uVar7 + 1) {
        if (*(char *)(*piVar10 + 1) == '\0') {
          uVar9 = uVar9 + 1 & 0xff;
        }
        piVar10 = (int *)piVar10[2];
      }
      if ((*(uint *)(iVar2 + 4) >> 2) + 1 < uVar9) {
        iVar8 = 2;
      }
      else if (uVar9 == 0) {
        iVar8 = 0;
      }
      else {
        iVar8 = 1;
      }
      local_44 = 0;
      local_40 = 0;
      uStack_3c = 0;
      iVar2 = FUN_08015260(*(undefined4 *)(*(int *)(iVar5 + 4) + 0x30),local_54[0],&local_44,
                           &local_40,&local_68,iVar8);
      if (iVar2 == 1) {
        iVar1 = *(int *)(iVar5 + 4);
        *(undefined4 *)(iVar1 + 0x78) = local_44;
        *(undefined4 *)(iVar1 + 0x7c) = local_44;
        *(undefined4 *)(iVar1 + 0x88) = local_40;
        *(undefined4 *)(iVar1 + 0x8c) = uStack_3c;
        *(undefined4 *)(iVar1 + 0x38) = local_68;
        *(undefined4 *)(iVar1 + 0x3c) = uStack_64;
        *(undefined4 *)(iVar1 + 0x40) = uStack_60;
        *(undefined4 *)(iVar1 + 0x44) = uStack_5c;
        *(undefined1 *)(iVar1 + 0x4d) = 0;
        *(undefined1 *)(iVar1 + 0xaf) = 0xff;
        iVar1 = 0;
      }
      else {
        iVar3 = *(int *)(iVar5 + 4);
        if (iVar2 != 2) goto LAB_08014cc8;
        *(undefined4 *)(iVar3 + 0x38) = local_68;
        *(undefined4 *)(iVar3 + 0x3c) = uStack_64;
        *(undefined4 *)(iVar3 + 0x40) = uStack_60;
        *(undefined4 *)(iVar3 + 0x44) = uStack_5c;
        *(undefined1 *)(iVar3 + 0x4d) = 0;
        *(undefined1 *)(iVar3 + 0xaf) = 0xff;
      }
    }
  }
  else {
    if (iVar2 == 3) {
      iVar3 = *(int *)(iVar5 + 4);
      if (*(byte *)(iVar3 + 0x4d) < 10) {
        bVar6 = *(byte *)(iVar3 + 0x4d) + 2;
LAB_08014cd2:
        *(byte *)(iVar3 + 0x4d) = bVar6;
        if (bVar6 < 10) goto LAB_08014cdc;
      }
    }
    else if (iVar2 == 5) {
      iVar3 = *(int *)(iVar5 + 4);
LAB_08014cc8:
      if (*(byte *)(iVar3 + 0x4d) < 10) {
        bVar6 = *(byte *)(iVar3 + 0x4d) + 1;
        goto LAB_08014cd2;
      }
    }
    else {
      if (iVar2 != 2) goto LAB_08014cdc;
      iVar3 = *(int *)(iVar5 + 4);
    }
    *(undefined1 *)(iVar3 + 0x39) = 2;
    *(undefined4 *)(iVar3 + 0x3c) = 0;
  }
LAB_08014cdc:
  iVar2 = *(int *)(iVar5 + 4);
  iVar2 = FUN_08015fa8(*(undefined4 *)(iVar2 + 0x68),*(undefined2 *)(iVar2 + 0x92),&local_58,
                       *(undefined4 *)(iVar2 + 0x50));
  if (iVar2 == 0) {
    iVar2 = *(int *)(*(int *)(iVar5 + 4) + 0x50);
    if (*(uint *)(iVar2 + 4) < 3) {
LAB_08014e62:
      if (iVar1 != 0) {
        return iVar1;
      }
    }
    else {
      piVar10 = *(int **)(iVar2 + 0x10);
      if ((iVar8 == 2) || (iVar1 == 0)) {
LAB_08014d56:
        uVar4 = 2;
      }
      else {
        uVar9 = 0;
        for (uVar7 = 0; uVar7 < *(int *)(iVar2 + 4) - 1U; uVar7 = uVar7 + 1) {
          if (*(char *)(*piVar10 + 1) == '\0') {
            uVar9 = uVar9 + 1 & 0xff;
          }
          piVar10 = (int *)piVar10[2];
        }
        if ((*(uint *)(iVar2 + 4) >> 2) + 1 < uVar9) goto LAB_08014d56;
        if ((uVar9 == 0) && (iVar8 != 1)) {
          uVar4 = 0;
        }
        else {
          uVar4 = 1;
        }
      }
      local_40 = 0;
      uStack_3c = 0;
      local_44 = 0;
      iVar2 = FUN_08015260(*(undefined4 *)(*(int *)(iVar5 + 4) + 0x50),local_58,&local_44,&local_40,
                           &local_68,uVar4);
      if (iVar1 != -1) goto LAB_08014e08;
      if (iVar2 != 1) {
        iVar5 = *(int *)(iVar5 + 4);
        if (iVar2 == 2) {
          *(undefined4 *)(iVar5 + 0x58) = local_68;
          *(undefined4 *)(iVar5 + 0x5c) = uStack_64;
          *(undefined4 *)(iVar5 + 0x60) = uStack_60;
          *(undefined4 *)(iVar5 + 100) = uStack_5c;
          *(undefined1 *)(iVar5 + 0x6d) = 0;
          return -1;
        }
        goto LAB_08014e4c;
      }
      iVar1 = *(int *)(iVar5 + 4);
      *(undefined4 *)(iVar1 + 0x78) = local_44;
      *(undefined4 *)(iVar1 + 0x7c) = local_44;
      *(undefined4 *)(iVar1 + 0x88) = local_40;
      *(undefined4 *)(iVar1 + 0x8c) = uStack_3c;
      *(undefined4 *)(iVar1 + 0x58) = local_68;
      *(undefined4 *)(iVar1 + 0x5c) = uStack_64;
      *(undefined4 *)(iVar1 + 0x60) = uStack_60;
      *(undefined4 *)(iVar1 + 100) = uStack_5c;
      *(undefined1 *)(iVar1 + 0x6d) = 0;
      if (*(char *)(iVar1 + 0x39) == '\x03') {
        *(undefined1 *)(iVar1 + 0x39) = 2;
        *(undefined4 *)(iVar1 + 0x3c) = 0;
        *(undefined1 *)(iVar1 + 0x4d) = 0;
      }
      iVar1 = FUN_08015f38(*(undefined4 *)(iVar1 + 0x30));
      if (iVar1 != 0) {
        iVar1 = *(int *)(iVar5 + 4);
        *(undefined1 *)(iVar1 + 0x39) = 2;
        *(undefined4 *)(iVar1 + 0x3c) = 0;
      }
      iVar1 = 0;
    }
    FUN_08013d7c(*(undefined4 *)(*(int *)(iVar5 + 4) + 0x50));
    FUN_08013d7c(*(undefined4 *)(*(int *)(iVar5 + 4) + 0x30));
    iVar5 = *(int *)(iVar5 + 4);
    *(undefined4 *)(iVar5 + 0x30) = 0;
    *(undefined4 *)(iVar5 + 0x50) = 0;
  }
  else {
    if (iVar2 == 3) {
      if (iVar1 != -1) {
LAB_08014e08:
        iVar2 = *(int *)(iVar5 + 4);
        iVar2 = FUN_08015f38(*(undefined4 *)(iVar2 + 0x50),iVar2 + 0x58,
                             *(undefined4 *)(iVar2 + 0x88),*(undefined4 *)(iVar2 + 0x8c),0x32,
                             iVar2 + 0x58);
        if (iVar2 == 0) goto LAB_08014e62;
        iVar2 = *(int *)(iVar5 + 4);
        goto LAB_08014e5c;
      }
      iVar5 = *(int *)(iVar5 + 4);
      if (*(byte *)(iVar5 + 0x6d) < 10) {
        bVar6 = *(byte *)(iVar5 + 0x6d) + 2;
        goto LAB_08014e34;
      }
    }
    else {
      if (iVar2 != 5) {
        if (iVar2 != 2) goto LAB_08014e62;
        iVar2 = *(int *)(iVar5 + 4);
LAB_08014e5c:
        *(undefined1 *)(iVar2 + 0x59) = 2;
        *(undefined4 *)(iVar2 + 0x5c) = 0;
        goto LAB_08014e62;
      }
      if (iVar1 != -1) goto LAB_08014e08;
      iVar5 = *(int *)(iVar5 + 4);
LAB_08014e4c:
      if (*(byte *)(iVar5 + 0x6d) < 10) {
        bVar6 = *(byte *)(iVar5 + 0x6d) + 1;
LAB_08014e34:
        *(byte *)(iVar5 + 0x6d) = bVar6;
        if (bVar6 < 10) {
          return iVar1;
        }
      }
    }
    *(undefined1 *)(iVar5 + 0x59) = 2;
    *(undefined4 *)(iVar5 + 0x5c) = 0;
  }
  return iVar1;
}


// ======== FUN_08014e88 @ 08014e88 size=364 ========

undefined4
FUN_08014e88(int param_1,undefined4 param_2,uint param_3,uint param_4,undefined4 param_5,
            undefined4 *param_6)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  uint extraout_r1;
  uint uVar4;
  undefined4 uVar5;
  uint extraout_r1_00;
  undefined4 uVar6;
  undefined4 uVar7;
  int *piVar8;
  undefined4 *puVar9;
  uint uVar10;
  undefined4 *puVar11;
  undefined4 uVar12;
  char cVar13;
  undefined4 uVar14;
  undefined4 uVar15;
  undefined8 uVar16;
  undefined8 uVar17;
  undefined4 local_58;
  undefined4 local_54;
  
  local_58 = 0;
  piVar8 = *(int **)(param_1 + 0x10);
  uVar16 = FUN_08013950(0,0x10);
  uVar4 = (uint)((ulonglong)uVar16 >> 0x20);
  iVar1 = (int)uVar16;
  if (iVar1 == 0) {
    return 0xffffffff;
  }
  for (uVar10 = 0; uVar10 < *(uint *)(param_1 + 4); uVar10 = uVar10 + 1) {
    iVar2 = *piVar8;
    uVar4 = param_3 - *(uint *)(iVar2 + 8);
    if (param_4 <= *(uint *)(iVar2 + 0xc) &&
        (uint)(*(uint *)(iVar2 + 8) <= param_3) <= param_4 - *(uint *)(iVar2 + 0xc)) {
      local_54 = 0;
      uVar16 = FUN_080179f8(param_3,param_4,*(undefined4 *)(iVar2 + 8),*(undefined4 *)(iVar2 + 0xc),
                            param_2,param_5,&local_58,&local_54);
      uVar4 = (uint)((ulonglong)uVar16 >> 0x20);
      if ((int)uVar16 == 0) {
        FUN_0801499e(iVar1,iVar2);
        uVar4 = extraout_r1;
      }
    }
    piVar8 = (int *)piVar8[2];
  }
  iVar2 = *(int *)(iVar1 + 4);
  uVar12 = 0xffffffff;
  if (iVar2 == 0) goto LAB_08014f98;
  if (iVar2 == 1) {
    puVar9 = (undefined4 *)**(int **)(iVar1 + 0xc);
    uVar12 = *puVar9;
    uVar4 = puVar9[1];
    uVar6 = puVar9[2];
    uVar7 = puVar9[3];
  }
  else {
    if (iVar2 != 2) goto LAB_08014f98;
    puVar9 = (undefined4 *)**(undefined4 **)(iVar1 + 0xc);
    puVar11 = (undefined4 *)**(undefined4 **)(iVar1 + 0x10);
    uVar4 = (uint)*(byte *)((int)puVar11 + 1);
    iVar2 = puVar9[1];
    uVar14 = (undefined4)DAT_08014ff8;
    uVar6 = (undefined4)((ulonglong)DAT_08014ff8 >> 0x20);
    uVar15 = (undefined4)DAT_08015000;
    uVar7 = (undefined4)((ulonglong)DAT_08015000 >> 0x20);
    if (*(byte *)((int)puVar9 + 1) == uVar4) {
      if (iVar2 < 0) {
        iVar2 = -iVar2;
      }
      uVar3 = puVar11[1];
      uVar10 = uVar3;
      if ((int)uVar3 < 0) {
        uVar10 = -uVar3;
      }
      cVar13 = uVar3 < 0x80000000;
      if (uVar10 == 0) goto LAB_08014f98;
      uVar16 = FUN_08010e52();
      uVar17 = FUN_08010e52(iVar2);
      uVar16 = FUN_08010d74((int)uVar17,(int)((ulonglong)uVar17 >> 0x20),(int)uVar16,
                            (int)((ulonglong)uVar16 >> 0x20));
      uVar5 = (undefined4)((ulonglong)uVar16 >> 0x20);
      FUN_08010efc((int)uVar16,uVar5,uVar14,uVar6);
      if ((cVar13 == '\0') &&
         (FUN_08010ecc((int)uVar16,uVar5,uVar15,uVar7), uVar4 = extraout_r1_00, cVar13 == '\0'))
      goto LAB_08014f98;
LAB_08014ff2:
      if (iVar2 <= (int)uVar10) {
LAB_08014f8c:
        uVar12 = *puVar11;
        uVar4 = puVar11[1];
        uVar6 = puVar11[2];
        uVar7 = puVar11[3];
        goto LAB_08014f90;
      }
    }
    else {
      if (iVar2 < 0) {
        iVar2 = -iVar2;
      }
      uVar3 = puVar11[1];
      uVar10 = uVar3;
      if ((int)uVar3 < 0) {
        uVar10 = -uVar3;
      }
      cVar13 = uVar3 < 0x80000000;
      if (uVar10 == 0) goto LAB_08014f98;
      uVar16 = FUN_08010e52();
      uVar17 = FUN_08010e52(iVar2);
      uVar16 = FUN_08010d74((int)uVar17,(int)((ulonglong)uVar17 >> 0x20),(int)uVar16,
                            (int)((ulonglong)uVar16 >> 0x20));
      uVar12 = (undefined4)((ulonglong)uVar16 >> 0x20);
      FUN_08010efc((int)uVar16,uVar12,uVar14,uVar6);
      if ((cVar13 != '\0') || (FUN_08010ecc((int)uVar16,uVar12,uVar15,uVar7), cVar13 != '\0'))
      goto LAB_08014ff2;
      if (*(char *)((int)puVar9 + 1) != '\0') goto LAB_08014f8c;
    }
    uVar12 = *puVar9;
    uVar4 = puVar9[1];
    uVar6 = puVar9[2];
    uVar7 = puVar9[3];
  }
LAB_08014f90:
  *param_6 = uVar12;
  param_6[1] = uVar4;
  param_6[2] = uVar6;
  param_6[3] = uVar7;
  uVar12 = 0;
LAB_08014f98:
  FUN_08013d7c(iVar1,uVar4);
  return uVar12;
}


// ======== FUN_08015008 @ 08015008 size=118 ========

undefined4
FUN_08015008(int param_1,undefined4 param_2,uint param_3,uint param_4,undefined4 param_5,
            undefined4 *param_6)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  uint uVar4;
  undefined4 uVar5;
  undefined4 *puVar6;
  char cVar7;
  uint uVar8;
  undefined4 *puVar9;
  undefined4 local_40;
  undefined4 local_3c;
  int iStack_38;
  undefined4 local_34;
  uint uStack_30;
  uint uStack_2c;
  
  cVar7 = '\0';
  local_40 = 0;
  local_3c = 0;
  puVar6 = *(undefined4 **)(param_1 + 0x10);
  iStack_38 = param_1;
  local_34 = param_2;
  uStack_30 = param_3;
  uStack_2c = param_4;
  for (uVar8 = 0; uVar8 < *(uint *)(param_1 + 4); uVar8 = uVar8 + 1) {
    puVar9 = (undefined4 *)*puVar6;
    uVar4 = puVar9[3];
    if (param_4 <= uVar4 && (uint)((uint)puVar9[2] <= param_3) <= param_4 - uVar4) {
      iVar1 = FUN_080179f8(param_3,param_4,puVar9[2],uVar4,local_34,param_5,&local_40,&local_3c);
      if (iVar1 == 0) {
        uVar3 = *puVar9;
        uVar5 = puVar9[1];
        uVar2 = puVar9[3];
        cVar7 = cVar7 + '\x01';
        param_6[2] = puVar9[2];
        param_6[3] = uVar2;
        *param_6 = uVar3;
        param_6[1] = uVar5;
      }
    }
    puVar6 = (undefined4 *)puVar6[2];
  }
  if (cVar7 == '\x01') {
    uVar2 = 0;
  }
  else {
    uVar2 = 0xffffffff;
  }
  return uVar2;
}


// ======== FUN_08015080 @ 08015080 size=474 ========

undefined4
FUN_08015080(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,int param_5,
            int param_6,int *param_7)

{
  undefined1 *puVar1;
  int iVar2;
  int *piVar3;
  int iVar4;
  undefined4 uVar5;
  uint uVar6;
  int iVar7;
  int iVar8;
  short sVar9;
  int *piVar10;
  int *piVar11;
  int iVar12;
  bool bVar13;
  int local_64;
  int local_60;
  undefined1 uStack_58;
  char local_57;
  undefined1 auStack_48 [16];
  int iStack_38;
  undefined4 uStack_34;
  undefined4 local_30;
  undefined4 uStack_2c;
  
  bVar13 = param_6 == 0;
  do {
    if (bVar13) {
      return 0xffffffff;
    }
    bVar13 = true;
  } while ((param_1 == 0) || (bVar13 = true, param_7 == (int *)0x0));
  iVar7 = 0;
  *param_7 = 0;
  if (*(uint *)(param_1 + 4) < 2) {
    return 0xffffffff;
  }
  iVar8 = -1;
  sVar9 = 0;
  local_60 = -1;
  local_64 = -1;
  piVar3 = *(int **)(param_1 + 0x10);
  piVar11 = (int *)(*(int **)(param_1 + 0x10))[2];
  iStack_38 = param_1;
  uStack_34 = param_2;
  local_30 = param_3;
  uStack_2c = param_4;
  for (uVar6 = 0; uVar6 < *(int *)(param_1 + 4) - 1U; uVar6 = uVar6 + 1) {
    iVar12 = *piVar3;
    iVar2 = *piVar11;
    if (*(ushort *)(iVar2 + 4) < *(ushort *)(iVar12 + 6)) goto LAB_08015250;
    if ((ushort)(*(ushort *)(iVar2 + 4) - *(ushort *)(iVar12 + 6)) < 3) {
      if (sVar9 == 0) {
        iVar2 = FUN_080178d0(iVar12,iVar2,local_30,uStack_2c,&uStack_58);
        if (iVar2 == 0) {
          if (*(char *)(iVar12 + 2) == '\0') {
            iVar8 = 0;
          }
          else {
            iVar8 = -1;
          }
LAB_080151c2:
          sVar9 = 2;
        }
        else if (iVar2 == 2) {
          if (local_64 != -1) goto LAB_080151c2;
          local_64 = 0;
        }
        else {
          sVar9 = 2;
          iVar8 = -1;
        }
      }
      else if (sVar9 == 4) {
        sVar9 = 5;
        iVar4 = FUN_080178d0(iVar12,iVar2,local_30,uStack_2c,auStack_48);
        if (((iVar4 != 0) || (*(char *)(iVar12 + 2) != '\0')) ||
           (local_60 = 0, *(char *)(iVar2 + 2) != '\0')) {
          local_60 = -1;
        }
      }
      else {
        sVar9 = sVar9 + 1;
      }
      piVar10 = (int *)piVar11[2];
    }
    else {
      if (((sVar9 == 2) || (sVar9 == 3)) || (sVar9 == 4)) {
        if (iVar8 == 0) {
LAB_08015152:
          puVar1 = &stack0xffffffd8;
LAB_08015154:
          iVar8 = -1;
          iVar2 = FUN_0801499e(param_6,puVar1 + -0x30);
          if (iVar2 != 0) {
            iVar7 = iVar7 + 1;
          }
        }
      }
      else if ((sVar9 == 5) || (sVar9 == 6)) {
        if (local_60 == 0 && iVar8 == 0) {
          local_60 = -1;
          iVar8 = FUN_0801499e(param_6,&uStack_58);
          if (iVar8 != 0) {
            iVar7 = iVar7 + 1;
          }
          puVar1 = &stack0xffffffe8;
          goto LAB_08015154;
        }
        if (iVar8 == 0) {
          iVar8 = -1;
          if (((param_5 != 0) || (*(ushort *)(*(int *)(DAT_0801525c + 4) + 0x90) < 2)) ||
             (local_57 == '\0')) goto LAB_08015152;
        }
        else if (local_60 == 0) {
          local_60 = -1;
        }
      }
      else {
        iVar8 = -1;
        local_60 = -1;
      }
      sVar9 = 0;
      piVar10 = (int *)piVar11[2];
      local_64 = -1;
    }
    piVar3 = piVar11;
    piVar11 = piVar10;
  }
  if (((sVar9 == 2) || (sVar9 == 3)) || (sVar9 == 4)) {
    if (iVar8 != 0) goto LAB_08015242;
LAB_08015200:
    puVar1 = &stack0xffffffd8;
  }
  else {
    if ((sVar9 == 0) || ((sVar9 != 5 && (sVar9 != 6)))) goto LAB_08015242;
    if (local_60 != 0 || iVar8 != 0) {
      if ((iVar8 != 0) ||
         (((param_5 == 0 && (1 < *(ushort *)(*(int *)(DAT_0801525c + 4) + 0x90))) &&
          (local_57 != '\0')))) goto LAB_08015242;
      goto LAB_08015200;
    }
    iVar8 = FUN_0801499e(param_6,&uStack_58);
    if (iVar8 != 0) {
      iVar7 = iVar7 + 1;
    }
    puVar1 = &stack0xffffffe8;
  }
  iVar8 = FUN_0801499e(param_6,puVar1 + -0x30);
  if (iVar8 != 0) {
    iVar7 = iVar7 + 1;
  }
LAB_08015242:
  *param_7 = iVar7;
  if (iVar7 == 0) {
LAB_08015250:
    uVar5 = 0xffffffff;
  }
  else {
    uVar5 = 0;
  }
  return uVar5;
}


// ======== FUN_08015260 @ 08015260 size=1832 ========

/* WARNING: Removing unreachable block (ram,0x08015694) */
/* WARNING: Type propagation algorithm not settling */

undefined4
FUN_08015260(uint *param_1,undefined4 *param_2,uint *param_3,undefined4 *param_4,undefined4 *param_5
            ,int param_6)

{
  uint uVar1;
  uint uVar2;
  undefined4 *puVar3;
  uint uVar4;
  int iVar5;
  undefined2 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 *puVar10;
  int iVar11;
  int *piVar12;
  undefined4 uVar13;
  int local_7c;
  uint local_78;
  undefined4 *local_74;
  uint local_70;
  uint local_6c;
  int *local_68;
  undefined4 *local_64 [2];
  uint local_5c;
  uint local_58;
  undefined4 *local_54;
  uint *local_50;
  undefined4 *local_4c;
  undefined4 *local_48;
  uint local_44;
  uint local_40;
  uint local_3c;
  uint local_38;
  uint local_34;
  uint local_30 [3];
  
  if ((((param_1 == (uint *)0x0) || (param_3 == (uint *)0x0)) ||
      ((undefined4 *)param_1[1] < (undefined4 *)0xb)) ||
     (((undefined4 *)param_1[1] == param_2 || (param_2 == (undefined4 *)0x0)))) {
    uVar8 = 0;
  }
  else {
    local_68 = (int *)param_1[3];
    uVar8 = 0;
    if (param_6 == 0) {
      uVar2 = 10;
    }
    else if (param_6 == 1) {
      uVar2 = 0x14;
    }
    else {
      uVar2 = 0x19;
    }
    iVar11 = *(int *)(*(int *)(DAT_08015688 + 4) + 0x78);
    local_5c = iVar11 * 6 + 200;
    local_7c = iVar11 << 2;
    if (*(int *)(*(int *)(DAT_08015688 + 4) + 0x7c) == 0) {
      iVar11 = 0x49b4;
      local_5c = DAT_0801568c;
      local_7c = 0x126d0;
    }
    puVar3 = (undefined4 *)0x0;
    while (puVar3 < param_2) {
      puVar10 = (undefined4 *)*local_68;
      local_64[0] = puVar3;
      if (*(char *)((int)puVar10 + 1) == '\0') {
        local_78 = puVar10[2];
        local_74 = (undefined4 *)puVar10[3];
        local_38 = 0;
        local_3c = 0;
        local_50 = (uint *)local_68[1];
        local_48 = (undefined4 *)0x0;
        local_54 = puVar10;
        while (puVar3 = (undefined4 *)((int)puVar3 + 1), puVar3 < (undefined4 *)param_1[1]) {
          local_44 = *local_50;
          local_34 = 0;
          local_30[0] = 0;
          if ((*(char *)(local_44 + 1) == '\0') &&
             (uVar4 = (int)local_74 - *(int *)(local_44 + 0xc),
             uVar1 = (uint)(local_78 < *(uint *)(local_44 + 8)),
             uVar4 == uVar1 &&
             (uint)(local_5c <= local_78 - *(uint *)(local_44 + 8)) <= uVar4 - uVar1)) {
            local_70 = 0xb4;
            local_30[1] = 0;
            uVar13 = 400;
            if (*(int *)(*(int *)(DAT_08015688 + 4) + 0x7c) != 0) {
              uVar13 = 0xfa;
              local_70 = 100;
            }
            iVar5 = FUN_080179f8(*(undefined4 *)(local_44 + 8),*(undefined4 *)(local_44 + 0xc),
                                 local_78,local_74,local_7c,uVar13,local_30,local_30 + 1);
            if ((iVar5 != 0) &&
               (iVar5 = FUN_080179f8(*(undefined4 *)(local_44 + 8),*(undefined4 *)(local_44 + 0xc),
                                     local_78,local_74,iVar11,local_70,&local_34,local_30 + 1),
               iVar5 == 0)) {
              local_78 = *(uint *)(local_44 + 8);
              local_74 = *(undefined4 **)(local_44 + 0xc);
              local_38 = local_38 + 1 & 0xff;
              if (2 < local_38) {
                local_3c = local_34;
                local_48 = (undefined4 *)local_44;
              }
            }
          }
          local_50 = (uint *)local_50[1];
        }
        if (2 < local_38) {
          local_50 = (uint *)0x0;
          local_6c = 0;
          FUN_080179f8(*(undefined4 *)((int)local_48 + 8),*(undefined4 *)((int)local_48 + 0xc),
                       local_54[2],local_54[3],local_3c,0xb4,&local_6c,&local_50);
          if ((local_6c - 0x45cd < 1999) || (local_6c - 0x3662 < 1999)) {
            *param_3 = local_6c;
            uVar9 = 1;
            uVar8 = local_54[3];
            *param_4 = local_54[2];
            param_4[1] = uVar8;
            uVar8 = local_54[1];
            uVar13 = local_54[2];
            uVar7 = local_54[3];
            *param_5 = *local_54;
            param_5[1] = uVar8;
            param_5[2] = uVar13;
            param_5[3] = uVar7;
            uVar6 = 0x14;
            goto LAB_08015974;
          }
        }
      }
      local_68 = (int *)local_68[1];
      puVar3 = (undefined4 *)((int)local_64[0] + 1);
    }
    if (*(int *)(*(int *)(DAT_08015688 + 4) + 0x7c) == 0) {
      local_5c = DAT_08015690;
      local_7c = 0xe924;
      local_68 = (int *)param_1[3];
      iVar11 = 0x3a49;
      puVar3 = (undefined4 *)0x0;
      while (puVar3 < param_2) {
        puVar10 = (undefined4 *)*local_68;
        local_64[0] = puVar3;
        if (*(char *)((int)puVar10 + 1) == '\0') {
          local_78 = puVar10[2];
          local_74 = (undefined4 *)puVar10[3];
          local_3c = 0;
          local_40 = 0;
          local_50 = (uint *)local_68[1];
          local_48 = (undefined4 *)0x0;
          local_54 = puVar10;
          while (puVar3 = (undefined4 *)((int)puVar3 + 1), puVar3 < (undefined4 *)param_1[1]) {
            local_44 = *local_50;
            local_34 = 0;
            local_30[0] = 0;
            if ((*(char *)(local_44 + 1) == '\0') &&
               (uVar4 = (int)local_74 - *(int *)(local_44 + 0xc),
               uVar1 = (uint)(local_78 < *(uint *)(local_44 + 8)),
               uVar4 == uVar1 &&
               (uint)(local_5c <= local_78 - *(uint *)(local_44 + 8)) <= uVar4 - uVar1)) {
              local_70 = 0xb4;
              local_30[1] = 0;
              uVar13 = 400;
              if (*(int *)(*(int *)(DAT_08015688 + 4) + 0x7c) != 0) {
                uVar13 = 0xfa;
                local_70 = 100;
              }
              iVar5 = FUN_080179f8(*(undefined4 *)(local_44 + 8),*(undefined4 *)(local_44 + 0xc),
                                   local_78,local_74,0xe924,uVar13,local_30,local_30 + 1);
              if ((iVar5 != 0) &&
                 (iVar5 = FUN_080179f8(*(undefined4 *)(local_44 + 8),*(undefined4 *)(local_44 + 0xc)
                                       ,local_78,local_74,0x3a49,local_70,&local_34,local_30 + 1),
                 iVar5 == 0)) {
                local_78 = *(uint *)(local_44 + 8);
                local_74 = *(undefined4 **)(local_44 + 0xc);
                local_3c = local_3c + 1 & 0xff;
                if (2 < local_3c) {
                  local_40 = local_34;
                  local_48 = (undefined4 *)local_44;
                }
              }
            }
            local_50 = (uint *)local_50[1];
          }
          if (2 < local_3c) {
            local_50 = (uint *)0x0;
            local_58 = 0;
            FUN_080179f8(*(undefined4 *)((int)local_48 + 8),*(undefined4 *)((int)local_48 + 0xc),
                         local_54[2],local_54[3],local_40,0xb4,&local_58,&local_50);
            if ((local_58 - 0x45cd < 1999) || (local_58 - 0x3662 < 1999)) {
              *param_3 = local_58;
              uVar9 = 1;
              uVar8 = local_54[3];
              *param_4 = local_54[2];
              param_4[1] = uVar8;
              uVar8 = local_54[1];
              uVar13 = local_54[2];
              uVar7 = local_54[3];
              *param_5 = *local_54;
              param_5[1] = uVar8;
              param_5[2] = uVar13;
              param_5[3] = uVar7;
              goto LAB_08015946;
            }
          }
        }
        local_68 = (int *)local_68[1];
        puVar3 = (undefined4 *)((int)local_64[0] + 1);
      }
    }
    if (*(int *)(*(int *)(DAT_08015688 + 4) + 0x7c) == 0) {
      iVar11 = 0x49b4;
      local_5c = DAT_0801568c;
      local_7c = 0x126d0;
    }
    uVar4 = param_1[1];
    if (uVar2 < uVar4) {
      local_54 = param_2;
      if (uVar4 != *param_1) {
        local_54 = (undefined4 *)(uVar4 - 2);
      }
      local_68 = (int *)param_1[3];
      for (puVar3 = (undefined4 *)0x0; puVar3 < local_54; puVar3 = (undefined4 *)((int)puVar3 + 1))
      {
        local_48 = (undefined4 *)*local_68;
        if ((*(char *)((int)local_48 + 1) == '\x01') && (20000 < (int)local_48[1])) {
          local_70 = local_48[2];
          local_74 = (undefined4 *)local_48[3];
          local_44 = 0;
          local_3c = 0;
          local_38 = 0;
          local_50 = (uint *)local_68[1];
          puVar10 = puVar3;
          while (puVar10 = (undefined4 *)((int)puVar10 + 1), puVar10 < (undefined4 *)param_1[1]) {
            local_40 = *local_50;
            local_30[0] = 0;
            local_30[1] = 0;
            if (((*(char *)(local_40 + 1) == '\x01') && (20000 < *(int *)(local_40 + 4))) &&
               (uVar4 = (int)local_74 - *(int *)(local_40 + 0xc),
               uVar1 = (uint)(local_70 < *(uint *)(local_40 + 8)),
               uVar4 == uVar1 &&
               (uint)(local_5c <= local_70 - *(uint *)(local_40 + 8)) <= uVar4 - uVar1)) {
              local_30[2] = 0;
              local_64[0] = (undefined4 *)0xb4;
              uVar13 = 400;
              if (*(int *)(*(int *)(DAT_08015688 + 4) + 0x7c) != 0) {
                uVar13 = 0x96;
                local_64[0] = (undefined4 *)0x32;
              }
              iVar5 = FUN_080179f8(*(undefined4 *)(local_40 + 8),*(undefined4 *)(local_40 + 0xc),
                                   local_70,local_74,local_7c,uVar13,local_30 + 1,local_30 + 2);
              if ((iVar5 != 0) &&
                 (iVar5 = FUN_080179f8(*(undefined4 *)(local_40 + 8),*(undefined4 *)(local_40 + 0xc)
                                       ,local_70,local_74,iVar11,local_64[0],local_30,local_30 + 2),
                 iVar5 == 0)) {
                local_70 = *(uint *)(local_40 + 8);
                local_74 = *(undefined4 **)(local_40 + 0xc);
                local_3c = local_3c + 1 & 0xff;
                if (2 < local_3c) {
                  local_38 = local_30[0];
                  local_44 = local_40;
                }
              }
            }
            local_50 = (uint *)local_50[1];
          }
          if (2 < local_3c) {
            local_50 = (uint *)0x0;
            local_64[0] = (undefined4 *)0x0;
            FUN_080179f8(*(undefined4 *)(local_44 + 8),*(undefined4 *)(local_44 + 0xc),local_48[2],
                         local_48[3],local_38,0xb4,local_64,&local_50);
            if (*(int *)(*(int *)(DAT_08015998 + 4) + 0x7c) == 0) {
              if (((int)local_64[0] - 0x45cdU < 1999) || ((int)local_64[0] - 0x3662U < 1999)) {
                *param_3 = (uint)local_64[0];
                uVar8 = local_48[3];
                *param_4 = local_48[2];
                param_4[1] = uVar8;
                uVar8 = local_48[1];
                uVar13 = local_48[2];
                uVar7 = local_48[3];
                *param_5 = *local_48;
                param_5[1] = uVar8;
                param_5[2] = uVar13;
                param_5[3] = uVar7;
                uVar8 = 2;
                *(undefined2 *)(*(int *)(DAT_08015998 + 4) + 0x9c) = 0x14;
                break;
              }
            }
            else if (((int)local_64[0] - 0x45cdU < 1999) || ((int)local_64[0] - 0x3662U < 1999)) {
              puVar10 = *(undefined4 **)(*(int *)(DAT_08015998 + 4) + 0x78);
              if (puVar10 < local_64[0]) {
                uVar4 = (int)local_64[0] - (int)puVar10;
              }
              else {
                uVar4 = (int)puVar10 - (int)local_64[0];
              }
              if (uVar4 < 10) {
                *param_3 = (uint)local_64[0];
                uVar8 = local_48[3];
                *param_4 = local_48[2];
                param_4[1] = uVar8;
                uVar8 = local_48[1];
                uVar13 = local_48[2];
                uVar7 = local_48[3];
                *param_5 = *local_48;
                param_5[1] = uVar8;
                param_5[2] = uVar13;
                param_5[3] = uVar7;
                uVar8 = 2;
                *(undefined2 *)(*(int *)(DAT_08015998 + 4) + 0x9c) = 0x14;
                break;
              }
            }
          }
        }
        local_68 = (int *)local_68[1];
      }
    }
    if (*(int *)(*(int *)(DAT_08015998 + 4) + 0x7c) == 0) {
      uVar13 = 0x3a49;
      local_5c = DAT_0801599c;
      uVar4 = param_1[1];
      if (uVar2 < uVar4) {
        local_74 = param_2;
        if (uVar4 != *param_1) {
          local_74 = (undefined4 *)(uVar4 - 2);
        }
        local_68 = (int *)param_1[3];
        for (puVar3 = (undefined4 *)0x0; puVar3 < local_74; puVar3 = (undefined4 *)((int)puVar3 + 1)
            ) {
          local_4c = (undefined4 *)*local_68;
          if ((*(char *)((int)local_4c + 1) == '\x01') && (20000 < (int)local_4c[1])) {
            local_70 = local_4c[2];
            local_6c = local_4c[3];
            local_40 = 0;
            local_48 = (undefined4 *)0x0;
            local_44 = 0;
            piVar12 = (int *)local_68[1];
            puVar10 = puVar3;
            while (puVar10 = (undefined4 *)((int)puVar10 + 1), puVar10 < (undefined4 *)param_1[1]) {
              iVar11 = *piVar12;
              local_34 = 0;
              local_30[0] = 0;
              if (((*(char *)(iVar11 + 1) == '\x01') && (20000 < *(int *)(iVar11 + 4))) &&
                 (uVar2 = local_6c - *(int *)(iVar11 + 0xc),
                 uVar4 = (uint)(local_70 < *(uint *)(iVar11 + 8)),
                 uVar2 == uVar4 &&
                 (uint)(local_5c <= local_70 - *(uint *)(iVar11 + 8)) <= uVar2 - uVar4)) {
                local_30[1] = 0;
                local_64[0] = (undefined4 *)0xb4;
                uVar7 = 400;
                if (*(int *)(*(int *)(DAT_08015998 + 4) + 0x7c) != 0) {
                  uVar7 = 0x96;
                  local_64[0] = (undefined4 *)0x32;
                }
                iVar5 = FUN_080179f8(*(undefined4 *)(iVar11 + 8),*(undefined4 *)(iVar11 + 0xc),
                                     local_70,local_6c,0xe924,uVar7,local_30,local_30 + 1,uVar13);
                if ((iVar5 != 0) &&
                   (iVar5 = FUN_080179f8(*(undefined4 *)(iVar11 + 8),*(undefined4 *)(iVar11 + 0xc),
                                         local_70,local_6c,uVar13,local_64[0],&local_34,local_30 + 1
                                        ), iVar5 == 0)) {
                  local_70 = *(uint *)(iVar11 + 8);
                  local_6c = *(uint *)(iVar11 + 0xc);
                  local_40 = local_40 + 1 & 0xff;
                  if (2 < local_40) {
                    local_44 = local_34;
                    local_48 = (undefined4 *)iVar11;
                  }
                }
              }
              piVar12 = (int *)piVar12[1];
            }
            if (2 < local_40) {
              local_54 = (undefined4 *)0x0;
              local_64[0] = (undefined4 *)0x0;
              FUN_080179f8(*(undefined4 *)((int)local_48 + 8),*(undefined4 *)((int)local_48 + 0xc),
                           local_4c[2],local_4c[3],local_44,0xb4,local_64,&local_54);
              if (*(int *)(*(int *)(DAT_08015998 + 4) + 0x7c) == 0) {
                if (((int)local_64[0] - 0x45cdU < 1999) || ((int)local_64[0] - 0x3662U < 1999)) {
                  *param_3 = (uint)local_64[0];
                  uVar9 = 2;
                  uVar8 = local_4c[3];
                  *param_4 = local_4c[2];
                  param_4[1] = uVar8;
                  uVar8 = local_4c[1];
                  uVar13 = local_4c[2];
                  uVar7 = local_4c[3];
                  *param_5 = *local_4c;
                  param_5[1] = uVar8;
                  param_5[2] = uVar13;
                  param_5[3] = uVar7;
LAB_08015946:
                  *(undefined2 *)(*(int *)(DAT_08015998 + 4) + 0x9c) = 0x19;
                  return uVar9;
                }
              }
              else if (((int)local_64[0] - 0x45cdU < 1999) || ((int)local_64[0] - 0x3662U < 1999)) {
                puVar10 = *(undefined4 **)(*(int *)(DAT_08015998 + 4) + 0x78);
                if (puVar10 < local_64[0]) {
                  uVar2 = (int)local_64[0] - (int)puVar10;
                }
                else {
                  uVar2 = (int)puVar10 - (int)local_64[0];
                }
                if (uVar2 < 10) {
                  *param_3 = (uint)local_64[0];
                  uVar9 = 2;
                  uVar8 = local_4c[3];
                  *param_4 = local_4c[2];
                  param_4[1] = uVar8;
                  uVar8 = local_4c[1];
                  uVar13 = local_4c[2];
                  uVar7 = local_4c[3];
                  *param_5 = *local_4c;
                  param_5[1] = uVar8;
                  param_5[2] = uVar13;
                  param_5[3] = uVar7;
                  uVar6 = 0x19;
LAB_08015974:
                  *(undefined2 *)(*(int *)(DAT_08015998 + 4) + 0x9c) = uVar6;
                  return uVar9;
                }
              }
            }
          }
          local_68 = (int *)local_68[1];
        }
      }
    }
  }
  return uVar8;
}


// ======== FUN_080159a0 @ 080159a0 size=704 ========

undefined4 FUN_080159a0(int param_1,uint param_2,int param_3,undefined4 *param_4)

{
  uint uVar1;
  undefined4 *puVar2;
  undefined4 uVar3;
  uint uVar4;
  int iVar5;
  uint uVar6;
  undefined4 *puVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  int iVar10;
  uint uVar11;
  int *piVar12;
  int *piVar13;
  uint local_80;
  uint local_7c;
  uint local_78;
  int iStack_74;
  uint local_6c;
  int local_5c;
  uint local_58;
  undefined4 local_50;
  undefined4 *local_4c;
  undefined4 local_48;
  uint local_44;
  int local_40;
  undefined4 local_3c;
  undefined4 local_38;
  int iStack_34;
  uint uStack_30;
  int local_2c;
  undefined4 *puStack_28;
  
  if (((((param_1 == 0) || (param_3 == 0)) || (param_4 == (undefined4 *)0x0)) ||
      ((uVar1 = *(uint *)(param_1 + 4), uVar1 < 4 || (uVar1 < param_2)))) || (param_2 == 0)) {
    return 1;
  }
  if (uVar1 != param_2) {
    local_5c = 1;
    piVar13 = *(int **)(param_1 + 0xc);
    local_58 = *(int *)(*(int *)(DAT_08015c64 + 4) + 0x78) * 4 - 100;
    iStack_34 = param_1;
    uStack_30 = param_2;
    local_2c = param_3;
    puStack_28 = param_4;
    for (uVar1 = 0; uVar1 < param_2; uVar1 = uVar1 + 1) {
      local_4c = (undefined4 *)*piVar13;
      if (*(char *)((int)local_4c + 1) == '\0') {
        local_80 = local_4c[2];
        local_7c = local_4c[3];
        local_6c = 0;
        piVar12 = (int *)piVar13[1];
        uVar4 = uVar1;
        while (uVar4 = uVar4 + 1, uVar4 < *(uint *)(param_1 + 4)) {
          iVar5 = *piVar12;
          local_50 = 0;
          if ((*(char *)(iVar5 + 1) == '\0') &&
             (uVar6 = local_7c - *(int *)(iVar5 + 0xc),
             uVar11 = (uint)(local_80 < *(uint *)(iVar5 + 8)),
             uVar6 == uVar11 &&
             (uint)(local_58 <= local_80 - *(uint *)(iVar5 + 8)) <= uVar6 - uVar11)) {
            local_48 = 0;
            iVar10 = FUN_080179f8(*(undefined4 *)(iVar5 + 8),*(undefined4 *)(iVar5 + 0xc),local_80,
                                  local_7c,local_2c,0x32,&local_50,&local_48);
            if (iVar10 == 0) {
              local_80 = *(uint *)(iVar5 + 8);
              local_7c = *(uint *)(iVar5 + 0xc);
              local_6c = local_6c + 1 & 0xff;
              if (1 < local_6c) {
                uVar3 = local_4c[1];
                uVar8 = local_4c[2];
                uVar9 = local_4c[3];
                *param_4 = *local_4c;
                param_4[1] = uVar3;
                param_4[2] = uVar8;
                param_4[3] = uVar9;
                local_5c = 0;
              }
            }
          }
          piVar12 = (int *)piVar12[1];
        }
        if (local_5c == 0) {
          return 0;
        }
      }
      piVar13 = (int *)piVar13[1];
    }
    piVar13 = *(int **)(param_1 + 0xc);
    local_80 = 0;
    for (uVar1 = 0; uVar1 < param_2; uVar1 = uVar1 + 1) {
      iVar5 = *piVar13;
      if (*(char *)(iVar5 + 1) == '\x01') {
        local_78 = *(uint *)(iVar5 + 8);
        iStack_74 = *(int *)(iVar5 + 0xc);
        local_44 = 0;
        local_4c = (undefined4 *)*piVar13;
        piVar12 = (int *)piVar13[1];
        local_7c = 0;
        local_40 = -1;
        uVar4 = uVar1;
        while (uVar4 = uVar4 + 1, uVar4 < *(uint *)(param_1 + 4)) {
          iVar5 = *piVar12;
          local_3c = 0;
          if ((*(char *)(iVar5 + 1) == '\x01') &&
             (uVar6 = iStack_74 - *(int *)(iVar5 + 0xc),
             uVar11 = (uint)(local_78 < *(uint *)(iVar5 + 8)),
             uVar6 == uVar11 &&
             (uint)(local_58 <= local_78 - *(uint *)(iVar5 + 8)) <= uVar6 - uVar11)) {
            local_38 = 0;
            iVar10 = FUN_080179f8(*(undefined4 *)(iVar5 + 8),*(undefined4 *)(iVar5 + 0xc),local_78,
                                  iStack_74,*(undefined4 *)(*(int *)(DAT_08015c64 + 4) + 0x78),0x32,
                                  &local_3c,&local_38);
            if (iVar10 == 0) {
              local_78 = *(uint *)(iVar5 + 8);
              iStack_74 = *(int *)(iVar5 + 0xc);
              local_44 = local_44 + 1 & 0xff;
              if (3 < local_44) {
                FUN_080179f8(local_78,iStack_74,local_4c[2],local_4c[3],
                             *(undefined4 *)(*(int *)(DAT_08015c64 + 4) + 0x78),0x32,&local_7c,
                             &local_38);
                uVar6 = *(uint *)(*(int *)(DAT_08015c64 + 4) + 0x78);
                if (uVar6 < local_7c) {
                  uVar6 = local_7c - uVar6;
                }
                else {
                  uVar6 = uVar6 - local_7c;
                }
                if (uVar6 < 10) {
                  local_40 = 0;
                  local_5c = 0;
                }
              }
            }
          }
          piVar12 = (int *)piVar12[1];
        }
        if (local_40 == 0) {
          if (local_80 == 0) {
            local_80 = local_7c;
            uVar3 = local_4c[1];
            uVar8 = local_4c[2];
            uVar9 = local_4c[3];
            *param_4 = *local_4c;
            param_4[1] = uVar3;
            param_4[2] = uVar8;
            param_4[3] = uVar9;
          }
          else {
            uVar4 = *(uint *)(*(int *)(DAT_08015c64 + 4) + 0x78);
            if (uVar4 < local_80) {
              uVar6 = local_80 - uVar4;
            }
            else {
              uVar6 = uVar4 - local_80;
            }
            if (uVar4 < local_7c) {
              uVar4 = local_7c - uVar4;
            }
            else {
              uVar4 = uVar4 - local_7c;
            }
            if (uVar4 < uVar6) {
              uVar3 = local_4c[1];
              uVar8 = local_4c[2];
              uVar9 = local_4c[3];
              *param_4 = *local_4c;
              param_4[1] = uVar3;
              param_4[2] = uVar8;
              param_4[3] = uVar9;
              local_80 = local_7c;
            }
          }
        }
      }
      piVar13 = (int *)piVar13[1];
    }
    if (local_5c == 0) {
      return 0;
    }
    uVar4 = 0;
    uVar1 = 0;
    piVar13 = *(int **)(param_1 + 0x10);
    uVar6 = *(uint *)(param_1 + 4);
    for (uVar11 = 0; uVar11 < uVar6; uVar11 = uVar11 + 1) {
      if (*(char *)(*piVar13 + 1) == '\0') {
        uVar4 = uVar4 + 1 & 0xff;
      }
      else {
        uVar1 = uVar1 + 1 & 0xff;
      }
      piVar13 = (int *)piVar13[2];
    }
    if (uVar6 == uVar4) {
      puVar2 = (undefined4 *)**(int **)(param_1 + 0xc);
      puVar7 = *(undefined4 **)(*(int **)(param_1 + 0xc))[1];
      iVar5 = puVar2[1];
      iVar10 = puVar7[1];
      if (iVar10 < iVar5) {
        uVar8 = *puVar2;
        uVar3 = puVar2[3];
        param_4[2] = puVar2[2];
        param_4[3] = uVar3;
        *param_4 = uVar8;
        param_4[1] = iVar5;
      }
      else {
        uVar3 = puVar7[2];
        uVar8 = puVar7[3];
        *param_4 = *puVar7;
        param_4[1] = iVar10;
        param_4[2] = uVar3;
        param_4[3] = uVar8;
      }
      return 3;
    }
    if (uVar6 == uVar1) {
      puVar7 = (undefined4 *)**(int **)(param_1 + 0xc);
      puVar2 = *(undefined4 **)(*(int **)(param_1 + 0xc))[1];
      iVar5 = puVar7[1];
      iVar10 = puVar2[1];
      if (iVar10 < iVar5) {
        uVar8 = *puVar2;
        uVar3 = puVar2[3];
        param_4[2] = puVar2[2];
        param_4[3] = uVar3;
        *param_4 = uVar8;
        param_4[1] = iVar10;
      }
      else {
        uVar3 = puVar7[3];
        uVar8 = *puVar7;
        param_4[2] = puVar7[2];
        param_4[3] = uVar3;
        *param_4 = uVar8;
        param_4[1] = iVar5;
      }
      return 4;
    }
  }
  return 2;
}


// ======== FUN_08015c68 @ 08015c68 size=352 ========

undefined4 FUN_08015c68(int param_1,int param_2,undefined4 param_3)

{
  short sVar1;
  short sVar2;
  short sVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  ushort *puVar7;
  ushort *puVar8;
  uint uVar9;
  undefined4 uVar10;
  uint uVar11;
  int iVar12;
  int iVar13;
  int iVar14;
  int iVar15;
  uint in_fpscr;
  float fVar16;
  undefined1 local_48;
  undefined1 local_47;
  undefined1 local_46;
  ushort local_44;
  ushort local_42;
  undefined2 local_40;
  float local_3c;
  int local_38;
  int iStack_30;
  int iStack_2c;
  undefined4 local_28;
  
  uVar10 = 0xffffffff;
  if (*(int *)(param_2 + 4) == 2) {
    puVar8 = (ushort *)**(undefined4 **)(param_2 + 0x10);
    puVar7 = (ushort *)**(undefined4 **)(param_2 + 0xc);
    if ((char)puVar8[1] == '\x01') {
      if ((char)puVar7[1] != '\0') {
        return 0xffffffff;
      }
    }
    else {
      if ((char)puVar8[1] != '\0') {
        return 0xffffffff;
      }
      if ((char)puVar7[1] != '\x01') {
        return 0xffffffff;
      }
    }
    uVar4 = (uint)*puVar8;
    if (uVar4 < *puVar7) {
      sVar3 = (*puVar7 - *puVar8) + 1;
      if ((int)sVar3 - 4U < 0x10) {
        uVar11 = 0;
        iVar14 = 0;
        iVar12 = 0;
        uVar9 = 0;
        uVar6 = (uint)*puVar7;
        local_3c = DAT_08015dc8;
        for (uVar5 = uVar4; uVar5 <= uVar6; uVar5 = uVar5 + 1 & 0xffff) {
          local_38 = (int)*(short *)(param_1 + uVar5 * 2);
          if (local_38 != 0) {
            uVar11 = uVar11 + 1 & 0xffff;
          }
          if ((char)puVar8[1] == '\x01') {
            iVar13 = iVar12;
            iVar15 = local_38;
            if (iVar14 < local_38) goto LAB_08015cea;
          }
          else {
            iVar13 = local_38;
            iVar15 = iVar14;
            if (local_38 < iVar12) {
LAB_08015cea:
              fVar16 = (float)VectorSignedToFloat((uVar6 - uVar4) + 1,(byte)(in_fpscr >> 0x16) & 3);
              local_3c = (float)VectorSignedToFloat((uVar5 - uVar4) + 1,(byte)(in_fpscr >> 0x16) & 3
                                                   );
              local_3c = local_3c / fVar16;
              uVar9 = uVar5;
              iVar12 = iVar13;
              iVar14 = iVar15;
            }
          }
        }
        if ((int)sVar3 / 2 <= (int)uVar11) {
          if (((char)puVar8[1] == '\x01') && ((char)puVar7[1] == '\0')) {
            local_46 = 0;
            for (; (int)uVar4 <= (int)(uVar6 - 1); uVar4 = uVar4 + 1 & 0xffff) {
              sVar2 = *(short *)(param_1 + uVar4 * 2);
              sVar1 = *(short *)(param_1 + uVar4 * 2 + 2);
              if (uVar4 < uVar9) {
                if (sVar1 < sVar2) {
LAB_08015d4e:
                  local_46 = 0xff;
                  break;
                }
              }
              else if (sVar2 < sVar1) goto LAB_08015d4e;
            }
            local_48 = 0;
            local_40 = (undefined2)iVar14;
          }
          else {
            local_46 = 0;
            for (; (int)uVar4 <= (int)(uVar6 - 1); uVar4 = uVar4 + 1 & 0xffff) {
              sVar2 = *(short *)(param_1 + uVar4 * 2);
              sVar1 = *(short *)(param_1 + uVar4 * 2 + 2);
              if (uVar4 < uVar9) {
                if (sVar2 < sVar1) {
LAB_08015d7e:
                  local_46 = 0xff;
                  break;
                }
              }
              else if (sVar1 < sVar2) goto LAB_08015d7e;
            }
            local_48 = 1;
            local_40 = (undefined2)iVar12;
          }
          local_47 = (undefined1)sVar3;
          local_44 = *puVar8;
          local_42 = *puVar7;
          iStack_30 = param_1;
          iStack_2c = param_2;
          local_28 = param_3;
          iVar12 = FUN_0801499e(param_3,&local_48);
          if (iVar12 != 0) {
            uVar10 = 0;
          }
        }
      }
    }
  }
  return uVar10;
}


// ======== FUN_08015dcc @ 08015dcc size=364 ========

undefined4
FUN_08015dcc(int param_1,int param_2,int param_3,undefined4 param_4,uint param_5,int param_6,
            undefined4 param_7,undefined4 param_8,int param_9)

{
  short sVar1;
  short sVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  ushort local_3c;
  undefined1 local_3a;
  int iStack_34;
  int iStack_30;
  int local_2c;
  undefined4 uStack_28;
  
  if (((param_1 != 0) && (param_9 != 0)) &&
     (iStack_34 = param_1, iStack_30 = param_2, local_2c = param_3, uStack_28 = param_4,
     iVar4 = FUN_08013950(2,4), iVar4 != 0)) {
    iVar5 = FUN_08013950(0x1e,0x10);
    if (iVar5 != 0) {
      uVar7 = 0;
      uVar6 = 0;
      do {
        if (param_2 - 1U <= uVar6) {
          FUN_08013d7c(iVar4);
          if (1 < *(uint *)(iVar5 + 4)) {
            uVar7 = local_2c * 0x14;
            iVar4 = FUN_08015080(iVar5,param_6,uVar7 + param_5,
                                 param_6 + ((int)uVar7 >> 0x1f) + (uint)CARRY4(uVar7,param_5),
                                 param_7,param_9,param_8);
            if (iVar4 == 0) {
              FUN_08013d7c(iVar5);
              return 0;
            }
          }
          FUN_08013d7c(iVar5);
          return 0xffffffff;
        }
        sVar2 = *(short *)(param_1 + uVar6 * 2);
        local_3c = (ushort)uVar6;
        uVar3 = uVar6;
        if (sVar2 < 1) {
          sVar1 = *(short *)(param_1 + uVar6 * 2 + 2);
          if ((sVar1 < 1) || (uVar7 == uVar6)) {
            if (-1 < sVar2) goto LAB_08015e52;
            if ((sVar1 < 0) || (uVar7 == uVar6 + 1)) goto LAB_08015e8e;
            local_3c = (ushort)(uVar6 + 1);
            local_3a = 1;
LAB_08015e84:
            uVar3 = uVar6 + 1;
          }
          else {
            local_3a = 1;
          }
LAB_08015e8a:
          uVar7 = uVar3 & 0xffff;
          if (*(int *)(iVar4 + 4) == 0) {
            FUN_0801499e(iVar4,&local_3c);
          }
          else if ((uint)*(ushort *)**(undefined4 **)(iVar4 + 0xc) < (uint)local_3c) {
            if (*(ushort *)**(undefined4 **)(iVar4 + 0xc) + 0x32 < (uint)local_3c) {
              FUN_0801364c(iVar4);
            }
            FUN_0801499e(iVar4,&local_3c);
            if (*(uint *)(iVar4 + 4) == 2) {
              FUN_08015c68(param_1,iVar4,iVar5);
              FUN_08013d30(iVar4,0);
            }
            else if (2 < *(uint *)(iVar4 + 4)) {
              FUN_0801364c(iVar4);
            }
          }
        }
        else {
LAB_08015e52:
          sVar1 = *(short *)(param_1 + uVar6 * 2 + 2);
          if ((sVar1 < 0) && (uVar7 != uVar6)) {
            local_3a = 0;
            goto LAB_08015e8a;
          }
          if (((0 < sVar2) && (sVar1 < 1)) && (uVar7 != uVar6 + 1)) {
            local_3c = (ushort)(uVar6 + 1);
            local_3a = 0;
            goto LAB_08015e84;
          }
LAB_08015e8e:
          local_3c = 0;
        }
        uVar6 = uVar6 + 1;
      } while( true );
    }
    FUN_08013d7c(iVar4);
  }
  return 0xffffffff;
}


// ======== FUN_08015f38 @ 08015f38 size=110 ========

undefined8
FUN_08015f38(int param_1,uint param_2,uint param_3,uint param_4,undefined4 param_5,uint *param_6)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  undefined4 *puVar5;
  uint *puVar6;
  undefined4 uVar7;
  
  uVar4 = 0;
  uVar7 = 0xffffffff;
  puVar5 = *(undefined4 **)(param_1 + 0xc);
  do {
    if (*(uint *)(param_1 + 4) <= uVar4) {
LAB_08015f96:
      return CONCAT44(param_2,uVar7);
    }
    puVar6 = (uint *)*puVar5;
    param_2 = puVar6[2];
    uVar1 = puVar6[3];
    if ((param_2 != 0 || uVar1 != 0) && (param_3 != 0 || param_4 != 0)) {
      if (param_4 < uVar1 || uVar1 - param_4 < (uint)(param_3 <= param_2)) {
        uVar3 = param_2 - param_3;
        iVar2 = (uVar1 - param_4) - (uint)(param_2 < param_3);
      }
      else {
        uVar3 = param_3 - param_2;
        iVar2 = (param_4 - uVar1) - (uint)(param_3 < param_2);
      }
      param_2 = 0x32 - uVar3;
      if ((uint)-iVar2 < (uint)(uVar3 < 0x33)) {
        param_2 = *puVar6;
        uVar4 = puVar6[1];
        uVar1 = puVar6[2];
        uVar3 = puVar6[3];
        *param_6 = param_2;
        param_6[1] = uVar4;
        param_6[2] = uVar1;
        param_6[3] = uVar3;
        uVar7 = 0;
        goto LAB_08015f96;
      }
    }
    puVar5 = (undefined4 *)puVar5[1];
    uVar4 = uVar4 + 1;
  } while( true );
}


// ======== FUN_08015fa8 @ 08015fa8 size=224 ========

undefined4 FUN_08015fa8(undefined4 param_1,int param_2)

{
  byte bVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  char cVar5;
  
  iVar2 = FUN_080195b8(param_2 << 1);
  iVar4 = DAT_08016088;
  if (iVar2 == 0) {
    return 2;
  }
  iVar3 = FUN_0801baf8(param_1,param_2,600,5,*(undefined1 *)(*(int *)(DAT_08016088 + 4) + 0xa8),
                       iVar2);
  cVar5 = '\0';
  if (iVar3 == 1) {
    iVar4 = *(int *)(iVar4 + 4);
    if (*(char *)(iVar4 + 0xa8) != '\0') {
      *(undefined1 *)(iVar4 + 0xa8) = 0;
      *(undefined1 *)(iVar4 + 0xa9) = 0;
      goto LAB_0801605e;
    }
  }
  else {
    iVar4 = *(int *)(iVar4 + 4);
    if (iVar3 != 2) {
      if (*(char *)(iVar4 + 0xa8) == '\x01') {
        if (*(char *)(iVar4 + 0xa9) == '\0') {
          *(undefined1 *)(iVar4 + 0xa8) = 0;
        }
        else {
          *(char *)(iVar4 + 0xa9) = *(char *)(iVar4 + 0xa9) + -1;
        }
      }
      else if (*(char *)(iVar4 + 0xa9) != '\0') {
        *(char *)(iVar4 + 0xa9) = *(char *)(iVar4 + 0xa9) + -1;
      }
      iVar4 = FUN_08015dcc(iVar2,param_2,0);
      if (iVar4 != 0) {
        FUN_080195ac(iVar2);
        return 5;
      }
      FUN_080195ac(iVar2);
      return 0;
    }
    bVar1 = *(byte *)(iVar4 + 0xa9);
    if (*(char *)(iVar4 + 0xa8) == '\0') {
      if (0x14 < bVar1) {
        *(undefined1 *)(iVar4 + 0xa8) = 1;
        goto LAB_0801605e;
      }
    }
    else if (0x13 < bVar1) goto LAB_0801605e;
    cVar5 = bVar1 + 1;
  }
  *(char *)(iVar4 + 0xa9) = cVar5;
LAB_0801605e:
  FUN_080195ac(iVar2);
  return 3;
}


// ======== FUN_0801608c @ 0801608c size=1066 ========

undefined4 FUN_0801608c(void)

{
  char cVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  undefined4 uVar5;
  int iVar6;
  byte bVar7;
  ushort uVar8;
  short sVar9;
  undefined4 uVar10;
  bool bVar11;
  undefined8 uVar12;
  undefined1 uStack_38;
  char local_37;
  undefined4 local_34;
  uint local_30;
  uint uStack_2c;
  undefined1 uStack_28;
  char local_27;
  undefined4 local_24;
  uint local_20;
  uint uStack_1c;
  
  iVar6 = DAT_08016498;
  local_27 = '\x02';
  local_24 = 0;
  iVar2 = FUN_080164bc(*(undefined4 *)(*(int *)(DAT_08016498 + 4) + 0x48),0x7d,&uStack_28,
                       *(undefined4 *)(*(int *)(DAT_08016498 + 4) + 0x34));
  local_37 = '\x02';
  local_34 = 0;
  iVar3 = FUN_080164bc(*(undefined4 *)(*(int *)(iVar6 + 4) + 0x68),0x7d,&uStack_38,
                       *(undefined4 *)(*(int *)(iVar6 + 4) + 0x54));
  if ((iVar2 == 0 && iVar3 == 0) || ((iVar2 == 1 && (iVar3 == 1)))) {
    if (uStack_2c < uStack_1c || uStack_1c - uStack_2c < (uint)(local_30 <= local_20)) {
      uVar4 = local_20 - local_30;
    }
    else {
      uVar4 = local_30 - local_20;
    }
    if (0x32 < uVar4) {
      iVar2 = 6;
      iVar3 = 6;
    }
  }
  FUN_08013036(iVar2,&uStack_28,*(int *)(iVar6 + 4) + 0x30);
  FUN_08013036(iVar3,&uStack_38,*(int *)(iVar6 + 4) + 0x50);
  uVar10 = 0;
  if (iVar2 == 0 && iVar3 == 0) {
    iVar2 = *(int *)(iVar6 + 4);
    *(undefined1 *)(iVar2 + 0xa2) = 0;
    if (*(byte *)(iVar2 + 0xae) < 2) {
      *(undefined1 *)(iVar2 + 0xae) = 0;
    }
    else {
      *(byte *)(iVar2 + 0xae) = *(byte *)(iVar2 + 0xae) - 2;
    }
    if (local_27 == '\0') {
      uVar8 = *(ushort *)(iVar2 + 0x90);
      if (local_37 == '\0') {
        if (uVar8 < 4) {
          *(undefined2 *)(iVar2 + 0x90) = 0;
LAB_080161f4:
          *(undefined1 *)(iVar2 + 0xa3) = 0xff;
        }
        else {
          *(ushort *)(iVar2 + 0x90) = uVar8 - 4;
          if (uVar8 == 4) goto LAB_080161f4;
        }
        if (*(char *)(iVar2 + 0xa3) != -1) goto LAB_080162ae;
        uVar12 = FUN_08013540(&uStack_28,&uStack_38);
        iVar2 = *(int *)(iVar6 + 4);
        *(undefined8 *)(iVar2 + 0x88) = uVar12;
        *(undefined1 *)(iVar2 + 0xad) = 0;
        uVar5 = *(undefined4 *)(iVar2 + 0x34);
      }
      else {
        if (uVar8 < 3) {
          *(undefined2 *)(iVar2 + 0x90) = 0;
LAB_08016226:
          *(undefined1 *)(iVar2 + 0xa3) = 0xff;
        }
        else {
          *(ushort *)(iVar2 + 0x90) = uVar8 - 3;
          if (uVar8 == 3) goto LAB_08016226;
        }
        if (*(char *)(iVar2 + 0xa3) != -1) goto LAB_080162ae;
LAB_0801625e:
        *(uint *)(iVar2 + 0x88) = local_20;
        *(uint *)(iVar2 + 0x8c) = uStack_1c;
        *(undefined1 *)(iVar2 + 0xad) = 0;
        uVar5 = *(undefined4 *)(iVar2 + 0x34);
      }
LAB_080162a2:
      FUN_0801364c(uVar5);
      FUN_0801364c(*(undefined4 *)(*(int *)(iVar6 + 4) + 0x54));
    }
    else {
      uVar8 = *(ushort *)(iVar2 + 0x90);
      if (local_37 == '\0') {
        if (uVar8 < 3) {
          *(undefined2 *)(iVar2 + 0x90) = 0;
LAB_0801624e:
          *(undefined1 *)(iVar2 + 0xa3) = 0xff;
        }
        else {
          *(ushort *)(iVar2 + 0x90) = uVar8 - 3;
          if (uVar8 == 3) goto LAB_0801624e;
        }
        local_20 = local_30;
        uStack_1c = uStack_2c;
        if (*(char *)(iVar2 + 0xa3) == -1) goto LAB_0801625e;
      }
      else {
        if (uVar8 == 0) {
          *(undefined2 *)(iVar2 + 0x90) = 0;
LAB_08016274:
          *(undefined1 *)(iVar2 + 0xa3) = 0xff;
        }
        else {
          *(ushort *)(iVar2 + 0x90) = uVar8 - 1;
          if (uVar8 == 1) goto LAB_08016274;
        }
        if (*(char *)(iVar2 + 0xa3) == -1) {
          if (*(byte *)(iVar2 + 0xad) < 3) {
            *(byte *)(iVar2 + 0xad) = *(byte *)(iVar2 + 0xad) + 1;
          }
          else {
            uVar12 = FUN_08013540(&uStack_28,&uStack_38);
            *(undefined8 *)(*(int *)(iVar6 + 4) + 0x88) = uVar12;
          }
          uVar5 = *(undefined4 *)(*(int *)(iVar6 + 4) + 0x34);
          goto LAB_080162a2;
        }
      }
    }
LAB_080162ae:
    (**(code **)(*(int *)(*(int *)(iVar6 + 4) + 4) + 0x10))();
  }
  else {
    if (iVar2 == 1) {
      if (iVar3 != 1) {
LAB_080162bc:
        iVar2 = *(int *)(iVar6 + 4);
        uVar8 = *(ushort *)(iVar2 + 0x90);
        if (uVar8 < 3) {
          *(undefined2 *)(iVar2 + 0x90) = 0;
LAB_08016378:
          *(undefined1 *)(iVar2 + 0xa3) = 0xff;
        }
        else {
          *(ushort *)(iVar2 + 0x90) = uVar8 - 3;
          if (uVar8 == 3) goto LAB_08016378;
        }
        *(undefined1 *)(iVar2 + 0xa2) = 0;
        *(undefined2 *)(iVar2 + 0x90) = 0;
        *(undefined1 *)(iVar2 + 0xae) = 0;
        goto LAB_0801638c;
      }
      iVar2 = *(int *)(iVar6 + 4);
      *(undefined1 *)(iVar2 + 0xa2) = 0;
      *(undefined1 *)(iVar2 + 0xae) = 0;
      uVar8 = *(ushort *)(iVar2 + 0x90);
      if (uVar8 < 5) {
        *(undefined2 *)(iVar2 + 0x90) = 0;
LAB_08016300:
        *(undefined1 *)(iVar2 + 0xa3) = 0xff;
      }
      else {
        *(ushort *)(iVar2 + 0x90) = uVar8 - 5;
        if (uVar8 == 5) goto LAB_08016300;
      }
      if (local_27 == '\0') {
        if (local_37 != '\0') goto LAB_08016320;
LAB_08016326:
        uVar12 = FUN_08013540(&uStack_28,&uStack_38);
        *(undefined8 *)(*(int *)(iVar6 + 4) + 0x88) = uVar12;
      }
      else {
        local_20 = local_30;
        uStack_1c = uStack_2c;
        if (local_37 != '\0') goto LAB_08016326;
LAB_08016320:
        *(uint *)(iVar2 + 0x88) = local_20;
        *(uint *)(iVar2 + 0x8c) = uStack_1c;
      }
      iVar2 = *(int *)(iVar6 + 4);
      *(undefined1 *)(iVar2 + 0xad) = 0;
      uVar5 = *(undefined4 *)(iVar2 + 0x34);
    }
    else {
      if (iVar3 != 1) {
        if (iVar2 == 1) goto LAB_080162bc;
        if (iVar2 == 7) {
          bVar11 = iVar3 == 7;
LAB_080163a6:
          if (bVar11) {
            iVar2 = *(int *)(iVar6 + 4);
            *(undefined1 *)(iVar2 + 0xad) = 0;
            bVar7 = *(char *)(iVar2 + 0xae) + 1;
            *(byte *)(iVar2 + 0xae) = bVar7;
            if (5 < bVar7) {
              uVar10 = 2;
            }
          }
          else {
LAB_08016132:
            if (iVar3 != 2) {
              iVar2 = *(int *)(iVar6 + 4);
              uVar8 = *(short *)(iVar2 + 0x90) + 1;
              *(ushort *)(iVar2 + 0x90) = uVar8;
              if (*(char *)(iVar2 + 0xae) != '\0') {
                *(char *)(iVar2 + 0xae) = *(char *)(iVar2 + 0xae) + -1;
              }
              *(undefined1 *)(iVar2 + 0xa3) = 0;
              *(undefined1 *)(iVar2 + 0xa2) = 0;
              *(undefined1 *)(iVar2 + 0xad) = 0;
              if (uVar8 < *(ushort *)(iVar2 + 0x9c)) goto LAB_08016168;
            }
LAB_08016166:
            uVar10 = 1;
          }
        }
        else {
          if (iVar2 != 3) {
            if (iVar2 != 2) goto LAB_08016132;
            goto LAB_08016166;
          }
          bVar11 = iVar3 == 3;
          if (!bVar11) goto LAB_080163a6;
          uVar4 = *(uint *)(iVar6 + 4);
          *(undefined1 *)(uVar4 + 0xad) = 0;
          if (*(char *)(uVar4 + 0xae) != '\0') {
            *(char *)(uVar4 + 0xae) = *(char *)(uVar4 + 0xae) + -1;
          }
          bVar7 = *(char *)(uVar4 + 0xa2) + 1;
          *(byte *)(uVar4 + 0xa2) = bVar7;
          bVar11 = 4 < bVar7;
          do {
            if (bVar11) goto LAB_08016166;
            uVar8 = *(ushort *)(uVar4 + 0x90) + 1;
            *(ushort *)(uVar4 + 0x90) = uVar8;
            uVar4 = (uint)*(ushort *)(uVar4 + 0x9c);
            bVar11 = true;
          } while (uVar4 <= uVar8);
        }
        goto LAB_08016168;
      }
      iVar2 = *(int *)(iVar6 + 4);
      uVar8 = *(ushort *)(iVar2 + 0x90);
      if (uVar8 < 3) {
        *(undefined2 *)(iVar2 + 0x90) = 0;
LAB_0801635e:
        *(undefined1 *)(iVar2 + 0xa3) = 0xff;
      }
      else {
        *(ushort *)(iVar2 + 0x90) = uVar8 - 3;
        if (uVar8 == 3) goto LAB_0801635e;
      }
      *(undefined1 *)(iVar2 + 0xa2) = 0;
      *(undefined2 *)(iVar2 + 0x90) = 0;
      *(undefined1 *)(iVar2 + 0xae) = 0;
      local_20 = local_30;
      uStack_1c = uStack_2c;
LAB_0801638c:
      *(uint *)(iVar2 + 0x88) = local_20;
      *(uint *)(iVar2 + 0x8c) = uStack_1c;
      *(undefined1 *)(iVar2 + 0xad) = 0;
      uVar5 = *(undefined4 *)(iVar2 + 0x34);
    }
    FUN_0801364c(uVar5);
    FUN_0801364c(*(undefined4 *)(*(int *)(iVar6 + 4) + 0x54));
  }
LAB_08016168:
  cVar1 = local_37;
  if ((local_27 == '\x01') || (cVar1 = local_27, local_37 == '\x01')) {
    if (cVar1 == '\0') {
LAB_08016456:
      iVar2 = *(int *)(iVar6 + 4);
      if (*(ushort *)(iVar2 + 0x98) < 3) {
        *(undefined2 *)(iVar2 + 0x98) = 0;
      }
      else {
        *(ushort *)(iVar2 + 0x98) = *(ushort *)(iVar2 + 0x98) - 3;
      }
    }
    else {
      iVar2 = *(int *)(iVar6 + 4);
      cVar1 = *(char *)(iVar2 + 0x9e);
      if (*(ushort *)(iVar2 + 0x90) < 0x1f) {
        if (cVar1 == '\0') {
          uVar8 = 0x168;
        }
        else if (cVar1 == '\x01') {
          uVar8 = 0xf0;
        }
        else {
          uVar8 = 0x78;
        }
      }
      else if (cVar1 == '\0') {
        uVar8 = 0x1e;
      }
      else {
        uVar8 = 0x19;
      }
      if (uVar8 <= *(ushort *)(iVar2 + 0x98)) {
        if (*(byte *)(iVar2 + 0x9e) < 2) {
          *(byte *)(iVar2 + 0x9e) = *(byte *)(iVar2 + 0x9e) + 1;
        }
        return 2;
      }
      *(ushort *)(iVar2 + 0x98) = *(ushort *)(iVar2 + 0x98) + 1;
    }
  }
  else {
    bVar11 = local_27 == '\0';
    do {
      if (bVar11) goto LAB_08016456;
      bVar11 = true;
    } while (local_37 == '\0');
  }
  iVar6 = *(int *)(iVar6 + 4);
  if (*(char *)(iVar6 + 0x9e) == '\0') goto LAB_080164ac;
  if ((local_27 == '\0') || (local_37 == '\0')) {
    if (0x1e < *(ushort *)(iVar6 + 0x9a)) {
      *(undefined1 *)(iVar6 + 0x9e) = 0;
      goto LAB_080164a8;
    }
    sVar9 = *(ushort *)(iVar6 + 0x9a) + 1;
  }
  else {
    if (*(ushort *)(iVar6 + 0x9a) < 2) {
LAB_080164a8:
      *(undefined2 *)(iVar6 + 0x9a) = 0;
      goto LAB_080164ac;
    }
    sVar9 = *(ushort *)(iVar6 + 0x9a) - 2;
  }
  *(short *)(iVar6 + 0x9a) = sVar9;
LAB_080164ac:
  FUN_080168b8(&uStack_28,&uStack_38);
  return uVar10;
}


// ======== FUN_080164bc @ 080164bc size=530 ========

/* WARNING: Type propagation algorithm not settling */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_080164bc(int param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  undefined4 uVar9;
  undefined4 *puVar10;
  undefined4 uVar11;
  int *piVar12;
  int local_30 [3];
  
  if (((param_1 == 0) || (param_3 == 0)) || (iVar1 = FUN_080195b8(param_2 << 1), iVar1 == 0)) {
    return 2;
  }
  uVar9 = 2;
  iVar2 = FUN_0801baf8(param_1,param_2,param_2,5,0,iVar1);
  if (iVar2 != 0) {
    if (iVar2 == 1) {
      uVar9 = 3;
    }
    else if (iVar2 == 2) {
      uVar9 = 4;
    }
LAB_080166c4:
    FUN_080195ac(iVar1);
    return uVar9;
  }
  iVar3 = FUN_08013950(0,0x10);
  iVar2 = _DAT_080166d0;
  local_30[2] = 0;
  if (iVar3 == 0) {
    FUN_080141e8(1,s_init_bd_08016724,s____src_application_process_deal__080166fc,DAT_080166f8,0x51c
                 ,s_create_wave_queue_error_0801676c);
    goto LAB_080166c4;
  }
  piVar12 = local_30 + 2;
  uVar11 = 0;
  local_30[0] = iVar3;
  iVar4 = FUN_08015dcc(iVar1,param_2,0);
  if (iVar4 != 0) {
    uVar9 = 5;
    FUN_08013d7c(iVar3);
    goto LAB_080166c4;
  }
  FUN_080195ac(iVar1);
  iVar1 = *(int *)(iVar2 + 4);
  iVar1 = FUN_08014e88(iVar3,*(undefined4 *)(iVar1 + 0x78),*(undefined4 *)(iVar1 + 0x88),
                       *(undefined4 *)(iVar1 + 0x8c),0x32,param_3,uVar11,piVar12);
  if (iVar1 == 0) {
    if (*(ushort *)(*(int *)(iVar2 + 4) + 0x90) < 3) {
      uVar9 = 0;
      goto LAB_0801669e;
    }
    iVar1 = FUN_0801499e(param_4,**(undefined4 **)(iVar3 + 0xc));
    if (iVar1 == 0) {
      FUN_080141e8(1,s_init_bd_08016724,s____src_application_process_deal__080166fc,DAT_080166f8,
                   0x4bf,s_enqueue_wave_to_disturb_wave_fai_080166d3 + 1);
      FUN_080141e8(1,s_init_bd_08016724,s____src_application_process_deal__080166fc,DAT_080166f8,
                   0x50e,s_fw_find_disturb_false__return_er_0801672c);
      goto LAB_0801669e;
    }
LAB_0801661e:
    if (3 < *(uint *)(param_4 + 4)) {
      iVar1 = FUN_080159a0(param_4,*(undefined4 *)(iVar3 + 4),
                           *(undefined4 *)(*(int *)(iVar2 + 4) + 0x78),param_3);
      if (iVar1 == 1) {
        FUN_080141e8(1,s_init_bd_08016724,s____src_application_process_deal__080166fc,DAT_080166f8,
                     0x4f3,s_disturb_find_error__size__d_08016750,*(undefined4 *)(param_4 + 4));
        uVar9 = 2;
        goto LAB_0801669e;
      }
      if (iVar1 == 0) {
        uVar9 = 1;
        goto LAB_0801669e;
      }
      if (iVar1 == 4) {
        uVar9 = 7;
        goto LAB_0801669e;
      }
      if ((iVar1 != 2) && (iVar1 == 3)) {
        uVar9 = 8;
        goto LAB_0801669e;
      }
    }
  }
  else {
    if (1 < *(uint *)(iVar3 + 4)) {
      if (*(int *)(param_4 + 4) != 0) {
        iVar1 = **(int **)(iVar3 + 0x10);
        do {
          iVar4 = **(int **)(param_4 + 0x10);
          do {
            uVar7 = *(uint *)(iVar4 + 8);
            uVar6 = *(int *)(*(int *)(iVar2 + 4) + 0x78) * 6;
            uVar8 = *(uint *)(iVar1 + 0xc);
            uVar5 = *(int *)(iVar4 + 0xc) + (uint)CARRY4(uVar7,uVar6);
            if ((uVar8 < uVar5 || uVar5 - uVar8 < (uint)(*(uint *)(iVar1 + 8) <= uVar7 + uVar6)) ||
               (*(int *)(param_4 + 4) == 0)) goto LAB_08016606;
            FUN_08013d30(param_4,0);
          } while (*(int *)(param_4 + 4) == 0);
        } while( true );
      }
LAB_08016606:
      puVar10 = *(undefined4 **)(iVar3 + 0x10);
      for (uVar5 = 0; uVar5 < *(uint *)(iVar3 + 4); uVar5 = uVar5 + 1) {
        FUN_0801499e(param_4,*puVar10);
        puVar10 = (undefined4 *)puVar10[2];
      }
      goto LAB_0801661e;
    }
    local_30[0] = 0;
    local_30[1] = 0;
    iVar1 = *(int *)(iVar2 + 4);
    FUN_080179f8(*(undefined4 *)(iVar1 + 0x88),*(undefined4 *)(iVar1 + 0x8c),
                 *(undefined4 *)(**(int **)(iVar3 + 0xc) + 8),
                 *(undefined4 *)(**(int **)(iVar3 + 0xc) + 0xc),*(undefined4 *)(iVar1 + 0x78),0x32,
                 local_30,local_30 + 1);
  }
  uVar9 = 6;
LAB_0801669e:
  FUN_08013d7c(iVar3);
  return uVar9;
}


// ======== FUN_08016784 @ 08016784 size=226 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4
FUN_08016784(int param_1,int param_2,undefined4 param_3,undefined4 param_4,undefined4 param_5,
            undefined4 param_6,undefined4 param_7,undefined4 param_8,int param_9,int param_10)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 *puVar6;
  int iVar7;
  undefined4 local_3c [2];
  int iStack_34;
  int iStack_30;
  undefined4 local_2c;
  undefined4 uStack_28;
  
  if (((param_1 == 0) || (param_10 == 0)) ||
     (iStack_34 = param_1, iStack_30 = param_2, local_2c = param_3, uStack_28 = param_4,
     iVar1 = FUN_080195b8(param_2 << 1), iVar1 == 0)) {
    uVar5 = 2;
  }
  else {
    uVar5 = 2;
    iVar2 = FUN_0801baf8(param_1,param_2,param_2,4,0,iVar1);
    if (iVar2 == 0) {
      iVar2 = FUN_08013950(0,0x10);
      local_3c[0] = 0;
      if (iVar2 == 0) {
        FUN_080141e8(1,s_init_bd_080168b0,s____src_application_process_deal__08016888,DAT_08016884,
                     0xfea,s_create_wave_queue_error_0801686b + 1);
      }
      else {
        puVar6 = local_3c;
        uVar5 = 0xffffffff;
        iVar7 = iVar2;
        iVar3 = FUN_08015dcc(iVar1,param_2,local_2c);
        if (iVar3 == 0) {
          FUN_080195ac(iVar1);
          uVar4 = 0x78;
          if (param_9 == 0) {
            uVar4 = 400;
          }
          iVar1 = FUN_08015008(iVar2,3000,param_5,param_6,uVar4,param_10,uVar5,puVar6,iVar7);
          if (iVar1 == 0) {
            uVar5 = 0;
          }
          else {
            uVar5 = 6;
          }
          FUN_08013d7c(iVar2);
          return uVar5;
        }
        uVar5 = 5;
        FUN_08013d7c(iVar2);
      }
    }
    else if (iVar2 == 1) {
      uVar5 = 3;
    }
    else if (iVar2 == 2) {
      uVar5 = 4;
    }
    FUN_080195ac(iVar1);
  }
  return uVar5;
}


// ======== FUN_080168b8 @ 080168b8 size=322 ========

/* WARNING: Removing unreachable block (ram,0x080168d2) */

undefined4 FUN_080168b8(int param_1,int param_2)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined1 uStack_48;
  undefined1 local_47;
  undefined4 local_44;
  undefined1 uStack_38;
  undefined1 local_37;
  undefined4 local_34;
  
  local_47 = 2;
  local_44 = 0;
  iVar7 = *(int *)(DAT_080169fc + 4);
  uVar1 = 0;
  uVar4 = 0;
  if (*(int *)(*(int *)(iVar7 + 8) + 4) != 0) {
    iVar5 = **(int **)(*(int *)(iVar7 + 8) + 0xc);
    uVar1 = *(undefined4 *)(iVar5 + 8);
    uVar4 = *(undefined4 *)(iVar5 + 0xc);
  }
  if (*(char *)(param_1 + 1) == '\x02') {
    uVar8 = 0;
    iVar5 = *(uint *)(iVar7 + 0x80) + 1000;
    iVar6 = *(int *)(iVar7 + 0x84) + (uint)(0xfffffc17 < *(uint *)(iVar7 + 0x80));
    iVar2 = *(int *)(iVar7 + 0x48);
  }
  else {
    uVar8 = 0xffffffff;
    iVar5 = *(int *)(param_1 + 8);
    iVar6 = *(int *)(param_1 + 0xc);
    iVar2 = *(int *)(iVar7 + 0x48);
  }
  uVar1 = FUN_08016784(iVar2 + 0x17c,0x2d,0xbe,iVar7,iVar5,iVar6,uVar1,uVar4,uVar8,&uStack_48);
  iVar7 = DAT_080169fc;
  local_37 = 2;
  local_34 = 0;
  iVar5 = *(int *)(DAT_080169fc + 4);
  uVar4 = 0;
  uVar8 = 0;
  if (*(int *)(*(int *)(iVar5 + 0xc) + 4) != 0) {
    iVar6 = **(int **)(*(int *)(iVar5 + 0xc) + 0xc);
    uVar4 = *(undefined4 *)(iVar6 + 8);
    uVar8 = *(undefined4 *)(iVar6 + 0xc);
  }
  if (*(char *)(param_2 + 1) == '\x02') {
    uVar9 = 0;
    iVar6 = *(uint *)(iVar5 + 0x80) + 1000;
    iVar2 = *(int *)(iVar5 + 0x84) + (uint)(0xfffffc17 < *(uint *)(iVar5 + 0x80));
    iVar3 = *(int *)(iVar5 + 0x68);
  }
  else {
    uVar9 = 0xffffffff;
    iVar6 = *(int *)(param_2 + 8);
    iVar2 = *(int *)(param_2 + 0xc);
    iVar3 = *(int *)(iVar5 + 0x68);
  }
  uVar4 = FUN_08016784(iVar3 + 0x17c,0x2d,0xbe,iVar5,iVar6,iVar2,uVar4,uVar8,uVar9,&uStack_38);
  iVar5 = *(int *)(iVar7 + 4);
  FUN_08013100(uVar1,*(undefined4 *)(iVar5 + 8),&uStack_48,iVar5 + 0xa5,iVar5 + 0xaa,iVar5 + 0x10);
  iVar7 = *(int *)(iVar7 + 4);
  FUN_08013100(uVar4,*(undefined4 *)(iVar7 + 0xc),&uStack_38,iVar7 + 0xa6,iVar7 + 0xab,iVar7 + 0x20)
  ;
  return 0;
}


// ======== FUN_08016a00 @ 08016a00 size=28 ========

void FUN_08016a00(void)

{
  int iVar1;
  undefined8 uVar2;
  
  iVar1 = DAT_08016a1c;
  (**(code **)(*(int *)(*(int *)(DAT_08016a1c + 4) + 0x74) + 8))();
  uVar2 = (**(code **)(*(int *)(*(int *)(iVar1 + 4) + 4) + 0x10))();
  *(undefined8 *)(*(int *)(iVar1 + 4) + 0x80) = uVar2;
  return;
}


// ======== FUN_08016a20 @ 08016a20 size=72 ========

undefined4 FUN_08016a20(uint param_1,undefined4 param_2)

{
  undefined4 *puVar1;
  int iVar2;
  int iVar3;
  
  puVar1 = DAT_08016a6c;
  iVar2 = DAT_08016a68;
  *DAT_08016a6c = 0x5555;
  iVar3 = iVar2;
  do {
    iVar3 = iVar3 + -1;
    if (iVar3 == 0) {
      if ((puVar1[3] & 1) != 0) {
        return 0;
      }
      break;
    }
  } while ((puVar1[3] & 1) != 0);
  puVar1[1] = param_2;
  do {
    iVar2 = iVar2 + -1;
    if (iVar2 == 0) {
      if ((puVar1[3] & 2) != 0) {
        return 0;
      }
      break;
    }
  } while ((puVar1[3] & 2) != 0);
  puVar1[2] = param_1 & 0xfff;
  *puVar1 = 0xaaaa;
  return 1;
}


// ======== FUN_08016a70 @ 08016a70 size=10 ========

void FUN_08016a70(void)

{
  *DAT_08016a7c = 0xaaaa;
  return;
}


// ======== FUN_08016a80 @ 08016a80 size=10 ========

void FUN_08016a80(void)

{
  *DAT_08016a8c = 0xcccc;
  return;
}


// ======== FUN_08016a90 @ 08016a90 size=6 ========

undefined2 FUN_08016a90(void)

{
  return *DAT_08016a98;
}


// ======== FUN_08016aa8 @ 08016aa8 size=6 ========

undefined4 FUN_08016aa8(void)

{
  return *(undefined4 *)(DAT_08016ab0 + 4);
}


// ======== FUN_08016ab4 @ 08016ab4 size=6 ========

undefined4 FUN_08016ab4(void)

{
  return *DAT_08016abc;
}


// ======== FUN_08016ac0 @ 08016ac0 size=6 ========

undefined4 FUN_08016ac0(void)

{
  return *(undefined4 *)(DAT_08016ac8 + 4);
}


// ======== FUN_08016ad8 @ 08016ad8 size=6 ========

undefined4 FUN_08016ad8(void)

{
  return *DAT_08016ae0;
}


// ======== FUN_08016ae4 @ 08016ae4 size=76 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

bool FUN_08016ae4(uint param_1,uint param_2)

{
  if (5 < param_1) {
    if ((code *)*DAT_08016b30 == (code *)0x0) {
      FUN_080141e8(0,&LAB_08016ba0,s____src_platform_easylogger_elog__08016b7c,DAT_08016b34,0x2a7,
                   s___s__has_assert_failed_at__s__ld_08016b57 + 1,
                   s_level_<__ELOG_LVL_VERBOSE_08016b38,DAT_08016b34,0x2a7);
      do {
                    /* WARNING: Do nothing block with infinite loop */
      } while( true );
    }
    (*(code *)*DAT_08016b30)(s_level_<__ELOG_LVL_VERBOSE_08016b38,DAT_08016b34);
  }
  return (*(uint *)(_DAT_08016b54 + param_1 * 4 + 0x4c) & param_2) != 0;
}


// ======== FUN_08016ba8 @ 08016ba8 size=124 ========

undefined4 * FUN_08016ba8(void)

{
  int *piVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  
  piVar1 = DAT_08016c24;
  puVar4 = (undefined4 *)*DAT_08016c24;
  if ((undefined4 *)*DAT_08016c24 == (undefined4 *)0x0) {
    iVar2 = FUN_080195b8(0x20);
    *piVar1 = iVar2;
    puVar4 = (undefined4 *)0x0;
    if (iVar2 != 0) {
      puVar3 = (undefined4 *)FUN_080195b8(0xb8);
      puVar5 = (undefined4 *)*piVar1;
      puVar5[7] = puVar3;
      puVar4 = (undefined4 *)0x0;
      if (puVar3 != (undefined4 *)0x0) {
        piVar1[1] = (int)puVar3;
        *puVar5 = DAT_08016c28;
        puVar5[1] = DAT_08016c2c;
        puVar5[2] = DAT_08016c30;
        puVar5[3] = DAT_08016c34;
        puVar5[4] = DAT_08016c38;
        puVar5[5] = DAT_08016c3c;
        puVar5[6] = DAT_08016c40;
        *(undefined1 *)((int)puVar3 + 0x9f) = 0;
        *puVar3 = 0;
        *(undefined1 *)(puVar3 + 0x28) = 0xff;
        puVar3[1] = 0;
        puVar3[0x20] = 0;
        puVar3[0x21] = 0;
        *(undefined1 *)((int)puVar3 + 0xa1) = 0;
        *(undefined2 *)((int)puVar3 + 0x92) = 800;
        *(undefined1 *)((int)puVar3 + 0xa3) = 0xff;
        *(undefined1 *)(puVar3 + 0x29) = 0;
        puVar3[2] = 0;
        puVar3[3] = 0;
        *(undefined2 *)((int)puVar3 + 0x96) = 0;
        *(undefined1 *)((int)puVar3 + 0xad) = 0;
        *(undefined1 *)((int)puVar3 + 0xaf) = 0;
        *(undefined1 *)(puVar3 + 0x2c) = 0;
        puVar4 = puVar5;
      }
    }
  }
  return puVar4;
}


// ======== FUN_08016c44 @ 08016c44 size=60 ========

undefined4 FUN_08016c44(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  iVar4 = DAT_08016c88;
  iVar2 = DAT_08016c84;
  if (*DAT_08016c80 == '\0') {
    iVar1 = FUN_08016f96(DAT_08016c88,0x800);
    iVar3 = iVar4 >> 0x12;
  }
  else {
    iVar1 = FUN_08016f96(DAT_08016c84,0x100);
    iVar3 = iVar2 >> 0x15;
    iVar4 = iVar2;
  }
  iVar2 = FUN_08016f96(iVar4,iVar3);
  if ((iVar1 != 0) && (iVar2 != 0)) {
    return 0;
  }
  return 1;
}


// ======== FUN_08016c98 @ 08016c98 size=6 ========

undefined4 FUN_08016c98(void)

{
  return *DAT_08016ca0;
}


// ======== FUN_08016d20 @ 08016d20 size=130 ========

undefined4 * FUN_08016d20(void)

{
  int *piVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined4 uVar4;
  undefined4 *puVar5;
  
  piVar1 = DAT_08016da4;
  if ((undefined4 *)*DAT_08016da4 != (undefined4 *)0x0) {
    return (undefined4 *)*DAT_08016da4;
  }
  iVar2 = FUN_080195b8(0x18);
  *piVar1 = iVar2;
  if (iVar2 == 0) {
    uVar4 = 0xa6;
  }
  else {
    puVar3 = (undefined4 *)FUN_080195b8(0x1c);
    puVar5 = (undefined4 *)*piVar1;
    puVar5[5] = puVar3;
    uVar4 = DAT_08016da8;
    if (puVar3 != (undefined4 *)0x0) {
      piVar1[1] = (int)puVar3;
      *puVar5 = uVar4;
      puVar5[1] = DAT_08016dac;
      puVar5[2] = DAT_08016db0;
      puVar5[3] = DAT_08016db4;
      puVar5[4] = DAT_08016db8;
      *(undefined1 *)(puVar3 + 3) = 0;
      *puVar3 = 10;
      puVar3[1] = 0;
      *(undefined1 *)((int)puVar3 + 0xd) = 0xff;
      *(undefined1 *)((int)puVar3 + 0xe) = 4;
      *(undefined1 *)((int)puVar3 + 0xf) = 0;
      *(undefined1 *)(puVar3 + 4) = 0;
      *(undefined1 *)((int)puVar3 + 0x11) = 0;
      *(undefined1 *)((int)puVar3 + 0x13) = 0xff;
      *(undefined1 *)((int)puVar3 + 0x12) = 0xff;
      *(undefined1 *)(puVar3 + 5) = 0xff;
      *(undefined1 *)((int)puVar3 + 0x15) = 0;
      *(undefined1 *)((int)puVar3 + 0x16) = 0;
      *(undefined1 *)((int)puVar3 + 0x17) = 0;
      *(undefined1 *)(puVar3 + 6) = 0xff;
      *(undefined1 *)((int)puVar3 + 0x19) = 0xff;
      return puVar5;
    }
    uVar4 = 0xa2;
  }
  FUN_080141e8(3,&DAT_08016e0c,s____src_application_process_comm__08016de8,DAT_08016de4,uVar4,
               s_create_comm_task_init_bd_service_08016dbc);
  return (undefined4 *)0x0;
}


// ======== FUN_08016e54 @ 08016e54 size=6 ========

undefined1 FUN_08016e54(void)

{
  return *DAT_08016e5c;
}


// ======== FUN_08016ee8 @ 08016ee8 size=4 ========

void FUN_08016ee8(int param_1,undefined4 param_2)

{
  *(undefined4 *)(param_1 + 0x14) = param_2;
  return;
}


// ======== FUN_08016ef0 @ 08016ef0 size=166 ========

void FUN_08016ef0(uint *param_1,uint param_2,uint param_3,uint param_4)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  
  uVar1 = param_2 & 0xf;
  if ((int)(param_2 << 0x1b) < 0) {
    if (param_3 == 4) {
      uVar1 = uVar1 | 3;
      param_1[0xf] = param_1[0xf] | param_4;
    }
    else {
      uVar1 = uVar1 | param_3;
    }
  }
  uVar2 = 0;
  do {
    uVar3 = 1 << (uVar2 & 0xff) & param_4;
    if (uVar3 != 0) {
      if (param_2 == 0x28) {
        param_1[5] = uVar3;
      }
      else if (param_2 == 0x48) {
        param_1[4] = uVar3;
      }
      *param_1 = uVar1 << (uVar2 << 2 & 0xff) | *param_1 & ~(0xf << (uVar2 << 2 & 0xff));
    }
    uVar2 = uVar2 + 1 & 0xffff;
  } while (uVar2 < 8);
  uVar2 = 8;
  do {
    uVar3 = 1 << (uVar2 & 0xff) & param_4;
    if (uVar3 != 0) {
      uVar4 = uVar2 * 4 - 0x20;
      if (param_2 == 0x28) {
        param_1[5] = uVar3;
      }
      else if (param_2 == 0x48) {
        param_1[4] = uVar3;
      }
      param_1[1] = uVar1 << (uVar4 & 0xff) | param_1[1] & ~(0xf << (uVar4 & 0xff));
    }
    uVar2 = uVar2 + 1 & 0xffff;
  } while (uVar2 < 0x10);
  return;
}


// ======== FUN_08016f96 @ 08016f96 size=10 ========

bool FUN_08016f96(int param_1,uint param_2)

{
  return (*(uint *)(param_1 + 8) & param_2) != 0;
}


// ======== FUN_08016fa0 @ 08016fa0 size=308 ========

undefined4 FUN_08016fa0(undefined4 param_1)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  iVar3 = DAT_080170d4;
  if ((*(byte *)(*(int *)(DAT_080170d4 + 4) + 0x9f) & 1) == 0) {
    uVar1 = FUN_08016aa8();
    *(undefined4 *)(*(int *)(iVar3 + 4) + 0x74) = uVar1;
    iVar2 = FUN_08016ad8();
    *(int *)(*(int *)(iVar3 + 4) + 4) = iVar2;
    (**(code **)(iVar2 + 4))(DAT_080170d8);
    iVar2 = *(int *)(iVar3 + 4);
    *(undefined2 *)(iVar2 + 0x92) = 800;
    *(undefined4 *)(iVar2 + 0x78) = 0x49b4;
    *(undefined2 *)(iVar2 + 0x94) = 0;
    *(undefined4 *)(iVar2 + 0x80) = 0;
    *(undefined4 *)(iVar2 + 0x84) = 0;
    *(undefined1 *)(iVar2 + 0x39) = 3;
    *(undefined1 *)(iVar2 + 0x59) = 3;
    *(undefined1 *)(iVar2 + 0x4c) = 0;
    *(undefined1 *)(iVar2 + 0x6c) = 1;
    *(undefined1 *)(iVar2 + 0x4d) = 0;
    *(undefined1 *)(iVar2 + 0x6d) = 0;
    uVar1 = FUN_08013950(0x10);
    *(undefined4 *)(*(int *)(iVar3 + 4) + 0x34) = uVar1;
    uVar1 = FUN_08013950(0x10);
    iVar2 = *(int *)(iVar3 + 4);
    *(undefined4 *)(iVar2 + 0x54) = uVar1;
    *(undefined1 *)(iVar2 + 0x9e) = 0;
    *(undefined1 *)(iVar2 + 0xa8) = 0;
    *(undefined1 *)(iVar2 + 0xa9) = 0;
    *(undefined1 *)(iVar2 + 0x11) = 2;
    *(undefined4 *)(iVar2 + 0x14) = 0;
    *(undefined1 *)(iVar2 + 0x21) = 2;
    *(undefined4 *)(iVar2 + 0x24) = 0;
    *(undefined1 *)(iVar2 + 0xac) = 0;
    iVar2 = FUN_08013950(0x28,0x10);
    *(int *)(*(int *)(iVar3 + 4) + 0x30) = iVar2;
    if (iVar2 == 0) {
      return 0xffffffff;
    }
    iVar2 = FUN_08013950(0x28,0x10);
    *(int *)(*(int *)(iVar3 + 4) + 0x50) = iVar2;
    if (iVar2 == 0) {
      return 0xffffffff;
    }
    iVar2 = FUN_08013950(5,0x10);
    *(int *)(*(int *)(iVar3 + 4) + 8) = iVar2;
    if (iVar2 == 0) {
      return 0xffffffff;
    }
    iVar2 = FUN_08013950(5,0x10);
    iVar4 = *(int *)(iVar3 + 4);
    *(int *)(iVar4 + 0xc) = iVar2;
    if (iVar2 == 0) {
      return 0xffffffff;
    }
    iVar2 = FUN_080195b8((uint)*(ushort *)(iVar4 + 0x92) << 1);
    iVar4 = *(int *)(iVar3 + 4);
    *(int *)(iVar4 + 0x48) = iVar2;
    if (iVar2 == 0) {
      return 0xffffffff;
    }
    iVar2 = FUN_080195b8((uint)*(ushort *)(iVar4 + 0x92) << 1);
    iVar4 = *(int *)(iVar3 + 4);
    *(int *)(iVar4 + 0x68) = iVar2;
    if (iVar2 == 0) {
      return 0xffffffff;
    }
    iVar2 = FUN_080195b8((uint)*(ushort *)(iVar4 + 0x92) << 2);
    *(int *)(*(int *)(iVar3 + 4) + 0x70) = iVar2;
    if (iVar2 == 0) {
      return 0xffffffff;
    }
    FUN_0801aac4();
    FUN_0801a978();
    iVar3 = *(int *)(iVar3 + 4);
    *(undefined1 *)(iVar3 + 0xa0) = 0xff;
    *(undefined1 *)(iVar3 + 0x9f) = 1;
    iVar3 = FUN_0801c280(DAT_080170e4,s_init_bd_080170dc,0x400,0,param_1,iVar3);
    if (iVar3 != 1) {
      return 0xffffffff;
    }
  }
  return 0;
}


// ======== FUN_0801715c @ 0801715c size=196 ========

void FUN_0801715c(void)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 uVar3;
  char *pcVar4;
  
  puVar1 = (undefined4 *)FUN_08019340();
  if (puVar1 == (undefined4 *)0x0) {
    pcVar4 = s_register_timer_driver_failed_080172dc;
    uVar3 = 0xa7;
LAB_08017170:
    FUN_080141e8(2,&DAT_08017268,s____src_application_rw_bdboard_in_08017240,DAT_0801723c,uVar3,
                 pcVar4);
  }
  else {
    iVar2 = (*(code *)*puVar1)();
    if (iVar2 != 0) {
      pcVar4 = s_init_timer_driver_failed_08017220;
      uVar3 = 0xa4;
      goto LAB_08017170;
    }
    (*(code *)puVar1[3])();
  }
  puVar1 = (undefined4 *)FUN_08019240();
  if (puVar1 == (undefined4 *)0x0) {
    pcVar4 = s_register_board_sensor_driver_fai_080172fc;
    uVar3 = 0xb0;
  }
  else {
    iVar2 = (*(code *)*puVar1)();
    if (iVar2 == 0) goto LAB_080171a8;
    pcVar4 = s_init_board_sensor_driver_failed_08017270;
    uVar3 = 0xad;
  }
  FUN_080141e8(2,&DAT_08017268,s____src_application_rw_bdboard_in_08017240,DAT_0801723c,uVar3,pcVar4
              );
LAB_080171a8:
  puVar1 = (undefined4 *)FUN_08019300();
  (*(code *)puVar1[2])(1);
  (*(code *)puVar1[3])(1);
  iVar2 = (*(code *)*puVar1)();
  if (iVar2 != 0) {
    FUN_080141e8(2,&DAT_08017268,s____src_application_rw_bdboard_in_08017240,DAT_0801723c,0xbb,
                 s_init_port_driver_failed_08017290);
  }
  puVar1 = (undefined4 *)FUN_080192c8();
  iVar2 = (*(code *)*puVar1)();
  if (iVar2 != 0) {
    FUN_080141e8(2,&DAT_08017268,s____src_application_rw_bdboard_in_08017240,DAT_0801723c,0xc1,
                 s_init_lift_driver_failed_080172a8);
  }
  puVar1 = (undefined4 *)FUN_08019288();
  iVar2 = (*(code *)*puVar1)();
  if (iVar2 != 0) {
    FUN_080141e8(2,&DAT_08017268,s____src_application_rw_bdboard_in_08017240,DAT_0801723c,199,
                 s_init_factory_driver_failed_080172c0);
  }
  return;
}


// ======== FUN_080175e8 @ 080175e8 size=274 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_080175e8(void)

{
  undefined4 *puVar1;
  undefined1 *puVar2;
  undefined4 *puVar3;
  int iVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 local_28;
  undefined4 uStack_24;
  undefined4 local_20;
  undefined4 uStack_1c;
  
  puVar1 = DAT_080176fc;
  local_28 = *DAT_080176fc;
  uStack_24 = DAT_080176fc[1];
  FUN_08019584(&local_28);
  local_20 = puVar1[2];
  uStack_1c = puVar1[3];
  FUN_08012e88(&local_20);
  puVar2 = (undefined1 *)FUN_080195b8(1);
  puVar1 = _DAT_08017700;
  _DAT_08017700[1] = puVar2;
  if (puVar2 == (undefined1 *)0x0) {
    FUN_080141e8(3,&DAT_08017748,s____src_application_rw_bdboard_in_08017720,DAT_080176fc + 4,0x60,
                 s_malloc_init_pri_data_failed_08017703 + 1);
  }
  else {
    *puVar2 = 0xff;
  }
  FUN_08017b44();
  FUN_0801715c();
  *(undefined1 *)puVar1[1] = 0;
  uVar6 = 0xc353;
  uVar5 = DAT_08017750;
  FUN_080141e8(3,&DAT_08017748,s____src_application_rw_bdboard_in_08017720,DAT_080176fc + 4,0x6a,
               s_hardwave_version__d__software_ve_08017754,DAT_08017750,0xc353);
  if (*(char *)puVar1[1] == '\0') {
    puVar3 = (undefined4 *)FUN_08016ba8();
    iVar4 = (*(code *)*puVar3)(0x17);
    if (iVar4 == 0) {
      (*(code *)puVar3[1])();
    }
    else {
      FUN_080141e8(2,&DAT_080177d8,s____src_application_rw_bdboard_in_080177b0,
                   (int)DAT_080176fc + 0x33,0xe4,s_init_process_init_board_failed_08017790,uVar5,
                   uVar6);
    }
    puVar3 = (undefined4 *)FUN_08016d20();
    iVar4 = (*(code *)*puVar3)(0x14);
    if (iVar4 == 0) {
      (*(code *)puVar3[1])();
    }
    else {
      FUN_080141e8(2,&DAT_080177d8,s____src_application_rw_bdboard_in_080177b0,
                   (int)DAT_080176fc + 0x45,0xef,s_init_process_comm_failed_080177e0);
    }
    if (puVar1[1] != 0) {
      FUN_080195ac();
      puVar1[1] = 0;
    }
    FUN_0801b790(*puVar1);
    return;
  }
  do {
    FUN_0801b65c(1000);
    FUN_080141e8(1,&DAT_08017748,s____src_application_rw_bdboard_in_08017720,DAT_080176fc + 4,0x7a,
                 s_init_error__d_08017780,(int)*(char *)puVar1[1],uVar6);
  } while( true );
}


// ======== FUN_080177fc @ 080177fc size=76 ========

uint * FUN_080177fc(int param_1)

{
  uint uVar1;
  int *piVar2;
  uint *puVar3;
  uint *puVar4;
  uint uVar5;
  uint uVar6;
  
  uVar5 = param_1 + 0xbU & 0xfffffff8;
  puVar4 = DAT_08017848;
  while( true ) {
    puVar3 = (uint *)*puVar4;
    if (puVar3 == (uint *)0x0) {
      return (uint *)0x0;
    }
    uVar1 = *puVar3;
    if (uVar5 <= uVar1) break;
    puVar4 = puVar3 + 1;
  }
  if (uVar5 < uVar1) {
    uVar6 = puVar3[1];
    FUN_08010912(puVar3 + 1);
    piVar2 = (int *)((int)puVar3 + uVar5);
    *piVar2 = uVar1 - uVar5;
    piVar2[1] = uVar6;
    *puVar4 = (uint)piVar2;
  }
  else {
    *puVar4 = puVar3[1];
    FUN_08010912(puVar3 + 1);
  }
  *puVar3 = uVar5;
  return puVar3 + 1;
}


// ======== FUN_08017884 @ 08017884 size=70 ========

undefined4 FUN_08017884(void)

{
  byte bVar1;
  int iVar2;
  int iVar3;
  
  iVar2 = DAT_080178cc;
  bVar1 = *(byte *)(*(int *)(DAT_080178cc + 4) + 0x15);
  if (bVar1 < 0x50) {
    *(byte *)(*(int *)(DAT_080178cc + 4) + 0x15) = bVar1 + 1;
    iVar3 = FUN_08016ab4();
    iVar3 = (**(code **)(iVar3 + 4))();
    if (iVar3 == 0) {
      *(char *)(*(int *)(iVar2 + 4) + 0x16) = *(char *)(*(int *)(iVar2 + 4) + 0x16) + '\x01';
    }
    else {
      iVar3 = *(int *)(iVar2 + 4);
      if (*(byte *)(iVar3 + 0x16) < 2) {
        *(undefined1 *)(iVar3 + 0x16) = 0;
        return 0xffffffff;
      }
      *(byte *)(iVar3 + 0x16) = *(byte *)(iVar3 + 0x16) - 2;
    }
    if (10 < *(byte *)(*(int *)(iVar2 + 4) + 0x16)) {
      return 0;
    }
  }
  return 0xffffffff;
}


// ======== FUN_080178d0 @ 080178d0 size=262 ========

undefined4 FUN_080178d0(char *param_1,char *param_2,uint param_3,int param_4,int param_5)

{
  byte bVar1;
  byte bVar2;
  undefined1 uVar3;
  ushort uVar4;
  int iVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  uint uVar8;
  char cVar9;
  undefined8 uVar10;
  undefined8 uVar11;
  
  bVar1 = param_2[1];
  bVar2 = param_1[1];
  if (bVar1 < bVar2) {
    uVar4 = (ushort)bVar2 - (ushort)bVar1;
  }
  else {
    uVar4 = (ushort)bVar1 - (ushort)bVar2;
  }
  if (0xc < uVar4) {
    return 1;
  }
  if (*param_1 == '\0') {
    if (*param_2 == '\x01') goto LAB_0801791a;
  }
  else if ((*param_1 == '\x01') && (*param_2 == '\0')) {
LAB_0801791a:
    uVar8 = (uint)*(short *)(param_1 + 8);
    if ((int)uVar8 < 0) {
      uVar8 = -uVar8;
    }
    iVar5 = (int)*(short *)(param_2 + 8);
    if (iVar5 < 0) {
      iVar5 = -iVar5;
    }
    if (iVar5 != 0) {
      uVar10 = FUN_08010e52();
      uVar11 = FUN_08010e52(uVar8);
      uVar10 = FUN_08010d74((int)uVar11,(int)((ulonglong)uVar11 >> 0x20),(int)uVar10,
                            (int)((ulonglong)uVar10 >> 0x20));
      uVar7 = (undefined4)((ulonglong)uVar10 >> 0x20);
      uVar6 = (undefined4)uVar10;
      cVar9 = 14999 < uVar8;
      if ((int)uVar8 < 0x3a99) {
        FUN_08010efc(uVar6,uVar7,(int)DAT_080179e8,(int)((ulonglong)DAT_080179e8 >> 0x20));
        uVar10 = DAT_080179f0;
      }
      else {
        FUN_08010efc(uVar6,uVar7,(int)DAT_080179d8,(int)((ulonglong)DAT_080179d8 >> 0x20));
        uVar10 = DAT_080179e0;
      }
      if (cVar9 == '\0') {
        cVar9 = '\0';
        FUN_08010ecc(uVar6,uVar7,(int)uVar10,(int)((ulonglong)uVar10 >> 0x20));
        if (cVar9 == '\0') {
          if (((byte)param_1[1] - 4 < 0xb) && ((byte)param_2[1] - 4 < 0x11)) {
            *(int *)(param_5 + 4) = (int)*(short *)(param_1 + 8);
            uVar4 = *(ushort *)(param_1 + 4);
            *(uint *)(param_5 + 8) = (uint)uVar4 * 0x14 + param_3;
            *(uint *)(param_5 + 0xc) = param_4 + (uint)CARRY4((uint)uVar4 * 0x14,param_3);
            if ((*param_1 != '\0') || (uVar3 = 1, *param_2 != '\x01')) {
              uVar3 = 0;
            }
            *(undefined1 *)(param_5 + 1) = uVar3;
            return 0;
          }
          return 3;
        }
      }
    }
    return 2;
  }
  return 4;
}


// ======== FUN_080179f8 @ 080179f8 size=182 ========

undefined4
FUN_080179f8(uint param_1,uint param_2,uint param_3,uint param_4,uint param_5,uint param_6,
            undefined4 *param_7,int *param_8)

{
  uint uVar1;
  uint uVar2;
  undefined4 uVar3;
  int iVar4;
  
  if (((((param_3 != 0 || param_4 != 0) && (param_1 != 0 || param_2 != 0)) &&
       (param_2 < param_4 || param_4 - param_2 < (uint)(param_1 <= param_3))) &&
      ((param_6 <= param_5 / 5 && (param_1 != 0 || param_2 != 0)))) && (param_5 != 0)) {
    uVar1 = param_3 - param_1;
    iVar4 = (param_4 - param_2) - (uint)(param_3 < param_1);
    uVar2 = FUN_080108a4(uVar1 + (param_5 >> 1),iVar4 + (uint)CARRY4(uVar1,param_5 >> 1),param_5,0);
    if ((uVar2 < 0x96) && (uVar2 != 0)) {
      uVar3 = FUN_080108a4(uVar1,iVar4,uVar2,0);
      iVar4 = uVar1 - uVar2 * param_5;
      if (uVar2 < 0x10) {
        if (uVar2 < 0xb) {
          if (5 < uVar2) {
            param_6 = param_6 + uVar2 * 3;
          }
        }
        else {
          param_6 = param_6 + uVar2 * 5;
        }
      }
      else {
        param_6 = param_6 + uVar2 * 6;
      }
      if ((iVar4 <= (int)param_6) && ((int)-param_6 <= iVar4)) {
        *param_7 = uVar3;
        *param_8 = iVar4;
        return 0;
      }
      *param_7 = uVar3;
      *param_8 = iVar4;
    }
  }
  return 0xffffffff;
}


// ======== thunk_FUN_08016a70 @ 08017aae size=4 ========

void thunk_FUN_08016a70(void)

{
  *DAT_08016a7c = 0xaaaa;
  return;
}


// ======== FUN_08017ab2 @ 08017ab2 size=4 ========

undefined4 FUN_08017ab2(void)

{
  return 0;
}


// ======== FUN_08017b44 @ 08017b44 size=122 ========

void FUN_08017b44(void)

{
  int iVar1;
  undefined4 unaff_lr;
  
  iVar1 = FUN_0801418c();
  if (iVar1 != 0) {
    FUN_080141e8(2,DAT_08017bb4 + 0x28,DAT_08017bb4,DAT_08017bb0,0x97,s_elog_init_failed_08017b9c);
    return;
  }
  FUN_080146b0(0,3);
  FUN_080146b0(1,0x83);
  FUN_080146b0(2,0x83);
  FUN_080146b0(3,0x83);
  FUN_080146b0(4,0x83);
  FUN_080146b0(5,3);
  *(undefined1 *)(DAT_08014794 + 0x65) = 1;
  FUN_080141e8(3,&DAT_080147f0,s____src_platform_easylogger_elog__080147cc,DAT_080147c8,0xd9,
               s_EasyLogger_V_s_is_initialize_suc_080147a0,s_2_2_99_08014798,unaff_lr);
  return;
}


// ======== FUN_08017bc4 @ 08017bc4 size=144 ========

void FUN_08017bc4(uint param_1,int param_2,uint param_3)

{
  uint extraout_r2;
  int iVar1;
  int iVar2;
  
  iVar2 = 0;
  iVar1 = 0;
  if ((*DAT_08017c54 & 0x7ff) >> 8 == 7) {
    iVar1 = 4;
  }
  else if ((*DAT_08017c54 & 0x7ff) >> 8 == 6) {
    iVar2 = 1;
    iVar1 = 3;
  }
  else {
    if ((*DAT_08017c54 & 0x7ff) >> 8 != 5) {
      if ((*DAT_08017c54 & 0x7ff) >> 8 == 4) {
        iVar2 = 3;
        iVar1 = 1;
        goto LAB_08017c22;
      }
      if ((*DAT_08017c54 & 0x7ff) >> 8 == 3) {
        iVar2 = 4;
        goto LAB_08017c22;
      }
      FUN_08017c58(0x500);
      param_3 = extraout_r2;
    }
    iVar2 = 2;
    iVar1 = 2;
  }
LAB_08017c22:
  (&DAT_e000e400)[param_1] =
       (char)((0xfU >> (4U - iVar1 & 0xff) & param_3 | param_2 << (4U - iVar2 & 0xff)) << 4);
  *(int *)((param_1 >> 5) * 4 + -0x1fff1f00) = 1 << (param_1 & 0x1f);
  return;
}


// ======== FUN_08017c58 @ 08017c58 size=10 ========

void FUN_08017c58(uint param_1)

{
  *DAT_08017c68 = param_1 | DAT_08017c64;
  return;
}


// ======== FUN_08017d4c @ 08017d4c size=64 ========

int FUN_08017d4c(int param_1)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  
  iVar1 = 0;
  uVar3 = 0;
  do {
    uVar2 = (uint)*(byte *)(param_1 + uVar3);
    if (uVar2 - 0x30 < 10) {
      iVar1 = iVar1 + -0x30;
    }
    else if (uVar2 - 0x41 < 6) {
      iVar1 = iVar1 + -0x37;
    }
    else {
      if (5 < uVar2 - 0x61) {
        return 0;
      }
      iVar1 = iVar1 + -0x57;
    }
    iVar1 = iVar1 + uVar2;
    if (uVar3 < 3) {
      iVar1 = iVar1 * 0x10;
    }
    uVar3 = uVar3 + 1;
  } while (uVar3 < 4);
  return iVar1;
}


// ======== FUN_08017f92 @ 08017f92 size=268 ========

undefined4 FUN_08017f92(int param_1,int *param_2)

{
  int iVar1;
  byte *pbVar2;
  uint uVar3;
  byte bVar4;
  int iVar5;
  int iVar6;
  byte *pbVar7;
  byte *pbVar8;
  byte *local_20;
  
  iVar6 = param_2[2];
  local_20 = (byte *)0x0;
  iVar1 = *param_2;
  pbVar8 = (byte *)(*param_2 + iVar6) + 1;
  if (*(char *)(iVar1 + iVar6) == '\"') {
    iVar5 = 0;
    uVar3 = param_2[1];
    pbVar7 = (byte *)(*param_2 + iVar6);
    while( true ) {
      pbVar2 = pbVar7;
      pbVar7 = pbVar2 + 1;
      if ((uVar3 <= (uint)((int)pbVar7 - iVar1)) || (*pbVar7 == 0x22)) break;
      if (*pbVar7 == 0x5c) {
        if (uVar3 <= ((int)pbVar7 - iVar1) + 1U) goto LAB_08018092;
        iVar5 = iVar5 + 1;
        pbVar7 = pbVar2 + 2;
      }
    }
    if (((uint)((int)pbVar7 - iVar1) < uVar3) &&
       ((*pbVar7 == 0x22 &&
        (pbVar2 = (byte *)(*(code *)param_2[4])(pbVar7 + (-iVar5 - (iVar1 + iVar6)) + 1),
        local_20 = pbVar2, pbVar2 != (byte *)0x0)))) {
      while( true ) {
        while( true ) {
          if (pbVar7 <= pbVar8) {
            *local_20 = 0;
            *(undefined4 *)(param_1 + 0xc) = 0x10;
            *(byte **)(param_1 + 0x10) = pbVar2;
            param_2[2] = (int)(pbVar7 + (1 - *param_2));
            return 1;
          }
          bVar4 = *pbVar8;
          if (bVar4 == 0x5c) break;
          pbVar8 = pbVar8 + 1;
          *local_20 = bVar4;
          local_20 = local_20 + 1;
        }
        iVar1 = 2;
        if ((int)pbVar7 - (int)pbVar8 < 1) break;
        bVar4 = pbVar8[1];
        if (bVar4 == 0x66) {
          bVar4 = 0xc;
LAB_08018048:
          *local_20 = bVar4;
          local_20 = local_20 + 1;
        }
        else {
          if (bVar4 < 0x67) {
            if (((bVar4 != 0x22) && (bVar4 != 0x2f)) && (bVar4 != 0x5c)) {
              if (bVar4 != 0x62) break;
              bVar4 = 8;
            }
            goto LAB_08018048;
          }
          if (bVar4 == 0x6e) {
            bVar4 = 10;
            goto LAB_08018048;
          }
          if (bVar4 == 0x72) {
            bVar4 = 0xd;
            goto LAB_08018048;
          }
          if (bVar4 == 0x74) {
            bVar4 = 9;
            goto LAB_08018048;
          }
          if ((bVar4 != 0x75) || (iVar1 = FUN_0801b2fc(pbVar8,pbVar7,&local_20), iVar1 == 0)) break;
        }
        pbVar8 = pbVar8 + iVar1;
      }
      if (pbVar2 != (byte *)0x0) {
        (*(code *)param_2[5])(pbVar2);
      }
    }
  }
LAB_08018092:
  if (pbVar8 != (byte *)0x0) {
    param_2[2] = (int)pbVar8 - *param_2;
  }
  return 0;
}


// ======== FUN_080180a0 @ 080180a0 size=920 ========

undefined4 FUN_080180a0(int param_1,int *param_2)

{
  byte bVar1;
  undefined8 uVar2;
  int *piVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  int *piVar7;
  int *piVar8;
  int *piVar9;
  bool bVar10;
  bool bVar11;
  undefined4 uVar12;
  undefined4 extraout_s1;
  byte local_60 [64];
  undefined1 *local_20;
  
  if ((param_2 == (int *)0x0) || (*param_2 == 0)) {
    return 0;
  }
  if (((uint)param_2[1] < param_2[2] + 4U) ||
     (iVar4 = FUN_080109be(*param_2 + param_2[2],&DAT_08018184,4), iVar4 != 0)) {
    if ((param_2[2] + 5U <= (uint)param_2[1]) &&
       (iVar4 = FUN_080109be(param_2[2] + *param_2,s_false_0801818c,5), iVar4 == 0)) {
      *(undefined4 *)(param_1 + 0xc) = 1;
      iVar4 = param_2[2] + 5;
      goto LAB_0801813c;
    }
    if (((uint)param_2[1] < param_2[2] + 4U) ||
       (iVar4 = FUN_080109be(param_2[2] + *param_2,&DAT_08018194,4), iVar4 != 0)) {
      uVar5 = param_2[1];
      uVar6 = param_2[2];
      if ((uVar6 < uVar5) && (*(char *)(*param_2 + uVar6) == '\"')) {
        uVar12 = FUN_08017f92(param_1,param_2);
        return uVar12;
      }
      if ((uVar5 <= uVar6) ||
         ((*(byte *)(*param_2 + uVar6) != 0x2d && (9 < *(byte *)(*param_2 + uVar6) - 0x30)))) {
        if ((uVar5 <= uVar6) || (*(char *)(*param_2 + uVar6) != '[')) {
          if (uVar5 <= uVar6) {
            return 0;
          }
          if (*(char *)(*param_2 + uVar6) != '{') {
            return 0;
          }
          piVar8 = (int *)0x0;
          if (((((uint)param_2[3] < 1000) && (param_2[3] = param_2[3] + 1, param_2 != (int *)0x0))
              && (uVar5 = param_2[2], uVar5 < (uint)param_2[1])) &&
             (*(char *)(*param_2 + uVar5) == '{')) {
            param_2[2] = uVar5 + 1;
            FUN_08012d4c(param_2);
            uVar5 = param_2[2];
            if ((uVar5 < (uint)param_2[1]) && (*(char *)(*param_2 + uVar5) == '}')) {
LAB_08017f6c:
              param_2[3] = param_2[3] + -1;
              *(int **)(param_1 + 8) = piVar8;
              *(undefined4 *)(param_1 + 0xc) = 0x40;
              param_2[2] = param_2[2] + 1;
              return 1;
            }
            param_2[2] = uVar5 - 1;
            if (uVar5 < (uint)param_2[1]) {
              piVar7 = (int *)0x0;
              piVar9 = piVar8;
              do {
                piVar3 = (int *)FUN_08012ed0(param_2 + 4);
                if (piVar3 == (int *)0x0) {
                  piVar8 = piVar9;
                  if (piVar9 == (int *)0x0) {
                    return 0;
                  }
                  goto LAB_08017f88;
                }
                piVar8 = piVar3;
                if (piVar9 != (int *)0x0) {
                  *piVar7 = (int)piVar3;
                  piVar3[1] = (int)piVar7;
                  piVar8 = piVar9;
                }
                param_2[2] = param_2[2] + 1;
                FUN_08012d4c(param_2);
                iVar4 = FUN_08017f92(piVar3,param_2);
                if (iVar4 == 0) goto LAB_08017f88;
                FUN_08012d4c(param_2);
                piVar3[8] = piVar3[4];
                piVar3[4] = 0;
                if (((param_2 == (int *)0x0) || (uVar5 = param_2[2], (uint)param_2[1] <= uVar5)) ||
                   (*(char *)(*param_2 + uVar5) != ':')) goto LAB_08017f88;
                param_2[2] = uVar5 + 1;
                FUN_08012d4c(param_2);
                iVar4 = FUN_080180a0(piVar3,param_2);
                if (iVar4 == 0) goto LAB_08017f88;
                FUN_08012d4c(param_2);
                uVar5 = param_2[2];
              } while ((uVar5 < (uint)param_2[1]) &&
                      (piVar7 = piVar3, piVar9 = piVar8, *(char *)(*param_2 + uVar5) == ','));
              if ((uVar5 < (uint)param_2[1]) && (*(char *)(*param_2 + uVar5) == '}'))
              goto LAB_08017f6c;
LAB_08017f88:
              FUN_08012e38(piVar8);
            }
          }
          return 0;
        }
        piVar8 = (int *)0x0;
        if ((uint)param_2[3] < 1000) {
          param_2[3] = param_2[3] + 1;
          if (*(char *)(*param_2 + param_2[2]) == '[') {
            param_2[2] = param_2[2] + 1;
            FUN_08012d4c(param_2);
            if (param_2 != (int *)0x0) {
              uVar5 = param_2[2];
              if ((uVar5 < (uint)param_2[1]) && (*(char *)(*param_2 + uVar5) == ']')) {
LAB_08017d26:
                param_2[3] = param_2[3] + -1;
                *(int **)(param_1 + 8) = piVar8;
                *(undefined4 *)(param_1 + 0xc) = 0x20;
                param_2[2] = param_2[2] + 1;
                return 1;
              }
              if (uVar5 < (uint)param_2[1]) {
                param_2[2] = uVar5 - 1;
                piVar7 = piVar8;
                piVar9 = (int *)0x0;
                do {
                  piVar3 = (int *)FUN_08012ed0(param_2 + 4);
                  if (piVar3 == (int *)0x0) {
                    if (piVar7 == (int *)0x0) {
                      return 0;
                    }
                    goto LAB_08017d42;
                  }
                  piVar8 = piVar3;
                  if (piVar7 != (int *)0x0) {
                    *piVar9 = (int)piVar3;
                    piVar3[1] = (int)piVar9;
                    piVar8 = piVar7;
                  }
                  param_2[2] = param_2[2] + 1;
                  FUN_08012d4c(param_2);
                  iVar4 = FUN_080180a0(piVar3,param_2);
                  piVar7 = piVar8;
                  if ((iVar4 == 0) || (FUN_08012d4c(param_2), param_2 == (int *)0x0))
                  goto LAB_08017d42;
                  uVar5 = param_2[2];
                } while ((uVar5 < (uint)param_2[1]) &&
                        (piVar9 = piVar3, *(char *)(*param_2 + uVar5) == ','));
                if (((uint)param_2[1] <= uVar5) || (*(char *)(*param_2 + uVar5) != ']')) {
LAB_08017d42:
                  FUN_08012e38(piVar7);
                  return 0;
                }
                goto LAB_08017d26;
              }
            }
            param_2[2] = param_2[2] + -1;
          }
        }
        return 0;
      }
      uVar5 = 0;
      local_20 = (undefined1 *)0x0;
      if (param_2 == (int *)0x0) {
        return 0;
      }
      if (*param_2 == 0) {
        return 0;
      }
LAB_08017dfe:
      if ((uint)param_2[1] <= param_2[2] + uVar5) {
switchD_08017dc2_caseD_2c:
        local_60[uVar5] = 0;
        uVar12 = FUN_08011e90(local_60,&local_20);
        uVar2 = DAT_08017e7c;
        bVar11 = local_20 <= local_60;
        bVar10 = local_60 == local_20;
        if (bVar10) {
          return 0;
        }
        *(ulonglong *)(param_1 + 0x18) = CONCAT44(extraout_s1,uVar12);
        FUN_08010efc(uVar12,extraout_s1,(int)uVar2,(int)((ulonglong)uVar2 >> 0x20));
        if (bVar11 && !bVar10) {
          FUN_08010ecc(uVar12,extraout_s1,(int)DAT_08017e84,(int)((ulonglong)DAT_08017e84 >> 0x20));
          if (bVar11 && !bVar10) {
            uVar12 = FUN_08010e8e(uVar12,extraout_s1);
          }
          else {
            uVar12 = 0x80000000;
          }
        }
        else {
          uVar12 = 0x7fffffff;
        }
        *(undefined4 *)(param_1 + 0x14) = uVar12;
        *(undefined4 *)(param_1 + 0xc) = 8;
        param_2[2] = (int)(local_20 + (param_2[2] - (int)local_60));
        return 1;
      }
      bVar1 = *(byte *)(uVar5 + param_2[2] + *param_2);
      if (bVar1 != 0x34) {
        if (bVar1 < 0x35) {
          switch(bVar1) {
          case 0x2b:
          case 0x2d:
          case 0x30:
          case 0x31:
          case 0x32:
          case 0x33:
            goto switchD_08017dc2_caseD_2b;
          default:
            goto switchD_08017dc2_caseD_2c;
          case 0x2e:
            local_60[uVar5] = 0x2e;
          }
          goto LAB_08017df6;
        }
        if (bVar1 != 0x38) {
          if (0x38 < bVar1) {
            if (((bVar1 == 0x39) || (bVar1 == 0x45)) || (bVar1 == 0x65))
            goto switchD_08017dc2_caseD_2b;
            goto switchD_08017dc2_caseD_2c;
          }
          if (((bVar1 != 0x35) && (bVar1 != 0x36)) && (bVar1 != 0x37))
          goto switchD_08017dc2_caseD_2c;
        }
      }
switchD_08017dc2_caseD_2b:
      local_60[uVar5] = bVar1;
LAB_08017df6:
      uVar5 = uVar5 + 1;
      if ((0x3e < uVar5) || (param_2 == (int *)0x0)) goto switchD_08017dc2_caseD_2c;
      goto LAB_08017dfe;
    }
    *(undefined4 *)(param_1 + 0x14) = 1;
    *(undefined4 *)(param_1 + 0xc) = 2;
  }
  else {
    *(undefined4 *)(param_1 + 0xc) = 4;
  }
  iVar4 = param_2[2] + 4;
LAB_0801813c:
  param_2[2] = iVar4;
  return 1;
}


// ======== FUN_0801819c @ 0801819c size=12 ========

void FUN_0801819c(void)

{
  *DAT_080181a8 = *DAT_080181a8 | 0x100;
  return;
}


// ======== FUN_08018648 @ 08018648 size=258 ========

undefined4 FUN_08018648(byte *param_1,int param_2)

{
  byte bVar1;
  byte *pbVar2;
  byte bVar3;
  byte *pbVar4;
  undefined4 uVar5;
  int iVar6;
  byte *pbVar7;
  byte *pbVar8;
  
  iVar6 = 0;
  if (param_2 == 0) {
    uVar5 = 0;
  }
  else {
    pbVar8 = param_1;
    if (param_1 == (byte *)0x0) {
      iVar6 = FUN_08014a02(param_2,3);
      if (iVar6 == 0) {
        return 0;
      }
      FUN_080109ac(iVar6,&DAT_0801874c);
    }
    else {
      for (; bVar3 = *pbVar8, bVar3 != 0; pbVar8 = pbVar8 + 1) {
        if (bVar3 == 0xc) {
LAB_080186c8:
          iVar6 = iVar6 + 1;
        }
        else {
          if (bVar3 < 0xd) {
            if (((bVar3 == 8) || (bVar3 == 9)) || (bVar3 == 10)) goto LAB_080186c8;
          }
          else if (((bVar3 == 0xd) || (bVar3 == 0x22)) || (bVar3 == 0x5c)) goto LAB_080186c8;
          if (bVar3 < 0x20) {
            iVar6 = iVar6 + 5;
          }
        }
      }
      pbVar8 = pbVar8 + (iVar6 - (int)param_1);
      pbVar4 = (byte *)FUN_08014a02(param_2,pbVar8 + 3);
      if (pbVar4 == (byte *)0x0) {
        return 0;
      }
      *pbVar4 = 0x22;
      pbVar2 = pbVar4;
      if (iVar6 == 0) {
        FUN_08010912(pbVar4 + 1,param_1,pbVar8);
      }
      else {
        while( true ) {
          bVar3 = *param_1;
          if (bVar3 == 0) break;
          if (((bVar3 < 0x20) || (bVar3 == 0x22)) || (pbVar7 = pbVar2 + 1, bVar3 == 0x5c)) {
            pbVar7 = pbVar2 + 2;
            pbVar2[1] = 0x5c;
            bVar1 = *param_1;
            bVar3 = 0x5c;
            if (bVar1 == 0xc) {
              bVar3 = 0x66;
              goto LAB_08018746;
            }
            if (bVar1 < 0xd) {
              if (bVar1 == 8) {
                bVar3 = 0x62;
              }
              else if (bVar1 == 9) {
                bVar3 = 0x74;
              }
              else {
                if (bVar1 != 10) goto LAB_0801870c;
                bVar3 = 0x6e;
              }
              goto LAB_08018746;
            }
            if (bVar1 == 0xd) {
              bVar3 = 0x72;
              goto LAB_08018746;
            }
            if (bVar1 == 0x22) {
              *pbVar7 = 0x22;
            }
            else {
              if (bVar1 == 0x5c) goto LAB_08018746;
LAB_0801870c:
              FUN_08011db8(pbVar7,s_u_04x_08018750);
              pbVar7 = pbVar2 + 6;
            }
          }
          else {
LAB_08018746:
            *pbVar7 = bVar3;
          }
          param_1 = param_1 + 1;
          pbVar2 = pbVar7;
        }
      }
      pbVar4[(int)(pbVar8 + 1)] = 0x22;
      pbVar4[(int)(pbVar8 + 2)] = 0;
    }
    uVar5 = 1;
  }
  return uVar5;
}


// ======== FUN_08018758 @ 08018758 size=796 ========

undefined1 * FUN_08018758(int param_1,int param_2)

{
  byte bVar1;
  uint uVar2;
  int iVar3;
  undefined1 *puVar4;
  undefined1 *puVar5;
  int iVar6;
  char *pcVar7;
  undefined4 uVar8;
  uint uVar9;
  int *piVar10;
  char cVar11;
  bool bVar12;
  undefined4 uVar13;
  char local_48 [32];
  undefined1 local_28 [12];
  
  if (param_1 == 0) {
    return (undefined1 *)0x0;
  }
  if (param_2 == 0) {
    return (undefined1 *)0x0;
  }
  bVar1 = *(byte *)(param_1 + 0xc);
  if (bVar1 == 0x10) {
    puVar4 = (undefined1 *)FUN_08018648(*(undefined4 *)(param_1 + 0x10));
    return puVar4;
  }
  if (bVar1 < 0x11) {
    if (bVar1 == 1) {
      iVar3 = FUN_08014a02(param_2,6);
      if (iVar3 == 0) {
        return (undefined1 *)0x0;
      }
      pcVar7 = s_false_08018818;
    }
    else if (bVar1 == 2) {
      iVar3 = FUN_08014a02(param_2,5);
      if (iVar3 == 0) {
        return (undefined1 *)0x0;
      }
      pcVar7 = &DAT_08018820;
    }
    else {
      if (bVar1 != 4) {
        if (bVar1 != 8) {
          return (undefined1 *)0x0;
        }
        uVar9 = 0;
        if (param_2 != 0) {
          uVar8 = (undefined4)((ulonglong)*(undefined8 *)(param_1 + 0x18) >> 0x20);
          uVar13 = (undefined4)*(undefined8 *)(param_1 + 0x18);
          uVar2 = FUN_08011db8(local_48,s__1_15g_080184f4,uVar13,uVar8);
          iVar3 = FUN_080109dc(local_48,&DAT_080184fc,local_28);
          cVar11 = iVar3 == 1;
          if ((!(bool)cVar11) ||
             (FUN_08010ecc((int)local_28._0_8_,SUB84(local_28._0_8_,4),uVar13,uVar8), cVar11 == '\0'
             )) {
            uVar2 = FUN_08011db8(local_48,s__1_17g_08018500,uVar13,uVar8);
          }
          if (uVar2 < 0x1a) {
            iVar3 = FUN_08014a02(param_2,uVar2 + 1);
            if (iVar3 == 0) {
              return (undefined1 *)0x0;
            }
            for (; uVar9 < uVar2; uVar9 = uVar9 + 1) {
              if (local_48[uVar9] == '.') {
                *(undefined1 *)(iVar3 + uVar9) = 0x2e;
              }
              else {
                *(char *)(iVar3 + uVar9) = local_48[uVar9];
              }
            }
            *(undefined1 *)(iVar3 + uVar9) = 0;
            *(uint *)(param_2 + 8) = *(int *)(param_2 + 8) + uVar2;
            return (undefined1 *)0x1;
          }
        }
        return (undefined1 *)0x0;
      }
      iVar3 = FUN_08014a02(param_2,5);
      if (iVar3 == 0) {
        return (undefined1 *)0x0;
      }
      pcVar7 = &DAT_08018810;
    }
    FUN_080109ac(iVar3,pcVar7);
  }
  else {
    if (bVar1 == 0x20) {
      piVar10 = *(int **)(param_1 + 8);
      if (param_2 == 0) {
        puVar4 = (undefined1 *)0x0;
      }
      else {
        puVar5 = (undefined1 *)FUN_08014a02(param_2,1);
        puVar4 = (undefined1 *)0x0;
        if (puVar5 != (undefined1 *)0x0) {
          *puVar5 = 0x5b;
          *(int *)(param_2 + 8) = *(int *)(param_2 + 8) + 1;
          *(int *)(param_2 + 0xc) = *(int *)(param_2 + 0xc) + 1;
          for (; piVar10 != (int *)0x0; piVar10 = (int *)*piVar10) {
            iVar3 = FUN_08018758(piVar10,param_2);
            if (iVar3 == 0) {
              return (undefined1 *)0x0;
            }
            FUN_0801b028(param_2);
            if (*piVar10 != 0) {
              if (*(int *)(param_2 + 0x14) == 0) {
                iVar3 = 1;
              }
              else {
                iVar3 = 2;
              }
              puVar4 = (undefined1 *)FUN_08014a02(param_2,iVar3 + 1);
              if (puVar4 == (undefined1 *)0x0) {
                return (undefined1 *)0x0;
              }
              *puVar4 = 0x2c;
              puVar5 = puVar4 + 1;
              if (*(int *)(param_2 + 0x14) != 0) {
                puVar5 = puVar4 + 2;
                puVar4[1] = 0x20;
              }
              *puVar5 = 0;
              *(int *)(param_2 + 8) = *(int *)(param_2 + 8) + iVar3;
            }
          }
          puVar5 = (undefined1 *)FUN_08014a02(param_2,2);
          puVar4 = (undefined1 *)0x0;
          if (puVar5 != (undefined1 *)0x0) {
            *puVar5 = 0x5d;
            puVar5[1] = 0;
            *(int *)(param_2 + 0xc) = *(int *)(param_2 + 0xc) + -1;
            puVar4 = (undefined1 *)0x1;
          }
        }
      }
      return puVar4;
    }
    if (bVar1 == 0x40) {
      piVar10 = *(int **)(param_1 + 8);
      if (param_2 == 0) {
        puVar4 = (undefined1 *)0x0;
      }
      else {
        if (*(int *)(param_2 + 0x14) == 0) {
          iVar3 = 1;
        }
        else {
          iVar3 = 2;
        }
        puVar4 = (undefined1 *)FUN_08014a02(param_2,iVar3 + 1);
        bVar12 = puVar4 == (undefined1 *)0x0;
        do {
          if (bVar12) {
            return puVar4;
          }
          *puVar4 = 0x7b;
          *(int *)(param_2 + 0xc) = *(int *)(param_2 + 0xc) + 1;
          if (*(int *)(param_2 + 0x14) != 0) {
            puVar4[1] = 10;
          }
          iVar6 = *(int *)(param_2 + 8) + iVar3;
          iVar3 = 9;
          *(int *)(param_2 + 8) = iVar6;
          for (; piVar10 != (int *)0x0; piVar10 = (int *)*piVar10) {
            if (*(int *)(param_2 + 0x14) != 0) {
              puVar4 = (undefined1 *)FUN_08014a02(param_2,*(undefined4 *)(param_2 + 0xc));
              if (puVar4 == (undefined1 *)0x0) {
                return (undefined1 *)0x0;
              }
              for (uVar9 = 0; uVar9 < *(uint *)(param_2 + 0xc); uVar9 = uVar9 + 1) {
                *puVar4 = 9;
                puVar4 = puVar4 + 1;
              }
              *(uint *)(param_2 + 8) = *(int *)(param_2 + 8) + *(uint *)(param_2 + 0xc);
            }
            iVar6 = FUN_08018648(piVar10[8],param_2);
            if (iVar6 == 0) {
              return (undefined1 *)0x0;
            }
            FUN_0801b028(param_2);
            if (*(int *)(param_2 + 0x14) == 0) {
              iVar6 = 1;
            }
            else {
              iVar6 = 2;
            }
            puVar4 = (undefined1 *)FUN_08014a02(param_2,iVar6);
            if (puVar4 == (undefined1 *)0x0) {
              return (undefined1 *)0x0;
            }
            *puVar4 = 0x3a;
            if (*(int *)(param_2 + 0x14) != 0) {
              puVar4[1] = 9;
            }
            *(int *)(param_2 + 8) = *(int *)(param_2 + 8) + iVar6;
            iVar6 = FUN_08018758(piVar10,param_2);
            if (iVar6 == 0) {
              return (undefined1 *)0x0;
            }
            FUN_0801b028(param_2);
            iVar6 = (uint)(*(int *)(param_2 + 0x14) != 0) + (uint)(*piVar10 != 0);
            puVar4 = (undefined1 *)FUN_08014a02(param_2,iVar6 + 1);
            if (puVar4 == (undefined1 *)0x0) {
              return (undefined1 *)0x0;
            }
            puVar5 = puVar4;
            if (*piVar10 != 0) {
              puVar5 = puVar4 + 1;
              *puVar4 = 0x2c;
            }
            puVar4 = puVar5;
            if (*(int *)(param_2 + 0x14) != 0) {
              puVar4 = puVar5 + 1;
              *puVar5 = 10;
            }
            *puVar4 = 0;
            *(int *)(param_2 + 8) = *(int *)(param_2 + 8) + iVar6;
          }
          if (*(int *)(param_2 + 0x14) == 0) {
            iVar6 = 2;
          }
          else {
            iVar6 = *(int *)(param_2 + 0xc) + 1;
          }
          puVar5 = (undefined1 *)FUN_08014a02(param_2,iVar6);
          bVar12 = true;
          puVar4 = (undefined1 *)0x0;
        } while (puVar5 == (undefined1 *)0x0);
        if (*(int *)(param_2 + 0x14) != 0) {
          for (uVar9 = 0; uVar9 < *(int *)(param_2 + 0xc) - 1U; uVar9 = uVar9 + 1) {
            *puVar5 = 9;
            puVar5 = puVar5 + 1;
          }
        }
        *puVar5 = 0x7d;
        puVar5[1] = 0;
        *(int *)(param_2 + 0xc) = *(int *)(param_2 + 0xc) + -1;
        puVar4 = (undefined1 *)0x1;
      }
      return puVar4;
    }
    if ((bVar1 != 0x80) || (*(int *)(param_1 + 0x10) == 0)) {
      return (undefined1 *)0x0;
    }
    iVar3 = FUN_08010982();
    iVar6 = FUN_08014a02(param_2,iVar3 + 1);
    if (iVar6 == 0) {
      return (undefined1 *)0x0;
    }
    FUN_08010912(iVar6,*(undefined4 *)(param_1 + 0x10),iVar3 + 1);
  }
  return (undefined1 *)0x1;
}


// ======== FUN_08018828 @ 08018828 size=318 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_08018828(void)

{
  byte bVar1;
  bool bVar2;
  byte *pbVar3;
  int *piVar4;
  byte *pbVar5;
  int iVar6;
  int iVar7;
  uint uVar8;
  uint *puVar9;
  int extraout_r2;
  undefined4 unaff_r4;
  undefined4 unaff_lr;
  undefined4 uVar10;
  uint uVar11;
  byte bVar12;
  int iVar13;
  
  FUN_08013fc8();
  FUN_0801c280(DAT_08018858,s_initial_0801884f + 1,0x80,0,0xf,_DAT_0801884c);
  uVar10 = 0;
  iVar13 = _DAT_0801ba14;
  iVar6 = FUN_0801c280(DAT_0801ba20,s_IDLE_0801ba17 + 1,0x40,0,0,_DAT_0801ba14,unaff_r4,unaff_lr);
  iVar7 = _DAT_0801ba14;
  if (iVar6 == 1) {
    bVar2 = (bool)isCurrentModePrivileged();
    if (bVar2) {
      setBasePriority(0x10);
    }
    DataSynchronizationBarrier(0xf);
    InstructionSynchronizationBarrier(0xf);
    *(undefined4 *)(_DAT_0801ba14 + -4) = 0xffffffff;
    *(undefined4 *)(iVar7 + -0x18) = 1;
    *(undefined4 *)(iVar7 + -0x20) = 0;
    piVar4 = DAT_0801bc1c;
    if (*DAT_0801bc1c == _DAT_0801bc20) {
      FUN_08011a1a(0,s_Error__s__d_0801bc44,s_A___src_platform_freertos_port_c_0801bc23 + 1,0x159,
                   uVar10,iVar13,unaff_r4,unaff_lr);
    }
    if (*piVar4 == _DAT_0801bc20 + -1) {
      FUN_08011a1a(0,s_Error__s__d_0801bc44,s_A___src_platform_freertos_port_c_0801bc23 + 1,0x15a,
                   uVar10,iVar13,unaff_r4,unaff_lr);
    }
    pbVar5 = DAT_0801bc54;
    bVar12 = *DAT_0801bc54;
    *DAT_0801bc54 = 0xff;
    bVar1 = *pbVar5;
    uVar11 = (uint)bVar1;
    if ((bVar1 & 0xf0) != bVar1) {
      FUN_08011a1a(0,s_Error__s__d_0801bc44,s_A___src_platform_freertos_port_c_0801bc23 + 1,0x173,
                   uVar11,bVar12,unaff_r4,unaff_lr);
    }
    pbVar3 = DAT_0801bc58;
    *DAT_0801bc58 = (byte)uVar11 & 0x10;
    uVar8 = 7;
    pbVar3[8] = 7;
    pbVar3[9] = 0;
    pbVar3[10] = 0;
    pbVar3[0xb] = 0;
    for (; (int)(uVar11 << 0x18) < 0; uVar11 = (uint)(byte)((char)uVar11 << 1)) {
      uVar8 = uVar8 - 1;
    }
    *(uint *)(pbVar3 + 8) = (uVar8 & 7) << 8;
    *pbVar5 = bVar12;
    puVar9 = (uint *)(DAT_0801bc1c + 8);
    *puVar9 = *puVar9 | 0xf00000;
    *puVar9 = *puVar9 | 0xf0000000;
    FUN_0801b5b8();
    *(undefined4 *)(extraout_r2 + 4) = 0;
    FUN_08010184();
    *DAT_0801bc5c = *DAT_0801bc5c | 0xc0000000;
    FUN_08010164();
    return 0;
  }
  if (iVar6 + 1 == 0) {
    iVar7 = FUN_08011a1a(0,s_Error__s__d_0801ba48,s____src_platform_freertos_tasks_c_0801ba24,0x788)
    ;
    return iVar7;
  }
  return iVar6 + 1;
}


// ======== FUN_0801885c @ 0801885c size=102 ========

void FUN_0801885c(int param_1,int param_2)

{
  int *piVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  
  piVar1 = DAT_080188c4;
  uVar4 = DAT_080188c4[3];
  iVar2 = FUN_0801b3ce(*DAT_080188c4 + 4);
  if (iVar2 == 0) {
    piVar1[4] = piVar1[4] & ~(1 << *(sbyte *)(*piVar1 + 0x2c));
  }
  if ((param_1 == -1) && (param_2 != 0)) {
    FUN_0801b43e(DAT_080188c8,*piVar1 + 4);
    return;
  }
  uVar3 = param_1 + uVar4;
  *(uint *)(*piVar1 + 4) = uVar3;
  if (uVar3 < uVar4) {
    FUN_0801b40e(piVar1[0xe],*piVar1 + 4);
    return;
  }
  FUN_0801b40e(piVar1[0xd],*piVar1 + 4);
  if (uVar3 < (uint)piVar1[10]) {
    piVar1[10] = uVar3;
  }
  return;
}


// ======== FUN_080188cc @ 080188cc size=190 ========

void FUN_080188cc(int param_1)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  
  FUN_0801b458();
  piVar1 = DAT_0801898c;
  DAT_0801898c[2] = DAT_0801898c[2] + 1;
  iVar2 = DAT_08018990;
  if (*piVar1 == 0) {
    *piVar1 = param_1;
    if (piVar1[2] == 1) {
      uVar4 = 0;
      do {
        FUN_0801b3f2(iVar2 + uVar4 * 0x14);
        uVar4 = uVar4 + 1;
      } while (uVar4 < 0x20);
      FUN_0801b3f2(DAT_08018994);
      FUN_0801b3f2(DAT_08018994 + 0x14);
      FUN_0801b3f2(DAT_08018994 + 0x28);
      FUN_0801b3f2(DAT_08018994 + 0x3c);
      FUN_0801b3f2(DAT_08018994 + 0x50);
      iVar3 = DAT_08018994;
      piVar1[0xd] = DAT_08018994;
      piVar1[0xe] = iVar3 + 0x14;
    }
  }
  else if ((piVar1[5] == 0) && (*(uint *)(*piVar1 + 0x2c) <= *(uint *)(param_1 + 0x2c))) {
    *piVar1 = param_1;
  }
  piVar1[9] = piVar1[9] + 1;
  uVar4 = *(uint *)(param_1 + 0x2c);
  piVar1[4] = 1 << (uVar4 & 0xff) | piVar1[4];
  FUN_0801b43e(iVar2 + uVar4 * 0x14,param_1 + 4);
  FUN_0801b4c4();
  if ((piVar1[5] != 0) && (*(uint *)(*piVar1 + 0x2c) < *(uint *)(param_1 + 0x2c))) {
    *DAT_08018998 = 0x10000000;
    DataSynchronizationBarrier(0xf);
    InstructionSynchronizationBarrier(0xf);
  }
  return;
}


// ======== FUN_080189ec @ 080189ec size=38 ========

void FUN_080189ec(undefined4 *param_1,undefined4 param_2)

{
  uint uVar1;
  
  if (param_1[0x10] != 0) {
    uVar1 = param_1[3] + param_1[0x10];
    param_1[3] = uVar1;
    if ((uint)param_1[1] <= uVar1) {
      param_1[3] = *param_1;
    }
    FUN_08010912(param_2);
    return;
  }
  return;
}


// ======== FUN_08018a12 @ 08018a12 size=108 ========

undefined4 FUN_08018a12(uint *param_1,undefined4 param_2,int param_3)

{
  uint uVar1;
  uint uVar2;
  undefined4 uVar3;
  
  uVar2 = param_1[0xe];
  uVar3 = 0;
  if (param_1[0x10] == 0) {
    if (*param_1 == 0) {
      uVar3 = FUN_0801c6a4(param_1[1]);
      param_1[1] = 0;
    }
  }
  else if (param_3 == 0) {
    FUN_08010912(param_1[2]);
    uVar1 = param_1[2];
    param_1[2] = uVar1 + param_1[0x10];
    if (param_1[1] <= uVar1 + param_1[0x10]) {
      param_1[2] = *param_1;
    }
  }
  else {
    FUN_08010912(param_1[3]);
    uVar1 = param_1[3] - param_1[0x10];
    param_1[3] = uVar1;
    if (uVar1 < *param_1) {
      param_1[3] = param_1[1] - param_1[0x10];
    }
    if ((param_3 == 2) && (uVar2 != 0)) {
      uVar2 = uVar2 - 1;
    }
  }
  param_1[0xe] = uVar2 + 1;
  return uVar3;
}


// ======== FUN_08018a7e @ 08018a7e size=20 ========

void FUN_08018a7e(int param_1)

{
  FUN_0801b51c(*(undefined4 *)(param_1 + 0x30));
  FUN_0801b51c(param_1);
  return;
}


// ======== FUN_08018a94 @ 08018a94 size=66 ========

void FUN_08018a94(void)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  int iVar3;
  undefined4 *puVar4;
  
  puVar1 = DAT_08018adc;
  iVar3 = 0x6c00;
  puVar2 = DAT_08018ad8;
  if (((uint)DAT_08018ad8 & 7) != 0) {
    puVar2 = (undefined4 *)((int)DAT_08018ad8 + 7U & 0xfffffff8);
    iVar3 = 0x6c00 - ((int)puVar2 - (int)DAT_08018ad8);
  }
  *DAT_08018adc = puVar2;
  puVar1[1] = 0;
  puVar4 = (undefined4 *)((int)puVar2 + iVar3 + -8 & 0xfffffff8);
  puVar1[-4] = puVar4;
  puVar4[1] = 0;
  *puVar4 = 0;
  iVar3 = (int)puVar4 - (int)puVar2;
  *puVar2 = puVar4;
  puVar2[1] = iVar3;
  puVar1[-2] = iVar3;
  puVar1[-3] = iVar3;
  puVar1[-1] = 0x80000000;
  return;
}


// ======== FUN_08018b08 @ 08018b08 size=150 ========

/* WARNING: Removing unreachable block (ram,0x08018b30) */

void FUN_08018b08(undefined4 param_1,int param_2,int param_3,undefined4 param_4,uint param_5,
                 undefined4 *param_6,undefined4 *param_7)

{
  int iVar1;
  uint uVar2;
  undefined4 uVar3;
  
  iVar1 = param_7[0xc];
  uVar2 = 0;
  param_7[0x11] = param_3;
  do {
    *(undefined1 *)((int)param_7 + uVar2 + 0x34) = *(undefined1 *)(param_2 + uVar2);
    if (*(char *)(param_2 + uVar2) == '\0') break;
    uVar2 = uVar2 + 1;
  } while (uVar2 < 0x10);
  *(undefined1 *)((int)param_7 + 0x43) = 0;
  if (0x1f < param_5) {
    param_5 = 0x1f;
  }
  param_7[0xb] = param_5;
  param_7[0x12] = param_5;
  param_7[0x13] = 0;
  FUN_0801b408(param_7 + 1);
  FUN_0801b408(param_7 + 6);
  param_7[4] = param_7;
  param_7[9] = param_7;
  param_7[6] = 0x20 - param_5;
  param_7[0x14] = 0;
  *(undefined1 *)(param_7 + 0x15) = 0;
  uVar3 = FUN_08018e50(iVar1 + param_3 * 4 + -4 & 0xfffffff8,param_1,param_4);
  *param_7 = uVar3;
  if (param_6 != (undefined4 *)0x0) {
    *param_6 = param_7;
  }
  return;
}


// ======== FUN_08018bd4 @ 08018bd4 size=72 ========

void FUN_08018bd4(uint *param_1)

{
  uint *puVar1;
  uint *puVar2;
  
  puVar2 = DAT_08018c1c;
  do {
    puVar1 = puVar2;
    puVar2 = (uint *)*puVar1;
  } while (puVar2 < param_1);
  if ((uint *)(puVar1[1] + (int)puVar1) == param_1) {
    puVar1[1] = param_1[1] + puVar1[1];
    param_1 = puVar1;
  }
  if ((uint *)(param_1[1] + (int)param_1) == puVar2) {
    if (puVar2 == (uint *)DAT_08018c1c[-4]) {
      *param_1 = DAT_08018c1c[-4];
      goto LAB_08018c10;
    }
    param_1[1] = puVar2[1] + param_1[1];
    puVar2 = *(uint **)*puVar1;
  }
  *param_1 = (uint)puVar2;
LAB_08018c10:
  if (puVar1 != param_1) {
    *puVar1 = (uint)param_1;
  }
  return;
}


// ======== FUN_08018c20 @ 08018c20 size=28 ========

bool FUN_08018c20(int param_1)

{
  int iVar1;
  
  FUN_0801b458();
  iVar1 = *(int *)(param_1 + 0x38);
  FUN_0801b4c4();
  return iVar1 == 0;
}


// ======== FUN_08018c3c @ 08018c3c size=26 ========

void FUN_08018c3c(void)

{
  undefined4 uVar1;
  
  if (**(int **)(DAT_08018c58 + 0x34) == 0) {
    uVar1 = 0xffffffff;
  }
  else {
    uVar1 = *(undefined4 *)(*(int *)(*(int *)(*(int *)(DAT_08018c58 + 0x34) + 0xc) + 0xc) + 4);
  }
  *(undefined4 *)(DAT_08018c58 + 0x28) = uVar1;
  return;
}


// ======== FUN_08018cb8 @ 08018cb8 size=106 ========

void FUN_08018cb8(int param_1)

{
  char cVar1;
  int iVar2;
  
  FUN_0801b458();
  cVar1 = *(char *)(param_1 + 0x45);
  while (('\0' < cVar1 && (*(int *)(param_1 + 0x24) != 0))) {
    iVar2 = FUN_0801c76c(param_1 + 0x24);
    if (iVar2 != 0) {
      FUN_0801b870();
    }
    cVar1 = cVar1 + -1;
  }
  *(undefined1 *)(param_1 + 0x45) = 0xff;
  FUN_0801b4c4();
  FUN_0801b458();
  cVar1 = *(char *)(param_1 + 0x44);
  while (('\0' < cVar1 && (*(int *)(param_1 + 0x10) != 0))) {
    iVar2 = FUN_0801c76c(param_1 + 0x10);
    if (iVar2 != 0) {
      FUN_0801b870();
    }
    cVar1 = cVar1 + -1;
  }
  *(undefined1 *)(param_1 + 0x44) = 0xff;
  FUN_0801b4c4();
  return;
}


// ======== FUN_08018d24 @ 08018d24 size=220 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint FUN_08018d24(uint param_1)

{
  int *piVar1;
  int *piVar2;
  int *piVar3;
  int *piVar4;
  int *piVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  int iVar9;
  
  uVar8 = 0;
  FUN_0801ba58();
  piVar1 = _DAT_08018e00;
  if (*_DAT_08018e00 == 0) {
    FUN_08018a94();
  }
  if (((param_1 & piVar1[3]) == 0) && (param_1 != 0)) {
    uVar6 = param_1 + 8;
    if (((param_1 & 7) != 0) && (uVar6 = uVar6 + (8 - (uVar6 & 7)), (uVar6 & 7) != 0)) {
      FUN_08011a1a(0,s_Error__s__d_08018e28,s____src_platform_freertos_heap_4__08018e03 + 1,0xbf);
    }
    if ((uVar6 != 0) && (uVar6 <= (uint)piVar1[1])) {
      piVar2 = _DAT_08018e00 + 4;
      piVar3 = (int *)_DAT_08018e00[4];
      do {
        piVar5 = piVar3;
        piVar4 = piVar2;
        if (uVar6 <= (uint)piVar5[1]) break;
        piVar2 = piVar5;
        piVar3 = (int *)*piVar5;
      } while ((int *)*piVar5 != (int *)0x0);
      if (piVar5 != (int *)*piVar1) {
        iVar9 = *piVar4;
        *piVar4 = *piVar5;
        uVar8 = iVar9 + 8;
        if (0x10 < piVar5[1] - uVar6) {
          uVar7 = (int)piVar5 + uVar6;
          if ((uVar7 & 7) != 0) {
            FUN_08011a1a(0,s_Error__s__d_08018e28,s____src_platform_freertos_heap_4__08018e03 + 1,
                         0xec);
          }
          *(uint *)(uVar7 + 4) = piVar5[1] - uVar6;
          piVar5[1] = uVar6;
          FUN_08018bd4(uVar7);
        }
        uVar7 = piVar5[1];
        uVar6 = piVar1[1] - uVar7;
        piVar1[1] = uVar6;
        if (uVar6 < (uint)piVar1[2]) {
          piVar1[2] = uVar6;
        }
        uVar6 = piVar1[3];
        *piVar5 = 0;
        piVar5[1] = uVar7 | uVar6;
      }
    }
  }
  FUN_0801c818();
  if ((uVar8 & 7) != 0) {
    FUN_08011a1a(0,s_Error__s__d_08018e28,s____src_platform_freertos_heap_4__08018e03 + 1,300);
  }
  return uVar8;
}


// ======== FUN_08018e38 @ 08018e38 size=18 ========

int FUN_08018e38(void)

{
  int *piVar1;
  
  piVar1 = DAT_08018e4c;
  if (*DAT_08018e4c != 0) {
    *(int *)(*DAT_08018e4c + 0x4c) = *(int *)(*DAT_08018e4c + 0x4c) + 1;
  }
  return *piVar1;
}


// ======== FUN_08018e50 @ 08018e50 size=40 ========

int FUN_08018e50(int param_1,uint param_2,undefined4 param_3)

{
  *(undefined4 *)(param_1 + -4) = 0x1000000;
  *(uint *)(param_1 + -8) = param_2 & 0xfffffffe;
  *(undefined4 *)(param_1 + -0xc) = DAT_08018e78;
  *(undefined4 *)(param_1 + -0x20) = param_3;
  *(undefined4 *)(param_1 + -0x24) = 0xfffffffd;
  return param_1 + -0x44;
}


// ======== FUN_08018e7c @ 08018e7c size=62 ========

void FUN_08018e7c(uint param_1)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  
  iVar1 = DAT_08018ec8;
  uVar2 = *(uint *)(DAT_08018ec8 + 4) & DAT_08018ecc;
  uVar3 = *(uint *)(DAT_08018ec8 + 0x2c) & 0xdfffffff;
  switch(param_1) {
  case 0:
  case 1:
  case 2:
  case 3:
    uVar2 = uVar2 | param_1 << 0xe;
    break;
  case 5:
  case 7:
    uVar2 = uVar2 | (param_1 & 0xfffffffb) << 0xe | 0x10000000;
    break;
  case 8:
  case 9:
  case 10:
  case 0xb:
    uVar2 = uVar2 | (param_1 & 0xfffffff3) << 0xe;
    uVar3 = uVar3 | 0x20000000;
  }
  *(uint *)(DAT_08018ec8 + 4) = uVar2;
  *(uint *)(iVar1 + 0x2c) = uVar3;
  return;
}


// ======== FUN_08018ed0 @ 08018ed0 size=12 ========

void FUN_08018ed0(void)

{
  *(uint *)(DAT_08018edc + 0x24) = *(uint *)(DAT_08018edc + 0x24) | 0x1000000;
  return;
}


// ======== FUN_08018ee0 @ 08018ee0 size=12 ========

void FUN_08018ee0(void)

{
  *(uint *)(DAT_08018eec + 0x20) = *(uint *)(DAT_08018eec + 0x20) & 0xfffeffff;
  return;
}


// ======== FUN_08018ef0 @ 08018ef0 size=12 ========

void FUN_08018ef0(void)

{
  *(uint *)(DAT_08018efc + 0x20) = *(uint *)(DAT_08018efc + 0x20) | 0x10000;
  return;
}


// ======== FUN_08018f00 @ 08018f00 size=222 ========

uint FUN_08018f00(int param_1)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  undefined4 local_30;
  undefined4 uStack_2c;
  undefined4 uStack_28;
  undefined4 uStack_24;
  undefined4 local_20;
  undefined4 uStack_1c;
  undefined4 local_18;
  undefined4 uStack_14;
  
  local_30 = DAT_08018fe0;
  uStack_2c = DAT_08018fe4;
  uStack_28 = DAT_08018fe8;
  uStack_24 = DAT_08018fec;
  local_20 = DAT_08018fe4;
  uStack_1c = DAT_08018fe8;
  local_18 = DAT_08018fe4;
  uStack_14 = DAT_08018fe8;
  uVar2 = (*(uint *)(DAT_08018ff0 + 4) & 0xf) >> 2;
  uVar1 = DAT_08018ff4;
  if (((uVar2 == 0) || (uVar2 == 1)) || (uVar2 != 2)) goto LAB_08018f94;
  if ((*(uint *)(DAT_08018ff0 + 4) & 0x10000) == 0x10000) {
    if ((*(uint *)(DAT_08018ff0 + 0x2c) & 0x40000000) != 0) {
      uVar1 = DAT_08018ffc;
    }
    if ((*(uint *)(DAT_08018ff0 + 4) & 0x20000) == 0x20000) goto LAB_08018f60;
  }
  else {
LAB_08018f60:
    uVar1 = DAT_08018ff8;
  }
  uVar2 = (*(uint *)(DAT_08018ff0 + 4) & 0x3fffff) >> 0x12;
  if (*(int *)(DAT_08018ff0 + 4) << 4 < 0) {
    uVar2 = uVar2 | 0x10;
  }
  if (*(int *)(DAT_08018ff0 + 4) << 1 < 0) {
    uVar2 = uVar2 | 0x20;
  }
  if (uVar2 < 0xf) {
    iVar3 = uVar2 + 2;
  }
  else if (uVar2 - 0xf < 0x30) {
    iVar3 = uVar2 + 1;
  }
  else {
    iVar3 = 0x3f;
  }
  uVar1 = uVar1 * iVar3;
LAB_08018f94:
  uVar2 = uVar1 >> *(sbyte *)((int)&local_30 + ((*(uint *)(DAT_08018ff0 + 4) & 0xff) >> 4));
  if (((param_1 != 0) && (uVar1 = uVar2, param_1 != 1)) &&
     ((uVar1 = uVar2 >> *(sbyte *)((int)&local_20 + ((*(uint *)(DAT_08018ff0 + 4) & 0x7ff) >> 8)),
      param_1 != 2 && (uVar1 = 0, param_1 == 3)))) {
    uVar1 = uVar2 >> *(sbyte *)((int)&local_18 + ((*(uint *)(DAT_08018ff0 + 4) & 0x3fff) >> 0xb));
  }
  return uVar1;
}


// ======== FUN_08019000 @ 08019000 size=28 ========

undefined4 FUN_08019000(uint param_1)

{
  if ((*(uint *)(DAT_0801901c + (param_1 >> 6)) & 1 << (param_1 & 0x1f)) != 0) {
    return 1;
  }
  return 0;
}


// ======== FUN_08019020 @ 08019020 size=12 ========

void FUN_08019020(void)

{
  *DAT_0801902c = *DAT_0801902c | 0x80000;
  return;
}


// ======== FUN_08019030 @ 08019030 size=22 ========

void FUN_08019030(uint param_1)

{
  uint *puVar1;
  
  puVar1 = (uint *)(DAT_08019048 + (param_1 >> 6));
  *puVar1 = *puVar1 & ~(1 << (param_1 & 0x1f));
  return;
}


// ======== FUN_0801904c @ 0801904c size=22 ========

void FUN_0801904c(uint param_1)

{
  uint *puVar1;
  
  puVar1 = (uint *)(DAT_08019064 + (param_1 >> 6));
  *puVar1 = *puVar1 | 1 << (param_1 & 0x1f);
  return;
}


// ======== FUN_08019068 @ 08019068 size=192 ========

bool FUN_08019068(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  int extraout_r3;
  int extraout_r3_00;
  int extraout_r3_01;
  int extraout_r3_02;
  int extraout_r3_03;
  int extraout_r3_04;
  
  iVar3 = DAT_08019128;
  if (param_1 == 0x800) {
    uVar2 = 0x801;
    do {
      iVar3 = FUN_08019000(0x801);
      if (iVar3 != 0) break;
    } while (extraout_r3_00 != 0x3fffffe);
  }
  else if (param_1 < 0x801) {
    if (param_1 == 0) {
      do {
        iVar3 = FUN_08019000(1);
        if (iVar3 != 0) break;
      } while (extraout_r3_01 != 0xfffe);
      uVar2 = 1;
    }
    else if (param_1 == 0x10) {
      do {
        iVar3 = FUN_08019000(0x11);
        if (iVar3 != 0) break;
      } while (extraout_r3 != 0xfffe);
      uVar2 = 0x11;
    }
    else {
      if (param_1 != 0x18) {
        return false;
      }
      do {
        iVar1 = FUN_08019000(0x19);
        if (iVar1 != 0) break;
      } while (extraout_r3_04 + 1 != iVar3);
      uVar2 = 0x19;
    }
  }
  else if (param_1 == 0x900) {
    uVar2 = 0x901;
    do {
      iVar1 = FUN_08019000(0x901);
      if (iVar1 != 0) break;
    } while (extraout_r3_03 + 1 != iVar3);
  }
  else {
    if (param_1 != 0x3010) {
      return false;
    }
    uVar2 = 0x3011;
    do {
      iVar1 = FUN_08019000(0x3011);
      if (iVar1 != 0) break;
    } while (extraout_r3_02 + 1 != iVar3);
  }
  iVar3 = FUN_08019000(uVar2);
  return iVar3 != 0;
}


// ======== FUN_0801912c @ 0801912c size=22 ========

void FUN_0801912c(uint param_1)

{
  uint *puVar1;
  
  puVar1 = (uint *)(DAT_08019144 + (param_1 >> 6));
  *puVar1 = *puVar1 & ~(1 << (param_1 & 0x1f));
  return;
}


// ======== FUN_08019148 @ 08019148 size=22 ========

void FUN_08019148(uint param_1)

{
  uint *puVar1;
  
  puVar1 = (uint *)(DAT_08019160 + (param_1 >> 6));
  *puVar1 = *puVar1 | 1 << (param_1 & 0x1f);
  return;
}


// ======== FUN_08019180 @ 08019180 size=22 ========

void FUN_08019180(uint param_1)

{
  uint *puVar1;
  
  puVar1 = (uint *)(DAT_08019198 + (param_1 >> 6));
  *puVar1 = *puVar1 | 1 << (param_1 & 0x1f);
  return;
}


// ======== FUN_0801919c @ 0801919c size=14 ========

void FUN_0801919c(uint param_1)

{
  *(uint *)(DAT_080191ac + 0x20) = *(uint *)(DAT_080191ac + 0x20) & 0xfffffcff | param_1;
  return;
}


// ======== FUN_080191b0 @ 080191b0 size=14 ========

void FUN_080191b0(uint param_1)

{
  *(uint *)(DAT_080191c0 + 4) = *(uint *)(DAT_080191c0 + 4) & 0xfffffffc | param_1;
  return;
}


// ======== FUN_080191c4 @ 080191c4 size=10 ========

uint FUN_080191c4(void)

{
  return *(uint *)(DAT_080191d0 + 4) & 0xc;
}


// ======== FUN_080191d4 @ 080191d4 size=102 ========

void FUN_080191d4(undefined4 *param_1,uint param_2)

{
  int *piVar1;
  undefined4 *puVar2;
  int iVar3;
  uint uVar4;
  int iVar5;
  undefined4 uVar6;
  undefined8 uVar7;
  
  if (param_1 != (undefined4 *)0x0) {
    iVar5 = param_1[-1];
    uVar6 = *param_1;
    uVar4 = iVar5 - 4;
    if (param_2 < uVar4) {
      uVar4 = param_2;
    }
    uVar7 = FUN_08011e30(param_1);
    iVar3 = (int)((ulonglong)uVar7 >> 0x20);
    piVar1 = (int *)uVar7;
    puVar2 = (undefined4 *)FUN_080177fc(param_2,param_1,uVar4);
    if (puVar2 == (undefined4 *)0x0) {
      param_1[-1] = iVar5;
      *param_1 = uVar6;
      if (piVar1 == (int *)0x0) {
        *DAT_0801923c = iVar3;
      }
      else {
        if (param_1 + -1 < (undefined4 *)((int)piVar1 + *piVar1)) {
          *piVar1 = (int)param_1 + (-4 - (int)piVar1);
        }
        piVar1[1] = iVar3;
      }
    }
    else {
      *puVar2 = uVar6;
    }
    return;
  }
  FUN_08011eb8(param_2);
  return;
}


// ======== FUN_08019240 @ 08019240 size=46 ========

undefined4 * FUN_08019240(void)

{
  int iVar1;
  undefined4 *puVar2;
  
  iVar1 = DAT_08019270;
  if (*(int *)(DAT_08019270 + 4) == 0) {
    puVar2 = (undefined4 *)FUN_080195b8(0x18);
    *(undefined4 **)(iVar1 + 4) = puVar2;
    if (puVar2 != (undefined4 *)0x0) {
      *puVar2 = DAT_08019274;
      puVar2[1] = DAT_08019278;
      puVar2[2] = DAT_0801927c;
      puVar2[3] = DAT_08019280;
      puVar2[4] = DAT_08019284;
      return puVar2;
    }
  }
  return (undefined4 *)0x0;
}


// ======== FUN_08019288 @ 08019288 size=42 ========

undefined4 * FUN_08019288(void)

{
  int *piVar1;
  undefined4 *puVar2;
  
  piVar1 = DAT_080192b4;
  if (*DAT_080192b4 == 0) {
    puVar2 = (undefined4 *)FUN_080195b8(0x14);
    *piVar1 = (int)puVar2;
    if (puVar2 != (undefined4 *)0x0) {
      *puVar2 = DAT_080192b8;
      puVar2[1] = DAT_080192bc;
      puVar2[2] = DAT_080192c0;
      puVar2[3] = DAT_080192c4;
      return puVar2;
    }
  }
  return (undefined4 *)0x0;
}


// ======== FUN_080192c8 @ 080192c8 size=38 ========

undefined4 * FUN_080192c8(void)

{
  int iVar1;
  undefined4 *puVar2;
  
  iVar1 = DAT_080192f0;
  if (*(int *)(DAT_080192f0 + 4) == 0) {
    puVar2 = (undefined4 *)FUN_080195b8(0x10);
    *(undefined4 **)(iVar1 + 4) = puVar2;
    if (puVar2 != (undefined4 *)0x0) {
      *puVar2 = DAT_080192f4;
      puVar2[1] = DAT_080192f8;
      puVar2[2] = DAT_080192fc;
      return puVar2;
    }
  }
  return (undefined4 *)0x0;
}


// ======== FUN_08019300 @ 08019300 size=42 ========

int FUN_08019300(void)

{
  int *piVar1;
  int iVar2;
  undefined4 *puVar3;
  
  piVar1 = DAT_0801932c;
  if ((*DAT_0801932c == 0) && (iVar2 = FUN_080138dc(), iVar2 != 0)) {
    puVar3 = (undefined4 *)piVar1[1];
    *puVar3 = DAT_08019330;
    puVar3[1] = DAT_08019334;
    puVar3[2] = DAT_08019338;
    puVar3[3] = DAT_0801933c;
    return *piVar1;
  }
  return 0;
}


// ======== FUN_08019340 @ 08019340 size=46 ========

undefined4 * FUN_08019340(void)

{
  int *piVar1;
  undefined4 *puVar2;
  
  piVar1 = DAT_08019370;
  if (*DAT_08019370 == 0) {
    puVar2 = (undefined4 *)FUN_080195b8(0x18);
    *piVar1 = (int)puVar2;
    if (puVar2 != (undefined4 *)0x0) {
      *puVar2 = DAT_08019374;
      puVar2[1] = DAT_08019378;
      puVar2[2] = DAT_0801937c;
      puVar2[3] = DAT_08019380;
      puVar2[4] = DAT_08019384;
      return puVar2;
    }
  }
  return (undefined4 *)0x0;
}


// ======== FUN_08019388 @ 08019388 size=238 ========

undefined4 FUN_08019388(char *param_1,uint *param_2,undefined4 param_3,int *param_4,int param_5)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  undefined4 uVar6;
  
  uVar3 = *param_2;
  uVar6 = 0xffffffff;
  if (uVar3 < 4) {
    return 0xffffffff;
  }
  if ((*param_1 == '&') && (param_1[uVar3 - 1] == '#')) {
    if (param_5 == 1) {
      *param_4 = uVar3 - 3;
      uVar2 = FUN_08013008(param_1 + 1);
      if ((byte)param_1[uVar3 - 2] != uVar2) {
        return 0xffffffff;
      }
      iVar5 = *param_4;
    }
    else {
      iVar5 = uVar3 - 2;
      *param_4 = iVar5;
    }
  }
  else {
    uVar2 = 0;
    if (param_5 == 1) {
      for (; (uVar2 < uVar3 && (param_1[uVar2] != '&')); uVar2 = uVar2 + 1) {
      }
      uVar4 = uVar2;
      if (uVar3 - 2 <= uVar2) {
        return 0xffffffff;
      }
      do {
        uVar1 = uVar4;
        uVar4 = uVar1 + 1;
        if (uVar3 <= uVar4) break;
      } while (param_1[uVar4] != '#');
      if (uVar3 <= uVar4) {
        return 0xffffffff;
      }
      if ((int)(uVar1 + 2) <= (int)(uVar2 + 3)) {
        return 0xffffffff;
      }
      *param_4 = (uVar4 - uVar2) + -2;
      uVar3 = FUN_08013008(param_1 + uVar2 + 1);
      if ((byte)param_1[uVar1] != uVar3) {
        return 0xffffffff;
      }
      iVar5 = *param_4;
      param_1 = param_1 + uVar2;
    }
    else {
      for (; (uVar2 < uVar3 && (param_1[uVar2] != '&')); uVar2 = uVar2 + 1) {
      }
      uVar4 = uVar2;
      if (uVar3 - 2 <= uVar2) {
        return 0xffffffff;
      }
      do {
        uVar1 = uVar4;
        uVar4 = uVar1 + 1;
        if (uVar3 <= uVar4) break;
      } while (param_1[uVar4] != '#');
      if (uVar3 <= uVar4) {
        return 0xffffffff;
      }
      if ((int)(uVar1 + 2) <= (int)(uVar2 + 2)) {
        return 0xffffffff;
      }
      iVar5 = (uVar4 - uVar2) + -1;
      *param_4 = iVar5;
      param_1 = param_1 + uVar2;
    }
  }
  iVar5 = FUN_08010912(param_3,param_1 + 1,iVar5);
  if (iVar5 != 0) {
    uVar6 = 0;
  }
  return uVar6;
}


// ======== FUN_08019478 @ 08019478 size=12 ========

void FUN_08019478(void)

{
  *DAT_08019484 = *DAT_08019484 | 0x10;
  return;
}


// ======== FUN_08019498 @ 08019498 size=16 ========

uint FUN_08019498(void)

{
  return *DAT_080194a8 | DAT_080194a8[-1] << 0x10;
}


// ======== FUN_080194cc @ 080194cc size=10 ========

void FUN_080194cc(uint param_1)

{
  *DAT_080194d8 = *DAT_080194d8 & ~param_1;
  return;
}


// ======== FUN_080194dc @ 080194dc size=16 ========

undefined4 FUN_080194dc(uint param_1)

{
  if ((*DAT_080194ec & param_1) != 0) {
    return 1;
  }
  return 0;
}


// ======== FUN_080194f0 @ 080194f0 size=10 ========

void FUN_080194f0(uint param_1)

{
  *DAT_080194fc = *DAT_080194fc | param_1;
  return;
}


// ======== FUN_08019500 @ 08019500 size=10 ========

void FUN_08019500(void)

{
  do {
  } while (-1 < *DAT_0801950c << 0x1a);
  return;
}


// ======== FUN_08019510 @ 08019510 size=42 ========

void FUN_08019510(undefined4 param_1,undefined4 param_2)

{
  uint *puVar1;
  uint extraout_r2;
  
  FUN_08019478(param_1,param_2,param_1);
  puVar1 = DAT_08019530;
  *DAT_08019530 = (extraout_r2 & 0xfffff) >> 0x10;
  puVar1[1] = extraout_r2 & 0xffff;
  *DAT_08019494 = *DAT_08019494 & 0xffffffef;
  return;
}


// ======== FUN_08019534 @ 08019534 size=18 ========

void FUN_08019534(void)

{
  uint *puVar1;
  
  puVar1 = DAT_08019548;
  *DAT_08019548 = *DAT_08019548 & 0xfffffff7;
  do {
  } while (-1 < (int)(*puVar1 << 0x1c));
  return;
}


// ======== FUN_08019584 @ 08019584 size=28 ========

void FUN_08019584(int *param_1)

{
  int *piVar1;
  int iVar2;
  
  piVar1 = DAT_080195a0;
  if (param_1 == (int *)0x0) {
    *DAT_080195a0 = DAT_080195a4;
  }
  else {
    iVar2 = *param_1;
    if (*param_1 == 0) {
      iVar2 = DAT_080195a4;
    }
    *DAT_080195a0 = iVar2;
    iVar2 = param_1[1];
    if (param_1[1] != 0) goto LAB_08019596;
  }
  iVar2 = DAT_080195a8;
LAB_08019596:
  piVar1[1] = iVar2;
  return;
}


// ======== FUN_080195ac @ 080195ac size=6 ========

void FUN_080195ac(void)

{
                    /* WARNING: Could not recover jumptable at 0x080195b0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(DAT_080195b4 + 4))();
  return;
}


// ======== FUN_080195b8 @ 080195b8 size=6 ========

void FUN_080195b8(void)

{
                    /* WARNING: Could not recover jumptable at 0x080195bc. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)*DAT_080195c0)();
  return;
}


// ======== FUN_08019914 @ 08019914 size=850 ========

void FUN_08019914(void)

{
  char cVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int iVar5;
  undefined4 extraout_r1;
  undefined4 extraout_r1_00;
  undefined4 extraout_r1_01;
  uint uVar6;
  undefined4 in_r3;
  
  iVar5 = *(int *)(DAT_0801995c + 4);
  if (*(char *)(iVar5 + 0x13) == -1) {
    *(undefined1 *)(iVar5 + 0x13) = 0;
  }
  else {
    uVar6 = (uint)*(byte *)(iVar5 + 0x12);
    if (uVar6 == 0) {
      *(undefined1 *)(iVar5 + 0x12) = 0xff;
      *(undefined1 *)(iVar5 + 0x13) = 0xff;
      iVar5 = FUN_08012e20();
      if (iVar5 == 0) {
        FUN_080141e8(1,DAT_08019bd4 + 0x4c,DAT_08019bd4 + 0x28,DAT_08019bf0,0x1ef,DAT_08019bd4 + 4);
      }
      else {
        FUN_08012d78((int)DAT_08019bcc,iVar5,DAT_08019bd4);
        FUN_08012d78((int)DAT_08019bd8,iVar5,&DAT_08019be0);
        FUN_08012d78((int)DAT_08019be4,iVar5,&DAT_08019bec);
        iVar2 = FUN_08012fd4(iVar5);
        FUN_08012e38(iVar5);
        if (iVar2 != 0) {
          iVar5 = FUN_08016c98();
          (**(code **)(iVar5 + 0x10))(iVar2);
          FUN_080195ac(iVar2,extraout_r1_01,uVar6,in_r3);
          return;
        }
      }
      return;
    }
    if (*(char *)(iVar5 + 0x14) == '\0') {
      *(undefined1 *)(iVar5 + 0x14) = 0xff;
      *(undefined1 *)(iVar5 + 0x13) = 0xff;
      return;
    }
    uVar6 = (uint)*(byte *)(iVar5 + 0x18);
    if (uVar6 == 0) {
      *(undefined1 *)(iVar5 + 0x18) = 0xff;
      *(undefined1 *)(iVar5 + 0x13) = 0xff;
      iVar5 = FUN_08012e20();
      if (iVar5 == 0) {
        FUN_080141e8(1,DAT_080199c0 + 0x4c,DAT_080199c0 + 0x28,DAT_080199c4,0x202,DAT_080199c0 + 4);
      }
      else {
        FUN_08012d78((int)DAT_080199b8,iVar5,DAT_080199c0);
        iVar2 = FUN_08012fd4(iVar5);
        FUN_08012e38(iVar5);
        if (iVar2 != 0) {
          iVar5 = FUN_08016c98();
          (**(code **)(iVar5 + 0x10))(iVar2);
          FUN_080195ac(iVar2,extraout_r1_00,uVar6,in_r3);
          return;
        }
      }
      return;
    }
    uVar6 = (uint)*(byte *)(iVar5 + 0x19);
    if (uVar6 == 0) {
      *(undefined1 *)(iVar5 + 0x19) = 0xff;
      *(undefined1 *)(iVar5 + 0x13) = 0xff;
      iVar5 = FUN_08012e20();
      if (iVar5 == 0) {
        FUN_080141e8(1,&DAT_08019908,s____src_application_process_comm__080198e4,DAT_080198e0,0x214,
                     s_create_cjson_config_data_failed_080198c0);
      }
      else {
        FUN_08012d78((int)DAT_080198b4,iVar5,&DAT_080198bc);
        iVar2 = FUN_08012fd4(iVar5);
        FUN_08012e38(iVar5);
        if (iVar2 != 0) {
          iVar5 = FUN_08016c98();
          (**(code **)(iVar5 + 0x10))(iVar2);
          FUN_080195ac(iVar2,extraout_r1,uVar6,in_r3);
          return;
        }
      }
      return;
    }
  }
  iVar5 = FUN_08012e20();
  if (iVar5 == 0) {
    FUN_080141e8(1,&DAT_08019814,s____src_application_process_comm__080197f0,DAT_080197ec,0x188,
                 s_create_cjson_config_data_failed_080197cc);
    return;
  }
  FUN_08012d78((int)DAT_080197b8,iVar5,&DAT_080197c0);
  iVar2 = FUN_08016ba8();
  iVar2 = (**(code **)(iVar2 + 8))();
  uVar4 = (undefined4)DAT_080197c4;
  if (*(char *)(iVar2 + 1) != '\x03') {
    iVar2 = FUN_08016ba8();
    iVar2 = (**(code **)(iVar2 + 8))();
    uVar3 = uVar4;
    if (*(char *)(iVar2 + 1) != '\x02') {
      iVar2 = FUN_08016ba8();
      iVar2 = (**(code **)(iVar2 + 8))();
      cVar1 = *(char *)(iVar2 + 1);
      iVar2 = FUN_08016ba8();
      iVar2 = (**(code **)(iVar2 + 8))();
      iVar2 = *(int *)(iVar2 + 4);
      if (cVar1 == '\0') {
        if (0 < iVar2) goto LAB_0801964c;
      }
      else if ((cVar1 == '\x01') && (iVar2 < 0)) {
LAB_0801964c:
        iVar2 = -iVar2;
        FUN_080141e8(2,&DAT_08019814,s____src_application_process_comm__080197f0,DAT_080197ec,0x198,
                     s_left_wave_data_error__area__d__s_0801981c,cVar1,iVar2);
      }
      uVar3 = FUN_08010e52(iVar2);
    }
    FUN_08012d78(uVar3,iVar5,&DAT_08019844);
  }
  iVar2 = FUN_08016ba8();
  iVar2 = (**(code **)(iVar2 + 0xc))();
  if (*(char *)(iVar2 + 1) == '\x03') goto LAB_080196f4;
  iVar2 = FUN_08016ba8();
  iVar2 = (**(code **)(iVar2 + 0xc))();
  uVar3 = uVar4;
  if (*(char *)(iVar2 + 1) != '\x02') {
    iVar2 = FUN_08016ba8();
    iVar2 = (**(code **)(iVar2 + 0xc))();
    cVar1 = *(char *)(iVar2 + 1);
    iVar2 = FUN_08016ba8();
    iVar2 = (**(code **)(iVar2 + 0xc))();
    iVar2 = *(int *)(iVar2 + 4);
    if (cVar1 == '\0') {
      if (0 < iVar2) goto LAB_080196c6;
    }
    else if ((cVar1 == '\x01') && (iVar2 < 0)) {
LAB_080196c6:
      iVar2 = -iVar2;
      FUN_080141e8(2,&DAT_08019814,s____src_application_process_comm__080197f0,DAT_080197ec,0x1a6,
                   s_left_wave_data_error__area__d__s_0801981c,cVar1,iVar2);
    }
    uVar3 = FUN_08010e52(iVar2);
  }
  FUN_08012d78(uVar3,iVar5,&DAT_08019848);
LAB_080196f4:
  uVar3 = FUN_08010e74(*(undefined1 *)(*(int *)(DAT_0801984c + 4) + 0xe));
  FUN_08012d78(uVar3,iVar5,&DAT_08019850);
  iVar2 = FUN_08016ba8();
  iVar2 = (**(code **)(iVar2 + 0x10))();
  uVar3 = uVar4;
  if (*(char *)(iVar2 + 1) != '\x02') {
    iVar2 = FUN_08016ba8();
    iVar2 = (**(code **)(iVar2 + 0x10))();
    if (*(char *)(iVar2 + 1) != '\x03') {
      iVar2 = FUN_08016ba8();
      iVar2 = (**(code **)(iVar2 + 0x10))();
      uVar3 = FUN_08010e52(*(undefined4 *)(iVar2 + 4));
    }
  }
  FUN_08012d78(uVar3,iVar5,&DAT_08019854);
  iVar2 = FUN_08016ba8();
  iVar2 = (**(code **)(iVar2 + 0x14))();
  if (*(char *)(iVar2 + 1) != '\x02') {
    iVar2 = FUN_08016ba8();
    iVar2 = (**(code **)(iVar2 + 0x14))();
    if (*(char *)(iVar2 + 1) != '\x03') {
      iVar2 = FUN_08016ba8();
      iVar2 = (**(code **)(iVar2 + 0x14))();
      uVar4 = FUN_08010e52(*(undefined4 *)(iVar2 + 4));
    }
  }
  FUN_08012d78(uVar4,iVar5,&DAT_08019858);
  iVar2 = FUN_08012fd4(iVar5);
  FUN_08012e38(iVar5);
  if (iVar2 == 0) {
    return;
  }
  iVar5 = FUN_08016c98();
  (**(code **)(iVar5 + 0x10))(iVar2);
  FUN_080195ac(iVar2);
  return;
}


// ======== FUN_080199cc @ 080199cc size=70 ========

undefined4 FUN_080199cc(undefined4 param_1)

{
  int iVar1;
  int iVar2;
  undefined1 auStack_98 [128];
  undefined4 local_18 [2];
  
  iVar2 = FUN_08010982();
  iVar1 = DAT_08019a14;
  if (iVar2 != 0) {
    local_18[0] = 0;
    iVar2 = FUN_08013b30(param_1,iVar2,auStack_98,local_18,
                         (int)*(char *)(*(int *)(DAT_08019a14 + 4) + 0x20),
                         (int)*(char *)(*(int *)(DAT_08019a14 + 4) + 0x28));
    if (iVar2 == 0) {
      (**(code **)(*(int *)(iVar1 + 4) + 4))(auStack_98,local_18[0]);
      return 0;
    }
  }
  return 0xffffffff;
}


// ======== FUN_08019a18 @ 08019a18 size=158 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_08019a18(undefined4 param_1,uint param_2,undefined4 param_3,undefined4 param_4)

{
  char *pcVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  uint uVar6;
  
  uVar2 = _DAT_08019b28;
  uVar5 = 0xffffffff;
  if (param_2 < 0x81) {
    if (param_2 == 0) {
      pcVar1 = s__send_len_0_08019b2b;
      uVar6 = 0x49;
    }
    else {
      iVar4 = 50000;
      uVar6 = param_2;
      while (iVar3 = FUN_0801b246(uVar2,6), iVar3 == 0) {
        if (iVar4 == 0) goto LAB_08019a9e;
        iVar4 = iVar4 + -1;
      }
      if (iVar4 != 0) {
        FUN_0801b232(uVar2,6);
        FUN_08010948(DAT_08019b38,0x80);
        FUN_08010912(DAT_08019b38,param_1,param_2);
        uVar2 = _DAT_08019b3c;
        FUN_08013d96(_DAT_08019b3c,1);
        FUN_08013f74(uVar2,1,param_2);
        FUN_08013db6(uVar2,1);
        uVar5 = 0;
        goto LAB_08019a96;
      }
LAB_08019a9e:
      pcVar1 = s__wait_send_string_overtime_08019b3f;
      uVar6 = 0x53;
    }
    FUN_080141e8(2,s_mport_drv_08019b1c,s____src_platform_driver_driver_po_08019adc,DAT_08019ad8,
                 uVar6,pcVar1 + 1,param_4);
  }
  else {
    uVar6 = 0x44;
    FUN_080141e8(2,s_mport_drv_08019b1c,s____src_platform_driver_driver_po_08019adc,DAT_08019ad8,
                 0x44,s_send_string_length__d_too_long_08019ab8,param_2);
  }
LAB_08019a96:
  return CONCAT44(uVar6,uVar5);
}


// ======== FUN_08019bf4 @ 08019bf4 size=260 ========

void FUN_08019bf4(void)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = DAT_08019cf0;
  FUN_08012aa2(DAT_08019cf0,0x100);
  FUN_08012aa2(iVar1,2,0);
  uVar2 = DAT_08019cf4;
  FUN_08012aa2(DAT_08019cf4,0x100,1);
  FUN_08012aa2(uVar2,2,0);
  FUN_08012900(iVar1,1,0x60000);
  FUN_08012900(uVar2,1,0xe0000);
  FUN_08012900(iVar1,2,0x7000);
  FUN_0801285e(iVar1,0);
  FUN_0801285e(uVar2,0);
  FUN_080129ec(0x20000);
  FUN_08012828(iVar1,1);
  FUN_08012828(uVar2,1);
  FUN_08012828(iVar1,2,1);
  FUN_08012a04(iVar1,0,5,7);
  FUN_08012a04(uVar2,0,9,7);
  FUN_080128ca(iVar1,1);
  FUN_080128ca(uVar2,1);
  FUN_0801292e(iVar1,0,7);
  FUN_080128ca(iVar1,2,1);
  FUN_080128ba(iVar1);
  FUN_0801b65c(1);
  FUN_0801280a(iVar1);
  FUN_080128ba(uVar2);
  FUN_0801b65c(1);
  FUN_0801280a(uVar2);
  *(uint *)(iVar1 + 8) = *(uint *)(iVar1 + 8) | 0x100;
  return;
}


// ======== FUN_08019d34 @ 08019d34 size=108 ========

void FUN_08019d34(void)

{
  undefined4 uVar1;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 uStack_14;
  undefined1 local_10;
  undefined1 local_f;
  undefined1 local_e;
  
  uVar1 = DAT_08019da0;
  FUN_08013e16(DAT_08019da0,0);
  local_28 = DAT_08019da4;
  local_10 = 0;
  local_f = 1;
  local_24 = 0x200;
  local_1c = 0x800;
  local_18 = 0;
  uStack_14 = 0x2000;
  local_20 = 0;
  local_e = 0;
  FUN_08013e4c(uVar1,0,&local_28);
  FUN_08013df6(uVar1,0);
  FUN_08013d96(uVar1,0);
  FUN_08017bc4(0xb,5,0);
  FUN_08013ec4(uVar1,0,2);
  FUN_08013ee4(uVar1,0,2);
  return;
}


// ======== FUN_08019e34 @ 08019e34 size=126 ========

void FUN_08019e34(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar1 = DAT_08019eb4;
  iVar2 = *(int *)(DAT_08019eb4 + 4);
  *(undefined2 *)(iVar2 + 0x92) = 800;
  if (*(int *)(iVar2 + 0x48) != 0) {
    FUN_080195ac();
    *(undefined4 *)(*(int *)(iVar1 + 4) + 0x48) = 0;
  }
  if (*(int *)(*(int *)(iVar1 + 4) + 0x68) != 0) {
    FUN_080195ac();
    *(undefined4 *)(*(int *)(iVar1 + 4) + 0x68) = 0;
  }
  if (*(int *)(*(int *)(iVar1 + 4) + 0x70) != 0) {
    FUN_080195ac();
  }
  iVar2 = FUN_080195b8((uint)*(ushort *)(*(int *)(iVar1 + 4) + 0x92) << 1);
  iVar3 = *(int *)(iVar1 + 4);
  *(int *)(iVar3 + 0x48) = iVar2;
  if (iVar2 != 0) {
    iVar2 = FUN_080195b8((uint)*(ushort *)(iVar3 + 0x92) << 1);
    iVar3 = *(int *)(iVar1 + 4);
    *(int *)(iVar3 + 0x68) = iVar2;
    if (iVar2 != 0) {
      iVar2 = FUN_080195b8((uint)*(ushort *)(iVar3 + 0x92) << 2);
      iVar3 = *(int *)(iVar1 + 4);
      *(int *)(iVar3 + 0x70) = iVar2;
      if (iVar2 != 0) {
        (**(code **)(*(int *)(iVar3 + 0x74) + 4))(iVar2,*(undefined2 *)(iVar3 + 0x92));
        *(undefined1 *)(*(int *)(iVar1 + 4) + 0xa1) = 0;
      }
    }
  }
  return;
}


// ======== FUN_08019eb8 @ 08019eb8 size=116 ========

void FUN_08019eb8(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar1 = DAT_08019f2c;
  iVar2 = *(int *)(DAT_08019f2c + 4);
  *(undefined2 *)(iVar2 + 0x92) = 0xeb;
  if (*(int *)(iVar2 + 0x48) != 0) {
    FUN_080195ac();
  }
  if (*(int *)(*(int *)(iVar1 + 4) + 0x68) != 0) {
    FUN_080195ac();
  }
  if (*(int *)(*(int *)(iVar1 + 4) + 0x70) != 0) {
    FUN_080195ac();
  }
  iVar2 = FUN_080195b8((uint)*(ushort *)(*(int *)(iVar1 + 4) + 0x92) << 1);
  iVar3 = *(int *)(iVar1 + 4);
  *(int *)(iVar3 + 0x48) = iVar2;
  if (iVar2 != 0) {
    iVar2 = FUN_080195b8((uint)*(ushort *)(iVar3 + 0x92) << 1);
    iVar3 = *(int *)(iVar1 + 4);
    *(int *)(iVar3 + 0x68) = iVar2;
    if (iVar2 != 0) {
      iVar2 = FUN_080195b8((uint)*(ushort *)(iVar3 + 0x92) << 2);
      iVar3 = *(int *)(iVar1 + 4);
      *(int *)(iVar3 + 0x70) = iVar2;
      if (iVar2 != 0) {
        (**(code **)(*(int *)(iVar3 + 0x74) + 4))(iVar2,*(undefined2 *)(iVar3 + 0x92));
        *(undefined1 *)(*(int *)(iVar1 + 4) + 0xa1) = 1;
      }
    }
  }
  return;
}


// ======== FUN_08019f30 @ 08019f30 size=76 ========

void FUN_08019f30(undefined4 param_1,undefined4 param_2)

{
  undefined4 uVar1;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 uStack_1c;
  undefined1 local_18;
  undefined1 local_17;
  undefined1 local_16;
  
  uVar1 = DAT_08019f7c;
  FUN_08013d96(DAT_08019f7c,0);
  local_30 = DAT_08019f80;
  local_18 = 0;
  local_17 = 1;
  local_2c = 0x200;
  local_16 = 0;
  uStack_1c = 0x2000;
  local_24 = 0x800;
  local_28 = param_1;
  local_20 = param_2;
  FUN_08013e4c(uVar1,0,&local_30);
  FUN_08013db6(uVar1,0);
  return;
}


// ======== FUN_08019f84 @ 08019f84 size=120 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_08019f84(void)

{
  char *pcVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined8 uVar5;
  
  iVar2 = _DAT_08019ffc;
  if (*(int *)(*(int *)(_DAT_08019ffc + 4) + 0x70) == 0) {
    iVar3 = FUN_080195b8((uint)*(ushort *)(*(int *)(_DAT_08019ffc + 4) + 0x92) << 2);
    iVar4 = *(int *)(iVar2 + 4);
    *(int *)(iVar4 + 0x70) = iVar3;
    if (iVar3 == 0) {
      FUN_080141e8(1,s_init_bd_0801a04c,s____src_application_process_deal__0801a024,DAT_0801a020,
                   0xece,s_malloc_all_sensor_data_failed_08019fff + 1);
    }
    else {
      (**(code **)(*(int *)(iVar4 + 0x74) + 4))(iVar3,*(undefined2 *)(iVar4 + 0x92));
    }
  }
  FUN_0801ba58();
  (**(code **)(*(int *)(*(int *)(iVar2 + 4) + 0x74) + 8))();
  uVar5 = (**(code **)(*(int *)(*(int *)(iVar2 + 4) + 4) + 0x10))();
  *(undefined8 *)(*(int *)(iVar2 + 4) + 0x80) = uVar5;
  iVar3 = FUN_0801c818();
  if (iVar3 == 0) {
    *DAT_0801a054 = 0x10000000;
    DataSynchronizationBarrier(0xf);
    InstructionSynchronizationBarrier(0xf);
  }
  pcVar1 = (char *)(*(int *)(iVar2 + 4) + 0xac);
  *pcVar1 = *pcVar1 + '\x01';
  return;
}


// ======== FUN_0801a060 @ 0801a060 size=22 ========

void FUN_0801a060(void)

{
  undefined4 uVar1;
  
  uVar1 = DAT_0801a078;
  FUN_0801aeea(DAT_0801a078,0);
  FUN_0801af72(uVar1);
  return;
}


// ======== FUN_0801a07c @ 0801a07c size=170 ========

void FUN_0801a07c(void)

{
  uint *puVar1;
  uint *puVar2;
  int iVar3;
  
  puVar1 = DAT_0801a128;
  iVar3 = 0;
  *DAT_0801a128 = *DAT_0801a128 | 0x10000;
  do {
    iVar3 = iVar3 + 1;
    if ((*puVar1 & 0x20000) != 0) break;
  } while (iVar3 != 0xffff);
  if (-1 < (int)(*puVar1 << 0xe)) {
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  puVar1[7] = puVar1[7] | 0x10000000;
  puVar2 = DAT_0801a12c;
  *DAT_0801a12c = *DAT_0801a12c | 0xc000;
  puVar1[1] = puVar1[1];
  puVar1[1] = puVar1[1];
  puVar1[1] = puVar1[1] | 0x400;
  puVar1[1] = puVar1[1] & 0xfffcffff;
  puVar1[1] = puVar1[1] | 0x30000;
  puVar1[1] = puVar1[1] & DAT_0801a130;
  puVar1[1] = puVar1[1] | DAT_0801a134;
  *puVar1 = *puVar1 | 0x1000000;
  do {
  } while (-1 < (int)(*puVar1 << 6));
  *puVar2 = *puVar2 | 0x10000;
  do {
  } while (-1 < (int)(puVar2[1] << 0xf));
  *puVar2 = *puVar2 | 0x20000;
  do {
  } while (-1 < (int)(puVar2[1] << 0xe));
  puVar1[1] = puVar1[1] & 0xfffffffc;
  puVar1[1] = puVar1[1] | 2;
  do {
  } while (-1 < (int)(puVar1[1] << 0x1c));
  return;
}


// ======== thunk_FUN_0801a07c @ 0801a138 size=4 ========

void thunk_FUN_0801a07c(void)

{
  uint *puVar1;
  uint *puVar2;
  int iVar3;
  
  puVar1 = DAT_0801a128;
  iVar3 = 0;
  *DAT_0801a128 = *DAT_0801a128 | 0x10000;
  do {
    iVar3 = iVar3 + 1;
    if ((*puVar1 & 0x20000) != 0) break;
  } while (iVar3 != 0xffff);
  if (-1 < (int)(*puVar1 << 0xe)) {
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  puVar1[7] = puVar1[7] | 0x10000000;
  puVar2 = DAT_0801a12c;
  *DAT_0801a12c = *DAT_0801a12c | 0xc000;
  puVar1[1] = puVar1[1];
  puVar1[1] = puVar1[1];
  puVar1[1] = puVar1[1] | 0x400;
  puVar1[1] = puVar1[1] & 0xfffcffff;
  puVar1[1] = puVar1[1] | 0x30000;
  puVar1[1] = puVar1[1] & DAT_0801a130;
  puVar1[1] = puVar1[1] | DAT_0801a134;
  *puVar1 = *puVar1 | 0x1000000;
  do {
  } while (-1 < (int)(*puVar1 << 6));
  *puVar2 = *puVar2 | 0x10000;
  do {
  } while (-1 < (int)(puVar2[1] << 0xf));
  *puVar2 = *puVar2 | 0x20000;
  do {
  } while (-1 < (int)(puVar2[1] << 0xe));
  puVar1[1] = puVar1[1] & 0xfffffffc;
  puVar1[1] = puVar1[1] | 2;
  do {
  } while (-1 < (int)(puVar1[1] << 0x1c));
  return;
}


// ======== FUN_0801a13c @ 0801a13c size=24 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0801a13c(int param_1)

{
  if (param_1 == 4) {
    _DAT_e000e010 = _DAT_e000e010 | 4;
  }
  else {
    _DAT_e000e010 = _DAT_e000e010 & 0xfffffffb;
  }
  return;
}


// ======== FUN_0801a154 @ 0801a154 size=20 ========

void FUN_0801a154(void)

{
  FUN_08012874(DAT_0801a168);
  FUN_0801912c(0x609);
  return;
}


// ======== FUN_0801a16c @ 0801a16c size=132 ========

void FUN_0801a16c(void)

{
  undefined4 uVar1;
  
  FUN_08019148(0x602);
  FUN_08016ef0(DAT_0801a1f0,0,4,0x80);
  FUN_08019148(0x609);
  FUN_08018e7c(1);
  uVar1 = DAT_0801a1f4;
  FUN_08012aa2(DAT_0801a1f4,0x100);
  FUN_08012aa2(uVar1,2,0);
  FUN_08012900(uVar1,2,0x7000);
  FUN_080129ec(0);
  FUN_0801285e(uVar1,0);
  FUN_08012828(uVar1,2,1);
  FUN_0801292e(uVar1,0,7);
  FUN_080128ca(uVar1,2,1);
  FUN_080128ba(uVar1);
  FUN_0801280a(uVar1);
  return;
}


// ======== FUN_0801a1f8 @ 0801a1f8 size=84 ========

bool FUN_0801a1f8(void)

{
  int iVar1;
  int iVar2;
  byte bVar3;
  uint uVar4;
  int *piVar5;
  
  iVar1 = DAT_0801a24c;
  uVar4 = 0;
  bVar3 = 0;
  piVar5 = (int *)(DAT_0801a24c + 0x3c);
  do {
    FUN_08012a88(iVar1,2);
    do {
      iVar2 = FUN_08012920(iVar1,2);
      if (iVar2 != 0) break;
      iVar2 = FUN_08012920(iVar1,4);
    } while (iVar2 == 0);
    bVar3 = bVar3 + 1;
    uVar4 = uVar4 + *piVar5;
    if (7 < bVar3) {
      *DAT_0801a250 = (short)(uVar4 * 0x2000 >> 0x10);
      return ((uVar4 & 0x7ffff) >> 3) - 0x772 < 0x11d;
    }
  } while( true );
}


// ======== thunk_FUN_0801a1f8 @ 0801a254 size=4 ========

bool thunk_FUN_0801a1f8(void)

{
  int iVar1;
  int iVar2;
  byte bVar3;
  uint uVar4;
  int *piVar5;
  
  iVar1 = DAT_0801a24c;
  uVar4 = 0;
  bVar3 = 0;
  piVar5 = (int *)(DAT_0801a24c + 0x3c);
  do {
    FUN_08012a88(iVar1,2);
    do {
      iVar2 = FUN_08012920(iVar1,2);
      if (iVar2 != 0) break;
      iVar2 = FUN_08012920(iVar1,4);
    } while (iVar2 == 0);
    bVar3 = bVar3 + 1;
    uVar4 = uVar4 + *piVar5;
    if (7 < bVar3) {
      *DAT_0801a250 = (short)(uVar4 * 0x2000 >> 0x10);
      return ((uVar4 & 0x7ffff) >> 3) - 0x772 < 0x11d;
    }
  } while( true );
}


// ======== FUN_0801a258 @ 0801a258 size=374 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_0801a258(void)

{
  longlong lVar1;
  int *piVar2;
  int iVar3;
  undefined4 uVar4;
  uint uVar5;
  uint uVar6;
  int iVar7;
  int iVar8;
  ulonglong uVar9;
  undefined8 uVar10;
  
  iVar8 = 1000;
  iVar7 = 1000;
  FUN_08019148(0x71c);
  FUN_08019148(0x71b);
  FUN_0801819c();
  FUN_08018ef0();
  FUN_08018ee0();
  FUN_08019030(0x800);
  FUN_0801904c(0x900);
  while (iVar3 = FUN_08019000(0x901), iVar3 == 0) {
    if (iVar7 == 0) goto LAB_0801a2b2;
    iVar7 = iVar7 + -1;
  }
  if (iVar7 == 0) {
LAB_0801a2b2:
    uVar4 = 0;
  }
  else {
    iVar7 = 10000;
    FUN_0801904c(0x10);
    while (iVar3 = FUN_08019000(0x11), iVar3 == 0) {
      if (iVar7 == 0) goto LAB_0801a324;
      iVar7 = iVar7 + -1;
    }
    if (iVar7 == 0) {
LAB_0801a324:
      uVar4 = 1;
    }
    else {
      FUN_08019020();
      FUN_0801919c(0x200);
      FUN_08019148(0x80f);
      FUN_08019534();
      FUN_08019500();
      FUN_08019510(0);
      FUN_0801a994(0);
      FUN_0801a13c(4);
      _DAT_e000e014 = 48000;
      _DAT_e000e018 = 0;
      _DAT_e000e010 = _DAT_e000e010 | 1;
      do {
      } while (-1 < (int)(_DAT_e000e010 << 0xf));
      iVar7 = FUN_0801a984();
      piVar2 = DAT_0801a3d0;
      DAT_0801a3d0[2] = iVar7;
      FUN_080191b0(1);
      while (iVar7 = FUN_080191c4(), iVar7 != 4) {
        if (iVar8 == 0) goto LAB_0801a366;
        iVar8 = iVar8 + -1;
      }
      if (iVar8 == 0) {
LAB_0801a366:
        uVar4 = 2;
      }
      else {
        FUN_0801a994(0);
        _DAT_e000e014 = 48000;
        _DAT_e000e018 = 0;
        do {
        } while (-1 < (int)(_DAT_e000e010 << 0xf));
        iVar7 = FUN_0801a984();
        piVar2[1] = iVar7;
        iVar8 = piVar2[2] * DAT_0801a3d4;
        *piVar2 = iVar8;
        if (iVar7 == 0) {
          iVar8 = -1;
        }
        uVar9 = FUN_080108a4(iVar8,0,iVar7,0);
        lVar1 = (uVar9 & 0xffffffff) * (ulonglong)DAT_0801a3d4;
        uVar10 = FUN_080108a4((int)lVar1,
                              (int)(uVar9 >> 0x20) * DAT_0801a3d4 + (int)((ulonglong)lVar1 >> 0x20),
                              DAT_0801a3d4,0,uVar9);
        uVar6 = (uint)((ulonglong)uVar10 >> 0x20);
        uVar5 = (uint)uVar10;
        iVar7 = uVar5 - DAT_0801a3d8;
        if ((uVar6 == 0 && (DAT_0801a3d8 <= uVar5) <= uVar6) ||
           (iVar7 = DAT_0801a3dc - uVar5, (uint)(uVar5 <= DAT_0801a3dc) <= -uVar6)) {
          FUN_080191b0(0,iVar7);
          uVar4 = 3;
        }
        else {
          FUN_0801912c(0x80f,iVar7);
          uVar4 = 4;
        }
      }
    }
  }
  return uVar4;
}


// ======== FUN_0801a3e0 @ 0801a3e0 size=182 ========

undefined8 FUN_0801a3e0(undefined4 param_1,undefined4 param_2,undefined4 param_3,uint param_4)

{
  undefined4 uVar1;
  undefined2 local_20;
  undefined2 local_1e;
  undefined2 local_1c;
  undefined2 local_1a;
  undefined4 local_18;
  uint local_14;
  
  local_1c = (undefined2)param_2;
  local_1a = (undefined2)((uint)param_2 >> 0x10);
  local_20 = (undefined2)param_1;
  local_1e = (undefined2)((uint)param_1 >> 0x10);
  local_18 = param_3;
  local_14 = param_4;
  FUN_08019148(0x71b);
  FUN_08019148(0x71c);
  FUN_0801819c();
  FUN_08012ce8();
  FUN_0801904c(0x900);
  FUN_08019068(0x900);
  FUN_0801919c(0x200);
  FUN_08019148(0x80f);
  FUN_08019534();
  FUN_08019500();
  FUN_080194f0(1);
  FUN_08019500();
  FUN_08019510(0x7fff);
  FUN_08019500();
  FUN_08017bc4(3,1,0);
  *DAT_0801a498 = 0;
  FUN_08019148(0x705);
  uVar1 = DAT_0801a49c;
  FUN_0801aef0(DAT_0801a49c);
  local_20 = 0x77;
  local_1c = 0;
  local_18 = 20000;
  local_1e = 0;
  local_1a = 0;
  local_14 = local_14 & 0xffffff00;
  FUN_0801af7c(uVar1,&local_20);
  FUN_0801b018(uVar1,1);
  FUN_08017bc4(0x37,2,0);
  FUN_0801af72(uVar1);
  return CONCAT26(local_1a,CONCAT24(local_1c,CONCAT22(local_1e,local_20)));
}


// ======== FUN_0801a4a0 @ 0801a4a0 size=64 ========

undefined4 FUN_0801a4a0(void)

{
  uint *puVar1;
  uint uVar2;
  int iVar3;
  
  puVar1 = DAT_0801a4e0;
  uVar2 = *DAT_0801a4e0;
  if (uVar2 <= DAT_0801a4e4) {
    FUN_080138a4(uVar2,0x30);
    *puVar1 = *puVar1 + 0xc0;
    return 0;
  }
  FUN_080138a4(uVar2,0x2f);
  iVar3 = FUN_080138c0();
  if (iVar3 == *(int *)(DAT_0801a4e4 + 0xc0)) {
    FUN_0801a4e8();
    return 2;
  }
  FUN_0801a4e8();
  return 1;
}


// ======== FUN_0801a4e8 @ 0801a4e8 size=10 ========

void FUN_0801a4e8(void)

{
  *DAT_0801a4f8 = DAT_0801a4f4;
  FUN_0801a4fc();
  return;
}


// ======== FUN_0801a4fc @ 0801a4fc size=18 ========

void FUN_0801a4fc(void)

{
  FUN_08019148(0x506);
  *(uint *)(DAT_080138d8 + 8) = *(uint *)(DAT_080138d8 + 8) | 1;
  return;
}


// ======== FUN_0801a510 @ 0801a510 size=28 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0801a510(void)

{
  _DAT_e000e010 = _DAT_e000e010 & 0xfffffffd;
  FUN_08011a1a(0,s________________IEC60730_Test_Fai_0801a52c);
  do {
    *DAT_0801a568 = DAT_0801a564;
  } while( true );
}


// ======== FUN_0801a56c @ 0801a56c size=72 ========

undefined4 FUN_0801a56c(uint param_1)

{
  uint uVar1;
  undefined4 local_18;
  undefined4 uStack_14;
  
  local_18 = param_1;
  if (param_1 == 0) {
    local_18 = 0xffffffff;
  }
  uStack_14 = 0;
  if ((local_18 < 0x4c) && (uStack_14 = local_18 - 0x19, 0x18 < local_18)) {
    *(undefined4 *)(DAT_0801a5b4 + 4) = 0;
  }
  else {
    uVar1 = *(uint *)(DAT_0801a5b4 + 4);
    *(uint *)(DAT_0801a5b4 + 4) = uVar1 + 1;
    if (4 < uVar1) {
      FUN_080191b0(0,uVar1,uVar1 + 1,uStack_14);
      return 3;
    }
  }
  return 4;
}


// ======== FUN_0801a5b8 @ 0801a5b8 size=24 ========

int FUN_0801a5b8(void)

{
  uint uVar1;
  uint uVar2;
  
  uVar2 = *DAT_0801a5d0;
  uVar1 = *DAT_0801a5d4;
  if (uVar2 < uVar1) {
    uVar1 = ~uVar1;
  }
  else {
    uVar1 = -uVar1;
  }
  *DAT_0801a5d4 = uVar2;
  return uVar2 + uVar1;
}


// ======== FUN_0801a5f0 @ 0801a5f0 size=318 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0801a5f0(void)

{
  undefined1 *puVar1;
  int *piVar2;
  undefined1 uVar3;
  int iVar4;
  undefined4 uVar5;
  char *pcVar6;
  int iVar7;
  undefined1 uVar8;
  
  puVar1 = _DAT_0801a724;
  *_DAT_0801a724 = 0;
  iVar4 = FUN_08010470();
  uVar8 = 1;
  if (iVar4 != 1) {
    pcVar6 = s_CPU_Test_PreRun__Fail__0801a727 + 1;
    goto LAB_0801a6aa;
  }
  FUN_08011a1a(0,s_CPU_Test_PreRun__Success__0801a744);
  iVar4 = FUN_0801ab88();
  if (iVar4 == 1) {
    FUN_0801a4fc();
    iVar4 = FUN_080138a4(DAT_0801a760,0xbfff);
    piVar2 = DAT_0801a764;
    *(int *)(puVar1 + 4) = iVar4;
    iVar7 = *piVar2;
    if (iVar7 == iVar4) {
      FUN_08011a1a(0,s_FLASH_CRC32_Test_PreRun__Success_0801a7a0,iVar7,iVar4);
      iVar4 = 1000;
      FUN_080191b0(0);
      uVar8 = 2;
      while (iVar7 = FUN_080191c4(), iVar7 != 0) {
        if (iVar4 == 0) goto LAB_0801a680;
        iVar4 = iVar4 + -1;
      }
      if (iVar4 == 0) {
LAB_0801a680:
        pcVar6 = s_Switch_back_to_IRC8M_by_closing_P_0801a7c4;
        goto LAB_0801a6aa;
      }
      FUN_08019030(0x18);
      uVar5 = FUN_0801a258();
      switch(uVar5) {
      case 0:
        pcVar6 = s_IRC40K_stable_Fail__0801a838;
        goto LAB_0801a6aa;
      case 1:
        pcVar6 = s_HXTAL_stable_Fail__0801a850;
        goto LAB_0801a6aa;
      case 2:
        pcVar6 = s_IRC8M_switch_to_HXTAL_Fail__0801a868;
        goto LAB_0801a6aa;
      case 3:
        *puVar1 = 2;
        FUN_08011a1a(0,s_3_4_HXTAL_<_HXTAL_(PreRun)_<_5_4_0801a888);
        goto LAB_0801a6c6;
      case 4:
        thunk_FUN_0801a07c();
        FUN_08011a1a(0,s_Clock_Frequency_Test_Success__0801a7f0);
        FUN_0801a978();
        iVar4 = FUN_0801a968();
        if (iVar4 == 1) {
          FUN_08011a1a(0,s_Program_counter_test_PreRun__Suc_0801a8e8);
          FUN_0801a16c();
          iVar4 = FUN_0801a1f8();
          if (iVar4 == 1) {
            FUN_08011a1a(0,s_adc_test_PreRun__Success__0801a92c);
            FUN_0801a154();
            disableIRQinterrupts();
            iVar4 = FUN_08010268(0x20000000,_DAT_0801a948,0x55555555);
            if (iVar4 == 1) {
              FUN_08011a1a(0,s_ram_test_PreRun__Success__0801a94b + 1);
              enableIRQinterrupts();
              (*DAT_08010218)();
                    /* WARNING: Could not recover jumptable at 0x08010202. Too many branches */
                    /* WARNING: Treating indirect jump as call */
              (*DAT_0801021c)();
              return;
            }
            uVar3 = 4;
          }
          else {
            FUN_08011a1a(0,s_adc_test_PreRun__Failed__0801a910);
            uVar3 = 7;
          }
        }
        else {
          FUN_08011a1a(0,s_Program_counter_test_PreRun__Fai_0801a810);
          uVar3 = 6;
        }
        break;
      default:
        goto switchD_0801a676_default;
      }
    }
    else {
      FUN_08011a1a(0,s_FLASH_CRC32_Test_PreRun__Error__C_0801a768,iVar7,iVar4);
      uVar3 = 3;
    }
  }
  else {
    uVar3 = 8;
  }
  *puVar1 = uVar3;
LAB_0801a6c6:
  do {
    FUN_0801a510();
switchD_0801a676_default:
    pcVar6 = s_Clock_Test_PreRun__Unexpectedly_A_0801a8bc;
LAB_0801a6aa:
    FUN_08011a1a(0,pcVar6);
    *puVar1 = uVar8;
  } while( true );
}


// ======== FUN_0801a968 @ 0801a968 size=10 ========

undefined4 FUN_0801a968(void)

{
  *DAT_0801a974 = 3;
  return 1;
}


// ======== FUN_0801a978 @ 0801a978 size=8 ========

void FUN_0801a978(void)

{
  *DAT_0801a980 = 0;
  return;
}


// ======== FUN_0801a984 @ 0801a984 size=12 ========

void FUN_0801a984(void)

{
  undefined4 uVar1;
  
  uVar1 = FUN_08019498();
  *DAT_0801a990 = uVar1;
  return;
}


// ======== FUN_0801a994 @ 0801a994 size=36 ========

void FUN_0801a994(undefined4 param_1)

{
  uint *puVar1;
  undefined4 *puVar2;
  uint extraout_r2;
  
  puVar2 = DAT_0801a99c;
  *DAT_0801a99c = param_1;
  FUN_08019478(param_1,puVar2,param_1);
  puVar1 = DAT_080194c8;
  *DAT_080194c8 = extraout_r2 >> 0x10;
  puVar1[1] = extraout_r2 & 0xffff;
  *DAT_08019494 = *DAT_08019494 & 0xffffffef;
  return;
}


// ======== FUN_0801a9a0 @ 0801a9a0 size=200 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0801a9a0(void)

{
  undefined1 *puVar1;
  uint *puVar2;
  uint *puVar3;
  undefined1 uVar4;
  int iVar5;
  uint uVar6;
  char *pcVar7;
  
  iVar5 = FUN_0801a4a0();
  puVar1 = _DAT_0801aa68;
  if (iVar5 == 1) {
    uVar4 = 3;
    goto LAB_0801aa62;
  }
  if ((iVar5 != 0) && (iVar5 == 2)) {
    FUN_08011a1a(0,s_FLASH_CRC_Run_Time__Test_Success_0801aa6b + 1);
  }
  puVar2 = DAT_0801aa90;
  disableIRQinterrupts();
  *DAT_0801aa90 = *DAT_0801aa90 + 7;
  iVar5 = FUN_08011a4c();
  puVar3 = _DAT_0801aa94;
  *_DAT_0801aa94 = *_DAT_0801aa94 - 7;
  if (iVar5 != 0) {
    if (iVar5 == 2) {
      pcVar7 = s_ram_test_ok_0801aaac;
    }
    else {
      *puVar1 = 4;
      pcVar7 = s_ram_test_failure_0801aa97 + 1;
    }
    FUN_08011a1a(0,pcVar7);
  }
  uVar6 = *puVar2;
  if ((uVar6 ^ *puVar3) == 0xffffffff) {
    if (iVar5 == 2) {
      if (uVar6 != 0xd7dc) goto LAB_0801aa16;
      *puVar2 = 0;
      *puVar3 = 0xffffffff;
    }
  }
  else {
LAB_0801aa16:
    *puVar1 = 4;
  }
  enableIRQinterrupts();
  if (*DAT_0801aabc != '\x01') {
    return;
  }
  *DAT_0801aabc = '\0';
  iVar5 = FUN_080106c4();
  if (iVar5 == 1) {
    iVar5 = FUN_0801a56c(*(undefined4 *)(DAT_0801aac0 + 0xc));
    if (iVar5 == 3) {
      uVar4 = 2;
    }
    else {
      iVar5 = FUN_0801ab50();
      if (iVar5 == 1) {
        iVar5 = thunk_FUN_0801a1f8();
        if (iVar5 == 1) {
          return;
        }
        uVar4 = 7;
      }
      else {
        uVar4 = 5;
      }
    }
  }
  else {
    uVar4 = 1;
  }
LAB_0801aa62:
  *puVar1 = uVar4;
  return;
}


// ======== FUN_0801aac4 @ 0801aac4 size=82 ========

void FUN_0801aac4(void)

{
  undefined4 *puVar1;
  
  *DAT_0801ab0c = 0;
  *DAT_0801ab10 = 0;
  FUN_08011ac0();
  *DAT_0801ab14 = 0;
  *DAT_0801ab18 = 0xffffffff;
  *(undefined4 *)(DAT_0801ab1c + 0xc) = 0;
  FUN_0801a3e0();
  puVar1 = DAT_0801ab20;
  *DAT_0801ab20 = 0xaaaaaaaa;
  puVar1[1] = 0x55555555;
  puVar1[2] = 0;
  puVar1[3] = 0xffffffff;
  puVar1[4] = 0x99999999;
  puVar1[5] = 0x66666666;
  FUN_0801a4e8();
  return;
}


// ======== FUN_0801ab50 @ 0801ab50 size=52 ========

undefined4 FUN_0801ab50(void)

{
  if ((((*DAT_0801ab84 == -0x55555556) && (DAT_0801ab84[1] == 0x55555555)) && (DAT_0801ab84[2] == 0)
      ) && (((DAT_0801ab84[3] == -1 && (DAT_0801ab84[4] == -0x66666667)) &&
            (DAT_0801ab84[5] == 0x66666666)))) {
    return 1;
  }
  return 0;
}


// ======== FUN_0801ab88 @ 0801ab88 size=206 ========

undefined4 FUN_0801ab88(void)

{
  int iVar1;
  int iVar2;
  byte bVar3;
  
  FUN_08019148(0x71b);
  FUN_08019148(0x71c);
  FUN_0801819c();
  iVar1 = FUN_08012cf8(6);
  iVar2 = FUN_08019000(0x91b);
  bVar3 = iVar2 == 1;
  iVar2 = FUN_08019000(0x91a);
  if (iVar2 == 1) {
    bVar3 = bVar3 | 2;
  }
  iVar2 = FUN_08019000(0x91e);
  if (iVar2 == 1) {
    bVar3 = bVar3 | 4;
  }
  iVar2 = FUN_08019000(0x91d);
  if (iVar2 == 1) {
    bVar3 = bVar3 | 8;
  }
  iVar2 = FUN_08019000(0x91f);
  if (iVar2 == 1) {
    bVar3 = bVar3 | 0x10;
  }
  iVar2 = FUN_08019000(0x91c);
  if (iVar2 == 1) {
    bVar3 = bVar3 | 0x20;
  }
  if (bVar3 == 1) {
    if (iVar1 == 0) {
      FUN_08016a20(1,0);
      FUN_08016a70();
      FUN_08016a80();
      FUN_08018ed0();
      FUN_08012d24(6,1);
      do {
                    /* WARNING: Do nothing block with infinite loop */
      } while( true );
    }
  }
  else if (bVar3 == 8) {
    if (iVar1 != 1) {
      return 1;
    }
    FUN_08012d24(6,0);
    FUN_08018ed0();
    return 1;
  }
  if (iVar1 != 1) {
    return 1;
  }
  FUN_08012d24(6,0);
  return 0;
}


// ======== FUN_0801ac58 @ 0801ac58 size=458 ========

void FUN_0801ac58(int param_1,int param_2,ushort *param_3)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  
  iVar2 = DAT_0801ae28;
  iVar1 = DAT_0801ae24;
  if (param_2 == 0) {
    *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) & 0xfffffffe;
    *(uint *)(param_1 + 0x18) = *(uint *)(param_1 + 0x18) & 0xfffffffc;
    *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) | (uint)*param_3;
    *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) & 0xfffffffd;
    *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) | (uint)param_3[2];
    if ((param_1 != iVar1) && (param_1 != iVar2)) {
      return;
    }
    *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) & 0xfffffffb;
    *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) | (uint)param_3[1];
    *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) & 0xfffffff7;
    *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) | (uint)param_3[3];
    *(uint *)(param_1 + 4) = *(uint *)(param_1 + 4) & 0xfffffeff;
    *(uint *)(param_1 + 4) = *(uint *)(param_1 + 4) | (uint)param_3[4];
    *(uint *)(param_1 + 4) = *(uint *)(param_1 + 4) & 0xfffffdff;
    uVar3 = *(uint *)(param_1 + 4) | (uint)param_3[5];
  }
  else if (param_2 == 1) {
    *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) & 0xffffffef;
    *(uint *)(param_1 + 0x18) = *(uint *)(param_1 + 0x18) & 0xfffffcff;
    *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) | (uint)*param_3 << 4;
    *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) & 0xffffffdf;
    *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) | (uint)param_3[2] << 4;
    if ((param_1 != iVar1) && (param_1 != iVar2)) {
      return;
    }
    *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) & 0xffffffbf;
    *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) | (uint)param_3[1] << 4;
    *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) & 0xffffff7f;
    *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) | (uint)param_3[3] << 4;
    *(uint *)(param_1 + 4) = *(uint *)(param_1 + 4) & 0xfffffbff;
    *(uint *)(param_1 + 4) = *(uint *)(param_1 + 4) | (uint)param_3[4] << 2;
    *(uint *)(param_1 + 4) = *(uint *)(param_1 + 4) & 0xfffff7ff;
    uVar3 = *(uint *)(param_1 + 4) | (uint)param_3[5] << 2;
  }
  else if (param_2 == 2) {
    *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) & 0xfffffeff;
    *(uint *)(param_1 + 0x1c) = *(uint *)(param_1 + 0x1c) & 0xfffffffc;
    *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) | (uint)*param_3 << 8;
    *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) & 0xfffffdff;
    *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) | (uint)param_3[2] << 8;
    if ((param_1 != iVar1) && (param_1 != iVar2)) {
      return;
    }
    *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) & 0xfffffbff;
    *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) | (uint)param_3[1] << 8;
    *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) & 0xfffff7ff;
    *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) | (uint)param_3[3] << 8;
    *(uint *)(param_1 + 4) = *(uint *)(param_1 + 4) & 0xffffefff;
    *(uint *)(param_1 + 4) = *(uint *)(param_1 + 4) | (uint)param_3[4] << 4;
    *(uint *)(param_1 + 4) = *(uint *)(param_1 + 4) & 0xffffdfff;
    uVar3 = *(uint *)(param_1 + 4) | (uint)param_3[5] << 4;
  }
  else {
    if (param_2 != 3) {
      return;
    }
    *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) & 0xffffefff;
    *(uint *)(param_1 + 0x1c) = *(uint *)(param_1 + 0x1c) & 0xfffffcff;
    *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) | (uint)*param_3 << 0xc;
    *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) & 0xffffdfff;
    *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) | (uint)param_3[2] << 0xc;
    if ((param_1 != iVar1) && (param_1 != iVar2)) {
      return;
    }
    *(uint *)(param_1 + 4) = *(uint *)(param_1 + 4) & 0xffffbfff;
    uVar3 = *(uint *)(param_1 + 4) | (uint)param_3[4] << 6;
  }
  *(uint *)(param_1 + 4) = uVar3;
  return;
}


// ======== FUN_0801ae2c @ 0801ae2c size=78 ========

void FUN_0801ae2c(int param_1,int param_2,uint param_3)

{
  if (param_2 == 0) {
    *(uint *)(param_1 + 0x18) = *(uint *)(param_1 + 0x18) & 0xffffff8f;
    param_3 = *(uint *)(param_1 + 0x18) | param_3;
  }
  else {
    if (param_2 != 1) {
      if (param_2 == 2) {
        *(uint *)(param_1 + 0x1c) = *(uint *)(param_1 + 0x1c) & 0xffffff8f;
        param_3 = *(uint *)(param_1 + 0x1c) | param_3;
      }
      else {
        if (param_2 != 3) {
          return;
        }
        *(uint *)(param_1 + 0x1c) = *(uint *)(param_1 + 0x1c) & 0xffff8fff;
        param_3 = *(uint *)(param_1 + 0x1c) | param_3 << 8;
      }
      *(uint *)(param_1 + 0x1c) = param_3;
      return;
    }
    *(uint *)(param_1 + 0x18) = *(uint *)(param_1 + 0x18) & 0xffff8fff;
    param_3 = *(uint *)(param_1 + 0x18) | param_3 << 8;
  }
  *(uint *)(param_1 + 0x18) = param_3;
  return;
}


// ======== FUN_0801ae7a @ 0801ae7a size=30 ========

void FUN_0801ae7a(int param_1,int param_2,undefined4 param_3)

{
  if (param_2 == 0) {
    *(undefined4 *)(param_1 + 0x34) = param_3;
    return;
  }
  if (param_2 != 1) {
    if (param_2 != 2) {
      if (param_2 == 3) {
        *(undefined4 *)(param_1 + 0x40) = param_3;
      }
      return;
    }
    *(undefined4 *)(param_1 + 0x3c) = param_3;
    return;
  }
  *(undefined4 *)(param_1 + 0x38) = param_3;
  return;
}


// ======== FUN_0801ae98 @ 0801ae98 size=78 ========

void FUN_0801ae98(int param_1,int param_2,uint param_3)

{
  if (param_2 == 0) {
    *(uint *)(param_1 + 0x18) = *(uint *)(param_1 + 0x18) & 0xfffffff7;
    param_3 = *(uint *)(param_1 + 0x18) | param_3;
  }
  else {
    if (param_2 != 1) {
      if (param_2 == 2) {
        *(uint *)(param_1 + 0x1c) = *(uint *)(param_1 + 0x1c) & 0xfffffff7;
        param_3 = *(uint *)(param_1 + 0x1c) | param_3;
      }
      else {
        if (param_2 != 3) {
          return;
        }
        *(uint *)(param_1 + 0x1c) = *(uint *)(param_1 + 0x1c) & 0xfffff7ff;
        param_3 = *(uint *)(param_1 + 0x1c) | param_3 << 8;
      }
      *(uint *)(param_1 + 0x1c) = param_3;
      return;
    }
    *(uint *)(param_1 + 0x18) = *(uint *)(param_1 + 0x18) & 0xfffff7ff;
    param_3 = *(uint *)(param_1 + 0x18) | param_3 << 8;
  }
  *(uint *)(param_1 + 0x18) = param_3;
  return;
}


// ======== FUN_0801aee6 @ 0801aee6 size=4 ========

undefined4 FUN_0801aee6(int param_1)

{
  return *(undefined4 *)(param_1 + 0x24);
}


// ======== FUN_0801aeea @ 0801aeea size=4 ========

void FUN_0801aeea(int param_1,undefined4 param_2)

{
  *(undefined4 *)(param_1 + 0x24) = param_2;
  return;
}


// ======== FUN_0801aef0 @ 0801aef0 size=110 ========

void FUN_0801aef0(int param_1)

{
  uint *puVar1;
  uint uVar2;
  uint uVar3;
  
  uVar2 = param_1 - DAT_0801af60;
  uVar3 = DAT_0801af60 >> 0x14;
  if (param_1 == DAT_0801af60) {
    uVar3 = 0x404;
  }
  else if (param_1 < DAT_0801af60) {
    if (param_1 != 0x40000000) {
      uVar2 = param_1 + DAT_0801af64;
      if (uVar2 == 0) {
        uVar3 = 0x401;
      }
      else if (uVar2 == uVar3) {
        uVar3 = 0x402;
      }
      else {
        if (uVar2 != 0x800) {
          return;
        }
        uVar3 = 0x403;
      }
    }
  }
  else if (uVar2 == uVar3) {
    uVar3 = 0x405;
  }
  else if (uVar2 == 0x11c00) {
    uVar3 = 0x30b;
  }
  else {
    if (uVar2 != 0x12400) {
      return;
    }
    uVar3 = 0x30d;
  }
  FUN_08019180(uVar3);
  puVar1 = (uint *)(DAT_0801917c + (uVar3 >> 6));
  *puVar1 = *puVar1 & ~(1 << (uVar3 & 0x1f));
  return;
}


// ======== FUN_0801af68 @ 0801af68 size=10 ========

void FUN_0801af68(uint *param_1)

{
  *param_1 = *param_1 & 0xfffffffe;
  return;
}


// ======== FUN_0801af72 @ 0801af72 size=10 ========

void FUN_0801af72(uint *param_1)

{
  *param_1 = *param_1 | 1;
  return;
}


// ======== FUN_0801af7c @ 0801af7c size=120 ========

void FUN_0801af7c(uint *param_1,ushort *param_2)

{
  uint *puVar1;
  uint *puVar2;
  
  param_1[10] = (uint)*param_2;
  puVar2 = DAT_0801aff8;
  puVar1 = DAT_0801aff4;
  if ((((param_1 == DAT_0801aff4) || (param_1 == (uint *)0x40000000)) || (param_1 == DAT_0801affc))
     || (((param_1 == DAT_0801b000 || (param_1 == DAT_0801b004)) || (param_1 == DAT_0801aff8)))) {
    *param_1 = *param_1 & 0xffffff8f;
    *param_1 = *param_1 | (uint)param_2[1];
    *param_1 = *param_1 | (uint)param_2[2];
  }
  param_1[0xb] = *(uint *)(param_2 + 4);
  if (param_1 == DAT_0801b008) {
LAB_0801afe2:
    if (param_1 != puVar2) goto LAB_0801afea;
  }
  else {
    if (param_1 == DAT_0801b00c) goto LAB_0801afea;
    *param_1 = *param_1 & 0xfffffcff;
    *param_1 = *param_1 | (uint)param_2[3];
    if (param_1 != puVar1) goto LAB_0801afe2;
  }
  param_1[0xc] = (uint)(byte)param_2[6];
LAB_0801afea:
  param_1[5] = param_1[5] | 1;
  return;
}


// ======== FUN_0801b010 @ 0801b010 size=8 ========

void FUN_0801b010(int param_1,uint param_2)

{
  *(uint *)(param_1 + 0xc) = *(uint *)(param_1 + 0xc) & ~param_2;
  return;
}


// ======== FUN_0801b018 @ 0801b018 size=8 ========

void FUN_0801b018(int param_1,uint param_2)

{
  *(uint *)(param_1 + 0xc) = *(uint *)(param_1 + 0xc) | param_2;
  return;
}


// ======== FUN_0801b020 @ 0801b020 size=8 ========

void FUN_0801b020(int param_1,uint param_2)

{
  *(uint *)(param_1 + 0x10) = *(uint *)(param_1 + 0x10) & ~param_2;
  return;
}


// ======== FUN_0801b028 @ 0801b028 size=28 ========

void FUN_0801b028(int *param_1)

{
  int iVar1;
  
  if ((param_1 != (int *)0x0) && (*param_1 != 0)) {
    iVar1 = FUN_08010982(*param_1 + param_1[2]);
    param_1[2] = iVar1 + param_1[2];
  }
  return;
}


// ======== FUN_0801b044 @ 0801b044 size=78 ========

void FUN_0801b044(int param_1,uint param_2)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = 0;
  if (param_1 == DAT_0801b094) {
LAB_0801b08e:
    uVar2 = 2;
  }
  else {
    if (param_1 < DAT_0801b094) {
      if ((param_1 + DAT_0801b098 != 0) && (param_1 + DAT_0801b098 != DAT_0801b094 >> 0x14))
      goto LAB_0801b074;
      goto LAB_0801b08e;
    }
    if (param_1 - DAT_0801b094 == DAT_0801b094 >> 0x14) goto LAB_0801b08e;
    if (param_1 - DAT_0801b094 != 0xec00) goto LAB_0801b074;
    uVar2 = 3;
  }
  iVar1 = FUN_08018f00(uVar2);
LAB_0801b074:
  *(uint *)(param_1 + 8) = (iVar1 + (param_2 >> 1)) / param_2 & 0xffff;
  return;
}


// ======== FUN_0801b09c @ 0801b09c size=134 ========

void FUN_0801b09c(void)

{
  int iVar1;
  
  FUN_08019148(0x603);
  FUN_08019148(0x603);
  FUN_08019148(0x712);
  FUN_08019148(0x600);
  iVar1 = DAT_0801b118;
  FUN_08016ef0(DAT_0801b118,0x18,4,0x400);
  FUN_08016ef0(iVar1,4,4,iVar1 >> 0x13);
  iVar1 = DAT_0801b11c;
  FUN_0801b128(DAT_0801b11c);
  FUN_0801b044(iVar1,0x1c200);
  FUN_0801b2b2(iVar1,0);
  FUN_0801b2ea(iVar1,0);
  FUN_0801b2ce(iVar1,0);
  FUN_0801b2c2(iVar1,4);
  *(uint *)(iVar1 + 0xc) = *(uint *)(iVar1 + 0xc) & 0xfffffff7 | 8;
  return;
}


// ======== FUN_0801b120 @ 0801b120 size=8 ========

uint FUN_0801b120(int param_1)

{
  return *(uint *)(param_1 + 4) & 0x1ff;
}


// ======== FUN_0801b128 @ 0801b128 size=80 ========

void FUN_0801b128(int param_1)

{
  uint *puVar1;
  uint uVar2;
  
  if (param_1 == DAT_0801b178) {
    uVar2 = 0x413;
  }
  else if (param_1 < DAT_0801b178) {
    if (param_1 + DAT_0801b17c == 0) {
      uVar2 = 0x411;
    }
    else {
      if (param_1 + DAT_0801b17c != DAT_0801b178 >> 0x14) {
        return;
      }
      uVar2 = 0x412;
    }
  }
  else if (param_1 - DAT_0801b178 == DAT_0801b178 >> 0x14) {
    uVar2 = 0x414;
  }
  else {
    if (param_1 - DAT_0801b178 != 0xec00) {
      return;
    }
    uVar2 = 0x30e;
  }
  FUN_08019180(uVar2);
  puVar1 = (uint *)(DAT_0801917c + (uVar2 >> 6));
  *puVar1 = *puVar1 & ~(1 << (uVar2 & 0x1f));
  return;
}


// ======== FUN_0801b180 @ 0801b180 size=144 ========

void FUN_0801b180(void)

{
  undefined4 uVar1;
  int local_28 [3];
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 local_14;
  undefined1 local_10;
  undefined1 local_f;
  undefined1 local_e;
  
  uVar1 = DAT_0801b210;
  FUN_08013e16(DAT_0801b210,1);
  FUN_08019148(0x500);
  local_28[0] = DAT_0801b214;
  local_28[2] = DAT_0801b218;
  local_e = 1;
  local_18 = 0x80;
  local_10 = 0;
  local_f = 1;
  local_14 = 0x3000;
  local_1c = 0;
  local_28[1] = 0;
  FUN_08013e4c(uVar1,1,local_28);
  FUN_08013f3a(uVar1,1);
  FUN_08013dd6(uVar1,1);
  FUN_08017bc4(0xc,5,0);
  FUN_08013ee4(uVar1,1,8);
  FUN_08013ec4(uVar1,1,8);
  FUN_08013d96(uVar1,1);
  FUN_0801b21c(DAT_0801b214 + -4,0x80);
  *(undefined4 *)(DAT_0801b218 + 0x100) = 0;
  return;
}


// ======== FUN_0801b21c @ 0801b21c size=12 ========

void FUN_0801b21c(int param_1,uint param_2)

{
  *(uint *)(param_1 + 0x14) = *(uint *)(param_1 + 0x14) & 0xffffff7f | param_2;
  return;
}


// ======== FUN_0801b228 @ 0801b228 size=10 ========

void FUN_0801b228(int param_1)

{
  *(uint *)(param_1 + 0xc) = *(uint *)(param_1 + 0xc) | 0x2000;
  return;
}


// ======== FUN_0801b232 @ 0801b232 size=20 ========

void FUN_0801b232(int param_1,uint param_2)

{
  uint *puVar1;
  
  puVar1 = (uint *)(param_1 + (param_2 >> 6));
  *puVar1 = *puVar1 & ~(1 << (param_2 & 0x1f));
  return;
}


// ======== FUN_0801b246 @ 0801b246 size=22 ========

bool FUN_0801b246(int param_1,uint param_2)

{
  return (*(uint *)(param_1 + (param_2 >> 6)) & 1 << (param_2 & 0x1f)) != 0;
}


// ======== FUN_0801b25c @ 0801b25c size=20 ========

void FUN_0801b25c(int param_1,uint param_2)

{
  uint *puVar1;
  
  puVar1 = (uint *)(param_1 + (param_2 >> 6));
  *puVar1 = *puVar1 | 1 << (param_2 & 0x1f);
  return;
}


// ======== FUN_0801b270 @ 0801b270 size=20 ========

void FUN_0801b270(int param_1,uint param_2)

{
  uint *puVar1;
  
  puVar1 = (uint *)(param_1 + (param_2 >> 0x16));
  *puVar1 = *puVar1 & ~(1 << ((param_2 & 0x1fffff) >> 0x10));
  return;
}


// ======== FUN_0801b284 @ 0801b284 size=46 ========

undefined4 FUN_0801b284(int param_1,uint param_2)

{
  if (((*(uint *)(param_1 + (param_2 >> 0x16)) & 1 << ((param_2 & 0x1fffff) >> 0x10)) != 0) &&
     ((*(uint *)(param_1 + ((param_2 & 0xffff) >> 6)) & 1 << (param_2 & 0x1f)) != 0)) {
    return 1;
  }
  return 0;
}


// ======== FUN_0801b2b2 @ 0801b2b2 size=16 ========

void FUN_0801b2b2(int param_1,uint param_2)

{
  *(uint *)(param_1 + 0xc) = *(uint *)(param_1 + 0xc) & 0xfffff9ff;
  *(uint *)(param_1 + 0xc) = *(uint *)(param_1 + 0xc) | param_2;
  return;
}


// ======== FUN_0801b2c2 @ 0801b2c2 size=12 ========

void FUN_0801b2c2(int param_1,uint param_2)

{
  *(uint *)(param_1 + 0xc) = *(uint *)(param_1 + 0xc) & 0xfffffffb | param_2;
  return;
}


// ======== FUN_0801b2ce @ 0801b2ce size=16 ========

void FUN_0801b2ce(int param_1,uint param_2)

{
  *(uint *)(param_1 + 0x10) = *(uint *)(param_1 + 0x10) & 0xffffcfff;
  *(uint *)(param_1 + 0x10) = *(uint *)(param_1 + 0x10) | param_2;
  return;
}


// ======== FUN_0801b2ea @ 0801b2ea size=16 ========

void FUN_0801b2ea(int param_1,uint param_2)

{
  *(uint *)(param_1 + 0xc) = *(uint *)(param_1 + 0xc) & 0xffffefff;
  *(uint *)(param_1 + 0xc) = *(uint *)(param_1 + 0xc) | param_2;
  return;
}


// ======== FUN_0801b2fc @ 0801b2fc size=208 ========

undefined4 FUN_0801b2fc(int param_1,int param_2,int *param_3)

{
  byte bVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  byte bVar5;
  undefined4 uVar6;
  
  bVar5 = 0;
  if ((param_2 - param_1 < 6) || (uVar2 = FUN_08017d4c(param_1 + 2), uVar2 - 0xdc00 < 0x400)) {
LAB_0801b3ca:
    uVar6 = 0;
  }
  else {
    if (uVar2 - 0xd800 < 0x400) {
      uVar6 = 0xc;
      if ((((param_2 - (param_1 + 6) < 6) || (*(char *)(param_1 + 6) != '\\')) ||
          (*(char *)(param_1 + 7) != 'u')) ||
         (uVar3 = FUN_08017d4c(param_1 + 8), 0x3ff < uVar3 - 0xdc00)) goto LAB_0801b3ca;
      uVar2 = (uVar3 & 0x3ff | (uVar2 & 0x3ff) << 10) + 0x10000;
    }
    else {
      uVar6 = 6;
    }
    if (uVar2 < 0x80) {
      uVar3 = 1;
      uVar4 = uVar3;
    }
    else if (uVar2 < 0x800) {
      uVar3 = 2;
      bVar5 = 0xc0;
      uVar4 = uVar3;
    }
    else if (uVar2 < 0x10000) {
      uVar3 = 3;
      bVar5 = 0xe0;
      uVar4 = uVar3;
    }
    else {
      if (0x10ffff < uVar2) goto LAB_0801b3ca;
      uVar3 = 4;
      bVar5 = 0xf0;
      uVar4 = uVar3;
    }
    while( true ) {
      uVar3 = uVar3 - 1 & 0xff;
      bVar1 = (byte)uVar2;
      if (uVar3 == 0) break;
      uVar2 = uVar2 >> 6;
      *(byte *)(*param_3 + uVar3) = bVar1 & 0xbf | 0x80;
    }
    if (uVar4 < 2) {
      bVar1 = bVar1 & 0x7f;
    }
    else {
      bVar1 = bVar1 | bVar5;
    }
    *(byte *)*param_3 = bVar1;
    *param_3 = *param_3 + uVar4;
  }
  return uVar6;
}


// ======== FUN_0801b3ce @ 0801b3ce size=36 ========

void FUN_0801b3ce(int param_1)

{
  int *piVar1;
  
  piVar1 = *(int **)(param_1 + 0x10);
  *(undefined4 *)(*(int *)(param_1 + 4) + 8) = *(undefined4 *)(param_1 + 8);
  *(undefined4 *)(*(int *)(param_1 + 8) + 4) = *(undefined4 *)(param_1 + 4);
  if (piVar1[1] == param_1) {
    piVar1[1] = *(int *)(param_1 + 8);
  }
  *(undefined4 *)(param_1 + 0x10) = 0;
  *piVar1 = *piVar1 + -1;
  return;
}


// ======== FUN_0801b3f2 @ 0801b3f2 size=22 ========

void FUN_0801b3f2(undefined4 *param_1)

{
  undefined4 *puVar1;
  
  puVar1 = param_1 + 2;
  param_1[1] = puVar1;
  param_1[2] = 0xffffffff;
  param_1[3] = puVar1;
  param_1[4] = puVar1;
  *param_1 = 0;
  return;
}


// ======== FUN_0801b408 @ 0801b408 size=6 ========

void FUN_0801b408(int param_1)

{
  *(undefined4 *)(param_1 + 0x10) = 0;
  return;
}


// ======== FUN_0801b40e @ 0801b40e size=48 ========

void FUN_0801b40e(int *param_1,uint *param_2)

{
  uint *puVar1;
  uint *puVar2;
  uint uVar3;
  
  if (*param_2 == 0xffffffff) {
    puVar2 = (uint *)param_1[4];
  }
  else {
    puVar1 = (uint *)(param_1 + 2);
    do {
      puVar2 = puVar1;
      puVar1 = (uint *)puVar2[1];
    } while (*(uint *)puVar2[1] <= *param_2);
  }
  uVar3 = puVar2[1];
  param_2[1] = uVar3;
  *(uint **)(uVar3 + 8) = param_2;
  param_2[2] = (uint)puVar2;
  puVar2[1] = (uint)param_2;
  param_2[4] = (uint)param_1;
  *param_1 = *param_1 + 1;
  return;
}


// ======== FUN_0801b43e @ 0801b43e size=24 ========

void FUN_0801b43e(int *param_1,int param_2)

{
  int iVar1;
  
  iVar1 = param_1[1];
  *(int *)(param_2 + 4) = iVar1;
  *(undefined4 *)(param_2 + 8) = *(undefined4 *)(iVar1 + 8);
  *(int *)(*(int *)(iVar1 + 8) + 4) = param_2;
  *(int *)(iVar1 + 8) = param_2;
  *(int **)(param_2 + 0x10) = param_1;
  *param_1 = *param_1 + 1;
  return;
}


// ======== FUN_0801b458 @ 0801b458 size=52 ========

void FUN_0801b458(void)

{
  bool bVar1;
  int iVar2;
  
  bVar1 = (bool)isCurrentModePrivileged();
  if (bVar1) {
    setBasePriority(0x10);
  }
  DataSynchronizationBarrier(0xf);
  InstructionSynchronizationBarrier(0xf);
  iVar2 = *(int *)(DAT_0801b48c + 4) + 1;
  *(int *)(DAT_0801b48c + 4) = iVar2;
  if ((iVar2 == 1) && ((*DAT_0801b490 & 0xff) != 0)) {
    FUN_08011a1a(0,s_Error__s__d_0801b4b4,s____src_platform_freertos_port_c_0801b494,0x1b9);
    return;
  }
  return;
}


// ======== FUN_0801b4c4 @ 0801b4c4 size=36 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0801b4c4(void)

{
  bool bVar1;
  int iVar2;
  int iVar3;
  
  iVar2 = _DAT_0801b4e8;
  if (*(int *)(_DAT_0801b4e8 + 4) == 0) {
    FUN_08011a1a(0,s_Error__s__d_0801b50c,s____src_platform_freertos_port_c_0801b4eb + 1,0x1c0);
  }
  iVar3 = *(int *)(iVar2 + 4) + -1;
  *(int *)(iVar2 + 4) = iVar3;
  if ((iVar3 == 0) && (bVar1 = (bool)isCurrentModePrivileged(), bVar1)) {
    setBasePriority(0);
  }
  return;
}


// ======== FUN_0801b51c @ 0801b51c size=98 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0801b51c(int param_1)

{
  int iVar1;
  int *piVar2;
  
  iVar1 = _DAT_0801b580;
  if (param_1 != 0) {
    piVar2 = (int *)(param_1 + -8);
    if ((*(uint *)(param_1 + -4) & *(uint *)(_DAT_0801b580 + 0xc)) == 0) {
      FUN_08011a1a(0,s_Error__s__d_0801b5a8,s____src_platform_freertos_heap_4__0801b583 + 1,0x140);
    }
    if (*piVar2 != 0) {
      FUN_08011a1a(0,s_Error__s__d_0801b5a8,s____src_platform_freertos_heap_4__0801b583 + 1,0x141);
    }
    if (((*(uint *)(param_1 + -4) & *(uint *)(iVar1 + 0xc)) != 0) && (*piVar2 == 0)) {
      *(uint *)(param_1 + -4) = *(uint *)(param_1 + -4) & ~*(uint *)(iVar1 + 0xc);
      FUN_0801ba58();
      *(int *)(iVar1 + 4) = *(int *)(param_1 + -4) + *(int *)(iVar1 + 4);
      FUN_08018bd4(piVar2);
      FUN_0801c818();
      return;
    }
  }
  return;
}


// ======== FUN_0801b5b8 @ 0801b5b8 size=26 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0801b5b8(void)

{
  _DAT_e000e014 = *DAT_0801b5d4 / 1000 - 1;
  _DAT_e000e010 = 7;
  return;
}


// ======== FUN_0801b5d8 @ 0801b5d8 size=74 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0801b5d8(void)

{
  byte *pbVar1;
  uint uVar2;
  
  uVar2 = FUN_080101f4();
  pbVar1 = _DAT_0801b624;
  if ((0xf < uVar2) && (*(byte *)(uVar2 + 0xe000e3f0) < *_DAT_0801b624)) {
    FUN_08011a1a(0,s_Error__s__d_0801b648,s____src_platform_freertos_port_c_0801b627 + 1,0x300);
  }
  if (*(uint *)(pbVar1 + 8) < (*DAT_0801b658 & 0x700)) {
    FUN_08011a1a(0,s_Error__s__d_0801b648,s____src_platform_freertos_port_c_0801b627 + 1,0x310);
    return;
  }
  return;
}


// ======== FUN_0801b65c @ 0801b65c size=64 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0801b65c(int param_1)

{
  int iVar1;
  
  if (param_1 != 0) {
    if (*(int *)(_DAT_0801b69c + 0x30) != 0) {
      FUN_08011a1a(0,s_Error__s__d_0801b6c4,s____src_platform_freertos_tasks_c_0801b69f + 1,0x4d3);
    }
    FUN_0801ba58();
    FUN_0801885c(param_1,0);
    iVar1 = FUN_0801c818();
    if (iVar1 != 0) {
      return;
    }
  }
  *DAT_0801b6d4 = 0x10000000;
  DataSynchronizationBarrier(0xf);
  InstructionSynchronizationBarrier(0xf);
  return;
}


// ======== FUN_0801b6d8 @ 0801b6d8 size=124 ========

void FUN_0801b6d8(uint *param_1,int param_2)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  
  if (param_1 == (uint *)0x0) {
    FUN_08011a1a(0,s_Error__s__d_0801b778,s____src_platform_freertos_tasks_c_0801b754,0x47d);
  }
  if (param_2 == 0) {
    FUN_08011a1a(0,s_Error__s__d_0801b778,s____src_platform_freertos_tasks_c_0801b754,0x47e);
  }
  iVar1 = DAT_0801b788;
  if (*(int *)(DAT_0801b788 + 0x30) != 0) {
    FUN_08011a1a(0,s_Error__s__d_0801b778,s____src_platform_freertos_tasks_c_0801b754,0x47f);
  }
  FUN_0801ba58();
  uVar3 = *(uint *)(iVar1 + 0xc);
  uVar4 = *param_1;
  uVar2 = uVar4 + param_2;
  if (uVar3 < uVar4) {
    if (uVar2 < uVar4) {
LAB_0801b722:
      if (uVar3 < uVar2) goto LAB_0801b72c;
    }
    *param_1 = uVar2;
  }
  else {
    if (uVar4 <= uVar2) goto LAB_0801b722;
LAB_0801b72c:
    *param_1 = uVar2;
    FUN_0801885c(uVar2 - uVar3,0);
  }
  iVar1 = FUN_0801c818();
  if (iVar1 == 0) {
    *DAT_0801b78c = 0x10000000;
    DataSynchronizationBarrier(0xf);
    InstructionSynchronizationBarrier(0xf);
  }
  return;
}


// ======== FUN_0801b790 @ 0801b790 size=154 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0801b790(int param_1)

{
  int *piVar1;
  int iVar2;
  
  FUN_0801b458();
  piVar1 = DAT_0801b82c;
  if (param_1 == 0) {
    param_1 = *DAT_0801b82c;
  }
  iVar2 = FUN_0801b3ce(param_1 + 4);
  if (iVar2 == 0) {
    if (*(int *)(DAT_0801b830 + *(uint *)(param_1 + 0x2c) * 0x14) == 0) {
      piVar1[4] = piVar1[4] & ~(1 << (*(uint *)(param_1 + 0x2c) & 0xff));
    }
  }
  if (*(int *)(param_1 + 0x28) != 0) {
    FUN_0801b3ce(param_1 + 0x18);
  }
  piVar1[9] = piVar1[9] + 1;
  if (param_1 == *piVar1) {
    FUN_0801b43e(_DAT_0801b834,param_1 + 4);
    piVar1[1] = piVar1[1] + 1;
  }
  else {
    piVar1[2] = piVar1[2] + -1;
    FUN_08018a7e(param_1);
    FUN_08018c3c();
  }
  FUN_0801b4c4();
  if ((piVar1[5] != 0) && (param_1 == *piVar1)) {
    if (piVar1[0xc] != 0) {
      FUN_08011a1a(0,s_Error__s__d_0801b85c,s____src_platform_freertos_tasks_c_0801b837 + 1,0x469);
    }
    *DAT_0801b86c = 0x10000000;
    DataSynchronizationBarrier(0xf);
    InstructionSynchronizationBarrier(0xf);
  }
  return;
}


// ======== FUN_0801b870 @ 0801b870 size=8 ========

void FUN_0801b870(void)

{
  *(undefined4 *)(DAT_0801b878 + 0x1c) = 1;
  return;
}


// ======== FUN_0801b87c @ 0801b87c size=44 ========

void FUN_0801b87c(int param_1,undefined4 param_2)

{
  if (param_1 == 0) {
    FUN_08011a1a(0,s_Error__s__d_0801b8cc,s____src_platform_freertos_tasks_c_0801b8a8,0xb0a);
  }
  FUN_0801b40e(param_1,*DAT_0801b8dc + 0x18);
  FUN_0801885c(param_2,1);
  return;
}


// ======== FUN_0801b8e0 @ 0801b8e0 size=124 ========

void FUN_0801b8e0(int param_1)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  
  piVar1 = DAT_0801b95c;
  if ((param_1 != 0) && (*(uint *)(param_1 + 0x2c) < *(uint *)(*DAT_0801b95c + 0x2c))) {
    if (-1 < *(int *)(param_1 + 0x18)) {
      *(int *)(param_1 + 0x18) = 0x20 - *(int *)(*DAT_0801b95c + 0x2c);
    }
    iVar2 = DAT_0801b960;
    if (*(int *)(param_1 + 0x14) == DAT_0801b960 + *(int *)(param_1 + 0x2c) * 0x14) {
      iVar3 = FUN_0801b3ce(param_1 + 4);
      if ((iVar3 == 0) && (*(int *)(iVar2 + *(uint *)(param_1 + 0x2c) * 0x14) == 0)) {
        piVar1[4] = piVar1[4] & ~(1 << (*(uint *)(param_1 + 0x2c) & 0xff));
      }
      uVar4 = *(uint *)(*piVar1 + 0x2c);
      *(uint *)(param_1 + 0x2c) = uVar4;
      piVar1[4] = 1 << (uVar4 & 0xff) | piVar1[4];
      FUN_0801b43e(iVar2 + uVar4 * 0x14,param_1 + 4);
      return;
    }
    *(undefined4 *)(param_1 + 0x2c) = *(undefined4 *)(*piVar1 + 0x2c);
  }
  return;
}


// ======== FUN_0801b964 @ 0801b964 size=30 ========

void FUN_0801b964(undefined4 *param_1)

{
  int iVar1;
  
  if (param_1 == (undefined4 *)0x0) {
    FUN_08011a1a(0,s_Error__s__d_0801b9a8,s____src_platform_freertos_tasks_c_0801b984,0xbc5);
  }
  iVar1 = DAT_0801b9b8;
  *param_1 = *(undefined4 *)(DAT_0801b9b8 + 0x20);
  param_1[1] = *(undefined4 *)(iVar1 + 0xc);
  return;
}


// ======== FUN_0801ba58 @ 0801ba58 size=10 ========

void FUN_0801ba58(void)

{
  *(int *)(DAT_0801ba64 + 0x30) = *(int *)(DAT_0801ba64 + 0x30) + 1;
  return;
}


// ======== FUN_0801ba68 @ 0801ba68 size=82 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0801ba68(void)

{
  undefined4 *puVar1;
  int iVar2;
  int iVar3;
  
  puVar1 = DAT_0801babc;
  if (DAT_0801babc[0xc] != 0) {
    DAT_0801babc[7] = 1;
    return;
  }
  DAT_0801babc[7] = 0;
  iVar3 = _DAT_0801bac0;
  iVar2 = 0x1f - LZCOUNT(puVar1[4]);
  if (*(int *)(_DAT_0801bac0 + iVar2 * 0x14) == 0) {
    FUN_08011a1a(0,s_Error__s__d_0801bae8,s____src_platform_freertos_tasks_c_0801bac3 + 1,0xafa);
  }
  iVar3 = iVar3 + iVar2 * 0x14;
  iVar2 = *(int *)(*(int *)(iVar3 + 4) + 4);
  *(int *)(iVar3 + 4) = iVar2;
  if (iVar2 == iVar3 + 8) {
    iVar2 = *(int *)(iVar2 + 4);
    *(int *)(iVar3 + 4) = iVar2;
  }
  *puVar1 = *(undefined4 *)(iVar2 + 0xc);
  return;
}


// ======== FUN_0801baf8 @ 0801baf8 size=88 ========

undefined4 FUN_0801baf8(int param_1,int param_2,uint param_3,uint param_4,int param_5,int param_6)

{
  short sVar1;
  undefined4 uVar2;
  int iVar3;
  uint uVar4;
  short sVar5;
  short sVar6;
  
  sVar5 = 0x9c4;
  sVar6 = -0x9c4;
  if ((param_5 != 0) && (param_5 == 1)) {
    sVar5 = 5000;
    sVar6 = -5000;
  }
  uVar4 = 0;
  for (iVar3 = 0; iVar3 < param_2; iVar3 = iVar3 + 1) {
    sVar1 = *(short *)(param_1 + iVar3 * 2);
    if ((sVar5 < sVar1) || (sVar1 < sVar6)) {
      *(short *)(param_6 + iVar3 * 2) = sVar1;
      uVar4 = uVar4 + 1 & 0xffff;
    }
    else {
      *(undefined2 *)(param_6 + iVar3 * 2) = 0;
    }
  }
  if (param_3 < uVar4) {
    uVar2 = 2;
  }
  else if (uVar4 < param_4) {
    uVar2 = 1;
  }
  else {
    uVar2 = 0;
  }
  return uVar2;
}


// ======== FUN_0801bc8c @ 0801bc8c size=36 ========

undefined4 * FUN_0801bc8c(undefined4 param_1)

{
  undefined4 *puVar1;
  
  puVar1 = (undefined4 *)FUN_0801bcb0(1,0,param_1);
  if (puVar1 != (undefined4 *)0x0) {
    puVar1[1] = 0;
    *puVar1 = 0;
    puVar1[3] = 0;
    FUN_0801bf78(puVar1,0,0,0);
  }
  return puVar1;
}


// ======== FUN_0801bcb0 @ 0801bcb0 size=68 ========

int * FUN_0801bcb0(int param_1,int param_2)

{
  int iVar1;
  int *piVar2;
  
  if (param_1 == 0) {
    FUN_08011a1a(0,s_Error__s__d_0801bd18,s____src_platform_freertos_queue_c_0801bcf4,0x188);
  }
  if (param_2 == 0) {
    iVar1 = 0;
  }
  else {
    iVar1 = param_1 * param_2;
  }
  piVar2 = (int *)FUN_08018d24(iVar1 + 0x48);
  if (piVar2 != (int *)0x0) {
    if (param_2 == 0) {
      *piVar2 = (int)piVar2;
    }
    else {
      *piVar2 = (int)(piVar2 + 0x12);
    }
    piVar2[0xf] = param_1;
    piVar2[0x10] = param_2;
    FUN_0801bec0(piVar2,1);
  }
  return piVar2;
}


// ======== FUN_0801bd28 @ 0801bd28 size=352 ========

undefined4 FUN_0801bd28(int *param_1,int param_2,int param_3,int param_4)

{
  bool bVar1;
  undefined4 *puVar2;
  int iVar3;
  int iVar4;
  undefined1 auStack_40 [12];
  int *piStack_34;
  int iStack_30;
  int local_2c;
  int iStack_28;
  
  bVar1 = false;
  piStack_34 = param_1;
  iStack_30 = param_2;
  local_2c = param_3;
  iStack_28 = param_4;
  if (param_1 == (int *)0x0) {
    FUN_08011a1a(0,s_Error__s__d_0801beac,s____src_platform_freertos_queue_c_0801be88,0x4dc);
  }
  if ((param_2 == 0) && (param_1[0x10] != 0)) {
    FUN_08011a1a(0,s_Error__s__d_0801beac,s____src_platform_freertos_queue_c_0801be88,0x4dd);
  }
  iVar3 = FUN_0801c4f4();
  puVar2 = DAT_0801bebc;
  if ((iVar3 == 0) && (local_2c != 0)) {
    FUN_08011a1a(0,s_Error__s__d_0801beac,s____src_platform_freertos_queue_c_0801be88,0x4e0);
    puVar2 = DAT_0801bebc;
  }
  while( true ) {
    FUN_0801b458();
    iVar3 = param_1[0xe];
    if (iVar3 != 0) break;
    if (local_2c == 0) {
      FUN_0801b4c4();
      return 0;
    }
    if (!bVar1) {
      FUN_0801b964(auStack_40);
      bVar1 = true;
    }
    FUN_0801b4c4();
    FUN_0801ba58();
    FUN_0801b458();
    if ((char)param_1[0x11] == -1) {
      *(undefined1 *)(param_1 + 0x11) = 0;
    }
    if (*(char *)((int)param_1 + 0x45) == -1) {
      *(undefined1 *)((int)param_1 + 0x45) = 0;
    }
    FUN_0801b4c4();
    iVar3 = FUN_0801c1e8(auStack_40,&local_2c);
    if (iVar3 == 0) {
      iVar3 = FUN_08018c20(param_1);
      if (iVar3 == 0) {
        FUN_08018cb8(param_1);
        FUN_0801c818();
      }
      else {
        if (*param_1 == 0) {
          FUN_0801b458();
          FUN_0801b8e0(param_1[1]);
          FUN_0801b4c4();
        }
        FUN_0801b87c(param_1 + 9,local_2c);
        FUN_08018cb8(param_1);
        iVar3 = FUN_0801c818();
        if (iVar3 == 0) {
          *puVar2 = 0x10000000;
          DataSynchronizationBarrier(0xf);
          InstructionSynchronizationBarrier(0xf);
        }
      }
    }
    else {
      FUN_08018cb8();
      FUN_0801c818();
      iVar3 = FUN_08018c20(param_1);
      if (iVar3 != 0) {
        return 0;
      }
    }
  }
  iVar4 = param_1[3];
  FUN_080189ec(param_1,param_2);
  if (param_4 == 0) {
    param_1[0xe] = iVar3 + -1;
    if (*param_1 == 0) {
      iVar3 = FUN_08018e38();
      param_1[1] = iVar3;
    }
    if (param_1[4] == 0) goto LAB_0801bdb6;
    param_1 = param_1 + 4;
  }
  else {
    param_1[3] = iVar4;
    if (param_1[9] == 0) goto LAB_0801bdb6;
    param_1 = param_1 + 9;
  }
  iVar3 = FUN_0801c76c(param_1);
  if (iVar3 != 0) {
    *puVar2 = 0x10000000;
    DataSynchronizationBarrier(0xf);
    InstructionSynchronizationBarrier(0xf);
  }
LAB_0801bdb6:
  FUN_0801b4c4();
  return 1;
}


// ======== FUN_0801bec0 @ 0801bec0 size=128 ========

undefined4 FUN_0801bec0(int *param_1,int param_2)

{
  int iVar1;
  
  if (param_1 == (int *)0x0) {
    FUN_08011a1a(0,s_Error__s__d_0801bf64,s____src_platform_freertos_queue_c_0801bf40,0x11b);
  }
  FUN_0801b458();
  param_1[1] = param_1[0xf] * param_1[0x10] + *param_1;
  param_1[0xe] = 0;
  param_1[2] = *param_1;
  param_1[3] = (param_1[0xf] + -1) * param_1[0x10] + *param_1;
  *(undefined1 *)(param_1 + 0x11) = 0xff;
  *(undefined1 *)((int)param_1 + 0x45) = 0xff;
  if (param_2 == 0) {
    if ((param_1[4] != 0) && (iVar1 = FUN_0801c76c(param_1 + 4), iVar1 != 0)) {
      *DAT_0801bf74 = 0x10000000;
      DataSynchronizationBarrier(0xf);
      InstructionSynchronizationBarrier(0xf);
    }
  }
  else {
    FUN_0801b3f2();
    FUN_0801b3f2(param_1 + 9);
  }
  FUN_0801b4c4();
  return 1;
}


// ======== FUN_0801bf78 @ 0801bf78 size=334 ========

undefined4 FUN_0801bf78(int param_1,int param_2,int param_3,int param_4)

{
  bool bVar1;
  undefined4 *puVar2;
  int iVar3;
  undefined1 auStack_38 [8];
  int iStack_30;
  int iStack_2c;
  int local_28;
  int iStack_24;
  
  bVar1 = false;
  iStack_30 = param_1;
  iStack_2c = param_2;
  local_28 = param_3;
  iStack_24 = param_4;
  if (param_1 == 0) {
    FUN_08011a1a(0,s_Error__s__d_0801c0ec,s____src_platform_freertos_queue_c_0801c0c8,0x2d9);
  }
  if ((param_2 == 0) && (*(int *)(param_1 + 0x40) != 0)) {
    FUN_08011a1a(0,s_Error__s__d_0801c0ec,s____src_platform_freertos_queue_c_0801c0c8,0x2da);
  }
  if ((param_4 == 2) && (*(int *)(param_1 + 0x3c) != 1)) {
    FUN_08011a1a(0,s_Error__s__d_0801c0ec,s____src_platform_freertos_queue_c_0801c0c8,0x2db);
  }
  iVar3 = FUN_0801c4f4();
  puVar2 = DAT_0801c0fc;
  if ((iVar3 == 0) && (local_28 != 0)) {
    FUN_08011a1a(0,s_Error__s__d_0801c0ec,s____src_platform_freertos_queue_c_0801c0c8,0x2de);
    puVar2 = DAT_0801c0fc;
  }
  while( true ) {
    FUN_0801b458();
    if ((*(uint *)(param_1 + 0x38) < *(uint *)(param_1 + 0x3c)) || (param_4 == 2)) {
      iVar3 = FUN_08018a12(param_1,param_2,param_4);
      if (*(int *)(param_1 + 0x24) != 0) {
        iVar3 = FUN_0801c76c(param_1 + 0x24);
      }
      if (iVar3 != 0) {
        *puVar2 = 0x10000000;
        DataSynchronizationBarrier(0xf);
        InstructionSynchronizationBarrier(0xf);
      }
      FUN_0801b4c4();
      return 1;
    }
    if (local_28 == 0) {
      FUN_0801b4c4();
      return 0;
    }
    if (!bVar1) {
      FUN_0801b964(auStack_38);
      bVar1 = true;
    }
    FUN_0801b4c4();
    FUN_0801ba58();
    FUN_0801b458();
    if (*(char *)(param_1 + 0x44) == -1) {
      *(undefined1 *)(param_1 + 0x44) = 0;
    }
    if (*(char *)(param_1 + 0x45) == -1) {
      *(undefined1 *)(param_1 + 0x45) = 0;
    }
    FUN_0801b4c4();
    iVar3 = FUN_0801c1e8(auStack_38,&local_28);
    if (iVar3 != 0) break;
    FUN_0801b458();
    if (*(int *)(param_1 + 0x38) == *(int *)(param_1 + 0x3c)) {
      FUN_0801b4c4();
      FUN_0801b87c(param_1 + 0x10,local_28);
      FUN_08018cb8(param_1);
      iVar3 = FUN_0801c818();
      if (iVar3 == 0) {
        *puVar2 = 0x10000000;
        DataSynchronizationBarrier(0xf);
        InstructionSynchronizationBarrier(0xf);
      }
    }
    else {
      FUN_0801b4c4();
      FUN_08018cb8(param_1);
      FUN_0801c818();
    }
  }
  FUN_08018cb8(param_1);
  FUN_0801c818();
  return 0;
}


// ======== FUN_0801c100 @ 0801c100 size=180 ========

undefined4 FUN_0801c100(int param_1,int param_2,undefined4 *param_3,int param_4)

{
  char cVar1;
  bool bVar2;
  undefined4 uVar3;
  int iVar4;
  undefined4 uVar5;
  
  if (param_1 == 0) {
    FUN_08011a1a(0,s_Error__s__d_0801c1d8,s____src_platform_freertos_queue_c_0801c1b4,0x39f);
  }
  if ((param_2 == 0) && (*(int *)(param_1 + 0x40) != 0)) {
    FUN_08011a1a(0,s_Error__s__d_0801c1d8,s____src_platform_freertos_queue_c_0801c1b4,0x3a0);
  }
  if ((param_4 == 2) && (*(int *)(param_1 + 0x3c) != 1)) {
    FUN_08011a1a(0,s_Error__s__d_0801c1d8,s____src_platform_freertos_queue_c_0801c1b4,0x3a1);
  }
  FUN_0801b5d8();
  uVar5 = 0;
  bVar2 = (bool)isCurrentModePrivileged();
  if (bVar2) {
    uVar5 = getBasePriority();
  }
  bVar2 = (bool)isCurrentModePrivileged();
  if (bVar2) {
    setBasePriority(0x10);
  }
  DataSynchronizationBarrier(0xf);
  InstructionSynchronizationBarrier(0xf);
  if ((*(uint *)(param_1 + 0x38) < *(uint *)(param_1 + 0x3c)) || (param_4 == 2)) {
    cVar1 = *(char *)(param_1 + 0x45);
    FUN_08018a12(param_1,param_2,param_4);
    if (cVar1 == -1) {
      if (((*(int *)(param_1 + 0x24) != 0) && (iVar4 = FUN_0801c76c(param_1 + 0x24), iVar4 != 0)) &&
         (param_3 != (undefined4 *)0x0)) {
        *param_3 = 1;
      }
    }
    else {
      *(char *)(param_1 + 0x45) = cVar1 + '\x01';
    }
    uVar3 = 1;
  }
  else {
    uVar3 = 0;
  }
  bVar2 = (bool)isCurrentModePrivileged();
  if (bVar2) {
    setBasePriority(uVar5);
  }
  return uVar3;
}


// ======== FUN_0801c1e8 @ 0801c1e8 size=96 ========

undefined4 FUN_0801c1e8(int *param_1,uint *param_2)

{
  uint uVar1;
  uint uVar2;
  undefined4 uVar3;
  
  if (param_1 == (int *)0x0) {
    FUN_08011a1a(0,s_Error__s__d_0801c26c,s____src_platform_freertos_tasks_c_0801c248,0xbcf);
  }
  if (param_2 == (uint *)0x0) {
    FUN_08011a1a(0,s_Error__s__d_0801c26c,s____src_platform_freertos_tasks_c_0801c248,0xbd0);
  }
  FUN_0801b458();
  uVar1 = *param_2;
  if (uVar1 == 0xffffffff) {
LAB_0801c23a:
    uVar3 = 0;
  }
  else {
    if ((*param_1 == *(int *)(DAT_0801c27c + 0x20)) ||
       (*(uint *)(DAT_0801c27c + 0xc) < (uint)param_1[1])) {
      uVar2 = *(uint *)(DAT_0801c27c + 0xc) - param_1[1];
      if (uVar2 < uVar1) {
        *param_2 = uVar1 - uVar2;
        FUN_0801b964(param_1);
        goto LAB_0801c23a;
      }
    }
    uVar3 = 1;
  }
  FUN_0801b4c4();
  return uVar3;
}


// ======== FUN_0801c280 @ 0801c280 size=86 ========

undefined4
FUN_0801c280(undefined4 param_1,undefined4 param_2,int param_3,undefined4 param_4,undefined4 param_5
            ,undefined4 param_6)

{
  int iVar1;
  int iVar2;
  
  iVar1 = FUN_08018d24(param_3 << 2);
  if (iVar1 != 0) {
    iVar2 = FUN_08018d24(0x58);
    if (iVar2 != 0) {
      *(int *)(iVar2 + 0x30) = iVar1;
      FUN_08018b08(param_1,param_2,param_3,param_4,param_5,param_6,iVar2,0);
      FUN_080188cc(iVar2);
      return 1;
    }
    FUN_0801b51c(iVar1);
  }
  return 0xffffffff;
}


// ======== FUN_0801c2d8 @ 0801c2d8 size=180 ========

undefined4 FUN_0801c2d8(int param_1,uint param_2,undefined4 param_3,undefined4 *param_4)

{
  char cVar1;
  int *piVar2;
  uint uVar3;
  undefined4 uVar4;
  
  uVar4 = 1;
  if (param_1 == 0) {
    FUN_08011a1a(0,s_Error__s__d_0801c3b8,s____src_platform_freertos_tasks_c_0801c394,0x1101);
  }
  FUN_0801b458();
  if (param_4 != (undefined4 *)0x0) {
    *param_4 = *(undefined4 *)(param_1 + 0x50);
  }
  cVar1 = *(char *)(param_1 + 0x54);
  *(undefined1 *)(param_1 + 0x54) = 2;
  switch(param_3) {
  default:
    goto switchD_0801c310_caseD_0;
  case 1:
    param_2 = *(uint *)(param_1 + 0x50) | param_2;
    break;
  case 2:
    param_2 = *(int *)(param_1 + 0x50) + 1;
    break;
  case 3:
    goto switchD_0801c310_caseD_3;
  case 4:
    if (cVar1 == '\x02') {
      uVar4 = 0;
      goto LAB_0801c388;
    }
    goto switchD_0801c310_caseD_3;
  }
  *(uint *)(param_1 + 0x50) = param_2;
switchD_0801c310_caseD_0:
  if (cVar1 == '\x01') {
    FUN_0801b3ce(param_1 + 4);
    piVar2 = DAT_0801c3c8;
    uVar3 = *(uint *)(param_1 + 0x2c);
    DAT_0801c3c8[4] = 1 << (uVar3 & 0xff) | DAT_0801c3c8[4];
    FUN_0801b43e(DAT_0801c3cc + uVar3 * 0x14,param_1 + 4);
    if (*(int *)(param_1 + 0x28) != 0) {
      FUN_08011a1a(0,s_Error__s__d_0801c3b8,s____src_platform_freertos_tasks_c_0801c394,0x1139);
    }
    if (*(uint *)(*piVar2 + 0x2c) < *(uint *)(param_1 + 0x2c)) {
      *DAT_0801c3d0 = 0x10000000;
      DataSynchronizationBarrier(0xf);
      InstructionSynchronizationBarrier(0xf);
    }
  }
LAB_0801c388:
  FUN_0801b4c4();
  return uVar4;
switchD_0801c310_caseD_3:
  *(uint *)(param_1 + 0x50) = param_2;
  goto switchD_0801c310_caseD_0;
}


// ======== FUN_0801c3d4 @ 0801c3d4 size=216 ========

undefined4
FUN_0801c3d4(int param_1,uint param_2,undefined4 param_3,undefined4 *param_4,undefined4 *param_5)

{
  char cVar1;
  bool bVar2;
  int *piVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  
  uVar8 = 1;
  if (param_1 == 0) {
    FUN_08011a1a(0,s_Error__s__d_0801c4d8,s____src_platform_freertos_tasks_c_0801c4b4,0x116c);
  }
  FUN_0801b5d8();
  uVar7 = 0;
  bVar2 = (bool)isCurrentModePrivileged();
  if (bVar2) {
    uVar7 = getBasePriority();
  }
  bVar2 = (bool)isCurrentModePrivileged();
  if (bVar2) {
    setBasePriority(0x10);
  }
  DataSynchronizationBarrier(0xf);
  InstructionSynchronizationBarrier(0xf);
  if (param_4 != (undefined4 *)0x0) {
    *param_4 = *(undefined4 *)(param_1 + 0x50);
  }
  cVar1 = *(char *)(param_1 + 0x54);
  *(undefined1 *)(param_1 + 0x54) = 2;
  switch(param_3) {
  default:
    goto switchD_0801c422_caseD_0;
  case 1:
    param_2 = *(uint *)(param_1 + 0x50) | param_2;
    break;
  case 2:
    param_2 = *(int *)(param_1 + 0x50) + 1;
    break;
  case 3:
    goto switchD_0801c422_caseD_3;
  case 4:
    if (cVar1 == '\x02') {
      uVar8 = 0;
      goto LAB_0801c4a8;
    }
    goto switchD_0801c422_caseD_3;
  }
  *(uint *)(param_1 + 0x50) = param_2;
switchD_0801c422_caseD_0:
  if (cVar1 == '\x01') {
    if (*(int *)(param_1 + 0x28) != 0) {
      FUN_08011a1a(0,s_Error__s__d_0801c4d8,s____src_platform_freertos_tasks_c_0801c4b4,0x11b3);
    }
    piVar3 = DAT_0801c4e8;
    if (DAT_0801c4e8[0xc] == 0) {
      FUN_0801b3ce(param_1 + 4);
      uVar6 = *(uint *)(param_1 + 0x2c);
      piVar3[4] = 1 << (uVar6 & 0xff) | piVar3[4];
      iVar4 = DAT_0801c4f0 + uVar6 * 0x14;
      iVar5 = param_1 + 4;
    }
    else {
      iVar5 = param_1 + 0x18;
      iVar4 = DAT_0801c4ec;
    }
    FUN_0801b43e(iVar4,iVar5);
    if (*(uint *)(*piVar3 + 0x2c) < *(uint *)(param_1 + 0x2c)) {
      if (param_5 == (undefined4 *)0x0) {
        piVar3[7] = 1;
      }
      else {
        *param_5 = 1;
      }
    }
  }
LAB_0801c4a8:
  bVar2 = (bool)isCurrentModePrivileged();
  if (bVar2) {
    setBasePriority(uVar7);
  }
  return uVar8;
switchD_0801c422_caseD_3:
  *(uint *)(param_1 + 0x50) = param_2;
  goto switchD_0801c422_caseD_0;
}


// ======== FUN_0801c4f4 @ 0801c4f4 size=22 ========

undefined4 FUN_0801c4f4(void)

{
  if (*(int *)(DAT_0801c50c + 0x14) == 0) {
    return 1;
  }
  if (*(int *)(DAT_0801c50c + 0x30) != 0) {
    return 0;
  }
  return 2;
}


// ======== FUN_0801c510 @ 0801c510 size=6 ========

undefined4 FUN_0801c510(void)

{
  return *(undefined4 *)(DAT_0801c518 + 0xc);
}


// ======== FUN_0801c51c @ 0801c51c size=190 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_0801c51c(void)

{
  int *piVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  uint uVar5;
  undefined4 uVar6;
  
  piVar1 = _DAT_0801c5dc;
  uVar6 = 0;
  if (_DAT_0801c5dc[0xc] == 0) {
    uVar5 = _DAT_0801c5dc[3] + 1;
    _DAT_0801c5dc[3] = uVar5;
    if (uVar5 == 0) {
      if (*(int *)piVar1[0xd] != 0) {
        FUN_08011a1a(0,s_Error__s__d_0801c604,s____src_platform_freertos_tasks_c_0801c5df + 1,0x9dd)
        ;
      }
      iVar2 = piVar1[0xd];
      piVar1[0xd] = piVar1[0xe];
      piVar1[0xe] = iVar2;
      piVar1[8] = piVar1[8] + 1;
      FUN_08018c3c();
    }
    iVar2 = DAT_0801c614;
    if ((uint)piVar1[10] <= uVar5) {
      while (*(int *)piVar1[0xd] != 0) {
        iVar4 = *(int *)(*(int *)(piVar1[0xd] + 0xc) + 0xc);
        uVar3 = *(uint *)(iVar4 + 4);
        if (uVar5 < uVar3) goto LAB_0801c584;
        FUN_0801b3ce(iVar4 + 4);
        if (*(int *)(iVar4 + 0x28) != 0) {
          FUN_0801b3ce(iVar4 + 0x18);
        }
        uVar3 = *(uint *)(iVar4 + 0x2c);
        piVar1[4] = 1 << (uVar3 & 0xff) | piVar1[4];
        FUN_0801b43e(iVar2 + uVar3 * 0x14,iVar4 + 4);
        if (*(uint *)(*piVar1 + 0x2c) <= *(uint *)(iVar4 + 0x2c)) {
          uVar6 = 1;
        }
      }
      uVar3 = 0xffffffff;
LAB_0801c584:
      piVar1[10] = uVar3;
    }
    if (1 < *(uint *)(iVar2 + *(int *)(*piVar1 + 0x2c) * 0x14)) {
      uVar6 = 1;
    }
  }
  else {
    _DAT_0801c5dc[6] = _DAT_0801c5dc[6] + 1;
  }
  if (piVar1[7] != 0) {
    uVar6 = 1;
  }
  return uVar6;
}


// ======== FUN_0801c618 @ 0801c618 size=130 ========

bool FUN_0801c618(uint param_1,uint param_2,undefined4 *param_3,int param_4)

{
  int *piVar1;
  bool bVar2;
  
  FUN_0801b458();
  piVar1 = DAT_0801c69c;
  if (*(char *)(*DAT_0801c69c + 0x54) != '\x02') {
    *(uint *)(*DAT_0801c69c + 0x50) = *(uint *)(*DAT_0801c69c + 0x50) & ~param_1;
    *(undefined1 *)(*piVar1 + 0x54) = 1;
    if (param_4 != 0) {
      FUN_0801885c(param_4,1);
      *DAT_0801c6a0 = 0x10000000;
      DataSynchronizationBarrier(0xf);
      InstructionSynchronizationBarrier(0xf);
    }
  }
  FUN_0801b4c4();
  FUN_0801b458();
  if (param_3 != (undefined4 *)0x0) {
    *param_3 = *(undefined4 *)(*piVar1 + 0x50);
  }
  bVar2 = *(char *)(*piVar1 + 0x54) != '\x01';
  if (bVar2) {
    *(uint *)(*piVar1 + 0x50) = *(uint *)(*piVar1 + 0x50) & ~param_2;
  }
  *(undefined1 *)(*piVar1 + 0x54) = 0;
  FUN_0801b4c4();
  return bVar2;
}


// ======== FUN_0801c6a4 @ 0801c6a4 size=140 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_0801c6a4(int param_1)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  undefined4 uVar5;
  
  piVar1 = _DAT_0801c730;
  uVar5 = 0;
  if (param_1 != 0) {
    if (param_1 != *_DAT_0801c730) {
      FUN_08011a1a(0,s_Error__s__d_0801c758,s____src_platform_freertos_tasks_c_0801c733 + 1,0xecb);
    }
    if (*(int *)(param_1 + 0x4c) == 0) {
      FUN_08011a1a(0,s_Error__s__d_0801c758,s____src_platform_freertos_tasks_c_0801c733 + 1,0xecd);
    }
    iVar2 = *(int *)(param_1 + 0x4c) + -1;
    *(int *)(param_1 + 0x4c) = iVar2;
    if ((*(int *)(param_1 + 0x2c) != *(int *)(param_1 + 0x48)) && (iVar2 == 0)) {
      iVar3 = FUN_0801b3ce(param_1 + 4);
      iVar2 = DAT_0801c768;
      if ((iVar3 == 0) && (*(int *)(DAT_0801c768 + *(uint *)(param_1 + 0x2c) * 0x14) == 0)) {
        piVar1[4] = piVar1[4] & ~(1 << (*(uint *)(param_1 + 0x2c) & 0xff));
      }
      *(int *)(param_1 + 0x2c) = *(int *)(param_1 + 0x48);
      *(int *)(param_1 + 0x18) = 0x20 - *(int *)(param_1 + 0x48);
      uVar4 = *(uint *)(param_1 + 0x2c);
      piVar1[4] = 1 << (uVar4 & 0xff) | piVar1[4];
      FUN_0801b43e(iVar2 + uVar4 * 0x14,param_1 + 4);
      uVar5 = 1;
    }
  }
  return uVar5;
}


// ======== FUN_0801c76c @ 0801c76c size=106 ========

bool FUN_0801c76c(int param_1)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  
  iVar6 = *(int *)(*(int *)(param_1 + 0xc) + 0xc);
  if (iVar6 == 0) {
    FUN_08011a1a(0,s_Error__s__d_0801c7fc,s____src_platform_freertos_tasks_c_0801c7d8,0xb65);
  }
  iVar2 = iVar6 + 0x18;
  FUN_0801b3ce();
  piVar1 = DAT_0801c80c;
  iVar3 = DAT_0801c810;
  if (DAT_0801c80c[0xc] == 0) {
    FUN_0801b3ce(iVar6 + 4);
    uVar4 = *(uint *)(iVar6 + 0x2c);
    piVar1[4] = 1 << (uVar4 & 0xff) | piVar1[4];
    iVar2 = iVar6 + 4;
    iVar3 = DAT_0801c814 + uVar4 * 0x14;
  }
  FUN_0801b43e(iVar3,iVar2);
  uVar4 = *(uint *)(iVar6 + 0x2c);
  uVar5 = *(uint *)(*piVar1 + 0x2c);
  if (uVar5 < uVar4) {
    piVar1[7] = 1;
  }
  return uVar5 < uVar4;
}


// ======== FUN_0801c818 @ 0801c818 size=182 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_0801c818(void)

{
  int *piVar1;
  int *piVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  undefined4 uVar6;
  
  piVar1 = _DAT_0801c8d0;
  iVar5 = 0;
  uVar6 = 0;
  if (_DAT_0801c8d0[0xc] == 0) {
    FUN_08011a1a(0,s_Error__s__d_0801c8f8,s____src_platform_freertos_tasks_c_0801c8d3 + 1,0x7ec);
  }
  FUN_0801b458();
  piVar1[0xc] = piVar1[0xc] + -1;
  piVar2 = DAT_0801c908;
  if ((piVar1[0xc] == 0) && (piVar1[2] != 0)) {
    while (*piVar2 != 0) {
      iVar5 = *(int *)(piVar2[3] + 0xc);
      FUN_0801b3ce(iVar5 + 0x18);
      FUN_0801b3ce(iVar5 + 4);
      uVar3 = *(uint *)(iVar5 + 0x2c);
      piVar1[4] = 1 << (uVar3 & 0xff) | piVar1[4];
      FUN_0801b43e(piVar2 + uVar3 * 5 + -0xaa,iVar5 + 4);
      if (*(uint *)(*piVar1 + 0x2c) <= *(uint *)(iVar5 + 0x2c)) {
        piVar1[7] = 1;
      }
    }
    if (iVar5 != 0) {
      FUN_08018c3c();
    }
    iVar5 = piVar1[6];
    if (iVar5 != 0) {
      do {
        iVar4 = FUN_0801c51c();
        if (iVar4 != 0) {
          piVar1[7] = 1;
        }
        iVar5 = iVar5 + -1;
      } while (iVar5 != 0);
      piVar1[6] = 0;
    }
    if (piVar1[7] != 0) {
      uVar6 = 1;
      *DAT_0801c90c = 0x10000000;
      DataSynchronizationBarrier(0xf);
      InstructionSynchronizationBarrier(0xf);
    }
  }
  FUN_0801b4c4();
  return uVar6;
}

