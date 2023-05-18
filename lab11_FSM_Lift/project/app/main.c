/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zuercher Hochschule Winterthur             -
 * --  _| |_| | | | |____ ____) |  (University of Applied Sciences)           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 * --
 * -- Project     : CT2 lab - Elevator FSM
 * -- Description : Main program.
 * --
 * -- $Id: main.c 5526 2022-01-18 07:26:31Z ruan $
 * ------------------------------------------------------------------------- */

/* standard includes */
#include <stdint.h>

/* user includes */
#include "event_handler.h"
#include "state_machine.h"
#include "timer.h"


/* -- M A I N
 * ------------------------------------------------------------------------- */

int main(void)
{
    /// STUDENTS: To be programmed
		fsm_init();
		eh_init();
		timer_init();
	
		while(1){
			event_t doorEvent = eh_get_event();
			fsm_handle_event(doorEvent);
		}
	
    /// END: To be programmed
}

