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
#include "drivers/switch.h"

// devices


// kernel
#include "kernel/gstr.h"
#include "kernel/gio.h"
#include "kernel/gsys.h" // initialization


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
    // initialize gort system
    gsys_init();

    int gsh_code;
    // if switch 0 is pressed after system init, go to gsh
    if (switch_poll(0) != 0)
    {
        // launch gort shell
        gsh_code = main_gsh();
        
        gsys_log("gsh: exited with code:");
        gsys_log(uint_to_dec(gabs(gsh_code)));
    }

    // if switch 1 is pressed after system init, display lcd test
    if (switch_poll(1) != 0)
    {
        lcd_test();
        gout = 1;
        glear();
        gout = 0;
    }
    
    // begin default execution loop
    while (1)
    {
        gsys_update();
    } // halt / loop forever
    return 0;
}
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
