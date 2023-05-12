/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/**
 *  \brief  Implementation of module hal_timer.
 *
 *  The hardware abstraction layer for the timers 2 to 5.
 *
 *  $Id$
 * ------------------------------------------------------------------------- */

/* User includes */
#include "hal_timer.h"


/* -- Macros
 * ------------------------------------------------------------------------- */
 
#define CHECK_ARRAY_OUT_OF_BOUNDS_COMPARE        0x04


/* -- Local type definitions 
 * ------------------------------------------------------------------------- */

/**
 *  \struct irq_param_t
 *  \brief  Contains misc. parameters about Timer irq, used internally only.
 */
typedef struct {
    uint32_t nvic_irq_mask;         /**< Mask for the interrupt. */
    uint32_t nvic_reg_nr;           /**< "Bank" nr of the related register. */
} irq_param_t;


/* -- Local function declarations
 * ------------------------------------------------------------------------- */

static irq_param_t get_irq_param(reg_tim_t *timer, hal_timer_irq_t irq);
static void set_channel_polarity(reg_tim_t *timer, 
                                 hal_timer_channel_t channel,
                                 hal_timer_polarity_t polarity);
static void set_channel_ccreg(reg_tim_t *timer, 
                              hal_timer_channel_t channel,
                              uint32_t reg);
static void adv_main_output_set(reg_tim_t *timer,
                                hal_bool_t status);


/* -- Public function definitions
 * ------------------------------------------------------------------------- */

/*
 * See header file
 */
void hal_timer_reset(reg_tim_t *timer)
{
    irq_param_t irq_params;
    
    /* Disable interrupt */
    if(timer == TIM1 || timer == TIM8) {
        irq_params = get_irq_param(timer, HAL_TIMER_IRQ_UE);
        if (irq_params.nvic_reg_nr == 0u) {
            NVIC->ICER0 |= irq_params.nvic_irq_mask;
        } else if (irq_params.nvic_reg_nr == 1u) {
            NVIC->ICER1 |= irq_params.nvic_irq_mask;
        }
          
        irq_params = get_irq_param(timer, HAL_TIMER_IRQ_CC1);
        if (irq_params.nvic_reg_nr == 0u) {
            NVIC->ICER0 |= irq_params.nvic_irq_mask;
        } else if (irq_params.nvic_reg_nr == 1u) {
            NVIC->ICER1 |= irq_params.nvic_irq_mask;
        }        
        
        irq_params = get_irq_param(timer, HAL_TIMER_IRQ_TRIGGER);
        if (irq_params.nvic_reg_nr == 0u){
            NVIC->ICER0 |= irq_params.nvic_irq_mask;
        } else if (irq_params.nvic_reg_nr == 1u) {
            NVIC->ICER1 |= irq_params.nvic_irq_mask;
        } 
        
    } else {
        irq_params = get_irq_param(timer, HAL_TIMER_IRQ_UE);
        if (irq_params.nvic_reg_nr == 0u){
            NVIC->ICER0 |= irq_params.nvic_irq_mask;
        } else if (irq_params.nvic_reg_nr == 1u) {
            NVIC->ICER1 |= irq_params.nvic_irq_mask;
        }
    }

    /* Reset timer registers */
    timer->CR1 = 0u;
    timer->CR2 = 0u;
    timer->SMCR = 0u;
    timer->DIER = 0u;
    timer->CCMR1 = 0u;
    timer->CCMR2 = 0u;
    timer->CCER = 0u;
    timer->PSC = 0u;
    timer->ARR = 0u;
    timer->CCR1 = 0u;
    timer->CCR2 = 0u;
    timer->CCR3 = 0u;
    timer->CCR4 = 0u;
    timer->DCR = 0u;
    timer->DMAR = 0u;
    timer->BDTR = 0u;
}


/*
 * See header file
 */
void hal_timer_init_base(reg_tim_t *timer,
                         hal_timer_base_init_t init)
{
    uint32_t reg;
    
    /* process prescaler */
    if (init.prescaler != 0u) {
        init.prescaler--;       // 0x00 means prescaler = 1!
    }
    timer->PSC = init.prescaler;

    /* process mode */
    reg = (init.mode << 4u) |
          (init.run_mode << 3u);
    timer->CR1 &= ~(0x78);
    timer->CR1 |= reg;

    /* process counter value, TIM2 and TIM5 are 32 bit counter! */
    if (timer == TIM2 || timer == TIM5) {
        timer->ARR = init.count;
    } else {
        timer->ARR = (uint16_t)init.count;
    }
    
    /* process master mode */
    timer->CR2 &= ~(0x7 << 4u);
    timer->CR2 |= (init.master_mode << 4u);
}


/*
 * See header file
 */
void hal_timer_init_clock(reg_tim_t *timer, hal_timer_clock_init_t init)
{    
    /* Rest SMCR */
    timer->SMCR = 0u;
    
    /* Clock source */
    if (init.source == HAL_TIMER_CLKSRC_ETRF) {
        timer->CR1 |= (init.filter_div << 8u);
        timer->SMCR |= (init.filter.prescaler << 12u);
        timer->SMCR |= (init.filter.mode << 8u);
        timer->SMCR |= (init.filter.polarity << 15u);
    }
    
    /* Trigger source */
    timer->SMCR |= (init.trg_source << 4u);
    
    /* Slavemode */
    timer->SMCR |= init.slave_mode;
}


/*
 * See header file
 */
void hal_timer_init_input(reg_tim_t *timer,
                          hal_timer_channel_t channel, 
                          hal_timer_input_init_t init)
{
    uint32_t reg;

    /* process input */
    reg = (init.input << 0u) |              // CCMR: CCyS
          (init.filter.prescaler << 2u) |   // CCMR: ICyPSC
          (init.filter.mode << 4u);         // CCMR: ICyF
    // prescaler
    set_channel_ccreg(timer, channel, reg);

    /* process polarity and state */
    set_channel_polarity(timer, channel, init.filter.polarity);
    hal_timer_channel_set(timer, channel, init.input_state);
}


/*
 * See header file
 */
void hal_timer_init_output(reg_tim_t *timer,
                           hal_timer_channel_t channel,
                           hal_timer_output_init_t init)
{
    uint32_t reg;

    /* process mode and set as output */
    reg = (init.mode << 4u);    // CCMR: OCyM
    set_channel_ccreg(timer, channel, reg);

    /* process polarity and state */
    set_channel_polarity(timer, channel, init.polarity);
    hal_timer_channel_set(timer, channel, init.output_state);

    /* set compare */
    hal_timer_compare_write(timer, channel, init.pulse);
    
    /* Advanced timer 1/8 */
    if (timer == TIM1 || timer == TIM8) {
        adv_main_output_set(timer, ENABLE);
    }    
}


/*
 * See header file
 */
void hal_timer_start(reg_tim_t *timer)
{
    /* Start timer */
    timer->CR1 |= (0x1 << 0u);
}


/*
 * See header file
 */
void hal_timer_stop(reg_tim_t *timer)
{
    /* Stop timer */
    timer->CR1 &= ~(0x1 << 0u);
}


/*
 * See header file
 */
void hal_timer_channel_set(reg_tim_t *timer,
                           hal_timer_channel_t channel,
                           hal_bool_t status)
{
    uint8_t bit_pos = 0u;
    uint8_t ch_pos = (channel & 0xf) * 4;
    
    /* Shift bit to complementary position */
    if ((channel & 0x10)) {
        bit_pos = 2u;
    }
    
    if (status == ENABLE) {
        timer->CCER |= (((0x1 << bit_pos) << ch_pos));
    } else {
        timer->CCER &= ~(((0x1 << bit_pos) << ch_pos));
    }        
}


/*
 * See header file
 */
void hal_timer_irq_set(reg_tim_t *timer,
                       hal_timer_irq_t irq,
                       hal_bool_t status)
{
    irq_param_t irq_params = get_irq_param(timer, irq);

    if (status == ENABLE) {
        timer->DIER |= irq;
        if (irq_params.nvic_reg_nr == 0) {
            NVIC->ISER0 |= irq_params.nvic_irq_mask;
        } else if (irq_params.nvic_reg_nr == 1) {
            NVIC->ISER1 |= irq_params.nvic_irq_mask;
        }
    } else {
        timer->DIER &= ~irq;
        if (irq_params.nvic_reg_nr == 0) {
            NVIC->ICER0 |= irq_params.nvic_irq_mask;
        } else if (irq_params.nvic_reg_nr == 1) {
            NVIC->ICER1 |= irq_params.nvic_irq_mask;
        }
    }
}


/*
 * See header file
 */
hal_bool_t hal_timer_irq_status(reg_tim_t *timer, hal_timer_irq_t irq)
{
    hal_bool_t status = DISABLED;

    if ((timer->DIER && irq) &&
        (timer->SR && irq)) {
        status = ENABLED;
    }

    return status;
}


/*
 * See header file
 */
void hal_timer_dma_set(reg_tim_t *timer, 
                       hal_timer_dmasrc_t dma,
                       hal_bool_t status)
{
    if (status == ENABLE) {
        timer->DIER |= dma;
    } else {
        timer->DIER &= ~dma;
    }
}


/*
 * See header file
 */
void hal_timer_irq_clear(reg_tim_t *timer, hal_timer_irq_t irq)
{
    timer->SR &= ~irq;
}


/*
 * See header file
 */
uint32_t hal_timer_counter_read(reg_tim_t *timer)
{
    uint32_t value = timer->CNT;

    if (timer != TIM2 && timer != TIM5) {
        value &= 0xffff;
    }

    return value;
}


/*
 * See header file
 */
void hal_timer_reload_write(reg_tim_t *timer, uint32_t value)
{
    if (timer == TIM2 || timer == TIM5) {
        timer->ARR = value;
    } else {
        timer->ARR = (uint16_t)value;
    }
}


/*
 * See header file
 */
void hal_timer_prescaler_write(reg_tim_t *timer, uint32_t value)
{
    /* Decrement prescaler if not 0. */
    if (value != 0u) {
        value--;
    }
    
    if (timer == TIM2 || timer == TIM5) {
        timer->PSC = value;
    } else {
        timer->PSC = (uint16_t)value;
    }
}


/*
 * See header file
 */
uint32_t hal_timer_compare_read(reg_tim_t *timer,
                                hal_timer_channel_t channel)
{
    uint32_t value = 0;

    if (channel == HAL_TIMER_CH1N) {
        channel = HAL_TIMER_CH1;
    } else if (channel == HAL_TIMER_CH2N) {
        channel = HAL_TIMER_CH2;
    } else if (channel == HAL_TIMER_CH3N) {
        channel = HAL_TIMER_CH3;
    }
    
    if (channel >= CHECK_ARRAY_OUT_OF_BOUNDS_COMPARE) {
        return (0);
    }
    
    /* convert pulse */
    if (channel == 0) {
        value = timer->CCR1;
    } else if (channel == 1) {
        value = timer->CCR2;
    } else if (channel == 2) {
        value = timer->CCR3;
    } else if (channel == 3) {
        value = timer->CCR4;
    }

    if (timer != TIM2 && timer != TIM5) {
        value &= 0xffff;
    }

    return value;
}


/*
 * See header file
 */
int8_t hal_timer_compare_write(reg_tim_t *timer,
                                hal_timer_channel_t channel,
                                uint32_t value)
{
    if (channel == HAL_TIMER_CH1N) {
        channel = HAL_TIMER_CH1;
    } else if (channel == HAL_TIMER_CH2N) {
        channel = HAL_TIMER_CH2;
    } else if (channel == HAL_TIMER_CH3N) {
        channel = HAL_TIMER_CH3;
    }

    if (channel >= CHECK_ARRAY_OUT_OF_BOUNDS_COMPARE) {
        return (-1);
    } else {
        /* convert pulse */
        if (timer != TIM2 && timer != TIM5){
            value = (uint16_t)value;
        }

        if (channel == 0) {
            timer->CCR1 = value;
        } else if (channel == 1) {
            timer->CCR2 = value;
        } else if (channel == 2) {
            timer->CCR3 = value;
        } else if (channel == 3) {
            timer->CCR4 = value;
        }

        return 0;
    }

}

/* -- Local function definitions
 * ------------------------------------------------------------------------- */

/**
 *  \brief  Get interrupt parameters for specified timer.
 *  \param  timer : Timer to get interrupt number from.
 *  \param  irq : Specifies interrupt.
 *  \return Interrupt parameters.
 */
static irq_param_t get_irq_param(reg_tim_t *timer, hal_timer_irq_t irq)
{
    irq_param_t params = { 0u, 0u };
    
    if (timer == TIM1) {
        params.nvic_reg_nr = 0u;
        switch(irq) {
            default:
            case HAL_TIMER_IRQ_UE:
                params.nvic_irq_mask = (0x1 << 25u);
                break;
            case HAL_TIMER_IRQ_CC1:
            case HAL_TIMER_IRQ_CC2:
            case HAL_TIMER_IRQ_CC3:
            case HAL_TIMER_IRQ_CC4:
                params.nvic_irq_mask = (0x1 << 27u);
                break;
            case HAL_TIMER_IRQ_TRIGGER:
                params.nvic_irq_mask = (0x1 << 26u);
                break;
            // BRK not implemented yet
        }        
    } 
    else if (timer == TIM2) {
        params.nvic_reg_nr = 0u;
        params.nvic_irq_mask = (0x1 << 28u);
    } 
    else if (timer == TIM3) {
        params.nvic_reg_nr = 0u;
        params.nvic_irq_mask = (0x1 << 29u);
    } 
    else if (timer == TIM4) {
        params.nvic_reg_nr = 0u;
        params.nvic_irq_mask = (0x1 << 30u);
    } 
    else if (timer == TIM5) {
        params.nvic_reg_nr = 1u;
        params.nvic_irq_mask = (0x1 << 18u);
    } 
    else if (timer == TIM6) {
        params.nvic_reg_nr = 1u;
        params.nvic_irq_mask = (0x1 << 22u);
    } 
    else if (timer == TIM7) {
        params.nvic_reg_nr = 1u;
        params.nvic_irq_mask = (0x1 << 23u);
    } 
    else if (timer == TIM8) {
        params.nvic_reg_nr = 1u;
        switch(irq) {
            default:
            case HAL_TIMER_IRQ_UE:
                params.nvic_irq_mask = (0x1 << 12u);
                break;
            case HAL_TIMER_IRQ_CC1:
            case HAL_TIMER_IRQ_CC2:
            case HAL_TIMER_IRQ_CC3:
            case HAL_TIMER_IRQ_CC4:
                params.nvic_irq_mask = (0x1 << 14u);
                break;
            case HAL_TIMER_IRQ_TRIGGER:
                params.nvic_irq_mask = (0x1 << 13u);
                break;
            // BRK not implemented yet
        }
    } 
    else if (timer == TIM9) {
        params.nvic_reg_nr = 0u;
        params.nvic_irq_mask = (0x1 << 24u);
    } 
    else if (timer == TIM10) {
        params.nvic_reg_nr = 0u;
        params.nvic_irq_mask = (0x1 << 25u);
    } 
    else if (timer == TIM11) {
        params.nvic_reg_nr = 0u;
        params.nvic_irq_mask = (0x1 << 26u);
    } 
    else if (timer == TIM12) {
        params.nvic_reg_nr = 1u;
        params.nvic_irq_mask = (0x1 << 11u);
    } 
    else if (timer == TIM13) {
        params.nvic_reg_nr = 1u;
        params.nvic_irq_mask = (0x1 << 12u);
    } 
    else if (timer == TIM14) {
        params.nvic_reg_nr = 1u;
        params.nvic_irq_mask = (0x1 << 13u);
    }        

    return params;
}


/**
 *  \brief  Set the channel polarity bits in the CCE register.
 *  \param  timer : Timer to get interrupt number from.
 *  \param  channel : Specifies channel.
 *  \param  polarity : Polarity to configure.
 */
static void set_channel_polarity(reg_tim_t *timer, 
                                 hal_timer_channel_t channel,
                                 hal_timer_polarity_t polarity)
{
    uint8_t bit_pos = 1u;
    uint8_t ch_pos = (channel & 0xf) * 4;
    
    /* Shift bit to complementary position */
    if ((channel & 0x10)) {
        bit_pos <<= 2u;
    }
    
    /* Shift polarity bit to bit position (complementary output).
       Shift to correct channel position. */
    timer->CCER &= ~(((0x1 << bit_pos) << ch_pos));
    timer->CCER |= (((polarity << bit_pos) << ch_pos));
}


/**
 *  \brief  Set the channels capture/compare mode.
 *  \param  timer : Timer to get interrupt number from.
 *  \param  channel : Specifies channel.
 *  \param  reg : Bits to set in the register.
 */
static void set_channel_ccreg(reg_tim_t *timer, 
                              hal_timer_channel_t channel,
                              uint32_t reg)
{
    uint32_t clear_reg = ~(0xff);
    switch (channel) {
        case HAL_TIMER_CH2:
        case HAL_TIMER_CH2N:
            clear_reg <<= 8u;       // shift for ch. 2
            clear_reg |= 0xff;      // don't overwrite ch. 1
            reg <<= 8u;             // shift for ch. 2

        default:
        case HAL_TIMER_CH1:
        case HAL_TIMER_CH1N:
            timer->CCMR1 &= clear_reg;
            timer->CCMR1 |= reg;
            break;

        case HAL_TIMER_CH4:
            clear_reg <<= 8u;       // shift for ch. 4
            clear_reg |= 0xff;      // don't overwrite ch. 3
            reg <<= 8u;             // shift for ch. 4

        case HAL_TIMER_CH3:
        case HAL_TIMER_CH3N:
            timer->CCMR2 &= clear_reg;
            timer->CCMR2 |= reg;
            break;
    }
}


/**
 *  \brief  Set the main output of advanced timers.
 *  \param  timer : Timer to get interrupt number from.
 *  \param  status : ENABLE/DISABLE main output.
 */
static void adv_main_output_set(reg_tim_t *timer,
                                hal_bool_t status)
{
    /* Main output enable / disable */
    if (status == ENABLE) {
        timer->BDTR |= (0x1 << 15u);
    } else {
        timer->BDTR &= ~(0x1 << 15u);
    }
}
