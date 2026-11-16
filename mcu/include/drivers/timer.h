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

// led pattern + heartbeat timer, always on
// #define TIMER_B0_TOGGLE 1
#define TIMER_B0_SRC    TBSSEL__ACLK    // source = aclk (32.768 kHz)
#define TIMER_B0_DIV    ID__1           // divide by 1
#define TIMER_B0_LEN    CNTL_0          // len = 16-bit
#define TIMER_B0_MODE   MC__UP          // up mode
#define TIMER_B0_INT    1               // ccr0 interrupts
#define TIMER_B0_CCR0   8192            // ccr0 = 8192 (4 Hz)

// rtc + lcd + dial + rot timer
#define TIMER_B1_TOGGLE 1
#define TIMER_B1_SRC    TBSSEL__ACLK    // source = aclk (32.768 kHz)
#define TIMER_B1_DIV    ID__1           // divide by 1
#define TIMER_B1_LEN    CNTL_0          // len = 16-bit
#define TIMER_B1_MODE   MC__UP          // up mode
#define TIMER_B1_INT    1               // ccr0 interrupts
#define TIMER_B1_CCR0   4096            // ccr0 = 4096 (8 Hz)

// not used currently
#define TIMER_B2_TOGGLE 0
#define TIMER_B2_SRC    TBSSEL__ACLK    // source = aclk (32.768 kHz)
#define TIMER_B2_DIV    ID__1           // divide by 1
#define TIMER_B2_LEN    CNTL_0          // len = 16-bit
#define TIMER_B2_MODE   MC__CONTINOUS   // continuous mode
#define TIMER_B2_INT    0               // overflow interrupts
#define TIMER_B2_CCR0   128             // ccr0 = 128 (256 Hz)


//-----------------------------------------------------------------------------
//  FUNCTION PROTOTYPES
//-----------------------------------------------------------------------------

extern volatile unsigned int timer_qcnt;
extern volatile unsigned int timer_fcnt;

void timer_init(void); // initialize timers
void wdt_stop(void); // stop watchdog timer

// set a capture-compare register in a timer
void timer_ccr_set(
    unsigned char timer,
    unsigned char ccr,
    unsigned int val
);

#endif
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
