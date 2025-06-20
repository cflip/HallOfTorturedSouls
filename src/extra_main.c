/**
 * extra_main.c
 *
 * Provides a main function for running the game as a standalone executable.
 */

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "torturedsouls.h"

HPALETTE xl_hPalette;
HINSTANCE xl_hInstance;

#ifdef STANDALONE

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nShowCmd);

	xl_hInstance = hInstance;

	HTS_RunGame();

	return 0;
}

#endif
