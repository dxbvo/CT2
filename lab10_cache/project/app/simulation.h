/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */

#ifndef SIM_H_
#define SIM_H_

/* CT board includes */
#include "hal_ct_seg7.h"
#include "reg_ctboard.h"
#include "hal_ct_lcd.h"
#include "hal_timer.h"

/* User includes */
#include "arrays.h"
#include "cache.h"
#include "config.h"

/* Typedefs */
typedef enum {
    DEBUG_LEVEL_HIT,
    DEBUG_LEVEL_MISS,
    DEBUG_LEVEL_INFO
} debug_level_t;

typedef enum {
    PRINT_CACHE_MODE_HITS,
    PRINT_CACHE_MODE_MISSES
} print_cache_mode_t;

/* print_results
 *
 * Print the hits and misses.
 *
 * @param				HitMiss				Hits and misses
 *
 * @return      void
 */
void print_results(struct HitMiss *hit_miss);

/* debug_line_out
 *
 * Prints out to the LCD
 *
 * @param       level       Debug level
 * @param				text				Line to write
 *
 * @return      void
 */
void debug_line_out(debug_level_t level, char text[]);

/* delay
 *
 * Simple delay function
 *
 * @return      void
 */
void delay(void);

/* button1_pressed
 *
 * Is T0 pressed
 *
 * @return		button_pressed
 */
uint8_t button1_pressed(void);

/* button2_pressed
 *
 * Is T0 pressed
 *
 * @return		button_pressed
 */
uint8_t button2_pressed(void);

#endif
/* SIM_H_ */

