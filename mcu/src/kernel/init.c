/** ---------------------------------------------------------------------------
 * 
 * GORTOS
 * gort system initialization sequence
 * 
 * created by rdupu13
 * 
 * @file init.c
 *
----------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
//  LIBRARIES
//-----------------------------------------------------------------------------

#include <msp430fr2153.h>

#include "kernel/init.h"

// drivers
#include "drivers/timer.h"
#include "drivers/uart.h"
#include "drivers/i2c.h"
#include "drivers/led.h"
#include "drivers/switch.h"
#include "drivers/rtc.h"

// kernel
#include "kernel/gio.h"
#include "kernel/gsys.h"


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//  FUNCTIONS
//-----------------------------------------------------------------------------

/**
 * @brief initialize gort system
 * 
 * @param none
 * @return none
 */
void init()
{
    WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer
    
    // initalize ports to outputs as default
    P1SEL0 = 0x00; P2SEL0 = 0x00; P3SEL0 = 0x00; P4SEL0 = 0x00; P5SEL0 = 0x00;
    P1SEL1 = 0x00; P2SEL1 = 0x00; P3SEL1 = 0x00; P4SEL1 = 0x00; P5SEL1 = 0x00;
    P1DIR = 0xFF;  P2DIR = 0xFF;  P3DIR = 0xFF;  P4DIR = 0xFF;  P5DIR = 0x03;
    P1OUT = 0x00;  P2OUT = 0x00;  P3OUT = 0x00;  P4OUT = 0x00;  P5OUT = 0x00;
    
    PM5CTL0 &= ~LOCKLPM5; // turn off low-power mode

    LED_HEARTBEAT_PORT |= LED_HEARTBEAT_PIN; // show signs of life

    // initialize core gort system drivers
    // timer config -------------------
    timer_init();
    // uart config --------------------
    uart_init();
    // i2c config ---------------------
    i2c_init();
    // --------------------------------
    
    eep(INIT_EEP_PERIOD_MS); // epp for a lil to let clockies warm up
    
    __enable_interrupt(); // globally enable interrupts

    // intialize more gort system drivers
    // led config ---------------------
    led_init();
    // switch config ------------------
    //switch_init();
    // --------------------------------

    // rtc config ---------------------
    rtc_init();
    // --------------------------------

    // TODO: make GOUT and GIN variable, set GOUT = 0, GIN = 0
    glear(); // clear GOUT
}
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
