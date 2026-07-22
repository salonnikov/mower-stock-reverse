
/* ===== FORCE @ 0x0806fce0 ===== */
/* function: FUN_0806fce0 size=8 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_0806fce0(void)

{
  return *(undefined4 *)(*_DAT_0806fce8 + 0x18);
}


/* ===== FORCE @ 0x08070268 ===== */
/* function: FUN_08070268 size=194 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_08070268(void)

{
  byte bVar1;
  undefined1 uVar2;
  undefined1 uVar3;
  int iVar4;
  ushort *puVar5;
  int in_r3;
  int iVar6;
  int unaff_r5;
  int iVar7;
  uint uVar8;
  int unaff_r6;
  int unaff_r7;
  undefined4 uVar9;
  int *unaff_r8;
  undefined4 unaff_r9;
  undefined4 in_stack_00000014;
  
  *(int *)(unaff_r6 + 0x54) = in_r3;
  puVar5 = *(ushort **)(unaff_r6 + 0x14);
  uVar9 = *(undefined4 *)(unaff_r7 * 0x200000 + 4);
  iVar7 = *(int *)(unaff_r5 + 0x74);
  iVar6 = *(int *)(unaff_r6 + 0x74);
  *(char *)(unaff_r6 + 1) = (char)in_r3;
  *(int *)(iVar6 + 0x54) = iVar7;
  *(int *)(iVar6 + 0x14) = unaff_r6;
  iVar4 = *(int *)(iVar7 + 0x44);
  *(int *)(iVar6 + 0x44) = iVar7;
  uVar8 = (uint)*puVar5;
  uVar2 = (undefined1)iVar6;
  *(undefined1 *)(in_r3 + 0xd) = uVar2;
  *(ushort **)(unaff_r6 + 0x34) = puVar5;
  *(undefined1 *)(in_r3 + 1) = uVar2;
  *(int *)(uVar8 + 0x14) = iVar6;
  *(int *)(unaff_r6 + 100) = iVar6;
  uVar3 = (undefined1)uVar9;
  *(undefined1 *)(uVar8 + 9) = uVar3;
  bVar1 = *(byte *)(uVar8 + iVar4);
  *(undefined1 *)(iVar6 + 9) = uVar2;
  *(char *)(bVar1 + 0x19) = (char)iVar4;
  *(byte *)(iVar6 + 9) = bVar1;
  *(int *)(in_r3 + 0x44) = iVar6;
  *(int *)(unaff_r6 + 0x54) = unaff_r6;
  iVar7 = *(int *)(bVar1 + 0x74);
  iVar6 = *(int *)(unaff_r6 + 0x74);
  *(char *)(iVar6 + 9) = (char)iVar6;
  *(char *)(iVar7 + 0x19) = (char)iVar4;
  *(char *)(iVar6 + 9) = (char)iVar7;
  *(undefined1 *)(in_r3 + 9) = uVar3;
  *(int *)(iVar7 + 0x74) = iVar4;
  *(undefined1 *)(iVar7 + 0x11) = 0;
  uVar2 = (undefined1)*(undefined4 *)(in_r3 + 0x54);
  *(undefined1 *)(iVar7 + 0x11) = uVar2;
  *(undefined1 *)(iVar7 + 9) = uVar2;
  *(undefined1 *)(in_r3 + 0xd) = uVar2;
  iVar7 = *(int *)(iVar7 + 0x34);
  *(undefined1 *)(in_r3 + 0x19) = uVar2;
  uVar9 = *(undefined4 *)(iVar7 + 0x54);
  *(char *)(iVar6 + 9) = (char)iVar6;
  iVar4 = FUN_0801f4dc(0,iVar4,uVar9,iVar6 << 1);
  if (iVar4 != 0) {
    FUN_08048adc(2,0x80702d0,0x8070290,_DAT_08070308);
  }
  *(undefined4 *)(*unaff_r8 + 0x10) = unaff_r9;
  FUN_0806fb54(in_stack_00000014);
  FUN_0807f696(&TIMER2_CTL0,2,0x1c20);
  *(undefined2 *)(DAT_2000057c + 2) = 0x1c20;
  return;
}

