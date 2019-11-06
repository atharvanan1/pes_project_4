/*
 * main.c
 *
 *  Created on: Oct 30, 2019
 *      Author: azzentys
 */

#include "main.h"

error_t errno;
system_state_t __system = {0, 0, 0, 0, 0, 0, 0, 0, 1};
system_state_t* system_state = &__system;

int main(void)
{
	//Initializing board pins
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
	BOARD_InitDebugConsole();

	printf("\x1B[1;33m\n\r\n\r\n\r----------Starting Program----------\x1B[0m\n\r");
	// Logger Setup
#ifdef RUN
	logger.Set_Log_Level(lNormal);
#elif LOG
	logger.Set_Log_Level(lDebug);
#endif

	// POST
	I2C_Init();
	errno = post();
	if(errno == POST_Successful)
	{
		logger.Log_Write(__func__, mStatus, Get_Error_Message(errno));
	}
	else if (errno == POST_Failed)
	{
		logger.Log_Write(__func__, mError, Get_Error_Message(errno));
		Turn_On_Only_LED(Red);
		End_Program();
	}

	// I2C Initialization Routine
	I2C_Write(TMP102.config_reg_address, 0x78, 0x80);
	for(volatile int i = 10000; i > 0; i--);
	I2C_Write(TMP102.tmp_HI_reg_address, 0x05, 0x00);
	for(volatile int i = 10000; i > 0; i--);
	I2C_Write(TMP102.tmp_LOW_reg_address, 0x00, 0x00);
	I2C_Alert_Init();

	// State Machine Setup
	state_machine_t* sm1 = NULL;
	state_machine_t* sm2 = NULL;
	sm1 = State_Machine_Init(State_Driven);
	sm2 = State_Machine_Init(Table_Driven);
	system_state->state_machine_id = 1;
	system_state->alert = 0;

	while(1)
	{
		// Check if device is disconnected
		if(I2C_Check() == DISCONNECTED)
		{
			system_state->disconnect = 1;
		}

		// Toggling between state machines
		if(system_state->state_machine_id == 1)
		{
			if(system_state->print_flag)
			{
				logger.Log_Write(__func__, mDebug, "State Machine 1");
				system_state->print_flag = 0;
			}
			Event_Handler(sm1, system_state);
		}
		else if (system_state->state_machine_id == 2)
		{
			if(system_state->print_flag)
			{
				logger.Log_Write(__func__, mDebug, "State Machine 2");
				system_state->print_flag = 0;
			}
			Event_Handler(sm2, system_state);
		}

		// Delay for disconnect check
		for(volatile int i = 1000; i > 0; i--);
	}
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
}
