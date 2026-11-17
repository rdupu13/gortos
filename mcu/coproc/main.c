
/** ---------------------------------------------------------------------------
 * 
 * MSP430FR2310 coprocessor main file
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
#include <msp430fr2310.h>


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------

#define INIT_MODE 0
#define INIT_EEP_PERIOD_MS 1000
#define DELAY_LOOP_MS 80
#define HEARTBEAT_MS 1000
#define SCAN_SPEED_MS 10
#define SLAVE_ADDR 0x48

unsigned char mode = INIT_MODE;

unsigned char row;
volatile unsigned char col;

volatile unsigned char pressed = 0;

// dial pad layout!!
volatile char row3[] = {'1', '2', '3'};
volatile char row2[] = {'4', '5', '6'};
volatile char row1[] = {'7', '8', '9'};
volatile char row0[] = {'*', '0', '#'};

char cur_char;


//-----------------------------------------------------------------------------
//  FUNCTIONS
//-----------------------------------------------------------------------------

void eep(unsigned int delay);

/**
 * @brief main for coprocessor
 * 
 * @return never!
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer
    
    // PORT ASSIGNMENTS -----------------------------------
    // heartbeat + interrupt signal:
    P1DIR |= (BIT0 | BIT1);
    P1OUT |= (BIT0 | BIT1);

    // row outputs:
    P1DIR |= (BIT4 | BIT5 | BIT6 | BIT7);
    P1OUT &= ~(BIT4 | BIT5 | BIT6 | BIT7);
    row = 0;

    // column inputs:
    P2DIR &= ~(BIT0 | BIT6 | BIT7); // inputs
    P2REN |= (BIT0 | BIT6 | BIT7); // enable resistor
    P2OUT &= ~(BIT0 | BIT6 | BIT7); // pull-down resistor
    P2IES &= ~(BIT0 | BIT6 | BIT7); // rising-edge interrupts
    P2IE |= (BIT0 | BIT6 | BIT7); // enable interrupts
    col = 0;
    
    // i2c sda & scl:
    P1SEL1 |= (BIT2 | BIT3);
    // ----------------------------------------------------
    
    PM5CTL0 &= ~LOCKLPM5; // turn off low-power mode

    eep(INIT_EEP_PERIOD_MS);
    P1OUT &= ~BIT0;

    mode = 1;
    cur_char = '\0';

    // TIMER ----------------------------------------------
    TB0CTL |= TBCLR; // clear timer and dividers
    TB0CTL &= ~MC; // stop timer
    TB0CTL |= CNTL__16; // count length = 16 (65536)
    TB0CTL |= TBSSEL__SMCLK; // source clk = 1 MHz
    TB0CTL |= ID__4; // div = 1
    TB0CTL &= ~TBIE; // disable interrupts
    // ----------------------------------------------------

    /* I2C ------------------------------------------------
    UCB0CTLW0 |= UCSWRST;
    UCB0CTLW0 |= (UCMODE_3 | UCSYNC);
    UCB0I2COA0 = UCOAEN | SLAVE_ADDR;
    UCB0CTLW0 &= ~UCSWRST;
    UCB0IE |= UCRXIE0 | UCTXIE0;
    // ----------------------------------------------------*/

    // globally enable interrupts
    __asm__ __volatile__("nop");
    __asm__ __volatile__("eint" ::: "memory");
    __asm__ __volatile__("nop");

    while (1)
    {
        if (mode == 0)
        {
            eep(HEARTBEAT_MS);
            P1OUT ^= BIT0; // toggle heartbeat led
        }
        else
        {
            while (pressed == 1) {}

            P1OUT |= BIT0; // switch heartbeat led to indicate user input

            row = 0;
            P1OUT |= BIT7;
            eep(SCAN_SPEED_MS);
            P1OUT &= ~BIT7;

            row = 1;
            P1OUT |= BIT4;
            eep(SCAN_SPEED_MS);
            P1OUT &= ~BIT4;

            row = 2;
            P1OUT |= BIT5;
            eep(SCAN_SPEED_MS);
            P1OUT &= ~BIT5;

            row = 3;
            P1OUT |= BIT6;
            eep(SCAN_SPEED_MS);
            P1OUT &= ~BIT6;
        }
        
    } // halt / loop forever
    return 0;
    // ----------------------------------------------------
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

//-----------------------------------------------------------------------------
//  INTERRUPT SERVICE ROUTINES
//-----------------------------------------------------------------------------

void __attribute__((interrupt(PORT2_VECTOR))) isr_port(void)
{
    switch(P2IV)
    {
        case 0x0E: col = 0; break;
        case 0x10: col = 1; break;
        case 0x02: col = 2; break;
        default: break;
    }

    // turn off heartbeat led
    pressed = 1;
    P1OUT &= ~BIT0;
    P1OUT &= ~BIT1;
    // start timer for heartbeat led to be off
    TB0CTL |= MC__CONTINOUS;
    TB0CTL &= ~TBIFG;
    TB0CTL |= TBIE;

    switch (row) {
        case 0: cur_char = row0[col]; break;
        case 1: cur_char = row1[col]; break;
        case 2: cur_char = row2[col]; break;
        case 3: cur_char = row3[col]; break;
        default: break;
    }
}

void __attribute__((interrupt(TIMER0_B1_VECTOR))) isr_tb0_overflow(void)
{
    TB0CTL &= ~TBIFG; // clear interrupt flag
    TB0CTL &= ~TBIE; // disable interrupts
    TB0CTL |= TBCLR; // clear timer and dividers
    TB0CTL &= ~MC; // stop timer
    
    P1OUT |= BIT0; // turn heartbeat led back on
    P1OUT |= BIT1;
    pressed = 0;
}

/*
void __attribute__((interrupt(EUSCI_B0_VECTOR))) isr_eusci_b0(void)
{
    switch(UCB0IV)
    {
        case USCI_I2C_UCTXIFG0:
            UCB0TXBUF = cur_char;
            break;
        
        default: break;
    }
}
*/
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
