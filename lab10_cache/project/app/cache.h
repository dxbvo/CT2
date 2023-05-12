/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */

#ifndef CACHE_H_
#define CACHE_H_

#include <stdint.h>
#include <stdlib.h>

/* User includes */
#include "config.h"

/* Masks */
#define OFFSET_MASK ((1 << OFFSET) - 1)
#define INDEX_MASK  (((1 << INDEX) - 1) << OFFSET)
#define TAG_MASK    (0xFFFFFFFF ^ (OFFSET_MASK | INDEX_MASK))

/* Getter macro */
#define OFFSET_GET(addr) ((addr & OFFSET_MASK))
#define INDEX_GET(addr)  ((addr & INDEX_MASK) >> OFFSET)
#define TAG_GET(addr)    ((addr & TAG_MASK) >> (OFFSET + INDEX))

/* Block
 *
 * Holds an integer that states the validity of the bit (0 = invalid,
 * 1 = valid) and the tag being held.
 */
struct BlockLine {
    uint8_t valid;
    uint32_t tag;
};

/* HitMiss
 *
 * Count of Hits and Misses
 *
 */
struct HitMiss {
    uint16_t hits;
    uint16_t misses;
};

/* Typedefs */
typedef enum {
    RESULT_HIT,
    RESULT_MISS
} result_t;

/* init_cache
 *
 * Function to initialize the cache simulation
 *
 * @return void
 *
 */
void init_cache(void);

/* access_cache
 *
 * Function that reads data from a cache. Returns 0 on failure
 * or 1 on success.
 *
 * @param       address
 *
 * @return      result_t
 */
result_t access_cache(uint32_t address);

/* get_cache_result
 *
 * Return a pointer to the cache results
 *
 * @return      Cache
 */
struct HitMiss *get_cache_result(void);

#endif
/* CACHE_H_ */
