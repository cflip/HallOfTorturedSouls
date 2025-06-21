/**
 * map.c
 *
 * Map loading code; contains a utility function for parsing numerical values from a string list and the function that
 * parses the map data and loads it into the various structures used by the game.
 */

#include "floating.h"
#include "torturedsouls.h"

/**
 * Copies up to 255 characters from `src` to `dst`, and adds a null terminator at the end. Returns true on success, and
 * false if the source string was too long.
 */
bool HTS_CopyLine(char *src, char *dst)
{
    int i;
    char c;

    i = 0;
    while ((c = src[i], c != '\n' && (c != '\0')))
    {
        dst[i] = c;
        i++;
        if (i >= 255)
        {
            return false;
        }
    }
    dst[i] = '\0';
    return true;
}

/**
 * Parses a double from the given list of strings, incrementing the pointer and storing the result in the global 64-bit
 * float.
 */
void HTS_ParseDecimal(char **strlist)
{
    char *nextString;
    bool isNegative;
    double result;
    double value10;
    char digitChar;
    char start;
    char tmpEnd;
    char *currentString;
    char end;

    FP_SetZero();
    FP_CopyTo(&result);
    FP_SetInteger(10);
    FP_CopyTo(&value10);

    // Find next number in the string list
    if (**strlist != '\0')
    {
        while (true)
        {
            start = **strlist;
            if ((start >= '0' && start <= '9') || (start == '-'))
                break;
            if ((start == '.') || (currentString = *strlist + 1, *strlist = currentString, *currentString == '\0'))
                break;
        }
    }

    isNegative = **strlist == '-';
    if (isNegative)
    {
        // Skip over the minus sign
        (*strlist)++;
    }

    // Read in a number as long as there are more digits to read
    digitChar = **strlist;
    while (digitChar >= '0' && **strlist <= '9')
    {
        FP_Set(&value10);
        FP_Mul(&result);
        FP_CopyTo(&result);
        FP_SetInteger(**strlist - '0');
        FP_Add(&result);
        FP_CopyTo(&result);
        currentString = *strlist;
        *strlist = currentString + 1;
        digitChar = currentString[1];
    }

    // Parse a single decimal point value if a point exists
    if (**strlist == '.')
    {
        nextString = *strlist + 1;
        *strlist = nextString;
        FP_SetInteger((byte)*nextString - L'0');
        FP_Div(&value10);
        FP_Add(&result);
        FP_CopyTo(&result);
        (*strlist)++;
    }

    // Advance strlist pointer to next number or end of a string
    end = **strlist;
    while (end != '\0')
    {
        tmpEnd = **strlist;
        if ((tmpEnd >= '0' && tmpEnd <= '9') || (tmpEnd == '-'))
            break;
        currentString = *strlist + 1;
        *strlist = currentString;
        end = *currentString;
    }

    FP_Set(&result);
    if (isNegative)
    {
        FP_Negate();
    }

    return;
}

/**
 * Reads through each line of the map data string and sets up the relevant data structures.
 */
bool HTS_LoadMapData(void)
{
    bool didCopy;
    undefined4 tmp1;
    int tmp2;
    int i;
    wall *wPtr;
    wall *zPtr;
    char *coordStr;
    double scale;
    double offsetY;
    double offsetX;
    double loadedX;
    double loadedY;
    double loadedZ;
    char line[25];
    int counter;

    FP_SetZero();
    FP_CopyTo(&offsetX);
    FP_CopyTo(&offsetY);
    FP_CopyTo(&hts_sectors->floorHeight);
    FP_SetInteger(10);
    FP_CopyTo(&hts_sectors->ceilHeight);
    FP_CopyTo(&scale);

    hts_numSolidWalls = 0;
    hts_numThruWalls = 0;
    hts_numSectors = 0;
    i = 0;
    hts_sectors->floorCol = 13;
    hts_sectors->ceilCol = 9;

    didCopy = HTS_CopyLine(hts_mapData[i], line);
    counter = hts_numThruWalls;
    tmp2 = hts_numSolidWalls;
    while (didCopy && line[0] != '\0')
    {
        hts_numThruWalls = counter;
        hts_numSolidWalls = tmp2;
        switch (line[0])
        {
        case 'b':
            // Set the position offset values for any future walls
            coordStr = line + 2;
            HTS_ParseDecimal(&coordStr);
            FP_Mul(&scale);
            FP_CopyTo(&offsetX);
            HTS_ParseDecimal(&coordStr);
            FP_Mul(&scale);
            FP_CopyTo(&offsetY);
            break;
        case 'e':
            // Define a sector and its floor height, ceiling height, floor colour, and ceiling colour
            coordStr = line + 2;
            HTS_ParseDecimal(&coordStr);
            FP_Mul(&scale);
            FP_CopyTo(&hts_sectors[hts_numSectors].floorHeight);
            HTS_ParseDecimal(&coordStr);
            FP_Mul(&scale);
            FP_CopyTo(&hts_sectors[hts_numSectors].ceilHeight);
            HTS_ParseDecimal(&coordStr);
            tmp1 = FP_AsInteger();
            hts_sectors[hts_numSectors].floorCol = tmp1;
            HTS_ParseDecimal(&coordStr);
            tmp1 = FP_AsInteger();
            hts_sectors[hts_numSectors].ceilCol = tmp1;
            hts_numSectors = hts_numSectors + 1;
            break;
        case 's':
            // Set the player's starting X, Y, Z, and the index of the starting sector
            coordStr = line + 2;
            HTS_ParseDecimal(&coordStr);
            FP_Mul(&scale);
            FP_CopyTo(&loadedX);
            HTS_ParseDecimal(&coordStr);
            FP_Mul(&scale);
            FP_CopyTo(&loadedY);
            HTS_ParseDecimal(&coordStr);
            FP_Mul(&scale);
            FP_CopyTo(&loadedZ);
            HTS_ParseDecimal(&coordStr);
            tmp1 = FP_AsInteger();
            HTS_SetCameraPos(&loadedX, &loadedY, &loadedZ, tmp1);
            break;
        case 'w':
            // Add a solid wall from a start X, Y, an end X, Y, and a texture index
            coordStr = line + 2;
            hts_numSolidWalls = tmp2 + 1;
            wPtr = hts_solidWalls + tmp2;
            HTS_ParseDecimal(&coordStr);
            FP_Mul(&scale);
            FP_Add(&offsetX);
            FP_CopyTo(&wPtr->startX);
            HTS_ParseDecimal(&coordStr);
            FP_Mul(&scale);
            FP_Add(&offsetY);
            FP_CopyTo(&wPtr->startY);
            HTS_ParseDecimal(&coordStr);
            FP_Mul(&scale);
            FP_Add(&offsetX);
            FP_CopyTo(&wPtr->endX);
            HTS_ParseDecimal(&coordStr);
            FP_Mul(&scale);
            FP_Add(&offsetY);
            FP_CopyTo(&wPtr->endY);
            HTS_ParseDecimal(&coordStr);
            tmp1 = FP_AsInteger();
            wPtr->upperTex = tmp1;
            FP_SetInteger(0x10000);
            FP_CopyTo(&wPtr->fixedPointScale);
            wPtr->sector2 = NULL;
            wPtr->sector1 = NULL;
            wPtr->lowerTex = 0;
            break;
        case 'z':
            // Add a line connecting two sectors from a start X, Y, and an end X, Y
            coordStr = line + 2;
            hts_numThruWalls = counter + 1;
            zPtr = hts_thruWalls + counter;
            HTS_ParseDecimal(&coordStr);
            FP_Mul(&scale);
            FP_Add(&offsetX);
            FP_CopyTo(&zPtr->startX);
            HTS_ParseDecimal(&coordStr);
            FP_Mul(&scale);
            FP_Add(&offsetY);
            FP_CopyTo(&zPtr->startY);
            HTS_ParseDecimal(&coordStr);
            FP_Mul(&scale);
            FP_Add(&offsetX);
            FP_CopyTo(&zPtr->endX);
            HTS_ParseDecimal(&coordStr);
            FP_Mul(&scale);
            FP_Add(&offsetY);
            FP_CopyTo(&zPtr->endY);
            FP_SetInteger(0x10000);
            FP_CopyTo(&zPtr->fixedPointScale);
            // Move to next line, which should start with 'x' and contain the sectors and textures for the current wall
            HTS_CopyLine(hts_mapData[i + 1], line);
            coordStr = line + 2;
            HTS_ParseDecimal(&coordStr);
            counter = FP_AsInteger();
            HTS_ParseDecimal(&coordStr);
            tmp2 = FP_AsInteger();
            HTS_ParseDecimal(&coordStr);
            tmp1 = FP_AsInteger();
            zPtr->upperTex = tmp1;
            HTS_ParseDecimal(&coordStr);
            tmp1 = FP_AsInteger();
            zPtr->lowerTex = tmp1;
            zPtr->sector1 = hts_sectors + counter;
            zPtr->sector2 = hts_sectors + tmp2;
            // i = i + 1;
        }
        // counter = i * 25;
        i++;
        didCopy = HTS_CopyLine(hts_mapData[i], line);
        counter = hts_numThruWalls;
        tmp2 = hts_numSolidWalls;
    }
    hts_numThruWalls = counter;
    hts_numSolidWalls = tmp2;
    return true;
}

void HTS_SetCameraPos(double *x, double *y, double *z, int sector)
{
    FP_Set(x);
    FP_CopyTo(&hts_playerX);
    FP_Set(y);
    FP_CopyTo(&hts_playerY);
    FP_Set(z);
    FP_CopyTo(&hts_playerZ);
    hts_playerSector = &hts_sectors[sector];
    return;
}
