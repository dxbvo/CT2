/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zurich University of             -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                 -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Description:  Interface of module dice_counter
 * --               The module provides a counter that can be used as
 * --               pseudo random number generator for a dice.
 * --
 * -- $Id: dice_counter.h 1254 2015-02-04 09:51:17Z ruan $
 * ------------------------------------------------------------------
 */

/* re-definition guard */
#ifndef _DICE_COUNTER_H
#define _DICE_COUNTER_H

/* standard includes */
#include <stdint.h>

/* macros */
#define NR_OF_DICE_VALUES 6

/* function declarations */

/*
 * Increments the module internal counter; The function has to be called
 * regularly in a loop to allow generation of a pseudo random number
 */
void dice_counter_increment(void);

/*
 * Returns the current value of the module internal counter. The return
 * value can be used as a pseudo random number.
 */
uint8_t dice_counter_read(void);
#endif
