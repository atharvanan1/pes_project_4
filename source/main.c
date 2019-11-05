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
	getEvent();
	state_machine_1();
	state_machine_2();
}

void PORTA_IRQHandler(void)
{
	if(PORTA->ISFR & ALERT_PIN)
	{
		system_state->alert = 1;
	}
}

void SysTick_Handler(void)
{
	system_state->counter++;
}
