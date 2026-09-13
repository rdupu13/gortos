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

// kernel
#include "kernel/gsys.h"


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------

volatile unsigned char *spi_tx_buf_ptr;
volatile unsigned char *spi_rx_buf_ptr;
volatile unsigned char *spi_addr_ptr;
volatile unsigned int spi_cnt;
volatile unsigned int spi_len;
volatile unsigned char spi_busy;
volatile unsigned char spi_mode;

unsigned int spi_timeout;


//-----------------------------------------------------------------------------
//  FUNCTIONS
//-----------------------------------------------------------------------------

int spi_wait(void);
void spi_start(unsigned char slave);
void spi_stop(unsigned char slave);

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
    spi_mode = 0;
    spi_timeout = timeout;
    spi_addr_ptr = 0;
    spi_len = 0;

    SPI_BUSY_PORT &= ~SPI_BUSY_PIN;
}

/**
 * @brief write an array to a spi slave
 * 
 * @param arr       pointer to array to be written
 * @param len       length in bytes of array to be written
 * @param slave     selected slave
 * @param addr      pointer to array to send before writing
 * @param addr_len  length in bytes of addr array
 * 
 * @return status of write:
 *           0: ok
 *          -1: bus busy
 *          -2: 0 length error
 *          -3: timeout error
 */
int spi_write(
    unsigned char *arr,
    unsigned int len,
    unsigned char slave,
    unsigned char *addr,
    unsigned int addr_len
) {
    if (spi_busy) { return -1; }
    if (len == 0) { return -2; }

    SPI_BUSY_PORT |= SPI_BUSY_PIN;

    spi_tx_buf_ptr = (volatile unsigned char *) arr;
    spi_addr_ptr = (volatile unsigned char *) addr;
    spi_cnt = len + addr_len;
    spi_len = len;

    spi_mode = 0; // write mode
    UCA0IFG = 0; // clear interrupt flags
    UCA0IE |= UCTXIE; // enable tx complete interrupts
    
    spi_start(slave);
    spi_busy = 1; // turn on bus

    // tx first addr/data byte (triggers tx interrupt)
    spi_cnt--;
    UCA0TXBUF = addr_len ? *spi_addr_ptr++ : *spi_tx_buf_ptr++;

    int stat = spi_wait(); // wait until tx done (with timeout)

    spi_stop(slave);

    eep(SPI_DELAY_MS);

    SPI_BUSY_PORT &= ~SPI_BUSY_PIN;

    return stat;
}

/**
 * @brief read an array from a spi slave
 * 
 * @param arr       pointer to array to store received data
 * @param len       length in bytes of array to be read
 * @param slave     selected slave
 * @param addr      pointer to array to send before reading
 * @param addr_len  length in bytes of addr array
 * 
 * @return status of read:
 *           0: ok
 *          -1: bus busy
 *          -2: 0 length error
 *          -3: timeout error
 */
int spi_read(
    unsigned char *arr,
    unsigned int len,
    unsigned char slave,
    unsigned char *addr,
    unsigned int addr_len
) {
    if (spi_busy) { return -1; }
    if (len == 0) { return -2; }

    SPI_BUSY_PORT |= SPI_BUSY_PIN;

    spi_rx_buf_ptr = (volatile unsigned char *) arr;
    spi_addr_ptr = (volatile unsigned char *) addr;
    spi_cnt = len + addr_len;
    spi_len = len;

    spi_mode = 1; // read mode
    UCA0IFG = 0; // clear interrupt flags

    spi_start(slave);
    spi_busy = 1; // turn on bus

    // tx first addr byte or dummy (triggers tx interrupt)
    spi_cnt--;
    if (addr_len == 0)
    {
        UCA0IE |= UCRXIE; // enable rx buffer full interrupts
        UCA0TXBUF = 0;
    }
    else
    {
        UCA0IE |= UCTXIE; // enable tx complete interrupts
        UCA0TXBUF = *spi_addr_ptr++;
    }

    int stat = spi_wait(); // wait until rx done (with timeout)

    spi_stop(slave);

    eep(SPI_DELAY_MS);

    SPI_BUSY_PORT &= ~SPI_BUSY_PIN;

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
        // recover bus? TODO: hmmmmmm
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
void spi_start(unsigned char slave)
{
    switch(slave)
    {
        case 0:
            SPI_CS0_PORT &= ~SPI_CS0_PIN;
            break;
        default: break;
    }
}

/**
 * @brief clear gpio chip select for specific slave
 * 
 * @return none
 */
void spi_stop(unsigned char slave)
{
    switch(slave)
    {
        case 0:
            SPI_CS0_PORT |= SPI_CS0_PIN;
            break;
        default: break;
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
                UCA0IFG &= ~UCRXIFG;    // clear rx buffer full interrupt flag
                UCA0IE &= ~UCRXIE;      // disable rx buffer full interrupts
                spi_busy = 0;
            }
            else
            {
                UCA0TXBUF = 0;  // send dummy byte
                spi_cnt--;      // decrement counter
            }
            break;
        
        case 4:
            // TXIFG (tx complete)
            if (spi_cnt == 0)
            {
                UCA0IFG &= ~UCTXIFG;    // clear tx complete interrupt flag
                UCA0IE &= ~UCTXIE;      // disable tx complete interrupts
                spi_busy = 0;
            }
            else if (spi_cnt > spi_len)
            {
                UCA0TXBUF = *spi_addr_ptr++;    // send next addr byte
                spi_cnt--;                      // decrement counter
            }
            else if (spi_mode && (spi_cnt == spi_len))
            {
                UCA0IFG = 0;        // clear interrupt flags
                UCA0IE &= ~UCTXIE;  // disable tx complete interrupts
                UCA0IE |= UCRXIE;   // enable rx buffer full interrupts
                UCA0TXBUF = 0;      // send dummy byte
                spi_cnt--;          // decrement counter
            }
            else
            {
                UCA0TXBUF = *spi_tx_buf_ptr++;  // send next byte
                spi_cnt--;                      // decrement counter
            }
            break;
        
        default: break;
    }
}
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
