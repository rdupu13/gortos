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

#include <msp430fr2153.h>


//-----------------------------------------------------------------------------
//  MACROS
//-----------------------------------------------------------------------------

#define TIMER_B0_SRC    TBSSEL__ACLK    // source = aclk (32.768 kHz)
#define TIMER_B0_DIV    ID__1           // divide by 1
#define TIMER_B0_LEN    CNTL_0          // len = 16-bit
#define TIMER_B0_MODE   MC__UP          // up mode
#define TIMER_B0_INT    1               // ccr0 interrupts
#define TIMER_B0_CCR0   8192            // ccr0 = 8192 (4 Hz)

#define TIMER_B1_TOGGLE 0
#define TIMER_B1_SRC    TBSSEL__ACLK    // source = aclk (32.768 kHz)
#define TIMER_B1_DIV    ID__1           // divide by 1
#define TIMER_B1_LEN    CNTL_0          // len = 16-bit
#define TIMER_B1_MODE   MC__UP          // up mode
#define TIMER_B1_INT    1               // ccr0 interrupts
#define TIMER_B1_CCR0   256             // ccr0 = 256 (128 Hz)

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

void timer_init(); // initialize timer


#endif
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
