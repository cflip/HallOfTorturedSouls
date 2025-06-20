#include "torturedsouls.h"
#include "floating.h"

double hts_viewZ;
double hts_viewSin;
double hts_viewCos;
double hts_viewHeight = 0.667; // view height?

byte *hts_screen;
undefined4 hts_screenStride;

undefined4 *hts_alloc1;
double *hts_xRotTable;
double *hts_yRotTable;
double *hts_alloc4MidWidthDiv;
textureinfo *hts_textures;

undefined4 hts_drawingColumn;
sector *hts_playerSector;
wall *hts_firstWallInColumn;
wall *hts_lastSpanThruWall;
int hts_playerXFixed;
int hts_playerYFixed;

void HTS_UpdateSectorRelHeights(void)

{
  int i;
  for (i = 0; i < hts_numSectors; i++) {
    FP_Set(&hts_sectors[i].floorHeight);
    FP_Sub(&hts_viewZ);
    FP_CopyTo(&hts_sectors[i].relFloorHeight);
    FP_Set(&hts_sectors[i].ceilHeight);
    FP_Sub(&hts_viewZ);
    FP_CopyTo(&hts_sectors[i].relCeilHeight);
  }
  return;
}

/* screenAngle = (screenX - 160) * (5/8)
   
   local_18  = local_10 * viewCos
   alloc2[screenX] = screenHeight * viewSin + local_18
   
   local_18 = local_10 * viewSin
   alloc3[screenX] = screenHeight * viewCos - local_18 */

void HTS_UpdateRotationData(void)

{
  int screenX;
  int iNext;
  double local_18;
  double screenAngle;
  double fiveDividedByEight;
  
  FP_SetInteger(8);
  FP_CopyTo(&fiveDividedByEight);
  FP_SetInteger(5);
  FP_Div(&fiveDividedByEight);
  FP_CopyTo(&fiveDividedByEight);
  screenX = 0;
  do {
    iNext = screenX + 1;
    FP_SetInteger(screenX + -160);
    FP_Mul(&fiveDividedByEight);
    FP_CopyTo(&screenAngle);
    FP_Mul(&hts_viewCos);
    FP_CopyTo(&local_18);
    FP_Set(&hts_scalarScreenHeight);
    FP_Mul(&hts_viewSin);
    FP_Add(&local_18);
    FP_CopyTo(hts_xRotTable + screenX);
    FP_Set(&screenAngle);
    FP_Mul(&hts_viewSin);
    FP_CopyTo(&local_18);
    FP_Set(&hts_scalarScreenHeight);
    FP_Mul(&hts_viewCos);
    FP_Sub(&local_18);
    FP_CopyTo(hts_yRotTable + screenX);
    screenX = iNext;
  } while (iNext < 320);
  hts_rotationNeedsUpdate = 0;
  return;
}

void HTS_AddSolidWallsToView(void)

{
  double *pdVar1;
  double *wallFirstCol;
  double *wallLastCol;
  int cmp;
  int iVar2;
  viewspan *pvVar3;
  viewspan *pvVar4;
  viewspan *curSpan;
  viewspan *local_1c;
  wall *pWall;
  int i;
  viewspan **ppaVar1;
  viewspan **ppaVar2;
  wall **ppw;
  viewspan *pwVar2;
  
  for (iVar2 = 1; iVar2 < 1000; iVar2++) {
    hts_viewspans[iVar2].next = &hts_viewspans[iVar2 + 1];
    hts_viewspans[iVar2].thruWalls = NULL;
  }
  pvVar4 = &hts_viewspans[1];
  hts_viewspans[999].next = NULL;
  FP_SetZero();
  FP_CopyTo(&hts_viewspans->firstColumn);
  FP_SetInteger(319);
  FP_CopyTo(&hts_viewspans->lastColumn);
  i = 0;
  hts_viewspans->solidWalls = NULL;
  hts_viewspans->next = NULL;
  hts_viewspans->thruWalls = NULL;
  pWall = hts_solidWalls;
  if (0 < hts_numSolidWalls) {
    do {
      FP_Set((double *)&fp_scalar1);
      FP_Negate();
      wallFirstCol = &pWall->firstColumn;
      cmp = FP_CompareTo(wallFirstCol);
      curSpan = hts_viewspans;
      if (cmp != 0) {
        while ((curSpan != NULL &&
               (cmp = FP2_Compare(wallFirstCol,&curSpan->lastColumn), -1 < cmp))) {
          curSpan = curSpan->next;
        }
        local_1c = NULL;
        if (curSpan != NULL) {
          wallLastCol = &pWall->lastColumn;
          pvVar3 = pvVar4;
          do {
            cmp = FP2_Compare(wallLastCol,&curSpan->firstColumn);
            pvVar4 = pvVar3;
            if (cmp < 1) break;
            ppw = &curSpan->solidWalls;
            cmp = HTS_WallCompute2(*ppw,pWall);
            if (cmp == 0) {
              cmp = FP2_Compare(wallFirstCol,&curSpan->firstColumn);
              if (cmp < 1) {
                cmp = FP2_Compare(wallLastCol,&curSpan->lastColumn);
                if (-1 < cmp) {
                  if (local_1c == NULL) {
                    *ppw = pWall;
                    if (hts_viewspans != curSpan) {
                      local_1c = curSpan;
                    }
                  }
                  else {
                    FP_Set(&curSpan->lastColumn);
                    FP_CopyTo(&local_1c->lastColumn);
                    local_1c->next = curSpan->next;
                    curSpan->next = pvVar3;
                    pvVar4 = curSpan;
                    curSpan = local_1c;
                  }
                  goto LAB_50305b6e;
                }
              }
              ppaVar1 = &pvVar3->next;
              ppaVar2 = &curSpan->next;
              pwVar2 = *ppaVar1;
              *ppaVar1 = *ppaVar2;
              *ppaVar2 = pvVar3;
              cmp = FP2_Compare(wallFirstCol,&curSpan->firstColumn);
              pvVar4 = pwVar2;
              if (cmp < 1) {
                if (local_1c == NULL) {
                  pvVar3->solidWalls = *ppw;
                  *ppw = pWall;
                  FP_Set(wallLastCol);
                  FP_CopyTo(&pvVar3->firstColumn);
                  FP_Set(&curSpan->lastColumn);
                  FP_CopyTo(&pvVar3->lastColumn);
                  FP_Set(wallLastCol);
                  FP_CopyTo(&curSpan->lastColumn);
                  curSpan = pvVar3;
                  if (hts_viewspans != pvVar3) {
                    local_1c = pvVar3;
                  }
                }
                else {
                  FP_Set(wallLastCol);
                  FP_CopyTo(&local_1c->lastColumn);
                  FP_CopyTo(&curSpan->firstColumn);
                  *ppaVar2 = *ppaVar1;
                  *ppaVar1 = pwVar2;
                  pvVar4 = pvVar3;
                }
              }
              else {
                pdVar1 = &curSpan->lastColumn;
                cmp = FP2_Compare(wallLastCol,pdVar1);
                if (cmp < 0) {
                  pvVar4 = pwVar2->next;
                  pwVar2->next = *ppaVar1;
                  *ppaVar1 = pwVar2;
                  pvVar3->solidWalls = pWall;
                  FP_Set(wallFirstCol);
                  FP_CopyTo(&pvVar3->firstColumn);
                  FP_Set(wallLastCol);
                  FP_CopyTo(&pvVar3->lastColumn);
                  pwVar2->solidWalls = *ppw;
                  FP_Set(wallLastCol);
                  FP_CopyTo(&pwVar2->firstColumn);
                  FP_Set(pdVar1);
                  FP_CopyTo(&pwVar2->lastColumn);
                  FP_Set(wallFirstCol);
                  FP_CopyTo(pdVar1);
                  curSpan = pwVar2;
                }
                else {
                  pvVar3->solidWalls = pWall;
                  FP_Set(pdVar1);
                  FP_CopyTo(&pvVar3->lastColumn);
                  FP_Set(wallFirstCol);
                  FP_CopyTo(pdVar1);
                  FP_CopyTo(&pvVar3->firstColumn);
                  curSpan = pvVar3;
                  local_1c = pvVar3;
                }
              }
            }
LAB_50305b6e:
            curSpan = curSpan->next;
            pvVar3 = pvVar4;
          } while (curSpan != NULL);
        }
      }
      pWall = pWall + 1;
      i = i + 1;
    } while (i < hts_numSolidWalls);
  }
  return;
}

void HTS_AddThruWallsToView(void)

{
  int cmp;
  wall *wall;
  viewspan *span;
  int i;
  double minusOne;
  
  i = 0;
  FP_SetInteger(-1);
  FP_CopyTo(&minusOne);
  wall = hts_thruWalls;
  if (0 < hts_numThruWalls) {
    do {
      cmp = FP2_Compare(&wall->firstColumn,&minusOne);
      if ((cmp != 0) && (span = hts_viewspans, hts_viewspans != NULL)) {
        do {
          cmp = FP2_Compare(&wall->firstColumn,&span->lastColumn);
          if (cmp < 1) break;
          span = span->next;
        } while (span != NULL);
        while (span != NULL) {
          cmp = FP2_Compare(&wall->lastColumn,&span->firstColumn);
          if (cmp < 1) break;
          cmp = HTS_WallCompute2(span->solidWalls,wall);
          if (cmp == 0) {
            wall->next = span->thruWalls;
            span->thruWalls = wall;
            break;
          }
          span = span->next;
        }
      }
      i = i + 1;
      wall = wall + 1;
    } while (i < hts_numThruWalls);
  }
  return;
}
void HTS_SetupWallLinks(viewspan *span)

{
  wall *wall1;
  wall *wall2;
  
  wall1 = span->thruWalls;
  if (wall1 != NULL) {
    wall2 = wall1->next;
    while (wall2 != NULL) {
      wall1 = wall1->next;
      wall2 = wall1->next;
    }
    wall1->next = hts_lastSpanThruWall;
    hts_lastSpanThruWall = span->thruWalls;
  }
  return;
}

void HTS_DrawWallColumn(int startRow,int endRow,wall *wall,int useUpperTex,double *param_5)

{
  int texnum;
  uint xIterator;
  int xStep;
  int texY;
  byte *draw;
  byte *end;
  uint texX;
  double local_18;
  double local_10;
  double startRowFloat;
  int texHeight;
  byte *texPixels;
  int texWidth;
  textureinfo *textures;
  uint xItMask;
  
  textures = hts_textures;
  if (useUpperTex == 0) {
    texnum = wall->lowerTex;
  }
  else {
    texnum = wall->upperTex;
  }
                    /* x iterator setup */
  xItMask = hts_textures[texnum].xIteratorMask;
  FP_SetInteger(startRow);
  FP_CopyTo(&startRowFloat);
  FP_Set(&hts_scalarScreenHeight);
  FP_Mul(&hts_viewHeight);
  FP_Sub(&startRowFloat);
  FP_Mul(param_5);
  FP_Div(&hts_scalarScreenWidth);
  FP_Add(&hts_viewZ);
  FP_CopyTo(&local_10);
  FP_Mul(&wall->fixedPointScale);
  xIterator = FP_AsInteger();
                    /* determine fixed point step */
  if (startRow < endRow) {
    FP_SetInteger(endRow - startRow);
    FP_CopyTo(&local_18);
    FP_SetInteger(endRow);
    FP_CopyTo(&startRowFloat);
    FP_Set(&hts_scalarScreenHeight);
    FP_Mul(&hts_viewHeight);
    FP_Sub(&startRowFloat);
    FP_Mul(param_5);
    FP_Div(&hts_scalarScreenWidth);
    FP_Add(&hts_viewZ);
    FP_Sub(&local_10);
    FP_Mul(&wall->fixedPointScale);
    FP_Div(&local_18);
    xStep = FP_AsInteger();
  }
  else {
    xStep = 0;
  }
                    /* determine y offset in texture */
  FP_Set(param_5);
  FP_Sub(&wall->relStartX);
  FP_Mul(&wall->field10_0x50);
  texY = FP_AsInteger();
  texHeight = textures[texnum].height;
  texWidth = textures[texnum].width;
  texPixels = textures[texnum].pixels;
  end = hts_screen + hts_drawingColumn + endRow * 320;
  for (draw = hts_screen + hts_drawingColumn + startRow * 320; draw <= end; draw = draw + 320) {
    texX = xIterator & (xItMask | 0xffff);
    xIterator = texX + xStep;
    *draw = texPixels[((int)texX >> 16) + (texY % texHeight) * texWidth];
  }
  return;
}

/* fills a column of pixels with a solid colour */

void HTS_FillColumn(int startRow,int endRow,undefined1 colour)

{
  byte *end;
  byte *draw;
  
  end = hts_screen + hts_drawingColumn + hts_screenStride * endRow;
  for (draw = hts_screen + hts_drawingColumn + hts_screenStride * startRow; draw <= end;
      draw = draw + 320) {
    *draw = colour;
  }
  return;
}

void HTS_DrawFloorColumn(int startRow,int endRow,int texnum,sector *sector)

{
  int *piVar1;
  int calcResult1;
  int calcResult2;
  byte *end;
  byte *draw;
  double local_8;
  undefined4 otherItShift;
  int texHeight;
  int texWidth;
  byte *texpixels;
  textureinfo *textures;
  uint xItMask;
  uint yItMask;
  
  textures = hts_textures;
  FP_Set(&sector->floorHeight);
  FP_Sub(&hts_viewZ);
  FP_Abs();
  FP_CopyTo(&local_8);
  FP_Mul(hts_xRotTable + hts_drawingColumn);
  calcResult1 = FP_AsInteger();
  FP_Set(&local_8);
  FP_Mul(hts_yRotTable + hts_drawingColumn);
  calcResult2 = FP_AsInteger();
  yItMask = textures[texnum].yIteratorMask;
  xItMask = textures[texnum].xIteratorMask;
  texHeight = textures[texnum].height;
  texWidth = textures[texnum].width;
  texpixels = textures[texnum].pixels;
  end = hts_screen + hts_drawingColumn + hts_screenStride * endRow;
  otherItShift = textures[texnum].field7_0x1c;
  piVar1 = hts_alloc1 + startRow;
  for (draw = hts_screen + hts_drawingColumn + hts_screenStride * startRow; draw <= end;
      draw = draw + 320) {
    *draw = texpixels[((int)((calcResult2 * *piVar1 + hts_playerYFixed) & (xItMask | 0xffff)) >> 16) +
                      ((int)((calcResult1 * *piVar1 + hts_playerXFixed) & yItMask) >>
                      ((16U - (char)otherItShift) & 31)) + (0 % texHeight) * texWidth];
    piVar1 = piVar1 + 1;
  }
  return;
}

void HTS_RenderColumn(viewspan *param_1)

{
  sector *wallSector;
  int iVar1;
  int wallSectorScreenPos;
  sector *playerSector;
  int endRow;
  int startRow;
  int playerSectorScreenPos;
  double depth;
  double local_30;
  double local_28;
  double local_20;
  double local_18;
  double bigNumber;
  double local_8;
  wall *wall;
  
  playerSector = hts_playerSector;
  startRow = 0;
  FP_SetInteger(9999999);
  FP_CopyTo(&bigNumber);
  endRow = 199;
  for (wall = hts_firstWallInColumn; wall != NULL; wall = wall->next) {
    wallSectorScreenPos = endRow;
    if (wall->shouldRender != 0) {
      wallSector = wall->sector1;
      if (wallSector == playerSector) {
        wallSector = wall->sector2;
      }
      FP_Set(hts_alloc4MidWidthDiv + hts_drawingColumn);
      FP_Mul(&wall->f1);
      FP_Sub(&wall->f2);
      FP_Negate();
      FP_CopyTo(&local_8);
      FP_Set(&wall->g);
      FP_Div(&local_8);
      iVar1 = FP_Sign();
      if (iVar1 != 0) {
        FP_CopyTo(&depth);
        FP_Set(&hts_scalarScreenWidth);
        FP_Mul(&playerSector->relCeilHeight);
        FP_Div(&depth);
        FP_CopyTo(&local_18);
        FP_Set(&hts_scalarScreenHeight);
        FP_Mul(&hts_viewHeight);
        FP_Sub(&local_18);
        playerSectorScreenPos = FP_AsInteger();
        if (startRow <= playerSectorScreenPos) {
          if (endRow < playerSectorScreenPos) {
            playerSectorScreenPos = endRow;
          }
          HTS_FillColumn(startRow,playerSectorScreenPos,playerSector->ceilCol);
          startRow = playerSectorScreenPos + 1;
        }
        FP_Set(&hts_scalarScreenWidth);
        FP_Mul(&playerSector->relFloorHeight);
        FP_Div(&depth);
        FP_CopyTo(&local_20);
        FP_Set(&hts_scalarScreenHeight);
        FP_Mul(&hts_viewHeight);
        FP_Sub(&local_20);
        playerSectorScreenPos = FP_AsInteger();
        if (playerSectorScreenPos <= endRow) {
          if (playerSectorScreenPos < startRow) {
            playerSectorScreenPos = startRow;
          }
          HTS_DrawFloorColumn(playerSectorScreenPos,endRow,playerSector->floorCol,playerSector);
          HTS_FillColumn(playerSectorScreenPos,endRow,playerSector->floorCol);
          endRow = playerSectorScreenPos + -1;
        }
        FP_Set(&hts_scalarScreenWidth);
        FP_Mul(&wallSector->relCeilHeight);
        FP_Div(&depth);
        FP_CopyTo(&local_28);
        FP_Set(&hts_scalarScreenHeight);
        FP_Mul(&hts_viewHeight);
        FP_Sub(&local_28);
        wallSectorScreenPos = FP_AsInteger();
        if (startRow <= wallSectorScreenPos) {
          if (endRow < wallSectorScreenPos) {
            wallSectorScreenPos = endRow;
          }
          HTS_DrawWallColumn(startRow,wallSectorScreenPos,wall,1,&depth);
          startRow = wallSectorScreenPos + 1;
        }
        FP_Set(&hts_scalarScreenWidth);
        FP_Mul(&wallSector->relFloorHeight);
        FP_Div(&depth);
        FP_CopyTo(&local_30);
        FP_Set(&hts_scalarScreenHeight);
        FP_Mul(&hts_viewHeight);
        FP_Sub(&local_30);
        iVar1 = FP_AsInteger();
        playerSector = wallSector;
        wallSectorScreenPos = endRow;
        if (iVar1 <= endRow) {
          if (iVar1 < startRow) {
            iVar1 = startRow;
          }
          wallSectorScreenPos = iVar1 + -1;
          HTS_DrawWallColumn(iVar1,endRow,wall,0,&depth);
        }
      }
    }
    endRow = wallSectorScreenPos;
  }
  wall = param_1->solidWalls;
  if (wall != NULL) {
    FP_Set(hts_alloc4MidWidthDiv + hts_drawingColumn);
    FP_Mul(&wall->f1);
    FP_Sub(&wall->f2);
    FP_Negate();
    FP_CopyTo(&local_8);
    FP_Set(&wall->g);
    FP_Div(&local_8);
    wallSectorScreenPos = FP_Sign();
    if (wallSectorScreenPos != 0) {
      FP_CopyTo(&depth);
      FP_Set(&hts_scalarScreenWidth);
      FP_Mul(&playerSector->relCeilHeight);
      FP_Div(&depth);
      FP_CopyTo(&local_8);
      FP_Set(&hts_scalarScreenHeight);
      FP_Mul(&hts_viewHeight);
      FP_Sub(&local_8);
      playerSectorScreenPos = FP_AsInteger();
      if (startRow <= playerSectorScreenPos) {
        if (endRow < playerSectorScreenPos) {
          playerSectorScreenPos = endRow;
        }
        HTS_FillColumn(startRow,playerSectorScreenPos,playerSector->ceilCol);
        startRow = playerSectorScreenPos + 1;
      }
      FP_Set(&hts_scalarScreenWidth);
      FP_Mul(&playerSector->relFloorHeight);
      FP_Div(&depth);
      FP_CopyTo(&local_8);
      FP_Set(&hts_scalarScreenHeight);
      FP_Mul(&hts_viewHeight);
      FP_Sub(&local_8);
      playerSectorScreenPos = FP_AsInteger();
      if (playerSectorScreenPos <= endRow) {
        if (playerSectorScreenPos < startRow) {
          playerSectorScreenPos = startRow;
        }
        HTS_FillColumn(playerSectorScreenPos,endRow,playerSector->floorCol);
        endRow = playerSectorScreenPos + -1;
      }
      if (endRow < startRow) {
        return;
      }
      HTS_DrawWallColumn(startRow,endRow,wall,1,&depth);
      return;
    }
  }
  FP_Set(&hts_scalarScreenWidth);
  FP_Mul(&fp_scalar0);
  FP_Div(&bigNumber);
  FP_CopyTo(&local_8);
  FP_Set(&hts_scalarScreenHeight);
  FP_Mul(&hts_viewHeight);
  FP_Sub(&local_8);
  wallSectorScreenPos = FP_AsInteger();
  if (startRow <= wallSectorScreenPos) {
    if (endRow < wallSectorScreenPos) {
      wallSectorScreenPos = endRow;
    }
    HTS_FillColumn(startRow,wallSectorScreenPos,playerSector->ceilCol);
    startRow = wallSectorScreenPos + 1;
  }
  if (wallSectorScreenPos <= endRow) {
    if (wallSectorScreenPos < startRow) {
      wallSectorScreenPos = startRow;
    }
    HTS_FillColumn(wallSectorScreenPos,endRow,playerSector->floorCol);
  }
  return;
}

void HTS_Render(void)

{
  int cmp;
  wall **ppwVar3;
  viewspan *viewSpan;
  int currentColumn;
  double column;
  wall *wallptr2;
  wall *temp_e23a751c16;
  wall *thruWall;
  wall **ppwVar1;
  wall **ppwVar2;
  wall *pwVar1;
  wall **wallptr3;
  
  FP_Set(&hts_playerRot);
  FP_Sin();
  FP_CopyTo(&hts_viewSin);
  FP_Set(&hts_playerRot);
  FP_Cos();
  FP_CopyTo(&hts_viewCos);
  FP_SetInteger(0x32);
  FP_Add(&hts_playerZ);
  FP_CopyTo(&hts_viewZ);
  FP_Set(&hts_playerX);
  cmp = FP_AsInteger();
  hts_playerXFixed = cmp << 16;
  FP_Set(&hts_playerY);
  cmp = FP_AsInteger();
  hts_playerYFixed = cmp << 16;
  hts_firstWallInColumn = NULL;
  hts_lastSpanThruWall = NULL;
  if (hts_rotationNeedsUpdate != 0) {
    HTS_UpdateRotationData();
  }
  HTS_UpdateSectorRelHeights();
  HTS_UpdateWallRelPositions(hts_solidWalls,hts_numSolidWalls);
  HTS_UpdateWallRelPositions(hts_thruWalls,hts_numThruWalls);
  HTS_AddSolidWallsToView();
  HTS_AddThruWallsToView();
  viewSpan = hts_viewspans;
  HTS_SetupWallLinks(hts_viewspans);
  currentColumn = 0;
  do {
    hts_drawingColumn = currentColumn;
    FP_SetInteger(currentColumn);
    FP_CopyTo(&column);
                    /* search for the viewspan that covers this column */
    cmp = FP2_Compare(&column,&viewSpan->lastColumn);
    while (0 < cmp) {
      viewSpan = viewSpan->next;
      HTS_SetupWallLinks(viewSpan);
      cmp = FP2_Compare(&column,&viewSpan->lastColumn);
    }
    wallptr3 = &hts_lastSpanThruWall;
    thruWall = hts_lastSpanThruWall;
    while (thruWall != NULL) {
      cmp = FP2_Compare(&(*wallptr3)->firstColumn,&column);
      if (cmp < 1) {
        wallptr2 = *wallptr3;
        ppwVar1 = &wallptr2->next;
        *wallptr3 = *ppwVar1;
        if (hts_firstWallInColumn == NULL) {
          hts_firstWallInColumn = wallptr2;
          *ppwVar1 = NULL;
        }
        else {
          ppwVar2 = &hts_firstWallInColumn;
          if (hts_firstWallInColumn != NULL) {
            do {
              ppwVar3 = ppwVar2;
              cmp = HTS_WallCompute2(*ppwVar3,wallptr2);
              pwVar1 = *ppwVar3;
              if (cmp == 0) {
                *ppwVar1 = pwVar1;
                *ppwVar3 = wallptr2;
                goto LAB_50306758;
              }
              ppwVar2 = &pwVar1->next;
            } while (pwVar1->next != NULL);
            *ppwVar1 = NULL;
            (*ppwVar3)->next = wallptr2;
          }
        }
      }
      else {
        wallptr3 = &(*wallptr3)->next;
      }
LAB_50306758:
      thruWall = *wallptr3;
    }
    wallptr3 = &hts_firstWallInColumn;
    pwVar1 = hts_firstWallInColumn;
    while (pwVar1 != NULL) {
      cmp = FP2_Compare(&(*wallptr3)->lastColumn,&column);
      if (cmp < 0) {
        *wallptr3 = (*wallptr3)->next;
      }
      else {
        temp_e23a751c16 = *wallptr3;
        cmp = HTS_WallCompute2(viewSpan->solidWalls,temp_e23a751c16);
        temp_e23a751c16->shouldRender = (uint)(cmp == 0);
        wallptr3 = &(*wallptr3)->next;
      }
      pwVar1 = *wallptr3;
    }
    HTS_RenderColumn(viewSpan);
    currentColumn = currentColumn + 1;
    if (319 < currentColumn) {
      return;
    }
  } while( true );
}
