/**
  * File Name 		- post.c
  * Description 	- contains power on self test
  * Author			- Atharva Nandanwar
  * Tools			- GNU C Compiler / ARM Compiler Toolchain
  * Leveraged Code 	-
  * URL				-
  */

#include "post.h"

/*
 * Function - post
 * Brief - executes Power On Self Test
 * Returns -
 * error enum value indicating POST success or fail
 */
uint16_t post(void)
{
	if(logger.Get_Log_Level() == lDebug)
	{
		logger.Log_Write(__func__, mDebug, "Power On Self Test Started");
	}

	// Do a write operation, and read if it's written
	I2C_Write(TMP102.config_reg_address, 0x78, 0x80);
	for(volatile int i = 10000; i > 0; i--);
	volatile uint16_t temp = I2C_Read(TMP102.config_reg_address);

	// If write value == read value, then SUCCESS!!
	if(temp == 0x7880 || temp == 0x78A0)
	{
		return POST_Successful;
	}
	else
	{
		return POST_Failed;
	}
}
