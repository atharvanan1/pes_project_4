/*
 * logger.c
 *
 *  Created on: Oct 31, 2019
 *      Author: azzentys
 */

#include "logger.h"

void test_writeLog(uint8_t log_level, const char *msg, ... )
{
	va_list args;
	void* args_array[10];
	uint8_t arg_counts = 0;
	va_start(args, msg);

	// Log Level Logic

	// Pure print
	vprintf(msg, args);
}
