/*
 * test.c
 *
 *  Created on: Oct 31, 2019
 *      Author: azzentys
 */
#include "test.h"

error_t errno;
system_state_t __system = {0, 0, 0, 0, 0, 0, 0, 0};
system_state_t* system_state = &__system;

void unit_tests(void)
{
	UCUNIT_TestcaseBegin("Starting Test Cases\n\r");
	UCUNIT_TestcaseBegin("Test Case for Write Log\n\r");
	errno = post();
	UCUNIT_CheckIsEqual(errno, POST_Successful);
	logger.Log_Write("%s\n\r", Get_Error_Message(errno));
	UCUNIT_TestcaseEnd();

	uint16_t dummy;
	UCUNIT_TestcaseBegin("Test Case for I2C Read & Write Opeartion\n\r");
	if(I2C_Init())
	{
		logger.Log_Write("Initialization successful!\n\r");
	}
	logger.Log_Write("I2C Write Operation\n\r");
	logger.Log_Write("I2C Data Receive is %d\n\r", dummy = I2C_Read(TMP102.tmp_HI_reg_address));
	UCUNIT_CheckIsEqual(dummy, 0x0500);
	logger.Log_Write("I2C Data Receive is %d\n\r", dummy = I2C_Read(TMP102.tmp_LOW_reg_address));
	UCUNIT_CheckIsEqual(dummy, 0x0000);
	logger.Log_Write("I2C Data Receive is %d\n\r", dummy = I2C_Read(TMP102.tmp_reg_address));
	UCUNIT_TestcaseEnd();

	UCUNIT_TestcaseBegin("State Machine Testing - State Driven\n\r");
	UCUNIT_TestcaseBegin("Test Case for State Machine Init\n\r");
	state_machine_t* sm_test = NULL;
	sm_test = (state_machine_t *) State_Machine_Init(Table_Driven);
	logger.Log_Write("Testing initial cases\n\r");
	UCUNIT_CheckIsEqual(sm_test->event, eStart);
	UCUNIT_CheckIsEqual(sm_test->state, sTemperature_Reading);
	UCUNIT_CheckIsEqual(sm_test->type, State_Driven);
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
	logger.Log_Write("%d\n\r", system_state->counter);
	UCUNIT_CheckIsEqual(system_state->timeout_started, 1);
	UCUNIT_TestcaseEnd();

	while(system_state->counter != 150);

	UCUNIT_TestcaseBegin("Test Case for State Machine Timeout\n\r");
	Event_Handler(sm_test, system_state);
	UCUNIT_CheckIsEqual(system_state->counter, 150);
	UCUNIT_CheckIsEqual(sm_test->event, eTimeout_Complete);
	UCUNIT_CheckIsEqual(sm_test->state, sAverage_Wait);
	UCUNIT_CheckIsEqual(system_state->timeout_started, 0);
	UCUNIT_TestcaseEnd();

	UCUNIT_TestcaseBegin("Test Case for State Machine Temperature Reading after Timeout\n\r");
	Event_Handler(sm_test, system_state);
	UCUNIT_CheckIsEqual(system_state->counter, 150);
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
	UCUNIT_CheckIsEqual(sm_test->state, sTemperature_Reading);
	UCUNIT_TestcaseEnd();

	UCUNIT_TestcaseBegin("Test Case for State Machine Alert State\n\r");
	Event_Handler(sm_test, system_state);
	UCUNIT_CheckIsEqual(sm_test->event, eAlert);
	UCUNIT_CheckIsEqual(sm_test->state, sTemperature_Alert);
	UCUNIT_TestcaseEnd();

	UCUNIT_TestcaseBegin("Test Case for State Machine Alert Addressed, shifting to Wait\n\r");
	Event_Handler(sm_test, system_state);
	UCUNIT_CheckIsEqual(system_state->counter, 0);
	UCUNIT_CheckIsEqual(sm_test->event, eRead_Complete);
	UCUNIT_CheckIsEqual(sm_test->state, sAverage_Wait);
	UCUNIT_TestcaseEnd();

	UCUNIT_TestcaseBegin("Test Case for State Machine Alert Addressed, shifting to Wait\n\r");
	Event_Handler(sm_test, system_state);
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
	UCUNIT_CheckIsEqual(system_state->timeout_started, 0);
	UCUNIT_TestcaseEnd();

	while(1)
	{
		if(system_state->disconnect == 1)
			break;
	}
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

void SysTick_Handler(void)
{
	if(system_state->timeout_started)
		system_state->counter++;
}

void PORTA_IRQHandler(void)
{
	if(PORTA->ISFR & ALERT_PIN)
	{
		system_state->alert = 1;
	}
	PORTA->PCR[5] |= PORT_PCR_ISF_MASK;
	logger.Log_Write("Here!\n\r");
}
