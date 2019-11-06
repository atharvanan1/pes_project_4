/**
  * File Name 		- errno.c
  * Description 	- contains error enums, and related functions
  * Author			- Atharva Nandanwar
  * Tools			- GNU C Compiler / ARM Compiler Toolchain
  * Leveraged Code 	- https://android.googlesource.com/kernel/lk/+/upstream-master/include/errno.h
  * URL				-
  */

#include "errno.h"

/*
 * Function - Get_Error_Message
 * Brief - Returns pre-defined error messages
 * Argument -
 * error_t -> error code for pre-defined errors, or events
 * Return -
 * returns a string with error message
 */
const char* Get_Error_Message(error_t error)
{
	switch(error)
	{
	case Starting_Program:
		return "Starting Program";
		break;
	case Initiating_POST:
		return "Initiating POST";
		break;
	case POST_Successful:
		return "POST Successful";
		break;
	case POST_Failed:
		return "POST Failed";
		break;
	case Entering_SM1:
		return "Entering State Machine 1";
		break;
	case Exiting_SM1:
		return "Exiting State Machine 1";
		break;
	case Entering_SM2:
		return "Entering State Machine 2";
		break;
	case Exiting_SM2:
		return "Exiting State Machine 2";
		break;
	case Reading_Temperature:
		return "Reading Temperature";
		break;
	case Reading_Temperature_Complete:
		return "Reading Temperature Complete";
		break;
	case Waiting:
		return "In Wait State";
		break;
	case Timeout:
		return "Timeout!";
		break;
	case Alert_LOW_Temperature:
		return "Alert - Low Temperature";
		break;
	case LED_Red_ON:
		return "LED Red is ON";
		break;
	case LED_Red_OFF:
		return "LED Red is OFF";
		break;
	case LED_Green_ON:
		return "LED Green is ON";
		break;
	case LED_Green_OFF:
		return "LED Green is OFF";
		break;
	case LED_Blue_ON:
		return "LED Blue is ON";
		break;
	case LED_Blue_OFF:
		return "LED Blue is OFF";
		break;
	case Device_Disconnected:
		return "Device Disconnected";
		break;
	case Unhandled_Exception:
		return "Unhandled Exception";
		break;
	default:
		return "Incorrect Status Code";
		break;
	}
}
