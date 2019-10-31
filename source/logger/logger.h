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

void test_writeLog(uint8_t log_level, const char *msg, ... );
#endif /* LOGGER_LOGGER_H_ */
