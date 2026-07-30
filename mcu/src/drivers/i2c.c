/** ---------------------------------------------------------------------------
 * 
 * GORTOS
 * i2c driver
 * 
 * created by rdupu13
 * 
 * @file i2c.c
 *
----------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
//  LIBRARIES
//-----------------------------------------------------------------------------

#include <msp430fr2153.h>
#include "drivers/i2c.h"


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------

volatile unsigned char *i2c_tx_buf_ptr;
volatile unsigned char *i2c_rx_buf_ptr;
volatile unsigned int i2c_cnt;
volatile unsigned char i2c_busy;

volatile unsigned int i2c_len;
volatile unsigned char i2c_reg_addr;
volatile unsigned char i2c_mode;


//-----------------------------------------------------------------------------
//  FUNCTIONS
//-----------------------------------------------------------------------------

/**
 * @brief initialize i2c
 * 
 * @param none
 * @return none
 */
void i2c_init(void)
{
    I2C_SEL0 |= I2C_PINS; // configure pins
    
    // setup peripheral
    UCB1CTLW0 |= UCSWRST;       // put peripheral into software reset
    UCB1CTLW0 |= UCSSEL__SMCLK; // clock source = smclk (1 MHz)
    UCB1BRW = 10;               // divide clock by 10 (100 kHz)
    UCB1CTLW0 |= UCMODE_3;      // i2c mode
    UCB1CTLW0 |= UCMST;         // master mode
    UCB1CTLW0 &= ~UCA10;        // addressing mode = 7-bit
    UCB1CTLW0 &= ~UCSLA10;      // slave addressing mode = 7-bit
    UCB1CTLW0 &= ~UCSWRST;      // take peripheral out of software reset

    // setup interrupts
    UCB1IE |= UCNACKIE;                 // enable no acknowledge interrupt
    UCB1IFG &= ~UCNACKIFG;              // clear no acknowledge interrupt flag
    UCB1IE &= ~(UCTXIE0 | UCRXIE0);     // disable tx/rx interrupts
    UCB1IFG &= ~(UCTXIFG0 | UCRXIFG0);  // clear tx/rx interrupt flags

    // initialize variables
    i2c_tx_buf_ptr = 0;
    i2c_rx_buf_ptr = 0;
    i2c_cnt = 0;
    i2c_busy = 0; I2C_BUSY_PORT &= ~I2C_BUSY_PIN;
    i2c_len = 0;
    i2c_reg_addr = 0;
    i2c_mode = 0;
}

/**
 * @brief write an array to an i2c slave
 * 
 * @param slave_addr    slave address
 * @param reg_addr      slave register address to start writing at
 * @param len           length in bytes of array to be written
 * @param arr           pointer to array to be written
 * 
 * @return none
 */
void i2c_write(
    unsigned int slave_addr,
    unsigned char reg_addr,
    unsigned int len,
    unsigned char *arr
) {
    if (len == 0) { return; }

    if (i2c_busy) { return; }
    i2c_busy = 1; I2C_BUSY_PORT |= I2C_BUSY_PIN; // busy

    // TODO: perhaps disable timer interrupts

    UCB1I2CSA = slave_addr;
    i2c_reg_addr = reg_addr;
    i2c_cnt = len + 1;
    i2c_tx_buf_ptr = arr;
    
    i2c_len = len;
    i2c_mode = 0; // write mode

    UCB1CTLW0 |= UCTR; // put peripheral into tx mode
    UCB1IE |= UCTXIE0; // enable tx complete interrupts
    UCB1CTLW0 |= UCTXSTT; // send start + send slave addr

    // TODO: add timeout (tx didn't work for some reason)
    while (i2c_busy) {} // wait until tx done
}

/**
 * @brief read an array from an i2c slave
 * 
 * @param slave_addr    slave address
 * @param reg_addr      slave register address to start reading from
 * @param len           length in bytes of array to be read
 * @param arr           pointer to array to store received data
 * 
 * @return none
 */
void i2c_read(
    unsigned int slave_addr,
    unsigned char reg_addr,
    unsigned int len,
    unsigned char *arr
) {
    if (len == 0) { return; }

    if (i2c_busy) { return; }
    i2c_busy = 1; I2C_BUSY_PORT |= I2C_BUSY_PIN; // busy

    // TODO: perhaps disable timer interrupts

    UCB1I2CSA = slave_addr;
    i2c_reg_addr = reg_addr;
    i2c_cnt = len + 1;
    i2c_rx_buf_ptr = arr;

    i2c_len = len;
    i2c_mode = 1; // read mode

    UCB1CTLW0 |= UCTR; // put peripheral into tx mode
    UCB1IE |= UCTXIE0; // enable tx complete interrupts
    UCB1CTLW0 |= UCTXSTT; // send start + slave addr

    // TODO: add timeout
    while (i2c_busy) {} // wait until rx done
}


//-----------------------------------------------------------------------------
//  INTERRUPT SERVICE ROUTINES
//-----------------------------------------------------------------------------

#pragma vector = EUSCI_B1_VECTOR
__interrupt void isr_eusci_b1(void)
{
    switch(__even_in_range(UCB1IV, 0x1E))
    {
        case 0x00: break; // no interrupts
        case 0x02: break; // ALIFG (arbitration lost)
        case 0x04:
            // NACKIFG (no acknowledge received)
            UCB1IE &= ~(UCTXIE0 | UCRXIE0); // disable tx/rx interrupts
            UCB1IFG &= ~(UCTXIFG0 | UCRXIFG0); // clear tx/rx interrupt flags
            UCB1CTLW0 |= UCTXSTP; // send stop condition
            i2c_busy = 0; I2C_BUSY_PORT &= ~I2C_BUSY_PIN; // not busy
            break;
        
        case 0x06: break; // STTIFG
        case 0x08: break; // STPIFG
        case 0x16:
            // RXIFG0 (rx buffer full)
            *i2c_rx_buf_ptr++ = UCB1RXBUF; // store received byte
            i2c_cnt--; // decrement counter

            if (i2c_cnt == 1)
            {
                UCB1CTLW0 |= UCTXSTP; // send stop condition after last byte
            }
            else if (i2c_cnt == 0)
            {
                UCB1IE &= ~UCRXIE0; // disable rx buffer full interrupts
                i2c_busy = 0; I2C_BUSY_PORT &= ~I2C_BUSY_PIN; // not busy
            }
            break;

        case 0x18:
            // TXIFG0 (tx complete)
            if (i2c_mode)
            {
                // read mode
                if (i2c_cnt > i2c_len)
                {
                    UCB1TXBUF = i2c_reg_addr; // send register address
                    i2c_cnt--; // decrement counter
                }
                else
                {
                    // transition to reading from slave:
                    UCB1CTLW0 &= ~UCTR; // put peripheral into rx mode
                    UCB1IE &= ~UCTXIE0; // disable tx complete interrupts
                    UCB1IE |= UCRXIE0; // enable rx buffer full interrupts
                    UCB1IFG &= ~UCRXIFG; // clear rx buffer full interrupt flag
                    UCB1CTLW0 |= UCTXSTT; // send repeated start + slave addr
                    
                    // stupid fucking special case for 1-byte reads
                    if (i2c_len == 1)
                    {
                        while (UCB1CTLW0 & UCTXSTT); // wait for repeated start
                        UCB1CTLW0 |= UCTXSTP;        // prepare to stop early
                    }
                }
            }
            else
            {
                // write mode
                if (i2c_cnt == 0)
                {
                    UCB1IE &= ~UCTXIE0; // disable tx complete interrupts
                    UCB1CTLW0 |= UCTXSTP; // send stop condition
                    i2c_busy = 0; I2C_BUSY_PORT &= ~I2C_BUSY_PIN; // not busy
                }
                else if (i2c_cnt > i2c_len)
                {
                    UCB1TXBUF = i2c_reg_addr; // send register address
                    i2c_cnt--; // decrement counter
                }
                else
                {
                    UCB1TXBUF = *i2c_tx_buf_ptr++; // send next byte
                    i2c_cnt--; // decrement counter
                }
            }
            break;

        default: break;
    }
}
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
