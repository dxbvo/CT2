/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 * --
 * -- Description:  Interface of module event_handler.
 * --
 * -- Generates events based on rising or falling edges on inport.
 * --
 * -- $Id: event_handler.h 5526 2022-01-18 07:26:31Z ruan $
 * ------------------------------------------------------------------------- */

/* re-definition guard */
#ifndef _EVENT_HANDLER_H
#define _EVENT_HANDLER_H

/* standard includes */
#include <stdint.h>
#include <stdbool.h>


/* -- Type definitions
 * ------------------------------------------------------------------------- */

typedef enum {
    EV_NO_EVENT,
    EV_TIMEOUT,
    EV_DOOR0_CLOSE_REQ,
    EV_DOOR0_OPEN_REQ,
    EV_DOOR1_CLOSE_REQ,
    EV_DOOR1_OPEN_REQ,
    EV_BUTTON_F0,
    EV_BUTTON_F1,
    EV_F0_REACHED,
    EV_F1_REACHED,
    EV_WEIGHT_OK,
    EV_WEIGHT_TOO_HIGH
} event_t;


typedef enum {
    WCTL_ENABLE,
    WCTL_DISABLE
} weight_control_t;



/* -- Public variable declarations
 * ------------------------------------------------------------------------- */

/*
 * Since we're simulating the movement of the elevator, there are no real
 * sensors. These flags are set by the animation part of action_handler.c 
 * and must be reset by the eh_get_event function.
 */
extern bool flag_F0_Reached;
extern bool flag_F1_Reached;


/* -- Public function declarations
 * ------------------------------------------------------------------------- */

/*
 * Initialize event handler;
 * - setup ADC3 for continuous 6bit readings of the CT-board potentiometer
 */
void eh_init(void);


/*
 * Check all event sources and return resulting events
 */
event_t eh_get_event(void);


/*
 * Task 4.3:
 * This function is used to enable/disable the weight control functionality.
 * When enabled, the EV_WEIGHT_OK and EV_WEIGHT_TOO_HIGH events are generated
 * based on the position of the CT-board potentiometer.
 * Parameters:
 *      wctl_cmd        
 *              WCTL_DISABLE    Switch off the weight control functionality.
 *                              - no more related events are generated
 *                              - 7seg display is cleared
 *              WCTL_ENABLE     Turn on the weight control:
 *                              - the CT-board potentiometer is read peridocally
 *                              - the resulting value (0..63) is shown on the 
 *                                7seg display..
 *                              - ..and compared to the given weight limit 
 *                                  (see below)
 *                              - the EV_WEIGHT_OK / EV_WEIGHT_TOO_HIGH events 
 *                                are generated continuously, as long as the 
 *                                weight control is enabled.
 *      weight_limit
 *              Up to (and including) this limit the weight is ok.
 *              If the value read from the potentiometer (0..63) exceeds this 
 *              limit, EV_WEIGHT_TOO_HIGH is generated.
 */
void eh_weight_control(weight_control_t wctl_cmd, uint16_t weight_limit);

#endif
