#include "torturedsouls.h"
#include "floating.h"

void HTS_WallCompute1(wall *wall,int param_2)

{
  int isZero;
  double tmpScalar;
  double scalar4Over128;
  double paramSmaller;
  double local_10;
  double local_8;
  
  FP_SetInteger(125);
  FP_CopyTo(&tmpScalar);
  FP_SetInteger(4);
  FP_Div(&tmpScalar);
  FP_CopyTo(&scalar4Over128);
  FP_SetInteger(10000);
  FP_CopyTo(&tmpScalar);
  FP_SetInteger(param_2 + -160);
  FP_Div(&tmpScalar);
  FP_CopyTo(&paramSmaller);
  FP_Set(&scalar4Over128);
  FP_Mul(&wall->relStartY);
  FP_CopyTo(&tmpScalar);
  FP_Set(&paramSmaller);
  FP_Mul(&wall->relStartX);
  FP_Sub(&tmpScalar);
  FP_CopyTo(&local_8);
  FP_Set(&wall->relEndY);
  FP_Sub(&wall->relStartX);
  FP_Mul(&paramSmaller);
  FP_CopyTo(&tmpScalar);
  FP_Set(&wall->relEndX);
  FP_Sub(&wall->relStartY);
  FP_Mul(&scalar4Over128);
  FP_Sub(&tmpScalar);
  isZero = FP_Sign();
  if (isZero != 0) {
    FP_CopyTo(&local_10);
    FP_Set(&local_8);
    FP_Div(&local_10);
  }
  return;
}

int FP2_Compare(double *param_1,double *param_2)

{
#if 0
  int result;
  uint ahigh;
  uint bhigh;
  
                    /* edi = param2 high, esi = param1 high */
  bhigh = (uint *)param_2[1];
  ahigh = (uint *)param_1[1];
  if ((int)ahigh < (int)bhigh) {
                    /* param_1 < param_2 */
    result = -1;
  }
  else if ((int)bhigh < (int)ahigh) {
                    /* param_1 > param_2 */
    result = 1;
  }
  else {
    if (*(uint *)param_2 == *(uint *)param_1) {
      return 0;
    }
    result = (-(uint)(*(uint *)param_1 < *(uint *)param_2) & 0xfffffffe) + 1;
  }
  if (-1 < (int)(ahigh & bhigh)) {
    return result;
  }
  return -result;
#else
	if (*param_1 == *param_2) 
		return 0;
	if (*param_1 < *param_2)
		return -1;
	return 1;
#endif
}

undefined4 HTS_WallCompute2(wall *param_1,wall *param_2)

{
  int iVar1;
  double *screen1;
  double *screen2;
  double local_18;
  double local_10;
  double local_8;
  
  if (param_1 == (wall *)0x0) {
    return 0;
  }
  screen1 = &param_1->firstColumn;
  screen2 = &param_2->lastColumn;
  iVar1 = FP2_Compare(screen1,screen2);
  if ((iVar1 < 0) && (iVar1 = FP2_Compare(&param_1->lastColumn,&param_2->firstColumn), 0 < iVar1)) {
    iVar1 = FP2_Compare(screen1,&param_2->firstColumn);
    if (iVar1 < 0) {
      screen1 = &param_2->firstColumn;
    }
    FP_Set(screen1);
    iVar1 = FP2_Compare(&param_1->lastColumn,screen2);
    if (iVar1 < 1) {
      screen2 = &param_1->lastColumn;
    }
    FP_Add(screen2);
    FP_Div(&fp_scalar2);
    FP_Sub(&hts_scalarHalfScreenWidth);
                    /* 0.032 */
    FP_Mul(&DOUBLE_504315c8);
    FP_Div(&hts_scalarScreenWidth);
                    /* local_18 = ((screen1 + screen2) / 2 - 160) * 0.032 / 320; */
    FP_CopyTo(&local_18);
    FP_Set(&local_18);
    FP_Mul(&param_2->f1);
    FP_Sub(&param_2->f2);
    FP_Negate();
                    /* local_8 = -(local_18 * wall2->(relEndY - relStartX) - wall2->(relEndX -
                       relStartY)) */
    FP_CopyTo(&local_8);
    FP_Set(&param_2->g);
    FP_Div(&local_8);
                    /* local_10 = wall2->g / local_8 */
    FP_CopyTo(&local_10);
    FP_Set(&local_18);
    FP_Mul(&param_1->f1);
    FP_Sub(&param_1->f2);
    FP_Negate();
    FP_CopyTo(&local_8);
    FP_Set(&param_1->g);
    FP_Div(&local_8);
    iVar1 = FP_CompareTo(&local_10);
    if (0 < iVar1) {
      return 0;
    }
  }
  return 1;
}

void HTS_UpdateWallRelPositions(wall *walls,int numWalls)

{
  int iVar1;
  int remaining;
  double two;
  double local_28;
  double local_20;
  double local_18;
  double local_10;
  double local_8;
  double *firstCol;
  double *lastCol;
  double *relStartX;
  
  if (0 < numWalls) {
    remaining = numWalls;
    do {
      FP_Set(&walls->startY);
      FP_Sub(&hts_playerY);
      FP_Mul(&hts_viewSin);
      FP_CopyTo(&two);
      FP_Set(&walls->startX);
      FP_Sub(&hts_playerX);
      FP_Mul(&hts_viewCos);
      FP_Sub(&two);
      FP_CopyTo(&walls->relStartY);
      FP_Set(&walls->startX);
      FP_Sub(&hts_playerX);
      FP_Mul(&hts_viewSin);
      FP_CopyTo(&two);
      FP_Set(&walls->startY);
      relStartX = &walls->relStartX;
      FP_Sub(&hts_playerY);
      FP_Mul(&hts_viewCos);
      FP_Add(&two);
      FP_CopyTo(relStartX);
      FP_Set(&walls->endY);
      FP_Sub(&hts_playerY);
      FP_Mul(&hts_viewSin);
      FP_CopyTo(&two);
      FP_Set(&walls->endX);
      FP_Sub(&hts_playerX);
      FP_Mul(&hts_viewCos);
      FP_Sub(&two);
      FP_CopyTo(&walls->relEndX);
      FP_Set(&walls->endX);
      FP_Sub(&hts_playerX);
      FP_Mul(&hts_viewSin);
      FP_CopyTo(&two);
      FP_Set(&walls->endY);
      FP_Sub(&hts_playerY);
      FP_Mul(&hts_viewCos);
      FP_Add(&two);
      FP_CopyTo(&walls->relEndY);
      iVar1 = FP2_Compare(relStartX,&walls->relEndY);
      if (iVar1 == 0) {
        FP_PowerOfTen(0xfffffffd);
        FP_Add(relStartX);
        FP_CopyTo(&walls->relEndY);
      }
      lastCol = &walls->f2;
      FP_SetInteger(1000);
      FP_CopyTo(&local_28);
      FP_SetInteger(32);
      FP_Div(&local_28);
      FP_CopyTo(&local_28);
      FP_Set(&walls->relEndY);
      FP_Sub(relStartX);
      FP_CopyTo(&walls->f1);
      FP_Mul(&walls->relStartY);
      FP_CopyTo(&two);
      FP_Set(&walls->relEndX);
      FP_Sub(&walls->relStartY);
      FP_CopyTo(lastCol);
      FP_Mul(relStartX);
      FP_Sub(&two);
      FP_Mul(&local_28);
      FP_CopyTo(&walls->g);
      FP_Set(lastCol);
      FP_Mul(&local_28);
      FP_CopyTo(lastCol);
      FP_SetInteger(2);
      FP_CopyTo(&two);
      FP_Set(relStartX);
      FP_Mul(&local_28);
      FP_Div(&two);
      FP_CopyTo(&two);
      FP_Set(&local_28);
      FP_Mul(&walls->relStartY);
      iVar1 = FP_CompareTo(&two);
      if (iVar1 < 1) {
        FP_Negate();
        iVar1 = FP_CompareTo(&two);
        if (0 < iVar1) goto LAB_50305311;
        FP_Set(&hts_scalarScreenWidth);
        FP_Mul(&walls->relStartY);
        FP_Div(relStartX);
        FP_Add(&hts_scalarHalfScreenWidth);
LAB_5030513a:
        firstCol = &walls->firstColumn;
        FP_CopyTo(firstCol);
        iVar1 = FP2_Compare(&hts_scalarScreenWidth,firstCol);
        if (iVar1 < 0) {
          FP_SetInteger(319);
          FP_CopyTo(firstCol);
        }
        FP_SetInteger(2);
        FP_CopyTo(&two);
        FP_Set(&walls->relEndY);
        FP_Mul(&local_28);
        FP_Div(&two);
        FP_CopyTo(&two);
        FP_Set(&local_28);
        FP_Mul(&walls->relEndX);
        iVar1 = FP_CompareTo(&two);
        if (iVar1 < 1) {
          FP_Negate();
          iVar1 = FP_CompareTo(&two);
          if (0 < iVar1) goto LAB_5030541c;
          FP_Set(&hts_scalarScreenWidth);
          FP_Mul(&walls->relEndX);
          FP_Div(&walls->relEndY);
          FP_Add(&hts_scalarHalfScreenWidth);
LAB_50305207:
          lastCol = &walls->lastColumn;
          FP_CopyTo(lastCol);
          iVar1 = FP2_Compare(&hts_scalarScreenWidth,lastCol);
          if (iVar1 < 0) {
            FP_SetInteger(319);
            FP_CopyTo(lastCol);
          }
          FP_SetInteger(319);
          iVar1 = FP_CompareTo(firstCol);
          if ((iVar1 < 0) && (iVar1 = FP_CompareTo(lastCol), iVar1 == 0)) {
            FP_SetZero();
            FP_CopyTo(lastCol);
          }
          FP_SetInteger(319);
          iVar1 = FP_CompareTo(lastCol);
          if ((iVar1 < 0) && (iVar1 = FP_CompareTo(firstCol), iVar1 == 0)) {
            FP_SetZero();
            FP_CopyTo(firstCol);
          }
          iVar1 = FP2_Compare(firstCol,lastCol);
          if (0 < iVar1) {
            FP_Set(firstCol);
            XL_CopyArray((undefined1 *)lastCol,(undefined1 *)firstCol,8);
            FP_CopyTo(lastCol);
          }
          FP_Set(relStartX);
          FP_Sub(&walls->relEndY);
          FP_Negate();
          FP_CopyTo(&local_28);
          FP_Mul(&fp_value);
          FP_CopyTo(&two);
          FP_Set(&walls->relStartY);
          FP_Sub(&walls->relEndX);
          FP_Mul(&fp_value);
          FP_Add(&two);
          FP_Sqrt();
          FP_Div(&local_28);
          firstCol = &walls->field10_0x50;
        }
        else {
LAB_5030541c:
          HTS_WallCompute1(walls,319);
          iVar1 = FP_Sign();
          if ((-1 < iVar1) && (iVar1 = FP_CompareTo((double *)&fp_scalar1), iVar1 < 1)) {
            FP_Set(hts_alloc4MidWidthDiv + 0x13f);
            FP_Mul(&walls->f1);
            FP_Sub(lastCol);
            FP_Negate();
            FP_CopyTo(&local_18);
            FP_Set(&walls->g);
            FP_Div(&local_18);
            iVar1 = FP_Sign();
            if (0 < iVar1) {
              FP_SetInteger(0x13f);
              goto LAB_50305207;
            }
          }
          HTS_WallCompute1(walls,0);
          iVar1 = FP_Sign();
          if ((0 < iVar1) && (iVar1 = FP_CompareTo((double *)&fp_scalar1), iVar1 < 1)) {
            FP_Set(hts_alloc4MidWidthDiv);
            FP_Mul(&walls->f1);
            FP_Sub(lastCol);
            FP_Negate();
            FP_CopyTo(&local_20);
            FP_Set(&walls->g);
            FP_Div(&local_20);
            iVar1 = FP_Sign();
            if (0 < iVar1) {
              FP_SetZero();
              goto LAB_50305207;
            }
          }
          FP_SetInteger(-1);
        }
      }
      else {
LAB_50305311:
        HTS_WallCompute1(walls,0);
        iVar1 = FP_Sign();
        if ((-1 < iVar1) && (iVar1 = FP_CompareTo((double *)&fp_scalar1), iVar1 < 1)) {
          FP_Set(hts_alloc4MidWidthDiv);
          FP_Mul(&walls->f1);
          FP_Sub(lastCol);
          FP_Negate();
          FP_CopyTo(&local_8);
          FP_Set(&walls->g);
          FP_Div(&local_8);
          iVar1 = FP_Sign();
          if (0 < iVar1) {
            FP_SetZero();
            goto LAB_5030513a;
          }
        }
        HTS_WallCompute1(walls,319);
        iVar1 = FP_Sign();
        if ((-1 < iVar1) && (iVar1 = FP_CompareTo((double *)&fp_scalar1), iVar1 < 1)) {
          FP_Set(hts_alloc4MidWidthDiv + 319);
          FP_Mul(&walls->f1);
          FP_Sub(lastCol);
          FP_Negate();
          FP_CopyTo(&local_10);
          FP_Set(&walls->g);
          FP_Div(&local_10);
          iVar1 = FP_Sign();
          if (0 < iVar1) {
            FP_SetInteger(0x13f);
            goto LAB_5030513a;
          }
        }
        FP_SetInteger(-1);
        firstCol = &walls->firstColumn;
      }
      walls = walls + 1;
      FP_CopyTo(firstCol);
      remaining = remaining + -1;
    } while (remaining != 0);
  }
  return;
}
