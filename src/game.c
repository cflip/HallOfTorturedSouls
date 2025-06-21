#include "floating.h"
#include "torturedsouls.h"

double hts_scalarScreenWidth = 320.0;
double hts_scalarHalfScreenWidth = 160.0;
double hts_scalarScreenHeight = 200.0;
double DOUBLE_504315c8 = 0.032;
double DOUBLE_504315d0 = 0.02;
double DOUBLE_504315d8 = 133.3333333333333;

double hts_playerX;
double hts_playerY;
double hts_playerZ;
double hts_playerRot;
double hts_playerVel;

wall *hts_thruWalls;
wall *hts_solidWalls;
viewspan *hts_viewspans;
sector *hts_sectors;

undefined4 hts_numSolidWalls;
undefined4 hts_numThruWalls;
undefined4 hts_numSectors;

double hts_normalSpeed;
double hts_fastSpeed;
byte *hts_creditsTexPixels;
undefined4 DAT_50453cc8 = 0x438;
undefined4 DAT_50453cd0;
byte hts_input;
undefined4 hts_cheatCodeProgress;
undefined4 hts_cheatCodeTimeout;

/**
 * Initializes the game, handles key input, and runs the game loop.
 */
void HTS_RunGame(void)

{
    ushort key;
    BOOL didInitialize;
    int lastTicks;
    int currentTicks;
    double dStack_2c;
    SYSTEM_INFO systemInfo;
    uint savedFpuFlags;

    didInitialize = HTS_Initialize();
    savedFpuFlags = fp_fpuflags;
    if (didInitialize == 0)
    {
        return;
    }
    fp_fpuflags = fp_fpuflags | 4;
    FP_Set((double *)&fp_scalar1);
    FP_CopyTo(&hts_normalSpeed);
    FP_Set(&fp_scalar2);
    FP_Add((double *)&fp_scalar1);
    FP_CopyTo(&hts_fastSpeed);
    GetSystemInfo(&systemInfo);
    if (systemInfo.dwProcessorType == 486)
    {
        FP_Set(&hts_normalSpeed);
        FP_Mul(&fp_scalar2);
        FP_CopyTo(&hts_normalSpeed);
        FP_Set(&hts_fastSpeed);
    }
    else
    {
        if (systemInfo.dwProcessorType != 386)
            goto LAB_50304653;
        FP_Set(&hts_normalSpeed);
        FP_Mul(&fp_scalar2);
        FP_Mul(&fp_scalar2);
        FP_Mul(&fp_scalar2);
        FP_CopyTo(&hts_normalSpeed);
        FP_Set(&hts_fastSpeed);
        FP_Mul(&fp_scalar2);
        FP_Mul(&fp_scalar2);
    }
    FP_Mul(&fp_scalar2);
    FP_CopyTo(&hts_fastSpeed);
LAB_50304653:
    hts_textures[14].pixels = hts_creditsTexPixels + (0x800 - hts_textures[14].field2_0x8);
    lastTicks = 0;
    while (true)
    {
        key = GetAsyncKeyState(VK_ESCAPE);
        if ((key & 0xf000) == 0)
        {
            hts_input &= HTS_KEYOFF(HTS_INPUT_QUIT);
        }
        else
        {
            hts_input |= HTS_INPUT_QUIT;
        }
        key = GetAsyncKeyState(VK_RIGHT);
        if ((key & 0xf000) == 0)
        {
            hts_input &= HTS_KEYOFF(HTS_INPUT_TURNRIGHT);
        }
        else
        {
            hts_input |= HTS_INPUT_TURNRIGHT;
        }
        key = GetAsyncKeyState(VK_LEFT);
        if ((key & 0xf000) == 0)
        {
            hts_input &= HTS_KEYOFF(HTS_INPUT_TURNLEFT);
        }
        else
        {
            hts_input |= HTS_INPUT_TURNLEFT;
        }
        key = GetAsyncKeyState(VK_DOWN);
        if ((key & 0xf000) == 0)
        {
            hts_input &= HTS_KEYOFF(HTS_INPUT_BACKWARD);
        }
        else
        {
            hts_input |= HTS_INPUT_BACKWARD;
        }
        key = GetAsyncKeyState(VK_UP);
        if ((key & 0xf000) == 0)
        {
            hts_input &= HTS_KEYOFF(HTS_INPUT_FORWARD);
        }
        else
        {
            hts_input |= HTS_INPUT_FORWARD;
        }
        key = GetAsyncKeyState(VK_LCONTROL);
        if ((key & 0xf000) == 0)
        {
            hts_input &= HTS_KEYOFF(HTS_INPUT_FLY);
        }
        else
        {
            hts_input |= HTS_INPUT_FLY;
        }
        key = GetAsyncKeyState(VK_LSHIFT);
        if ((key & 0xf000) == 0)
        {
            hts_input &= HTS_KEYOFF(HTS_INPUT_RUN);
        }
        else
        {
            hts_input |= HTS_INPUT_RUN;
        }
        key = GetAsyncKeyState(VK_LMENU);
        if ((key & 0xf000) == 0)
        {
            hts_input &= HTS_KEYOFF(HTS_INPUT_ALT);
        }
        else
        {
            hts_input |= HTS_INPUT_ALT;
        }
        key = GetAsyncKeyState(L'D');
        if ((key & 0xf000) != 0)
        {
            FP_Set(&fp_scalarHalf);
            FP_Div(&fp_scalar10);
            FP_Add(&hts_viewHeight);
            FP_CopyTo(&hts_viewHeight);
            FP_Set((double *)&fp_scalar1);
            FP_Add(&fp_scalarHalf);
            currentTicks = FP_CompareTo(&hts_viewHeight);
            if (currentTicks < 0)
            {
                FP_CopyTo(&hts_viewHeight);
            }
        }
        key = GetAsyncKeyState(L'C');
        if ((key & 0xf000) != 0)
        {
            FP_Set(&fp_scalarHalf);
            FP_Div(&fp_scalar10);
            FP_CopyTo(&dStack_2c);
            FP_Set(&hts_viewHeight);
            FP_Sub(&dStack_2c);
            FP_CopyTo(&hts_viewHeight);
            FP_PowerOfTen(0xffffffff);
            FP_CopyTo(&dStack_2c);
            FP_Add(&dStack_2c);
            FP_Add(&dStack_2c);
            currentTicks = FP_CompareTo(&hts_viewHeight);
            if (0 < currentTicks)
            {
                FP_CopyTo(&hts_viewHeight);
            }
        }
        currentTicks = hts_cheatCodeTimeout;
        hts_cheatCodeTimeout = hts_cheatCodeTimeout + 1;
        if (DAT_50453cc8 < currentTicks)
        {
            hts_cheatCodeTimeout = 0;
            hts_cheatCodeProgress = 0;
        }
        key = GetAsyncKeyState(L'X');
        if ((key & 0xf000) != 0)
        {
            hts_cheatCodeTimeout = 0;
            hts_cheatCodeProgress = 1;
        }
        key = GetAsyncKeyState(L'L');
        if ((key & 0xf000) != 0)
        {
            if (hts_cheatCodeProgress == 1)
            {
                hts_cheatCodeProgress = 3;
            }
            else if (3 < hts_cheatCodeProgress)
            {
                hts_cheatCodeProgress = 0;
            }
        }
        key = GetAsyncKeyState(L'K');
        if ((key & 0xf000) != 0)
        {
            if (hts_cheatCodeProgress == 3)
            {
                hts_cheatCodeProgress = 7;
            }
            else if (7 < hts_cheatCodeProgress)
            {
                hts_cheatCodeProgress = 0;
            }
        }
        key = GetAsyncKeyState(L'F');
        if ((key & 0xf000) != 0)
        {
            if (hts_cheatCodeProgress == 7)
            {
                hts_cheatCodeProgress = 15;
            }
            else if (0xf < hts_cheatCodeProgress)
            {
                hts_cheatCodeProgress = 0;
            }
        }
        key = GetAsyncKeyState(L'A');
        if ((key & 0xf000) != 0)
        {
            if (hts_cheatCodeProgress == 15)
            {
                FP_Set(&hts_solidWalls[6].startX);
                FP_CopyTo(&hts_solidWalls[6].endX);
                FP_Set(&hts_solidWalls[6].startY);
                FP_CopyTo(&hts_solidWalls[6].endY);
            }
            else if (15 < hts_cheatCodeProgress)
            {
                hts_cheatCodeProgress = 0;
            }
        }
        key = GetAsyncKeyState(L'U');
        if ((key & 0xf000) != 0)
        {
            if (hts_cheatCodeProgress == 3)
            {
                hts_cheatCodeProgress = 0x1f;
            }
            else if (0x1f < hts_cheatCodeProgress)
            {
                hts_cheatCodeProgress = 0;
            }
        }
        key = GetAsyncKeyState(L'2');
        if ((key & 0xf000) != 0)
        {
            if (hts_cheatCodeProgress == 0x1f)
            {
                // XLU2 cheat code:
                // Allows the credits texture to scroll all the way,
                // revealing a new section of the credits!
                DAT_50453cc8 = 0x800 - hts_textures[14].field2_0x8;
            }
            else
            {
                hts_cheatCodeProgress = 0;
            }
        }
        currentTicks = lastTicks + 1;
        if (5 < lastTicks)
        {
            currentTicks = 0;
            HTS_ReadWindowMessages();
        }
        if ((hts_input & HTS_INPUT_QUIT) != 0)
            break;
        hts_textures[14].pixels = hts_textures[14].pixels + -1;
        lastTicks = DAT_50453cd0;
        DAT_50453cd0 = DAT_50453cd0 + 1;
        if (DAT_50453cc8 < lastTicks)
        {
            hts_textures[14].pixels = hts_creditsTexPixels + (0x800 - hts_textures[14].field2_0x8);
            DAT_50453cc8 = 0x438;
            DAT_50453cd0 = 0;
        }
        HTS_Update();
        HTS_Render();
        HTS_CopyFramebuffer();
#ifdef STANDALONE
        Sleep(20);
#endif
        lastTicks = currentTicks;
    }
    fp_fpuflags = savedFpuFlags;
    HTS_Cleanup();
    return;
}
