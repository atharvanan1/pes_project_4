/*
 * test.c
 *
 *  Created on: Oct 31, 2019
 *      Author: azzentys
 */
#include "test.h"

state_t errno;

void unit_tests(void)
{
	UCUNIT_TestcaseBegin("Starting Demo Test Case\n\r");
	int a = test_state_machine();
	UCUNIT_CheckIsEqual(a, 0);
	UCUNIT_TestcaseEnd();

	UCUNIT_TestcaseBegin("Test Case for Write Log\n\r");
	errno = POST_Failed;
//	int integer = 5;
//	char* string = "HHHH";
//	char character = 'c';
	logger.Log_Write("%s\n\r", Get_Error_Message(errno));
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
