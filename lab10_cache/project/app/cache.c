/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */

/* User includes */
#include "cache.h"

/* Struct to store the results */
static struct HitMiss hit_miss;

/* Struct to store the blocks */
static struct BlockLine block_lines[LINE_COUNT];

/* init_cache
 *
 * Function to initialize the cache simulation
 *
 * @return void
 *
 */
void init_cache(void)
{
    /* Local Variables */
    int i;

    /* Init blocks where valid = 0 */
    for (i = 0; i < LINE_COUNT; i++) {
        block_lines[i].valid = 0;
    }

    /* Init hit/miss counter to 0 */
    hit_miss.hits = 0;
    hit_miss.misses = 0;
}

/* access_cache
 *
 * Function that reads data from a cache. Returns 0 on failure
 * or 1 on success.
 *
 * @param       address
 *
 * @return      result_t
 */
result_t access_cache(uint32_t address)
{
    /* Calculate tag and index*/
    uint32_t tag = TAG_GET(address);
    uint32_t index = INDEX_GET(address);

    /* Check if block is valid and if the tags matches */
    if (block_lines[index].valid == 1 && block_lines[index].tag == tag) {
        /* Hit*/
        hit_miss.hits++;

        return RESULT_HIT;
    }else {
        /* Miss */
        hit_miss.misses++;

        /* Simulate Block read from RAM -> block is valid now */
        block_lines[index].valid = 1;
        block_lines[index].tag = tag;

        return RESULT_MISS;
    }
}

/* get_cache_result
 *
 * Return a pointer to the cache results
 *
 * @return      Cache
 */
struct HitMiss *get_cache_result(void)
{
    return &hit_miss;
}
