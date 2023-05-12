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
 * --
 * -- $Id: hal_spi.c 4720 2019-03-04 10:11:31Z akdi $
 * ------------------------------------------------------------------
 */

#ifdef MOCKED_SPI_DISPLAY
#include "hal_spi.h"
#include "hal_mocked.h"
void hal_spi_init(void)
{
    hal_mocked_spi_init();
}
uint8_t hal_spi_read_write(uint8_t send_byte)
{
    return hal_mocked_spi_read_write(send_byte);
}
#else  // !MOCKED_SPI_DISPLAY
#include <reg_stm32f4xx.h>
#include "hal_spi.h"

#define BIT_TXE  (uint32_t)0x00000002
#define BIT_RXNE (uint32_t)0x00000001
#define BIT_BSY  (uint32_t)0x00000080

#define RXNE 0
#define TXE 1
#define BSY 7
#define MSTR 2
#define BR 3
#define SPE 6
#define LSBFIRST 7
#define SSI 8
#define SSM 9
#define RXONLY 10
#define DFF 11

static void set_ss_pin_low(void);
static void set_ss_pin_high(void);
static void wait_10_us(void);

/*
 * according to description in header file
 */
void hal_spi_init(void)
{
    RCC->APB2ENR |= 0x00001000;     // enable SPI clock
    RCC->AHB1ENR |= 0x00000001;     // start clock on GPIO A
    GPIOA->OSPEEDR &= 0xFFFF00FF;     // clear P4 to P7
    GPIOA->OSPEEDR |= 0x0000FF00;     // set P4 to P7 to 100 MHz
    GPIOA->MODER &= 0xFFFF00FF;       // clear mode on P5 to P7
    // P5 to P7, P4 output mode
    GPIOA->MODER |= 0x0000A900;       // Set alternate function mode on
    // P5 to P7, P4 output mode
    GPIOA->AFRL &= 0x0000FFFF;      // clear alternate function
    GPIOA->AFRL |= 0x55550000;      // Set SPI1 alternate function

    SPI1->CR2 = 0x0000;               // set spi to default state
    SPI1->CR1 = 0x0000;               // set spi to default state

		SPI1->CR1 |=(1<<MSTR) | (7<<BR) | (1<<SSM)| (1<<SSI); //SPI1->CR1 = 0x33C;
		SPI1->CR1 |=(1<<SPE);				// enable SPI
		
		// sets up the SPI1 interface as a master device with the lowest possible baud rate 
		// and enables software slave management, internal slave select, and the SPI interface itself.

    set_ss_pin_high();
}

/*
 * according to description in header file
 */
uint8_t hal_spi_read_write(uint8_t send_byte) {
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
 * Set Slave-Select Pin (Port A, Pin 5) low
 *
 * No parameters
 *
 * No returns
 */
static void set_ss_pin_low(void)
{
    GPIOA->BSRR |= 0x0010 << 16;              // Set Port A, Pin 5 low
}

/**
 * Set Slave-Select Pin (Port A, Pin 5) high
 *
 * No parameters
 *
 * No returns
 */
static void set_ss_pin_high(void)
{
    GPIOA->BSRR |= 0x0010;             // Set Port A, Pin 5 high
}

/**
 * Wait for approximately 10us
 *
 * No parameters
 *
 * No returns
 */
static void wait_10_us(void)
{
    uint8_t counter = 0;
    while (counter < 160) {
        counter++;
    }
}
#endif // MOCKED_SPI_DISPLAY
