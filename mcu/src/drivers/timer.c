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

volatile unsigned int timer_qcnt; // variable
volatile unsigned int timer_fcnt; // 8 Hz


//-----------------------------------------------------------------------------
//  FUNCTIONS
//-----------------------------------------------------------------------------

void init_clocks(void);

/**
 * @brief initialize timers
 * 
 * @return none
 */
void timer_init(void)
{
    wdt_stop(); // stop watchdog timer
    
    init_clocks();
    
    // timer b0 ---------------------------------------------------------------
    TB0CTL |= TBCLR;            // clear timer and dividers
    TB0CTL |= TIMER_B0_SRC;     // set clock source
    TB0CTL |= TIMER_B0_DIV;     // set divider
    TB0CTL |= TIMER_B0_LEN;     // set length
    TB0CTL |= TIMER_B0_MODE;    // set mode
    #if TIMER_B0_INT
        // ccr0 interrupts
        timer_ccr_set(0, 0, TIMER_B0_CCR0);    // set ccr0
        TB0CCTL0 &= ~CCIFG;         // clear ccr0 interrupt flag
        TB0CCTL0 |= CCIE;           // enable ccr0 interrupts
    #else
        // overflow interrupts
        TB0CTL &= ~TBIFG;           // clear overflow interrupt flag
        TB0CTL |= TBIE;             // enable overflow interrupts
    #endif
    // ------------------------------------------------------------------------

    // timer b1 ---------------------------------------------------------------
    TB1CTL |= TBCLR; // clear timer and dividers
    #if TIMER_B1_TOGGLE
        TB1CTL |= TIMER_B1_SRC;     // set clock source
        TB1CTL |= TIMER_B1_DIV;     // set divider
        TB1CTL |= TIMER_B1_LEN;     // set length
        TB1CTL |= TIMER_B1_MODE;    // set mode
        #if TIMER_B1_INT
            // ccr0 interrupts
            timer_ccr_set(1, 0, TIMER_B1_CCR0);    // set ccr0
            TB1CCTL0 &= ~CCIFG;         // clear ccr0 interrupt flag
            TB1CCTL0 |= CCIE;           // enable ccr0 interrupts
        #else
            // overflow interrupts
            TB1CTL &= ~TBIFG;           // clear overflow interrupt flag
            TB1CTL |= TBIE;             // enable overflow interrupts
        #endif
    #else
        TB1CCTL0 &= ~CCIFG; // clear ccr0 interrupt flag
        TB1CCTL0 &= ~CCIE;  // disable ccr0 interrupts
        TB1CTL &= ~TBIFG;   // clear overflow interrupt flag
        TB1CTL &= ~TBIE;    // disable overflow interrupts
    #endif
    // ------------------------------------------------------------------------

    // timer b2 ---------------------------------------------------------------
    TB2CTL |= TBCLR; // clear timer and dividers
    #if TIMER_B2_TOGGLE
        TB2CTL |= TIMER_B2_SRC;     // set clock source
        TB2CTL |= TIMER_B2_DIV;     // set divider
        TB2CTL |= TIMER_B2_LEN;     // set length
        TB2CTL |= TIMER_B2_MODE;    // set mode
        #if TIMER_B2_INT
            // ccr0 interrupts
            TB2CCR0 = TIMER_B2_CCR0;    // set ccr0
            TB2CCTL0 &= ~CCIFG;         // clear ccr0 interrupt flag
            TB2CCTL0 |= CCIE;           // enable ccr0 interrupts
        #else
            // overflow interrupts
            TB2CTL &= ~TBIFG;           // clear overflow interrupt flag
            TB2CTL |= TBIE;             // enable overflow interrupts
        #endif
    #else
        TB2CCTL0 &= ~CCIFG; // clear ccr0 interrupt flag
        TB2CCTL0 &= ~CCIE;  // disable ccr0 interrupts
        TB2CTL &= ~TBIFG;   // clear overflow interrupt flag
        TB2CTL &= ~TBIE;    // disable overflow interrupts
    #endif
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

/**
 * @brief set a capture-compare register in a timer
 * 
 * @param timer timer b #
 * @param ccr capture-compare register #
 * @param val value to set register to
 * 
 * @return none
 */
void timer_ccr_set(
    unsigned char timer,
    unsigned char ccr,
    unsigned int val
) {
    switch(timer)
    {
        case 0:
            switch(ccr)
            {
                case 0: TB0CCR0 = val; break;
                default: break;
            }
            break;
        case 1:
            switch(ccr)
            {
                case 0: TB1CCR0 = val; break;
                default: break;
            }
            break;
        default: break;
    }
}

void init_clocks(void) {
    // FRAM needs extra wait states once MCLK goes above 8MHz.
    // FRCTLPW confirmed in your header. NWAITS_1 is the commonly-required
    // value above 8MHz on this family -- if timing looks off, check your
    // datasheet's FRAM access-time table for the exact count at 16MHz.
    FRCTL0 = FRCTLPW | NWAITS_1;

    // CSCTL1 holds DCORSEL -- confirmed directly from your header.
    // No CSKEY/password unlock exists on this part, so no unlock step needed.
    CSCTL1 = DCORSEL_5;              // DCO = 16 MHz

    // CSCTL2: SELA (ACLK source, high byte) + SELMS (MCLK/SMCLK source, low byte)
    // both confirmed to exist in your header; this packing (both fields in
    // one word) is inferred from their bit positions not overlapping, not
    // directly confirmed line-by-line.
    CSCTL2 = SELA__VLOCLK | SELMS__DCOCLKDIV;

    // CSCTL3: your header confirmed DIVS__1 (SMCLK divider) exists.
    // MCLK/ACLK dividers (DIVM/DIVA) likely live in the same register --
    // uncomment/add them if your compiler errors on this line missing them.
    CSCTL3 = DIVS__1;
}

//-----------------------------------------------------------------------------
//  INTERRUPT SERVICE ROUTINES
//-----------------------------------------------------------------------------

// TIMER B0 -------------------------------------------------------------------
void __attribute__((interrupt(TIMER0_B0_VECTOR))) isr_tb0_ccr0(void)
{
    qcnt_update(timer_qcnt);
    timer_qcnt++;
    TB0CCTL0 &= ~CCIFG; // clear interrupt flag
}
// ----------------------------------------------------------------------------

#if TIMER_B1_TOGGLE // --------------------------------------------------------
void __attribute__((interrupt(TIMER1_B0_VECTOR))) isr_tb1_ccr0(void)
{
    //fcnt_update(timer_fcnt);
    timer_fcnt++;
    TB1CCTL0 &= ~CCIFG; // clear interrupt flag
}
#endif // ---------------------------------------------------------------------

#if TIMER_B2_TOGGLE // --------------------------------------------------------

#endif // ---------------------------------------------------------------------


//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
