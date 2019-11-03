/*
 * tmp102.h
 *
 *  Created on: Nov 1, 2019
 *      Author: azzentys
 */

#ifndef I2C_TMP102_H_
#define I2C_TMP102_H_
#include <stdint.h>
#include "MKL25Z4.h"

#define MSB			(0x01 << 11)
#define RESOLUTION 	0.0625

// Struct for device addresses and registers
typedef struct {
	uint8_t address;
	uint8_t tmp_reg_address;
	uint8_t config_reg_address;
	uint8_t tmp_HI_reg_address;
	uint8_t tmp_LOW_reg_address;
}device;

extern device TMP102;

// Functions
int16_t get_temperature(uint16_t data);
#endif /* I2C_TMP102_H_ */
