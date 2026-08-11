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

#include "drivers/uart.h"

// hardware
#include "hw/pfc.h"


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------

volatile unsigned char *uart_tx_buf_ptr;
volatile unsigned char *uart_rx_buf_ptr;
volatile unsigned int uart_cnt;
volatile unsigned char uart_busy;

volatile unsigned char uart_rxmode;
volatile unsigned char uart_stop;
volatile unsigned char uart_echo;

volatile unsigned char uart_rx_buf[UART_RX_BUF_SIZE];
volatile unsigned char uart_idle_rx;


//-----------------------------------------------------------------------------
//  FUNCTIONS
//-----------------------------------------------------------------------------

/**
 * @brief initialize uart
 * 
 * @param baud baud rate (divided by 100 to fit in int, e.g. 96 for 9600 baud)
 * @param echo echo enable (0 = disabled, 1 = enabled)
 * 
 * @return none
 */
void uart_init(
    unsigned int baud,
    unsigned char echo
) {
    UART_SEL0 |= UART_PINS; // configure pins

    // setup peripheral
    UCA1CTLW0 |= UCSWRST; // put peripheral into software reset
    switch(baud)
    {
        case 96:
            // 9600 baud
            UCA1CTLW0 |= UCSSEL__ACLK;  // clock source = aclk (32.768 kHz)
            UCA1BRW = 3;                // divide brclk by 3
            UCA1MCTLW |= 0x9200;        // modulation = 0x9200 (9600 baud)
            break;
        case 1152:
            // 115200 baud
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
    UCA1IFG = 0;                        // clear interrupt flags
    UCA1IE &= ~(UCTXCPTIE | UCRXIE);    // disable tx/rx interrupts
    
    // initialize variables
    uart_tx_buf_ptr = 0;
    uart_rx_buf_ptr = uart_rx_buf;
    uart_cnt = 0;
    uart_busy = 0;
    uart_rxmode = 0;
    uart_stop = '\r';
    uart_echo = echo;
    uart_idle_rx = 0;
}

/**
 * @brief transmit an array over uart
 * 
 * @param arr pointer to array to be transmitted
 * @param len length in bytes of array to be transmitted
 * 
 * @return status of transmit:
 *               0: ok
 *               1: uart busy
 *              -1: 0 length error
 */
int uart_tx(
    volatile unsigned char *arr, // pronounced "char star arr"
    unsigned int len
) {
    if (len == 0) { return -1; }

    if (uart_busy) { return 1; }
    
    uart_tx_buf_ptr = arr;
    uart_cnt = len - 1;
    
    UCA1IFG = 0;            // clear interrupt flags
    UCA1IE &= ~UCRXIE;      // disable rx interrupts
    UCA1IE |= UCTXCPTIE;    // enable tx complete interrupts
    
    uart_busy = 1;
    UCA1TXBUF = *uart_tx_buf_ptr++; // tx first byte, triggering TXCPTIFG
    
    while(uart_busy) {} // wait until tx done

    UCA1IE &= ~UCTXCPTIE; // disable tx complete interrupts
    
    // restore previous rx interrupt enable state
    if (uart_idle_rx) {
        UCA1IE |= UCRXIE;
    }
    
    return 0;
}

/**
 * @brief receive an array over uart
 * 
 * @param arr   pointer to array to store received data
 * @param len   number of bytes to read (> 0)
 * @param stop  if len = 0, read until this character received
 * 
 * @return status of receive:
 *               0: ok
 *               1: uart busy
 */
int uart_rx(
    volatile unsigned char *arr,
    unsigned int len,
    unsigned char stop
) {
    if (uart_busy) { return 1; }

    uart_rx_buf_ptr = arr;
    uart_cnt = len;
    uart_stop = stop;

    uart_rxmode = (len == 0) ? 1 : 0;
    
    UCA1IFG = 0; // clear interrupt flags
    UCA1IE |= UCRXIE; // enable rx buffer full interrupts
    
    uart_busy = 1;
    while(uart_busy) {} // wait until rx done
    
    // restore previous rx interrupt enable state
    if (!uart_idle_rx) {
        UCA1IE &= ~UCRXIE;
    }

    return 0;
}


//-----------------------------------------------------------------------------
//  INTERRUPT SERVICE ROUTINES
//-----------------------------------------------------------------------------

void __attribute__((interrupt(UART_VECTOR))) isr_uart(void)
{
    switch(UCA1IV)
    {
        case 0x00: break; // no interrupts
        case 0x02:
            // UCRXIFG (rx buffer full)
            UCA1IFG &= ~UCRXIFG; // clear rx buffer full interrupt flag
            *uart_rx_buf_ptr = UCA1RXBUF; // store received byte
            uart_cnt--; // decrement counter
            
            // echo char if enabled
            if (uart_echo)
            {
                UCA1TXBUF = *uart_rx_buf_ptr;
            }

            // if idley receiving, create circular buffer
            if (uart_idle_rx
                && (uart_rx_buf_ptr == (uart_rx_buf + UART_RX_BUF_SIZE - 1))
            ) {
                uart_rx_buf_ptr = uart_rx_buf;
                break;
            }
            
            // check stop criteria
            if ((!uart_rxmode && (uart_cnt == 0))
                || (uart_rxmode && (*uart_rx_buf_ptr == uart_stop))
            ) {
                uart_busy = 0; // not busy
            }
                        
            uart_rx_buf_ptr++; // increment pointer
            break;
        
        case 0x04: break; // UCTXIFG (transmit??)
        case 0x06: break; // UCSTTIFG (start condition)
        case 0x08:
            // UCTXCPTIFG (tx complete)
            UCA1IFG &= ~UCTXCPTIFG; // clear tx complete interrupt flag    
            if (uart_cnt == 0)
            {
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
