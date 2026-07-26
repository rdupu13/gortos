#include "gstr.h"

/**
 * copy n bytes of one array to another array
 */
void gopy(char *dst, char *src, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        *dst++ = *src++;        
    }
}

/**
 * get the length of an array
 */
int girth(char *arr)
{
    int i;
    for (i = 0; arr[i] != '\0'; i++) {}
    return i;
}

/**
 * 
 *
void gsplits(char *arr, char delim)
{
    int i;
    while ((arr[i] != delim) )
    {
        i++;
    }

}*/