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

// kernel
#include "kernel/gsys.h"
#include "kernel/gio.h" // testing
#include "kernel/gstr.h" // testing

// applications
#include "apps/gsh.h"

// devices
#include "devices/rtc.h" // testing

// drivers
#include "drivers/adc.h" // testing
#include "drivers/led.h" // testing


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
 * @return never returns!
 */
int main(void)
{
    gsys_init(); // initialize gort system

    //int gsh = main_gsh(); // launch gort shell

    while (1)
    {
        //helloworld(hex(adc_read(0)));
        //helloworld("\n");

        rtc_get();

        eep(61);
    } // halt / loop forever
    return 0;
}
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
