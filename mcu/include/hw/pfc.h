/** ---------------------------------------------------------------------------
 * 
 * GORTOS
 * port function control
 * 
 * created by rdupu13
 * 
 * @file pfc.h
 * controls the port functions of the specific mcu in use
 * 
 * mcu: msp430fr2153
 * package: tdbt
 *
----------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
//  HEADER DEFINITION
//-----------------------------------------------------------------------------

#ifndef PFC_H
#define PFC_H


//-----------------------------------------------------------------------------
//  LIBRARIES
//-----------------------------------------------------------------------------

// hardware
#include <msp430fr2153.h>


//-----------------------------------------------------------------------------
//  MACROS
//-----------------------------------------------------------------------------

// adc ----------------------------------------------------
// channel 0 = A2
#define ADC_CH0_SEL0    P1SEL0
#define ADC_CH0_SEL1    P1SEL1
#define ADC_CH0_PIN     BIT2
#define ADC_CH0_INCH    ADCINCH_2
// TODO: peripheral base addr
// --------------------------------------------------------

// i2c ----------------------------------------------------
#define I2C_SEL0        P4SEL0
#define I2C_PINS        BIT6 | BIT7
// TODO: peripheral base addr
// --------------------------------------------------------

// led ----------------------------------------------------
#define LED_HEARTBEAT_PORT  P1OUT
#define LED_HEARTBEAT_PIN   BIT0

#define LED_TEST0_PORT      P1OUT
#define LED_TEST0_PIN       BIT1
#define LED_TEST1_PORT      P4OUT
#define LED_TEST1_PIN       BIT5
#define LED_TEST2_PORT      P4OUT
#define LED_TEST2_PIN       BIT4

#define LEDBAR_PORT0        P2OUT
#define LEDBAR_PORT1        P4OUT
#define LEDBAR_BIT8         BIT0
#define LEDBAR_BIT9         BIT1
// --------------------------------------------------------

// spi ----------------------------------------------------
#define SPI_SEL0        P1SEL0
#define SPI_PINS        BIT7 | BIT6 | BIT5 | BIT4
// TODO: peripheral base addr
// --------------------------------------------------------

// switch -------------------------------------------------
#define SWITCH_PORT_VECTOR  PORT3_VECTOR
#define SWITCH_IV           P3IV

#define SWITCH0_DIR     P3DIR
#define SWITCH0_REN     P3REN
#define SWITCH0_OUT     P3OUT
#define SWITCH0_IN      P3IN
#define SWITCH0_IES     P3IES
#define SWITCH0_IE      P3IE
#define SWITCH0_IFG     P3IFG
#define SWITCH0_PIN     BIT7
#define SWITCH0_IV_VAL  0x10

#define SWITCH1_DIR     P3DIR
#define SWITCH1_REN     P3REN
#define SWITCH1_OUT     P3OUT
#define SWITCH1_IN      P3IN
#define SWITCH1_IES     P3IES
#define SWITCH1_IE      P3IE
#define SWITCH1_IFG     P3IFG
#define SWITCH1_PIN     BIT6
#define SWITCH1_IV_VAL  0x0E
// --------------------------------------------------------

// timer --------------------------------------------------
// TODO: carry over timer stuff
// TODO: peripheral base addr
// --------------------------------------------------------

// uart ---------------------------------------------------
#define UART_SEL0       P4SEL0
#define UART_PINS       BIT3 | BIT2
// TODO: peripheral base addr
// --------------------------------------------------------


//-----------------------------------------------------------------------------
//  FUNCTION PROTOTYPES
//-----------------------------------------------------------------------------


#endif
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
