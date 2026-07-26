#include <msp430fr2153.h>
#include "led.h"
#include "timer.h"
#include "uart.h"
#include "i2c.h"
#include "gio.h"

void timer_init()
{
    TB0CTL |= TBCLR;            // clear timer and dividers
    TB0CTL |= TIMER_B0_SRC;     // set clock source
    TB0CTL |= TIMER_B0_DIV;     // set divider
    TB0CTL |= TIMER_B0_LEN;     // set length
    TB0CTL |= TIMER_B0_MODE;    // set mode
    
    if (TIMER_B0_INT)
    {
        TB0CCR0 = TIMER_B0_CCR0;    // set ccr0
        TB0CCTL0 |= CCIE;           // enable ccr0 interrupts
        TB0CCTL0 &= ~CCIFG;         // clear ccr0 interrupt flag
    }
    else
    {
        TB0CTL |= TBIE;             // enable overflow interrupts
        TB0CTL &= ~TBIFG;           // clear overflow interrupt flag
    }

    timer_qcnt = 0;
}

#pragma vector = TIMER0_B0_VECTOR
__interrupt void isr_tb0_ccr0(void)
{
    led_heartbeat_update(timer_qcnt);
    ledbar_update(timer_qcnt);
    
    timer_qcnt++;
    
    TB0CCTL0 &= ~CCIFG; // clear interrupt flag
}

/*
#pragma vector = TIMER0_B1_VECTOR
__interrupt void isr_tb0_iv(void)
{
    switch(TB0IV)
    {
  
    
        TB0CCTL0 &= ~CCIFG; // clear interrupt flag
    }
}
*/
