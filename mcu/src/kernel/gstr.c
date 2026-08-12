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

char hex_str[5];
char dec_str[] = "00000";


//-----------------------------------------------------------------------------
//  FUNCTIONS
//-----------------------------------------------------------------------------

/**
 * @brief copy an array to another array
 * 
 * @param dst   destination array
 * @param src   source array
 * @param len   number of bytes to copy
 * 
 * @return none
 */
void gopy(char *dst, char *src, int len)
{
    int i;
    for (i = 0; i < len; i++)
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
unsigned int girth(char *arr)
{
    unsigned int i;
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
 * @brief convert hexadecimal to string representation
 * 
 * @param str string representation of hexadecimal
 * @param hex hexadecimal to be converted
 * @param len length of hexadecimal array
 * 
 * @return none
 */
void hex_to_str(char *str, unsigned char *hex, unsigned int len)
{
    unsigned int i;
    unsigned int j = 0;
    unsigned char hex_h;
    unsigned char hex_l;
    for (i = 0; i < len; i++)
    {
        hex_h = ((hex[i] >> 4) & 0x0F);
        switch(hex_h)
        {
            case 0x0A: str[j] = 'A'; break;
            case 0x0B: str[j] = 'B'; break;
            case 0x0C: str[j] = 'C'; break;
            case 0x0D: str[j] = 'D'; break;
            case 0x0E: str[j] = 'E'; break;
            case 0x0F: str[j] = 'F'; break;
            default: str[j] = hex_h + '0'; break;
        }

        hex_l = (hex[i] & 0x0F);
        switch(hex_l)
        {
            case 0x0A: str[j + 1] = 'A'; break;
            case 0x0B: str[j + 1] = 'B'; break;
            case 0x0C: str[j + 1] = 'C'; break;
            case 0x0D: str[j + 1] = 'D'; break;
            case 0x0E: str[j + 1] = 'E'; break;
            case 0x0F: str[j + 1] = 'F'; break;
            default: str[j + 1] = hex_l + '0'; break;
        }

        j += 2;
    }
}

/**
 * @brief convert string representation to hexadecimal
 * 
 * @param hex hexadecimal array from string
 * @param str string representation to be converted
 * 
 * @return none
 */
void str_to_hex(unsigned char *hex, char *str)
{
    unsigned int i = 0;
    unsigned int j;
    unsigned char hex_h;
    unsigned char hex_l;
    for (j = 0; str[j] != '\0'; j += 2)
    {
        switch(str[j])
        {
            case 'A': hex_h = 0xA0; break;
            case 'B': hex_h = 0xB0; break;
            case 'C': hex_h = 0xC0; break;
            case 'D': hex_h = 0xD0; break;
            case 'E': hex_h = 0xE0; break;
            case 'F': hex_h = 0xF0; break;
            default: hex_h = (str[j] << 4) & 0xF0; break;
        }
        hex[i] = hex_h;
        
        if (str[j + 1] == '\0') { i++; break; }
        
        switch(str[j + 1])
        {
            case 'A': hex_l = 0x0A; break;
            case 'B': hex_l = 0x0B; break;
            case 'C': hex_l = 0x0C; break;
            case 'D': hex_l = 0x0D; break;
            case 'E': hex_l = 0x0E; break;
            case 'F': hex_l = 0x0F; break;
            default: hex_l = str[j + 1] & 0x0F; break;
        }
        hex[i] |= hex_l;

        i++;
    }
}

/**
 * 
 */
char *hex(unsigned int n)
{
    hex_str[4] = '\0';
    
    if (n & 0xFF00)
    {
        unsigned int n_l = ((n << 8) & 0xFF00);
        unsigned int n_h = ((n >> 8) & 0x00FF);
        unsigned int n_swap = n_h | n_l;
        
        hex_to_str(hex_str, (unsigned char *) &n_swap, 2);
        return hex_str;
    }
    
    hex_to_str(hex_str + 2, (unsigned char *) &n, 1);
    return hex_str + 2;
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
 * @brief convert unsigned integer to string representation
 *
char *uint_to_dec(unsigned int num)
{
    // 5 x 4-bit BCD (char[3]) -> char[5]
}
*/

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
 * @brief convert decimal string representation to unsigned integer
 *
unsigned int dec_to_uint(char *str)
{
    // char[5] -> 5 x 4-bit BCD (char[3])
}
*/

/**
 * @brief absolute value
 * 
 * @param i
 * 
 * @return 
 */
unsigned int gabs(int i)
{
    if (i & 0x8000) {
        return (unsigned int) -i;
    }
    return (unsigned int) i;
}
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
