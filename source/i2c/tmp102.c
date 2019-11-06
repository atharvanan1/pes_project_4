/**
  * File Name 		- tmp102.c
  * Description 	- contains function for TMP102 device temperature
  * 				  and device structure that has register values
  * Author			- Atharva Nandanwar
  * Tools			- GNU C Compiler / ARM Compiler Toolchain
  * Leveraged Code 	-
  * URL				-
  */

#include "tmp102.h"

/*
 * Function - Get_Temperature
 * Brief - Converts raw data into temperature value
 * Arguments -
 * data -> 16-bit integer value
 * Return -
 * 16-bit integer, which is converted temperature value of data argument
 */
int16_t Get_Temperature(uint16_t data)
{
	// Manipulation done after referring to TMP102
	// datasheet
	data = data >> 4;
	if ((data & MSB) == 1)
	{
		// Negative temperatures
		data = ((~data & 0x0FFF) + 1) * -1;
		return data * RESOLUTION;
	}
	else if ((data & MSB) == 0)
	{
		return data * RESOLUTION;
	}
}

// TMP102 register addresses
device TMP102 = {0x48, 0x00, 0x01, 0x03, 0x02};
