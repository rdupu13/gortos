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

    //int gsh = main_gsh(); // launch gort shell

    while (1)
    {
        //helloworld(hex(adc_read(0)));
        //helloworld("\n");

        //rtc_get();
        //eep(610);

        volatile unsigned char mode_reg;
        volatile unsigned char instr = 0x05; // read mode register
        spi_write(&instr, 1, MMM_SPI_SLAVE_NUM);
        spi_read(&mode_reg, 1, MMM_SPI_SLAVE_NUM);
        helloworld(hex((unsigned int) mode_reg));
        eep(610);

        //rtc_get();
        //eep(50);
        //switch_wait(0, 0);
        //eep(50);
        //switch_wait(0, 1);
        
    } // halt / loop forever
    return 0;
}
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
