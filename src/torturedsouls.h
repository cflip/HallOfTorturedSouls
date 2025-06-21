#ifndef _H_TORTUREDSOULS
#define _H_TORTUREDSOULS

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "core_types.h"

// BUILD CONFIGURATION

#define STANDALONE

// STRUCT DEFINITIONS

typedef struct sector
{
	double floorHeight;
	double ceilHeight;
	undefined4 floorCol;
	undefined4 ceilCol;
	double relFloorHeight;
	double relCeilHeight;
} sector;

typedef struct wall
{
	double startX;
	double startY;
	double endX;
	double endY;
	double relStartY;
	double relStartX;
	double relEndX;
	double relEndY;
	double firstColumn;
	double lastColumn;
	double field10_0x50;
	double fixedPointScale;
	undefined4 unknown2;
	undefined4 unknown3;
	double g;
	double f2;
	double f1;
	undefined4 upperTex;
	undefined4 lowerTex;
	sector *sector1;
	sector *sector2;
	uint shouldRender;
	struct wall *next;
} wall;

typedef struct viewspan
{
	double firstColumn;
	double lastColumn;
	wall *solidWalls;
	wall *thruWalls;
	struct viewspan *next;
} viewspan;

typedef struct textureinfo
{
	undefined4 field0_0x0;
	undefined4 height;
	undefined4 field2_0x8;
	undefined4 width;
	undefined4 yIteratorMask;
	undefined4 xIteratorMask; // masks the x iterator in the texture mapper to wrap the texture
	undefined4 field6_0x18;
	undefined4 field7_0x1c;
	byte *pixels;
} textureinfo;

// GLOBAL FUNCTIONS

/* 50303670 */	undefined4 HTS_MakeCreditsBitmap(void);
/* 5030386c */	undefined4 HTS_SetupFramebuffer(void);
/* 50303baa */	void HTS_GetScreenAndStride(byte **screenOut,uint *strideOut);
/* 50303bd8 */	void HTS_CopyFramebuffer(void);
/* 50303c03 */	void HTS_SetupPalette(void);
/* 50303d6b */	void HTS_SetupWindow(void);
/* 50303e62 */	void HTS_Cleanup(void);
/* 50303eb1 */	int HTS_CopyLine(char *src,char *dst);
/* 50303ee6 */	void HTS_ParseDecimal(char **strlist);
/* 5030401b */	undefined4 HTS_LoadMapData(void);
/* 50304535 */	void HTS_ReadWindowMessages(void);
/* 503045a9 */	void HTS_RunGame(void);
/* 50304b24 */	void HTS_UpdateSectorRelHeights(void);
/* 50304b97 */	void HTS_WallCompute1(wall *wall,int param_2);
/* 50304cbe */	int FP2_Compare(double *param_1,double *param_2);
/* 50304d04 */	undefined4 HTS_WallCompute2(wall *param_1,wall *param_2);
/* 50304e61 */	void HTS_UpdateWallRelPositions(wall *walls,int numWalls);
/* 50305551 */	void HTS_FreeAll(void);
/* 503055ed */	undefined4 HTS_InitMapStructures(void);
/* 50305842 */	int HTS_InitPt2(void);
/* 503058e1 */	void HTS_UpdateRotationData(void);
/* 503059dc */	void HTS_AddSolidWallsToView(void);
/* 50305d3c */	void HTS_AddThruWallsToView(void);
/* 50305de1 */	void HTS_SetupWallLinks(viewspan *span);
/* 50305e1c */	void HTS_DrawWallColumn(int startRow,int endRow,wall *wall,int useUpperTex,double *param_5);
/* 50305ff3 */	void HTS_FillColumn(int startRow,int endRow,undefined1 colour);
/* 50306037 */	void HTS_DrawFloorColumn(int startRow,int endRow,int texnum,sector *sector);
/* 50306169 */	void HTS_RenderColumn(viewspan *param_1);
/* 503065e4 */	void HTS_Render(void);
/* 5030683e */	undefined4 HTS_ResetPlayerPos(void);
/* 50306882 */	void HTS_DoNothing(void);
/* 50306883 */	undefined4 HTS_Update(void);
/* 50306f40 */	void HTS_SetCameraPos(double *x,double *y,double *z,int sector);
/* 50306f94 */	bool HTS_Initialize(void);

/* 5014a22c */	int XL_StringLength(char *str);
/* 501548a7 */	byte *XL_CopyArray(byte *src, byte *dst, uint count);

// GLOBAL VARIABLES

/* 504315b0 */	extern double hts_scalarScreenWidth; // = 320.0
/* 504315b8 */	extern double hts_scalarHalfScreenWidth; // = 160.0
/* 504315c0 */	extern double hts_scalarScreenHeight; // = 200.0
/* 504315c8 */	extern double DOUBLE_504315c8; // = 0.032
/* 504315d0 */	extern double DOUBLE_504315d0; // = 0.02
/* 504315d8 */	extern double DOUBLE_504315d8; // 133.3333333333333

/* 50431ae0 */	extern byte hts_creditsData[];
/* 5043a5d0 */	extern textureinfo hts_textureData[15];
/* 5043a7ec */	extern char hts_mapData[][25];
/* 5043c634 */	extern byte hts_paletteColours[768];

/* 5043c958 */	extern double hts_rotSpeed; // = 0.05
/* 5043c960 */	extern double hts_scalarPi; // = 3.1415925
/* 5043c968 */	extern double hts_scalarTwoPi; // = 6.283185

/* 5044c004 */	extern sector *hts_playerSector;
/* 5044c008 */	extern undefined4 hts_numThruWalls;
/* 5044c010 */	extern double hts_viewZ;
/* 5044c018 */	extern double hts_playerZ;
/* 5044c020 */	extern wall *hts_firstWallInColumn;
/* 5044c024 */	extern undefined4 hts_cheatCodeProgress;
/* 5044c028 */	extern double hts_playerVel;
/* 5044c030 */	extern undefined4 hts_cheatCodeTimeout;
/* 5044c034 */	extern textureinfo *hts_textures;
/* 5044c038 */	extern double hts_viewCos;
/* 5044c040 */	extern double hts_playerY;
/* 5044c048 */	extern undefined4 hts_numSectors;
/* 5044c04c */	extern wall *hts_solidWalls;
/* 5044c050 */	extern double hts_fastSpeed;
/* 5044c058 */	extern LPBITMAPINFO hts_bitmapinfo;
/* 5044c05c */	extern byte hts_keys;
/* 5044c060 */	extern undefined4 hts_screenStride;
/* 5044c064 */	extern undefined4 hts_numSolidWalls;
/* 5044c068 */	extern double hts_normalSpeed;
/* 5044c070 */	extern undefined4 hts_drawingColumn;
/* 5044c074 */	extern viewspan *hts_viewspans;
/* 5044c078 */	extern HDC hts_hDC1;
/* 5044c07c */	extern byte *hts_creditsTexPixels;
/* 5044c080 */	extern wall *hts_lastSpanThruWall;
/* 5044c084 */	extern wall *hts_thruWalls;
/* 5044c088 */	extern int hts_playerXFixed;
/* 5044c08c */	extern int hts_playerYFixed;
/* 5044c090 */	extern LPBITMAPINFO hts_copiedBitmap;
/* 5044c098 */	extern double hts_viewSin;
/* 5044c0a0 */	extern byte *hts_screen; // pointer to framebuffer data
/* 5044c0a8 */	extern double hts_playerRot;
/* 5044c0b0 */	extern double hts_playerX;
/* 5044c0b8 */	extern HPALETTE hts_palette;

/* 50453cc8 */	extern undefined4 DAT_50453cc8; // = 438h
/* 50453ccc */	extern HWND hts_hWnd;
/* 50453cd0 */	extern undefined4 DAT_50453cd0;
/* 50453cd4 */	extern void *hts_dibSectionBits;
/* 50453cd8 */	extern HDC hts_dc;
/* 50453cdc */	extern HGDIOBJ hts_brush;
/* 50453ce0 */	extern HGDIOBJ HGDIOBJ_50453ce0;
/* 50453ce4 */	extern HBITMAP hts_creditsBitmap;
/* 50453ce8 */	extern HDC hts_hCreditsDC;
/* 50453cec */	extern sector *hts_sectors;
/* 50453cf0 */	extern undefined4 *hts_alloc1;
/* 50453cf4 */	extern undefined4 hts_rotationNeedsUpdate; // = 1
/* 50453cf8 */	extern double *hts_xRotTable;
/* 50453cfc */	extern double *hts_yRotTable;
/* 50453d00 */	extern double *hts_alloc4MidWidthDiv;
/* 50453d04 */	extern double hts_viewHeight; // = 0.667

/* 5044d980 */	extern HPALETTE xl_hPalette;
/* 5044fe9c */	extern HINSTANCE xl_hInstance;

#endif
