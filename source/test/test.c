/*
 * test.c
 *
 *  Created on: Oct 31, 2019
 *      Author: azzentys
 */
#include "test.h"

void unit_tests(void)
{
	UCUNIT_TestcaseBegin("Starting Demo Test Case\n\r");
	int a = test_state_machine();
	UCUNIT_CheckIsEqual(a, 0);
	UCUNIT_TestcaseEnd();

	UCUNIT_TestcaseBegin("Test Case for Write Log\n\r");
	int integer = 5;
	char* string = "HHHH";
	char character = 'c';
	test_writeLog(1, "Here is a test message, %d, %s, %c\n\r", integer, string, character);
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
