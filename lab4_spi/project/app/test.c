/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zuercher Hochschule Winterthur   -
 * --  _| |_| | | | |____ ____) |  (University of Applied Sciences) -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Project     : CT2 lab - SPI Display
 * -- Description : Main program
 * --
 * --               Used to verify the sending of characters in the
 * --               SPI driver.
 * --
 * --               Send out the character read from the dip_switches
 * --               S0 to S7 and display them on Led7 to Led0.
 * --
 * -- $Id: test.c 3683 2016-10-10 11:59:49Z kesr $
 * ------------------------------------------------------------------
 */
#include <stdint.h>
#include <reg_ctboard.h>
#include "hal_spi.h"

int32_t main(void)
{
    uint8_t send_byte;
    uint8_t rec_byte;

    hal_spi_init();

    while (1) {
				// read switches
        send_byte = CT_DIPSW->BYTE.S7_0;
			
				// write leds
        CT_LED->BYTE.LED7_0 = send_byte;
			
				// store send_byte in SPI1->DR and assign to rec_byte
        rec_byte = hal_spi_read_write(send_byte);
			
				// display received byte on leds
        CT_LED->BYTE.LED23_16 = rec_byte;
    }
}
