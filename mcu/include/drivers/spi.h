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


//-----------------------------------------------------------------------------
//  MACROS
//-----------------------------------------------------------------------------

/*
#define SPI_CTLW0 // control word 0
#define SPI_BRW // baud rate word
#define SPI_IFG // interrupt flags
#define SPI_IE // interrupt enable
#define SPI_IV // interrupt vector
#define SPI_TXBUF // tx buffer

#define SPI_SWRST // software reset
#define SPI_SSEL // clock source select
#define SPI_SYNC // synchronous mode (??)
#define SPI_MST // master mode
#define SPI_MODE // peripheral mode
#define SPI_STEM // chip select enable
#define SPI_TXIFG // tx complete interrupt flag
#define SPI_TXIE // tx complete interrupt enable
#define SPI_RXIFG // rx buffer full interrupt flag
#define SPI_RXIE // rx buffer full interrupt enable
*/



//-----------------------------------------------------------------------------
//  FUNCTION PROTOTYPES
//-----------------------------------------------------------------------------

void spi_init(
    unsigned int timeout,
    unsigned int clock_div
); // initialize spi

// write an array to a spi slave
int spi_write(
    volatile unsigned char *arr,
    unsigned int len,
    unsigned char slave
);

// read an array from a spi slave
int spi_read(
    volatile unsigned char *arr,
    unsigned int len,
    unsigned char slave,
    volatile unsigned char *addr,
    unsigned int addr_len
);

void spi_start(void);   // set gpio chip select for specific slave
void spi_stop(void);    // clear gpio chip select for specific slave


#endif
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
