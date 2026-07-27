/** ---------------------------------------------------------------------------
 * 
 * GORTOS
 * spi driver library
 * 
 * created by rdupu13
 * 
 * @file spi.h
 *
----------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
//  HEADER DEFINITION
//-----------------------------------------------------------------------------

#ifndef SPI_H
#define SPI_H


//-----------------------------------------------------------------------------
//  LIBRARIES
//-----------------------------------------------------------------------------

#include <msp430fr2153.h>
#include <stdint.h>


//-----------------------------------------------------------------------------
//  MACROS
//-----------------------------------------------------------------------------

#define SPI_SEL0        P1SEL0
#define SPI_PINS        BIT4 | BIT5 | BIT6 | BIT7

#define SPI_BUF_SIZE    255


//-----------------------------------------------------------------------------
//  FUNCTION PROTOTYPES
//-----------------------------------------------------------------------------

volatile uint8_t spi_tx_buf[SPI_BUF_SIZE];
volatile uint8_t spi_rx_buf[SPI_BUF_SIZE];
volatile uint8_t *spi_tx_buf_ptr;
volatile uint8_t *spi_rx_buf_ptr;

volatile uint8_t spi_len;

void spi_init();                                                    // initialize spi
//void spi_write(uint8_t reg_addr, uint8_t len, uint8_t *data);  // write an array to a spi slave
//uint8_t *spi_read(uint8_t reg_addr, uint8_t len);              // read an array from a spi slave


#endif
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
