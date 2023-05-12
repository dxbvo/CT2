/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 * --
 * -- Project     : CT Lab on bus cycles (logic analyzer)
 * -- Description : Read dip switches and write to LED.
 * -- 
 * -- $Id: main.c 3012 2016-02-16 13:33:26Z feur $
 * ------------------------------------------------------------------------- */
 
#include <stdint.h>


/* ----------------------------------------------------------------------------
 * Configure access mode
 * uncoment a single line below to set the desired access mode
 * ------------------------------------------------------------------------- */

//#define MODE_8BIT_EVEN
//#define MODE_8BIT_ODD
#define MODE_16BIT
//#define MODE_32BIT


/* ----------------------------------------------------------------------------
 * define macros
 * ------------------------------------------------------------------------- */

/* addresses */
#define ADDR_LED        ((uint32_t) 0x60000100)
#define ADDR_LED_15_8   ((uint32_t) 0x60000101)

#define ADDR_DIPSW      ((uint32_t) 0x60000200)
#define ADDR_DIPSW_15_8 ((uint32_t) 0x60000201)

/* macros to acccess LEDs and DIP-switches */
#if defined (MODE_32BIT)
    #define CT_LED          (*((volatile uint32_t *) ADDR_LED))
    #define CT_DIPSW        (*((volatile uint32_t *) ADDR_DIPSW))
        
#elif defined (MODE_16BIT)
    #define CT_LED          (*((volatile uint16_t *) ADDR_LED))
    #define CT_DIPSW        (*((volatile uint16_t *) ADDR_DIPSW))
    
#elif defined (MODE_8BIT_EVEN)
    #define CT_LED          (*((volatile uint8_t *) ADDR_LED))
    #define CT_DIPSW        (*((volatile uint8_t *) ADDR_DIPSW))
    
#elif defined (MODE_8BIT_ODD)
    #define CT_LED          (*((volatile uint8_t *) ADDR_LED_15_8))
    #define CT_DIPSW        (*((volatile uint8_t *) ADDR_DIPSW_15_8))

#else
    // halfword access to odd address
    /// STUDENTS: To be programmed




    /// END: To be programmed
#endif    

/* ----------------------------------------------------------------------------
 * Main
 * ------------------------------------------------------------------------- */

int main(void)
{        
    while(1) {
        CT_LED = CT_DIPSW;
    }
}
