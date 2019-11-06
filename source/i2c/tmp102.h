/**
  * File Name 		- tmp102.h
  * Description 	- header file for tmp102.c
  * Author			- Atharva Nandanwar
  * Tools			- GNU C Compiler / ARM Compiler Toolchain
  * Leveraged Code 	-
  * URL				-
  */

#ifndef I2C_TMP102_H_
#define I2C_TMP102_H_
#include <stdint.h>
#include "MKL25Z4.h"

// Macros
#define MSB			(0x01 << 11)
#define RESOLUTION 	0.0625
#define ALERT_PIN	(0x0001 << 5)

// Struct for device addresses and registers
typedef struct {
	uint8_t address;
	uint8_t tmp_reg_address;
	uint8_t config_reg_address;
	uint8_t tmp_HI_reg_address;
	uint8_t tmp_LOW_reg_address;
}device;

extern device TMP102;

// Prototype Functions
int16_t Get_Temperature(uint16_t data);
#endif /* I2C_TMP102_H_ */
