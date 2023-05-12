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
 * -- Description : Contains the prototypes and interface descriptions
 * --               of the functions to write and read frames from and
 * --               to the TFT-LCD display EAeDIPTFT43-A.
 * --
 * -- $Id: lcd_io.h 1649 2015-03-12 08:57:56Z muln $
 * ------------------------------------------------------------------
 */


#ifndef _LCD_IO_H
#define _LCD_IO_H

#include <stdint.h>

/*
 * The function brings the display interface to a defined state. After
 * the execution the display is ready for communication, i.e. writing to
 * or reading from.
 */
void init_display_interface(void);

/*
 * The function triggers a readout of the display buffer and stores the
 * received data in the string pointed to by readBuffer.
 * The function returns the number of characters read; returns zero in
 * case of an error
 */
uint8_t read_display_buffer(uint8_t *readBuffer);

/*
 * The function receives a command buffer in the form of a character string.
 * It adds the required protocol overhead and uses the SPI to write the
 * complete frame to the display. Afterwards it waits for the acknowledge
 * character from the display.
 *
 * The transmitted frame on the SPI consists of the required control characters
 * (DC1, len and ESC) followed by the contents of the specified command buffer
 * and the checksum (bcc).
 *
 * The function returns zero if the operation is successful, i.e. if an
 * acknowledge character is received from the display; It returns
 * one otherwise, i.e. if no acknowledge character is received from the display
 */
uint8_t write_cmd_to_display(const uint8_t *cmdBuffer, uint8_t length);
#endif    /* _LCD_IO_H */
