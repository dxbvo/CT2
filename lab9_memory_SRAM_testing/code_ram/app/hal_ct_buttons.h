/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zurich University of             -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                 -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Description:  Interface of module hal_ct_button
 * --               The hardware abstraction layer for the buttons of
 * --               the CT board
 * --
 * -- $Id: hal_ct_buttons.h 5317 2020-12-04 15:56:47Z ruan $
 * ------------------------------------------------------------------
 */

/* re-definition guard */
#ifndef _HAL_CT_BUTTONS_H
#define _HAL_CT_BUTTONS_H

/* standard includes */
#include <stdint.h>

/* type definitions */
typedef enum {
    HAL_CT_BUTTON_T0 = 0x00,
    HAL_CT_BUTTON_T1 = 0x01,
    HAL_CT_BUTTON_T2 = 0x02,
    HAL_CT_BUTTON_T3 = 0x03
}hal_ct_button_t;

/* function declarations */

/*
 * Returns 'true' if the specified 'button'is pressed, 'false' otherwise
 * If pressed the function waits for the release of the button
 */
uint32_t hal_ct_button_is_pressed(hal_ct_button_t button);
#endif
