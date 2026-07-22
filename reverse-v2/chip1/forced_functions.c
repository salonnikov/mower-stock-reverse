
/* ===== FORCE @ 0x0805f620 ===== */
/* function: FUN_0805f620 size=1114 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0805f620(undefined4 param_1,uint param_2)

{
  int *piVar1;
  ushort uVar2;
  int iVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  int iVar6;
  int iVar7;
  uint uVar8;
  uint uVar9;
  int iVar10;
  uint uVar11;
  uint uVar12;
  uint uVar13;
  
  piVar1 = _DAT_0805fa18;
  uVar11 = 0;
  iVar7 = *_DAT_0805fa18;
  uVar12 = param_2 * 0x1c20;
  iVar10 = *(int *)(iVar7 + 0x10);
  iVar6 = iVar7 + 0x30;
  iVar3 = iVar7 + 0x28;
  if (*(char *)(iVar7 + 8) != '\x01') {
    uVar8 = param_2 - 0x266;
    uVar13 = param_2 - 0x99;
    uVar9 = param_2 - 0x132;
    if (iVar10 == 1) {
      if (uVar8 < 0x1598) {
LAB_0805f8ac:
        uVar2 = (ushort)(uVar12 / 0x17fd);
      }
      else {
        if (0x17fd < param_2) {
LAB_0805f976:
          uVar11 = 0x1c20;
          goto LAB_0805f9cc;
        }
        if (0x265 < param_2) goto LAB_0805f9cc;
        if (0xacb < uVar9) {
          if (0x564 < uVar13) {
            iVar3 = FUN_0801f4dc(iVar3,iVar6,2);
            if (iVar3 != 0) {
              FUN_08048adc(2,0x805fab8,0x805fa78,_DAT_0805fa74,0x178,0x805fa58);
            }
            *(undefined4 *)(*piVar1 + 0x10) = 2;
            goto LAB_0805f9c8;
          }
          iVar3 = FUN_0801f4dc(iVar3,iVar6,2);
          if (iVar3 != 0) {
            FUN_08048adc(2,0x805fab8,0x805fa78,_DAT_0805fa74,0x171,0x805fa58);
          }
          *(undefined4 *)(*piVar1 + 0x10) = 2;
          goto LAB_0805f9ac;
        }
        iVar3 = FUN_0801f4dc(iVar3,iVar6,0);
        if (iVar3 != 0) {
          uVar5 = 0x16a;
LAB_0805fa00:
          FUN_08048adc(2,0x805fab8,0x805fa78,_DAT_0805fa74,uVar5,0x805fa58);
        }
LAB_0805fa10:
        *(undefined4 *)(*piVar1 + 0x10) = 0;
LAB_0805f93c:
        uVar2 = (ushort)(uVar12 / 0xbfd);
      }
    }
    else {
      if (iVar10 == 0) {
        if (uVar9 < 0xacc) goto LAB_0805f93c;
        if (0xbfd < param_2) {
          iVar3 = FUN_0801f4dc(iVar3,iVar6,1);
          if (iVar3 != 0) {
            FUN_08048adc(2,0x805fab8,0x805fa78,_DAT_0805fa74,0x185,0x805fa58);
          }
          *(undefined4 *)(*piVar1 + 0x10) = 1;
          if (0x1597 < uVar8) goto LAB_0805f976;
          goto LAB_0805f8ac;
        }
        iVar3 = FUN_0801f4dc(iVar3,iVar6,2);
        if (iVar3 != 0) {
          FUN_08048adc(2,0x805fab8,0x805fa78,_DAT_0805fa74,0x191,0x805fa58);
        }
        *(undefined4 *)(*piVar1 + 0x10) = 2;
        if (0x564 < uVar13) goto LAB_0805f9c8;
      }
      else {
        if (iVar10 != 2) goto LAB_0805f9cc;
        if (0x564 < uVar13) {
          if (param_2 < 0x99) {
LAB_0805f9c8:
            uVar11 = 0x2ce;
            goto LAB_0805f9cc;
          }
          if (uVar9 < 0xacc) {
            iVar3 = FUN_0801f4dc(iVar3,iVar6,0);
            if (iVar3 != 0) {
              uVar5 = 0x1a5;
              goto LAB_0805fa00;
            }
            goto LAB_0805fa10;
          }
          if (0x1597 < uVar8) {
            iVar3 = FUN_0801f4dc(iVar3,iVar6,1);
            if (iVar3 != 0) {
              FUN_08048adc(2,0x805fab8,0x805fa78,_DAT_0805fb2c,0x1b3,0x805fa58);
            }
            *(undefined4 *)(*piVar1 + 0x10) = 1;
            goto LAB_0805f976;
          }
          iVar3 = FUN_0801f4dc(iVar3,iVar6,1);
          if (iVar3 != 0) {
            FUN_08048adc(2,0x805fab8,0x805fa78,_DAT_0805fb2c,0x1ac,0x805fa58);
          }
          *(undefined4 *)(*piVar1 + 0x10) = 1;
          goto LAB_0805f8ac;
        }
      }
LAB_0805f9ac:
      uVar2 = (ushort)(uVar12 / 0x5fd);
    }
    uVar11 = (uint)uVar2;
    goto LAB_0805f9cc;
  }
  uVar11 = param_2 - 0x5ff;
  uVar8 = param_2 - 0x2ff;
  uVar9 = param_2 - 0x17f;
  if (iVar10 == 1) {
    if (uVar11 < 0x11ff) {
      uVar2 = (ushort)(uVar12 / 0x17fd);
      goto LAB_0805f760;
    }
    uVar13 = param_2;
    if (0x17fd < param_2) {
      *(undefined1 *)(iVar7 + 8) = 0;
      uVar5 = 0x805fa1c;
      uVar11 = 0x1c20;
      uVar4 = 0xfc;
LAB_0805f740:
      FUN_08048adc(2,0x805fab8,0x805fa78,_DAT_0805fa74,uVar4,uVar5,param_1,param_2,uVar11,param_1,
                   uVar13);
      goto LAB_0805f9cc;
    }
    if (uVar8 < 0x8ff) {
      iVar3 = FUN_0801f4dc(iVar3,iVar6,0);
      if (iVar3 != 0) {
        uVar5 = 0xff;
LAB_0805f830:
        FUN_08048adc(2,0x805fab8,0x805fa78,_DAT_0805fa74,uVar5,0x805fa58);
      }
LAB_0805f840:
      uVar12 = uVar12 / 0xbfd;
      iVar3 = *piVar1;
      *(undefined4 *)(iVar3 + 0x10) = 0;
    }
    else {
      if (0x47e < uVar9) {
        iVar3 = FUN_0801f4dc(iVar3,iVar6,2);
        if (iVar3 != 0) {
          FUN_08048adc(2,0x805fab8,0x805fa78,_DAT_0805fa74,0x10f,0x805fa58);
        }
        uVar5 = _DAT_0805fac0;
        iVar3 = *piVar1;
        uVar11 = 0x706;
        *(undefined4 *)(iVar3 + 0x10) = 2;
        *(undefined1 *)(iVar3 + 8) = 0;
        uVar4 = 0x114;
        goto LAB_0805f740;
      }
      iVar3 = FUN_0801f4dc(iVar3,iVar6,2);
      if (iVar3 != 0) {
        uVar5 = 0x107;
LAB_0805f7c0:
        FUN_08048adc(2,0x805fab8,0x805fa78,_DAT_0805fa74,uVar5,0x805fa58);
      }
LAB_0805f7d0:
      uVar12 = uVar12 / 0x5fd;
      iVar3 = *piVar1;
      *(undefined4 *)(iVar3 + 0x10) = 2;
    }
LAB_0805f708:
    uVar11 = uVar12 & 0xffff;
  }
  else {
    if (iVar10 != 0) {
      if (uVar9 < 0x47f) {
        uVar2 = (ushort)(uVar12 / 0x5fd);
LAB_0805f760:
        uVar11 = (uint)uVar2;
      }
      else {
        if (0x17e < param_2) {
          if (uVar8 < 0x8ff) {
            iVar3 = FUN_0801f4dc(iVar3,iVar6,0);
            if (iVar3 != 0) {
              uVar5 = 0x147;
              goto LAB_0805f830;
            }
            goto LAB_0805f840;
          }
          if (0x11fe < uVar11) {
            iVar3 = FUN_0801f4dc(iVar3,iVar6,1);
            if (iVar3 != 0) {
              uVar5 = 0x157;
              goto LAB_0805f88e;
            }
            goto LAB_0805f79c;
          }
          iVar3 = FUN_0801f4dc(iVar3,iVar6,1);
          if (iVar3 != 0) {
            uVar5 = 0x14f;
            goto LAB_0805f864;
          }
LAB_0805f874:
          uVar12 = uVar12 / 0x17fd;
          iVar3 = *piVar1;
          *(undefined4 *)(iVar3 + 0x10) = 1;
          goto LAB_0805f708;
        }
        uVar11 = 0x706;
      }
      *(undefined1 *)(iVar7 + 8) = 0;
      goto LAB_0805f9cc;
    }
    if (uVar8 < 0x8ff) {
      uVar2 = (ushort)(uVar12 / 0xbfd);
      goto LAB_0805f760;
    }
    if (uVar11 < 0x11ff) {
      iVar3 = FUN_0801f4dc(iVar3,iVar6,1);
      if (iVar3 != 0) {
        uVar5 = 0x11d;
LAB_0805f864:
        FUN_08048adc(2,0x805fab8,0x805fa78,_DAT_0805fa74,uVar5,0x805fa58);
      }
      goto LAB_0805f874;
    }
    if (param_2 < 0x17fe) {
      if (uVar9 < 0x47f) {
        iVar3 = FUN_0801f4dc(iVar3,iVar6,2);
        if (iVar3 != 0) {
          uVar5 = 0x12d;
          goto LAB_0805f7c0;
        }
        goto LAB_0805f7d0;
      }
      iVar3 = FUN_0801f4dc(iVar3,iVar6,2);
      if (iVar3 != 0) {
        FUN_08048adc(2,0x805fab8,0x805fa78,_DAT_0805fa74,0x135,0x805fa58);
      }
      iVar3 = *piVar1;
      uVar11 = 0x706;
      *(undefined4 *)(iVar3 + 0x10) = 2;
    }
    else {
      iVar3 = FUN_0801f4dc(iVar3,iVar6,1);
      if (iVar3 != 0) {
        uVar5 = 0x125;
LAB_0805f88e:
        FUN_08048adc(2,0x805fab8,0x805fa78,_DAT_0805fa74,uVar5,0x805fa58);
      }
LAB_0805f79c:
      iVar3 = *piVar1;
      uVar11 = 0x1c20;
      *(undefined4 *)(iVar3 + 0x10) = 1;
    }
  }
  *(undefined1 *)(iVar3 + 8) = 0;
LAB_0805f9cc:
  FUN_0805f300(param_1);
  FUN_0807f696(_DAT_0805fac4,3,uVar11);
  *(short *)(*_DAT_0805fa18 + 2) = (short)uVar11;
  return;
}


/* ===== FORCE @ 0x0805f2a0 ===== */
/* function: FUN_0805f2a0 size=26 */

/* WARNING: Removing unreachable block (ram,0x08084a3c) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_0805f2a0(undefined4 param_1,undefined4 param_2,undefined1 param_3)

{
  char cVar1;
  int *piVar2;
  int iVar3;
  uint uVar4;
  int unaff_r6;
  
  *(int *)(unaff_r6 + 0x54) = unaff_r6;
  *(undefined1 *)(unaff_r6 + 0x11) = param_3;
  iVar3 = *_DAT_0805f2bc;
  if (iVar3 == 0) {
    FUN_0801da86(0,s_Error__s__d_08084af8,s____src_platform_freertos_tasks_c_08084ad4,0x1101);
  }
  FUN_0808340c();
  cVar1 = *(char *)(iVar3 + 0x5c);
  *(undefined1 *)(iVar3 + 0x5c) = 2;
  *(uint *)(iVar3 + 0x58) = *(uint *)(iVar3 + 0x58) | 8;
  if (cVar1 == '\x01') {
    FUN_08083332(iVar3 + 4);
    piVar2 = DAT_08084b08;
    uVar4 = *(uint *)(iVar3 + 0x2c);
    DAT_08084b08[4] = 1 << (uVar4 & 0xff) | DAT_08084b08[4];
    FUN_080833f4(DAT_08084b0c + uVar4 * 0x14,iVar3 + 4);
    if (*(int *)(iVar3 + 0x28) != 0) {
      FUN_0801da86(0,s_Error__s__d_08084af8,s____src_platform_freertos_tasks_c_08084ad4,0x1139);
    }
    if (*(uint *)(*piVar2 + 0x2c) < *(uint *)(iVar3 + 0x2c)) {
      *DAT_08084b10 = 0x10000000;
      DataSynchronizationBarrier(0xf);
      InstructionSynchronizationBarrier(0xf);
    }
  }
  FUN_08083478();
  return 1;
}

