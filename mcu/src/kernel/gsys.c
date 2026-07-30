/** ---------------------------------------------------------------------------
 * 
 * GORTOS
 * gort system
 * 
 * created by rdupu13
 * 
 * @file gsys.c
 *
----------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
//  LIBRARIES
//-----------------------------------------------------------------------------

#include "kernel/gsys.h"

// drivers
#include "drivers/led.h"
#include "drivers/switch.h"
#include "drivers/patterns.h"

// devices
#include "devices/rtc.h"

// kernel
#include "kernel/gio.h"


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------

unsigned char ledbar_cursel;


//-----------------------------------------------------------------------------
//  FUNCTIONS
//-----------------------------------------------------------------------------

/**
 * @brief initialize gort system
 * 
 * @param none
 * @return none
 */
void gsys_init(void)
{
    ledbar_cursel = 1;
}

/**
 * @brief take a gort nap
 * 
 * @param delay
 * 
 * @return none
 */
void eep(unsigned int delay)
{
    volatile unsigned int i;
    volatile unsigned int j;
    for (i = 0; i < delay; i++) {
        for (j = 0; j < DELAY_LOOP_MS; j++) {} // ~1 ms delay loop
    }
}

/**
 * blink the TEST0 LED for delay ms
 */
void blinky(unsigned int delay)
{
    LED_TEST0_PORT |= LED_TEST0_PIN;
    eep(delay);
    LED_TEST0_PORT &= ~LED_TEST0_PIN;
}

/**
 * @brief write current gort system time to gout
 * 
 * @param none
 * @return none
 */
void print_systime(void)
{
    char *systime;
    systime = rtc_getstr();
    helloworld(systime);
    helloworld("\n");
}

//-----------------------------------------------------------------------------
//  INTERRUPT SERVICE ROUTINES
//-----------------------------------------------------------------------------

/**
 * @brief update things (4 Hz)
 * 
 * @param 
 */
void qcnt_update(unsigned int qcnt)
{
    // heartbeat LED
    led_heartbeat_update(qcnt);
    
    // LED patterns
    patterns_update(qcnt); // advance patterns
    ledbar_sel(ledbar_cursel); // update ledbar
}

/**
 * @brief update things faster (256 Hz)
 * 
 * @param 
 */
void fcnt_update(unsigned int fcnt)
{
    
}

/**
 * @brief executes when switch 0 is pressed
 * 
 * @param none
 * @return none
 */
void switch_0_pressed(void)
{
    if (ledbar_cursel == 1)
    {
        ledbar_cursel = 2;
        LED_TEST0_PORT |= LED_TEST0_PIN;
        rtc_display_sel(0);
    }
    else
    {
        ledbar_cursel = 1;
        LED_TEST0_PORT &= ~LED_TEST0_PIN;
        patterns_sel(0);
    }
}

/**
 * @brief executes when switch 1 is pressed
 * 
 * @param none
 * @return none
 */
void switch_1_pressed(void)
{
    patterns_next();
    rtc_display_next();
}
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
