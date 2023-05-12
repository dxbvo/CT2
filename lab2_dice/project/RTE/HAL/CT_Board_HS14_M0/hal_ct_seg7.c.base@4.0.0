/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/**
 *  \brief  Implementation of module hal_ct_seg7.
 * 
 *  $Id$
 * ------------------------------------------------------------------------- */

/* User includes */
#include "hal_ct_seg7.h"
#include "reg_ctboard.h"


/* -- Macros
 * ------------------------------------------------------------------------- */
 
#define LCD_DISP_CLEAR_MASK                 0xff
#define LCD_ALL_CLEAR_MASK                  0xffffffff
#define CHECK_ARRAY_OUT_OF_BOUNDS_BCD       0x64
#define CHECK_ARRAY_OUT_OF_BOUNDS_SEG7      0x10
 
 
/* -- Public function definitions
 * ------------------------------------------------------------------------- */

/*
 * See header file
 */
void hal_ct_seg7_raw_write(hal_ct_seg7_t display,
                           uint8_t segment)
{
    switch (display) {
        default:
        case HAL_CT_SEG7_DS0:
            CT_SEG7->RAW.BYTE.DS0 = ~segment;
            break;
        
        case HAL_CT_SEG7_DS1:
            CT_SEG7->RAW.BYTE.DS1 = ~segment;
            break;
        
        case HAL_CT_SEG7_DS2:
            CT_SEG7->RAW.BYTE.DS2 = ~segment;
            break;
        
        case HAL_CT_SEG7_DS3:
            CT_SEG7->RAW.BYTE.DS3 = ~segment;
            break;
    }
}


/*
 * See header file
 */
void hal_ct_seg7_bin_write(uint16_t value)
{    
    CT_SEG7->BIN.HWORD = value;
}


/*
 * See header file
 */
void hal_ct_seg7_clear(hal_ct_seg7_t display)
{
    CT_SEG7->RAW.WORD |= (LCD_DISP_CLEAR_MASK << display);
}


/*
 * See header file
 */
void hal_ct_seg7_clear_all(void)
{
    CT_SEG7->RAW.WORD = LCD_ALL_CLEAR_MASK;
}


/*
 * See header file
 */
void hal_ct_seg7_dot_set(hal_ct_seg7_t display)
{
    CT_SEG7->RAW.WORD &= ~((HAL_CT_SEG7_SEGDP) << display);
}


/*
 * See header file
 */
void hal_ct_seg7_dot_clear(hal_ct_seg7_t display)
{
    CT_SEG7->RAW.WORD |= (HAL_CT_SEG7_SEGDP << display);
}


/*
 *  See header file
 */
uint8_t hal_ct_seg7_get_bcd(uint8_t value)
{
    if (value >= CHECK_ARRAY_OUT_OF_BOUNDS_BCD){
        return 0x99;
    } else {
        return (value % 10u) | ((value / 10u) << 4u);
    }
}


/*
 *  See header file
 */
uint8_t hal_ct_seg7_get_seg7(uint8_t value)
{
    if (value >= CHECK_ARRAY_OUT_OF_BOUNDS_SEG7){
        return 0x8e;
    } else {
        static const uint8_t conv_tab[16u] = { 0xc0, 0xf9, 0xa4, 0xb0,
                                               0x99, 0x92, 0x82, 0xf8,
                                               0x80, 0x90, 0x88, 0x83,
                                               0xa7, 0xa1, 0x86, 0x8e };
        
        return conv_tab[value];
    }
}

