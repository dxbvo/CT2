/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zuercher Hochschule Winterthur   -
 * --  _| |_| | | | |____ ____) |  (University of Applied Sciences) -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Project     : CT2 lab - SPI Display
 * -- Description : Contains constants and function prototypes
 * --               for the basic control functions of the TFT-LCD
 * --               display EAeDIPTFT43-A
 * -- Note        : This header file and its associated implementation
 * --               contains only a very limited number of commands
 * --               supported by the display. It is not intended to
 * --               be a fully featured device driver
 * --
 * -- $Id: cmd_lcd.h 1649 2015-03-12 08:57:56Z muln $
 * ------------------------------------------------------------------
 */

#ifndef _CMD_LCD_H
#define _CMD_LCD_H

#include <stdint.h>

/* ------------------------------------------------------------------
 * -- constants
 * ------------------------------------------------------------------
 */

/*
 * display specific, predefined font numbers according to datasheet
 */
#define FONT_4x6_MONO          (uint8_t)1
#define FONT_6x8_MONO          (uint8_t)2
#define FONT_7x12_MONO         (uint8_t)3
#define FONT_GENEVA10_PROP     (uint8_t)4
#define FONT_CHICAGO14_PROP    (uint8_t)5
#define FONT_SWISS30_BOLD_PROP (uint8_t)6
#define FONT_BIGZIF50          (uint8_t)7
#define FONT_BIGZIF100         (uint8_t)8

/*
 * display specific, predefined color numbers according to datasheet
 */
#define COLOR_TRANSPARENT (uint8_t)0
#define COLOR_BLACK       (uint8_t)1
#define COLOR_BLUE        (uint8_t)2
#define COLOR_RED         (uint8_t)3
#define COLOR_GREEN       (uint8_t)4
#define COLOR_MAGENTA     (uint8_t)5
#define COLOR_LIGHT_BLUE  (uint8_t)6
#define COLOR_YELLOW      (uint8_t)7
#define COLOR_WHITE       (uint8_t)8

/*
 * Turn the cursor on or off
 */
#define CURSOR_OFF (uint8_t)0
#define CURSOR_ON  (uint8_t)1

/* ------------------------------------------------------------------
 * -- Function prototypes
 * ------------------------------------------------------------------
 */

/*
 * Clear the display by filling it with its background color
 * Return zero on success; non-zero otherwise
 */
uint8_t clear_display(void);

/*
 * The interface to the display is initialized and the display is brought
 * into a defined initial state
 */
uint8_t init_display(void);

/*
 * Set the font on the display to one of the display specific font
 * numbers as outlined in the enum Fonts.
 * Return zero on success; non-zero otherwise
 */
uint8_t set_display_font(uint8_t font_number);

/*
 * Set the zoom factors for a font on the display. The factors in the
 * range from 1 to 8 can be set individually for x and y directions.
 * Return zero on success; non-zero otherwise
 */
uint8_t set_font_zoom_factor(uint8_t x_zoom_factor,
                             uint8_t y_zoom_factor);

/*
 * Set the color of the Font and its background to one of the display
 * specific colors as outlined in the enum Colors.
 * Return zero on success; non-zero otherwise
 */
uint8_t set_font_color(uint8_t font_color,
                       uint8_t background_color);

/*
 * Set the color of the TouchPanel
 * Return zero on success; non-zero otherwise
 */
uint8_t set_touch_panel_color(uint8_t normal_edge_inside,
                              uint8_t normal_edge_outside,
                              uint8_t normal_fill,
                              uint8_t select_edge_inside,
                              uint8_t select_edge_outside,
                              uint8_t select_fill);


/*
 * Set the Displaycolor of the font and its background.
 * specific colors as outlined in the enum Colors.
 * Return zero on success; non-zero otherwise
 */
uint8_t set_display_color(uint8_t font_color,
                          uint8_t background_color);

/*
 * Print the zero terminated string specified by text[] at the coordinates
 * specified by xPosition and yPosition. At most 254 characters can be
 * printed.
 * Return zero on success; non-zero otherwise
 */
uint8_t print_text_on_display(uint16_t x_position,
                              uint16_t y_position, uint8_t text[]);

/*
 * Fill the area specified by the top-left corner and the bottom-right
 * corner with the specified color from the enum Colors
 * Return zero on success; non-zero otherwise
 */
uint8_t fill_area(uint16_t left, uint16_t top,
                  uint16_t right, uint16_t bottom,
                  uint8_t fill_color);

/*
 * Turn the cursor on (CURSOR_ON) or off (CURSOR_OFF)
 * Return zero on success; non-zero otherwise
 */
uint8_t set_cursor_on_off(uint8_t on_off);
#endif    /* _CMD_LCD_H */
