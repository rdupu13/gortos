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
 * @note LEAST SIGNIFICANT DIGIT IS 1/10THS PLACE
 * 
 * @param analog adc value from temperature sensor
 * 
 * @return temperature in celsius
 */
int temp_analog(unsigned char adc_ch)
{
    unsigned int analog = adc_read(adc_ch);

    // T(A) = 1989 - 0.5*A
    int temp = 1989 - (analog >> 1);
    
    return temp;
}

/**
 * @brief 
 * 
 * @return 
 */
int temp_analog_avg(unsigned char adc_ch)
{
    unsigned int temp_avg;
    unsigned int analog = adc_read(adc_ch);
    int new_temp = temp_analog(analog);

    // move window # by #
    unsigned int i;
    for (i = window_size - 1; i > 0; i--)
    {
        temp_window[i] = temp_window[i - 1];
    }

    // add the new temperature value
    temp_window[0] = new_temp;

    // calculate average of window
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
