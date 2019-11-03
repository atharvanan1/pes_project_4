/*
 * errno.h
 *
 *  Created on: Oct 31, 2019
 *      Author: azzentys
 */

#ifndef LOGGER_ERRNO_H_
#define LOGGER_ERRNO_H_
#include <stdint.h>

typedef enum {
	Starting_Program 				= 0x0000,
	Initiating_POST 				= 0x1000,
	POST_Successful 				= 0x1001,
	POST_Failed 					= 0x1002,
	Entering_SM1					= 0x2000,
	Exiting_SM1 					= 0x200F,
	Entering_SM2 					= 0x4000,
	Exiting_SM2 					= 0x400F,
	Reading_Temperature 			= 0x8000,
	Reading_Temperature_Complete 	= 0x8001,
	Waiting 						= 0x8002,
	Timeout 						= 0x8004,
	Alert_LOW_Temperature 			= 0x8008,
	LED_Red_ON						= 0x8101,
	LED_Red_OFF						= 0x8102,
	LED_Green_ON					= 0x8201,
	LED_Green_OFF					= 0x8202,
	LED_Blue_ON						= 0x8401,
	LED_Blue_OFF					= 0x8402,
	Unhandled_Exception				= 0xEEEE,
	Device_Disconnected 			= 0xFFFF,
}error_t;

extern error_t errno;

const char* Get_Error_Message(error_t error);

#endif /* LOGGER_ERRNO_H_ */
