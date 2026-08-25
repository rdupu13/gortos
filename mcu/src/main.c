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
#include "drivers/adc.h" // testing
#include "drivers/led.h" // testing

// devices
#include "devices/mmm.h" // testing
#include "devices/rtc.h" // testing

// kernel
#include "kernel/gsys.h"
#include "kernel/gio.h" // testing
#include "kernel/gstr.h" // testing

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
 * @return never!
 */
int main(void)
{
    gsys_init(); // initialize gort system

    int gsh = main_gsh(); // launch gort shell

    while (1)
    {
        //helloworld(hex(adc_read(0)));
        //helloworld("\n");

        //print_systime();
        //helloworld("\n");
        //eep(610);

        //int stat = rtc_get();
        //if (stat) {
        //    gsys_log("rtc: error");
        //    helloworld(hex(gabs(stat)));
        //}
        //eep(610);

        //eep(610);
        //mmm_init();
        
    } // halt / loop forever
    return 0;
}
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
