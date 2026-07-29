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

// drivers
// TODO: make led application at kernel level, led shouldn't need other drivers
#include "drivers/patterns.h"
#include "drivers/rtc.h"


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------

volatile uint16_t *ledbar_cur;


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

    ledbar_sel(LEDBAR_INIT_SEL);
}

/**
 * @brief update heartbeat led based on quarter-second counter
 * 
 * @param qcnt timer quarter-second counter
 * 
 * @return none
 */
void led_heartbeat_update(unsigned int qcnt)
{
    if (!(qcnt & (LED_HEARTBEAT_QDIV - 1))) {
        LED_HEARTBEAT_PORT ^= LED_HEARTBEAT_PIN; // toggle heartbeat led
    }
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
    LEDBAR_PORT0 = (uint8_t) (*ledbar_cur & 0x00FF);
    
    if (*ledbar_cur & 0x0100) {
        LEDBAR_PORT1 |= LEDBAR_BIT8;
    } else {
        LEDBAR_PORT1 &= ~LEDBAR_BIT8;
    }
    
    if (*ledbar_cur & 0x0200) {
        LEDBAR_PORT1 |= LEDBAR_BIT9;
    } else {
        LEDBAR_PORT1 &= ~LEDBAR_BIT9;
    }
}

/**
 * @brief select ledbar pointer
 * 
 * @param sel selection
 * 
 * @return none
 */
void ledbar_sel(uint8_t sel)
{
    unsigned int rtc_display_ext;
    switch(sel)
    {
        case 1:
            ledbar_cur = cur_pattern;
            break;

        case 2:
            rtc_display_ext = (unsigned int) (*rtc_display);
            ledbar_cur = &rtc_display_ext;
            break;

        default:
            ledbar_cur = cur_pattern;
            patterns_sel(-1);
            break;
    }
    
    ledbar_setpins();
}
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
