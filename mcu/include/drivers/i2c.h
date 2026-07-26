/** ---------------------------------------------------------------------------
 * 
 * GORTOS
 * i2c driver library
 * 
 * created by rdupu13
 * 
 * @file i2c.h
 *
----------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
//  HEADER DEFINITION
//-----------------------------------------------------------------------------

#ifndef I2C_H
#define I2C_H


//-----------------------------------------------------------------------------
//  LIBRARIES
//-----------------------------------------------------------------------------

#include <msp430fr2153.h>
#include <stdint.h>

#include "drivers/led.h"


//-----------------------------------------------------------------------------
//  MACROS
//-----------------------------------------------------------------------------

#define I2C_SEL0        P4SEL0
#define I2C_PINS        BIT6 | BIT7

#define I2C_BUSY_PORT   LED_TEST1_PORT
#define I2C_BUSY_PIN    LED_TEST1_PIN

#define I2C_BUF_SIZE    255


//-----------------------------------------------------------------------------
//  FUNCTION PROTOTYPES
//-----------------------------------------------------------------------------

volatile uint8_t i2c_tx_buf[I2C_BUF_SIZE];
volatile uint8_t i2c_rx_buf[I2C_BUF_SIZE];
volatile uint8_t *i2c_tx_buf_ptr;
volatile uint8_t *i2c_rx_buf_ptr;

volatile uint8_t i2c_len;
volatile uint8_t i2c_mode;
volatile uint8_t i2c_busy;

void i2c_init();                                                                    // initialize i2c
void i2c_write(uint16_t slave_addr, uint8_t reg_addr, uint8_t len, uint8_t *data);  // write an array to an i2c slave
uint8_t *i2c_read(uint16_t slave_addr, uint8_t reg_addr, uint8_t len);              // read an array from an i2c slave


#endif
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
