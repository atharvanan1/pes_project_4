/*
 * test.c
 *
 *  Created on: Oct 31, 2019
 *      Author: azzentys
 */
#include "test.h"

error_t errno;
system_state_t __system = {0, 0, 0, 0, 0, 0, 0};
system_state_t* system_state = &__system;

void state_machine_test(void)
{
	uint8_t i = 100;
	state_machine_t* sm1 = NULL;
	sm1 = State_Machine_Init();
	while(1)
	{
		Event_Handler(sm1, system_state);
	}
	State_Machine_End(sm1);
}

void unit_tests(void)
{
	UCUNIT_TestcaseBegin("Test Case for Write Log\n\r");
	errno = post();
	logger.Log_Write("%s\n\r", Get_Error_Message(errno));
	UCUNIT_TestcaseEnd();


	uint16_t dummy;
	UCUNIT_TestcaseBegin("Test Case for I2C Read & Write Opeartion\n\r");
	if(I2C_Init())
	{
		logger.Log_Write("Initialization successful!\n\r");
	}
	logger.Log_Write("I2C Write Operation\n\r");
	I2C_Write(TMP102.config_reg_address, 0x78, 0xA0);
	for(volatile int i = 10000; i > 0; i--);
	logger.Log_Write("I2C Data Receive is %d\n\r", dummy = I2C_Read(TMP102.tmp_reg_address));
	logger.Log_Write("Temperature Data is %d\n\r", system_state->temperature=Get_Temperature(dummy));
	UCUNIT_TestcaseEnd();

	UCUNIT_TestcaseBegin("Test Case for State Machine Init\n\r");
	state_machine_test();
	UCUNIT_TestcaseEnd();
}

int main(void)
{
	SysTick_Config(48000000L/10L);
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
//	logger.Log_Write("%d\n\r", system_state->counter);
}

void PORTA_IRQHandler(void)
{
	if(PORTA->ISFR & ALERT_PIN)
	{
		system_state->alert = 1;
	}
}
