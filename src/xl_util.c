/*
 *  xl_util.c
 *
 * Miscellaneous utility functions that are used across the Excel codebase.
 */

#include "torturedsouls.h"

/*
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

/*
 * Copies `count` bytes from `src` to `dst`.
 */
undefined *XL_CopyArray(undefined1 *src, undefined1 *dst, uint count)

{
    uint i;
    undefined4 *pSrc;
    undefined1 *puVar1;
    undefined4 *pDst;
    undefined1 *puVar2;

    if (count != 0)
    {
        if (src <= dst)
        {
            pSrc = (undefined4 *)(src + (count - 4));
            pDst = (undefined4 *)(dst + (count - 4));
            for (i = count >> 2; i != 0; i = i - 1)
            {
                *pDst = *pSrc;
                pSrc = pSrc + -1;
                pDst = pDst + -1;
            }
            puVar1 = (undefined1 *)((char *)pSrc + 3);
            puVar2 = (undefined1 *)((char *)pDst + 3);
            for (i = count & 3; i != 0; i = i - 1)
            {
                *puVar2 = *puVar1;
                puVar1 = puVar1 + -1;
                puVar2 = puVar2 + -1;
            }
            return dst + count;
        }
        pSrc = (undefined4 *)dst;
        for (i = count >> 2; i != 0; i = i - 1)
        {
            *pSrc = *(undefined4 *)src;
            src = (undefined1 *)((char *)src + 4);
            pSrc = pSrc + 1;
        }
        for (i = count & 3; i != 0; i = i - 1)
        {
            *(undefined1 *)pSrc = *src;
            src = (undefined1 *)((char *)src + 1);
            pSrc = (undefined4 *)((char *)pSrc + 1);
        }
    }
    return dst + count;
}
