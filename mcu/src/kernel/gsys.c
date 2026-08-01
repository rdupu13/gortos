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

// hardware
#include "hw/pfc.h"

// drivers
#include "drivers/adc.h"
#include "drivers/i2c.h"
#include "drivers/led.h"
#include "drivers/spi.h"
#include "drivers/switch.h"
#include "drivers/timer.h"
#include "drivers/uart.h"

// devices
//#include "devices/dial.h"
//#include "devices/lcd.h"
//#include "devices/lora.h"
//#include "devices/mmm.h"
#include "devices/patterns.h"
//#include "devices/pwm.h"
#include "devices/rtc.h"

// kernel
#include "kernel/gfs.h"
#include "kernel/gio.h"
#include "kernel/gstr.h"


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------

volatile unsigned char display_mode;


//-----------------------------------------------------------------------------
//  FUNCTIONS
//-----------------------------------------------------------------------------

/**
 * @brief initialize gort system
 * 
 * @return none
 */
void gsys_init(void)
{
    wdt_stop(); // stop watchdog timer
    
    pfc_init(); // initialize ports

    LED_HEARTBEAT_PORT |= LED_HEARTBEAT_PIN; // show signs of life

    // DRIVERS --------------------------------------------
    // initialize core gort system drivers
    timer_init();
    uart_init(96, 1); // 9600 baud, echo enabled
    i2c_init(60000); // timeout = 60000
    spi_init(60000); // timeout = 60000
    adc_init();
    
    eep(INIT_EEP_PERIOD_MS); // eep for a lil to let clockies warm up
    
    __enable_interrupt(); // globally enable interrupts

    // intialize more gort system drivers
    led_init();
    switch_init();
    // ----------------------------------------------------

    // DEVICES --------------------------------------------
    patterns_init();
    rtc_init();
    // ----------------------------------------------------

    // KERNEL ---------------------------------------------
    //gfs_init();
    
    display_mode = 1;

    // gin & gout = uart
    gin = 0;
    gout = 0;
    // ----------------------------------------------------

    // print start message
    glear();
    helloworld("~~~ Gort OS ~~~\n");
    helloworld("(c) rdupu13 2026\n\n");
    print_systime();
    helloworld("\n");
}

/**
 * @brief take a gort nap
 * 
 * @param delay duration of eep in ms
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

// rtc ----------------------------------------------------
/**
 * @brief write current gort system time to gout
 * 
 * @return none
 */
void print_systime(void)
{
    char *systime;
    systime = rtc_getstr();

    helloworld("Current time: ");
    helloworld(systime);
    helloworld("\n");
}
// --------------------------------------------------------

// led ----------------------------------------------------
/**
 * @brief blink a test led
 * 
 * @param led   test led number
 * @param delay duration of blink in ms
 * 
 * @return none
 */
void blinky(unsigned char led, unsigned int delay)
{
    led_test_on(led);
    eep(delay);
    led_test_off(led);
}

/**
 * @brief select current thing displayed on ledbar
 * 
 * @param sel selection
 * 
 * @return none
 */
void ledbar_sel(unsigned char sel)
{
    unsigned int n;
    switch(sel)
    {
        case 1: n = *cur_pattern; break;
        case 2: n = (unsigned int) *rtc_display; break;
        default: n = *cur_pattern; patterns_sel(-1); break;
    }

    ledbar_setpins(n);
}
// --------------------------------------------------------


//-----------------------------------------------------------------------------
//  INTERRUPT SERVICE ROUTINES
//-----------------------------------------------------------------------------

/**
 * @brief update things (4 Hz)
 * 
 * @param qcnt timer quarter-second counter
 * 
 * @return none
 */
void qcnt_update(unsigned int qcnt)
{
    // update heartbeat led
    led_heartbeat_update(qcnt);
    
    // update patterns
    patterns_update(qcnt);

    // update ledbar
    ledbar_sel(display_mode);
}

/**
 * @brief update things faster (256 Hz)
 * 
 * @param fcnt timer "fast" counter
 * 
 * @return none
 */
void fcnt_update(unsigned int fcnt)
{
    
}

/**
 * @brief executes when switch 0 is pressed
 * 
 * @return none
 */
void switch_0_pressed(void)
{
    if (display_mode == 1)
    {
        display_mode = 2;
        led_test_on(0);
        rtc_display_sel(0);
    }
    else
    {
        display_mode = 1;
        led_test_off(0);
        patterns_sel(0);
    }
}

/**
 * @brief executes when switch 1 is pressed
 * 
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
