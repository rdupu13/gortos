/** ---------------------------------------------------------------------------
 * 
 * GORTOS
 * gort kernel
 * 
 * created by rdupu13
 * 
 * @file main.c
 *
----------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
//  LIBRARIES
//-----------------------------------------------------------------------------

// drivers
#include "drivers/timer.h"
#include "drivers/uart.h"
#include "drivers/i2c.h"
#include "drivers/led.h"
#include "drivers/switch.h"
#include "drivers/rtc.h"

// kernel
#include "kernel/gio.h"
#include "kernel/gstr.h"
#include "kernel/gsys.h"
#include "kernel/init.h"

// applications
#include "apps/gsh.h"

//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//  FUNCTIONS
//-----------------------------------------------------------------------------

/**
 * @brief gort kernel!
 * 
 * @param none
 * @return never returns
 */
int main(void)
{
    init(); // initialize gort system

    int gsh = main_gsh();

    while (1) {} // loop forever
    return 0;
}
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
