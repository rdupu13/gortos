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

#include <msp430fr2153.h>
#include <stdint.h>


//-----------------------------------------------------------------------------
//  MACROS
//-----------------------------------------------------------------------------

#define UART_SEL0       P4SEL0
#define UART_PINS       BIT2 | BIT3
#define UART_BAUD       115200
#define UART_ECHO       1
#define UART_BUF_SIZE   64


//-----------------------------------------------------------------------------
//  FUNCTION PROTOTYPES
//-----------------------------------------------------------------------------

volatile uint8_t uart_rx_buf[UART_BUF_SIZE];
volatile uint8_t *uart_tx_buf_ptr;
volatile uint8_t *uart_rx_buf_ptr;
volatile uint8_t uart_n;
volatile uint8_t uart_cnt;
volatile uint8_t uart_stop;
volatile uint8_t uart_tx_done;
volatile uint8_t uart_rx_done;

void uart_init();                           // initialize uart
void uart_tx(uint8_t *arr);                 // transmit a null-terminated array over uart
uint8_t *uart_rx(uint8_t n, uint8_t stop);  // receive an array over uart


#endif
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
