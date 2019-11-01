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
