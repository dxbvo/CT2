/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zurich University of             -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                 -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Description:  Implementation of module statistics
 * --               Collect and provide statistical data on throws
 * --               of the dice
 * --
 * -- $Id: statistics.c 2977 2016-02-15 16:05:50Z ruan $
 * ------------------------------------------------------------------
 */

/* user includes */
#include "statistics.h"

/* variables visible within the whole module*/

// index 0:         total number of throws
// index 1 to 6:    number of throws for each digit
static uint8_t nr_of_throws[NR_OF_DICE_VALUES + 1];

/* function definitions */

/// STUDENTS: To be programmed
/*
 * Increments the total number of throws as well as the number for the throws
 * with the result throw_value.
 * throw_value has to be in the range of 1 up to NR_OF_DICE_VALUES, otherwise
 * it will be ignored
 */
 
void stat_add_throw(uint8_t throw_value) {
	if (throw_value <= NR_OF_DICE_VALUES) {
			nr_of_throws[0] += 1;
			nr_of_throws[throw_value] += 1;
	} 
}
/*
 * Return the number of throws with the result 'dice_number'
 * For 'dice_number' equal zero the total number of throws will be returned.
 * If 'dice_number' is above NR_OF_DICE_VALUES the error code ERROR_VALUE will
 * be returned.
 */
uint8_t stat_read(uint8_t dice_number) {
	if (dice_number <= NR_OF_DICE_VALUES) {
		return nr_of_throws[dice_number];
	} else {
		return ERROR_VALUE;
	}
}

/// END: To be programmed
