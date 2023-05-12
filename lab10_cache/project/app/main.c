/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zurich University of             -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                 -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Project     : MC1 Cache
 * --------------------------------------------------------------- */

/* User includes */
#include "simulation.h"
#include "cache.h"
#include "config.h"


/* run_simulation
 *
 * Run the simulation
 *
 * @return      void
 */
static void run_simulation(void)
{
    /* Loop through columns */
    for (uint16_t j = 0; j < ARRAY_COLUMNS; j++) {
        /* Loop through rows */
        for (uint16_t i = 0; i < ARRAY_ROWS; i++) {
            // Replaces a[x, y] = b[x, y] + c[x, y]
            a_equals_b_plus_c(i, j);
        }
    }
}


/* Main */
int main(void)
{
    /* Create the cache */
    init_cache();

    /* Cache created */
    debug_line_out(DEBUG_LEVEL_INFO, "Cache created       -> Press T0 or T1");

    /* Start the simulation*/
    run_simulation();

    /* Print simulation results */
    print_results(get_cache_result());

    while (1) {}
}
