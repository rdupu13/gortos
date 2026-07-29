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

#include <msp430fr2153.h>
#include <stdint.h>

#include "drivers/spi.h"


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------

volatile uint8_t *spi_tx_buf_ptr;
volatile uint8_t *spi_rx_buf_ptr;
volatile uint16_t spi_cnt;
volatile uint8_t spi_busy;


//-----------------------------------------------------------------------------
//  FUNCTIONS
//-----------------------------------------------------------------------------

/**
 * @brief initialize spi
 * 
 * @param none
 * @return none
 */
void spi_init()
{
    SPI_SEL0 |= SPI_PINS; // configure pins
    
    // setup peripheral
    UCA0CTLW0 |= UCSWRST;       // put peripheral into software reset
    UCA0CTLW0 |= UCSSEL__SMCLK; // clock source = smclk (1 MHz)
    UCA0BRW = 10;               // divide clock by 10 (100 kHz)
    UCA0CTLW0 |= UCSYNC;        // synchronous mode
    UCA0CTLW0 |= UCMST;         // master mode
    UCA0CTLW0 |= UCMODE_2;      // 4-pin spi mode
    UCA0CTLW0 |= UCSTEM;        // use STE as output enable for slaves
    UCA0CTLW0 &= ~UCSWRST;      // take peripheral out of software reset

    // setup interrupts
    UCA0IE &= ~(UCTXIE | UCRXIE);       // disable tx/rx interrupts
    UCA0IFG &= ~(UCTXIFG | UCRXIFG);    // clear tx/rx interrupt flags

    // initialize variables
    spi_tx_buf_ptr = 0;
    spi_rx_buf_ptr = 0;
    spi_cnt = 0;
    spi_busy = 0;
}

/**
 * @brief write an array to a spi slave
 * 
 * @param reg_addr      slave register address to start writing at
 * @param len           length in bytes of array to be written
 * @param arr           pointer to array to be written
 * 
 * @return none
 */
void spi_write(uint8_t reg_addr, uint16_t len, uint8_t *arr)
{
    if (len == 0) { return; }

    if (spi_busy) { return; }
    spi_busy = 1; // busy

    // TODO: handle reg_addr

    spi_tx_buf_ptr = arr;
    spi_cnt = len - 1;

    UCA0IE |= UCTXIE; // enable tx complete interrupts
    UCA0IFG &= ~UCTXIFG; // clear tx complete interrupt flag
    
    UCA0TXBUF = *spi_tx_buf_ptr++; // tx first byte, triggering TXIFG

    // TODO: add timeout (tx didn't work for some reason)
    while(spi_busy) {} // wait until tx done
}


/**
 * @brief read an array from a spi slave
 * 
 * @param reg_addr      slave register address to start reading from
 * @param len           length in bytes of array to be read
 * @param arr           pointer to array to store received data
 * 
 * @return pointer to receive buffer
 */
void spi_read(uint8_t reg_addr, uint16_t len, uint8_t *arr)
{
    if (len == 0) { return; }

    if (spi_busy) { return; }
    spi_busy = 1; // busy

    // TODO: handle reg_addr

    spi_rx_buf_ptr = arr;
    spi_cnt = len;

    UCA0IE |= UCRXIE; // enable rx buffer full interrupts
    UCA0IFG &= ~UCRXIFG; // clear rx buffer full interrupt flag

    // TODO: add timeout
    while(spi_busy) {} // wait until rx done
}

//-----------------------------------------------------------------------------
//  INTERRUPT SERVICE ROUTINES
//-----------------------------------------------------------------------------

#pragma vector = EUSCI_A0_VECTOR
__interrupt void isr_eusci_a0(void)
{
    switch(UCA0IV)
    {
        case 2:
            // RXIFG (rx buffer full)
            if (spi_cnt == 0)
            {
                UCA0IE &= ~UCRXIE;
                spi_busy = 0;
            }
            else
            {
                *spi_rx_buf_ptr++ = UCA0RXBUF; // store received byte
                spi_cnt--; // decrement counter
            }
            break;
        
        case 4:
            // TXIFG (tx complete)
            if (spi_cnt == 0)
            {
                UCA0IE &= ~UCTXIE; // disable tx complete interrupts
                spi_busy = 0; // not busy
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
