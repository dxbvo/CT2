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
 * --
 * -- $Id: main.c 3686 2016-10-10 12:27:05Z kesr $
 * ------------------------------------------------------------------
 */
#include <stdint.h>
#include <reg_ctboard.h>
#include "cmd_touch.h"
#include "lcd_io.h"
#include "cmd_lcd.h"

#define BUTTON1_DOWN              (uint8_t)1
#define BUTTON1_UP                (uint8_t)2
#define LED_ON                    (uint8_t)0xFF
#define LED_OFF                   (uint8_t)0x00
#define MIN_LENGTH_BUFFER_MESSAGE (uint8_t)3

int main(void)
{
    uint8_t readBuffer[256];
    uint8_t nrOfChars;
	  uint8_t pressed = LED_OFF;


    // initialization and refresh display configuration
    init_display();
    set_display_color(COLOR_BLACK, COLOR_BLACK);
    set_font_zoom_factor(1, 1);
    clear_display();

    // set title
    set_font_zoom_factor(3, 2);
    set_font_color(COLOR_BLUE, COLOR_TRANSPARENT);
    set_display_font(FONT_SWISS30_BOLD_PROP);
    print_text_on_display(50, 25, (uint8_t *)"HELLO SPI");
    fill_area(50, 80, 435, 82, COLOR_GREEN);

    // draw touch-button and activate touchscreen
    set_touch_enable(TOUCH_ON);
    set_touch_font_color(COLOR_BLACK, COLOR_WHITE);
    set_touch_panel_color(COLOR_WHITE, COLOR_BLACK, COLOR_YELLOW, COLOR_WHITE,
                          COLOR_BLACK, COLOR_BLACK);
    set_touch_font_zoom_factor(1, 1);
    set_touch_font(FONT_CHICAGO14_PROP);
    define_touch_button(205, 160, 275, 230, BUTTON1_DOWN, BUTTON1_UP,
                        (uint8_t *)"PRESS ME");
    set_cursor_on_off(CURSOR_OFF);

    while (1) {
        /// STUDENTS: To be programmed
			
		// read data from display buffer
		nrOfChars = read_display_buffer(readBuffer);
		
		// check if sufficient data is read and if button is pressed
    if (MIN_LENGTH_BUFFER_MESSAGE < nrOfChars && readBuffer[3] == BUTTON1_DOWN)
    {
      pressed = LED_ON;
    }
    else if (MIN_LENGTH_BUFFER_MESSAGE < nrOfChars && readBuffer[3] == BUTTON1_UP)
    {
      pressed = LED_OFF;
    }
		
		// turn on led accordingly
    CT_LED->BYTE.LED7_0 = pressed;

        /// END: To be programmed
    }
}
