/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/**
 *  \brief  Implementation of module hal_ct_lcd.
 * 
 *  $Id$
 * ------------------------------------------------------------------------- */
 
/* User includes */
#include "hal_ct_lcd.h"
#include "reg_ctboard.h"


/* -- Macros
 * ------------------------------------------------------------------------- */

#define LCD_ADDR_LINE1      0u
#define LCD_ADDR_LINE2      20u

#define LCD_CLEAR           "                    "


/* -- Public function definitions
 * ------------------------------------------------------------------------- */

/*
 * See header file
 */
void hal_ct_lcd_write(uint8_t position, char text[])
{    
    uint8_t i = 0u;
    
    /* Send text */
    while(position < 40u && text[i] != 0u) {
        CT_LCD->ASCII[position++] = text[i++];
    }
}


/*
 * See header file
 */
void hal_ct_lcd_color(hal_ct_lcd_color_t color, uint16_t value)
{
    switch (color) {
        case HAL_LCD_RED:
            CT_LCD->BG.RED = value;
            break;
        
        case HAL_LCD_GREEN:
            CT_LCD->BG.GREEN = value;
            break;
        
        case HAL_LCD_BLUE:
            CT_LCD->BG.BLUE = value;
            break;
    }
}


/*
 * See header file
 */
void hal_ct_lcd_clear(void)
{
    hal_ct_lcd_write(LCD_ADDR_LINE1, LCD_CLEAR);
    hal_ct_lcd_write(LCD_ADDR_LINE2, LCD_CLEAR);
}

