#include <msp430fr2153.h>
#include "uart.h"

/**
 * initialize uart
 */
void uart_init()
{
    UART_SEL0 |= UART_PINS; // set pins

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
            UCA1CTLW0 |= UCSSEL__SMCLK; // clock source = smclk (1 MHz)
            UCA1BRW = 8;                // divide brclk by 8
            UCA1MCTLW |= 0xD600;        // modulation = 0xD608 (115200 baud)
            break;
    }
    UCA1CTLW0 &= ~UCSWRST; // take peripheral out of software reset
    
    // initialize variables
    uart_tx_buf_ptr = 0;
    uart_rx_buf_ptr = uart_rx_buf;
    uart_stop = '\r';
    uart_tx_done = 1;
    uart_rx_done = 1;
    uart_n = 0;
    uart_cnt = 1;

    //__delay_cycles(100); // maybe delay?
}

/**
 * transmit an array of data over uart
 */
void uart_tx(uint8_t *data)
{
    if (!uart_tx_done) {
        return;
    }

    uart_tx_done = 0; // clear done condition
    uart_tx_buf_ptr = data; // point to tx data
    
    UCA1IE |= UCTXCPTIE; // enable tx complete interrupts
    UCA1IFG &= ~UCTXCPTIFG; // clear tx complete interrupt flag
    
    UCA1TXBUF = *uart_tx_buf_ptr++; // tx first byte, triggering TXCPTIFG

    while(!uart_tx_done) {} // wait until tx done
}

/**
 * receive n bytes over uart, if n = 0, until stop
 */
uint8_t *uart_rx(uint8_t n, uint8_t stop)
{
    if (!uart_rx_done) {
        return 0;
    }

    uart_n = n;
    uart_cnt = n;
    uart_stop = stop;

    uart_rx_done = 0; // clear done condition
    uart_rx_buf_ptr = uart_rx_buf; // reset buffer pointer
    
    UCA1IE |= UCRXIFG; // enable rx interrupts
    UCA1IFG &= ~UCRXIFG; // clear rx interrupt flag
    while(!uart_rx_done) {} // wait until stop condition

    return uart_rx_buf; // return pointer to start of buffer
}

/**
 * interrupt service routines
 */
#pragma vector = EUSCI_A1_VECTOR
__interrupt void eusci_a1_isr(void)
{
    switch(__even_in_range(UCA1IV, 18))
    {
        case 0x00: break; // no interrupts
        case 0x02:
            // UCRXIFG (received byte)
            *uart_rx_buf_ptr = UCA1RXBUF; // store received byte

            if (UART_ECHO) { UCA1TXBUF = *uart_rx_buf_ptr; } // echo char if enabled
            if (uart_n != 0) { uart_cnt--; } // decrement counter
            
            // stop condition
            if (((uart_n != 0) && (uart_cnt == 0))
                || ((uart_n == 0) && (*uart_rx_buf_ptr == uart_stop)))
            {
                UCA1IE &= ~UCRXIFG;
                uart_rx_done = 1;
            }
            uart_rx_buf_ptr++; // increment pointer
            break;
        
        case 0x04: break; // UCTXIFG (transmit??)
        case 0x06: break; // UCSTTIFG (start condition)
        case 0x08:
            // UCTXCPTIFG (transmit complete)
            if (*uart_tx_buf_ptr == '\0')
            {
                UCA1IE &= ~UCTXCPTIE; // disable transmit complete interrupts
                uart_tx_done = 1; // set done condition
            }
            else
            {
                UCA1TXBUF = *uart_tx_buf_ptr++; // tx next byte
            }
            break;
        
        default: break;
    }
}
