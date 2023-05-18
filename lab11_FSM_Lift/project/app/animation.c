/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 * --
 * -- Description:  Implementation of module timer.
 * --
 * -- $Id: animation.c 5526 2022-01-18 07:26:31Z ruan $
 * ------------------------------------------------------------------------- */

/* standard includes */
#include <stdint.h>
#include <stdbool.h>

/* user includes */
#include "animation.h"
#include "event_handler.h"
#include "hal_timer.h"


/* Module-wide constants & variables
 * ------------------------------------------------------------------------- */

static const uint16_t Animation_Freq = 500u;
static uint16_t timer_count = 0u;

static bool is_moving = false;


/* Public function definitions
 * ------------------------------------------------------------------------- */

/*
 * See header file
 */
void animation_init(void)
{
    hal_timer_base_init_t timer_init;

    TIM3_ENABLE();

    timer_init.prescaler = 1282u;           // --> 65.5 kHz
    timer_init.mode = HAL_TIMER_MODE_UP;
    timer_init.run_mode = HAL_TIMER_RUN_CONTINOUS;
    timer_init.count = 0xffff / Animation_Freq;

    hal_timer_init_base(TIM3, timer_init);
    hal_timer_irq_set(TIM3, HAL_TIMER_IRQ_UE, ENABLED);

    hal_timer_start(TIM3);
}



/* Interrupt service routines
 * ------------------------------------------------------------------------- */

void TIM3_IRQHandler(void)
{
    if (hal_timer_irq_status(TIM3, HAL_TIMER_IRQ_UE)) {
        hal_timer_irq_clear(TIM3, HAL_TIMER_IRQ_UE);

        if (timer_count > 0u) {
            timer_count--;
        } 
        else {
            
        }
    }
}


