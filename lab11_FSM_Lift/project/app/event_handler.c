/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 * --
 * -- Description:  Implementation of module event_handler.
 * --
 * -- $Id: event_handler.c 5526 2022-01-18 07:26:31Z ruan $
 * ------------------------------------------------------------------------- */

/* standard includes */
#include <stdint.h>
#include <stdio.h>
#include <reg_stm32f4xx.h>

/* user includes */
#include "event_handler.h"
#include "reg_ctboard.h"
#include "timer.h"


/* -- Macros
 * ------------------------------------------------------------------------- */

// ADC configuration for weight input using the CT-board potentiometer
#define PERIPH_GPIOF_ENABLE (0x00000020)
#define PERIPH_ADC3_ENABLE  (0x00000400)
#define GPIOF_MODER_ANALOG  (0x3 << 12)

// CT-board buttons for base floor (F0) and upper floor (F1)
#define BUTTON_F0_MASK      (0x01)
#define BUTTON_F1_MASK      (0x02)

// CT-board switches for opening/closing the doors on both floors
#define DOOR0_MASK          (0x00000080)
#define DOOR1_MASK          (0x00800000)


/* Module-wide constants, variables & declarations
 * ------------------------------------------------------------------------- */
 
static const uint16_t WEIGHT_MAX_VALUE =    63;
static uint16_t current_weight_limit =      WEIGHT_MAX_VALUE;
static weight_control_t wctl_state =        WCTL_DISABLE;

static uint16_t weight_value_old =  0xffffu; // used in 2 functions

static void eh_7seg_display(bool turn_display_on, uint16_t value);


/* Public functions & variables
 * ------------------------------------------------------------------------- */

/*
 * See header file
 */
bool flag_F0_Reached = false;
bool flag_F1_Reached = false;


/*
 * See header file
 */
void eh_init(void)
{
    // setup ADC3 for 6bit continuous readings of the CT-board potentiometer
    RCC->AHB1ENR |= PERIPH_GPIOF_ENABLE;        // enable clock on GPIOF
    RCC->APB2ENR |= PERIPH_ADC3_ENABLE;         // enable ADC3
    GPIOF->MODER |= GPIOF_MODER_ANALOG;         // PF.6 is used as analog input
    ADCCOM->CCR =   0x0;
    ADC3->SMPR1 =   0x0;
    ADC3->SMPR2 =   (0x6 << 12u);               // ch4: 144 cycles sampling time
    ADC3->SQR1 =    0x0;                        // sequence length: 1
    ADC3->SQR2 =    0x0;
    ADC3->SQR3 =    0x4;                        // ch4 is the first and only 
                                                // channel in sequence
    ADC3->CR1 =     (0x3 << 24u);               // 6bit resolution
    ADC3->CR2 =     0x3;                        // continuous conversion, enable
                                                // ADC, right align
    ADC3->CR2 |=    (0x1 << 30u);               // start conversion
}


/*
 * See header file
 */

event_t eh_get_event(void)
{
    static uint8_t  button_value_old =  0xffu;
    static uint32_t dip_value_old =     0xffffffffu;
    static uint16_t timer_old =         0u;

    /* detect timer event */
    uint16_t timer = timer_read();
    bool timer_elapsed = (timer == 0u && timer_old != 0u);
    timer_old = timer;
    
    if (timer_elapsed) {
        return EV_TIMEOUT;
    }
    
    
    /* detect button events */
    uint8_t button_value = CT_BUTTON;
    uint8_t button_edge_pos = ~button_value_old & button_value;
    button_value_old = button_value;
    
    if (button_edge_pos & BUTTON_F0_MASK) {
        return EV_BUTTON_F0;
    }
    else if (button_edge_pos & BUTTON_F1_MASK) {
        return EV_BUTTON_F1;
    }
    
    
    /* detect dip switch events */
    uint32_t dip_value = CT_DIPSW->WORD;
    uint32_t dip_edge_pos = ~dip_value_old &  dip_value;
    uint32_t dip_edge_neg =  dip_value_old & ~dip_value;
    dip_value_old = dip_value;

    if (dip_edge_pos & DOOR0_MASK) {
        return EV_DOOR0_OPEN_REQ;
    }
    else if (dip_edge_neg & DOOR0_MASK) {
        return EV_DOOR0_CLOSE_REQ;
    }
    else if (dip_edge_pos & DOOR1_MASK) {
        return EV_DOOR1_OPEN_REQ;
    }
    else if (dip_edge_neg & DOOR1_MASK) {
        return EV_DOOR1_CLOSE_REQ;
    }
    
    
    /* detect weight/potentiometer event, if weight control is enabled */
    if (wctl_state == WCTL_ENABLE) {
        uint16_t weight_value = WEIGHT_MAX_VALUE - ADC3->DR;
        // 2 kg Hysteresis to avoid too many events
        if ((weight_value > (weight_value_old + 1)) || 
                (weight_value_old > (weight_value + 1))) {
            weight_value_old = weight_value;
            eh_7seg_display(true, weight_value);
            
            // weight has changed.. compare it against the "filtered" current 
            // weight limit
            if (weight_value_old > current_weight_limit) {
                return EV_WEIGHT_TOO_HIGH;
            } else {
                return EV_WEIGHT_OK;
            }
        }
    }


    /* detect simulated movement events */
    if (flag_F0_Reached) {
        flag_F0_Reached = false;
        return EV_F0_REACHED;
    }
    else if (flag_F1_Reached) {
        flag_F1_Reached = false;
        return EV_F1_REACHED;
    }

    
    return EV_NO_EVENT;
}


/*
 * See header file
 */
void eh_weight_control(weight_control_t wctl_cmd, uint16_t weight_limit)
{
    wctl_state = wctl_cmd;
    if (wctl_cmd == WCTL_ENABLE) {
        // store given weight limit & reset 'last weight value'
        current_weight_limit = weight_limit;
        weight_value_old = 0xffffu;
        
    } else {     // WCTL_DISABLE
        // clear 7seg display
        eh_7seg_display(false, 0);
    }
}



/* Local function definitions
 * ------------------------------------------------------------------------- */

static void eh_7seg_display(bool turn_display_on, uint16_t value)
{
    const uint8_t digit_pattern[10] = { 0x3f, 0x06, 0x5b, 0x4f, 0x66, 
                                        0x6d, 0x7d, 0x07, 0x7f, 0x6f };
    
    uint32_t raw_pattern = 0x0;     // default: all segments off
    char s[6];
    
    if (turn_display_on) {
        // turn the given value into its decimal string representation
        int len = snprintf(s, sizeof(s), "%u", value);
        
        // turn up to 4 digits into segment patterns
        if (len > 0 && len <= 4) {
            for (uint16_t i = 1; i<=len; i++) {
                uint8_t decimal_digit = s[i-1] - '0';
                raw_pattern |= (digit_pattern[decimal_digit] << 8*(len-i));
            }
        }
    }
    
    // send pattern to 7seg display (segments are active low)
    CT_SEG7->RAW.WORD = ~raw_pattern;
}


