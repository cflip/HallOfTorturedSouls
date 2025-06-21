/**
 * window.c
 *
 * Window and graphics setup using Win32 and GDI. Also loads the credits data and makes the texture.
 */

#include "torturedsouls.h"

HDC hts_windowDC;
HPALETTE hts_palette;

HWND hts_hWnd;
void *hts_dibSectionBits;
HDC hts_dc;
HGDIOBJ hts_brush;
LPBITMAPINFO hts_bitmapinfo;

LPBITMAPINFO hts_creditsBitmapInfo;
HBITMAP hts_creditsBitmap;
HGDIOBJ hts_creditsSelectedBitmap;
HDC hts_hCreditsDC;

undefined4 DAT_5044fed0 = 0;
undefined4 DAT_5044d988 = 1;

/**
 * Parses the encoded credits data, draws the decoded text into a bitmap, and copies the bitmap data to the 15th
 * texture.
 */
bool HTS_MakeCreditsBitmap(void)
{
    HGDIOBJ gdiobj;
    byte *pixels;
    int textLen;
    LPRECT lprc;
    RECT rc;
    UINT drawTextFormat;
    UINT format;
    byte *encodedText;
    char text[256];
    byte encodedByte;
    uint i;
    int xp;
    int yp;
    int srcn;
    int srcp;

    rc.top = 0;
    rc.left = 0;
    rc.right = HTS_CREDITS_WIDTH;
    rc.bottom = 14;

    drawTextFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE;

    gdiobj = GetStockObject(SYSTEM_FIXED_FONT);
    SelectObject(hts_hCreditsDC, gdiobj);
    SetBkMode(hts_hCreditsDC, TRANSPARENT);
    SetTextColor(hts_hCreditsDC, 0x0000ff);

    encodedText = hts_creditsData; // Originally hts_textures[4].pixels + 0x100;
    encodedByte = *encodedText;
    while (encodedByte != 0xff)
    {
        encodedByte = *encodedText;
        if (encodedByte == 0xfe)
        {
        nextLine:
            rc.bottom = rc.bottom + 14;
            rc.top = rc.top + 14;
        }
        else if (encodedByte == 0xfd)
        {
            SetTextColor(hts_hCreditsDC, 0x00ffff);
        }
        else if (encodedByte == 0xfc)
        {
            SetTextColor(hts_hCreditsDC, 0x00ff00);
        }
        else if (encodedByte == 0xfb)
        {
            SetTextColor(hts_hCreditsDC, 0x0000ff);
        }
        else if (encodedByte == 0xfa)
        {
            drawTextFormat = DT_VCENTER | DT_SINGLELINE;
        }
        else
        {
            if (encodedByte != 0xef)
            {
                textLen = 0;
                for (i = (uint)encodedByte; i != 0; i = i - 1)
                {
                    text[textLen] = encodedText[1] ^ 0x4a;
                    textLen++;
                    encodedText++;
                }
                lprc = &rc;
                text[textLen] = '\0';
                format = drawTextFormat;
                textLen = XL_StringLength(text);
                DrawTextA(hts_hCreditsDC, text, textLen, lprc, format);
                goto nextLine;
            }
            drawTextFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
        }
        encodedText++;
        encodedByte = *encodedText;
    }

    pixels = GlobalAlloc(0, HTS_CREDITS_SIZE);
    hts_textures[14].pixels = pixels;
    if (hts_textures[14].pixels != NULL)
    {
        textLen = 0;
        xp = 0;
        while (textLen < HTS_CREDITS_SIZE)
        {
            srcn = 0;
            yp = 0;
            while (yp < HTS_CREDITS_SIZE)
            {
                srcp = srcn + textLen;
                yp += HTS_CREDITS_HEIGHT;
                srcn++;
                hts_textures[14].pixels[yp + (-xp - 1)] = hts_creditsTexPixels[srcp];
            }
            textLen += HTS_CREDITS_WIDTH;
            xp++;
        }
        hts_creditsTexPixels = hts_textures[14].pixels;
        if (hts_hCreditsDC != NULL)
        {
            gdiobj = SelectObject(hts_hCreditsDC, hts_creditsSelectedBitmap);
            DeleteObject(gdiobj);
            DeleteDC(hts_hCreditsDC);
        }
        return true;
    }
    return false;
}

/**
 * Creates the bitmaps for the screen and credits image and loads the palette data.
 */
bool HTS_SetupFramebuffer(void)
{
    bool success;
    HDC hdc;
    HBITMAP h;
    int srcIndex;
    int dstIndex;
    PALETTEENTRY *entries;
    LOGPALETTE logicalPalette[128];
    BYTE colour;

    success = 0;

    logicalPalette[0].palVersion = 0x300;
    logicalPalette[0].palNumEntries = 256;
    entries = logicalPalette[0].palPalEntry;
    for (srcIndex = 256; srcIndex != 0; srcIndex--)
    {
        entries->peRed = 0;
        entries->peGreen = 0;
        entries->peBlue = 0;
        entries->peFlags = 0;
        entries++;
    }

    if (DAT_5044d988 != 0)
    {
        hts_bitmapinfo = GlobalAlloc(0, sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256);
        if (hts_bitmapinfo == NULL)
            return false;
        hts_creditsBitmapInfo = GlobalAlloc(0, sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256);
        if (hts_creditsBitmapInfo == NULL)
            return false;

        hts_bitmapinfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        hts_bitmapinfo->bmiHeader.biPlanes = 1;
        hts_bitmapinfo->bmiHeader.biBitCount = 8;
        hts_bitmapinfo->bmiHeader.biCompression = 0;
        hts_bitmapinfo->bmiHeader.biSizeImage = 0;
        hts_bitmapinfo->bmiHeader.biClrUsed = 0;
        hts_bitmapinfo->bmiHeader.biClrImportant = 0;
        hts_bitmapinfo->bmiHeader.biWidth = HTS_SCREEN_WIDTH;
        hts_bitmapinfo->bmiHeader.biHeight = -HTS_SCREEN_HEIGHT;

        hdc = GetDC(NULL);
        GetSystemPaletteEntries(hdc, 0, 10, logicalPalette[0].palPalEntry);
        GetSystemPaletteEntries(hdc, 246, 10, &logicalPalette[0].palPalEntry[246]);
        ReleaseDC(NULL, hdc);

        srcIndex = 0;
        while (srcIndex < 10)
        {
            hts_bitmapinfo->bmiColors[srcIndex].rgbRed = logicalPalette[0].palPalEntry[srcIndex].peRed;
            hts_bitmapinfo->bmiColors[srcIndex].rgbGreen = logicalPalette[0].palPalEntry[srcIndex].peGreen;
            hts_bitmapinfo->bmiColors[srcIndex].rgbBlue = logicalPalette[0].palPalEntry[srcIndex].peBlue;
            hts_bitmapinfo->bmiColors[srcIndex].rgbReserved = 0;
            logicalPalette[0].palPalEntry[srcIndex].peFlags = 0;

            hts_bitmapinfo->bmiColors[srcIndex + 246].rgbRed = logicalPalette[0].palPalEntry[srcIndex + 246].peRed;
            hts_bitmapinfo->bmiColors[srcIndex + 246].rgbGreen = logicalPalette[0].palPalEntry[srcIndex + 246].peGreen;
            hts_bitmapinfo->bmiColors[srcIndex + 246].rgbBlue = logicalPalette[0].palPalEntry[srcIndex + 246].peBlue;
            hts_bitmapinfo->bmiColors[srcIndex + 246].rgbReserved = 0;
            logicalPalette[0].palPalEntry[srcIndex + 246].peFlags = 0;
            srcIndex++;
        }

        dstIndex = 40;
        srcIndex = 0;
        while (dstIndex < (246 * 4))
        {
            colour = hts_paletteColours[srcIndex];
            (&logicalPalette[0].palPalEntry[0].peRed)[dstIndex] = colour;
            (&hts_bitmapinfo->bmiColors[0].rgbRed)[dstIndex] = colour;

            colour = hts_paletteColours[srcIndex + 1];
            (&logicalPalette[0].palPalEntry[0].peGreen)[dstIndex] = colour;
            (&hts_bitmapinfo->bmiColors[0].rgbGreen)[dstIndex] = colour;

            colour = hts_paletteColours[srcIndex + 2];
            (&logicalPalette[0].palPalEntry[0].peBlue)[dstIndex] = colour;
            (&hts_bitmapinfo->bmiColors[0].rgbBlue)[dstIndex] = colour;

            (&hts_bitmapinfo->bmiColors[0].rgbReserved)[dstIndex] = 0;
            (&logicalPalette[0].palPalEntry[0].peFlags)[dstIndex] = PC_NOCOLLAPSE;

            dstIndex += 4;
            srcIndex += 3;
        };

        hts_palette = CreatePalette(logicalPalette);

        hts_dc = CreateCompatibleDC(NULL);
        SelectPalette(hts_dc, hts_palette, 0);
        RealizePalette(hts_dc);
        h = CreateDIBSection(hts_dc, hts_bitmapinfo, 0, &hts_dibSectionBits, NULL, 0);
        hts_brush = SelectObject(hts_dc, h);
        PatBlt(hts_dc, 0, 0, HTS_SCREEN_WIDTH, HTS_SCREEN_HEIGHT, BLACKNESS);

        XL_CopyArray(
            (byte *)hts_bitmapinfo, (byte *)hts_creditsBitmapInfo, sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256);
        hts_creditsBitmapInfo->bmiHeader.biWidth = HTS_CREDITS_WIDTH;
        hts_creditsBitmapInfo->bmiHeader.biHeight = -HTS_CREDITS_HEIGHT;

        hts_hCreditsDC = CreateCompatibleDC(NULL);
        SelectPalette(hts_hCreditsDC, hts_palette, 0);
        RealizePalette(hts_hCreditsDC);
        hts_creditsBitmap =
            CreateDIBSection(hts_hCreditsDC, hts_creditsBitmapInfo, 0, (void **)&hts_creditsTexPixels, NULL, 0);
        hts_creditsSelectedBitmap = SelectObject(hts_hCreditsDC, hts_creditsBitmap);
        PatBlt(hts_hCreditsDC, 0, 0, HTS_CREDITS_WIDTH, HTS_CREDITS_HEIGHT, BLACKNESS);
        success = HTS_MakeCreditsBitmap();
    }
    return success;
}

void HTS_GetScreenAndStride(byte **screenOut, uint *strideOut)
{
    *screenOut = (byte *)hts_dibSectionBits;
    *strideOut = (((hts_bitmapinfo->bmiHeader.biWidth * hts_bitmapinfo->bmiHeader.biBitCount) + 31) & ~31) >> 3;
    return;
}

void HTS_CopyFramebuffer(void)
{
    BitBlt(hts_windowDC, 0, 0, HTS_SCREEN_WIDTH, HTS_SCREEN_HEIGHT, hts_dc, 0, 0, SRCCOPY);
    return;
}

void HTS_SetupPalette(void)
{
    PALETTEENTRY *entries;
    HPALETTE hPalette;
    LOGPALETTE logicalPalette[128]; // Need enough room on the stack for 256 palette entries
    int i;

    logicalPalette[0].palVersion = 0x300;
    logicalPalette[0].palNumEntries = 256;

    // Coding oversight? The palette entries are all initialized to zero, then the same thing is done again but with the
    // palette entry flags set to NOCOLLAPSE.
    entries = logicalPalette[0].palPalEntry;
    for (i = 256; i != 0; i--)
    {
        entries->peRed = 0;
        entries->peGreen = 0;
        entries->peBlue = 0;
        entries->peFlags = 0;
        entries++;
    }

    entries = logicalPalette[0].palPalEntry;
    while (entries < (logicalPalette->palPalEntry + 256))
    {
        entries->peRed = 0;
        entries->peGreen = 0;
        entries->peBlue = 0;
        entries->peFlags = PC_NOCOLLAPSE;
        entries++;
    }

    hPalette = CreatePalette(logicalPalette);
    if (hPalette != NULL)
    {
        hPalette = SelectPalette(hts_windowDC, hPalette, 0);
        RealizePalette(hts_windowDC);
        hPalette = SelectPalette(hts_windowDC, hPalette, 0);
        DeleteObject(hPalette);
    }

    return;
}

// 50303c95
LRESULT CALLBACK HTS_WndProc(HWND hWnd, uint msg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch (msg)
    {
    case WM_ACTIVATE:
    case WM_ACTIVATEAPP:
        if (wParam == 0)
        {
            hts_keys = hts_keys | 1;
        }
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        SelectPalette(hdc, xl_hPalette, 0);
        RealizePalette(hdc);
        HTS_CopyFramebuffer();
        EndPaint(hWnd, &ps);
        return 0;
    case WM_PALETTECHANGED:
        hts_keys = hts_keys | 1;
        break;
    case WM_QUIT:
        hts_keys = hts_keys | 1;
        return 0;
    case WM_DESTROY:
        break;
    }
    return DefWindowProcA(hWnd, msg, wParam, lParam);
}

void HTS_SetupWindow(void)
{
    ATOM wndclass;
    WNDCLASSA wc;

    if (DAT_5044fed0 == 0)
    {
        wc.hCursor = LoadCursorA(NULL, (LPCSTR)IDC_ARROW);
        wc.hIcon = LoadIconA(NULL, (LPCSTR)IDI_APPLICATION);
        wc.lpszMenuName = "AppMenu";
        wc.lpszClassName = "zip";
        wc.hbrBackground = NULL;
        wc.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS | CS_OWNDC | CS_BYTEALIGNCLIENT;
        wc.lpfnWndProc = HTS_WndProc;
        wc.cbWndExtra = 0;
        wc.cbClsExtra = 0;
        wc.hInstance = xl_hInstance;
        wndclass = RegisterClassA(&wc);
        if (wndclass == 0)
            return;
    }

    HTS_SetupPalette();

    hts_hWnd = CreateWindowExA(
        0, "zip", "Hall of Tortured Souls", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, HTS_SCREEN_WIDTH + 8,
        HTS_SCREEN_HEIGHT + 26, NULL, NULL, NULL, NULL);
    ShowWindow(hts_hWnd, SW_SHOW);

    hts_windowDC = GetDC(hts_hWnd);
    SelectPalette(hts_windowDC, xl_hPalette, 0);
    RealizePalette(hts_windowDC);

    return;
}

void HTS_ReadWindowMessages(void)
{
    BOOL gotMsg;
    MSG msg;

    gotMsg = PeekMessageA(&msg, NULL, 0, 0, 1);
    if (gotMsg != 0)
    {
        while (msg.message != WM_QUIT)
        {
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
            gotMsg = PeekMessageA(&msg, NULL, 0, 0, 1);
            if (gotMsg == 0)
            {
                return;
            }
        }
        hts_keys = hts_keys | 1;
    }
    return;
}
