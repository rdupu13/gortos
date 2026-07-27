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
#include <stdint.h>

#include "drivers/i2c.h"


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//  FUNCTIONS
//-----------------------------------------------------------------------------

/**
 * @brief initialize i2c
 * 
 * @param none
 * @return none
 */
void i2c_init()
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
    i2c_len = 0;
    i2c_mode = 0;
    i2c_busy = 0;
    I2C_BUSY_PORT &= ~I2C_BUSY_PIN;
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
void i2c_write(uint16_t slave_addr, uint8_t reg_addr, uint8_t len, uint8_t *arr)
{
    if (i2c_busy) { return; } // do nothing if i2c busy
    i2c_busy = 1;
    I2C_BUSY_PORT |= I2C_BUSY_PIN;

    // TODO: make len an int
    // TODO: check len

    UCB1I2CSA = slave_addr;
    i2c_tx_buf[0] = reg_addr;
    i2c_len = len + 1;
    
    // copy array of data into i2c_tx_buf after reg_addr
    uint8_t i;
    for (i = 0; i < len; i++)
    {
        i2c_tx_buf[i + 1] = arr[i];
    }

    i2c_tx_buf_ptr = i2c_tx_buf;
    i2c_mode = 0; // write mode
    UCB1CTLW0 |= UCTR; // put peripheral into tx mode
    UCB1IE |= UCTXIE0; // enable tx complete interrupts
    UCB1CTLW0 |= UCTXSTT; // send start + send slave addr

    while (i2c_busy) {} // wait until tx done
}

/**
 * @brief read an array from an i2c slave
 * 
 * @param slave_addr    slave address
 * @param reg_addr      slave register address to start reading from
 * @param len           length in bytes of array to be read
 * 
 * @return pointer to receive buffer
 */
uint8_t *i2c_read(uint16_t slave_addr, uint8_t reg_addr, uint8_t len)
{
    if (i2c_busy) { return (uint8_t *) 0; } // do nothing if i2c busy
    i2c_busy = 1;
    I2C_BUSY_PORT |= I2C_BUSY_PIN;

    // TODO: make len an int
    // TODO: check len

    UCB1I2CSA = slave_addr;
    i2c_tx_buf[0] = reg_addr;
    i2c_len = len;

    i2c_tx_buf_ptr = i2c_tx_buf;
    i2c_rx_buf_ptr = i2c_rx_buf;
    i2c_mode = 1; // read mode
    UCB1CTLW0 |= UCTR; // put peripheral into tx mode
    UCB1IE |= UCTXIE0; // enable tx complete interrupts
    UCB1CTLW0 |= UCTXSTT; // send start + slave addr

    while (i2c_busy) {} // wait until done receiving

    return i2c_rx_buf;
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
            i2c_busy = 0; // not busy
            I2C_BUSY_PORT &= ~I2C_BUSY_PIN;
            break;
        
        case 0x06: break; // STTIFG
        case 0x08: break; // STPIFG
        case 0x16:
            // RXIFG0 (rx buffer full)
            *i2c_rx_buf_ptr++ = UCB1RXBUF; // store received byte
            if (i2c_len > 1 && (i2c_rx_buf_ptr == (i2c_rx_buf + i2c_len - 1)))
            {
                UCB1CTLW0 |= UCTXSTP; // send stop condition after last byte
            }
            else if (i2c_rx_buf_ptr == (i2c_rx_buf + i2c_len))
            {
                UCB1IE &= ~UCRXIE0; // disable rx buffer full interrupts
                i2c_busy = 0; // not busy
                I2C_BUSY_PORT &= ~I2C_BUSY_PIN;
            }
            break;

        case 0x18:
            // TXIFG0 (tx complete)
            if (i2c_mode)
            {
                // read mode
                if (i2c_tx_buf_ptr == i2c_tx_buf)
                {
                    UCB1TXBUF = *i2c_tx_buf_ptr++; // send register address
                }
                else
                {
                    // transition to reading from slave:
                    UCB1CTLW0 &= ~UCTR; // put peripheral into rx mode
                    UCB1IE &= ~UCTXIE0; // disable tx complete interrupts
                    UCB1IE |= UCRXIE0; // enable rx buffer full interrupts
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
                if (i2c_tx_buf_ptr == (i2c_tx_buf + i2c_len))
                {
                    UCB1IE &= ~UCTXIE0; // disable tx complete interrupts
                    UCB1CTLW0 |= UCTXSTP; // send stop condition
                    i2c_busy = 0; // not busy
                    I2C_BUSY_PORT &= ~I2C_BUSY_PIN;
                }
                else
                {
                    UCB1TXBUF = *i2c_tx_buf_ptr++; // send next byte
                }
            }
            break;

        default: break;
    }
}
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
