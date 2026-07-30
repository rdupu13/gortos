/** ---------------------------------------------------------------------------
 * 
 * GORTOS
 * patterns generator
 * 
 * created by rdupu13
 * 
 * @file patterns.c
 *
----------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
//  LIBRARIES
//-----------------------------------------------------------------------------

#include "drivers/patterns.h"


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------

// animated frame buffers -------------
volatile unsigned int inout[INOUT_LEN];
volatile unsigned char inout_cnt;

volatile unsigned int fillr[FILLR_LEN];
volatile unsigned char fillr_cnt;
// ------------------------------------

volatile int cur_pattern_num;

volatile unsigned int test_pattern;
volatile unsigned int pattern0;
volatile unsigned int pattern1;
volatile unsigned int pattern2;
volatile unsigned int pattern3;
volatile unsigned int pattern4; volatile unsigned char pattern4_qcnt; // annoying TODO: remove somehow
volatile unsigned int pattern5;


//-----------------------------------------------------------------------------
//  FUNCTIONS
//-----------------------------------------------------------------------------

/**
 * @brief initialize patterns
 * 
 * @param none
 * @return none
 */
void patterns_init(void)
{
    test_pattern = PATTERNS_TEST;

    inout_cnt = 0;
    inout[0] = INOUT0;
    inout[1] = INOUT1;
    inout[2] = INOUT2;
    inout[3] = INOUT3;
    inout[4] = INOUT4;
    inout[5] = INOUT5;
    inout[6] = INOUT6;
    inout[7] = INOUT7;

    fillr_cnt = 0;
    fillr[0] = FILLR0;
    fillr[1] = FILLR1;
    fillr[2] = FILLR2;
    fillr[3] = FILLR3;
    fillr[4] = FILLR4;
    fillr[5] = FILLR5;
    fillr[6] = FILLR6;
    fillr[7] = FILLR7;
    fillr[8] = FILLR8;
    fillr[9] = FILLR9;

    pattern0 = PATTERN0_INIT;
    pattern1 = PATTERN1_INIT;
    pattern2 = PATTERN2_INIT;
    pattern3 = PATTERN3_INIT;
    pattern4 = PATTERN4_INIT;
    pattern5 = PATTERN5_INIT;

    cur_pattern = &pattern0;
    cur_pattern_num = 0;
}

/**
 * @brief select current pattern
 * 
 * @param sel pattern number to set as current pattern
 * 
 * @return none
 */
void patterns_sel(int sel)
{
    cur_pattern_num = sel;
    switch(sel)
    {
        case -1: cur_pattern = &test_pattern; break;
        case 0: cur_pattern = &pattern0; break;
        case 1: cur_pattern = &pattern1; break;
        case 2: cur_pattern = &pattern2; break;
        case 3: cur_pattern = &pattern3; break;
        case 4: cur_pattern = &pattern4; break;
        case 5: cur_pattern = &pattern5; break;
        default: cur_pattern = &test_pattern; break;
    }
}

/**
 * @brief select the next pattern in cycle
 * 
 * @param none
 * 
 * @return none
 */
void patterns_next(void)
{
    cur_pattern_num++;
    if (cur_pattern_num > (PATTERNS_TOTAL_NUM - 1))
    {
        cur_pattern_num = 0;
    }
    patterns_sel(cur_pattern_num);
}

/**
 * @brief update patterns based on quarter-second counter
 * 
 * @param qcnt timer quarter-second counter
 * 
 * @return none
 */
void patterns_update(unsigned int qcnt)
{
    // pattern 0: alternating ---------------------------------
    if (!(qcnt & (PATTERN0_QDIV - 1)))
    {
        pattern0 ^= 0x03FF;
    }
    // --------------------------------------------------------

    // pattern 1: up counter ----------------------------------
    if (!(qcnt & (PATTERN1_QDIV - 1)))
    {
        pattern1++;
        if (pattern1 == 0x0400) {
            pattern1 = 0x0000;
        }
    }
    // --------------------------------------------------------
    
    // pattern 2: inout ---------------------------------------
    if (!(qcnt & (PATTERN2_QDIV - 1)))
    {
        inout_cnt++;
        if (inout_cnt == INOUT_LEN) {
            inout_cnt = 0;
        }
        pattern2 = inout[inout_cnt];
    }
    // --------------------------------------------------------

    // pattern 3: down counter --------------------------------
    if (!(qcnt & (PATTERN3_QDIV - 1)))
    {
        pattern3--;
        if (pattern3 == 0xFFFF) {
            pattern3 = 0x00FF;
        }
    }
    // --------------------------------------------------------

    // pattern 4: rotate left ---------------------------------
    pattern4_qcnt++; // TODO: fix, assumes this is called every quarter second 
    if (pattern4_qcnt == 3) {
        pattern4 = (pattern4 << 1);
        if (pattern4 == 0x0400) {
            pattern4 = 0x0001;
        }
        pattern4_qcnt = 0;
    }
    // --------------------------------------------------------
    
    // pattern 5: fill right ----------------------------------
    if (!(qcnt & (PATTERN5_QDIV - 1)))
    {
        fillr_cnt++;
        if (fillr_cnt == FILLR_LEN) {
            fillr_cnt = 0;
        }
        pattern5 = fillr[fillr_cnt];
    }
    // --------------------------------------------------------
}
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
