/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 * -- $Id: main.c 4800 2019-05-09 15:30:18Z ruan $
 * ------------------------------------------------------------------------- */

#include <stdint.h>
#include <stdio.h>
#include <reg_stm32f4xx.h>
#include "hal_ct_lcd.h"
#include "hal_timer.h"
#include <reg_ctboard.h>

/* -- macros --------------------------------------------------------------- */

#define NUMBER_OF_TIMER_2_INTERRUPTS (uint32_t)1000
#define RELOAD_VALUE_TIM2 (uint32_t)84000

#define IRQNUM_TIM2 28
#define IRQNUM_TIM3 29

#define STRING_LENGTH_FOR_32BIT 11 // 4G --> 10 bit plus end of string

/* -- functions with module-wide scope -------------------------------------- */

static void print_results(void);
// static uint8_t convert_uint32_t_to_string(char ret_val[], uint32_t value);
static uint16_t read_hex_switch(void);

/* Function prototypes */
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);


/* -- variables with module-wide scope -------------------------------------- */

static volatile hal_bool_t measurement_done = FALSE;
static uint32_t tim3_interrupt_counter = 0;
static uint32_t tim2_interrupt_counter = 0;
static uint32_t min_latency = 100000;
static uint32_t max_latency = 0;
static uint32_t sum_latency = 0;
static uint32_t avg_latency = 0;
static uint32_t sum_tisr = 0; // time of interrupt service routine
static uint32_t avg_tisr = 0; // average time of interrupt service routine
static volatile uint32_t dummy_counter;

/* -- M A I N --------------------------------------------------------------- */

int main(void)
{
    hal_timer_base_init_t timer_init;
    uint16_t reload_value_tim3;

    while (1)
    {
        /* wait for button press to start test */
        while (!(CT_BUTTON & 0x1))
        {
            /* dummy read to display the HEX switch position on SEG7 */
            read_hex_switch();
        }

        /* reset statistics */
        measurement_done = FALSE;
        tim3_interrupt_counter = 0;
        tim2_interrupt_counter = 0;
        min_latency = 100000;
        max_latency = 0;
        sum_latency = 0;
        avg_latency = 0;
        sum_tisr = 0;
        avg_tisr = 0;

        /* init display, Use RED background while test is running */
        hal_ct_lcd_clear();
        hal_ct_lcd_color(HAL_LCD_RED, 0xffff);
        hal_ct_lcd_color(HAL_LCD_BLUE, 0u);
        hal_ct_lcd_color(HAL_LCD_GREEN, 0u);

        /* init timer2 with a clock source frequency of 84MHz
           --> generate a timer2 interrupt every 1ms */
        TIM2_ENABLE();
        TIM2_RESET();

        timer_init.mode = HAL_TIMER_MODE_UP;
        timer_init.run_mode = HAL_TIMER_RUN_CONTINOUS;
        timer_init.prescaler = 0u;
        timer_init.count = RELOAD_VALUE_TIM2; //counter overflow every 1ms

        hal_timer_init_base(TIM2, timer_init);
        hal_timer_irq_set(TIM2, HAL_TIMER_IRQ_UE, ENABLE);

        /* read and display the amount of load selected for timer 3*/
        reload_value_tim3 = read_hex_switch();

        /* init timer3 with a clock source frequency of 84MHz */
        TIM3_ENABLE();
        TIM3_RESET();

        timer_init.mode = HAL_TIMER_MODE_UP;
        timer_init.run_mode = HAL_TIMER_RUN_CONTINOUS;
        timer_init.prescaler = 0u;
        timer_init.count = reload_value_tim3; // from hex switch

        hal_timer_init_base(TIM3, timer_init);
        hal_timer_irq_set(TIM3, HAL_TIMER_IRQ_UE, ENABLE);

        /* set default interrupt priorities:
                load on both timers set to same priority   */
        NVIC->IP[IRQNUM_TIM2] = 0x10; //set priority level of timer2 to 1
        NVIC->IP[IRQNUM_TIM3] = 0x10; //set priority level of timer3 to 1

        /* Set interrupt priorities based on dip switches
         *  - S7..S4   --> priority for load on timer3
         *  - S15..S11 --> priority for timer2
         *
         * Reading of dip switches  through  CT_DIPSW->BYTE.S7_0 and
         * CT_DIPSW->BYTE.S15_8
         *
         * Priorities
         *  - The lower a priority level, the greater the priority
         *  - 4-bit priority level in bits [7:4] of NVIC->IP[] register,
         *    i.e. 0x00 � 0xF0 
         */

        /// STUDENTS: To be programmed
        NVIC->IP[IRQNUM_TIM3] = (CT_DIPSW->BYTE.S7_0 & 0xF0) >> 4;
        NVIC->IP[IRQNUM_TIM2] = (CT_DIPSW->BYTE.S15_8 & 0xF0) >> 4;
        /// END: To be programmed

        /* start timer2 */
        hal_timer_start(TIM2);

        /* if there is load --> start timer 3 */
        if (reload_value_tim3 != 0)
        {
            hal_timer_start(TIM3);
        }

        /* wait for measurement to finish */
        while (!measurement_done)
        {
        }

        /* print out measurement */
        avg_latency = sum_latency / tim2_interrupt_counter;
        avg_tisr = sum_tisr / tim2_interrupt_counter;
        print_results();
    }
}

/**
 * \brief   Timer 2 ISR: Measuring Interrupt latency and Interrupt Service Time
 */
void TIM2_IRQHandler(void)
{
    /// STUDENTS: To be programmed
    uint16_t timer_val = (uint16_t)TIM2->CNT;
    hal_timer_irq_clear(TIM2, HAL_TIMER_IRQ_UE);
    // increase interrupt counter
    tim2_interrupt_counter++;

    // update latency variables
    if (timer_val < min_latency)
    {
        min_latency = timer_val;
    }
    if (timer_val > max_latency)
    {
        max_latency = timer_val;
    }
    sum_latency += timer_val;

    // stop timers if reading done.
    if (tim2_interrupt_counter == NUMBER_OF_TIMER_2_INTERRUPTS)
    {
        hal_timer_stop(TIM2);
        hal_timer_stop(TIM3);
        measurement_done = TRUE;
    }
    sum_tisr += TIM2->CNT - timer_val;
    /// END: To be programmed
}

/**
 * \brief  Timer 3 ISR: Generating load
 */
void TIM3_IRQHandler(void)
{
    hal_timer_irq_clear(TIM3, HAL_TIMER_IRQ_UE);
    tim3_interrupt_counter++;

    /* add a little bit of delay in the ISR */
    for (dummy_counter = 0; dummy_counter < 3; dummy_counter++)
    {
    }
}

/* -- local function definitions --------------------------------------------- */

/**
 * \brief  Prints the minimal, maximal and average interrupt latency and the
 *         number of occurred timer3 interrupts to the display
 */
static void print_results(void) {

    char label_min[] = "min ";
    char label_max[] = "max ";
    char label_avg[] = "avg ";
    char label_load[] = "load ";

    // char ret_val[STRING_LENGTH_FOR_32BIT]; // unused
    // uint8_t length = 0; // unused
    uint8_t pos = 0;
		
		char lcdtext[100];  // Move the declaration to the beginning

    // set lcd backlight green
    hal_ct_lcd_color(HAL_LCD_RED, 0u);
    hal_ct_lcd_color(HAL_LCD_GREEN, 0xffff);

    // write label "min"
    hal_ct_lcd_write(pos, label_min);

    /* Add writing of min, max and average values to LCD,
     * include the appropriate labels.
     *
     * Add writing of the counted number of external interrupts to LCD,
     * include the appropriate label
     */

    /// STUDENTS: To be programmed
    hal_ct_lcd_clear();
    sprintf(lcdtext, "%s%2i %s%3i %3i  %s%2i %s%i",
            label_min,
            min_latency,
            label_max,
            max_latency,
            avg_tisr,
            label_avg,
            avg_latency,
            label_load,
            tim3_interrupt_counter);
    hal_ct_lcd_write(0, lcdtext);
    /// END: To be programmed
}

/**
 * \brief  Converts an uint32_t value into a string.
 * \param  ret_val: Pointer to the array where the result of the conversion will
 *                  be stored. The array must be large enough to hold the string
 * \param  value:   The value to be converted
 * \return The functions returns a negative value in the case of an error.
 *         Otherwise, the number of characters written is returned.
 */
//static uint8_t convert_uint32_t_to_string(char ret_val[], uint32_t value) {
//    return (uint8_t)snprintf(ret_val, STRING_LENGTH_FOR_32BIT, "%d", value);
//}

/**
 * \brief  Read the setting on the hex switch and display the selected load
 *         on the 7-segment: none / 10 kHz / 100 kHz / 1000 kHz
 * \return The appropriate reload value for the load on timer3
 */
static uint16_t read_hex_switch(void) {
    uint8_t hex_sw_pos = CT_HEXSW & 0x0F;
    uint16_t ret_value;

    switch (hex_sw_pos)
    {
    case 1:
    case 5:
    case 9:
    case 13:
        /* 10 kHz */
        CT_SEG7->RAW.WORD = (uint32_t)0xFFFFF9C0; // 10
        ret_value = (uint16_t)8400;
        break;
    case 2:
    case 6:
    case 10:
    case 14:
        /* 100 kHz */
        CT_SEG7->RAW.WORD = (uint32_t)0xFFF9C0C0; // 100
        ret_value = (uint16_t)840;
        break;
    case 3:
    case 7:
    case 11:
    case 15:
        /* 1000 kHz */
        CT_SEG7->RAW.WORD = (uint32_t)0xF9C0C0C0; // 1000
        ret_value = (uint16_t)84;
        break;
    default:
        /* no load */
        CT_SEG7->RAW.WORD = (uint32_t)0xFFFFFFC0; // 0 --> no load
        ret_value = (uint16_t)0;
    }
    return ret_value;
}

