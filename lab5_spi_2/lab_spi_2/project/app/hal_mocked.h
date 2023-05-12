/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zuercher Hochschule Winterthur   -
 * --  _| |_| | | | |____ ____) |  (University of Applied Sciences) -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Module      : SPI display mock
 * --
 * -- $Id: $
 * ------------------------------------------------------------------
 */

#ifndef _HAL_MOCKED_H
#define _HAL_MOCKED_H

#include <stdint.h>

/**
 * simulate hal_spi_init()
 */
void hal_mocked_spi_init(void);


/**
 * simulate hal_spi_read_write()
 */
uint8_t hal_mocked_spi_read_write(uint8_t send_byte);

/**
 * simulate hal_sbuf_init()
 */
void hal_mocked_sbuf_init(void);

/**
 * simulate hal_sbuf_get_state()
 */
uint8_t hal_mocked_sbuf_get_state(void);


#endif    /* _HAL_MOCKED_H */
