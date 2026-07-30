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

void gopy(char *src, char *dst, int n); // copy array to another array
int girth(char *arr);                   // get the length of an array
//void gsplits(char *arr, char delim);

void hex(char *hex, unsigned int n);

//void uint_to_bcd(unsigned char *bcd, unsigned int num);
void bcd_to_str(char *str, unsigned char *bcd, unsigned int len);

//char *uint_to_str(unsigned int num);

void str_to_bcd(unsigned char *bcd, char *str);
//void bcd_to_uint(unsigned int num, unsigned char *bcd, unsigned int len);

//unsigned int str_to_uint(char *str);


#endif
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
