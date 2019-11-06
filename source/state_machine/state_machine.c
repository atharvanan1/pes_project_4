/*
 *
 * state_machine.c
 *
 *  Created on: Oct 31, 2019
 *      Author: azzentys
 */

#include "state_machine.h"

state_struct LookUpTable[] = {{sTemperature_Reading, {fStart, fRead_Complete, fTimeoutComplete, fAlert, fDisconnect}},
							  {sAverage_Wait, {NULL, fRead_Complete, fTimeoutComplete, NULL, fDisconnect}},
							  {sTemperature_Alert, {NULL, fRead_Complete, NULL, fAlert, fDisconnect}},
							  {sDisconnected, {NULL, NULL, NULL, NULL, fDisconnect}},
							  };

#define S_TEMPERATURE_READING	LookUpTable[0]
#define S_AVERAGE_WAIT			LookUpTable[1]
#define S_TEMPERATURE_ALERT		LookUpTable[2]
#define S_DISCONNECTED			LookUpTable[3]

#define F_START					event_action[0]
#define F_READ_COMPLETE			event_action[1]
#define F_TIMEOUT_COMPLETE		event_action[2]
#define F_ALERT					event_action[3]
#define F_DISCONNECT			event_action[4]

void average_temperature(system_state_t* system)
{
	system->average_temperature = (system->average_temperature + system->temperature) / 2;
}
/*
 * Function -
 */
void Print_Message(error_t error)
{
	errno = error;
	logger.Log_Write("%s\n\r", Get_Error_Message(errno));
}

/*
 * Function -
 */
void End_Program(void)
{
	logger.Log_Write("Ending Program");
	while(1);
}

/*
 * Function -
 */
state_machine_t* State_Machine_Init(state_machine_type_t type)
{
	state_machine_t* sm = (state_machine_t *) malloc(sizeof(state_machine_t));

	if(sm == NULL)
	{
		logger.Log_Write("Failure to initialize State Machine\n\r");
		return NULL;
	}

	sm->state = sTemperature_Reading;
	sm->event = eStart;
	sm->type = type;

	return sm;
}

/*
 * Function -
 */
void State_Machine_End(state_machine_t* sm)
{
	if(sm == NULL)
		return;

	free(sm);
	sm = NULL;
}

/*
 * Function -
 */
void Set_Event(state_machine_t* sm, event_t event)
{
	sm->event = event;
}

/*
 * Function -
 */
void Set_State(state_machine_t* sm, state_t state)
{
	sm->state = state;
}

/*
 * Function -
 */
void Event_Handler(state_machine_t* sm, system_state_t* system)
{
	switch(sm->state)
	{
	case sTemperature_Reading:
		if(system->alert == 1)
		{
			if(sm->type == State_Driven)
				Set_Event(sm, eAlert);
			else if (sm->type == Table_Driven)
				S_TEMPERATURE_READING.F_ALERT(sm);
			system->alert = 0;
		}
		else if(system->disconnect == 1)
		{
			if(sm->type == State_Driven)
				Set_Event(sm, eDisconnect);
			else if (sm->type == Table_Driven)
				S_TEMPERATURE_READING.F_DISCONNECT(sm);
		}
		else if(sm->event == eStart)
		{
			Print_Message(Reading_Temperature);
			I2C_Init();
			int16_t temp = I2C_Read(TMP102.tmp_reg_address);
			system->temperature = Get_Temperature(temp);
			logger.Log_Write("Temperature is %d\n\r", system->temperature);
			Print_Message(Reading_Temperature_Complete);
			Set_State(sm, sAverage_Wait);
			if(sm->type == State_Driven)
				Set_Event(sm, eRead_Complete);
			else if (sm->type == Table_Driven)
				S_TEMPERATURE_READING.F_READ_COMPLETE(sm);
		}
		else if(sm->event == eRead_Complete)
		{
			;
		}
		else if(sm->event == eAlert)
		{
			Set_State(sm, sTemperature_Alert);
		}
		else if(sm->event == eTimeout_Complete)
		{
			;
		}
		else if(sm->event == eDisconnect)
		{
			Set_State(sm, sDisconnected);
		}
		break;
	case sAverage_Wait:
		if(system->disconnect == 1)
		{
			if(sm->type == State_Driven)
				Set_Event(sm, eDisconnect);
			else if (sm->type == Table_Driven)
				S_AVERAGE_WAIT.F_DISCONNECT(sm);
		}
		else if(sm->event == eRead_Complete)
		{
			if(system->timeout_started != 1)
			{
				average_temperature(system);
				Print_Message(Waiting);
				system->counter = 0;
				system->timeout_started = 1;
				SysTick_Config(48000000L/10L);
			}
			else
			{
				if(system->counter >= 150)
				{
					if(sm->type == State_Driven)
						Set_Event(sm, eTimeout_Complete);
					else if (sm->type == Table_Driven)
						S_AVERAGE_WAIT.F_TIMEOUT_COMPLETE(sm);
					Print_Message(Timeout);
					system->timeout_started = 0;
					system->counter = 0;
					SysTick->CTRL = 0;
				}
			}
		}
		else if(sm->event == eAlert)
		{
			;
		}
		else if(sm->event == eTimeout_Complete)
		{
			system->timeout_count++;
			if(system->timeout_count == 4)
			{
				system->state_machine_id = 2;
				system->timeout_count = 0;

			}
			Set_State(sm, sTemperature_Reading);
			if(sm->type == State_Driven)
				Set_Event(sm, eStart);
			else if (sm->type == Table_Driven)
				S_AVERAGE_WAIT.F_START(sm);
		}
		else if(sm->event == eDisconnect)
		{
			Set_State(sm, sDisconnected);
		}
		break;
	case sTemperature_Alert:
		if(system->disconnect == 1)
		{
			if(sm->type == State_Driven)
				Set_Event(sm, eDisconnect);
			else if (sm->type == Table_Driven)
				S_AVERAGE_WAIT.F_DISCONNECT(sm);
		}
		else if(sm->event == eAlert)
		{
			Print_Message(Alert_LOW_Temperature);
			Set_State(sm, sAverage_Wait);
			if(sm->type == State_Driven)
				Set_Event(sm, eRead_Complete);
			else if (sm->type == Table_Driven)
				S_AVERAGE_WAIT.F_READ_COMPLETE(sm);
		}
		else if(sm->event == eDisconnect)
		{
			Set_State(sm, sDisconnected);
		}
		break;
	case sDisconnected:
		if(sm->event == eDisconnect)
		{
			Print_Message(Device_Disconnected);
			exit(1);
		}
		break;
	}
}

//-------------------------------------------------------------------
void fStart(state_machine_t* sm)
{
	Set_Event(sm, eStart);
}

void fRead_Complete(state_machine_t* sm)
{
	Set_Event(sm, eRead_Complete);
}

void fTimeoutComplete(state_machine_t* sm)
{
	Set_Event(sm, eTimeout_Complete);
}

void fAlert(state_machine_t* sm)
{
	Set_Event(sm, eAlert);
}

void fDisconnect(state_machine_t* sm)
{
	Set_Event(sm, eDisconnect);
}
