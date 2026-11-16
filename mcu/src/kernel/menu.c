/** ---------------------------------------------------------------------------
 * 
 * GORTOS
 * lcd menu system
 * 
 * created by rdupu13
 * 
 * @file menu.c
 *
----------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
//  LIBRARIES
//-----------------------------------------------------------------------------

#include "kernel/gsys.h"

// hardware
#include "hw/pfc.h"

// drivers
#include "drivers/adc.h"
#include "drivers/i2c.h"
#include "drivers/led.h"
#include "drivers/patterns.h"
#include "drivers/spi.h"
#include "drivers/switch.h"
#include "drivers/timer.h"
#include "drivers/uart.h"

// devices
#include "devices/dial.h"
#include "devices/lcd.h"
#include "devices/rtc.h"
#include "devices/temp.h"
#include "devices/rot.h"

#include "kernel/gio.h"
#include "kernel/gstr.h"


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------

unsigned char cur_menu;



//-----------------------------------------------------------------------------
//  FUNCTIONS
//-----------------------------------------------------------------------------


/**
 * @brief 
 * 
 * @return 
 */
void (void)
{
    
}

/**
 * @brief 
 * 
 * @return 
 */
void (void)
{
}
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
