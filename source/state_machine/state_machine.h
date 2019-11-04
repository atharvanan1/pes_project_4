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
#include "tmp102.h"
#include "i2c.h"

typedef enum {
	eStart = 0x00,
	eRead_Complete = 0x01,
	eAlert = 0x02,
	eTimeout_Complete = 0x04,
	eDisconnect = 0x08,
} event_t;

typedef enum {
	sTemperature_Reading = 0x01,
	sAverage_Wait = 0x02,
	sTemperature_Alert = 0x04,
	sDisconnected = 0x08,
} state_t;

typedef struct {
	state_t state;
	event_t event;
} state_machine_t;

typedef struct {
	int16_t temperature;
	int16_t average_temperature;
	uint8_t timeout_count;
	uint32_t counter;
	uint8_t timeout_started;
	uint8_t disconnect;
	uint8_t alert;
} system_state_t;

void End_Program(void);
state_machine_t* State_Machine_Init(void);
void State_Machine_End(state_machine_t* sm);
void Set_Event(state_machine_t* sm, event_t event);
void Set_State(state_machine_t* sm, state_t state);
void Event_Handler(state_machine_t* sm, system_state_t* system);
void Print_Message(error_t error);

#endif /* STATE_MACHINE_STATE_MACHINE_H_ */
