/*
 * main.c
 *
 *  Created on: Oct 30, 2019
 *      Author: azzentys
 */

#include "main.h"

system_state_t system = {
		temperature = 0,
		average_temperature = 0,
		timeout_count = 0,
		counter = 0,
		timeout_started = 0,
		disconnect = 0,
		alert = 0,
};

int main(void)
{
	getEvent();
	state_machine_1();
	state_machine_2();
}

void PORTA_IRQHandler(void)
{
	printf("here in port interrupt!\n\r");
}

void SysTick_Handler(void)
{
	system->counter++;
}
