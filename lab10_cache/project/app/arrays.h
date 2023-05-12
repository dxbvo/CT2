/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */

#ifndef ARRAYS_H_
#define ARRAYS_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* User includes */
#include "simulation.h"
#include "config.h"
#include "cache.h"

/* Array indices */
typedef enum {
    ARRAY_INDEX_A = 0,
    ARRAY_INDEX_B = 1,
    ARRAY_INDEX_C = 2
} array_index_t;

/* Typedefs */
typedef enum {
    WRITE_ACCESS,
    READ_ACCESS
} access_t;


/* write_a
 *
 * Simulate write to array a
 *
 * @param       row index
 * @param				column index
 *
 * @return      void
 */
void write_a(uint16_t row, uint16_t col);

/* read_b
 *
 * Simulate read from array b
 *
 * @param       row index
 * @param				column index
 *
 * @return      void
 */
void read_b(uint16_t row, uint16_t col);

/* read_c
 *
 * Simulate read from array c
 *
 * @param       row index
 * @param				column index
 *
 * @return      void
 */
void read_c(uint16_t row, uint16_t col);

/* a_equals_b_plus_c
 *
 * Simulate a[row, col] = b[row, col] + c[row, col]
 *
 * @param       row index
 * @param				column index
 *
 * @return      void
 */
void a_equals_b_plus_c(uint16_t row, uint16_t col);

/* get_item_address
 *
 * Get the corresponding address
 *
 * @param				array_index
 * @param       row index
 * @param				column index
 *
 * @return      void
 */
uint32_t get_item_address(array_index_t array_index, uint16_t row, uint16_t col);

void display_result(access_t access, uint32_t address, result_t result);


#endif
/* ARRAYS_H_ */
