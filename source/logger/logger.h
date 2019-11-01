/*
 * logger.h
 *
 *  Created on: Oct 31, 2019
 *      Author: azzentys
 */

#ifndef LOGGER_LOGGER_H_
#define LOGGER_LOGGER_H_
#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <stdarg.h>

typedef enum {Test, Debug, Normal} log_level_t;
typedef enum {False, True} silent_t;

typedef struct {
	void ( * const Log_Write )( const char *msg, ... );
	void ( * const Log_Silent )( silent_t silent );
	void ( * const Set_Log_Level )( log_level_t level );
}logger_instance;

extern logger_instance const logger;

#endif /* LOGGER_LOGGER_H_ */
