/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 * --
 * -- Description:  Interface of module adc.
 * --
 * --
 * -- $Id: adc.h 1892 2015-04-13 07:11:19Z ruan $
 * ------------------------------------------------------------------------- */

/* re-definition guard */
#ifndef _TIMER_H
#define _TIMER_H

/* standard includes */
#include <stdint.h>


/* -- Type definitions
 * ------------------------------------------------------------------------- */

typedef enum {
    ADC_RES_12BIT = (0x0 << 24u),
    ADC_RES_10BIT = (0x1 << 24u),
    ADC_RES_8BIT = (0x2 << 24u),
    ADC_RES_6BIT = (0x3 << 24u),
} adc_resolution_t;


/* -- Public function declarations
 * ------------------------------------------------------------------------- */


/*
 * Initializes adc on channel/pin PF.6 (POT1 on CT Board)
 */
void adc_init(void);

/*
 * Sets the specified resolution, starts a single conversion, waits for the
 * end of conversion and returns the result of the conversion.
 * The number of valid bits in the returned result depends on the resolution.
 * The bits are right aligned.
 */
uint16_t adc_get_value(adc_resolution_t resolution);

/*
 * Moving average filter
 * Contains a FIFO with the history of the last 16 adc_values.
 * adc_value is added to the FIFO and the average over the whole FIFO is
 * calculated and returned.
 */
uint16_t adc_filter_value(uint16_t adc_value);
#endif
