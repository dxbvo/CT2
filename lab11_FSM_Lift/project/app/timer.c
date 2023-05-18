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
 * -- $Id: timer.c 5526 2022-01-18 07:26:31Z ruan $
 * ------------------------------------------------------------------------- */

/* standard includes */
#include <stdint.h>

/* user includes */
#include "timer.h"
#include "hal_timer.h"


/* Module-wide variables
 * ------------------------------------------------------------------------- */

static uint16_t timer_count = 0u;


/* Interrupt service routines
 * ------------------------------------------------------------------------- */

void TIM4_IRQHandler(void)
{
    if (hal_timer_irq_status(TIM4, HAL_TIMER_IRQ_UE)) {
        hal_timer_irq_clear(TIM4, HAL_TIMER_IRQ_UE);

        if (timer_count > 0u) {
            timer_count--;
        }
    }
}


/* Public function definitions
 * ------------------------------------------------------------------------- */

/*
 * See header file
 */
void timer_init(void)
{
    hal_timer_base_init_t timer_init;

    TIM4_ENABLE();

    timer_init.prescaler = 8400u;           // --> 10 kHz
    timer_init.mode = HAL_TIMER_MODE_UP;
    timer_init.run_mode = HAL_TIMER_RUN_CONTINOUS;
    timer_init.count = 100u;                // --> 100 Hz --> 10 ms

    hal_timer_init_base(TIM4, timer_init);
    hal_timer_irq_set(TIM4, HAL_TIMER_IRQ_UE, ENABLED);

    hal_timer_start(TIM4);
}

/*
 * See header file
 */
void timer_start(uint16_t duration)
{
    timer_count = duration;
}

/*
 * See header file
 */
void timer_stop(void)
{
    timer_count = 0u;
}

/*
 * See header file
 */
uint16_t timer_read(void)
{
    return timer_count;
}
