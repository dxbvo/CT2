/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */

/* User includes */
#include "simulation.h"

static uint32_t delay_multiplier = 0x10000;

/* print_results
 *
 * Print the hits and misses.
 *
 * @return      void
 */
void print_results(struct HitMiss *hit_miss)
{
    /* Local Variables */
    char str[80];

    sprintf(str, "HITS: %6d \n MISSES: %4d", hit_miss->hits, hit_miss->misses);

    /* Print line */
    debug_line_out(DEBUG_LEVEL_INFO, str);
}

/* debug_line_out
 *
 * Prints out to the LCD
 *
 * @param       level       Debug level
 * @param       text        Line to write
 *
 * @return      void
 */
void debug_line_out(debug_level_t level, char text[])
{
    /* Reset LCD */
    hal_ct_lcd_clear();

    /* Reset colors */
    hal_ct_lcd_color(HAL_LCD_GREEN, 0);
    hal_ct_lcd_color(HAL_LCD_RED, 0);
    hal_ct_lcd_color(HAL_LCD_BLUE, 0);

    /* Set debug level colors */
    switch (level) {
        case DEBUG_LEVEL_INFO:
            hal_ct_lcd_color(HAL_LCD_BLUE, 0xffFF);
            break;
        case DEBUG_LEVEL_MISS:
            hal_ct_lcd_color(HAL_LCD_RED, 0xffff);
            break;
        case DEBUG_LEVEL_HIT:
            hal_ct_lcd_color(HAL_LCD_GREEN, 0xffff);
            break;
    }

    /* Write line */
    hal_ct_lcd_write(0, text);
}

/* delay
 *
 * Simple delay function
 *
 *
 * @return      void
 */
void delay(void)
{
    /* Delay the simulation */
    for (uint32_t i = delay_multiplier; i > 0; i--) {}
}


/* button1_pressed
 *
 * Is T0 pressed
 *
 * @return      button_pressed
 */
uint8_t button1_pressed(void)
{
    /* Mask button */
    return CT_BUTTON & 0x1;
}


/* button2_pressed
 *
 * Is T0 pressed
 *
 * @return      button_pressed
 */
uint8_t button2_pressed(void)
{
    /* Mask button */
    return CT_BUTTON & 0x2;
}

