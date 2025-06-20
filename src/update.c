#include "torturedsouls.h"
#include "floating.h"

double hts_rotSpeed = 0.05;
double hts_scalarPi = 3.1415925;
double hts_scalarTwoPi = 6.283185;
undefined4 hts_rotationNeedsUpdate = 1;

undefined4 HTS_Update(void)

{
  sector *psVar1;
  wall *pwVar2;
  wall **ppwVar3;
  int iVar4;
  double *pdVar5;
  int iVar6;
  int iVar7;
  sector *psVar8;
  wall *thruWall;
  wall *solidWall;
  wall **ppwVar9;
  wall *local_48;
  int i;
  double local_40;
  double local_38;
  double moveAmt;
  double local_28;
  double playerRot;
  double viewCos;
  double viewSin;
  double velocity;
  
                    /* left or right turning? */
  if ((hts_keys & 6) != 0) {
    FP_Set(&hts_playerRot);
                    /* turning right? */
    if ((hts_keys & 2) != 0) {
      FP_Add(&hts_rotSpeed);
      iVar4 = FP_CompareTo(&hts_scalarTwoPi);
      if (0 < iVar4) {
        FP_Sub(&hts_scalarTwoPi);
      }
      hts_rotationNeedsUpdate = 1;
    }
                    /* turning left? */
    if ((hts_keys & 4) != 0) {
      FP_Sub(&hts_rotSpeed);
      iVar4 = FP_Sign();
      if (iVar4 < 0) {
        FP_Add(&hts_scalarTwoPi);
      }
      hts_rotationNeedsUpdate = 1;
    }
    FP_CopyTo(&hts_playerRot);
  }
                    /* forward or backward movement? */
  if ((hts_keys & 0x18) != 0) {
    FP_SetInteger(5);
    FP_CopyTo(&moveAmt);
                    /* shift key isn't down */
    pdVar5 = &hts_fastSpeed;
    if ((hts_keys & 0x20) == 0) {
      pdVar5 = &hts_normalSpeed;
    }
    FP_Set(pdVar5);
    FP_Div(&moveAmt);
    FP_CopyTo(&moveAmt);
    FP_Set(&hts_playerVel);
                    /* moving forward? */
    if ((hts_keys & 0x10) != 0) {
      FP_Add(&moveAmt);
    }
                    /* moving backward? */
    if ((hts_keys & 8) != 0) {
      FP_Sub(&moveAmt);
    }
    FP_CopyTo(&hts_playerVel);
  }
  FP_Set(&hts_playerVel);
  iVar4 = FP_Sign();
  iVar6 = FP_Sign();
  if (iVar6 != 0) {
    if (iVar4 < 0) {
      FP_Negate();
      FP_CopyTo(&hts_playerVel);
    }
    FP_Set((double *)&fp_scalar1);
    FP_Add(&fp_scalarHalf);
    iVar6 = FP_CompareTo(&hts_playerVel);
    if (iVar6 < 0) {
      FP_Add(&fp_value);
      FP_CopyTo(&moveAmt);
      FP_PowerOfTen(0xffffffff);
      FP_Add(&fp_value);
      FP_Sub(&hts_playerVel);
      FP_Negate();
      iVar6 = FP_CompareTo(&moveAmt);
      if (0 < iVar6) {
        FP_Set(&moveAmt);
      }
    }
    else {
      FP_PowerOfTen(0xffffffff);
      FP_CopyTo(&moveAmt);
      FP_Sub(&hts_playerVel);
      FP_Negate();
      iVar6 = FP_CompareTo(&moveAmt);
      if (iVar6 < 0) {
        FP_SetZero();
      }
    }
    if (iVar4 < 0) {
      FP_Negate();
    }
    FP_CopyTo(&hts_playerVel);
  }
  FP_Set(&hts_playerRot);
  FP_CopyTo(&playerRot);
  FP_Set(&hts_viewSin);
  FP_CopyTo(&viewSin);
  FP_Set(&hts_viewCos);
  FP_CopyTo(&viewCos);
  FP_Set(&hts_playerVel);
  FP_CopyTo(&velocity);
  iVar4 = FP_Sign();
  if (iVar4 < 0) {
    FP_Negate();
    FP_CopyTo(&hts_playerVel);
    FP_Set(&hts_viewSin);
    FP_Negate();
    FP_CopyTo(&hts_viewSin);
    FP_Set(&hts_viewCos);
    FP_Negate();
    FP_CopyTo(&hts_viewCos);
    FP_Set(&hts_playerRot);
    FP_Sub(&hts_scalarPi);
    iVar4 = FP_Sign();
    if (iVar4 < 0) {
      FP_Add(&hts_scalarTwoPi);
    }
    FP_CopyTo(&hts_playerRot);
  }
  FP_SetInteger(10);
  FP_Mul(&hts_playerVel);
  FP_CopyTo(&hts_playerVel);
  iVar4 = FP_Sign();
  if (iVar4 != 0) {
    HTS_UpdateWallRelPositions(hts_solidWalls,hts_numSolidWalls);
    iVar6 = 0;
    HTS_UpdateWallRelPositions(hts_thruWalls,hts_numThruWalls);
    psVar1 = hts_playerSector;
    local_48 = (wall *)0x0;
    FP_SetInteger(-1);
    FP_CopyTo(&local_28);
    iVar4 = 0;
    FP_SetInteger(0xa0);
    FP_CopyTo(&local_40);
    if (0 < hts_numSolidWalls) {
      do {
        solidWall = (wall *)((byte*)&hts_solidWalls->startX + iVar6);
        iVar7 = FP2_Compare(&solidWall->firstColumn,&local_28);
        if (((iVar7 != 0) && (iVar7 = FP2_Compare(&solidWall->firstColumn,&local_40), iVar7 < 1)) &&
           (iVar7 = FP2_Compare(&solidWall->lastColumn,&local_40), -1 < iVar7)) {
          HTS_WallCompute1(solidWall,160);
          FP_CopyTo(&local_38);
          FP_Set(&solidWall->relEndY);
          FP_Sub(&solidWall->relStartX);
          FP_Mul(&local_38);
          FP_Add(&solidWall->relStartX);
          iVar7 = FP_CompareTo(&hts_playerVel);
          if (iVar7 < 1) goto LAB_50306eeb;
        }
        iVar6 = iVar6 + sizeof(wall);
        iVar4 = iVar4 + 1;
      } while (iVar4 < hts_numSolidWalls);
    }
    i = 0;
    if (0 < hts_numThruWalls) {
      iVar4 = 0;
      do {
        thruWall = (wall *)((byte*)&hts_thruWalls->startX + iVar4);
        iVar6 = FP2_Compare(&thruWall->firstColumn,&local_28);
        if (((iVar6 != 0) && (iVar6 = FP2_Compare(&thruWall->firstColumn,&local_40), iVar6 < 1)) &&
           (iVar6 = FP2_Compare(&thruWall->lastColumn,&local_40), -1 < iVar6)) {
          HTS_WallCompute1(thruWall,0xa0);
          FP_CopyTo(&local_38);
          FP_Set(&thruWall->relEndY);
          FP_Sub(&thruWall->relStartX);
          FP_Mul(&local_38);
          FP_Add(&thruWall->relStartX);
          iVar6 = FP_CompareTo(&hts_playerVel);
          if (iVar6 < 1) {
            if (local_48 == (wall *)0x0) {
              thruWall->next = (wall *)0x0;
              local_48 = thruWall;
            }
            else {
              ppwVar3 = &local_48;
              if (local_48 != (wall *)0x0) {
                do {
                  ppwVar9 = ppwVar3;
                  iVar6 = HTS_WallCompute2(*ppwVar9,thruWall);
                  if (iVar6 == 0) {
                    thruWall->next = *ppwVar9;
                    *ppwVar9 = thruWall;
                    goto LAB_50306cf1;
                  }
                  ppwVar3 = &(*ppwVar9)->next;
                } while ((*ppwVar9)->next != (wall *)0x0);
                thruWall->next = (wall *)0x0;
                (*ppwVar9)->next = thruWall;
              }
            }
          }
        }
LAB_50306cf1:
        iVar4 = iVar4 + sizeof(wall);
        i = i + 1;
      } while (i < hts_numThruWalls);
    }
    pwVar2 = local_48;
    if (hts_playerSector != (sector *)0x0) {
      FP_SetInteger(0x14);
      pdVar5 = &hts_playerZ;
      if ((hts_keys & 0x80) == 0) {
        pdVar5 = &hts_playerSector->floorHeight;
      }
      FP_Add(pdVar5);
      FP_CopyTo(&moveAmt);
      pwVar2 = local_48;
    }
    for (; pwVar2 != (wall *)0x0; pwVar2 = pwVar2->next) {
      psVar8 = pwVar2->sector1;
      if (psVar8 == psVar1) {
        psVar8 = pwVar2->sector2;
      }
      iVar4 = FP2_Compare(&psVar8->floorHeight,&moveAmt);
      if (0 < iVar4) {
LAB_50306eeb:
        FP_Set(&viewSin);
        FP_CopyTo(&hts_viewSin);
        FP_Set(&viewCos);
        FP_CopyTo(&hts_viewCos);
        FP_SetZero();
        FP_CopyTo(&hts_playerVel);
        FP_Set(&playerRot);
        FP_CopyTo(&hts_playerRot);
        return 0;
      }
      psVar1 = psVar8;
    }
    FP_Set(&hts_playerRot);
    FP_Sin();
    FP_Mul(&hts_playerVel);
    FP_Add(&hts_playerX);
    FP_CopyTo(&hts_playerX);
    FP_Set(&hts_playerRot);
    FP_Cos();
    FP_Mul(&hts_playerVel);
    FP_Add(&hts_playerY);
    FP_CopyTo(&hts_playerY);
    psVar8 = (sector *)&hts_playerZ;
    if ((hts_keys & 0x80) == 0) {
      psVar8 = psVar1;
    }
    hts_playerSector = psVar1;
    FP_Set(&psVar8->floorHeight);
    FP_CopyTo(&hts_playerZ);
  }
  FP_Set(&viewSin);
  FP_CopyTo(&hts_viewSin);
  FP_Set(&viewCos);
  FP_CopyTo(&hts_viewCos);
  FP_Set(&velocity);
  FP_CopyTo(&hts_playerVel);
  FP_Set(&playerRot);
  FP_CopyTo(&hts_playerRot);
  return 1;
}
