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
	uint16_t temp = I2C_Read(TMP102.config_reg_address);
	if(temp == 0x78A0)
	{
		return POST_Successful;
	}
	else
	{
		return POST_Failed;
	}
}
