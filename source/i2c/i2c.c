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
    I2C0->SLTH |= I2C_SLTL_SSLT(0x01);
	return 1;
}

uint8_t I2C_Check_Connect(void)
{

}

uint16_t I2C_Read_From_Slave(void)
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
	I2C0->D = TMP102.tmp_reg_address;

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
	data = read << 4;

	// Wait for data
	while((I2C0->S & I2C_S_IICIF_MASK) == 0){ }
	I2C0->S |= I2C_S_IICIF_MASK;

	//Send NACK
	I2C0->C1 |= I2C_C1_TXAK_MASK;

	// Read data
	read = I2C0->D;
	data |= read >> 4;

	// Send stop signal
	I2C0->C1 &= ~I2C_C1_MST_MASK;

	return data;
}

/*
 * I2C Read Function - Abhijeet Srivastava
 int I2C_Read(void)
   {
       uint8_t data[3];
       uint16_t buffer = 0;
       I2C0->C1 |= I2C_C1_TX_MASK;                      // Transmit
       I2C0->C1 |= I2C_C1_MST_MASK;                     // Generate START SIGNAL

       //I2C0->D = ((SLAVE_ADDRESS << 1) | READ);         // Slave address with 8th bit low
       I2C0->D = 0x90;
       while((I2C0->S & I2C_S_IICIF_MASK)==0){}         // Wait for interrupt (1 interrupt pending/0 Not pending)
       I2C0->S |= I2C_S_IICIF_MASK;                     // cleared by writing 1 page 695 manual

       I2C0->D = 0x00;		                              // Temperature Register
       while((I2C0->S & I2C_S_IICIF_MASK)==0){}         // Wait for interrupt (1 interrupt pending/0 Not pending)
       I2C0->S |= I2C_S_IICIF_MASK;                     // cleared by writing 1 page 695 manual

       I2C0->C1 |= I2C_C1_RSTA_MASK;                    // Restart
       //I2C0->D = ((SLAVE_ADDRESS << 1) | WRITE);      // Slave address with 8th bit high
       I2C0->D = 0x91;
       while((I2C0->S & I2C_S_IICIF_MASK)==0){}         // Wait for interrupt (1 interrupt pending/0 Not pending)
       I2C0->S |= I2C_S_IICIF_MASK;                     // cleared by writing 1 page 695 manual

       I2C0->C1 &= ~I2C_C1_TX_MASK;                     // Set as receiver
       I2C0->C1 &= ~I2C_C1_TXAK_MASK;                   // ACK

        data[0] = I2C0->D;
        while((I2C0->S & I2C_S_IICIF_MASK)==0){}        // Wait for interrupt (1 interrupt pending/0 Not pending)
        I2C0->S |= I2C_S_IICIF_MASK;                    // cleared by writing 1 page 695 manual

        data[0] = I2C0->D;
        while((I2C0->S & I2C_S_IICIF_MASK)==0){}        // Wait for interrupt (1 interrupt pending/0 Not pending)
        I2C0->S |= I2C_S_IICIF_MASK;                    // cleared by writing 1 page 695 manual

        data[1] = I2C0->D;
        while((I2C0->S & I2C_S_IICIF_MASK)==0){}        // Wait for interrupt (1 interrupt pending/0 Not pending)
        I2C0->S |= I2C_S_IICIF_MASK;                    // cleared by writing 1 page 695 manual

        I2C0->C1 |= I2C_C1_TXAK_MASK;                       // NACK
        I2C0->C1 &= ~I2C_C1_MST_MASK;                   //MASTER STOP

        buffer = ((data[0]<<8) + (data[1]));

        return buffer;
   }
*/
