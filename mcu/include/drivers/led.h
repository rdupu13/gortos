/** ---------------------------------------------------------------------------
 * 
 * GORTOS
 * led driver library
 * 
 * created by rdupu13
 * 
 * @file led.h
 *
----------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
//  HEADER DEFINITION
//-----------------------------------------------------------------------------

#ifndef LED_H
#define LED_H


//-----------------------------------------------------------------------------
//  LIBRARIES
//-----------------------------------------------------------------------------

#include <msp430fr2153.h>


//-----------------------------------------------------------------------------
//  MACROS
//-----------------------------------------------------------------------------

// NOTE: init.c sets all ports + pins to outputs by default

#define LED_HEARTBEAT_PORT  P1OUT
#define LED_HEARTBEAT_PIN   BIT0
#define LED_HEARTBEAT_QDIV  4       // only powers of 2

#define LED_TEST0_PORT      P1OUT
#define LED_TEST0_PIN       BIT1
#define LED_TEST1_PORT      P4OUT
#define LED_TEST1_PIN       BIT5
#define LED_TEST2_PORT      P4OUT
#define LED_TEST2_PIN       BIT4

#define LEDBAR_LEN          10
#define LEDBAR_PORT0        P2OUT
#define LEDBAR_PORT1        P4OUT
#define LEDBAR_BIT8         BIT0
#define LEDBAR_BIT9         BIT1
#define LEDBAR_INIT_SEL     1


//-----------------------------------------------------------------------------
//  FUNCTION PROTOTYPES
//-----------------------------------------------------------------------------

void led_init(void);                          // initialize leds

void led_heartbeat_update(unsigned int qcnt); // update heartbeat led based on quarter-second counter

void ledbar_sel(unsigned char sel);           // select ledbar pointer


#endif
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
