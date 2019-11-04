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
	I2C0->F |= I2C_F_MULT(0x01);
	I2C0->F	|= I2C_F_ICR(0x3D);
	I2C0->C1 |= I2C_C1_IICEN_MASK; // | I2C_C1_IICIE_MASK;
	I2C0->C2 |= I2C_C2_HDRS_MASK;
    I2C0->SLTH |= I2C_SLTL_SSLT(0xFF);
	return 1;
}

uint8_t I2C_Check_Connect(void)
{

}

uint16_t I2C_Read(uint8_t register_address)
{
	uint16_t data = 0;
	uint8_t read;

	// Send start bit
	I2C0->C1 |= I2C_C1_TX_MASK;
	I2C0->C1 |= I2C_C1_MST_MASK;

	// Send slave address
	I2C0->D = (TMP102.address << 1) | WRITE;

	// Wait for acknowledge by Slave
	while((I2C0->S & I2C_S_IICIF_MASK) == 0){ }
	I2C0->S |= I2C_S_IICIF_MASK;

	// Send register address
	I2C0->D = register_address;

	// Wait for acknowledge by Slave
	while((I2C0->S & I2C_S_IICIF_MASK) == 0){ }
	I2C0->S |= I2C_S_IICIF_MASK;

	// Sent repeated start
	I2C0->C1 |= I2C_C1_RSTA_MASK;

	// Initiate read operation
	I2C0->D = (TMP102.address << 1) | READ;

	// Wait for acknowledge by Slave
	while((I2C0->S & I2C_S_IICIF_MASK) == 0){ }
	I2C0->S |= I2C_S_IICIF_MASK;

	// Change to Receiver mode
	I2C0->C1 &= ~I2C_C1_TX_MASK;

	// Send ACK
	I2C0->C1 &= ~I2C_C1_TXAK_MASK;

	// Do a dummy read
	read = I2C0->D;

	// Wait for data
	while((I2C0->S & I2C_S_IICIF_MASK) == 0){ }
	I2C0->S |= I2C_S_IICIF_MASK;

	// Read data
	read = I2C0->D;
	if(register_address == TMP102.tmp_reg_address)
		data = read << 4;
	else
		data = read << 8;
	logger.Log_Write("%x.\n\r", read);

	// Wait for data
	while((I2C0->S & I2C_S_IICIF_MASK) == 0){ }
	I2C0->S |= I2C_S_IICIF_MASK;

	//Send NACK
	I2C0->C1 |= I2C_C1_TXAK_MASK;

	// Read data
	read = I2C0->D;
	if(register_address == TMP102.tmp_reg_address)
		data = read >> 4;
	else
		data |= read;
	logger.Log_Write("%x\n\r", read);

	// Send stop signal
	I2C0->C1 &= ~I2C_C1_MST_MASK;
	I2C0->S |= I2C_S_IICIF_MASK;

	return data;
}

void I2C_Write(uint8_t register_address, uint8_t byte1, uint8_t byte2)
{
	// Send start bit
	I2C0->C1 |= I2C_C1_TX_MASK;
	I2C0->C1 |= I2C_C1_MST_MASK;

	// Send slave address
	I2C0->D = (TMP102.address << 1) | WRITE;

	// Wait for acknowledge by Slave
	while((I2C0->S & I2C_S_IICIF_MASK) == 0){ }
	I2C0->S |= I2C_S_IICIF_MASK;

	// Send register address
	I2C0->D = register_address;

	// Wait for acknowledge by Slave
	while((I2C0->S & I2C_S_IICIF_MASK) == 0){ }
	I2C0->S |= I2C_S_IICIF_MASK;

	// Send slave address
	I2C0->D = byte1;

	// Wait for acknowledge by Slave
	while((I2C0->S & I2C_S_IICIF_MASK) == 0){ }
	I2C0->S |= I2C_S_IICIF_MASK;


	// Send slave address
	I2C0->D = byte2;

	// Wait for acknowledge by Slave
	while((I2C0->S & I2C_S_IICIF_MASK) == 0){ }
	I2C0->S |= I2C_S_IICIF_MASK;

	// Send stop signal
	I2C0->C1 &= ~I2C_C1_MST_MASK;
}
