/**
 * xl_util.c
 *
 * Miscellaneous utility functions that are used across the Excel codebase.
 */

#include "torturedsouls.h"

/**
 * Calculates the length of a null-terminated string, just like strlen().
 */
int XL_StringLength(char *str)
{
    char *ptr;
    char *next;
    char c;

    ptr = str;
    do
    {
        next = ptr + 1;
        c = *ptr;
        ptr = next;
    } while (c != '\0');
    return (int)(next - 1 - str);
}

/**
 * Copies `count` bytes from `src` to `dst`, and returns a pointer to the end of the copied result.
 */
byte *XL_CopyArray(byte *src, byte *dst, uint count)
{
    uint i;
    uint *src4;
    byte *srcLeft;
    uint *dst4;
    byte *dstLeft;

    if (count != 0)
    {
        if (src <= dst)
        {
            // Copy four bytes at a time
            src4 = (uint *)(src + (count - 4));
            dst4 = (uint *)(dst + (count - 4));
            for (i = count >> 2; i != 0; i--)
            {
                *dst4 = *src4;
                src4--;
                dst4--;
            }
            // Copy remaining bytes if count is not a multiple of 4
            srcLeft = (byte *)((byte *)src4 + 3);
            dstLeft = (byte *)((byte *)dst4 + 3);
            for (i = count & 3; i != 0; i--)
            {
                *dstLeft = *srcLeft;
                srcLeft--;
                dstLeft--;
            }
            return dst + count;
        }
        // Same as above, but src4 is now used as the destination pointer
        src4 = (uint *)dst;
        for (i = count >> 2; i != 0; i--)
        {
            *src4 = *(uint *)src;
            src += 4;
            src4++;
        }
        for (i = count & 3; i != 0; i--)
        {
            *(byte *)src4 = *src;
            src += 1;
            src4 = (uint *)((byte *)src4 + 1);
        }
    }
    return dst + count;
}
