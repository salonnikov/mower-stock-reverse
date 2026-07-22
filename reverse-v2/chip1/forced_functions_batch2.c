
/* ===== FORCE @ 0x08053af0 ===== */
/* function: FUN_08053af0 size=326 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_08053af0(void)

{
  int iVar1;
  byte bVar2;
  byte bVar3;
  code *pcVar4;
  undefined4 in_r3;
  byte bVar5;
  byte bVar6;
  int iVar7;
  int iVar8;
  undefined4 local_8;
  
  local_8 = in_r3;
  local_8 = FUN_08084c50();
  iVar1 = _DAT_08053c40;
  bVar6 = 3;
  bVar2 = 1;
  iVar8 = 2;
  bVar3 = 4;
  bVar5 = 5;
  do {
    FUN_0808368c(&local_8,10);
    if (*(char *)(*(int *)(iVar1 + 4) + 0x6c) != '\0') {
      for (iVar7 = 0; iVar7 < (int)(uint)*(byte *)(*(int *)(iVar1 + 4) + 0x6c); iVar7 = iVar7 + 1) {
        pcVar4 = *(code **)(*(int *)(iVar1 + 4) + iVar7 * 4 + 8);
        if (pcVar4 != (code *)0x0) {
          (*pcVar4)();
        }
      }
    }
    if (*(char *)(*(int *)(iVar1 + 4) + 0x6e) != '\0') {
      if (iVar8 == 0) {
        iVar8 = 1;
      }
      else {
        iVar8 = 0;
        for (iVar7 = 0; iVar7 < (int)(uint)*(byte *)(*(int *)(iVar1 + 4) + 0x6e); iVar7 = iVar7 + 1)
        {
          pcVar4 = *(code **)(*(int *)(iVar1 + 4) + iVar7 * 4 + 0x30);
          if (pcVar4 != (code *)0x0) {
            (*pcVar4)();
          }
        }
      }
    }
    if (*(char *)(*(int *)(iVar1 + 4) + 0x6d) != '\0') {
      if (bVar2 < 9) {
        bVar2 = bVar2 + 1;
      }
      else {
        bVar2 = 0;
        for (iVar7 = 0; iVar7 < (int)(uint)*(byte *)(*(int *)(iVar1 + 4) + 0x6d); iVar7 = iVar7 + 1)
        {
          pcVar4 = *(code **)(*(int *)(iVar1 + 4) + iVar7 * 4 + 0x1c);
          if (pcVar4 != (code *)0x0) {
            (*pcVar4)();
          }
        }
      }
    }
    if (*(char *)(*(int *)(iVar1 + 4) + 0x6f) != '\0') {
      if (bVar6 < 0x13) {
        bVar6 = bVar6 + 1;
      }
      else {
        bVar6 = 0;
        for (iVar7 = 0; iVar7 < (int)(uint)*(byte *)(*(int *)(iVar1 + 4) + 0x6f); iVar7 = iVar7 + 1)
        {
          pcVar4 = *(code **)(*(int *)(iVar1 + 4) + iVar7 * 4 + 0x44);
          if (pcVar4 != (code *)0x0) {
            (*pcVar4)();
          }
        }
      }
    }
    if (*(char *)(*(int *)(iVar1 + 4) + 0x70) != '\0') {
      if (bVar3 < 0x18) {
        bVar3 = bVar3 + 1;
      }
      else {
        bVar3 = 0;
        for (iVar7 = 0; iVar7 < (int)(uint)*(byte *)(*(int *)(iVar1 + 4) + 0x70); iVar7 = iVar7 + 1)
        {
          pcVar4 = *(code **)(*(int *)(iVar1 + 4) + iVar7 * 4 + 0x50);
          if (pcVar4 != (code *)0x0) {
            (*pcVar4)();
          }
        }
      }
    }
    if (*(char *)(*(int *)(iVar1 + 4) + 0x71) != '\0') {
      if (bVar5 < 99) {
        bVar5 = bVar5 + 1;
      }
      else {
        bVar5 = 0;
        for (iVar7 = 0; iVar7 < (int)(uint)*(byte *)(*(int *)(iVar1 + 4) + 0x71); iVar7 = iVar7 + 1)
        {
          pcVar4 = *(code **)(*(int *)(iVar1 + 4) + iVar7 * 4 + 0x5c);
          if (pcVar4 != (code *)0x0) {
            (*pcVar4)();
          }
        }
      }
    }
  } while( true );
}


/* ===== FORCE @ 0x0806fe66 ===== */
/* function: FUN_0806fe66 size=1070 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0806fe66(undefined4 param_1,uint param_2)

{
  int *piVar1;
  ushort uVar2;
  int iVar3;
  undefined4 uVar4;
  int iVar5;
  int iVar6;
  uint uVar7;
  uint uVar8;
  int iVar9;
  uint uVar10;
  uint uVar11;
  uint uVar12;
  
  piVar1 = _DAT_08070268;
  uVar8 = 0;
  iVar9 = *_DAT_08070268;
  uVar7 = param_2 * 0x1c20;
  iVar6 = *(int *)(iVar9 + 0x10);
  iVar5 = iVar9 + 0x30;
  iVar3 = iVar9 + 0x28;
  if (*(char *)(iVar9 + 8) != '\x01') {
    uVar10 = param_2 - 0x266;
    uVar11 = param_2 - 0x99;
    uVar12 = param_2 - 0x132;
    if (iVar6 == 1) {
      if (uVar10 < 0x1598) {
LAB_080700cc:
        uVar2 = (ushort)(uVar7 / 0x17fd);
      }
      else {
        if (0x17fd < param_2) {
LAB_08070196:
          uVar8 = 0x1c20;
          goto LAB_080701ea;
        }
        if (0x265 < param_2) goto LAB_080701ea;
        if (0xacb < uVar12) {
          if (0x564 < uVar11) {
            iVar3 = FUN_0801f4dc(iVar3,iVar5,2);
            if (iVar3 != 0) {
              FUN_08048adc(2,0x80702d0,0x8070290,_DAT_0807028c,0x162,0x807026c);
            }
            *(undefined4 *)(*piVar1 + 0x10) = 2;
            goto LAB_080701e6;
          }
          iVar3 = FUN_0801f4dc(iVar3,iVar5,2);
          if (iVar3 != 0) {
            FUN_08048adc(2,0x80702d0,0x8070290,_DAT_0807028c,0x15c,0x807026c);
          }
          *(undefined4 *)(*piVar1 + 0x10) = 2;
          goto LAB_080701cc;
        }
        iVar3 = FUN_0801f4dc(iVar3,iVar5,0);
        if (iVar3 != 0) {
          uVar4 = 0x156;
LAB_0807021e:
          FUN_08048adc(2,0x80702d0,0x8070290,_DAT_0807028c,uVar4,0x807026c);
        }
LAB_0807022e:
        *(undefined4 *)(*piVar1 + 0x10) = 0;
LAB_0807015c:
        uVar2 = (ushort)(uVar7 / 0xbfd);
      }
    }
    else {
      if (iVar6 == 0) {
        if (uVar12 < 0xacc) goto LAB_0807015c;
        if (0xbfd < param_2) {
          iVar3 = FUN_0801f4dc(iVar3,iVar5,1);
          if (iVar3 != 0) {
            FUN_08048adc(2,0x80702d0,0x8070290,_DAT_0807028c,0x16d,0x807026c);
          }
          *(undefined4 *)(*piVar1 + 0x10) = 1;
          if (0x1597 < uVar10) goto LAB_08070196;
          goto LAB_080700cc;
        }
        iVar3 = FUN_0801f4dc(iVar3,iVar5,2);
        if (iVar3 != 0) {
          FUN_08048adc(2,0x80702d0,0x8070290,_DAT_0807028c,0x177,0x807026c);
        }
        *(undefined4 *)(*piVar1 + 0x10) = 2;
        if (0x564 < uVar11) goto LAB_080701e6;
      }
      else {
        if (iVar6 != 2) goto LAB_080701ea;
        if (0x564 < uVar11) {
          if (param_2 < 0x99) {
LAB_080701e6:
            uVar8 = 0x2ce;
            goto LAB_080701ea;
          }
          if (uVar12 < 0xacc) {
            iVar3 = FUN_0801f4dc(iVar3,iVar5,0);
            if (iVar3 != 0) {
              uVar4 = 0x187;
              goto LAB_0807021e;
            }
            goto LAB_0807022e;
          }
          if (0x1597 < uVar10) {
            iVar3 = FUN_0801f4dc(iVar3,iVar5,1);
            if (iVar3 != 0) {
              FUN_08048adc(2,0x80702d0,0x8070290,_DAT_08070308,0x193,0x807026c);
            }
            *(undefined4 *)(*piVar1 + 0x10) = 1;
            goto LAB_08070196;
          }
          iVar3 = FUN_0801f4dc(iVar3,iVar5,1);
          if (iVar3 != 0) {
            FUN_08048adc(2,0x80702d0,0x8070290,_DAT_0807028c,0x18d,0x807026c);
          }
          *(undefined4 *)(*piVar1 + 0x10) = 1;
          goto LAB_080700cc;
        }
      }
LAB_080701cc:
      uVar2 = (ushort)(uVar7 / 0x5fd);
    }
    uVar8 = (uint)uVar2;
    goto LAB_080701ea;
  }
  uVar8 = param_2 - 0x5ff;
  uVar10 = param_2 - 0x17f;
  uVar11 = param_2 - 0x2ff;
  if (iVar6 == 1) {
    if (uVar8 < 0x11ff) {
      uVar2 = (ushort)(uVar7 / 0x17fd);
      goto LAB_0806ff86;
    }
    if (param_2 < 0x17fe) {
      if (0x8fe < uVar11) {
        if (uVar10 < 0x47f) {
          iVar3 = FUN_0801f4dc(iVar3,iVar5,2);
          if (iVar3 != 0) {
            uVar4 = 0x103;
LAB_0806fff6:
            FUN_08048adc(2,0x80702d0,0x8070290,_DAT_0807028c,uVar4,0x807026c);
          }
          goto LAB_08070006;
        }
        iVar3 = FUN_0801f4dc(iVar3,iVar5,2);
        if (iVar3 != 0) {
          FUN_08048adc(2,0x80702d0,0x8070290,_DAT_0807028c,0x10a,0x807026c);
        }
        iVar3 = *piVar1;
LAB_08070036:
        uVar8 = 0x706;
        *(undefined4 *)(iVar3 + 0x10) = 2;
        goto LAB_0807003e;
      }
      iVar3 = FUN_0801f4dc(iVar3,iVar5,0);
      if (iVar3 != 0) {
        uVar4 = 0xfc;
LAB_08070068:
        FUN_08048adc(2,0x80702d0,0x8070290,_DAT_0807028c,uVar4,0x807026c);
      }
LAB_08070078:
      uVar7 = uVar7 / 0xbfd;
      iVar3 = *piVar1;
      *(undefined4 *)(iVar3 + 0x10) = 0;
      goto LAB_0806ff54;
    }
    uVar8 = 0x1c20;
  }
  else {
    if (iVar6 == 0) {
      if (0x8fe < uVar11) {
        if (uVar8 < 0x11ff) {
          iVar3 = FUN_0801f4dc(iVar3,iVar5,1);
          if (iVar3 != 0) {
            uVar4 = 0x116;
LAB_0807009e:
            FUN_08048adc(2,0x80702d0,0x8070290,_DAT_0807028c,uVar4,0x807026c);
          }
          goto LAB_080700ae;
        }
        if (param_2 < 0x17fe) {
          if (0x47e < uVar10) {
            iVar3 = FUN_0801f4dc(iVar3,iVar5,2);
            if (iVar3 != 0) {
              FUN_08048adc(2,0x80702d0,0x8070290,_DAT_0807028c,299,0x807026c);
            }
            iVar3 = *piVar1;
            goto LAB_08070036;
          }
          iVar3 = FUN_0801f4dc(iVar3,iVar5,2);
          if (iVar3 != 0) {
            uVar4 = 0x124;
            goto LAB_0806fff6;
          }
LAB_08070006:
          uVar7 = uVar7 / 0x5fd;
          iVar3 = *piVar1;
          *(undefined4 *)(iVar3 + 0x10) = 2;
          goto LAB_0806ff54;
        }
        iVar3 = FUN_0801f4dc(iVar3,iVar5,1);
        if (iVar3 != 0) {
          FUN_08048adc(2,0x80702d0,0x8070290,_DAT_0807028c,0x11d,0x807026c);
        }
LAB_0806ffce:
        iVar3 = *piVar1;
        uVar8 = 0x1c20;
        *(undefined4 *)(iVar3 + 0x10) = 1;
        goto LAB_0807003e;
      }
      uVar2 = (ushort)(uVar7 / 0xbfd);
    }
    else {
      if (0x47e < uVar10) {
        if (param_2 < 0x17f) {
          uVar8 = 0x706;
          goto LAB_0807004c;
        }
        if (uVar11 < 0x8ff) {
          iVar3 = FUN_0801f4dc(iVar3,iVar5,0);
          if (iVar3 != 0) {
            uVar4 = 0x13a;
            goto LAB_08070068;
          }
          goto LAB_08070078;
        }
        if (0x11fe < uVar8) {
          FUN_0801f4dc(iVar3,iVar5,1);
          goto LAB_0806ffce;
        }
        iVar3 = FUN_0801f4dc(iVar3,iVar5,1);
        if (iVar3 != 0) {
          uVar4 = 0x141;
          goto LAB_0807009e;
        }
LAB_080700ae:
        uVar7 = uVar7 / 0x17fd;
        iVar3 = *piVar1;
        *(undefined4 *)(iVar3 + 0x10) = 1;
LAB_0806ff54:
        uVar8 = uVar7 & 0xffff;
LAB_0807003e:
        *(undefined1 *)(iVar3 + 8) = 0;
        goto LAB_080701ea;
      }
      uVar2 = (ushort)(uVar7 / 0x5fd);
    }
LAB_0806ff86:
    uVar8 = (uint)uVar2;
  }
LAB_0807004c:
  *(undefined1 *)(iVar9 + 8) = 0;
LAB_080701ea:
  FUN_0806fb54(0);
  FUN_0807f696(_DAT_080702d8,2,uVar8);
  *(short *)(*_DAT_08070268 + 2) = (short)uVar8;
  return;
}


/* ===== FORCE @ 0x0806ff00 ===== */
/* function: FUN_0806ff00 size=232 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0806ff00(undefined4 param_1,undefined4 param_2,uint param_3,undefined4 param_4,uint param_5
                 ,uint param_6)

{
  int iVar1;
  uint unaff_r4;
  uint uVar2;
  undefined4 unaff_r6;
  uint unaff_r7;
  int *unaff_r8;
  undefined4 unaff_r10;
  uint unaff_r11;
  int in_r12;
  uint unaff_lr;
  undefined4 in_stack_00000014;
  
  if (param_6 < 0x11ff) {
    uVar2 = unaff_r4 / unaff_r7 & 0xffff;
  }
  else {
    if (param_3 <= unaff_r7) {
      if (unaff_lr < 0x8ff) {
        iVar1 = FUN_0801f4dc(param_1,param_2,0);
        if (iVar1 != 0) {
          FUN_08048adc(2,0x80702d0,0x8070290,_DAT_0807028c);
        }
        iVar1 = *unaff_r8;
        *(undefined4 *)(iVar1 + 0x10) = unaff_r6;
LAB_0806ff54:
        uVar2 = unaff_r4 / unaff_r11 & 0xffff;
      }
      else {
        if (param_5 < 0x47f) {
          iVar1 = FUN_0801f4dc(param_1,param_2,2);
          if (iVar1 != 0) {
            FUN_08048adc(2,0x80702d0,0x8070290,_DAT_0807028c);
          }
          unaff_r11 = 0x5fd;
          iVar1 = *unaff_r8;
          *(undefined4 *)(iVar1 + 0x10) = unaff_r10;
          goto LAB_0806ff54;
        }
        iVar1 = FUN_0801f4dc(param_1,param_2,2);
        if (iVar1 != 0) {
          FUN_08048adc(2,0x80702d0,0x8070290,_DAT_0807028c);
        }
        iVar1 = *unaff_r8;
        uVar2 = 0x706;
        *(undefined4 *)(iVar1 + 0x10) = unaff_r10;
      }
      *(char *)(iVar1 + 8) = (char)unaff_r6;
      goto LAB_080701ea;
    }
    uVar2 = 0x1c20;
  }
  *(char *)(in_r12 + 8) = (char)unaff_r6;
LAB_080701ea:
  FUN_0806fb54(in_stack_00000014);
  FUN_0807f696(_DAT_080702d8,2,uVar2);
  *(short *)(*_DAT_08070268 + 2) = (short)uVar2;
  return;
}

