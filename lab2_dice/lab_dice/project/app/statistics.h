/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zurich University of             -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                 -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Description:  Interface of module statistics
 * --               Collect and provide statistical data on throws
 * --               of the dice
 * --
 * -- $Id: statistics.h 1258 2015-02-04 15:25:43Z ruan $
 * ------------------------------------------------------------------
 */

/* re-definition guard */
#ifndef _HAL_CT_STATISTICS_H
#define _HAL_CT_STATISTICS_H

/* standard includes */
#include <stdint.h>

/* macros visible outside of module */
#define NR_OF_DICE_VALUES 6
#define ERROR_VALUE       0xFF

/* function declarations */

/*
 * Increments the total number of throws as well as the number for the throws
 * with the result throw_value.
 * throw_value has to be in the range of 1 up to NR_OF_DICE_VALUES, otherwise
 * it will be ignored
 */
void stat_add_throw(uint8_t throw_value);

/*
 * Return the number of throws with the result 'dice_number'
 * For 'dice_number' equal zero the total number of throws will be returned.
 * If 'dice_number' is above NR_OF_DICE_VALUES the error code ERROR_VALUE will
 * be returned.
 */
uint8_t stat_read(uint8_t dice_number);
#endif
