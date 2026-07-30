/** ---------------------------------------------------------------------------
 * 
 * GORTOS
 * switch driver library
 * 
 * created by rdupu13
 * 
 * @file switch.h
 *
----------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
//  HEADER DEFINITION
//-----------------------------------------------------------------------------

#ifndef SWITCH_H
#define SWITCH_H


//-----------------------------------------------------------------------------
//  LIBRARIES
//-----------------------------------------------------------------------------

#include <msp430fr2153.h>


//-----------------------------------------------------------------------------
//  MACROS
//-----------------------------------------------------------------------------

#define SWITCH_INT_EN   1

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


//-----------------------------------------------------------------------------
//  FUNCTION PROTOTYPES
//-----------------------------------------------------------------------------

void switch_init(void);                     // initialize switches

unsigned char switch_poll(unsigned char n); // 

#endif
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
