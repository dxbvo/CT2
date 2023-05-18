/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 * --
 * -- Description:  Interface of module action_handler.
 * --
 * -- Sets or clears pins on outport.
 * --
 * -- $Id: action_handler.h 5526 2022-01-18 07:26:31Z ruan $
 * ------------------------------------------------------------------------- */

/* re-definition guard */
#ifndef _ACTION_HANDLER_H
#define _ACTION_HANDLER_H


/* -- Type definitions
 * ------------------------------------------------------------------------- */

typedef enum {
    MOTOR_OFF,
    MOTOR_DOWN,
    MOTOR_UP
} motor_cmd_t;

typedef enum {
    DOOR_OPEN,
    DOOR_CLOSE,
    DOOR_LOCK,
    DOOR_UNLOCK
} door_cmd_t;

typedef enum {
    SIGNAL_OFF,
    SIGNAL_ON
} signal_cmd_t;

typedef enum {
    NORMAL,
    WARNING,
    ERROR
} exception_t;




/* -- Public function declarations
 * ------------------------------------------------------------------------- */

/*
 * initializes the action_handler module.
 */
void action_handler_init(void);


/*
 * control the elevator motor, door and upper floor signal.
 * these functions generate errors in the following cases:
 * - trying to switch on the motor while the doors are unlocked
 * - trying to unlock the doors while the motor is on
 */
void ah_motor(motor_cmd_t motor_cmd);
void ah_door(door_cmd_t door_cmd);
void ah_signal(signal_cmd_t signal_cmd);


/*
 * shows the given text (max 20 chars) on the upper line of the lcd.
 */
void ah_show_state(char text[]);


/*
 * shows the given text (max 20 chars) on the lower line of the lcd.
 * also controls the lcd's background color.
 */
void ah_show_exception(exception_t exception, char text[]);


#endif
