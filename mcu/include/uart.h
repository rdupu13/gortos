#ifndef UART_H
#define UART_H

#include <msp430fr2153.h>
#include <stdint.h>

#define UART_SEL0       P4SEL0
#define UART_PINS       BIT2 | BIT3
#define UART_BAUD       115200
#define UART_ECHO       1
#define UART_BUF_SIZE   64

volatile uint8_t uart_rx_buf[UART_BUF_SIZE];
volatile uint8_t *uart_tx_buf_ptr;
volatile uint8_t *uart_rx_buf_ptr;
volatile uint8_t uart_n;
volatile uint8_t uart_cnt;
volatile uint8_t uart_stop;
volatile uint8_t uart_tx_done;
volatile uint8_t uart_rx_done;

void uart_init(); // initialize uart
void uart_tx(uint8_t *data); // transmit an array of data over uart
uint8_t *uart_rx(uint8_t n, uint8_t stop); // receive n bytes over uart, if n = 0, until stop

#endif