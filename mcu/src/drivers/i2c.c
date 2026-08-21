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

// kernel
#include "kernel/gsys.h"


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

volatile unsigned int i2c_arblost;


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
    UCB1IFG = 0;                        // clear interrupt flags
    UCB1IE |= UCNACKIE;                 // enable no acknowledge interrupt
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

    i2c_arblost = 0;
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
 *          -1: bus busy
 *          -2: 0 length error
 *          -3: timeout error
 *          -4: nack error
 */
int i2c_write(
    volatile unsigned char *arr,
    unsigned int len,
    unsigned int slave_addr,
    unsigned char reg_addr
) {
    if (i2c_busy) { return -1; }

    if (len == 0) { return -2; }

    i2c_tx_buf_ptr = arr;
    i2c_cnt = len + 1;
    UCB1I2CSA = slave_addr;
    i2c_reg_addr = reg_addr;
    
    i2c_len = len;
    i2c_mode = 0; // write mode

    UCB1CTLW0 |= UCTR;      // put peripheral into tx mode
    UCB1IFG &= ~UCTXIFG0;   // clear tx complete interrupt flag
    UCB1IE |= UCTXIE0;      // enable tx complete interrupts

    i2c_busy_set();
    UCB1CTLW0 |= UCTXSTT; // send start + send slave addr

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
 *          -1: bus busy
 *          -2: 0 length error
 *          -3: timeout error
 *          -4: nack error
 */
int i2c_read(
    volatile unsigned char *arr,
    unsigned int len,
    unsigned int slave_addr,
    unsigned char reg_addr
) {
    if (i2c_busy) { return -1; }

    if (len == 0) { return -2; }

    i2c_rx_buf_ptr = arr;
    i2c_cnt = len + 1;
    UCB1I2CSA = slave_addr;
    i2c_reg_addr = reg_addr;

    i2c_len = len;
    i2c_mode = 1; // read mode

    UCB1CTLW0 |= UCTR; // put peripheral into tx mode
    UCB1IFG &= ~UCTXIFG0; // clear tx complete interrupt flag
    UCB1IE |= UCTXIE0; // enable tx complete interrupts

    i2c_busy_set();
    UCB1CTLW0 |= UCTXSTT; // send start + slave addr

    return i2c_wait(); // wait until rx done (with timeout)
}

/**
 * @brief wait for timeout countdown or busy flag to clear
 * 
 * @return status of wait:
 *           0: ok
 *          -3: timeout error
 *          -4: nack error
 */
int i2c_wait(void)
{
    unsigned int cnt = i2c_timeout;
    unsigned char i;

    // wait until i2c bus not busy or until countdown reaches 0
    while(i2c_busy && (cnt > 0)) { cnt--; }

    // timeout error case (recover bus and hard reset peripheral)
    if (cnt == 0)
    {
        UCB1CTLW0 |= UCSWRST; // put peripheral into software reset
        
        I2C_SEL0 &= ~I2C_PINS; // switch scl and sda to outputs
        P4DIR |= I2C_PINS;
        
        I2C_PORT |= I2C_PINS; // set scl and sda high
        for (i = 0; i < 18; i++)
        {
            __nop();
            __nop();
            __nop();
            __nop();
            I2C_PORT ^= I2C_SCL;
        }
        __nop();
        __nop();
        __nop();
        __nop();
        I2C_PORT |= I2C_SCL;
        __nop();
        __nop();
        __nop();
        __nop();
        I2C_PORT |= I2C_SDA;
        __nop();
        __nop();
        __nop();
        __nop();

        i2c_init(i2c_timeout);

        return -3;
    }

    // nack error case
    if (i2c_nack) { 
        i2c_nack = 0;
        return -4;
    }
    return 0;
}

/**
 * @brief set the busy flag
 * 
 * @return none
 */
void i2c_busy_set(void)
{
    I2C_BUSY_PORT |= I2C_BUSY_PIN;
    i2c_busy = 1;
}

/**
 * @brief clear the busy flag
 * 
 * @return none
 */
void i2c_busy_clear(void)
{
    I2C_BUSY_PORT &= ~I2C_BUSY_PIN;
    i2c_busy = 0;
}


//-----------------------------------------------------------------------------
//  INTERRUPT SERVICE ROUTINES
//-----------------------------------------------------------------------------

void __attribute__((interrupt(I2C_VECTOR))) isr_i2c(void)
{
    switch(UCB1IV)
    {
        case 0x00: break; // no interrupts
        case 0x02:
            // ALIFG (arbitration lost)
            i2c_arblost++;
            break;

        case 0x04:
            // NACKIFG (no acknowledge received)
            UCB1CTLW0 |= UCTR;              // put peripheral into tx mode
            UCB1IFG = 0;                    // clear interrupt flags
            UCB1IE &= ~(UCTXIE0 | UCRXIE0); // disable tx/rx interrupts

            UCB1CTLW0 |= UCTXSTP;           // send stop condition
            while (UCB1CTLW0 & UCTXSTP) {}
            i2c_busy_clear();
            i2c_nack = 1;
            break;
        
        case 0x06: break; // STTIFG
        case 0x08: break; // STPIFG
        case 0x16:
            // RXIFG0 (rx buffer full)
            *i2c_rx_buf_ptr++ = UCB1RXBUF; // store received byte
            i2c_cnt--;

            if (i2c_cnt == 1)
            {
                UCB1CTLW0 |= UCTXSTP; // arm stop condition on second-to-last byte
            }
            else if (i2c_cnt == 0)
            {
                UCB1IFG &= ~UCRXIFG0; // clear rx buffer full interrupt flag
                UCB1IE &= ~UCRXIE0; // disable rx buffer full interrupts
                //while (UCB1CTLW0 & UCTXSTP) {}
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
                    i2c_cnt--;
                }
                else
                {
                    // transition to reading from slave:
                    UCB1CTLW0 &= ~UCTR; // put peripheral into rx mode
                    UCB1IFG = 0;        // clear interrupt flags
                    UCB1IE &= ~UCTXIE0; // disable tx complete interrupts
                    UCB1IE |= UCRXIE0;  // enable rx buffer full interrupts

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
                    UCB1IFG &= ~UCTXIFG0;   // clear tx complete interrupt flag
                    UCB1IE &= ~UCTXIE0;     // disable tx complete interrupts

                    UCB1CTLW0 |= UCTXSTP; // send stop condition
                    while (UCB1CTLW0 & UCTXSTP) {}
                    i2c_busy_clear();
                }
                else if (i2c_cnt > i2c_len)
                {
                    UCB1TXBUF = i2c_reg_addr; // send register address
                    i2c_cnt--;
                }
                else
                {
                    UCB1TXBUF = *i2c_tx_buf_ptr++; // send next byte
                    i2c_cnt--;
                }
            }
            break;

        default: break;
    }
}
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
