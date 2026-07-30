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
#include "kernel/init.h"
#include "kernel/gio.h" // testing
#include "kernel/gstr.h" // testing

// applications
#include "apps/gsh.h"

// devices
#include "devices/rtc.h" // testing

// drivers
#include "drivers/adc.h" // testing


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
    init(); // initialize gort system

    //int gsh = main_gsh(); // launch gort shell

    while (1)
    {
        helloworld(hex(adc_read(0)));
        helloworld("\n");

        //rtc_get();
        
        eep(100);
    } // loop forever
    return 0;
}
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
