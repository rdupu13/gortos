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


//-----------------------------------------------------------------------------
//  MACROS
//-----------------------------------------------------------------------------

// device specific ----------------------------------------
// ds32318n ---------------------------
#define DS32318N_SLAVE_ADDR     0x0068
#define DS32318N_REG_SEC        0x00
#define DS32318N_REG_CTL        0x0E
// ------------------------------------

/* mcp7940n ---------------------------
#define MCP7940N_SLAVE_ADDR     0x0068
#define MCP7940N_REG_SEC        0x03
#define MCP7940N_REG_CTL        0x00
// ------------------------------------*/
// --------------------------------------------------------

// current device ---------------------
#define RTC_SLAVE_ADDR  DS32318N_SLAVE_ADDR
#define RTC_REG_SEC     DS32318N_REG_SEC
#define RTC_REG_CTL     DS32318N_REG_CTL
// ------------------------------------

#define RTC_STRLEN 19


//-----------------------------------------------------------------------------
//  FUNCTION PROTOTYPES
//-----------------------------------------------------------------------------

volatile unsigned char *rtc_display; // register currently being displayed


void rtc_init(void);    // initialize rtc
void rtc_start(void);   // start rtc
void rtc_stop(void);    // stop rtc
void rtc_get(void);     // get current rtc date and time
void rtc_set(void);     // set current date and time

char *rtc_getstr(void); // get current rtc date and time and convert to string
//void rtc_setdate(int month, int day, int year);
//void rtc_settime(int hour, int minute, int second);

void rtc_display_sel(unsigned char sel);    // select which register to display
void rtc_display_next(void);                // select next register to display


#endif
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
