/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zurich University of             -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                 -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Application for testing external memory
 * --
 * -- $Id: main.c 5605 2023-01-05 15:52:42Z frtt $
 * ------------------------------------------------------------------
 */

/* standard includes */
#include <stdint.h>
#include "reg_ctboard.h"
#include "hal_ct_buttons.h"

/// STUDENTS: To be programmed
#define EXT_MEM (*((volatile uint8_t *) (0x64000000)))

/// END: To be programmed

int main(void) {
    /// STUDENTS: To be programmed
	
	  uint32_t base_addr = 0x64000000;
    uint32_t addr = 0;
    
    uint16_t base_offset = 0x0400;
    uint16_t top_offset = 0x04FF;
    uint8_t offset = 0;
    uint8_t value = 0;
    uint8_t error = 0;
    
    while(1) {  
        addr = base_addr + base_offset + offset;
        value = (*((volatile uint8_t *)(addr)));
        
        if (base_offset + offset >= top_offset) {
            //finished
        } else {
            if (value != offset){
                error++;
                CT_LED->BYTE.LED7_0 = value;
                CT_LED->BYTE.LED23_16 = offset;
                while (!hal_ct_button_is_pressed(HAL_CT_BUTTON_T0));
            }
            CT_SEG7->BIN.BYTE.DS1_0 = error;
            offset++;
        }
    }
    /// END: To be programmed
}
