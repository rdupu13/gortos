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


//-----------------------------------------------------------------------------
//  MACROS
//-----------------------------------------------------------------------------

#define LED_HEARTBEAT_QDIV  4 // toggles at 1 Hz

#define LEDBAR_LEN          10
#define LEDBAR_INIT         0xFFFF


//-----------------------------------------------------------------------------
//  FUNCTION PROTOTYPES
//-----------------------------------------------------------------------------

void led_init(void);                            // initialize leds

void led_heartbeat_update(unsigned int qcnt);   // update heartbeat led based on quarter-second counter

void led_on(unsigned char n);              // turn on a test led
void led_off(unsigned char n);             // turn off a test led
void led_test_toggle(unsigned char n);          // toggle a test led

void ledbar_setpins(unsigned int n);            // set ledbar pins


#endif
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
