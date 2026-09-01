/** ---------------------------------------------------------------------------
 * 
 * GORTOS
 * timer driver library
 * 
 * created by rdupu13
 * 
 * @file timer.h
 *
----------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
//  HEADER DEFINITION
//-----------------------------------------------------------------------------

#ifndef TIMER_H
#define TIMER_H


//-----------------------------------------------------------------------------
//  LIBRARIES
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//  MACROS
//-----------------------------------------------------------------------------

#define TIMER_B1_TOGGLE 0
#define TIMER_B2_TOGGLE 0

#define TIMER_B0_INT    1               // ccr0 interrupts
#define TIMER_B0_CCR0   8192            // ccr0 = 8192 (4 Hz)

#define TIMER_B1_INT    1               // ccr0 interrupts
#define TIMER_B1_CCR0   256             // ccr0 = 256 (128 Hz)

#define TIMER_B2_INT    0               // overflow interrupts
#define TIMER_B2_CCR0   128             // ccr0 = 128 (256 Hz)


//-----------------------------------------------------------------------------
//  FUNCTION PROTOTYPES
//-----------------------------------------------------------------------------

extern volatile unsigned int timer_qcnt;

void timer_init(void); // initialize timers

void wdt_stop(void); // stop watchdog timer
void timer_ccr_set(
    unsigned char timer,
    unsigned char ccr,
    unsigned int val
);

#endif
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
