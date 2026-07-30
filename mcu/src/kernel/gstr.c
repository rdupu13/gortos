/** ---------------------------------------------------------------------------
 * 
 * GORTOS
 * array manipulation
 * 
 * created by rdupu13
 * 
 * @file gstr.c
 *
----------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
//  LIBRARIES
//-----------------------------------------------------------------------------

#include "kernel/gstr.h"


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//  FUNCTIONS
//-----------------------------------------------------------------------------

/**
 * @brief copy array to another array
 * 
 * @param dst   destination array
 * @param src   source array
 * @param n     number of bytes to copy
 * 
 * @return none
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
 * @brief get the length of an array
 * 
 * @param arr array to get the length of
 * 
 * @return length of given array
 */
int girth(char *arr)
{
    int i;
    for (i = 0; arr[i] != '\0'; i++) {}
    return i;
}

/**
 * @brief 
 *
void gsplits(char *arr, char delim)
{
    int i;
    while ((arr[i] != delim) )
    {
        i++;
    }

}
*/

/**
 * 
 */
void hex(char *hex, unsigned int n)
{
    unsigned int n_l = ((n << 8) & 0xFF00);
    unsigned int n_h = ((n >> 8) & 0x00FF);
    unsigned int n_swap = n_h | n_l;
    bcd_to_str(hex, &n_swap, 2);
}

/**
 * @brief convert an unsigned integer to binary-coded decimal
 *
void uint_to_bcd(unsigned char *bcd, unsigned int num)
{
    // 16-bit unsigned int -> 5 x 4-bit BCD (char[3])
    // 8-bit unsigned int -> 3 x 4-bit BCD (char[2])
}
*/

/**
 * @brief convert binary-coded decimal to string representation
 * 
 * @param str string representation of binary-coded decimal
 * @param bcd binary-coded decimal to be converted
 * @param len length of binary-coded decimal array
 * 
 * @return none
 */
void bcd_to_str(char *str, unsigned char *bcd, unsigned int len)
{
    unsigned int i;
    unsigned int j = 0;
    unsigned char bcd_l;
    unsigned char bcd_h;
    for (i = 0; i < len; i++)
    {
        bcd_l = ((bcd[i] >> 4) & 0x0F);
        switch(bcd_l)
        {
            case 0x0A: str[j] = 'A'; break;
            case 0x0B: str[j] = 'B'; break;
            case 0x0C: str[j] = 'C'; break;
            case 0x0D: str[j] = 'D'; break;
            case 0x0E: str[j] = 'E'; break;
            case 0x0F: str[j] = 'F'; break;
            default: str[j] = bcd_l + '0'; break;
        }

        bcd_h = (bcd[i] & 0x0F);
        switch(bcd_h)
        {
            case 0x0A: str[j + 1] = 'A'; break;
            case 0x0B: str[j + 1] = 'B'; break;
            case 0x0C: str[j + 1] = 'C'; break;
            case 0x0D: str[j + 1] = 'D'; break;
            case 0x0E: str[j + 1] = 'E'; break;
            case 0x0F: str[j + 1] = 'F'; break;
            default: str[j + 1] = bcd_h + '0'; break;
        }

        j += 2;
    }
    //str[j] = '\0';
}

/**
 * @brief convert unsigned integer to string representation
 *
char *uint_to_str(unsigned int num)
{
    // 5 x 4-bit BCD (char[3]) -> char[5]
}
*/

/**
 * @brief convert string representation to binary-coded decimal
 * 
 * @param bcd binary-coded decimal array from string
 * @param str string representation to be converted
 * 
 * @return none
 */
void str_to_bcd(unsigned char *bcd, char *str)
{
    unsigned int i = 0;
    unsigned int j;
    for (j = 0; str[j] != '\0'; j += 2)
    {
        bcd[i] = ((str[j] << 4) & 0xF0);
        if (str[j + 1] == '\0') { break; }
        bcd[i] |= (str[j + 1] & 0x0F);
        i++;
    }
}

/**
 * @brief convert binary-coded decimal to an unsigned integer
 *
void bcd_to_uint(unsigned int num, unsigned char *bcd, unsigned int len)
{
    // 5 x 4-bit BCD (char[3]) -> 16-bit unsigned int
    // 3 x 4-bit BCD (char[2]) -> 8-bit unsigned int
}
*/

/**
 * @brief convert string representation to unsigned integer
 *
unsigned int str_to_uint(char *str)
{
    // char[5] -> 5 x 4-bit BCD (char[3])
}
*/
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
