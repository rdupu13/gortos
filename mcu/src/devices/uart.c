/** ---------------------------------------------------------------------------
 * 
 * GORTOS
 * uart driver
 * 
 * created by rdupu13
 * 
 * @file uart.c
 *
----------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
//  LIBRARIES
//-----------------------------------------------------------------------------

#include <msp430fr2153.h>
#include "devices/uart.h"


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------

volatile unsigned char *uart_tx_buf_ptr;
volatile unsigned char *uart_rx_buf_ptr;
volatile unsigned int uart_cnt;
volatile unsigned char uart_busy;

volatile unsigned char uart_rxmode;
volatile unsigned char uart_stop;


//-----------------------------------------------------------------------------
//  FUNCTIONS
//-----------------------------------------------------------------------------

/**
 * @brief initialize uart
 * 
 * @param none
 * @return none
 */
void uart_init(void)
{
    UART_SEL0 |= UART_PINS; // configure pins

    // setup peripheral
    UCA1CTLW0 |= UCSWRST; // put peripheral into software reset
    switch(UART_BAUD)
    {
        case 9600:
            UCA1CTLW0 |= UCSSEL__ACLK;  // clock source = aclk (32.768 kHz)
            UCA1BRW = 3;                // divide brclk by 3
            UCA1MCTLW |= 0x9200;        // modulation = 0x9200 (9600 baud)
            break;
        case 115200:
            UCA1CTLW0 |= UCSSEL__SMCLK; // clock source = smclk (1 MHz)
            UCA1BRW = 8;                // divide brclk by 8
            UCA1MCTLW |= 0xD600;        // modulation = 0xD608 (115200 baud)
            break;
        default:
            // TODO: return error
            UCA1CTLW0 |= UCSSEL__SMCLK; // clock source = smclk (1 MHz)
            UCA1BRW = 8;                // divide brclk by 8
            UCA1MCTLW |= 0xD600;        // modulation = 0xD608 (115200 baud)
            break;
    }
    UCA1CTLW0 &= ~UCSWRST; // take peripheral out of software reset
    
    // setup interrupts
    UCA1IE &= ~(UCTXCPTIE | UCRXIFG);   // disable tx/rx interrupts
    UCA1IFG &= ~(UCTXCPTIFG | UCRXIFG); // clear tx/rx interrupt flags

    // initialize variables
    uart_tx_buf_ptr = 0;
    uart_rx_buf_ptr = 0;
    uart_cnt = 0;
    uart_busy = 0;
    uart_rxmode = 0;
    uart_stop = '\r';
}

/**
 * @brief transmit an array over uart
 * 
 * @param len length in bytes of array to be transmitted
 * @param arr pointer to array to be transmitted
 * 
 * @return none
 */
void uart_tx(unsigned int len, unsigned char *arr)
{
    if (len == 0) { return; }

    if (uart_busy) { return; }
    uart_busy = 1; // busy

    uart_cnt = len - 1;
    uart_tx_buf_ptr = arr;
    
    UCA1IE |= UCTXCPTIE; // enable tx complete interrupts
    UCA1IFG &= ~UCTXCPTIFG; // clear tx complete interrupt flag
    
    UCA1TXBUF = *uart_tx_buf_ptr++; // tx first byte, triggering TXCPTIFG

    // TODO: add timeout (tx didn't work for some reason)
    while(uart_busy) {} // wait until tx done
}

/**
 * @brief receive an array over uart
 * 
 * @param len   number of bytes to read (> 0)
 * @param arr   pointer to array to store received data
 * @param stop  if len = 0, read until this character received
 * 
 * @return none
 */
void uart_rx(unsigned int len, unsigned char *arr, unsigned char stop)
{
    if (uart_busy) { return 0; }
    uart_busy = 1; // busy

    uart_cnt = len;
    uart_rx_buf_ptr = arr;

    uart_rxmode = (len == 0) ? 1 : 0;
    uart_stop = stop;
    
    UCA1IE |= UCRXIFG; // enable rx buffer full interrupts
    UCA1IFG &= ~UCRXIFG; // clear rx buffer full interrupt flag

    // TODO: add timeout
    while(uart_busy) {} // wait until rx done
}


//-----------------------------------------------------------------------------
//  INTERRUPT SERVICE ROUTINES
//-----------------------------------------------------------------------------

#pragma vector = EUSCI_A1_VECTOR
__interrupt void isr_eusci_a1(void)
{
    switch(__even_in_range(UCA1IV, 18))
    {
        case 0x00: break; // no interrupts
        case 0x02:
            // UCRXIFG (rx buffer full)
            *uart_rx_buf_ptr = UCA1RXBUF; // store received byte
            
            // echo char if enabled
            if (UART_ECHO)
            {
                UCA1TXBUF = *uart_rx_buf_ptr;
            }
            
            uart_cnt--; // decrement counter

            // stop condition
            if ((!uart_rxmode && (uart_cnt == 0))
                || (uart_rxmode && (*uart_rx_buf_ptr == uart_stop)))
            {
                UCA1IE &= ~UCRXIE; // disable rx buffer full interrupts
                uart_busy = 0; // not busy
            }
            
            uart_rx_buf_ptr++; // increment pointer
            break;
        
        case 0x04: break; // UCTXIFG (transmit??)
        case 0x06: break; // UCSTTIFG (start condition)
        case 0x08:
            // UCTXCPTIFG (tx complete)
            if (uart_cnt == 0)
            {
                UCA1IE &= ~UCTXCPTIE; // disable tx complete interrupts
                uart_busy = 0; // not busy
            }
            else
            {
                UCA1TXBUF = *uart_tx_buf_ptr++; // send next byte
                uart_cnt--;
            }
            break;
        
        default: break;
    }
}
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
