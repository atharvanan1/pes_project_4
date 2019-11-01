/**
  * File Name 		- led_control.c
  * Description 	- contains function for turning on LEDs
  * Author			- Atharva Nandanwar
  * Tools			- GNU C Compiler / ARM Compiler Toolchain
  * Leveraged Code 	-
  * URL				-
  */

#include "led_control.h"

void Turn_On_Only_LED(uint8_t LED)
{
	// LED_string is used to print messages
	char* LED_string = NULL;
	// KL25Z board specific LED operations
	LED_RED_INIT(LOGIC_LED_OFF);
	LED_BLUE_INIT(LOGIC_LED_OFF);
	LED_GREEN_INIT(LOGIC_LED_OFF);

	if(LED == RED)
	{
		LED_string = "RED";
		printf("LED %s is ON\n\r", LED_string);
		LED_RED_ON();
		LED_GREEN_OFF();
		LED_BLUE_OFF();
	}

	else if (LED == BLUE)
	{
		LED_string = "BLUE";
		printf("LED %s is ON\n\r", LED_string);
		LED_RED_OFF();
		LED_GREEN_OFF();
		LED_BLUE_ON();
	}

	else if (LED == GREEN)
	{
		LED_string = "GREEN";
		printf("LED %s is ON\n\r", LED_string);
		LED_RED_OFF();
		LED_GREEN_ON();
		LED_BLUE_OFF();
	}
}
