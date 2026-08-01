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

#include "drivers/i2c.h"

// hardware
#include "hw/pfc.h"


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------

volatile unsigned char *i2c_tx_buf_ptr;
volatile unsigned char *i2c_rx_buf_ptr;
volatile unsigned int i2c_cnt;
volatile unsigned char i2c_busy;

volatile unsigned char i2c_mode;
volatile unsigned int i2c_len;
volatile unsigned char i2c_reg_addr;
volatile unsigned char i2c_nack;

unsigned int i2c_timeout;


//-----------------------------------------------------------------------------
//  FUNCTIONS
//-----------------------------------------------------------------------------

int i2c_wait(void);
void i2c_busy_set(void);
void i2c_busy_clear(void);

/**
 * @brief initialize i2c
 * 
 * @param timeout countdown period until bus timeout
 * 
 * @return none
 */
void i2c_init(unsigned int timeout)
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
    UCB1IFG &= ~UCNACKIFG;              // clear no acknowledge interrupt flag
    UCB1IE |= UCNACKIE;                 // enable no acknowledge interrupt
    UCB1IFG &= ~(UCTXIFG0 | UCRXIFG0);  // clear tx/rx interrupt flags
    UCB1IE &= ~(UCTXIE0 | UCRXIE0);     // disable tx/rx interrupts

    // initialize variables
    i2c_tx_buf_ptr = 0;
    i2c_rx_buf_ptr = 0;
    i2c_cnt = 0;
    i2c_busy_clear();
    i2c_mode = 0;
    i2c_len = 0;
    i2c_reg_addr = 0;
    i2c_timeout = timeout;
    i2c_nack = 0;
}

/**
 * @brief write an array to an i2c slave
 * 
 * @param arr           pointer to array to be written
 * @param len           length in bytes of array to be written
 * @param slave_addr    slave address
 * @param reg_addr      slave register address to start writing at
 * 
 * @return status of write:
 *           0: ok
 *           1: bus busy
 *          -1: 0 length error
 *          -2: timeout error
 *          -3: nack error
 */
int i2c_write(
    unsigned char *arr,
    unsigned int len,
    unsigned int slave_addr,
    unsigned char reg_addr
) {
    if (len == 0) { return -1; }

    if (i2c_busy) { return 1; }

    i2c_tx_buf_ptr = arr;
    i2c_cnt = len + 1;
    UCB1I2CSA = slave_addr;
    i2c_reg_addr = reg_addr;
    
    i2c_len = len;
    i2c_mode = 0; // write mode

    UCB1CTLW0 |= UCTR; // put peripheral into tx mode
    UCB1IFG &= ~UCTXIFG0; // clear tx complete interrupt flag
    UCB1IE |= UCTXIE0; // enable tx complete interrupts

    // critical section ---------------
    i2c_busy_set();
    UCB1CTLW0 |= UCTXSTT; // send start + send slave addr
    // --------------------------------

    return i2c_wait(); // wait until tx done (with timeout)
}

/**
 * @brief read an array from an i2c slave
 * 
 * @param arr           pointer to array to store received data
 * @param len           length in bytes of array to be read
 * @param slave_addr    slave address
 * @param reg_addr      slave register address to start reading from
 * 
 * @return status of read:
 *           0: ok
 *           1: bus busy
 *          -1: 0 length error
 *          -2: timeout error
 *          -3: nack error
 */
int i2c_read(
    unsigned char *arr,
    unsigned int len,
    unsigned int slave_addr,
    unsigned char reg_addr
) {
    if (len == 0) { return -1; }

    if (i2c_busy) { return 1; }

    i2c_rx_buf_ptr = arr;
    i2c_cnt = len + 1;
    UCB1I2CSA = slave_addr;
    i2c_reg_addr = reg_addr;

    i2c_len = len;
    i2c_mode = 1; // read mode

    UCB1CTLW0 |= UCTR; // put peripheral into tx mode
    UCB1IFG &= ~UCTXIFG0; // clear tx complete interrupt flag
    UCB1IE |= UCTXIE0; // enable tx complete interrupts

    // critical section ---------------
    i2c_busy_set();
    UCB1CTLW0 |= UCTXSTT; // send start + slave addr
    // --------------------------------

    return i2c_wait(); // wait until rx done (with timeout)
}

/**
 * @brief wait for timeout countdown or busy flag to clear
 * 
 * @return status of wait:
 *           0: ok
 *          -2: timeout error
 *          -3: nack error
 */
int i2c_wait(void)
{
    volatile unsigned int cnt = i2c_timeout;

    // wait until i2c bus not busy or until countdown reaches 0
    while(i2c_busy && (cnt > 0)) { cnt--; }

    if (cnt == 0) {
        // TODO: figure out how to recover bus
        UCB1CTLW0 |= UCTR; // put peripheral into tx mode
        UCB1IFG &= ~(UCTXIFG0 | UCRXIFG0);  // clear tx/rx interrupt flags
        UCB1IE &= ~(UCTXIE0 | UCRXIE0);     // disable tx/rx interrupts
        UCB1CTLW0 |= UCTXSTP; // send stop condition
        i2c_busy_clear();
        return -2;
    }
    if (i2c_nack) { return -3; } // handle nack case
    return 0;
}

/**
 * @brief set the busy flag
 * 
 * @return none
 */
void i2c_busy_set(void)
{
    i2c_busy = 1;
    I2C_BUSY_PORT |= I2C_BUSY_PIN;
}

/**
 * @brief clear the busy flag
 * 
 * @return none
 */
void i2c_busy_clear(void)
{
    i2c_busy = 0;
    I2C_BUSY_PORT &= ~I2C_BUSY_PIN;
}


//-----------------------------------------------------------------------------
//  INTERRUPT SERVICE ROUTINES
//-----------------------------------------------------------------------------

#pragma vector = I2C_VECTOR
__interrupt void isr_i2c(void)
{
    switch(__even_in_range(UCB1IV, 0x1E))
    {
        case 0x00: break; // no interrupts
        case 0x02: break; // ALIFG (arbitration lost)
        case 0x04:
            // NACKIFG (no acknowledge received)
            UCB1CTLW0 |= UCTR; // put peripheral into tx mode
            UCB1IFG &= ~(UCNACKIFG | UCTXIFG0 | UCRXIFG0); // clear nack/tx/rx interrupt flags
            UCB1IE &= ~(UCTXIE0 | UCRXIE0); // disable tx/rx interrupts
            UCB1CTLW0 |= UCTXSTP; // send stop condition
            i2c_busy_clear();
            i2c_nack = 1;
            break;
        
        case 0x06: break; // STTIFG
        case 0x08: break; // STPIFG
        case 0x16:
            // RXIFG0 (rx buffer full)
            *i2c_rx_buf_ptr++ = UCB1RXBUF; // store received byte
            i2c_cnt--; // decrement counter

            if (i2c_cnt == 1)
            {
                UCB1CTLW0 |= UCTXSTP; // arm stop condition on second-to-last byte
            }
            else if (i2c_cnt == 0)
            {
                UCB1IFG &= ~UCRXIE0; // clear rx buffer full interrupt flag
                UCB1IE &= ~UCRXIE0; // disable rx buffer full interrupts
                i2c_busy_clear();
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
                    UCB1IFG &= ~(UCTXIFG0 | UCRXIFG); // clear tx/rx interrupt flags
                    UCB1IE &= ~UCTXIE0; // disable tx complete interrupts
                    UCB1IE |= UCRXIE0; // enable rx buffer full interrupts

                    UCB1CTLW0 |= UCTXSTT; // send repeated start + slave addr
                    
                    // stupid special case for 1-byte reads (super lame, texas instruments!)
                    if (i2c_len == 1)
                    {
                        while (UCB1CTLW0 & UCTXSTT) {}  // wait for repeated start + slave addr
                        UCB1CTLW0 |= UCTXSTP;           // prepare to stop early
                    }
                }
            }
            else
            {
                // write mode
                if (i2c_cnt == 0)
                {
                    UCB1IFG &= ~UCTXIFG0; // clear tx complete interrupt flag
                    UCB1IE &= ~UCTXIE0; // disable tx complete interrupts

                    UCB1CTLW0 |= UCTXSTP; // send stop condition
                    i2c_busy_clear();
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
