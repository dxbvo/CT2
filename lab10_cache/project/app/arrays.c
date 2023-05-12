/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */

/* User includes */
#include "arrays.h"

/* a_equals_b_plus_c
 *
 * Simulate a[row, col] = b[row, col] + c[row, col]
 *
 * @param       row index
 * @param       column index
 *
 * @return      void
 */
void a_equals_b_plus_c(uint16_t row, uint16_t col)
{
    /* Read from array b */
    read_b(row, col);

    /* Read from array c */
    read_c(row, col);

    /* Write to array a*/
    write_a(row, col);
}

/* write_a
 *
 * Simulate a write to array a
 *
 * @param       row index
 * @param       column index
 *
 * @return      void
 */
void write_a(uint16_t row, uint16_t col)
{
    /* Get the address */
    uint32_t address = get_item_address(ARRAY_INDEX_A, row, col);

    /* Simulate cache access */
    result_t result = access_cache(address);

    display_result(WRITE_ACCESS, address, result);
}

/* read_b
 *
 * Simulate a read from array b
 *
 * @param       row index
 * @param       column index
 *
 * @return      void
 */
void read_b(uint16_t row, uint16_t col)
{
    /* Get the address */
    uint32_t address = get_item_address(ARRAY_INDEX_B, row, col);

    /* Simulate cache access */
    result_t result = access_cache(address);

    display_result(READ_ACCESS, address, result);
}

/* read_c
 *
 * Simulate a read from array c
 *
 * @param       row index
 * @param       column index
 *
 * @return      void
 */
void read_c(uint16_t row, uint16_t col)
{
    /* Get the address */
    uint32_t address = get_item_address(ARRAY_INDEX_C, row, col);

    /* Simulate cache access */
    result_t result = access_cache(address);

    display_result(READ_ACCESS, address, result);
}

/* display_result
 *
 * Get the corresponding address
 *
 * @param       str
 * @param       address
 * @param       address
 *
 * @return      
 */
void display_result(access_t access, uint32_t address, result_t result)
{
    /* Local Variables */
    debug_level_t debug_level;
    char str[40];

    while (button2_pressed()) {}
        while (!button1_pressed() && !button2_pressed()) {}

            if (access == WRITE_ACCESS) {
                sprintf(str, "Write 0x%.8x", address);
            } 
            else {
                sprintf(str, "Read  0x%.8x", address);
            }

    /* Prepare line */
    if (result == RESULT_HIT) {
        strcat(str, "    Result: Hit  ");
        debug_level = DEBUG_LEVEL_HIT;
    } else {
        strcat(str, "    Result: Miss");
        debug_level = DEBUG_LEVEL_MISS;
    }

    /* Print line */
    debug_line_out(debug_level, str);
    delay();
}


/* get_item_address
 *
 * Get the corresponding address
 *
 * @param       array_index
 * @param       row index
 * @param       column index
 *
 * @return      item address
 */
uint32_t get_item_address(array_index_t array_index, uint16_t row, uint16_t col)
{
    char str[40];
    uint32_t row_size = ITEM_SIZE * ARRAY_COLUMNS;
    uint32_t array_total_size = ITEM_SIZE * ARRAY_ROWS * ARRAY_COLUMNS;

    if (row >= ARRAY_ROWS) {
        sprintf(str, "Row out of bound    row:%d", row);
        debug_line_out(DEBUG_LEVEL_INFO, str);
        while (1) {}
    }
    if (col >= ARRAY_COLUMNS) {
        sprintf(str, "Columns out of boundcol:%d", col);
        debug_line_out(DEBUG_LEVEL_INFO, str);
        while (1) {}
    }

    /* Calculate address */
    return (array_index * array_total_size) + (row * row_size) + (col * ITEM_SIZE);
}
