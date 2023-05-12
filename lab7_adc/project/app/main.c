/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zuercher Hochschule Winterthur             -
 * --  _| |_| | | | |____ ____) |  (University of Applied Sciences)           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 * --
 * -- Description : Main program.
 * --
 * --               Task 1: - Setup ADC3 channel 4 (6 bit)
 * --                       - Read analog value and output to 7 Segment
 * --               Task 2: - Implement convert_to_fixpoint function:
 * --                         Converts binary data into fixpoint value
 * --               Task 3: - Implement possibility to choose resolution
 * --                         for analog conversion
 * --               Task 4: - Implement a moving average filter
 * --
 * -- $Id: main.c 5605 2023-01-05 15:52:42Z frtt $
 * ------------------------------------------------------------------------- */

/* standard includes */
#include <stdint.h>

/* user includes */
#include "reg_ctboard.h"
#include "adc.h"
#include "hal_ct_lcd.h"


/* -- Macros for accessing CT Board LCD
 * ------------------------------------------------------------------------- */

#define LCD (*(uint16_t *)0x60000310)

#define ASCII_CHAR_V   (0x56)
#define ASCII_CHAR_DOT (0x2e)
#define ASCII_ZERO     (0x30)

#define LCD_POSITION_0 (uint16_t)(0x0000)
#define LCD_POSITION_1 (uint16_t)(0x0100)
#define LCD_POSITION_2 (uint16_t)(0x0200)
#define LCD_POSITION_3 (uint16_t)(0x0300)
#define LCD_POSITION_4 (uint16_t)(0x0400)
#define LCD_POSITION_5 (uint16_t)(0x0500)


/* -- Macros used by student code
 * ------------------------------------------------------------------------- */

/// STUDENTS: To be programmed

/// END: To be programmed

/* -- Local function declaration
 * ------------------------------------------------------------------------- */
static uint16_t normalize_value(uint16_t value, adc_resolution_t resolution);
static void display_on_lcd(uint16_t bcd_value);
static void convert_hex_to_ascii(uint16_t hex_value, char* characters);


/* -- M A I N
 * ------------------------------------------------------------------------- */

int main(void)
{
    /// STUDENTS: To be programmed

    adc_resolution_t resolution;
    uint16_t bit_width;
    uint16_t adc_value;

    adc_init();

    while (1) {
        switch (CT_HEXSW & 0x3) {
        case 0:
            resolution = ADC_RES_6BIT;
            bit_width = 6;
            break;
        case 1:
            resolution = ADC_RES_8BIT;
            bit_width = 8;
            break;
        case 2:
            resolution = ADC_RES_10BIT;
            bit_width = 10;
            break;
        case 3:
            resolution = ADC_RES_12BIT;
            bit_width = 12;
            break;
        default:
            resolution = ADC_RES_6BIT;
            bit_width = 6;
            break;
        }

        adc_value = adc_get_value(resolution);
        display_on_lcd(normalize_value(adc_value, resolution));
        CT_SEG7->BIN.HWORD = adc_value;
        CT_LED->HWORD.LED15_0 = (1 << bit_width) - 1;
    }

    /// END: To be programmed
}


/* -- Local function definitions
 * ------------------------------------------------------------------------- */

/*
 * Normalize value to scale 0..3300 (3.3 V)
 */
static uint16_t normalize_value(uint16_t value, adc_resolution_t resolution)
{
    /* Use of 32bit -> 4096(12bit) * 3300 = 13'516'800 */
    uint32_t normalized;

    /// STUDENTS: To be programmed
	  switch (resolution) {
			case ADC_RES_12BIT:
				normalized = (value * 3300) / 4095;
				break;
			case ADC_RES_10BIT:
				normalized = (value * 3300) / 1023;
				break;
			case ADC_RES_8BIT:
				normalized = (value * 3300) / 255;
				break;
			case ADC_RES_6BIT:
				normalized = (value * 3300) / 63;
				break;
		}

    /// END: To be programmeds

    /* Return 16bit value -> max after normalization = 3300 */
    return (uint16_t)normalized;
}

/*
 * Display value on LCD display.
 * The data is interpreted and displayed as a fixed point number with
 * three places after the decimal point.
 */
static void display_on_lcd(uint16_t data_3dec_places)
{
    enum{
			value_size =5,
      voltage_size = 7,
      dot_position = 1
    };
    char character_values[value_size];
    char character_voltage[voltage_size];
    char temp_character_voltage[voltage_size];
    uint8_t character_size = 0;
    uint8_t start_point;
    uint8_t i;
    
    // get the values
    convert_hex_to_ascii(data_3dec_places, character_values);    
    
    // find first space or nul character
    for(i = 0; i < value_size; i++){
        if(character_values[i] < ASCII_ZERO){
            character_size = i;
            break;
        }
    }
    
    start_point = value_size - character_size - 1;
    
    // shift chars to right and fill with zeros
    for(i = 0; i < voltage_size; i++){
        if(i < start_point){
            temp_character_voltage[i] = ASCII_ZERO;
        } else{
            temp_character_voltage[i] = character_values[i - start_point];
        }
    }
    
    // add dot, voltage and nul char
    for(i = 0; i < value_size; i++){
        if(i < dot_position){
            character_voltage[i] = temp_character_voltage[i];
        } else if(i == dot_position){
            character_voltage[i] = ASCII_CHAR_DOT;
        } else if(i > dot_position){
            character_voltage[i] = temp_character_voltage[i-1];
        }
    }
    character_voltage[voltage_size - 2] = ASCII_CHAR_V;
    character_voltage[voltage_size - 1] = 0;
    
    // write characters to display
    hal_ct_lcd_write(0, character_voltage);
}

static void convert_hex_to_ascii(uint16_t hex_value, char* characters){
    uint8_t i = 0;
    uint8_t char_size;
    enum {array_size = 5};
    char temp_characters[array_size];
    
    // cut and convert digits to ascii
    while(hex_value > 0){
        temp_characters[i] = (hex_value % 10) + ASCII_ZERO;
        hex_value /= 10;
        i++;
    }
    
    // invert order
    char_size = i;
    for (i = 0; i < char_size; i++){
        characters[i] = temp_characters[char_size - i - 1];
    }
    
    // fill rest with spaces
    for (i = char_size; i < array_size; i++){
        characters[i] = 32;
    }
    
    // end last part with zero character
    characters[array_size - 1] = 0;
}
