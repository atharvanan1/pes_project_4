/**
  * File Name 		- state_machine.c
  * Description 	- contains state machine subroutines
  * Author			- Atharva Nandanwar
  * Tools			- GNU C Compiler / ARM Compiler Toolchain
  * Leveraged Code 	- https://github.com/EduMacedo99/FEUP-LCOM
  * URL				-
  */

#include "state_machine.h"

// Prototype to functions for Table-Driven State Machine
void fStart(state_machine_t* sm);
void fRead_Complete(state_machine_t* sm);
void fTimeoutComplete(state_machine_t* sm);
void fAlert(state_machine_t* sm);
void fDisconnect(state_machine_t* sm);

// Look Up Table for Table Driven State Machine
state_struct LookUpTable[] = {{sTemperature_Reading, {fStart, fRead_Complete, fTimeoutComplete, fAlert, fDisconnect}},
							  {sAverage_Wait, {fStart, fRead_Complete, fTimeoutComplete, NULL, fDisconnect}},
							  {sTemperature_Alert, {NULL, fRead_Complete, NULL, fAlert, fDisconnect}},
							  {sDisconnected, {NULL, NULL, NULL, NULL, fDisconnect}},
							  };

// Macros for Table Driven State Machine Execution
#define S_TEMPERATURE_READING	LookUpTable[0]
#define S_AVERAGE_WAIT			LookUpTable[1]
#define S_TEMPERATURE_ALERT		LookUpTable[2]
#define S_DISCONNECTED			LookUpTable[3]

#define F_START					event_action[0]
#define F_READ_COMPLETE			event_action[1]
#define F_TIMEOUT_COMPLETE		event_action[2]
#define F_ALERT					event_action[3]
#define F_DISCONNECT			event_action[4]

// Abstracting some magic numbers
#define SECONDS_15	150 	//Counter value when 15 seconds have passed

/*
 * Function - average_temperature
 * Brief - averages temperature value
 * Arguments -
 * system_state global struct -> to update global value of system
 */
void average_temperature(system_state_t* system)
{
	// If average == zero - that is acquisition just started
	if (system->average_temperature == 0)
		system->average_temperature = system->temperature;
	else
		system->average_temperature = (system->average_temperature + system->temperature) / 2;
}

/*
 * Function - Print_Message
 * Brief - prints status messages based on error enums
 * Arguments -
 * function_name -> to print function name
 * error_t -> error enum value
 * NOTE - this function only prints status messages
 */
void Print_Message(const char* function_name, error_t error)
{
	errno = error;
	logger.Log_Write(function_name, mStatus, "%s", Get_Error_Message(errno));
}

/*
 * Function - End_Program
 * Brief - ends the program by going into infinite loop
 */
void End_Program(void)
{
	while(1);
}

/*
 * Function - State_Machine_Init
 * Brief - Initializes state machine
 * Arguments -
 * state_machine_type -> State-driven or Table-driven state machine
 * Return -
 * returns a pointer to state machine struct
 */
state_machine_t* State_Machine_Init(state_machine_type_t type)
{
	if(logger.Get_Log_Level() == lDebug)
		logger.Log_Write(__func__, mDebug, "Initializing State Machine");
	state_machine_t* sm = (state_machine_t *) malloc(sizeof(state_machine_t));

	if(sm == NULL)
	{
		logger.Log_Write(__func__, mStatus, "Failed to initialize state machine");
		return NULL;
	}

	sm->state = sTemperature_Reading;
	sm->event = eStart;
	sm->type = type;

	logger.Log_Write(__func__, mStatus, "State Machine Initialized");

	return sm;
}

/*
 * Function - State_Machine_End
 * Brief - Ends state machine
 * Arguments -
 * pointer to state_machine struct
 */
void State_Machine_End(state_machine_t* sm)
{
	if(sm == NULL)
		return;

	free(sm);
	sm = NULL;
}

/*
 * Function - Set_Event
 * Brief - Sets events
 * Arguments -
 * state_machine struct -> which state_machine
 * event -> which event to set the state_machine to
 */
void Set_Event(state_machine_t* sm, event_t event)
{
	sm->event = event;
}

/*
 * Function - Set_State
 * Brief - Sets state
 * Arguments -
 * state_machine struct -> which state_machine
 * state -> which state to set the state_machine to
 */
void Set_State(state_machine_t* sm, state_t state)
{
	sm->state = state;
}

/*
 * Function - Temperature_Reading_State
 * Brief - Addresses Temperature Reading State
 * Arguments -
 * state_machine struct -> which state_machine
 * system_state struct -> a pointer to system state global struct
 */
static inline void Temperature_Reading_State(state_machine_t* sm, system_state_t* system)
{
	if(logger.Get_Log_Level() == lDebug)
	{
		logger.Log_Write(__func__, mDebug, "State - Temperature Reading");
	}

	// Check for Alert
	if(system->alert == 1)
	{
		// Transitions
		if(sm->type == State_Driven)
			Set_Event(sm, eAlert);
		else if (sm->type == Table_Driven)
			S_TEMPERATURE_READING.F_ALERT(sm);
		if(logger.Get_Log_Level() == lDebug)
		{
			logger.Log_Write(__func__, mDebug, "Alert Addressed");
		}
		system->alert = 0;
	}
	// Check for Disconnect
	else if(system->disconnect == 1)
	{
		if(sm->type == State_Driven)
			Set_Event(sm, eDisconnect);
		else if (sm->type == Table_Driven)
			S_TEMPERATURE_READING.F_DISCONNECT(sm);
		if(logger.Get_Log_Level() == lDebug)
		{
			logger.Log_Write(__func__, mDebug, "Disconnect Addressed");
		}
	}
	// Start Event
	if(sm->event == eStart)
	{
		if(logger.Get_Log_Level() == lDebug)
		{
			logger.Log_Write(__func__, mDebug, "Event - Start");
		}

		// Start Event actions
		Turn_On_Only_LED(Green);
		Print_Message(__func__, Reading_Temperature);
		int16_t temp = I2C_Read(TMP102.tmp_reg_address);
		system->temperature = Get_Temperature(temp);
		logger.Log_Write(__func__, mStatus, "Temperature = %d", system->temperature);
		Print_Message(__func__, Reading_Temperature_Complete);

		// Transitions
		Set_State(sm, sAverage_Wait);
		if(sm->type == State_Driven)
			Set_Event(sm, eRead_Complete);
		else if (sm->type == Table_Driven)
			S_TEMPERATURE_READING.F_READ_COMPLETE(sm);
	}

	// Alert Event
	else if(sm->event == eAlert)
	{
		if(logger.Get_Log_Level() == lDebug)
		{
			logger.Log_Write(__func__, mDebug, "Event - Alert");
		}
		// Transitions
		Set_State(sm, sTemperature_Alert);
	}

	// Disconnect Event
	else if(sm->event == eDisconnect)
	{
		if(logger.Get_Log_Level() == lDebug)
		{
			logger.Log_Write(__func__, mDebug, "Event - Disconnect");
		}
		// Transitions
		Set_State(sm, sDisconnected);
	}
}

/*
 * Function - Average_Wait_State
 * Brief - Addresses Average Wait State
 * Arguments -
 * state_machine struct -> which state_machine
 * system_state struct -> a pointer to system state global struct
 */
static inline void Average_Wait_State(state_machine_t* sm, system_state_t* system)
{
	// This is in wait, and I just want it to be printed once
	if(logger.Get_Log_Level() == lDebug && system->timeout_started == 0)
	{
		logger.Log_Write(__func__, mDebug, "State - Average Wait");
	}

	//Check for Disconnect
	if(system->disconnect == 1)
	{
		// Transitions
		if(sm->type == State_Driven)
			Set_Event(sm, eDisconnect);
		else if (sm->type == Table_Driven)
			S_AVERAGE_WAIT.F_DISCONNECT(sm);

		if(logger.Get_Log_Level() == lDebug)
		{
			logger.Log_Write(__func__, mDebug, "Disconnect Addressed");
		}
	}

	// Event - Read Complete
	if(sm->event == eRead_Complete)
	{

		// Timeout
		if(system->timeout_started != 1)
		{
			// This is in wait, and I just want it to be printed once
			if(logger.Get_Log_Level() == lDebug && system->timeout_started == 0)
			{
				logger.Log_Write(__func__, mDebug, "Event - Read Complete");
			}
			// Do an average
			average_temperature(system);
			logger.Log_Write(__func__, mStatus, "Average Temperature = %d",\
					system->average_temperature);
			Print_Message(__func__, Waiting);

			// Start Timeout Counting
			system->counter = 0;
			system->timeout_started = 1;
			SysTick_Config(48000000L/10L);
		}
		else
		{
			if(system->counter >= SECONDS_15)
			{
				// Transitions
				if(sm->type == State_Driven)
					Set_Event(sm, eTimeout_Complete);
				else if (sm->type == Table_Driven)
					S_AVERAGE_WAIT.F_TIMEOUT_COMPLETE(sm);

				Print_Message(__func__, Timeout);

				// Reset System State related to Timeout
				system->timeout_started = 0;
				system->counter = 0;
				SysTick->CTRL = 0;
			}
		}
	}

	// Event - Timeout Complete
	else if(sm->event == eTimeout_Complete)
	{
		if(logger.Get_Log_Level() == lDebug)
		{
			logger.Log_Write(__func__, mDebug, "Event - Timeout Complete");
		}


		// Increment Timeout Count
		system->timeout_count++;
		// Transition to other state machine once Timeout(4) occurs
		if(system->timeout_count == 4)
		{
			if(system->state_machine_id == 1)
				system->state_machine_id = 2;
			else if (system->state_machine_id == 2)
				system->state_machine_id = 1;
			system->timeout_count = 0;
			system->print_flag = 1;
		}

		// Transitions
		Set_State(sm, sTemperature_Reading);
		if(sm->type == State_Driven)
			Set_Event(sm, eStart);
		else if (sm->type == Table_Driven)
			S_AVERAGE_WAIT.F_START(sm);
	}

	// Event - Disconnect
	else if(sm->event == eDisconnect)
	{
		if(logger.Get_Log_Level() == lDebug)
		{
			logger.Log_Write(__func__, mDebug, "Event - Timeout Complete");
		}
		Set_State(sm, sDisconnected);
	}
}

/*
 * Function - Temperature_Alert_State
 * Brief - Addresses Temperature Alert State
 * Arguments -
 * state_machine struct -> which state_machine
 * system_state struct -> a pointer to system state global struct
 */
static inline void Temperature_Alert_State(state_machine_t* sm, system_state_t* system)
{
	if(logger.Get_Log_Level() == lDebug)
	{
		logger.Log_Write(__func__, mDebug, "State - Temperature Alert");
	}

	//Check for Disconnect
	if(system->disconnect == 1)
	{
		// Transitions
		if(sm->type == State_Driven)
			Set_Event(sm, eDisconnect);
		else if (sm->type == Table_Driven)
			S_AVERAGE_WAIT.F_DISCONNECT(sm);

		if(logger.Get_Log_Level() == lDebug)
		{
			logger.Log_Write(__func__, mDebug, "Disconnect Addressed");
		}
	}

	// Event - Alert
	if(sm->event == eAlert)
	{
		if(logger.Get_Log_Level() == lDebug)
		{
			logger.Log_Write(__func__, mDebug, "Event - Alert");
		}

		Turn_On_Only_LED(Blue);
		Print_Message(__func__, Alert_LOW_Temperature);

		// Transitions
		Set_State(sm, sAverage_Wait);
		if(sm->type == State_Driven)
			Set_Event(sm, eRead_Complete);
		else if (sm->type == Table_Driven)
			S_AVERAGE_WAIT.F_READ_COMPLETE(sm);
	}

	// Event Disconnect
	else if(sm->event == eDisconnect)
	{
		if(logger.Get_Log_Level() == lDebug)
		{
			logger.Log_Write(__func__, mDebug, "Event - Alert");
		}

		// Transitions
		Set_State(sm, sDisconnected);
	}
}

/*
 * Function - Event_Handler
 * Brief - handles the events related to state machines
 * Arguments -
 * state_machine struct -> which state_machine
 * system_state struct -> a pointer to system state global struct
 */
void Event_Handler(state_machine_t* sm, system_state_t* system)
{
	// If state machine doesn't exist, exit
	if (sm == NULL)
		exit(1);

	// Check for state
	switch(sm->state)
	{

	// State - Temperature Reading
	case sTemperature_Reading:
		Temperature_Reading_State(sm, system);
		break;

	// State - Average Wait
	case sAverage_Wait:
		Average_Wait_State(sm, system);
		break;

	// State - Temperature Alert
	case sTemperature_Alert:
		Temperature_Alert_State(sm, system);
		break;

	// State - Disconnect
	case sDisconnected:
		if(logger.Get_Log_Level() == lDebug)
		{
			logger.Log_Write(__func__, mDebug, "State - Disconnected");
		}
		if(sm->event == eDisconnect)
		{
			if(logger.Get_Log_Level() == lDebug)
			{
				logger.Log_Write(__func__, mDebug, "Event - Disconnect");
			}
			Turn_On_Only_LED(Red);
			logger.Log_Write(__func__, mError, "Disconnect Event Occured, Ending Program");
			Print_Message(__func__, Device_Disconnected);
			State_Machine_End(sm);
			End_Program();
		}
		break;
	}
}

//-------------------------------------------------------------------
// Functions for Table Driven Event Transitions
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
