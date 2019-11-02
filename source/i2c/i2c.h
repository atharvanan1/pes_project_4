/*
 * i2c.h
 *
 *  Created on: Oct 31, 2019
 *      Author: azzentys
 */

#ifndef I2C_I2C_H_
#define I2C_I2C_H_
#include <stdint.h>
#include "MKL25Z4.h"
#include "tmp102.h"

// I2C Macros
#define READ	1
#define WRITE	0

// Prototype Functions

uint8_t I2C_Init(void);
uint8_t I2C_Check_Connect(void);
uint16_t I2C_Read_From_Slave(void);
#endif /* I2C_I2C_H_ */
