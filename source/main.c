/*
 * main.c
 *
 *  Created on: Oct 30, 2019
 *      Author: azzentys
 */

#include "main.h"

error_t errno;
system_state_t __system = {0, 0, 0, 0, 0, 0, 0, 0};
system_state_t* system_state = &__system;

int main(void)
{
	//Initializing board pins
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
	BOARD_InitDebugConsole();

	// Logger Setup
	logger.Set_Log_Level(Normal);

	// POST
	I2C_Init();
	errno = post();
	logger.Log_Write("%s\n\r", Get_Error_Message(errno));

	// I2C Initialization Routine
//	I2C_Write(TMP102.config_reg_address, 0x78, 0x80);
//	for(volatile int i = 10000; i > 0; i--);
//	I2C_Write(TMP102.tmp_HI_reg_address, 0x05, 0x00);
//	for(volatile int i = 10000; i > 0; i--);
//	I2C_Write(TMP102.tmp_LOW_reg_address, 0x00, 0x00);
	I2C_Alert_Init();

	// State Machine Setup
	state_machine_t* sm1 = NULL;
	state_machine_t* sm2 = NULL;
	sm1 = State_Machine_Init(State_Driven);
	sm2 = State_Machine_Init(Table_Driven);
	system_state->state_machine_id = 1;

	while(1)
	{
		if(I2C_Read(TMP102.tmp_reg_address) == 0xFFFF)
			system_state->disconnect = 1;
		if(system_state->state_machine_id == 1)
			Event_Handler(sm1, system_state);
		else if (system_state->state_machine_id == 2)
			Event_Handler(sm2, system_state);
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
