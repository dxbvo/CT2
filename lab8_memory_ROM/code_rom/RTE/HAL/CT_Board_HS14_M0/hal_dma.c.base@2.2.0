/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/**
 *  \brief  Implementation of module hal_dma->
 *
 *  The hardware abstraction layer for direct memory access.
 *
 *  $Id$
 * ------------------------------------------------------------------------- */

/* User includes */
#include "hal_dma.h"


/* -- Macros
 * ------------------------------------------------------------------------- */

#define DMA_STREAM_ENABLE       0x00000001


/* -- Public function definitions
 * ------------------------------------------------------------------------- */

/*
 * See header file.
 */
void hal_dma_init_base(reg_dma_t *dma, 
                       hal_dma_stream_t stream, 
                       hal_dma_init_t init)
{    
    /* Setup channel */
    dma->STREAM[stream].CR &= ~(0x7 << 25u);
    dma->STREAM[stream].CR |= init.channel << 25u;
    
    /* Setup direction */
    dma->STREAM[stream].CR &= ~(0x3 << 6u);
    dma->STREAM[stream].CR |= init.direction << 6u;
    
    /* Setup source and destination */
    dma->STREAM[stream].M1AR = 0u;
    switch(init.direction) {
        default:
        case HAL_DMA_PER_TO_MEM:
        case HAL_DMA_MEM_TO_MEM:
            dma->STREAM[stream].PAR = init.source;
            dma->STREAM[stream].M0AR = init.destination;
            break;
        
        case HAL_DMA_MEM_TO_PER:
            dma->STREAM[stream].PAR = init.destination;
            dma->STREAM[stream].M0AR = init.source;
            break;
    }
    
    /* Setup buffer size */
    dma->STREAM[stream].CR &= ~(0xf << 11u);
    dma->STREAM[stream].CR |= init.size << 11u;
    dma->STREAM[stream].CR |= init.size << 13u;
    
    /* Setup nr of transfers */
    dma->STREAM[stream].NDTR = init.nr_transactions;
    
    /* Setup circular mode */
    dma->STREAM[stream].CR &= ~(0x1 << 8u);
    dma->STREAM[stream].CR |= init.continous << 8u;
}


/*
 * See header file.
 */
void hal_dma_start(reg_dma_t *dma, hal_dma_stream_t stream)
{
    dma->STREAM[stream].CR |= DMA_STREAM_ENABLE;
}


/*
 * See header file.
 */
void hal_dma_stop(reg_dma_t *dma, hal_dma_stream_t stream)
{
    dma->STREAM[stream].CR &= ~DMA_STREAM_ENABLE;
}


