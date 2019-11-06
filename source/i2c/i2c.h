/**
  * File Name 		- i2c.h
  * Description 	- header file for i2c.c
  * Author			- Atharva Nandanwar
  * Tools			- GNU C Compiler / ARM Compiler Toolchain
  * Leveraged Code 	-
  * URL				-
  */

#ifndef I2C_I2C_H_
#define I2C_I2C_H_
#include <stdint.h>
#include "MKL25Z4.h"
#include "tmp102.h"
#include "logger.h"

// I2C Macros
#define READ	1
#define WRITE	0

// Prototype Functions
void I2C_Init(void);
void I2C_Alert_Init(void);
uint8_t I2C_Check_Connect(void);
uint16_t I2C_Read(uint8_t register_address);
void I2C_Write(uint8_t register_address, uint8_t byte1, uint8_t byte2);
#endif /* I2C_I2C_H_ */
