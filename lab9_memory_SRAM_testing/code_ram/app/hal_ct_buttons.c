/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zurich University of             -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                 -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Description:  Implementation of module hal_ct_button
 * --               The hardware abstraction layer for the buttons of
 * --               the CT board
 * --
 * -- $Id: hal_ct_buttons.c 5317 2020-12-04 15:56:47Z ruan $
 * ------------------------------------------------------------------
 */

/* standard includes */
#include <stdbool.h>

/* user includes */
#include "hal_ct_buttons.h"

/* macros visible only inside of module */
#define BUTTON_REG     (*((volatile uint8_t *)(0x60000210)))
#define BUTTON_T0_MASK 0x01

/* function definitions */

/*
 * see header file
 */
uint32_t hal_ct_button_is_pressed(hal_ct_button_t button)
{
    uint32_t ret_value = false;

    // if button is pressed, wait until button is not pressed anymore
    while ((BUTTON_REG & (BUTTON_T0_MASK << button))) {
        ret_value = true;
    }
    return ret_value;
}
