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
 * @brief convert an unsigned integer to binary-coded decimal
 *
char *uint_to_bcd(int num)
{
    // 16-bit unsigned int -> 5 x 4-bit BCD (char[3])
    // 8-bit unsigned int -> 3 x 4-bit BCD (char[2])
}
*/

/**
 * @brief convert binary-coded decimal to string representation
 *
char *bcd_to_str(char *bcd)
{
    // 5 x 4-bit BCD (char[3]) -> char[5]
}
*/

/**
 * @brief convert unsigned integer to string representation
 *
char *uint_to_str(int num)
{

}
*/

/**
 * @brief convert string representation to binary-coded decimal
 *
char *str_to_bcd(char *str)
{
    // char[5] -> 5 x 4-bit BCD (char[3])
}
*/

/**
 * @brief convert binary-coded decimal to an unsigned integer
 *
int bcd_to_uint(char *bcd)
{
    // 5 x 4-bit BCD (char[3]) -> 16-bit unsigned int
    // 3 x 4-bit BCD (char[2]) -> 8-bit unsigned int
}
*/

/**
 * @brief convert string representation to unsigned integer
 *
int str_to_uint(char *str)
{

}
*/
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
