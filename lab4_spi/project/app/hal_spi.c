/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zuercher Hochschule Winterthur   -
 * --  _| |_| | | | |____ ____) |  (University of Applied Sciences) -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Module      : SPI Library
 * -- Description :
 * --
 * -- $Id: hal_spi.c 4707 2019-02-26 09:32:59Z ruan $
 * ------------------------------------------------------------------
 */
#include <reg_stm32f4xx.h>
#include "hal_spi.h"

#define BIT_TXE  (uint32_t)0x00000002
#define BIT_RXNE (uint32_t)0x00000001
#define BIT_BSY  (uint32_t)0x00000080

#define MSTR 2
#define BR 3
#define SPE 6
#define LSBFIRST 7
#define SSI 8
#define SSM 9
#define RXONLY 10
#define DFF 11

#define RXNE 0
#define TXE 1
#define BSY 7

static void set_ss_pin_low(void);
static void set_ss_pin_high(void);
static void wait_10_us(void);

/*
 * according to description in header file
 */
void hal_spi_init(void)
{
    RCC->APB2ENR |= 0x00001000;       // enable SPI clock
    RCC->AHB1ENR |= 0x00000001;       // start clock on GPIO A
    
    
    // configure GPIOs
    // P5.5 --> PA4 (NSS)       --> output
    // P5.6 --> PA5 (SCLK)      --> AF5 (see datasheet)
    // P5.7 --> PA6 (MISO)      --> AF5 (see datasheet)
    // P5.8 --> PA7 (MOSI)      --> AF5 (see datasheet)
    // P5.9 --> PA8 (NSBUF)     --> input
    
    GPIOA->OSPEEDR &= 0xFFFF00FF;   // clear PA4-PA7
    GPIOA->OSPEEDR |= 0x0000FF00;   // set PA4-PA7 to very high speed
    
    GPIOA->MODER &= 0xFFFC00FF;     // clear mode on PA4-PA8 --> PA8 = input
        
    GPIOA->MODER |= 0x0000A800;     // set alternate function on PA5-PA7
    GPIOA->MODER |= 0x00000100;     // set PA4 to output
    
    GPIOA->AFRL &= 0x000FFFFF;      // clear alternate function
    GPIOA->AFRL |= 0x55500000;      // Set SPI1 (AF5) alternate function PA5-PA7
    
    // configure SPI
    SPI1->CR2 = 0x0000;             // set spi to default state
    SPI1->CR1 = 0x0000;             // set spi to default state

    // add your SPI configs below (based on reference manual)
    
    /// STUDENTS: To be programmed
    SPI1->CR1 |=(1<<MSTR) | (7<<BR) | (1<<SSM)| (1<<SSI);
		//SPI1->CR1 = 0x33C;
		SPI1->CR1 |=(1<<SPE);				// enable SPI
		
		// sets up the SPI1 interface as a master device with the lowest possible baud rate 
		// and enables software slave management, internal slave select, and the SPI interface itself.
		
    /// END: To be programmed
    
    set_ss_pin_high();
}

/*
 * according to description in header file
 */
uint8_t hal_spi_read_write(uint8_t send_byte)
{
    /// STUDENTS: To be programmed
	
		// select slave
		set_ss_pin_low();
	
		// Waits until Data Register (TXE) flag in the SPI1 status register is set. 
		// This indicates that the SPI data register is empty and ready to receive data to be transmitted 
		while((SPI1->SR & (1<<TXE)) == 0){}
			
		// set data register (DR) to the value of send_byte
		SPI1->DR = send_byte;
		
		// waits until the Receive Data Register Not Empty (RXNE) flag in the SPI1 status register is set. 
		// This indicates that the SPI data register has received data and is ready to be read.
		while((SPI1->SR & (1<<RXNE)) == 0);
			
		// wait 10 us
		wait_10_us();
		
		// deselect slave
		set_ss_pin_high();
		
		// busy bit
		while((SPI1->SR & (1<<BSY)) == 1);
	
		return (uint8_t)SPI1->DR;
}

/**
 * \brief  Set Slave-Select Pin (P5.5 --> PA4) low
 *
 * No parameters
 *
 * No returns
 */
static void set_ss_pin_low(void)
{
    GPIOA->BSRR = 0x00100000;          // Set P5.5 --> PA4 low
}

/**
 * \brief  Set Slave-Select Pin (P5.5 --> PA4) high
 *
 * No parameters
 *
 * No returns
 */
static void set_ss_pin_high(void)
{
    GPIOA->BSRR = 0x00000010;          // Set P5.5 --> PA4 high
}

/**
 * \brief  Wait for approximately 10us
 *
 * No parameters
 *
 * No returns
 */
static void wait_10_us(void) {
    uint8_t counter = 0;
    while (counter < 160) {
        counter++;
    }
}
