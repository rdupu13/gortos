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

#define INIT_EEP_PERIOD_MS 2000

#define DELAY_LOOP_MS 103

#define BLK_SIZE_P2 8
#define BLK_SIZE 256 // 2^(BLK_SIZE_P2)

#define BLK_ALLOC_NUM 2


//-----------------------------------------------------------------------------
//  FUNCTION PROTOTYPES
//-----------------------------------------------------------------------------

// blk_t --------------------------------------------------
typedef struct
{
    // TODO: maybe just store block address on device?
    unsigned int num; // block number, used to load/store from device
    volatile unsigned char *data;
}
gblk_t;
// --------------------------------------------------------

// gtime_t ------------------------------------------------
// TODO: use
typedef struct
{
    unsigned char second;
    unsigned char minute;
    unsigned char hour;
    unsigned char weekday;
    unsigned char date;
    unsigned char month;
    unsigned int year;
}
gtime_t;
// --------------------------------------------------------

void gsys_init(void); // initialize gort system

void gsys_log(char *entry); // add an entry to gort's diary
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
