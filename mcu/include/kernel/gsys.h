/** ---------------------------------------------------------------------------
 * 
 * GORTOS
 * gort system library
 * 
 * created by rdupu13
 * 
 * @file gsys.h
 *
----------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
//  HEADER DEFINITION
//-----------------------------------------------------------------------------

#ifndef GSYS_H
#define GSYS_H


//-----------------------------------------------------------------------------
//  LIBRARIES
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//  MACROS
//-----------------------------------------------------------------------------

#define INIT_EEP_PERIOD_MS  2000

#define DELAY_LOOP_MS 103


//-----------------------------------------------------------------------------
//  FUNCTION PROTOTYPES
//-----------------------------------------------------------------------------

void gsys_init(void); // initialize gort system

void eep(unsigned int delay); // take a gort nap

// rtc
void print_systime(void); // write current gort system time to gout

// led
void blinky(unsigned char led, unsigned int delay); // blink a test led
void ledbar_sel(unsigned char sel);                 // select current thing displayed on ledbar

// interrupt service routines (for drivers only): ---------
// timer
void qcnt_update(unsigned int qcnt);
void fcnt_update(unsigned int fcnt);
// switch
void switch_0_pressed(void);
void switch_1_pressed(void);
// --------------------------------------------------------


#endif
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
