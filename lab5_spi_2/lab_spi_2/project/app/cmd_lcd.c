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
 * --               for the basic control of the TFT-LCD display
 * --               EAeDIPTFT43-A. The interfaces of the functions
 * --               are described in the header file.
 * --
 * -- $Id: cmd_lcd.c 3158 2016-03-17 12:32:34Z ruan $
 * ------------------------------------------------------------------
 */

#include "cmd_lcd.h"
#include "lcd_io.h"

uint8_t init_terminal(void);

/*
 * according to description in header file
 */
uint8_t clear_display(void)
{
    uint8_t cmd_buffer[10];

    cmd_buffer[0] = 'D';
    cmd_buffer[1] = 'L';

    return write_cmd_to_display(cmd_buffer, 2);
}

/*
 * according to description in header file
 */
uint8_t init_display(void)
{
    init_display_interface();
    init_terminal();
    set_display_color(COLOR_BLACK, COLOR_BLACK);
    set_font_zoom_factor(1, 1);
    clear_display();
    return 0; // :TODO: fix return
}

/*
 * Sends an init Terminal command to the display. This initializes and
 * prints the version
 */
uint8_t init_terminal(void)
{
    uint8_t cmd_buffer[10];

    cmd_buffer[0] = 'T';
    cmd_buffer[1] = 'I';

    return write_cmd_to_display(cmd_buffer, 2);
}

/*
 * according to description in header file
 */
uint8_t set_display_font(uint8_t font_number)
{
    uint8_t cmd_buffer[10];

    cmd_buffer[0] = 'Z';
    cmd_buffer[1] = 'F';
    cmd_buffer[2] = font_number;

    return write_cmd_to_display(cmd_buffer, 3);
}

/*
 * according to description in header file
 */
uint8_t print_text_on_display(uint16_t x_position,
                              uint16_t y_position, uint8_t text[])
{
    uint8_t i;

    uint8_t cmd_buffer[255];

    cmd_buffer[0] = 'Z';
    cmd_buffer[1] = 'L';
    cmd_buffer[2] = x_position & 0xFF;
    cmd_buffer[3] = (x_position & 0xFF00) >> 8;
    cmd_buffer[4] = y_position & 0xFF;
    cmd_buffer[5] = (y_position & 0xFF00) >> 8;

    for (i = 0; text[i] != 0 && i < 254; i++) {
        cmd_buffer[i + 6] = text[i];
    }

    cmd_buffer[i + 6] = 0;

    return write_cmd_to_display(cmd_buffer, i + 7);
}

/*
 * according to description in header file
 */
uint8_t set_font_zoom_factor(uint8_t x_zoom_factor,
                             uint8_t y_zoom_factor)
{
    uint8_t cmd_buffer[10];

    cmd_buffer[0] = 'Z';
    cmd_buffer[1] = 'Z';
    cmd_buffer[2] = x_zoom_factor;
    cmd_buffer[3] = y_zoom_factor;

    return write_cmd_to_display(cmd_buffer, 4);
}

/*
 * according to description in header file
 */
uint8_t set_font_color(uint8_t font_color,
                       uint8_t background_color)
{
    uint8_t cmd_buffer[10];

    // accept only values supported by display
    if (font_color > 32 || background_color > 32) {
        return 1;
    }

    cmd_buffer[0] = 'F';
    cmd_buffer[1] = 'Z';
    cmd_buffer[2] = font_color;
    cmd_buffer[3] = background_color;

    return write_cmd_to_display(cmd_buffer, 4);
}

/*
 * according to description in header file
 */
uint8_t set_display_color(uint8_t font_color,
                          uint8_t background_color)
{
    uint8_t cmd_buffer[10];

    // accept only values supported by display
    if (font_color > 32 || background_color > 32) {
        return 1;
    }

    cmd_buffer[0] = 'F';
    cmd_buffer[1] = 'D';
    cmd_buffer[2] = font_color;
    cmd_buffer[3] = background_color;

    return write_cmd_to_display(cmd_buffer, 4);
}

/*
 * according to description in header file
 */
uint8_t fill_area(uint16_t left, uint16_t top,
                  uint16_t right, uint16_t bottom,
                  uint8_t fill_color)
{
    uint8_t cmd_buffer[20];

    cmd_buffer[0] = 'R';
    cmd_buffer[1] = 'F';
    cmd_buffer[2] = left & 0xFF;
    cmd_buffer[3] = (left & 0xFF00) >> 8;
    cmd_buffer[4] = top & 0xFF;
    cmd_buffer[5] = (top & 0xFF00) >> 8;
    cmd_buffer[6] = right & 0xFF;
    cmd_buffer[7] = (right & 0xFF00) >> 8;
    cmd_buffer[8] = bottom & 0xFF;
    cmd_buffer[9] = (bottom & 0xFF00) >> 8;
    cmd_buffer[10] = fill_color;

    return write_cmd_to_display(cmd_buffer, 11);
}

/*
 * according to description in header file
 */
uint8_t set_cursor_on_off(uint8_t on_off)
{
    uint8_t cmd_buffer[10];

    cmd_buffer[0] = 'T';
    cmd_buffer[1] = 'C';
    cmd_buffer[2] = on_off;

    return write_cmd_to_display(cmd_buffer, 3);
}
