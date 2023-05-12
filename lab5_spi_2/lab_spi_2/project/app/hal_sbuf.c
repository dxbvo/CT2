/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zuercher Hochschule Winterthur   -
 * --  _| |_| | | | |____ ____) |  (University of Applied Sciences) -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Project     : CT2 lab - SPI Display supporting hal for sbuf signal
 * -- Description : Contains the implementations of the public functions.
 * --
 * -- $Id: $
 * ------------------------------------------------------------------
 */
#ifdef MOCKED_SPI_DISPLAY
#include "hal_sbuf.h"
#include "hal_mocked.h"
void hal_sbuf_init(void)
{
	hal_mocked_sbuf_init();
}
uint8_t hal_sbuf_get_state(void)
{
	return hal_mocked_sbuf_get_state();
}
#else // !MOCKED_SPI_DISPLAY
#include <reg_stm32f4xx.h>
#include "hal_sbuf.h"

#define SBUF_HIGH ((uint16_t)0x0100)

void hal_sbuf_init(void)
{
    RCC->AHB1ENR	|= 0x00000001;           // start clock on GPIO A
    GPIOA->OSPEEDR	|= 0x00030000;           // set P8 to 100 MHz
    GPIOA->MODER	&= 0xFFFCFFFF;           // set input mode on P8
    GPIOA->PUPDR	&= 0xFFFCFFFF;           // no pull-up/pull-down for P8
}

uint8_t hal_sbuf_get_state(void)
{
    uint16_t input_port_a = GPIOA->IDR;
    return (input_port_a &= SBUF_HIGH) == 0; // true if pin is 0, false if pin is 1
}
#endif // MOCKED_SPI_DISPLAY
