/*
 * main.c
 *
 *  Created on: Oct 30, 2019
 *      Author: azzentys
 */

#include "main.h"

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
