/*
 *
 * state_machine.c
 *
 *  Created on: Oct 31, 2019
 *      Author: azzentys
 */

#include "state_machine.h"

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
state_machine_t* State_Machine_Init(void)
{
	state_machine_t* sm = (state_machine_t *) malloc(sizeof(state_machine_t));

	if(sm == NULL)
	{
		logger.Log_Write("Failure to initialize State Machine\n\r");
		return NULL;
	}

	sm->state = sTemperature_Reading;
	sm->event = eStart;

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
			Set_Event(sm, eAlert);
		}
		if(system->disconnect == 1)
		{
			Set_Event(sm, eDisconnect);
		}
		if(sm->event == eStart)
		{
			Print_Message(Reading_Temperature);
//			I2C_Init();
//			int16_t temp = I2C_Read_Temperature();
//			system->temperature = Get_Temperature(temp);
			logger.Log_Write("Temperature is %d\n\r", system->temperature);
			Print_Message(Reading_Temperature_Complete);
			Set_State(sm, sAverage_Wait);
			Set_Event(sm, eRead_Complete);
		}
		if(sm->event == eRead_Complete)
		{
			;
		}
		if(sm->event == eAlert)
		{
			Set_State(sm, sTemperature_Alert);
		}
		if(sm->event == eTimeout_Complete)
		{
			;
		}
		if(sm->event == eDisconnect)
		{
			Set_State(sm, sDisconnected);
		}
		break;
	case sAverage_Wait:
		if(system->disconnect == 1)
		{
			Set_Event(sm, eDisconnect);
		}
		if(sm->event == eRead_Complete)
		{
			if(system->timeout_started != 1)
			{
				Print_Message(Waiting);
				system->counter = 0;
				system->timeout_started = 1;
			}
			else
			{
				if(system->counter == 150)
				{
					Set_Event(sm, eTimeout_Complete);
					Print_Message(Timeout);
					system->timeout_started = 0;
				}
			}
		}
		if(sm->event == eAlert)
		{
			;
		}
		if(sm->event == eTimeout_Complete)
		{
			system->timeout_count++;
			if(system->timeout_count)
			{
				//End this state machine, find out how
			}
			Set_State(sm, sTemperature_Reading);
			Set_Event(sm, eStart);
		}
		if(sm->event == eDisconnect)
		{
			Set_State(sm, sDisconnected);
		}
		break;
	case sTemperature_Alert:
		if(system->disconnect == 1)
		{
			Set_Event(sm, eDisconnect);
		}
		if(sm->event == eAlert)
		{
			Print_Message(Alert_LOW_Temperature);
			Set_State(sm, sAverage_Wait);
			Set_Event(sm, eRead_Complete);
		}
		if(sm->event == eDisconnect)
		{
			Set_State(sm, sDisconnected);
		}
		break;
	case sDisconnected:
		if(sm->event == eDisconnect)
		{
			Print_Message(Device_Disconnected);
			End_Program();
		}
		break;
	}
}
