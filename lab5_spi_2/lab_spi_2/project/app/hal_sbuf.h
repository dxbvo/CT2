/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zuercher Hochschule Winterthur   -
 * --  _| |_| | | | |____ ____) |  (University of Applied Sciences) -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Module      : CT2 lab - SPI Display supporting hal for sbuf signal
 * --
 * -- $Id: $
 * ------------------------------------------------------------------
 */

#ifndef _SBUF_H
#define _SBUF_H

#include <stdint.h>

/**
 * Initialize Port 5.9 as input to receive SBUF-signal from display
 */
void hal_sbuf_init(void);

/*
 * The function checks the SBUF pin of the display.
 * @return  It returns 1 if the pin is set; 0 otherwise
 */
uint8_t hal_sbuf_get_state(void);

#endif    /* _SBUF_H */
