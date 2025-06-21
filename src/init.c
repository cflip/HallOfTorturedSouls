#include "floating.h"
#include "torturedsouls.h"

bool HTS_Initialize(void)

{
    int iVar1;

    HTS_SetupWindow();
    iVar1 = HTS_InitPt2();
    if (iVar1 == 0)
    {
        return false;
    }
    iVar1 = HTS_SetupFramebuffer();
    if (iVar1 == 0)
    {
        return false;
    }
    iVar1 = HTS_InitMapStructures();
    if (iVar1 == 0)
    {
        return false;
    }
    iVar1 = HTS_ResetPlayerPos();
    if (iVar1 == 0)
    {
        return false;
    }
    iVar1 = HTS_LoadMapData();
    return iVar1 != 0;
}

int HTS_InitPt2(void)

{
    int counter;
    int i;
    undefined4 *memptr;
    undefined4 *alcptr;

    hts_alloc1 = (undefined4 *)GlobalAlloc(0, 800);
    if (hts_alloc1 == NULL)
    {
        return 0;
    }
    hts_xRotTable = (double *)GlobalAlloc(0, 0xa00);
    if (hts_xRotTable == NULL)
    {
        return 0;
    }
    hts_yRotTable = (double *)GlobalAlloc(0, 0xa00);
    if (hts_yRotTable == NULL)
    {
        return 0;
    }
    hts_alloc4MidWidthDiv = (double *)GlobalAlloc(0, 0xa00);
    if (hts_alloc4MidWidthDiv == NULL)
    {
        return 0;
    }
    hts_textures = (textureinfo *)GlobalAlloc(0, sizeof(textureinfo) * 15);
    if (hts_textures == NULL)
    {
        return 0;
    }
    counter = 0;
    do
    {
        memptr = (undefined4 *)((undefined1 *)hts_textureData + counter);
        alcptr = (undefined4 *)((undefined1 *)hts_textures + counter);
        for (i = sizeof(textureinfo) / 4; i != 0; i = i + -1)
        {
            *alcptr = *memptr;
            memptr++;
            alcptr++;
        }
        counter += sizeof(textureinfo);
    } while (counter < sizeof(textureinfo) * 15);
    return counter;
}

undefined4 HTS_InitMapStructures(void)

{
    int iVar1;
    undefined4 uVar2;
    int a4iter;
    double local_20;
    double local_18;
    double local_10;
    double a4div;

    HTS_GetScreenAndStride(&hts_screen, &hts_screenStride);
    hts_thruWalls = (wall *)GlobalAlloc(0, sizeof(wall) * 500);
    if (hts_thruWalls == NULL)
    {
        return 0;
    }
    hts_solidWalls = (wall *)GlobalAlloc(0, sizeof(wall) * 500);
    if (hts_solidWalls == NULL)
    {
        return 0;
    }
    hts_viewspans = (viewspan *)GlobalAlloc(0, sizeof(viewspan) * 1000);
    if (hts_viewspans == NULL)
    {
        return 0;
    }
    hts_sectors = (sector *)GlobalAlloc(0, sizeof(sector) * 100);
    if (hts_sectors == NULL)
    {
        return 0;
    }
    FP_Add((double *)&fp_scalar1);
    FP_CopyTo(&local_20);
    FP_SetInteger(0x10000);
    FP_CopyTo(&local_18);
    a4iter = 0;
    do
    {
        FP_SetInteger(a4iter);
        FP_Sub(&DOUBLE_504315d8);
        FP_Negate();
        FP_Mul(&DOUBLE_504315d0);
        FP_CopyTo(&local_10);
        FP_Set(&local_20);
        FP_Mul(&hts_scalarScreenHeight);
        FP_Mul(&DOUBLE_504315c8);
        FP_Div(&local_10);
        iVar1 = FP_CompareTo(&hts_scalarScreenHeight);
        if (0 < iVar1)
        {
            FP_Set(&hts_scalarScreenHeight);
        }
        iVar1 = a4iter + 1;
        FP_Mul(&local_18);
        FP_Div(&hts_scalarScreenHeight);
        uVar2 = FP_AsInteger();
        hts_alloc1[a4iter] = uVar2;
        a4iter = iVar1;
    } while (iVar1 < 0x85);
    hts_alloc1[0x85] = 0x10000;
    FP_SetZero();
    FP_Sub(&fp_scalar1);
    FP_CopyTo(&local_20);
    a4iter = 199;
    do
    {
        FP_SetInteger(a4iter);
        FP_Sub(&DOUBLE_504315d8);
        FP_Negate();
        FP_Mul(&DOUBLE_504315d0);
        FP_CopyTo(&local_10);
        FP_Set(&local_20);
        FP_Mul(&hts_scalarScreenHeight);
        FP_Mul(&DOUBLE_504315c8);
        FP_Div(&local_10);
        iVar1 = FP_CompareTo(&hts_scalarScreenHeight);
        if (0 < iVar1)
        {
            FP_Set(&hts_scalarScreenHeight);
        }
        iVar1 = a4iter + -1;
        FP_Mul(&local_18);
        FP_Div(&hts_scalarScreenHeight);
        uVar2 = FP_AsInteger();
        hts_alloc1[a4iter] = uVar2;
        a4iter = iVar1;
    } while (133 < iVar1);
    FP_PowerOfTen(4);
    FP_CopyTo(&a4div);
    a4iter = 0;
    do
    {
        iVar1 = a4iter + 1;
        FP_SetInteger(a4iter + -160);
        FP_Div(&a4div);
        FP_CopyTo(hts_alloc4MidWidthDiv + a4iter);
        a4iter = iVar1;
    } while (iVar1 < 320);
    return 1;
}

undefined4 HTS_ResetPlayerPos(void)

{
    FP_SetInteger(10);
    FP_CopyTo(&hts_playerX);
    FP_CopyTo(&hts_playerZ);
    FP_CopyTo(&hts_playerY);
    FP_SetZero();
    FP_CopyTo(&hts_playerRot);
    hts_playerSector = NULL;
    return 1;
}

/*
 * Does nothing, maybe originally contained some commented out code.
 */
void HTS_DoNothing(void)

{
    return;
}

void HTS_FreeAll(void)

{
    if (hts_alloc1 != NULL)
    {
        GlobalFree(hts_alloc1);
    }
    if (hts_xRotTable != NULL)
    {
        GlobalFree(hts_xRotTable);
    }
    if (hts_yRotTable != NULL)
    {
        GlobalFree(hts_yRotTable);
    }
    if (hts_alloc4MidWidthDiv != NULL)
    {
        GlobalFree(hts_alloc4MidWidthDiv);
    }
    if (hts_thruWalls != NULL)
    {
        GlobalFree(hts_thruWalls);
    }
    if (hts_solidWalls != NULL)
    {
        GlobalFree(hts_solidWalls);
    }
    if (hts_viewspans != NULL)
    {
        GlobalFree(hts_viewspans);
    }
    if (hts_textures != NULL)
    {
        GlobalFree(hts_textures);
    }
    if (hts_bitmapinfo != (LPBITMAPINFO)0x0)
    {
        GlobalFree(hts_bitmapinfo);
    }
    if (hts_copiedBitmap != (LPBITMAPINFO)0x0)
    {
        GlobalFree(hts_copiedBitmap);
    }
    if (hts_sectors != NULL)
    {
        GlobalFree(hts_sectors);
    }
    return;
}

void HTS_Cleanup(void)

{
    HTS_DoNothing();
    HTS_FreeAll();
    if (hts_creditsTexPixels != NULL)
    {
        GlobalFree(hts_creditsTexPixels);
    }
    if (hts_hWnd != (HWND)0x0)
    {
        ShowWindow(hts_hWnd, 0);
        DestroyWindow(hts_hWnd);
    }
    UnregisterClassA("zip", xl_hInstance);
    return;
}
