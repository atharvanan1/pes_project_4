/**
  * File Name 		- test.c
  * Description 	- contains test cases for the program
  * Author			- Atharva Nandanwar
  * Tools			- GNU C Compiler / ARM Compiler Toolchain
  * Leveraged Code 	-
  * URL				-
  */

#include "test.h"

// Global variables
error_t errno;
system_state_t __system = {0, 0, 0, 0, 0, 0, 0, 0};
system_state_t* system_state = &__system;

static inline void delay(void)
{
	for(volatile int i = 10000; i > 0; i--);
}
/*
 * Function - unit_tests
 * Brief - Executes unit tests
 */
void unit_tests(void)
{
	system_state->alert = 0;
	UCUNIT_TestcaseBegin("Starting Test Cases\n\r");
	UCUNIT_TestcaseBegin("Test Case for Write Log\n\r");
	errno = post();
	UCUNIT_CheckIsEqual(errno, POST_Successful);
	logger.Log_Write(__func__, mStatus, "%s", Get_Error_Message(errno));
	UCUNIT_TestcaseEnd();

	uint16_t dummy;
	UCUNIT_TestcaseBegin("Test Case for I2C Read & Write Operation\n\r");
	I2C_Init();
	I2C_Write(TMP102.tmp_LOW_reg_address, 0x00, 0x00);
	delay();
	logger.Log_Write(__func__, mStatus, "I2C Data Receive is %d", dummy = I2C_Read(TMP102.tmp_HI_reg_address));
	UCUNIT_CheckIsEqual(dummy, 0x0500);
	delay();
	logger.Log_Write(__func__, mStatus, "I2C Data Receive is %d", dummy = I2C_Read(TMP102.tmp_LOW_reg_address));
	UCUNIT_CheckIsEqual(dummy, 0x0000);
	delay();
	logger.Log_Write(__func__, mStatus, "I2C Data Receive is %d", dummy = I2C_Read(TMP102.tmp_reg_address));
	delay();
	UCUNIT_TestcaseEnd();

	UCUNIT_TestcaseBegin("State Machine Testing - State Driven\n\r");
	UCUNIT_TestcaseBegin("Test Case for State Machine Init\n\r");
	state_machine_t* sm_test = NULL;
	sm_test = (state_machine_t *) State_Machine_Init(Table_Driven);
	logger.Log_Write(__func__, mStatus, "Testing initial cases");
	UCUNIT_CheckIsEqual(sm_test->event, eStart);
	UCUNIT_CheckIsEqual(sm_test->state, sTemperature_Reading);
	UCUNIT_CheckIsEqual(sm_test->type, Table_Driven);
	UCUNIT_TestcaseEnd();

	UCUNIT_TestcaseBegin("Test Case for State Machine Temperature Reading State\n\r");
	Event_Handler(sm_test, system_state);
	UCUNIT_CheckIsEqual(sm_test->event, eRead_Complete);
	UCUNIT_CheckIsEqual(sm_test->state, sAverage_Wait);
	UCUNIT_TestcaseEnd();

	UCUNIT_TestcaseBegin("Test Case for State Machine Temperature Reading Complete State\n\r");
	UCUNIT_WriteString("and Transition to Average_Wait state\n\r");
	Event_Handler(sm_test, system_state);
	UCUNIT_CheckIsEqual(sm_test->event, eRead_Complete);
	UCUNIT_CheckIsEqual(sm_test->state, sAverage_Wait);
	logger.Log_Write(__func__, mStatus, "%d", system_state->counter);
	UCUNIT_CheckIsEqual(system_state->timeout_started, 1);
	UCUNIT_TestcaseEnd();

	while(system_state->counter != 150);

	UCUNIT_TestcaseBegin("Test Case for State Machine Timeout\n\r");
	Event_Handler(sm_test, system_state);
	UCUNIT_CheckIsEqual(sm_test->event, eTimeout_Complete);
	UCUNIT_CheckIsEqual(sm_test->state, sAverage_Wait);
	UCUNIT_CheckIsEqual(system_state->timeout_started, 0);
	UCUNIT_TestcaseEnd();

	UCUNIT_TestcaseBegin("Test Case for State Machine Temperature Reading after Timeout\n\r");
	Event_Handler(sm_test, system_state);
	UCUNIT_CheckIsEqual(sm_test->event, eStart);
	UCUNIT_CheckIsEqual(sm_test->state, sTemperature_Reading);
	UCUNIT_CheckIsEqual(system_state->timeout_started, 0);
	UCUNIT_TestcaseEnd();

	while(1)
	{
		if(system_state->alert == 1)
			break;
	}

	UCUNIT_TestcaseBegin("Test Case for State Machine Alert in Reading State\n\r");
	Event_Handler(sm_test, system_state);
	UCUNIT_CheckIsEqual(sm_test->event, eAlert);
	UCUNIT_CheckIsEqual(sm_test->state, sTemperature_Alert);
	UCUNIT_TestcaseEnd();

	UCUNIT_TestcaseBegin("Test Case for State Machine Alert State\n\r");
	Event_Handler(sm_test, system_state);
	UCUNIT_CheckIsEqual(sm_test->event, eRead_Complete);
	UCUNIT_CheckIsEqual(sm_test->state, sAverage_Wait);
	UCUNIT_TestcaseEnd();

	UCUNIT_TestcaseBegin("Test Case for State Machine Alert Addressed, shifting to Wait\n\r");
	Event_Handler(sm_test, system_state);
	UCUNIT_CheckIsEqual(system_state->counter, 0);
	UCUNIT_CheckIsEqual(sm_test->event, eRead_Complete);
	UCUNIT_CheckIsEqual(sm_test->state, sAverage_Wait);
	UCUNIT_TestcaseEnd();

	while(system_state->counter < 150);

	UCUNIT_TestcaseBegin("Test Case for State Machine Timeout\n\r");
	Event_Handler(sm_test, system_state);
	UCUNIT_CheckIsEqual(system_state->counter, 150);
	UCUNIT_CheckIsEqual(sm_test->event, eTimeout_Complete);
	UCUNIT_CheckIsEqual(sm_test->state, sAverage_Wait);
	UCUNIT_CheckIsEqual(system_state->timeout_started, 0);
	UCUNIT_TestcaseEnd();

	UCUNIT_TestcaseBegin("Test Case for State Machine Temperature Reading after Timeout\n\r");
	Event_Handler(sm_test, system_state);
	UCUNIT_CheckIsEqual(sm_test->event, eStart);
	UCUNIT_CheckIsEqual(sm_test->state, sTemperature_Reading);
	UCUNIT_TestcaseEnd();

	UCUNIT_TestcaseBegin("Test Case for State Machine Disconnect\n\r");
	system_state->disconnect = 1;
	Event_Handler(sm_test, system_state);
	UCUNIT_CheckIsEqual(sm_test->event, eDisconnect);
	UCUNIT_CheckIsEqual(sm_test->state, sDisconnected);
	UCUNIT_TestcaseEnd();
}

/*
 * Function - Main
 * Brief - Main testing routine
 */
int main(void)
{
	//Initializing board pins
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
	BOARD_InitDebugConsole();
	I2C_Init();
	I2C_Write(TMP102.config_reg_address, 0x78, 0x80);
	for(volatile int i = 10000; i > 0; i--);
	I2C_Write(TMP102.tmp_HI_reg_address, 0x05, 0x00);
	for(volatile int i = 10000; i > 0; i--);
	I2C_Write(TMP102.tmp_LOW_reg_address, 0x00, 0x00);
	I2C_Alert_Init();
	//logger.Set_Log_Level(Test);

	//Calling function to run tests
	unit_tests();
	return 0;
}

/*
 * Function - SysTick_Handler
 * Brief - Systick interrupt handler
 */
void SysTick_Handler(void)
{
	// Count up if timeout timer is started
	if(system_state->timeout_started)
		system_state->counter++;
}

/*
 * Function - PORTA_IRQHandler
 * Brief - Port A interrupt handler
 */
void PORTA_IRQHandler(void)
{
	// If interrupt from Pin 5, then set alert
	if(PORTA->ISFR & ALERT_PIN)
	{
		system_state->alert = 1;
	}
	// Clearing the interrupt
	PORTA->PCR[5] |= PORT_PCR_ISF_MASK;
}
