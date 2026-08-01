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

#define SPI_CS1_PORT // chip select 1 port
#define SPI_CS1_PIN // chip select 1 port

#define SPI_CS2_PORT // chip select 2 port
#define SPI_CS2_PIN // chip select 2 port

#define SPI_CS3_PORT // chip select 3 port
#define SPI_CS3_PIN // chip select 3 port
*/



//-----------------------------------------------------------------------------
//  FUNCTION PROTOTYPES
//-----------------------------------------------------------------------------

void spi_init(unsigned int timeout); // initialize spi

// write an array to a spi slave
int spi_write(
    unsigned char *arr,
    unsigned int len,
    unsigned char slave
);

// read an array from a spi slave
int spi_read(
    unsigned char *arr,
    unsigned int len,
    unsigned char slave
);


#endif
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
