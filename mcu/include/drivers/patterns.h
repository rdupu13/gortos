/** ---------------------------------------------------------------------------
 * 
 * GORTOS
 * patterns generator library
 * 
 * created by rdupu13
 * 
 * @file patterns.h
 *
----------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
//  HEADER DEFINITION
//-----------------------------------------------------------------------------

#ifndef PATTERNS_H
#define PATTERNS_H


//-----------------------------------------------------------------------------
//  LIBRARIES
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//  MACROS
//-----------------------------------------------------------------------------

#define PATTERNS_TOTAL_NUM 6

#define PATTERNS_TEST   0xBB // bb <3

// animated frames --------------------
// inout
#define INOUT_LEN    8
#define INOUT0       0x0030
#define INOUT1       0x0048
#define INOUT2       0x0084
#define INOUT3       0x0102
#define INOUT4       0x0201
#define INOUT5       0x0102
#define INOUT6       0x0084
#define INOUT7       0x0048
// fill right
#define FILLR_LEN    10
#define FILLR0       0x0200
#define FILLR1       0x0300
#define FILLR2       0x0380
#define FILLR3       0x03C0
#define FILLR4       0x03E0
#define FILLR5       0x03F0
#define FILLR6       0x03F8
#define FILLR7       0x03FC
#define FILLR8       0x03FE
#define FILLR9       0x03FF
// ------------------------------------

// only powers of 2:
#define PATTERN0_QDIV    4
#define PATTERN1_QDIV    2
#define PATTERN2_QDIV    2
#define PATTERN3_QDIV    1
// pattern 4 is divided by 3 and needs its own counter >:(
#define PATTERN5_QDIV    4

#define PATTERN0_INIT    0x02AA
#define PATTERN1_INIT    0x0000
#define PATTERN2_INIT    INOUT0
#define PATTERN3_INIT    0x00FF
#define PATTERN4_INIT    0x0001
#define PATTERN5_INIT    FILLR0


//-----------------------------------------------------------------------------
//  FUNCTION PROTOTYPES
//-----------------------------------------------------------------------------

extern volatile unsigned int *cur_pattern;  // current pattern pointer

void patterns_init(void);                   // initialize patterns

void patterns_sel(int sel);                 // select current pattern
void patterns_next(void);                   // select the next pattern in cycle
void patterns_update(unsigned int qcnt);    // update patterns based on quarter-second counter


#endif
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
