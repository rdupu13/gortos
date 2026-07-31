/** ---------------------------------------------------------------------------
 * 
 * GORTOS
 * timer driver
 * 
 * created by rdupu13
 * 
 * @file timer.c
 *
----------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
//  LIBRARIES
//-----------------------------------------------------------------------------

#include "drivers/timer.h"

// hardware
#include "hw/pfc.h"

// kernel
#include "kernel/gsys.h"


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------

volatile unsigned int timer_qcnt; // 4 Hz
volatile unsigned int timer_fcnt; // 256 Hz


//-----------------------------------------------------------------------------
//  FUNCTIONS
//-----------------------------------------------------------------------------

/**
 * @brief initialize timers
 * 
 * @return none
 */
void timer_init(void)
{
    // timer b0 ---------------------------------------------------------------
    TB0CTL |= TBCLR;            // clear timer and dividers
    TB0CTL |= TIMER_B0_SRC;     // set clock source
    TB0CTL |= TIMER_B0_DIV;     // set divider
    TB0CTL |= TIMER_B0_LEN;     // set length
    TB0CTL |= TIMER_B0_MODE;    // set mode
    if (TIMER_B0_INT) {
        TB0CCR0 = TIMER_B0_CCR0;    // set ccr0
        TB0CCTL0 |= CCIE;           // enable ccr0 interrupts
        TB0CCTL0 &= ~CCIFG;         // clear ccr0 interrupt flag
    } else {
        TB0CTL |= TBIE;             // enable overflow interrupts
        TB0CTL &= ~TBIFG;           // clear overflow interrupt flag
    }
    // ------------------------------------------------------------------------

    // timer b1 ---------------------------------------------------------------
    if (TIMER_B1_TOGGLE) {
        TB1CTL |= TBCLR;            // clear timer and dividers
        TB1CTL |= TIMER_B1_SRC;     // set clock source
        TB1CTL |= TIMER_B1_DIV;     // set divider
        TB1CTL |= TIMER_B1_LEN;     // set length
        TB1CTL |= TIMER_B1_MODE;    // set mode
        if (TIMER_B1_INT) {
            TB1CCR0 = TIMER_B1_CCR0;    // set ccr0
            TB1CCTL0 |= CCIE;           // enable ccr0 interrupts
            TB1CCTL0 &= ~CCIFG;         // clear ccr0 interrupt flag
        } else {
            TB1CTL |= TBIE;             // enable overflow interrupts
            TB1CTL &= ~TBIFG;           // clear overflow interrupt flag
        }
    }
    // ------------------------------------------------------------------------

    // timer b2 ---------------------------------------------------------------
    if (TIMER_B2_TOGGLE) {
        TB2CTL |= TBCLR;            // clear timer and dividers
        TB2CTL |= TIMER_B2_SRC;     // set clock source
        TB2CTL |= TIMER_B2_DIV;     // set divider
        TB2CTL |= TIMER_B2_LEN;     // set length
        TB2CTL |= TIMER_B2_MODE;    // set mode
        if (TIMER_B2_INT) {
            TB2CCR0 = TIMER_B2_CCR0;    // set ccr0
            TB2CCTL0 |= CCIE;           // enable ccr0 interrupts
            TB2CCTL0 &= ~CCIFG;         // clear ccr0 interrupt flag
        } else {
            TB2CTL |= TBIE;             // enable overflow interrupts
            TB2CTL &= ~TBIFG;           // clear overflow interrupt flag
        }
    }
    // ------------------------------------------------------------------------

    timer_qcnt = 0;
    timer_fcnt = 0;
}

/**
 * @brief stop watchdog timer
 * 
 * @return none
 */
void wdt_stop(void)
{
    WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer
}


//-----------------------------------------------------------------------------
//  INTERRUPT SERVICE ROUTINES
//-----------------------------------------------------------------------------

// TIMER B0 -------------------------------------------------------------------
#pragma vector = TIMER0_B0_VECTOR
__interrupt void isr_tb0_ccr0(void)
{
    qcnt_update(timer_qcnt);
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
// ----------------------------------------------------------------------------

#if TIMER_B1_TOGGLE // --------------------------------------------------------
#pragma vector = TIMER1_B0_VECTOR
__interrupt void isr_tb1_ccr0(void)
{
    fcnt_update(timer_fcnt);
    timer_fcnt++;
    TB1CCTL0 &= ~CCIFG; // clear interrupt flag
}

/*
#pragma vector = TIMER1_B1_VECTOR
__interrupt void isr_tb1_iv(void)
{
    switch(TB0IV)
    {
  
    
        TB1CCTL0 &= ~CCIFG; // clear interrupt flag
    }
}
*/
#endif // ---------------------------------------------------------------------

#if TIMER_B2_TOGGLE // --------------------------------------------------------
/*
#pragma vector = TIMER2_B0_VECTOR
__interrupt void isr_tb2_ccr0(void)
{
    TB2CCTL0 &= ~CCIFG; // clear interrupt flag
}
*/

#pragma vector = TIMER2_B1_VECTOR
__interrupt void isr_tb2_iv(void)
{
    switch(TB2IV)
    {
        case 0x0E:
            // overflow

            break;
        
        default: break;
    }
}
#endif // ---------------------------------------------------------------------


//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
