/*
 * post.c
 *
 *  Created on: Oct 31, 2019
 *      Author: azzentys
 */

#include "post.h"

uint16_t post(void)
{
	errno = Initiating_POST;
	logger.Log_Write("%s\n\r", Get_Error_Message(errno));

	I2C_Init();
	I2C_Write(TMP102.config_reg_address, 0x78, 0x80);
	for(volatile int i = 10000; i > 0; i--);
	volatile uint16_t temp = I2C_Read(TMP102.config_reg_address);
	if(temp == 0x7880)
	{
		return POST_Successful;
	}
	else
	{
		return POST_Failed;
	}
}
