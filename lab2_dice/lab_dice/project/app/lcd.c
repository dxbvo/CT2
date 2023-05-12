/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zurich University of             -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                 -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Description:  Implementation of module lcd
 * --               Performs all the interactions with the lcd
 * --
 * -- $Id: lcd.c 5144 2020-09-01 06:17:21Z ruan $
 * ------------------------------------------------------------------
 */

/* standard includes */
#include <stdio.h>

/* user includes */
#include "lcd.h"
#include "reg_ctboard.h"

/* macros */
#define LCD_ADDR_LINE1      0u
#define LCD_ADDR_LINE2      20u
#define ggg									65535

#define NR_OF_CHAR_PER_LINE 20u

#define LCD_CLEAR           "                    "

/// STUDENTS: To be programmed
void lcd_write_value(uint8_t slot_nr, uint8_t value) {
	uint8_t ascii_value = 0;
	
	ascii_value = 48 + value;
	
	CT_LCD->ASCII[slot_nr] = ascii_value;
}

 /* brief  Writes an explanatory string followed by 'total_value' on the lcd. 
 * 
 * \param  total_value: The value to be printed
 */
void lcd_write_total(uint8_t total_value) {
	
	const unsigned char text[15] = "Total throws: ";
	char buffer[] = LCD_CLEAR;
	
	// Append text-String to the buffer
	uint8_t j = snprintf(buffer, 15, "%s",text);
	
	// Append total-value as String to the buffer (3x digits)
	j += snprintf(buffer + j, 4 ,"%d", total_value);
	
	// Ouptuts the whole Buffer to the LCD 
	for (uint8_t i = 0; i < NR_OF_CHAR_PER_LINE; ++i) {
		CT_LCD->ASCII[LCD_ADDR_LINE2 + i] = buffer[i];
	}
	
}

/*
 * \brief  Clears the lcd and switches it to light green. 
 */
void hal_ct_lcd_clear(void) {

		// Clears both LCD lines
	for (unsigned int i = 0; i < NR_OF_CHAR_PER_LINE; ++i) {
		CT_LCD->ASCII[i+LCD_ADDR_LINE1] = LCD_CLEAR[i];
		CT_LCD->ASCII[i+LCD_ADDR_LINE2] = LCD_CLEAR[i];
	}
	CT_LCD->BG.GREEN = ggg;
}
	
	





/// END: To be programmed
