/** ---------------------------------------------------------------------------
 * 
 * GORTOS
 * rtc driver library
 * 
 * created by rdupu13
 * 
 * @file rtc.h
 *
----------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
//  HEADER DEFINITION
//-----------------------------------------------------------------------------

#ifndef RTC_H
#define RTC_H


//-----------------------------------------------------------------------------
//  LIBRARIES
//-----------------------------------------------------------------------------

// hardware
#include "hw/pfc.h"



//-----------------------------------------------------------------------------
//  MACROS
//-----------------------------------------------------------------------------

// device specific ----------------------------------------
// ds32318n ---------------------------
#ifdef RTC_DS32318N
#define RTC_SLAVE_ADDR      0x0068
#define RTC_REG_SEC         0x00
#define RTC_REG_CTL         0x0E
#define RTC_WEEKDAY_0       1
#define RTC_SWAP_WEEKDAY    0
#endif
// ------------------------------------

// mcp7940n ---------------------------
#ifdef RTC_MCP7940N
#define RTC_SLAVE_ADDR      0x0068
#define RTC_REG_SEC         0x03
#define RTC_REG_CTL         0x00
#define RTC_WEEKDAY_0       ?
#define RTC_SWAP_WEEKDAY    ?
#endif
// ------------------------------------
// --------------------------------------------------------

#define RTC_STRLEN 19

#define RTC_INIT_DT_STR "12-23-2005 12:24:18"


//-----------------------------------------------------------------------------
//  FUNCTION PROTOTYPES
//-----------------------------------------------------------------------------

extern volatile unsigned char *rtc_display; // register currently being displayed

extern int rtc_error;

int rtc_init(void);    // initialize rtc

int rtc_start(void);   // start rtc
int rtc_stop(void);    // stop rtc
int rtc_get(void);     // get current rtc date and time
int rtc_set(void);     // set current date and time

char *rtc_getstr(void); // get current rtc date and time and convert to string
//void rtc_setdate(int month, int day, int year);
//void rtc_settime(int hour, int minute, int second);

void rtc_display_sel(unsigned char sel);    // select which register to display
void rtc_display_next(void);                // select next register to display


#endif
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
