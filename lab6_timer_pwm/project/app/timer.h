/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 * --
 * -- Description:  Interface of module timer.
 * --
 * --               Initializes TIM3 and TIM4 in pwm / basic timer mode.
 * --
 * -- $Id: timer.h 3178 2016-03-23 12:05:41Z mady $
 * ------------------------------------------------------------------------- */

/* re-definition guard */
#ifndef _TIMER_H
#define _TIMER_H

/* standard includes */
#include <stdint.h>


/* -- Type definitions
 * ------------------------------------------------------------------------- */

typedef enum {
    PWM_CH1 = 0x0,
    PWM_CH2 = 0x1,
    PWM_CH3 = 0x2,
    PWM_CH4 = 0x3
} pwm_channel_t;


/* -- Public function declarations
 * ------------------------------------------------------------------------- */

/*
 * Initializes TIM3 for pwm mode
 */
void tim3_init(void);

/*
 * Set new value for specified pwm channel
 */
void tim3_set_compare_register(pwm_channel_t channel, uint16_t value);

/*
 * Initializes TIM4 for basic timer functions
 */
void tim4_init(void);

/*
 * Clear pending update interrupt flag
 */
void tim4_reset_uif(void);
#endif
