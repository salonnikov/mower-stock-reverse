
/* ===== FORCE @ 0x0801c818 ===== */
/* function: FUN_0801c818 size=182 */

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
    FUN_08011a1a(0,0x801c8f8,0x801c8d4,0x7ec);
  }
  FUN_0801b458();
  piVar1[0xc] = piVar1[0xc] + -1;
  piVar2 = _DAT_0801c908;
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
      *_DAT_0801c90c = 0x10000000;
      DataSynchronizationBarrier(0xf);
      InstructionSynchronizationBarrier(0xf);
    }
  }
  FUN_0801b4c4();
  return uVar6;
}

