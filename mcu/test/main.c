
/** ---------------------------------------------------------------------------
 * 
 * MSP430 test main file
 * 
 * created by rdupu13
 * 
 * @file main.c
 *
----------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
//  LIBRARIES
//-----------------------------------------------------------------------------

// hardware
#include <msp430fr2153.h>


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------

char packet[] = {0x05, 0x00};

volatile unsigned int position;

//-----------------------------------------------------------------------------
//  FUNCTIONS
//-----------------------------------------------------------------------------

/**
 * @brief main test
 * 
 * @return never!
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer

    PM5CTL0 &= ~LOCKLPM5; // turn off low-power mode

    // TEST CODE HERE -------------------------------------
    UCA0CTLW0 |= UCSWRST;
    UCA0CTLW0 |= UCSSEL__SMCLK;
    UCA0BRW = 10;
    UCA0CTLW0 |= UCSYNC;
    UCA0CTLW0 |= UCMST;
    UCA0CTLW0 &= ~UCMODE0;
    UCA0CTLW0 |= UCMODE1;
    UCA0CTLW0 |= UCSTEM;
    UCA0CTLW0 |= UCMSB;
    UCA0CTLW0 &= ~UCSWRST;
    
    P1SEL1 &= ~BIT4;
    P1SEL0 |= BIT4;
    P1SEL1 &= ~BIT5;
    P1SEL0 |= BIT5;
    P1SEL1 &= ~BIT6;
    P1SEL0 |= BIT6;
    P1SEL1 &= ~BIT7;
    P1SEL0 |= BIT7;

    UCA0IFG &= ~UCTXIFG;
    UCA0IE |= UCTXIE;

    __asm__ __volatile__("nop");
    __asm__ __volatile__("eint" ::: "memory");
    __asm__ __volatile__("nop");

    volatile unsigned int i;
    while (1)
    {
        position = 0;
        UCA0TXBUF = packet[position];

        for (i = 0; i < 10000; i++) {}
    }
    return 0;
    // ----------------------------------------------------
}


//-----------------------------------------------------------------------------
//  INTERRUPT SERVICE ROUTINES
//-----------------------------------------------------------------------------

void __attribute__((interrupt(EUSCI_A0_VECTOR))) isr_spi(void)
{
    position++;

    if (position < sizeof(packet)) {
        UCA0TXBUF = packet[position];
    } else {
        UCA0IFG &= ~UCTXIFG;
    }
}

//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
