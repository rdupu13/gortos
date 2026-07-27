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
    
}

/**
 * @brief write an array to a spi slave
 * 
 * @param reg_addr      slave register address to start writing at
 * @param len           length in bytes of array to be written
 * @param arr           pointer to array to be written
 * 
 * @return none
 *
void spi_write(uint8_t reg_addr, uint8_t len, uint8_t *arr)
{
    
}
*/

/**
 * @brief read an array from a spi slave
 * 
 * @param reg_addr      slave register address to start reading from
 * @param len           length in bytes of array to be read
 * 
 * @return pointer to receive buffer
 *
uint8_t *spi_read(uint8_t reg_addr, uint8_t len)
{
    
}
*/

//-----------------------------------------------------------------------------
//  INTERRUPT SERVICE ROUTINES
//-----------------------------------------------------------------------------

#pragma vector = EUSCI_A0_VECTOR
__interrupt void isr_eusci_a0(void)
{
    switch(UCA0IV)
    {
        case 2:
            // RXIFG

            break;
        
        case 4:
            // TXIFG

            break;
        
        default: break;
    }
}
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
