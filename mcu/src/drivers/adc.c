/** ---------------------------------------------------------------------------
 * 
 * GORTOS
 * adc driver
 * 
 * created by rdupu13
 * 
 * @file adc.c
 *
----------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
//  LIBRARIES
//-----------------------------------------------------------------------------

#include "drivers/adc.h"

// hardware
#include "hw/pfc.h"


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------

volatile unsigned int adc_val;
volatile unsigned char adc_busy;


//-----------------------------------------------------------------------------
//  FUNCTIONS
//-----------------------------------------------------------------------------

void adc_sel(unsigned char ch);

/**
 * @brief initialize adc
 * 
 * @param none
 * @return none
 */
void adc_init(void)
{
    // configure pins -----------------
    ADC_CH0_SEL0 |= ADC_CH0_PIN;
    ADC_CH0_SEL1 |= ADC_CH0_PIN;

    ADC_CH1_SEL0 |= ADC_CH1_PIN;
    ADC_CH1_SEL1 |= ADC_CH1_PIN;
    // --------------------------------
    
    // configure peripheral
    ADCCTL1 &= ~ADCSHS; ADCCTL1 |= ADCSHP;      // sample signal source = sampling timer
    ADCCTL1 |= ADCSSEL_2;                       // clock source = smclk (1 MHz)
    ADCCTL1 &= ~ADCDIV;                         // divide smclk by 1
    ADCCTL0 &= ~ADCSHT; ADCCTL0 |= ADCSHT_2;    // sample frequency = adcclk / 16 (62.5 kHz)
    ADCCTL2 &= ~ADCRES; ADCCTL2 |= ADCRES_2;    // resolution = 12-bit
    ADCMCTL0 &= ~ADCSREF;                       // reference voltages = VCC & VSS

    ADCCTL0 &= ~ADCON; // turn adc off
    ADCCTL0 &= ~ADCENC; // disable conversion
    ADCIE &= ~ADCIE0; // disable conversion complete interrupts
    ADCIFG &= ~ADCIFG0; // clear conversion complete interrupt flag

    // select input channel
    adc_sel(ADC_INIT_CH);

    // initialize variables
    adc_val = 0;
    adc_busy = 0;
}

/**
 * @brief select adc channel
 * 
 * @param ch channel number
 * 
 * @return none
 */
void adc_sel(unsigned char ch)
{
    ADCMCTL0 &= ~ADCINCH; // select channel A0 by default
    switch(ch)
    {
        case 0: ADCMCTL0 |= ADC_CH0_INCH; break;
        case 1: ADCMCTL0 |= ADC_CH1_INCH; break;
        default: ADCMCTL0 |= ADC_CH0_INCH; break;
    }
}

/**
 * @brief read the value from an adc channel
 * 
 * @param ch selected channel
 * 
 * @return digitized value from selected channel
 */
unsigned int adc_read(unsigned char ch)
{
    while (adc_busy) {} // wait until previous conversion done

    adc_sel(ch); // select channel
    
    ADCCTL0 |= ADCON; // turn on adc
    ADCCTL0 |= ADCENC; // enable conversion
    ADCIE |= ADCIE0; // enable conversion complete interrupts
    ADCIFG &= ~ADCIFG0; // clear conversion complete interrupt flag
    
    adc_busy = 1;
    ADCCTL0 |= ADCSC; // start sample and conversion (triggers ISR)
    while(adc_busy) {} // wait until done converting

    return adc_val;
}

//-----------------------------------------------------------------------------
//  INTERRUPT SERVICE ROUTINES
//-----------------------------------------------------------------------------

void __attribute__((interrupt(ADC_VECTOR))) isr_adc(void)
{
    adc_val = ADCMEM0; // store adc value (clears interrupt flag)

    ADCCTL0 &= ~ADCON; // turn off adc
    ADCCTL0 &= ~ADCENC; // disable conversion
    ADCIE &= ~ADCIE0; // disable conversion complete interrupts
    adc_busy = 0;
}
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
