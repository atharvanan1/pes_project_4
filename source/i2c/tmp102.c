/*
 * tmp102.c
 *
 *  Created on: Nov 1, 2019
 *      Author: azzentys
 */

#include "tmp102.h"

int16_t get_temperature(uint16_t data)
{
	data = data & 0x0FFF;
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

device TMP102 = {0x48, 0x00, 0x01, 0x02, 0x03};
