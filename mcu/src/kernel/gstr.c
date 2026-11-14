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

#include "kernel/gio.h" // debugging only


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------

char hex_str[5]; // chars from 16-bit hex
unsigned char to_hex[3]; // to 16-bit hex

char dec_str[] = "000000";




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
void gstr_copy(char *dst, char *src, int len)
{
    int i;
    for (i = 0; i < len; i++)
    {
        dst[i] = src[i];
    }
}

/**
 * @brief get the length of a string
 * 
 * @param arr string to get the length of
 * 
 * @return length of given string
 */
unsigned int gstr_len(char *arr)
{
    unsigned int i;
    for (i = 0; arr[i] != '\0'; i++) {}
    return i;
}

/**
 * @brief split a string
 * 
 * @param strptr_arr array of strings
 * @param arr string to be split up
 * @param delim delimiter character
 * 
 * @return none
 */
void gstr_split(char **strptr_arr, char *arr, char delim)
{
    // TODO: improve using gsh version
    unsigned int i;
    unsigned int j = 1;
    strptr_arr[0] = arr;
    
    for (i = 0; i < gstr_len(arr); i++)
    {
        if (arr[i] == delim) {
            arr[i] = '\0';
            strptr_arr[j++] = &arr[i + 1];
        }

        //if(j == MAX_PTRS) { break; }
    }

    strptr_arr[j] = (char *) 0;
}

/**
 * 
 */
unsigned char gstr_cmp(char *dst, char *src)
{
    unsigned int i;
    for (i = 0; i < gstr_len(src); i++)
    {
        if (dst[i] != src[i]) { return 0; }
    }
    return 1;
}

/**
 * @brief convert hexadecimal to string representation
 * 
 * @param str string representation of hexadecimal (len x2)
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
 * @param str string representation to be converted
 * 
 * @return none
 */
unsigned char *str_to_hex(char *str)
{
    int j;
    unsigned int strlen = gstr_len(str);
    unsigned int i;
    
    // clear hex bytes
    for (i = 0; i < 3; i++) {
        to_hex[i] = 0;
    }
    i = 0;

    unsigned char pol;
    if ((strlen & 1) == 0)
    {
        pol = 1; // even lengths start on odd index
    }
    else
    {
        pol = 0; // odd lengths start on even index
    }

    // decrement through string (hex bytes end up reversed)
    for (j = (strlen - 1); j != -1; j--)
    {
        if ((j & 1) == pol)
        {
            switch(str[j]) // left nibble
            {
                case 'A': to_hex[i] |= 0xA0; break;
                case 'B': to_hex[i] |= 0xB0; break;
                case 'C': to_hex[i] |= 0xC0; break;
                case 'D': to_hex[i] |= 0xD0; break;
                case 'E': to_hex[i] |= 0xE0; break;
                case 'F': to_hex[i] |= 0xF0; break;
                default: to_hex[i] |= ((str[j] & 0x0F) << 4); break;
            }
        }
        else
        {
            switch(str[j]) // right nibble
            {
                case 'A': to_hex[i] |= 0x0A; break;
                case 'B': to_hex[i] |= 0x0B; break;
                case 'C': to_hex[i] |= 0x0C; break;
                case 'D': to_hex[i] |= 0x0D; break;
                case 'E': to_hex[i] |= 0x0E; break;
                case 'F': to_hex[i] |= 0x0F; break;
                default: to_hex[i] |= (str[j] & 0x0F); break;
            }
            i++; // increment hex index after writing to right nibble
        }
    }

    return to_hex;
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
 * DOUBLE DABBLE ALGORITHM!!!!!!!!!!!!
 * 
 * @param bcd 3 BYTES LONG BCD
 * @param num 16-bit unsigned integer
 * 
 * @return none
 */
void uint_to_bcd(unsigned char *bcd, unsigned int num)
{
    unsigned char i;
    for (i = 0; i < 16; i++)
    {
        if ((bcd[2] & 0x0F) >= 0x05) { bcd[2] += 0x03; }
        if (bcd[1] >= 0x50)          { bcd[1] += 0x30; }
        if ((bcd[1] & 0x0F) >= 0x05) { bcd[1] += 0x03; }
        if (bcd[0] >= 0x50)          { bcd[0] += 0x30; }
        if ((bcd[0] & 0x0F) >= 0x05) { bcd[0] += 0x03; }

        bcd[2] = (bcd[2] << 1) | ((bcd[1] & 0x80) ? 1 : 0);
        bcd[1] = (bcd[1] << 1) | ((bcd[0] & 0x80) ? 1 : 0);
        bcd[0] = (bcd[0] << 1) | ((num & 0x8000) ? 1 : 0);
        num = (num << 1);
    }
    unsigned char tmp = bcd[2];
    bcd[2] = bcd[0];
    bcd[0] = tmp;
}

/**
 * @brief convert unsigned integer to string representation
 * 
 * @param num
 * 
 * @return 
 */
char *uint_to_dec(unsigned int num)
{
    // TODO: handle 0 case
    unsigned char bcd[] = {0x00, 0x00, 0x00};
    uint_to_bcd(bcd, num);
    hex_to_str(dec_str, bcd, 3);
    unsigned int i;
    for (i = 0; dec_str[i] == '0'; i++) {}
    return dec_str + i;
}

/**
 * @brief convert binary-coded decimal to an unsigned integer
 * 
 * @return none
 */
unsigned int bcd_to_uint(void)
{
    unsigned int num = 0;
    
    unsigned int ten_thou = ((to_hex[2] & 0xF0) >> 4);
    ten_thou = (ten_thou << 3) + (ten_thou << 1); // x10
    
    unsigned int thou = (to_hex[1] & 0x0F);
    thou = (thou << 3) + (thou << 1); // x10
    ten_thou = (ten_thou << 3) + (ten_thou << 1); // x100
    
    unsigned int hund = ((to_hex[1] & 0xF0) >> 4);
    hund = (hund << 3) + (hund << 1); // x10
    thou = (ten_thou << 3) + (thou << 1); // x100
    ten_thou = (ten_thou << 3) + (ten_thou << 1); // x1000

    unsigned int ten = (to_hex[0] & 0x0F);
    ten = (ten << 3) + (ten << 1); // x10
    hund = (hund << 3) + (hund << 1); // x100
    thou = (ten_thou << 3) + (thou << 1); // x1000
    ten_thou = (ten_thou << 3) + (ten_thou << 1); // x10000
    
    num = ten_thou + thou + hund + ten + ((to_hex[0] & 0xF0) >> 4);
    
    return num;
}

/**
 * @brief convert decimal string representation to unsigned integer
 */
unsigned int dec_to_uint(char *str)
{
    unsigned int new_uint;

    str_to_hex(str); // writes to_hex

    helloworld(hex(to_hex[0]));
    helloworld(" ");
    helloworld(hex(to_hex[1]));
    helloworld(" ");
    helloworld(hex(to_hex[2]));
    helloworld("\n");
    
    new_uint = bcd_to_uint(); // reads to_hex

    helloworld(uint_to_dec(new_uint));
    helloworld("\n");

    return 0;
}

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
