/**
  * File Name 		- i2c.c
  * Description 	- contains functions related to I2C routines
  * Author			- Atharva Nandanwar
  * Tools			- GNU C Compiler / ARM Compiler Toolchain
  * Leveraged Code 	- KL25Z I2C Example by Dean
  * URL				-
  */

#include "i2c.h"

/*
 * Function - I2C_Init
 * Brief - Initializes I2C peripheral
 */
void I2C_Init(void)
{
	if(logger.Get_Log_Level() == lDebug)
	{
		logger.Log_Write(__func__, mDebug, "Starting I2C peripheral initialization");
	}
	// Enabling clock on Port C and I2C0 peripheral
	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTC_MASK;

	// Configuring Port C
	PORTC->PCR[8] |= PORT_PCR_MUX(2);
	PORTC->PCR[9] |= PORT_PCR_MUX(2);

	// Configuring I2C Peripheral
	I2C0->F |= I2C_F_MULT(0x00);
	I2C0->F	|= I2C_F_ICR(0x3D);
	I2C0->C1 |= I2C_C1_IICEN_MASK;
	I2C0->C2 |= I2C_C2_HDRS_MASK;
    I2C0->SLTH |= I2C_SLTL_SSLT(0x01);

    // Log Message
    logger.Log_Write(__func__, mStatus, "I2C peripheral initialized");
}

/*
 * Function - I2C_Alert_Init
 * Brief - Initializes Alert Pin for TMP102
 */
void I2C_Alert_Init(void)
{
	if(logger.Get_Log_Level() == lDebug)
	{
		logger.Log_Write(__func__, mDebug, "Starting TMP102 Alert Pin Initialization");
	}
	// Port A Pin 5 setup - GPIO, Rising Edge Interrupt, Pull Down
	PORTA->PCR[5] |= PORT_PCR_MUX(1) | PORT_PCR_IRQC(0x09) | PORT_PCR_PE_MASK;
	PORTA->PCR[5] &= ~PORT_PCR_PS_MASK;
	GPIOA->PDDR &= ~ALERT_PIN;
	NVIC_EnableIRQ(PORTA_IRQn);

    // Log Message
    logger.Log_Write(__func__, mStatus, "TMP102 Alert Pin Initialized");
}

/*
 * Function - I2C_Check
 * Brief - function to check for disconnect event
 * Return -
 * returns connection status
 */
uint8_t I2C_Check(void)
{
	uint16_t data = 0;
	volatile uint8_t read;

	// Set I2C as Transmitter mode
	I2C0->C1 |= I2C_C1_TX_MASK;

	// Send start bit
	I2C0->C1 |= I2C_C1_MST_MASK;

	// Send slave address with write
	I2C0->D = (TMP102.address << 1) | WRITE;

	volatile int i = 0;
	// Wait for ACK
	while((I2C0->S & I2C_S_IICIF_MASK) == 0)
	{
		i++;
		if(i >= 40000)
		{
			return DISCONNECTED;
			break;
		}
	}
	if((I2C0->S & I2C_S_IICIF_MASK) == 1)
		return DISCONNECTED;
	I2C0->S |= I2C_S_IICIF_MASK;

	// Send register address
	I2C0->D = TMP102.config_reg_address;

	// Wait for ACK
	while((I2C0->S & I2C_S_IICIF_MASK) == 0)
	{
		i++;
		if(i >= 40000)
		{
			return DISCONNECTED;
			break;
		}
	}
	if((I2C0->S & I2C_S_IICIF_MASK) == 1)
		return DISCONNECTED;

	// Send stop signal
	I2C0->C1 &= ~I2C_C1_MST_MASK;
	I2C0->S |= I2C_S_IICIF_MASK;


	return CONNECTED;
}

/*
 * Function - I2C_Read
 * Brief - Reads data from a given register address
 * Argument -
 * register_address -> input appropriate register address
 * Return -
 * returns 16-bit integer which has data from I2C device
 */
uint16_t I2C_Read(uint8_t register_address)
{
	if(logger.Get_Log_Level() == lDebug)
	{
		logger.Log_Write(__func__, mDebug, "Starting I2C Read Operation");
	}
	uint16_t data = 0;
	volatile uint8_t read;

	// Set I2C as Transmitter mode
	I2C0->C1 |= I2C_C1_TX_MASK;

	// Send start bit
	I2C0->C1 |= I2C_C1_MST_MASK;

	// Send slave address with write
	I2C0->D = (TMP102.address << 1) | WRITE;

	// Wait for ACK
	while((I2C0->S & I2C_S_IICIF_MASK) == 0);
	I2C0->S |= I2C_S_IICIF_MASK;

	// Send register address
	I2C0->D = register_address;

	// Wait for ACK
	while((I2C0->S & I2C_S_IICIF_MASK) == 0);
	I2C0->S |= I2C_S_IICIF_MASK;

	// Send repeated start
	I2C0->C1 |= I2C_C1_RSTA_MASK;

	// Send slave address with read
	I2C0->D = (TMP102.address << 1) | READ;

	// Wait for ACK
	while((I2C0->S & I2C_S_IICIF_MASK) == 0);
	I2C0->S |= I2C_S_IICIF_MASK;

	// Receiver Mode
	I2C0->C1 &= ~I2C_C1_TX_MASK;

	// Send ACK
	I2C0->C1 &= ~I2C_C1_TXAK_MASK;

	// Dummy read
	read = I2C0->D;

	// Wait for data
	while((I2C0->S & I2C_S_IICIF_MASK) == 0);
	I2C0->S |= I2C_S_IICIF_MASK;

	// Send ACK
	I2C0->C1 &= ~I2C_C1_TXAK_MASK;

	// Proper Read
	read = I2C0->D;
	data = read << 8;

	// Wait for data
	while((I2C0->S & I2C_S_IICIF_MASK) == 0);
	I2C0->S |= I2C_S_IICIF_MASK;

	// Send NACK
	I2C0->C1 |= I2C_C1_TXAK_MASK;

	// Proper Read
	read = I2C0->D;
	data |= read;

	// Wait for data
	while((I2C0->S & I2C_S_IICIF_MASK) == 0);
	I2C0->S |= I2C_S_IICIF_MASK;

	// Send stop signal
	I2C0->C1 &= ~I2C_C1_MST_MASK;
	I2C0->S |= I2C_S_IICIF_MASK;

	logger.Log_Write(__func__, mStatus, "I2C Read Operation Finished");

	return data;
}

/*
 * Function - I2C_Write
 * Brief - Writes data to a given register
 * Argument -
 * register_address -> appropriate register address
 * byte1 & byte2 -> data to write
 */
void I2C_Write(uint8_t register_address, uint8_t byte1, uint8_t byte2)
{
	if(logger.Get_Log_Level() == lDebug)
	{
		logger.Log_Write(__func__, mDebug, "Starting I2C Write Operation");
	}
	// Send start bit
	I2C0->C1 |= I2C_C1_TX_MASK;
	I2C0->C1 |= I2C_C1_MST_MASK;

	// Send slave address
	I2C0->D = (TMP102.address << 1) | WRITE;

	// Wait for ACK
	while((I2C0->S & I2C_S_IICIF_MASK) == 0){ }
	I2C0->S |= I2C_S_IICIF_MASK;

	// Send register address
	I2C0->D = register_address;

	// Wait for ACK
	while((I2C0->S & I2C_S_IICIF_MASK) == 0){ }
	I2C0->S |= I2C_S_IICIF_MASK;

	// Send first data byte
	I2C0->D = byte1;

	// Wait for ACK
	while((I2C0->S & I2C_S_IICIF_MASK) == 0){ }
	I2C0->S |= I2C_S_IICIF_MASK;


	// Send second data byte
	I2C0->D = byte2;

	// Wait for ACK
	while((I2C0->S & I2C_S_IICIF_MASK) == 0){ }
	I2C0->S |= I2C_S_IICIF_MASK;

	// Send stop signal
	I2C0->C1 &= ~I2C_C1_MST_MASK;
	logger.Log_Write(__func__, mStatus, "I2C Write Operation Finished");
}
