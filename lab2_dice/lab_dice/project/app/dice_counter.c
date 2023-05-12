/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zurich University of             -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                 -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Description:  Implementation of module dice_counter
 * --               The module provides a counter that can be used as
 * --               pseudo random number generator for a dice.
 * --
 * -- $Id: dice_counter.c 2977 2016-02-15 16:05:50Z ruan $
 * ------------------------------------------------------------------
 */

/* user includes */
#include "dice_counter.h"
#include <stdlib.h>

/* variables visible within the whole module*/
static uint8_t dice_counter = 1;

/* function definitions */

/// STUDENTS: To be programmed
void dice_counter_increment(void) {
	if (dice_counter < 6) {
			dice_counter += 1;
	} else {
		dice_counter = 1;
	}
}

uint8_t dice_counter_read(void) {
	return dice_counter;
}

/// END: To be programmed
