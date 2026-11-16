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

#include "drivers/led.h"

// hardware
#include "hw/pfc.h"


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//  FUNCTIONS
//-----------------------------------------------------------------------------

/**
 * @brief initialize leds
 * 
 * @return none
 */
void led_init(void)
{
    LED_HEARTBEAT_PORT &= ~LED_HEARTBEAT_PIN; // turn off heartbeat led
    
    led_off(0);
    led_off(1);
    led_off(2);
    
    ledbar_setpins(LEDBAR_INIT);

    LEDSTICK_PORT |= LEDSTICK_PIN;
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
 * @brief turn on a test led
 * 
 * @param n test led number
 * 
 * @return none
 */
void led_on(unsigned char n)
{
    switch(n)
    {
        case 0: LED_TEST0_PORT |= LED_TEST0_PIN; break;
        case 1: LED_TEST1_PORT |= LED_TEST1_PIN; break;
        case 2: LED_TEST2_PORT |= LED_TEST2_PIN; break;
        case 3: LED_TEST3_PORT |= LED_TEST3_PIN; break;
        default: break;
    }
}

/**
 * @brief turn off a test led
 * 
 * @param n test led number
 * 
 * @return none
 */
void led_off(unsigned char n)
{
    switch(n)
    {
        case 0: LED_TEST0_PORT &= ~LED_TEST0_PIN; break;
        case 1: LED_TEST1_PORT &= ~LED_TEST1_PIN; break;
        case 2: LED_TEST2_PORT &= ~LED_TEST2_PIN; break;
        case 3: LED_TEST3_PORT &= ~LED_TEST3_PIN; break;
        default: break;
    }
}

/**
 * @brief toggle a test led
 * 
 * @param n test led number
 * 
 * @return none
 */
void led_toggle(unsigned char n)
{
    switch(n)
    {
        case 0: LED_TEST0_PORT ^= LED_TEST0_PIN; break;
        case 1: LED_TEST1_PORT ^= LED_TEST1_PIN; break;
        case 2: LED_TEST2_PORT ^= LED_TEST2_PIN; break;
        default: break;
    }
}

/**
 * @brief set ledbar pins
 * 
 * @return none
 */
void ledbar_setpins(unsigned int n)
{
    LEDBAR_PORT0 = n & 0x00FF;
    
    if (n & 0x0100) {
        LEDBAR_PORT1 |= LEDBAR_BIT8;
    } else {
        LEDBAR_PORT1 &= ~LEDBAR_BIT8;
    }
    
    if (n & 0x0200) {
        LEDBAR_PORT1 |= LEDBAR_BIT9;
    } else {
        LEDBAR_PORT1 &= ~LEDBAR_BIT9;
    }
}
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
