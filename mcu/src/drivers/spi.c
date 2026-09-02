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
volatile unsigned char spi_mode;
volatile unsigned char *spi_addr_ptr;
volatile unsigned int spi_len;

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
 * @param clock_div number to divide clock by
 * 
 * @return none
 */
void spi_init(
    unsigned int timeout,
    unsigned int clock_div
) {
    SPI_SEL0 |= SPI_PINS; // configure pins
    SPI_CS0_PORT |= SPI_CS0_PIN; // clear slave 0 chip select
    
    // setup peripheral
    UCA0CTLW0 |= UCSWRST;       // put peripheral into software reset
    UCA0CTLW0 |= UCSSEL__SMCLK; // clock source = smclk (1 MHz)
    UCA0BRW = clock_div;        // number to divide clock by
    UCA0CTLW0 |= UCSYNC;        // synchronous mode
    UCA0CTLW0 |= UCMST;         // master mode
    UCA0CTLW0 |= UCMODE_0;      // 3-pin spi mode
    UCA0CTLW0 |= UCMSB;         // MSB first
    UCA0CTLW0 &= ~UCSWRST;      // take peripheral out of software reset

    // setup interrupts
    UCA0IFG = 0;    // clear interrupt flags
    UCA0IE = 0;     // disable interrupts

    // initialize variables
    spi_tx_buf_ptr = 0;
    spi_rx_buf_ptr = 0;
    spi_cnt = 0;
    spi_busy = 0;
    spi_slave = 0;
    spi_mode = 0;
    spi_timeout = timeout;
    spi_addr_ptr = 0;
    spi_len = 0;
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
    volatile unsigned char *arr,
    unsigned int len,
    unsigned char slave
) {
    if (len == 0) { return -1; }

    if (spi_busy) { return 1; }

    spi_tx_buf_ptr = arr;
    spi_cnt = len - 1;
    spi_slave = slave;
    spi_len = 0;

    UCA0IFG = 0; // clear interrupt flags
    UCA0IE |= UCTXIE; // enable tx complete interrupts
    
    SPI_CS0_PORT &= ~SPI_CS0_PIN;
    spi_busy = 1; // turn on bus

    UCA0TXBUF = *spi_tx_buf_ptr++; // tx first byte (triggers tx interrupt)

    int stat = spi_wait(); // wait until tx done (with timeout)

    SPI_CS0_PORT |= SPI_CS0_PIN;

    return stat;
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
    volatile unsigned char *arr,
    unsigned int len,
    unsigned char slave,
    volatile unsigned char *addr,
    unsigned int addr_len
) {
    if (len == 0) { return -1; }

    if (spi_busy) { return 1; }

    spi_rx_buf_ptr = arr;
    spi_cnt = len + addr_len - 1;
    spi_slave = slave;
    spi_addr_ptr = addr;
    spi_len = len;

    //UCA0IFG = 0; // clear interrupt flags
    //UCA0IE |= UCRXIE; // enable rx buffer full interrupts
    //spi_busy = 1; // turn on bus
    //UCA0TXBUF = 0; // send dummy byte

    UCA0IFG = 0; // clear interrupt flags
    UCA0IE |= UCTXIE; // enable tx complete interrupts
    
    SPI_CS0_PORT &= ~SPI_CS0_PIN;
    spi_busy = 1; // turn on bus

    // tx first addr byte or dummy (triggers tx interrupt)
    UCA0TXBUF = addr_len ? *spi_addr_ptr++ : 0;

    int stat = spi_wait(); // wait until rx done (with timeout)

    SPI_CS0_PORT |= SPI_CS0_PIN;

    return stat;
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
    unsigned int cnt = spi_timeout;

    // wait until i2c bus not busy or until countdown reaches 0
    while(spi_busy && (cnt > 0)) { cnt--; }

    if (cnt == 0) {
        // recover bus? TODO: hmm
        UCA0IFG = 0;    // clear interrupt flags
        UCA0IE = 0;     // disable interrupts
        spi_busy = 0;   // turn off bus
        return -2;
    }
    return 0;
}

/**
 * @brief set gpio chip select for specific slave
 * 
 * @return none
 */
void spi_start(void)
{
    switch(spi_slave)
    {
        case 0:
            SPI_CS0_PORT &= ~SPI_CS0_PIN;
            break;
        default:
            SPI_CS0_PORT &= ~SPI_CS0_PIN;
            break;
    }
}

/**
 * @brief clear gpio chip select for specific slave
 * 
 * @return none
 */
void spi_stop(void)
{
    switch(spi_slave)
    {
        case 0:
            SPI_CS0_PORT |= SPI_CS0_PIN;
            break;
        default:
            SPI_CS0_PORT |= SPI_CS0_PIN;
            break;
    }
}


//-----------------------------------------------------------------------------
//  INTERRUPT SERVICE ROUTINES
//-----------------------------------------------------------------------------

void __attribute__((interrupt(SPI_VECTOR))) isr_spi(void)
{
    switch(UCA0IV)
    {
        case 2:
            // RXIFG (rx buffer full)
            *spi_rx_buf_ptr++ = UCA0RXBUF; // store received byte
            if (spi_cnt == 0)
            {
                UCA0IFG &= ~UCRXIFG; // clear rx buffer full interrupt flag
                UCA0IE &= ~UCRXIE; // disable rx buffer full interrupts
                spi_busy = 0;
            }
            else
            {
                spi_cnt--; // decrement counter
                UCA0TXBUF = 0; // send dummy byte
            }
            break;
        
        case 4:
            // TXIFG (tx complete)
            if (spi_cnt == 0)
            {
                UCA0IFG &= ~UCTXIFG; // clear tx complete interrupt flag
                UCA0IE &= ~UCTXIE; // disable tx complete interrupts
                spi_busy = 0;
            }
            else if (spi_cnt > spi_len)
            {
                UCA0TXBUF = *spi_addr_ptr++; // send next addr byte
                spi_cnt--; // decrement counter
            }
            else if (spi_cnt == spi_len)
            {
                UCA0IFG = 0; // clear interrupt flags
                UCA0IE &= ~UCTXIE; // disable tx complete interrupts
                UCA0IFG &= ~UCRXIFG; // clear tx complete interrupt flag
                UCA0IE |= UCRXIE; // enable rx buffer full interrupts
                spi_cnt--;
                UCA0TXBUF = 0;
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
