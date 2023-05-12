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
 * -- Description : Contains the implementations of the functions
 * --               for the touch functions of the TFT-LCD display
 * --               EAeDIPTFT43-A. The interfaces of the functions
 * --               are described in the header file.
 * --
 * -- $Id: cmd_touch.c 3158 2016-03-17 12:32:34Z ruan $
 * ------------------------------------------------------------------
 */

#include "cmd_touch.h"
#include "lcd_io.h"

/*
 * according to description in header file
 */
uint8_t set_touch_font(uint8_t font_number)
{
    uint8_t cmd_buffer[10];

    cmd_buffer[0] = 'A';
    cmd_buffer[1] = 'F';
    cmd_buffer[2] = font_number;

    return write_cmd_to_display(cmd_buffer, 3);
}

/*
 * according to description in header file
 */
uint8_t set_touch_font_zoom_factor(uint8_t x_zoom_factor,
                                   uint8_t y_zoom_factor)
{
    uint8_t cmd_buffer[10];

    cmd_buffer[0] = 'A';
    cmd_buffer[1] = 'Z';
    cmd_buffer[2] = x_zoom_factor;
    cmd_buffer[3] = y_zoom_factor;

    return write_cmd_to_display(cmd_buffer, 4);
}

/*
 * according to description in header file
 */
uint8_t set_touch_font_color(uint8_t normal_color,
                             uint8_t select_color)
{
    uint8_t cmd_buffer[10];

    // accept only values supported by display
    if (normal_color > 32 || select_color > 32) {
        return 1;
    }

    cmd_buffer[0] = 'F';
    cmd_buffer[1] = 'A';
    cmd_buffer[2] = normal_color;
    cmd_buffer[3] = select_color;

    return write_cmd_to_display(cmd_buffer, 4);
}

/*
 * according to description in header file
 */
uint8_t set_touch_panel_color(uint8_t normal_edge_inside,
                              uint8_t normal_edge_outside,
                              uint8_t normal_fill,
                              uint8_t select_edge_inside,
                              uint8_t select_edge_outside,
                              uint8_t select_fill)
{
    uint8_t cmd_buffer[10];

    // accept only values supported by display
    if (normal_edge_inside > 32 || normal_edge_outside > 32 || normal_fill > 32
        || select_edge_inside > 32 || select_edge_outside > 32 ||
        select_fill > 32) {
        return 1;
    }

    cmd_buffer[0] = 'F';
    cmd_buffer[1] = 'E';
    cmd_buffer[2] = normal_edge_inside;
    cmd_buffer[3] = normal_edge_outside;
    cmd_buffer[4] = normal_fill;
    cmd_buffer[5] = select_edge_inside;
    cmd_buffer[6] = select_edge_outside;
    cmd_buffer[7] = select_fill;

    return write_cmd_to_display(cmd_buffer, 8);
}

/*
 * according to description in header file
 */
uint8_t set_touch_enable(uint8_t on_off)
{
    uint8_t cmd_buffer[10];

    cmd_buffer[0] = 'A';
    cmd_buffer[1] = 'A';
    cmd_buffer[2] = on_off;

    return write_cmd_to_display(cmd_buffer, 3);
}

/*
 * according to description in header file
 */
uint8_t define_touch_button(uint16_t left, uint16_t top,
                            uint16_t right, uint16_t bottom,
                            uint8_t down_code,
                            uint8_t up_code,
                            uint8_t text[])
{
    int i;
    uint8_t cmd_buffer[255];

    cmd_buffer[0] = 'A';
    cmd_buffer[1] = 'K';
    cmd_buffer[2] = left & 0xFF;
    cmd_buffer[3] = (left & 0xFF00) >> 8;
    cmd_buffer[4] = top & 0xFF;
    cmd_buffer[5] = (top & 0xFF00) >> 8;
    cmd_buffer[6] = right & 0xFF;
    cmd_buffer[7] = (right & 0xFF00) >> 8;
    cmd_buffer[8] = bottom & 0xFF;
    cmd_buffer[9] = (bottom & 0xFF00) >> 8;
    cmd_buffer[10] = down_code;
    cmd_buffer[11] = up_code;

    for (i = 0; text[i] != 0 && i < (255 - 12); i++) {
        cmd_buffer[i + 12] = text[i];
    }

    cmd_buffer[i + 12] = 0;

    return write_cmd_to_display(cmd_buffer, (uint8_t) (i + 13));
}
