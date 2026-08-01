/** ---------------------------------------------------------------------------
 * 
 * GORTOS
 * spi driver
 * 
 * created by rdupu13
 * 
 * @file spi.c
 *
----------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
//  LIBRARIES
//-----------------------------------------------------------------------------

#include "drivers/spi.h"

// hardware
#include "hw/pfc.h"


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------

volatile unsigned char *spi_tx_buf_ptr;
volatile unsigned char *spi_rx_buf_ptr;
volatile unsigned int spi_cnt;
volatile unsigned char spi_busy;

volatile unsigned char spi_slave;

unsigned int spi_timeout;


//-----------------------------------------------------------------------------
//  FUNCTIONS
//-----------------------------------------------------------------------------

int spi_wait(void);
void spi_busy_set(void);
void spi_busy_clear(void);

/**
 * @brief initialize spi
 * 
 * @param timeout countdown period until bus timeout
 * 
 * @return none
 */
void spi_init(unsigned int timeout)
{
    SPI_SEL0 |= SPI_PINS; // configure pins
    
    // setup peripheral
    UCA0CTLW0 |= UCSWRST;       // put peripheral into software reset
    UCA0CTLW0 |= UCSSEL__SMCLK; // clock source = smclk (1 MHz)
    UCA0BRW = 10;               // divide clock by 10 (100 kHz)
    UCA0CTLW0 |= UCSYNC;        // synchronous mode
    UCA0CTLW0 |= UCMST;         // master mode
    UCA0CTLW0 |= UCMODE_2;      // 4-pin spi mode
    UCA0CTLW0 &= ~UCSWRST;      // take peripheral out of software reset

    // setup interrupts
    UCA0IFG &= ~(UCTXIFG | UCRXIFG);    // clear tx/rx interrupt flags
    UCA0IE &= ~(UCTXIE | UCRXIE);       // disable tx/rx interrupts

    // initialize variables
    spi_tx_buf_ptr = 0;
    spi_rx_buf_ptr = 0;
    spi_cnt = 0;
    spi_busy = 0;
    spi_slave = 0;
    spi_timeout = timeout;
}

/**
 * @brief write an array to a spi slave
 * 
 * @param arr   pointer to array to be written
 * @param len   length in bytes of array to be written
 * @param slave selected slave
 * 
 * @return status of write:
 *           0: ok
 *           1: bus busy
 *          -1: 0 length error
 *          -2: timeout error
 */
int spi_write(
    unsigned char *arr,
    unsigned int len,
    unsigned char slave
) {
    if (len == 0) { return -1; }

    if (spi_busy) { return 1; }

    spi_tx_buf_ptr = arr;
    spi_cnt = len - 1;
    spi_slave = slave;

    UCA0IFG &= ~UCTXIFG; // clear tx complete interrupt flag
    UCA0IE |= UCTXIE; // enable tx complete interrupts
    
    spi_busy_set();
    UCA0TXBUF = *spi_tx_buf_ptr++; // tx first byte (triggers tx interrupt)

    return spi_wait(); // wait until tx done (with timeout)
}


/**
 * @brief read an array from a spi slave
 * 
 * @param arr   pointer to array to store received data
 * @param len   length in bytes of array to be read
 * @param slave selected slave
 * 
 * @return status of read:
 *           0: ok
 *           1: bus busy
 *          -1: 0 length error
 *          -2: timeout error
 */
int spi_read(
    unsigned char *arr,
    unsigned int len,
    unsigned char slave
) {
    if (len == 0) { return -1; }

    if (spi_busy) { return 1; }

    spi_rx_buf_ptr = arr;
    spi_cnt = len;
    spi_slave = slave;

    UCA0IFG &= ~UCRXIFG; // clear rx buffer full interrupt flag
    UCA0IE |= UCRXIE; // enable rx buffer full interrupts
    
    spi_busy_set();
    UCA0TXBUF = 0; // send dummy byte

    return spi_wait(); // wait until rx done (with timeout)
}

/**
 * @brief wait for timeout countdown or busy flag to clear
 * 
 * @return status of wait:
 *           0: ok
 *          -2: timeout error
 */
int spi_wait(void)
{
    volatile unsigned int cnt = spi_timeout;

    // wait until i2c bus not busy or until countdown reaches 0
    while(spi_busy && (cnt > 0)) { cnt--; }

    if (cnt == 0) {
        // recover bus
        UCA0IFG &= ~(UCTXIFG | UCRXIFG);  // clear tx/rx interrupt flags
        UCA0IE &= ~(UCTXIE | UCRXIE);     // disable tx/rx interrupts
        spi_busy_clear();
        return -2;
    }
    return 0;
}

/**
 * @brief set busy flag and gpio chip select
 * 
 * @return none
 */
void spi_busy_set(void)
{
    // TODO: see if this breaks things
    UCA0CTLW0 &= ~UCSTEM; // disable chip select 0
    switch(spi_slave)
    {
        default:
            UCA0CTLW0 |= UCSTEM; // enable chip select 0
            break;
    }
    spi_busy = 1;
}

/**
 * @brief clear busy flag and gpio chip select
 * 
 * @return none
 */
void spi_busy_clear(void)
{
    switch(spi_slave)
    {
        default: break; // automatic for chip select 0 TODO: test
    }
    spi_busy = 0;
}


//-----------------------------------------------------------------------------
//  INTERRUPT SERVICE ROUTINES
//-----------------------------------------------------------------------------

#pragma vector = SPI_VECTOR
__interrupt void isr_spi(void)
{
    switch(UCA0IV)
    {
        case 2:
            // RXIFG (rx buffer full)
            if (spi_cnt == 0)
            {
                UCA0IFG &= ~UCRXIFG; // clear rx buffer full interrupt flag
                UCA0IE &= ~UCRXIE; // disable rx buffer full interrupts
                spi_busy_clear();
            }
            else
            {
                *spi_rx_buf_ptr++ = UCA0RXBUF; // store received byte
                UCA0TXBUF = 0; // send dummy byte
                spi_cnt--; // decrement counter
            }
            break;
        
        case 4:
            // TXIFG (tx complete)
            if (spi_cnt == 0)
            {
                UCA0IFG &= ~UCTXIFG; // clear tx complete interrupt flag
                UCA0IE &= ~UCTXIE; // disable tx complete interrupts
                spi_busy_clear();
            }
            else
            {
                UCA0TXBUF = *spi_tx_buf_ptr++; // send next byte
                spi_cnt--; // decrement counter
            }
            break;
        
        default: break;
    }
}
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
