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
 * --               for the touch functions of the TFT-LCD display
 * --               EAeDIPTFT43-A
 * -- Note        : This header file and its associated implementation
 * --               contains only a very limited number of commands
 * --               supported by the display. It is not intended to
 * --               be a fully featured device driver
 * --
 * -- $Id: cmd_touch.h 1649 2015-03-12 08:57:56Z muln $
 * ------------------------------------------------------------------
 */

#ifndef _CMD_TOUCH_H
#define _CMD_TOUCH_H

#include <stdint.h>

/*
 * Turn the touch function on or off
 */
#define TOUCH_OFF (uint8_t)0
#define TOUCH_ON  (uint8_t)1

/* ------------------------------------------------------------------
 * -- Function prototypes
 * ------------------------------------------------------------------
 */

/*
 * Set the Touchfont on the display to one of the display specific font
 * numbers as outlined in the enum Fonts.
 * Return zero on success; non-zero otherwise
 */
uint8_t set_touch_font(uint8_t font_number);

/*
 * Set the zoom factors for the Touchfont on the display. The factors in the
 * range from 1 to 8 can be set individually for x and y directions.
 * Return zero on success; non-zero otherwise
 */
uint8_t set_touch_font_zoom_factor(uint8_t x_zoom_factor,
                                   uint8_t y_zoom_factor);

/*
 * Set the color of the TouchFont
 * Return zero on success; non-zero otherwise
 */
uint8_t set_touch_font_color(uint8_t normal_color, uint8_t select_color);

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
 * Enable the touch functionality through TOUCH_ON and disable the touch
 * functionality with TOUCH_OFF
 * Return zero on success; non-zero otherwise
 */
uint8_t set_touch_enable(uint8_t on_off);

/*
 * Draw a touch button in the area specified by the top-left corner and
 * the bottom-right color. Display the the zero terminated string
 * specified by text[] on the button. When the button is asserted (pressed)
 * the value of downCode will be included in the receive message. When the
 * button is deasserted (released) the value of upCode will be included in the
 * receive message.
 * Return zero on success; non-zero otherwise
 */
uint8_t define_touch_button(uint16_t left, uint16_t top,
                            uint16_t right, uint16_t bottom,
                            uint8_t down_code,
                            uint8_t up_code,
                            uint8_t text[]);
#endif    /* _CMD_TOUCH_H */
