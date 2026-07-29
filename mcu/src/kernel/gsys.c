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

// drivers
#include "drivers/rtc.h"

// kernel
#include "kernel/gsys.h"

// drivers
#include "drivers/led.h"
#include "drivers/switch.h"
#include "drivers/patterns.h"
#include "drivers/rtc.h"


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------

unsigned char ledbar_cursel;

unsigned char switch_hist[2];


//-----------------------------------------------------------------------------
//  FUNCTIONS
//-----------------------------------------------------------------------------

/**
 * 
 */
void gsys_init()
{
    ledbar_cursel = 1;
    switch_hist[0] = 0;
    switch_hist[1] = 0;
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
 * @brief write gort system time to gout
 *
void print_systime()
{
    char *systime;
    systime = rtc_getstr();
    helloworld(systime);
}
*/

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
    patterns_update(qcnt);
    ledbar_sel(ledbar_cursel);
}

/**
 * @brief update things faster (256 Hz)
 * 
 * @param 
 */
void fcnt_update(unsigned int fcnt)
{
    /*
    switch_hist[0] = (switch_hist[0] << 1) | switch_poll(0);
    if (switch_hist[0] == 0xFF) { switch_0_pressed(); }
    
    switch_hist[1] = (switch_hist[1] << 1) | switch_poll(1);
    if (switch_hist[1] == 0xFF) { switch_1_pressed(); }
    */
}

/**
 * @brief executes when switch 0 is pressed
 * 
 * @param none
 * @return none
 */
void switch_0_pressed()
{
    switch(ledbar_cursel)
    {
        case 1:
            ledbar_cursel = 2;
            LED_TEST0_PORT |= LED_TEST0_PIN;
            patterns_sel(0);
            break;
        
        case 2:
            ledbar_cursel = 1;
            LED_TEST0_PORT &= ~LED_TEST0_PIN;
            rtc_display_sel(0);
            break;

        default:
            ledbar_cursel = 1;
            break;
    }
}

/**
 * @brief executes when switch 1 is pressed
 * 
 * @param none
 * @return none
 */
void switch_1_pressed()
{
    patterns_next();
    rtc_display_next();
}
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
