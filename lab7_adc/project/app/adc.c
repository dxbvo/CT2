/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 * --
 * -- Description:  Implementation of module adc.
 * --
 * --
 * -- $Id: adc.c 5605 2023-01-05 15:52:42Z frtt $
 * ------------------------------------------------------------------------- */

/* standard includes */
#include <stdint.h>
#include <reg_stm32f4xx.h>

/* user includes */
#include "adc.h"


/* -- Macros
 * ------------------------------------------------------------------------- */

#define PERIPH_GPIOF_ENABLE (0x00000020)
#define PERIPH_ADC3_ENABLE  (0x00000400)

/* Configuring pin for ADC: PF.6 */
#define GPIOF_MODER_ANALOG (0x3 << 12)


/* -- Macros used by student code
 * ------------------------------------------------------------------------- */

/// STUDENTS: To be programmed

/// END: To be programmed


/* Public function definitions
 * ------------------------------------------------------------------------- */

/*
 *  See header file
 */
void adc_init(void)
{
    /* Enable peripheral clocks */
    RCC->AHB1ENR |= PERIPH_GPIOF_ENABLE;
    RCC->APB2ENR |= PERIPH_ADC3_ENABLE;

    /* Configure PF.6 as input */
    GPIOF->MODER |= GPIOF_MODER_ANALOG;

    /* ADC common init */
    ADCCOM->CCR = 0;        // TSVREF = '0'    -> Temp sensor disabled
                            // VBATE = '0'     -> VBAT disabled
                            // ADCPRE = '00'   -> APB2 / 2 -> 21 MHz
                            // DMA = '00'      -> DMA disabled
                            // DELAY = '0000'  -> Delay 5 cycles
                            // MULTI = '00000' -> ADC independent mode

    /* Configure ADC3 */
    
    /// STUDENTS: To be programmed
	
	    // Make sure only one sequence is enabled
    ADC3->SQR1 = 0x0;
    ADC3->SQR2 = 0x0;
    ADC3->SQR3 = 0x4;
    // no scan
    ADC3->CR1 = 0x0;
    // enable ADC
    ADC3->CR2 = 0x1;
	
    /// END: To be programmed
}

/*
 *  See header file
 */
uint16_t adc_get_value(adc_resolution_t resolution)
{
    uint16_t adc_value = 0;

    /// STUDENTS: To be programmed
	
	    // reset resolution bits (24th and 25th) to 0
    ADC3->CR1 &= (uint32_t)~(0x3 << 24);
    // set given resolution
    ADC3->CR1 |= resolution;
    // setting the SWSTART bit in the ADC3->CR2 register to start conversion
    ADC3->CR2 |= (0x1 << 30);
    // The 2nd bit of the SR register (0x2) is the End of Conversion (EOC) flag.
		// If flag is 1, it means that the conversion process is complete
    while (!(ADC3->SR & 0x2))
    // Get value from dataregister
    adc_value = (uint16_t)ADC3->DR;

    /// END: To be programmed

    return adc_value;
}

/*
 *  See header file
 */
uint16_t adc_filter_value(uint16_t adc_value)
{
    uint16_t filtered_value = 0;

    /// STUDENTS: To be programmed
	
	  static uint16_t values[16] = {0};
    static uint8_t index = 0;
    uint32_t sum = 0;

    values[index] = adc_value;
    index = (index + 1) % 16;

    for (uint8_t i = 0; i < 16; i++) {
        sum += values[i];
    }

    filtered_value = (uint16_t)(sum / 16);

    /// END: To be programmed

    return filtered_value;
}
