/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */

#ifndef CONFIG_H_
#define CONFIG_H_

/* ------------------------------------------------------------------
 * Cache params
 * --------------------------------------------------------------- */

/* Size of the address in bits */
#define ADDRESS_SIZE 11

/* Split Up Cache:  / TAG / INDEX / OFFSET /                       */
/* Offset size in bits */
#define OFFSET 2
/* Index size in bits */
#define INDEX  2

/* Tag size in bits */
#define TAG    (ADDRESS_SIZE - INDEX - OFFSET)

/* Line Count */
#define LINE_COUNT (1 << INDEX)

/* ------------------------------------------------------------------
 * Array params
 * --------------------------------------------------------------- */

/* Row count of the arrays */
#define ARRAY_ROWS 5

/* Column count of the arrays */
#define ARRAY_COLUMNS 10

/* Array item size in Bytes */
#define ITEM_SIZE 1

#endif
/* CONFIG_H_ */
