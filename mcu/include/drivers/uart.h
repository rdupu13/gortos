/** ---------------------------------------------------------------------------
 * 
 * GORTOS
 * uart driver library
 * 
 * created by rdupu13
 * 
 * @file uart.h
 *
----------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
//  HEADER DEFINITION
//-----------------------------------------------------------------------------

#ifndef UART_H
#define UART_H


//-----------------------------------------------------------------------------
//  LIBRARIES
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//  MACROS
//-----------------------------------------------------------------------------

/*
#define UART_CTLW0 // control word 0
#define UART_BRW // baud rate word
#define UART_MCTLW // modulation control word
#define UART_IE // interrupt enable
#define UART_IFG // interrupt flags
#define UART_IV // interrupt vector
#define UART_TXBUF // tx buffer
#define UART_RXBUF // rx buffer

#define UART_SWRST // software reset
#define UART_SSEL // clock source select
#define UART_TXCPTIE // tx complete interrupt enable
#define UART_TXCPTIFG // tx complete interrupt flag
#define UART_RXIE // rx buffer full interrupt enable
#define UART_RXIFG // rx buffer full interrupt flag
*/

#define UART_RX_BUF_SIZE 64

//-----------------------------------------------------------------------------
//  FUNCTION PROTOTYPES
//-----------------------------------------------------------------------------

// initialize uart
void uart_init(
    unsigned int baud,
    unsigned char echo
);

// transmit an array over uart
int uart_tx(
    volatile unsigned char *arr,
    unsigned int len
);

// receive an array over uart
int uart_rx(
    volatile unsigned char *arr,
    unsigned int len,
    unsigned char stop
);


#endif
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
