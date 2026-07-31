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

// devices
#include "devices/patterns.h"
#include "devices/pwm.h"
#include "devices/rtc.h"
#include "devices/uart.h"

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
 * @param none
 * @return none
 */
void gsys_init(void)
{
    wdt_stop(); // stop watchdog timer
    
    // initalize ports to outputs by default:
    P1SEL0 = 0x00; P2SEL0 = 0x00; P3SEL0 = 0x00; P4SEL0 = 0x00; P5SEL0 = 0x00;
    P1SEL1 = 0x00; P2SEL1 = 0x00; P3SEL1 = 0x00; P4SEL1 = 0x00; P5SEL1 = 0x00;
    P1DIR  = 0xFF; P2DIR  = 0xFF; P3DIR  = 0xFF; P4DIR  = 0xFF; P5DIR  = 0xFF;
    P1OUT  = 0x00; P2OUT  = 0x00; P3OUT  = 0x00; P4OUT  = 0x00; P5OUT  = 0x00;

    // make compiler stfu (nvm it breaks the leds for some reason):
    //P6SEL0 = 0x00; PASEL0 = 0x00; PBSEL0 = 0x00; PCSEL0 = 0x00;
    //P6SEL1 = 0x00; PASEL1 = 0x00; PBSEL1 = 0x00; PCSEL1 = 0x00;
    //P6DIR  = 0xFF; PADIR  = 0xFF;  PBDIR = 0xFF; PCDIR  = 0xFF;
    //P6OUT  = 0x00; PAOUT  = 0x00;  PBOUT = 0x00; PCOUT  = 0x00;
    
    PM5CTL0 &= ~LOCKLPM5; // turn off low-power mode

    LED_HEARTBEAT_PORT |= LED_HEARTBEAT_PIN; // show signs of life

    // DRIVERS --------------------------------------------
    // initialize core gort system drivers
    timer_init();
    uart_init();
    i2c_init();
    spi_init();
    adc_init();
    
    eep(INIT_EEP_PERIOD_MS); // eep for a lil to let clockies warm up
    
    __enable_interrupt(); // globally enable interrupts

    // intialize more gort system drivers
    patterns_init();
    led_init();
    switch_init();
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
    helloworld("Current time: ");
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
 * @param none
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
