/*
 * logger.c
 *
 *  Created on: Oct 31, 2019
 *      Author: azzentys
 */

#include "logger.h"

typedef struct {
		log_level_t Logger_Log_Level;
		silent_t Logger_Silent_Mode;
}logger_data;

logger_data thisLogger;

void Log_Write(const char *msg, ... )
{
	va_list args;
	va_start(args, msg);

	// Log Level Logic


	// Pure print
	vprintf(msg, args);
}

void Log_Silent (silent_t silent)
{
	thisLogger.Logger_Silent_Mode = silent;
}

void Set_Log_Level (log_level_t level)
{
	thisLogger.Logger_Log_Level = level;
}

logger_instance const logger = {Log_Write, Log_Silent, Set_Log_Level};
