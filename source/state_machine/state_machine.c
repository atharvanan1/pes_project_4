/*
 *
 * state_machine.c
 *
 *  Created on: Oct 31, 2019
 *      Author: azzentys
 */

#include "state_machine.h"

state_machine_t sm1 = {State_Machine_1, {Temperature_Reading, Average_Wait, Temperature_Alert, Disconnected}};
state_machine_t sm2 = {State_Machine_2, {Temperature_Reading, Average_Wait, Temperature_Alert, Disconnected}};
state_t temp_reading = {Temperature_Reading, {Start, Read_Complete, Alert, Timeout_Complete, Disconnect}};
state_t average_wait = {Average_Wait, {Read_Complete, Timeout_Complete, Disconnect}};
state_t temp_alert  = {Temperature_Alert, {Alert, Disconnect}};
state_t disconnected = {Temperature_Reading, {Disconnect}};
event_t start = {Start};
event_t read_complete = {Read_Complete};
event_t alert = {Alert};
event_t disconnect = {Disconnect};
event_t timeout = {Timeout_Complete};

void Print_Message(error_t error)
{
	errno = error;
	logger.Log_Write("%s\n\r", Get_Error_Message(errno));
}

void trigger(event_t* event)
{
	switch(event->id)
	{
	case Start:
		transition(&temp_reading);
		current_event = &read_complete;
		uint16_t temperature = I2C_Read_From_Slave();
		logger.Log_Write("Temperature is %d\n\r", get_temperature(dummy));
		Print_Message(Reading_Temperature);
		break;
	case Read_Complete:
		transition(&average_wait);
		Print_Message(Reading_Temperature_Complete);
		break;
	case Alert:
		transition(&temp_alert);
		Print_Message(Alert_LOW_Temperature);
	case Disconnect:
		transition(&disconnected);
		Print_Message(Device_Disconnected);
		break;
	case Timeout_Complete:
		transition(&temp_reading);
		Print_Message(Timeout);
		Print_Message(Reading_Temperature);
		break;
	default:
		Print_Message(Unhandled_Exception);
		break;
	}
}

void transition(state_t* state)
{
	current_state = state;
}

uint8_t valid_event(event_t* event, state_t state)
{
	uint8_t length;

	if(state.id == Temperature_Reading)
	{
		length = 5;
	}
	else if(state.id == Average_Wait)
	{
		length = 3;
	}
	else if(state.id == Temperature_Alert)
	{
		length = 2;
	}
	else if(state.id == Disconnected)
	{
		length = 1;
	}

	for (uint8_t i = 0; i < length; i++)
	{
		if(event->id == state.valid_events[i])
		{
			return 1;
		}
	}
	return 0;
}

void state_machine_init(state_machine_t* state_machine)
{
	current_state_machine = state_machine;
	current_event = &start;
	current_state = &temp_reading;
}

