/*
 * state_machine.h
 *
 *  Created on: Oct 31, 2019
 *      Author: azzentys
 */

#ifndef STATE_MACHINE_STATE_MACHINE_H_
#define STATE_MACHINE_STATE_MACHINE_H_
#include <stdint.h>
#include <stdlib.h>
#include "logger.h"
#include "errno.h"

typedef enum {
	Start = 0x00,
	Read_Complete = 0x01,
	Alert = 0x02,
	Timeout_Complete = 0x04,
	Disconnect = 0x08,
} event_id;

typedef enum {
	Temperature_Reading = 0x01,
	Average_Wait = 0x02,
	Temperature_Alert = 0x04,
	Disconnected = 0x08,
} state_id;

typedef enum {
	State_Machine_1 = 0x01,
	State_Machine_2 = 0x02,
} state_machine_id;

typedef struct {
	state_machine_id machine;
	state_id states[];
} state_machine_t;

typedef struct {
	event_id id;
} event_t;

typedef struct {
	state_id id;
	event_id valid_events[];
} state_t;

extern state_t* current_state;
extern event_t* current_event;
extern state_machine_t* current_state_machine;

extern state_machine_t sm1;
extern state_machine_t sm2;

extern state_t temp_reading;
extern state_t average_wait;
extern state_t temp_alert;
extern state_t disconnected;

extern event_t start;
extern event_t read_complete;
extern event_t alert;
extern event_t disconnect;
extern event_t timeout;

void trigger(event_t* event);
void transition(state_t* state);
uint8_t valid_event(event_t* event, state_t state);
void state_machine_init(state_machine_t* state_machine);
void Print_Message(error_t error);

#endif /* STATE_MACHINE_STATE_MACHINE_H_ */
