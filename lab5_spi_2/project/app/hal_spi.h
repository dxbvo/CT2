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
 * -- $Id: hal_spi.h 1654 2015-03-12 10:13:22Z muln $
 * ------------------------------------------------------------------
 */

#ifndef _SPI_H
#define _SPI_H

#include <stdint.h>

/**
 * Initialize SPI1 Interface (P5.4, P5.5, P5.6, P5.7)
 *
 * No parameters
 *
 * No returns
 */
void hal_spi_init(void);


/**
 * Read/Write data via SPI1 on Port P5
 *
 * Parameters:
 * - uint8_t data: data to be sent via spi
 *
 * Returns: Data received from spi
 */
uint8_t hal_spi_read_write(uint8_t send_byte);
#endif    /* _SPI_H */
