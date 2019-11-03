/*
 * test.c
 *
 *  Created on: Oct 31, 2019
 *      Author: azzentys
 */
#include "test.h"

error_t errno;
state_machine_t* current_state_machine;
state_t* current_state;
event_t* current_event;

void unit_tests(void)
{
	UCUNIT_TestcaseBegin("Test Case for Write Log\n\r");
	errno = POST_Failed;
//	int integer = 5;
//	char* string = "HHHH";
//	char character = 'c';
	logger.Log_Write("%s\n\r", Get_Error_Message(errno));
	UCUNIT_TestcaseEnd();

	UCUNIT_TestcaseBegin("Test Case for State Machine Init\n\r");
	state_machine_init(&sm1);
	if(current_state_machine->machine == State_Machine_1)
	{
		// Print State Machine Message
		errno = Entering_SM1;
		logger.Log_Write("%s\n\r", Get_Error_Message(errno));
		// Do something with the event, but we already know the event
		// Maybe we can add a function which will figure out what to give to current_event
		// pointer
		int i = 1;
		while(i)
		{
			switch(current_state->id)
			{
			case Temperature_Reading:
				if(valid_event(current_event, temp_reading))
				{
					trigger(current_event);
				}
				break;
			case Average_Wait:
				if(valid_event(current_event, average_wait))
				{
					trigger(current_event);
				}
				break;
			case Temperature_Alert:
				if(valid_event(current_event, temp_alert))
				{
					trigger(current_event);
				}
				break;
			case Disconnected:
				if(valid_event(current_event, disconnected))
				{
					trigger(current_event);
					i = 0;
				}
				break;
			}
		}
	}
	UCUNIT_TestcaseEnd();

	uint16_t dummy;
	UCUNIT_TestcaseBegin("Test Case for I2C Read Opeartion\n\r");
	if(I2C_Init())
	{
		logger.Log_Write("Initialization successful!\n\r");
	}
	logger.Log_Write("I2C Data Receive is %d\n\r", dummy = I2C_Read_From_Slave());
	logger.Log_Write("Temperature Data is %d\n\r", get_temperature(dummy));
	UCUNIT_TestcaseEnd();
}

int main(void)
{
	//Initializing board pins
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
	BOARD_InitDebugConsole();
	//Calling function to run memory tests
	unit_tests();
	return 0;
}
