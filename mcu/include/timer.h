#ifndef TIMER_H
#define TIMER_H

#include <msp430fr2153.h>

#define TIMER_B0_SRC    TBSSEL__ACLK    // source = aclk (32.768 kHz)
#define TIMER_B0_DIV    ID__1           // divide by 1
#define TIMER_B0_LEN    CNTL_0          // 16-bit
#define TIMER_B0_MODE   MC__UP          // up mode
#define TIMER_B0_INT    1               // ccr0 interrupts
#define TIMER_B0_CCR0   8192            // ccr0 = 8192 (4 Hz)

volatile unsigned int timer_qcnt;

void timer_init(); // initialize timer

#endif
