/** ---------------------------------------------------------------------------
 * 
 * GORTOS
 * temperature sensor driver
 * 
 * created by rdupu13
 * 
 * @file temp.c
 *
----------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
//  LIBRARIES
//-----------------------------------------------------------------------------

#include "devices/temp.h"

// drivers
#include "drivers/adc.h"


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------

int temp_window[INIT_WINDOW_SIZE];
unsigned char window_size = INIT_WINDOW_SIZE;
int temp_avg;

//-----------------------------------------------------------------------------
//  FUNCTIONS
//-----------------------------------------------------------------------------

/**
 * @brief convert an analog temperature reading into degrees celsius
 * 
 * @param analog adc value from temperature sensor
 * 
 * @return temperature in celsius
 */
int temp_analog(unsigned int analog)
{
    unsigned int masked = (analog & 0x7FF);

    masked = ((masked >> 5) + (masked >> 6));   // magic division
    int temp = -masked + TEMP_OFFSET;           // magic number

    return temp;
}

/**
 * @brief 
 * 
 * @return 
 */
int temp_analog_avg(unsigned int analog)
{
    int new_temp = temp_analog(analog);

    unsigned int i;
    for (i = window_size - 1; i > 0; i--)
    {
        temp_window[i] = temp_window[i - 1];
    }
    temp_window[0] = new_temp;

    temp_avg = 0;
    for (i = 0; i < window_size; i++)
    {
        temp_avg += temp_window[i];
    }

    temp_avg /= window_size;

    return temp_avg;
}

void temp_window_set_size(unsigned int size)
{
    window_size = size;
}
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
