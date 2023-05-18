/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 * --
 * -- Description:  Implementation of module state_machine.
 * --
 * -- $Id: state_machine.c 5526 2022-01-18 07:26:31Z ruan $
 * ------------------------------------------------------------------------- */

/* standard includes */
#include <stdint.h>

/* user includes */
#include "state_machine.h"
#include "action_handler.h"
#include "timer.h"

/* -- Macros ----------------------------------------------------------------- */

#define SAFETY_DURATION      150u       // 150 * 10ms = 1.5s
#define SIGNAL_DURATION      100u       // 100 * 10ms = 1s

#define TEXT_F0_OPENED       "F0_OPENED"
#define TEXT_F0_CLOSED       "F0_CLOSED"
#define TEXT_F1_OPENED       "F1_OPENED"
#define TEXT_F1_CLOSED       "F1_CLOSED"
#define TEXT_MOVING_UP       "MOVING_UP"
#define TEXT_MOVING_DOWN     "MOVING_DOWN"

/* -- Type definitions ------------------------------------------------------- */

typedef enum {
    F0_OPENED,
    F0_CLOSED,
    F1_OPENED,
    F1_CLOSED,
    MOVING_UP,
    MOVING_DOWN,
    F0_DELAYED_START,
    F1_DELAYED_START,
    F0_WEIGHT_OK,
    F0_WEIGHT_NOK,
} state_t;

/* -- Module-wide variables & constants -------------------------------------- */

static state_t state = F0_CLOSED;

/* -- Function prototypes ---------------------------------------------------- */

static void transitionToState(state_t newState);

/* -- Public function definitions -------------------------------------------- */

void fsm_init(void)
{
    action_handler_init();
    ah_show_exception(NORMAL, "");

    /* Go to initial state & perform initial actions */
    transitionToState(F0_CLOSED);
}

void fsm_handle_event(event_t event)
{
    switch (state) {
        case F0_OPENED:
            if (event == EV_DOOR0_CLOSE_REQ) {
                ah_door(DOOR_CLOSE);
                transitionToState(F0_CLOSED);
            }
            break;

        case F0_CLOSED:
            if (event == EV_DOOR0_OPEN_REQ) {
                timer_stop();
                ah_door(DOOR_OPEN);
								ah_show_state("F0_WEIGHT_OK");
                transitionToState(F0_WEIGHT_OK);
                eh_weight_control(WCTL_ENABLE, 50);
            } else if (event == EV_BUTTON_F1) {
                ah_door(DOOR_LOCK);
								ah_show_state("F0_DELAYED_START");
                timer_start(SAFETY_DURATION);
                transitionToState(F0_DELAYED_START);
            }
            break;

        case F1_OPENED:
            if (event == EV_DOOR1_CLOSE_REQ) {
                ah_door(DOOR_CLOSE);
                transitionToState(F1_CLOSED);
            }
            break;

        case F1_CLOSED:
            if (event == EV_DOOR1_OPEN_REQ) {
                ah_door(DOOR_OPEN);
                transitionToState(F1_OPENED);
            } else if (event == EV_BUTTON_F0) {
                ah_door(DOOR_LOCK);
								ah_show_state("F1_DELAYED_START");
                timer_start(SAFETY_DURATION);
                transitionToState(F1_DELAYED_START);
            }
            break;

        case MOVING_UP:
            if (event == EV_F1_REACHED) {
                ah_motor(MOTOR_OFF);
                ah_door(DOOR_UNLOCK);
                transitionToState(F1_CLOSED);
            }
            break;

        case MOVING_DOWN:
            if (event == EV_F0_REACHED) {
                ah_motor(MOTOR_OFF);
                ah_door(DOOR_UNLOCK);
                ah_signal(SIGNAL_OFF);
                transitionToState(F0_CLOSED);
            }
            break;

        case F0_DELAYED_START:
            if (event == EV_TIMEOUT) {
                timer_stop();
                ah_motor(MOTOR_UP);
                transitionToState(MOVING_UP);
            }
            break;

        case F1_DELAYED_START:
            if (event == EV_TIMEOUT) {
                timer_stop();
                ah_motor(MOTOR_DOWN);
                transitionToState(MOVING_DOWN);
            }
            break;

        case F0_WEIGHT_OK:
            if (event == EV_DOOR0_CLOSE_REQ) {
                ah_door(DOOR_CLOSE);
                eh_weight_control(WCTL_DISABLE, 0);
                transitionToState(F0_CLOSED);
            } else if (event == EV_WEIGHT_TOO_HIGH) {
								ah_show_state("F0_WEIGHT_NOT_OK");
                ah_show_exception(WARNING, "Weight exceeded!");
                transitionToState(F0_WEIGHT_NOK);
            }
            break;

        case F0_WEIGHT_NOK:
            if (event == EV_WEIGHT_OK) {
								ah_show_state("F0_WEIGHT_OK");
                ah_show_exception(NORMAL, "");
                transitionToState(F0_WEIGHT_OK);
            }
            break;

        default:
            // Handle unknown state or event
            break;
    }
}

/* -- Private function definitions ------------------------------------------- */

static void transitionToState(state_t newState)
{
    state = newState;

    switch (newState) {
        case F0_OPENED:
            ah_show_state(TEXT_F0_OPENED);
            break;

        case F0_CLOSED:
            ah_show_state(TEXT_F0_CLOSED);
            break;

        case F1_OPENED:
            ah_show_state(TEXT_F1_OPENED);
            break;

        case F1_CLOSED:
            ah_show_state(TEXT_F1_CLOSED);
            break;

        case MOVING_UP:
            ah_show_state(TEXT_MOVING_UP);
            break;

        case MOVING_DOWN:
            ah_show_state(TEXT_MOVING_DOWN);
            break;

        default:
            // Handle unknown state
            break;
    }
}

