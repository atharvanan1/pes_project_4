/**
  * File Name 		- logger.c
  * Description 	- contains logger subroutines
  * Author			- Atharva Nandanwar
  * Tools			- GNU C Compiler / ARM Compiler Toolchain
  * Leveraged Code 	- https://github.com/ntpeters/SimpleLogger/
  * URL				-
  */
#include "logger.h"

// Struct for storing logger data
typedef struct {
		log_level_t Logger_Log_Level;
}logger_data;

logger_data thisLogger;

// Character codes for colors
// Leveraged Code - https://stackoverflow.com/questions/3585846/color-text-in-terminal-applications-in-unix
const char* red = "\x1B[31m";
const char* green = "\x1B[32m";
const char* blue = "\x1B[34m";
const char* end = "\x1B[0m";

/*
 * Function - Log_Write
 * Brief - Prints a log message
 * Arguments -
 * function_name -> name of the calling function
 * message_type -> Error, Debug or Status message
 * msg, ... -> printf style argument to hold a string and format specifiers
 * Leveraged Code - https://www.ozzu.com/cpp-tutorials/tutorial-writing-custom-printf-wrapper-function-t89166.html
 */
void Log_Write(const char* function_name, message_type_t message_type, const char *msg, ... )
{
	// To process variable argument list
	va_list args;
	va_start(args, msg);

	// Activate color based on message type
	switch(message_type)
	{
	case mError:
		printf("%s", red);
		break;
	case mDebug:
		printf("%s", blue);
		break;
	case mStatus:
		printf("%s", green);
		break;
	}

	// Log Level Logic
	switch(thisLogger.Logger_Log_Level)
	{
	case lTest:
		printf("Test:");
		break;
	case lDebug:
		printf("Debug:\t");
		break;
	case lNormal:
		printf("Run:\t");
		break;
	}

	printf("%-27s:\t\t", function_name);


	// Message print with color termination code
	vprintf(msg, args);
	printf("%s\n\r", end);
}

/*
 * Function - Get_Log_Level
 * Brief - returns the current log level
 * Return -
 * returns log_level_t enum value
 */
log_level_t Get_Log_Level (void)
{
	return thisLogger.Logger_Log_Level;
}

/*
 * Function - Set_Log_Level
 * Brief - sets the current log level
 * Arguments -
 * log_level_t enum value
 */
void Set_Log_Level (log_level_t level)
{
	thisLogger.Logger_Log_Level = level;
}

// Declaration for logger struct
logger_instance const logger = {Log_Write, Set_Log_Level, Get_Log_Level};
