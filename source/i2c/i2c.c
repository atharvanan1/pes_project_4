/*
 * i2c.c
 *
 *  Created on: Oct 31, 2019
 *      Author: azzentys
 */

#include "i2c.h"

uint8_t I2C_Init(void)
{
	// Enabling clock on Port C and I2C0 peripheral
	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;

	// Configuring Port C
	PORTC->PCR[8] |= PORT_PCR_MUX(2);
	PORTC->PCR[9] |= PORT_PCR_MUX(2);

	// Configuring I2C Peripheral
	I2C0->F |= I2C_F_MULT(2);
	I2C0->F	|= I2C_F_ICR(22);
	I2C0->C1 |= I2C_C1_IICEN_MASK; // | I2C_C1_IICIE_MASK;
	I2C0->C2 |= I2C_C2_HDRS_MASK;
	return 1;
}

uint8_t I2C_Check_Connect(void)
{

}

uint16_t I2C_Read_From_Slave(void)
{
	uint16_t data;
	uint8_t read;

	// Send start bit
	I2C0->C1 |= I2C_C1_MST(1) | I2C_C1_TX(1);

	// Send slave address
	I2C0->D = (TMP102.address << 1) | WRITE;

	// Wait for acknowledge by Slave
	while(I2C0->S & I2C_S_IICIF_MASK);
	I2C0->S |= I2C_S_IICIF_MASK;

	// Send register address
	I2C0->D = (TMP102.tmp_reg_address << 1) | WRITE;

	// Wait for acknowledge by Slave
	while(I2C0->S & I2C_S_IICIF_MASK);
	I2C0->S |= I2C_S_IICIF_MASK;

	// Sent repeated start
	I2C0->C1 |= I2C_C1_RSTA_MASK;

	// Initiate read operation
	I2C0->D = (TMP102.address << 1) | READ;

	// Wait for acknowledge by Slave
	while(I2C0->S & I2C_S_IICIF_MASK);
	I2C0->S |= I2C_S_IICIF_MASK;

	// Change to Receiver mode
	I2C0->C1 |= I2C_C1_TX(0);

	// Send ACK
	I2C0->C1 |= I2C_C1_TXAK(1);

	// Do a dummy read
	read = I2C0->D;

	// Wait for data
	while(I2C0->S & I2C_S_IICIF_MASK);
	I2C0->S |= I2C_S_IICIF_MASK;

	// Read data
	read = I2C0->D;
	data = read << 8;

	// Send NACK
	I2C0->C1 |= I2C_C1_TXAK(0);

	// Wait for data
	while(I2C0->S & I2C_S_IICIF_MASK);
	I2C0->S |= I2C_S_IICIF_MASK;

	// Send stop signal
	I2C0->C1 |= I2C_C1_MST(0);

	// Read data
	read = I2C0->D;
	data |= read;

	return data;
}
