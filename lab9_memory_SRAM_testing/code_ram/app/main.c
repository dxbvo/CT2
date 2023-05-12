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

#include <stdint.h>
#include "hal_rcc.h"
#include "hal_fmc.h"
#include "hal_ct_lcd.h"
#include "reg_ctboard.h"
#include "hal_ct_buttons.h"
#include "hal_ct_seg7.h"

#define NR_OF_DATA_LINES           8
#define NR_OF_ADDRESS_LINES       11
#define CHECKER_BOARD           0xAA
#define INVERSE_CHECKER_BOARD   0x55
#define WALKING_ONES_ADDRESS    0x7FF

/* Set-up the macros (#defines) for your test */
/// STUDENTS: To be programmed

#define SRAM_BASE_ADDR  0x64000000

void show_device_error(uint16_t adr);

void show_device_error(uint16_t adr){
	CT_SEG7->BIN.HWORD = adr;
	while (!hal_ct_button_is_pressed(HAL_CT_BUTTON_T0)) {}
}



/// END: To be programmed

int main(void) {
    hal_fmc_sram_init_t init;
    hal_fmc_sram_timing_t timing;
    
    /* add your required automatic (local) variables here */ 
    /// STUDENTS: To be programmed
		uint8_t *addr_ram =  (( uint8_t *)(0x64000000));
    uint8_t *flex_addr_ram = (( uint8_t *)(0x64000000));
    uint16_t test_addr_offset = 0;
    
    uint8_t test_value = 0x1;
    uint8_t read_value = 0;

    uint8_t loop = 0;
	
		uint8_t *base_addr = (uint8_t *) SRAM_BASE_ADDR;


	
//	for (int i = 1; i < NR_OF_DATA_LINES; i++) {
//		
//		if (position & 1) {
//			position = position << 1;
//		} 
//		else {
//			CT_LED->BYTE.LED7_0 = 0;
//		}
//	}

    /// END: To be programmed

    init.address_mux = DISABLE;                             // setup peripheral
    init.type = HAL_FMC_TYPE_SRAM;
    init.width = HAL_FMC_WIDTH_8B;
    init.write_enable = ENABLE;

    timing.address_setup = 0xFF;                            // all in HCLK
                                                            // cycles
    timing.address_hold = 0xFF;
    timing.data_setup = 0xFF;

    hal_fmc_init_sram(HAL_FMC_SRAM_BANK2, init, timing);    // init external bus
                                                            // bank 2 (NE2)
                                                            // async
    /* Data Bus Test - Walking ONES test */
    /// STUDENTS: To be programmed
		
		test_value = 0x1;
    read_value = 0;
    loop = 0;
    CT_LED->BYTE.LED7_0 = 0;
    
    for (loop = 0; loop<NR_OF_DATA_LINES; loop++) {
        *addr_ram = test_value;
        read_value = *addr_ram;
        
        CT_LED->BYTE.LED7_0 |= read_value ^ test_value;
        
        test_value = test_value<<1;
    }

    /// END: To be programmed
    
    /* Address Bus Test 
     * (1)  Write default values
     *
     *      Write the memory at all the power of 2 addresses (including 0x0000)
     *      to the default value of CHECKER_BOARD
     *
     * (2)  Perform tests
     *
     *      Select one power of 2 addresses after the other as test_address 
     *      (starting from the highest all the way down to 0x0000) --> 
     *          - Write the memory at test_address to INVERSE_CHECKER_BOARD
     *          - For all the power of 2 addresses including 0x0000
     *              o Read the memory content
     *              o Verify that the read value is either
     *                  (a) equal to CHECKER_BOARD in case a different address 
     *                      than test_addressed has been read or
     *                  (b) equal to INVERSE_CHECKER_BOARD in case the address
     *                      at test_address has been read
     *              o Errors found shall be indicated on LED31--16
     */
    
    /// STUDENTS: To be programmed
		
		test_addr_offset = 0x0400;
    test_value = 0xAA;

    //init value area
    while (test_addr_offset) {
        flex_addr_ram = ((uint8_t *)(addr_ram + test_addr_offset));
        
        *flex_addr_ram = test_value;
        test_addr_offset >>= 1;
    }
    //for base address and because i'm lazy
    flex_addr_ram = ((uint8_t *)(addr_ram));
    *flex_addr_ram = test_value;
    
    //actual test
    test_addr_offset = 0x1;
    test_value = 0x55;
    
    //for 0 because i'm lazy
    flex_addr_ram = ((uint8_t *)(addr_ram));
        
        read_value = *flex_addr_ram;
        *flex_addr_ram = test_value;
        
        if(read_value != 0xAA) { 
            CT_LED->HWORD.LED31_16 |= test_addr_offset;
        }
    //actual testing v2
    while (test_addr_offset <= 0x0400) {
        flex_addr_ram = ((uint8_t *)(addr_ram + test_addr_offset));
        
        read_value = *flex_addr_ram;
        *flex_addr_ram = test_value;
        
        if(read_value != 0xAA) { 
            CT_LED->HWORD.LED31_16 |= test_addr_offset;
        }
        
        test_addr_offset <<= 1;
    }
		
     /// END: To be programmed
    
    /* Device Test 
     * (1) Fill the whole memory with known increment pattern.
     *          Address     Data
     *          0x000       0x01
     *          0x001       0x02
     *          .....       ....
     *          0x0FE       0xFF
     *          0x0FF       0x00
     *          0x100       0x01
     *          .....       ....
     *
     * (2) First test: Read back each location and check pattern.
     *     In case of error, write address with wrong data to 7-segment and
     *     wait for press on button T0.
     *     Bitwise invert  the pattern in each location for the second test
     *
     * (3) Second test: Read back each location and check for new pattern.
     *     In case of error, write address with wrong data to 7-segment and
     *     wait for press on button T0.
     */
    /// STUDENTS: To be programmed
		for(uint16_t adr = 0;adr < 1<<NR_OF_ADDRESS_LINES; adr++) {
		
			*(base_addr + adr) = (adr + 1) % 0x100;
		}
		
		CT_LED->BYTE.LED15_8 = 0x3F;
		
		for(uint16_t adr = 0;adr < 1<<NR_OF_ADDRESS_LINES; adr++) {
			if (*(base_addr + adr) != (adr + 1) % 0x100){
				show_device_error(adr);
			}
			*(base_addr + adr) = ~((adr + 1) % 0x100);
		}
		
		for(uint16_t adr = 0;adr < 1<<NR_OF_ADDRESS_LINES; adr++) {
			if (*(base_addr + adr) != ~((adr + 1) % 0x100)){
				show_device_error(adr);
			}		
		}

    /// END: To be programmed
    
    // Write 'End'
    CT_SEG7->RAW.BYTE.DS0 = 0xA1;
    CT_SEG7->RAW.BYTE.DS1 = 0xAB;
    CT_SEG7->RAW.BYTE.DS2 = 0x86;
    CT_SEG7->RAW.BYTE.DS3 = 0xFF;
    
    while(1){
    }

}
