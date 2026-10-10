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
    gsys_init(); // initialize gort system

    int gsh_code;
    if (switch_poll(0) != 0)
    {
        gsh_code = main_gsh(); // launch gort shell
        gsys_log("gsh: exited with code:");
        gsys_log(uint_to_dec(gabs(gsh_code)));
    }
    
    while (1)
    {
        gsys_update(1);
    } // halt / loop forever
    return 0;
}
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
