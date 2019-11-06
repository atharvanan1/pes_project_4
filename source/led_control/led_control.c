/**
  * File Name 		- led_control.c
  * Description 	- contains function for turning on LEDs
  * Author			- Atharva Nandanwar
  * Tools			- GNU C Compiler / ARM Compiler Toolchain
  * Leveraged Code 	-
  * URL				-
  */

#include "led_control.h"

/*
 * Function - Turn_On_Only_LED
 * Brief - Turns on specified LED
 * Argument -
 * led color enum
 */
void Turn_On_Only_LED(led_color_t LED)
{
	if(logger.Get_Log_Level() == lDebug)
	{
		logger.Log_Write(__func__, mDebug, "Turn ON LED Operation Started");
	}
	// LED_string is used to print messages
	LED_RED_INIT(LOGIC_LED_OFF);
	LED_BLUE_INIT(LOGIC_LED_OFF);
	LED_GREEN_INIT(LOGIC_LED_OFF);

	if(LED == Red)
	{
		errno = LED_Red_ON;
		LED_RED_ON();
		LED_GREEN_OFF();
		LED_BLUE_OFF();
	}

	else if (LED == Blue)
	{
		errno = LED_Blue_ON;
		LED_RED_OFF();
		LED_GREEN_OFF();
		LED_BLUE_ON();
	}

	else if (LED == Green)
	{
		errno = LED_Green_ON;
		LED_RED_OFF();
		LED_GREEN_ON();
		LED_BLUE_OFF();
	}

	logger.Log_Write(__func__, mStatus, Get_Error_Message(errno));
	if(logger.Get_Log_Level() == lDebug)
	{
		logger.Log_Write(__func__, mDebug, "Turn ON LED Operation Finshed");
	}
}
