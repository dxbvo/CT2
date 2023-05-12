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
 * -- $Id: hal_spi.h 4707 2019-02-26 09:32:59Z ruan $
 * ------------------------------------------------------------------
 */

#ifndef _SPI_H
#define _SPI_H

#include <stdint.h>

/**
 * \brief Initialize SPI1 interface on port P5
 *
 * No parameters
 *
 * No returns
 */
void hal_spi_init(void);


/**
 * \brief Read/write data via SPI1 on Port P5
 *
 * \param data: data to be sent via spi
 *
 * \returns data received from spi
 */
uint8_t hal_spi_read_write(uint8_t send_byte);

#endif    /* _SPI_H */
