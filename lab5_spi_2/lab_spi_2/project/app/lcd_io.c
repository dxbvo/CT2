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
 * -- Description : Contains the implementations of the functions
 * --               to write and read frames from and to the TFT-LCD
 * --               display EAeDIPTFT43-A.
 * --
 * -- $Id: lcd_io.c 4720 2019-03-04 10:11:31Z akdi $
 * ------------------------------------------------------------------
 */
#include "lcd_io.h"
#include "hal_spi.h"
#include "hal_sbuf.h"

#define ACK_CHAR         (uint8_t)0x06
#define DC1_CHAR         (uint8_t)0x11
#define DC2_CHAR         (uint8_t)0x12
#define ESC_CHAR         (uint8_t)0x1B
#define ONE_CHAR         (uint8_t)0x01

#define NOTHING_RECEIVED (uint8_t)0
enum { SUCCESS = 0, ERRORCODE = 1 };

/* ------------------------------------------------------------------
 * -- Function prototypes
 * ------------------------------------------------------------------
 */
static void send_read_display_buffer_request(void);

/* ------------------------------------------------------------------
 * -- Function implementations
 * ------------------------------------------------------------------
 */

/*
 * according to description in header file
 */
void init_display_interface(void)
{
    hal_spi_init();
    hal_sbuf_init();
}

/*
 * according to description in header file
 */
uint8_t read_display_buffer(uint8_t *readBuffer)
{
    /// STUDENTS: To be programmed
	
	// Check if the state of hal_sbuf is ready to receive data
	if (!hal_sbuf_get_state()) {
		return NOTHING_RECEIVED;
	}

	// send request to display buffer
	send_read_display_buffer_request();

	// wait for ack character
	if (hal_spi_read_write(0x00) != ACK_CHAR) {
		return NOTHING_RECEIVED;
	}
	
	// wait for dc1 character
	// Why is the function called read_write ???
	if (hal_spi_read_write(0x00) != DC1_CHAR) {
		return NOTHING_RECEIVED;
	}

	// read length of the data
	// warning?
	uint8_t length = (uint8_t)hal_spi_read_write(0x00);

	// read data bytes and store them in the readBuffer
	for (uint8_t i = 0; i < length; i++) {
		uint8_t result = hal_spi_read_write(0x00);
		readBuffer[i] = result;
	}

	hal_spi_read_write(0x00);

	return length;
}

/*
 * according to description in header file
 */
uint8_t write_cmd_to_display(const uint8_t *cmdBuffer, uint8_t length) {
    /// STUDENTS: To be programmed
	
	hal_spi_read_write(DC1_CHAR);
	hal_spi_read_write(length+1);
	hal_spi_read_write(ESC_CHAR);
	
	uint8_t bcc = DC1_CHAR + length + 1 + ESC_CHAR;

	for (uint8_t i=0; i<length; i++) {
		hal_spi_read_write(cmdBuffer[i]);
		bcc += cmdBuffer[i];
	}
		hal_spi_read_write(bcc);
	
	if(hal_spi_read_write(0x00) == ACK_CHAR) {
		return SUCCESS;
	} else {
		return ERRORCODE; 
	}
}
/*
 * Assemble and send a packet to trigger the reading of the display buffer
 * Uses the sequence "<DC2>, 0x01, 0x53, checksum" according to datasheet
 */
static void send_read_display_buffer_request() {
    /// STUDENTS: To be programmed
	
	hal_spi_read_write(DC2_CHAR);
	hal_spi_read_write(ONE_CHAR);
	uint8_t charS = 'S'; // pay attention to "" vs ''
	hal_spi_read_write(charS);
	uint8_t bcc = DC2_CHAR + ONE_CHAR + charS;
	hal_spi_read_write(bcc);

    /// END: To be programmed
}
