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

void gsys_init(void);               // initialize gort system

void eep(unsigned int delay);       // take a gort nap
void blinky(unsigned int delay);    // blink the TEST0 LED for delay ms
void print_systime(void);           // write current gort system time to gout

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
