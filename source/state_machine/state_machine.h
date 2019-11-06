/**
  * File Name 		- state_machine.c
  * Description 	- header file for state_machine.c
  * Author			- Atharva Nandanwar
  * Tools			- GNU C Compiler / ARM Compiler Toolchain
  * Leveraged Code 	-
  * URL				-
  */

#ifndef STATE_MACHINE_STATE_MACHINE_H_
#define STATE_MACHINE_STATE_MACHINE_H_
#include <stdint.h>
#include <stdlib.h>
#include "logger.h"
#include "errno.h"
#include "tmp102.h"
#include "i2c.h"
#include "led_control.h"

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

typedef enum {
	State_Driven = 0x01,
	Table_Driven = 0x02,
} state_machine_type_t;

typedef struct {
	state_t state;
	event_t event;
	state_machine_type_t type;
} state_machine_t;

typedef struct {
	int16_t temperature;
	int16_t average_temperature;
	uint8_t timeout_count;
	uint8_t counter;
	uint8_t timeout_started;
	uint8_t disconnect;
	uint8_t alert;
	uint8_t state_machine_id;
	uint8_t print_flag;
} system_state_t;

typedef struct {
	state_t state_id;
	void (*event_action[5])(state_machine_t* sm);
}state_struct;

void End_Program(void);
state_machine_t* State_Machine_Init(state_machine_type_t);
void State_Machine_End(state_machine_t* sm);
void Set_Event(state_machine_t* sm, event_t event);
void Set_State(state_machine_t* sm, state_t state);
void Event_Handler(state_machine_t* sm, system_state_t* system);
void Print_Message(const char* function_name, error_t error);

#endif /* STATE_MACHINE_STATE_MACHINE_H_ */
