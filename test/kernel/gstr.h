/** ---------------------------------------------------------------------------
 * 
 * GORTOS
 * array manipulation library
 * 
 * created by rdupu13
 * 
 * @file gstr.h
 *
----------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
//  HEADER DEFINITION
//-----------------------------------------------------------------------------

#ifndef GSTR_H
#define GSTR_H


//-----------------------------------------------------------------------------
//  LIBRARIES
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//  MACROS
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//  FUNCTION PROTOTYPES
//-----------------------------------------------------------------------------

void gstr_copy(char *src, char *dst, int len);   // copy an array to another array
unsigned int gstr_len(char *arr);              // get the length of an array
void gstr_split(char **strptr_arr, char *arr, char delim);
unsigned char gstr_cmp(char *dst, char *src);

void hex_to_str(char *str, unsigned char *hex, unsigned int len);
void str_to_hex(unsigned char *hex, char *str);

char *hex(unsigned int n);

//void uint_to_bcd(unsigned char *bcd, unsigned int num);

//char *uint_to_dec(unsigned int num);

//void bcd_to_uint(unsigned int num, unsigned char *bcd, unsigned int len);

//unsigned int dec_to_uint(char *str);

unsigned int gabs(int i); // absolute value

#endif
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
