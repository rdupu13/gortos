/** ---------------------------------------------------------------------------
 * 
 * GORTOS
 * led driver
 * 
 * created by rdupu13
 * 
 * @file led.c
 *
----------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
//  LIBRARIES
//-----------------------------------------------------------------------------

#include <msp430fr2153.h>
#include <stdint.h>

#include "drivers/led.h"


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//  FUNCTIONS
//-----------------------------------------------------------------------------

/**
 * @brief initialize leds
 * 
 * @param none
 * @return none
 */
void led_init()
{
    LED_HEARTBEAT_PORT &= ~LED_HEARTBEAT_PIN;
    LED_TEST0_PORT &= ~LED_TEST0_PIN;
    LED_TEST1_PORT &= ~LED_TEST1_PIN;
    LED_TEST2_PORT &= ~LED_TEST2_PIN;

    ledbar_inout_cnt = 0;
    ledbar_inout[0] = LEDBAR_INOUT0;
    ledbar_inout[1] = LEDBAR_INOUT1;
    ledbar_inout[2] = LEDBAR_INOUT2;
    ledbar_inout[3] = LEDBAR_INOUT3;
    ledbar_inout[4] = LEDBAR_INOUT4;
    ledbar_inout[5] = LEDBAR_INOUT5;
    ledbar_inout[6] = LEDBAR_INOUT6;
    ledbar_inout[7] = LEDBAR_INOUT7;

    ledbar_fillr_cnt = 0;
    ledbar_fillr[0] = LEDBAR_FILLR0;
    ledbar_fillr[1] = LEDBAR_FILLR1;
    ledbar_fillr[2] = LEDBAR_FILLR2;
    ledbar_fillr[3] = LEDBAR_FILLR3;
    ledbar_fillr[4] = LEDBAR_FILLR4;
    ledbar_fillr[5] = LEDBAR_FILLR5;
    ledbar_fillr[6] = LEDBAR_FILLR6;
    ledbar_fillr[7] = LEDBAR_FILLR7;
    ledbar_fillr[8] = LEDBAR_FILLR8;
    ledbar_fillr[9] = LEDBAR_FILLR9;

    ledbar_pattern0 = 0x02AA;
    ledbar_pattern1 = 0x0000;
    ledbar_pattern2 = ledbar_inout[0];
    ledbar_pattern3 = 0x00FF;
    ledbar_pattern4 = 0x0001;
    ledbar_pattern5 = ledbar_fillr[0];

    ledbar_sel(0);
}

/**
 * @brief update heartbeat led based on quarter-second counter
 * 
 * @param qcnt timer quarter-second counter
 * 
 * @return none
 */
void led_heartbeat_update(int qcnt)
{
    if (!(qcnt & (LED_HEARTBEAT_QDIV - 1))) {
        LED_HEARTBEAT_PORT ^= LED_HEARTBEAT_PIN; // toggle heartbeat led
    }
}

/**
 * @brief select ledbar pattern
 * 
 * @param sel pattern select (0-5)
 * 
 * @return none
 */
void ledbar_sel(int sel)
{
    // TODO: check sel
    
    switch(sel)
    {
        case 0: ledbar_cur_pattern = &ledbar_pattern0; break;
        case 1: ledbar_cur_pattern = &ledbar_pattern1; break;
        case 2: ledbar_cur_pattern = &ledbar_pattern2; break;
        case 3: ledbar_cur_pattern = &ledbar_pattern3; break;
        case 4: ledbar_cur_pattern = &ledbar_pattern4; break;
        case 5: ledbar_cur_pattern = &ledbar_pattern5; break;
        default: ledbar_cur_pattern = &ledbar_pattern0; break;
    }

    ledbar_setpins();
}

/**
 * @brief set ledbar pins based on current pattern
 * 
 * @param none
 * 
 * @return none
 */
void ledbar_setpins()
{
    LEDBAR_PORT0 = (uint8_t) (*ledbar_cur_pattern & 0x00FF);
    
    if (*ledbar_cur_pattern & 0x0100) {
        LEDBAR_PORT1 |= LEDBAR_BIT8;
    } else {
        LEDBAR_PORT1 &= ~LEDBAR_BIT8;
    }
    
    if (*ledbar_cur_pattern & 0x0200) {
        LEDBAR_PORT1 |= LEDBAR_BIT9;
    } else {
        LEDBAR_PORT1 &= ~LEDBAR_BIT9;
    }
}

/**
 * @brief update ledbar patterns based on quarter-second counter
 * 
 * @param qcnt timer quarter-second counter
 * 
 * @return none
 */
void ledbar_update(int qcnt)
{
    // pattern 0 ----------------------------------------------
    if (!(qcnt & (LEDBAR_PATTERN0_QDIV - 1)))
    {
        ledbar_pattern0 ^= 0x03FF;
    }
    // --------------------------------------------------------

    // pattern 1 ----------------------------------------------
    if (!(qcnt & (LEDBAR_PATTERN1_QDIV - 1)))
    {
        ledbar_pattern1++;
        if (ledbar_pattern1 == 0x0400) {
            ledbar_pattern1 = 0x0000;
        }
    }
    // --------------------------------------------------------
    
    // pattern 2 ----------------------------------------------
    if (!(qcnt & (LEDBAR_PATTERN2_QDIV - 1)))
    {
        ledbar_inout_cnt++;
        if (ledbar_inout_cnt == LEDBAR_INOUT_LEN) {
            ledbar_inout_cnt = 0;
        }
        ledbar_pattern2 = ledbar_inout[ledbar_inout_cnt];
    }
    // --------------------------------------------------------

    // pattern 3 ----------------------------------------------
    if (!(qcnt & (LEDBAR_PATTERN3_QDIV - 1)))
    {
        ledbar_pattern3--;
        if (ledbar_pattern3 == 0xFFFF) {
            ledbar_pattern3 = 0x00FF;
        }
    }
    // --------------------------------------------------------

    // pattern 4 ----------------------------------------------
    ledbar_pattern4_qcnt++;
    if (ledbar_pattern4_qcnt == 3) {
        ledbar_pattern4 = (ledbar_pattern4 << 1);
        if (ledbar_pattern4 == 0x0400) {
            ledbar_pattern4 = 0x0001;
        }
        ledbar_pattern4_qcnt = 0;
    }
    // --------------------------------------------------------
    
    // pattern 5 ----------------------------------------------
    if (!(qcnt & (LEDBAR_PATTERN5_QDIV - 1)))
    {
        ledbar_fillr_cnt++;
        if (ledbar_fillr_cnt == LEDBAR_FILLR_LEN) {
            ledbar_fillr_cnt = 0;
        }
        ledbar_pattern5 = ledbar_fillr[ledbar_fillr_cnt];
    }
    // --------------------------------------------------------

    ledbar_setpins();
}
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
