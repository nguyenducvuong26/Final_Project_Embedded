#include "MPU6050.h"

void I2C_Init() {		
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
  	Init_GPIO(PB,6,OUT_50,O_AF_OD);
	Init_GPIO(PB,7,OUT_50,O_AF_OD);
		
  	I2C1->CR2 = 36;   // He so chia tan so I2C (APB1_CLK = 36 MHz)
  	I2C1->CCR = 180;  // Toc do truyen du lieu tr�n bus I2C (100 kHz)
  	I2C1->TRISE = 37; 
  	I2C1->CR1 |= I2C_CR1_PE;  // B?t I2C1
}

void i2c_Start() {
	I2C1->CR1 |= (1<<10);  // Enable the ACK
	I2C1->CR1 |= (1<<8);  // Generate START
	while (!(I2C1->SR1 & (1<<0)));  // Wait fror SB bit to set
}
	
void I2C_Write(uint8_t data) {
	while (!(I2C1->SR1 & (1<<7)));  // wait for TXE bit to set
	I2C1->DR = data;
	while (!(I2C1->SR1 & (1<<2)));  // wait for BTF bit to set
}

void I2C_Address(uint8_t Address) {
	I2C1->DR = Address;  //  send the address
	while (!(I2C1->SR1 & (1<<1)));  // wait for ADDR bit to set
	uint8_t temp = I2C1->SR1 | I2C1->SR2;  // read SR1 and SR2 to clear the ADDR bit
}
	
void I2C_Stop(void) {
	I2C1->CR1 |= (1<<9);  // Stop I2C
}

void I2C_Read(uint8_t Address, uint8_t *buffer, uint8_t size) {
	int remaining = size;
	
	if (size == 1) {

		I2C1->DR = Address;  //  send the address
		while (!(I2C1->SR1 & (1<<1)));  // wait for ADDR bit to set
		
		I2C1->CR1 &= ~(1<<10);  // clear the ACK bit 
		uint8_t temp = I2C1->SR1 | I2C1->SR2;  // read SR1 and SR2 to clear the ADDR bit.... EV6 condition
		I2C1->CR1 |= (1<<9);  // Stop I2C

		while (!(I2C1->SR1 & (1<<6)));  // wait for RxNE to set
		
		buffer[size-remaining] = I2C1->DR;  // Read the data from the DATA REGISTER
		
	} else {
		I2C1->DR = Address;  //  send the address
		while (!(I2C1->SR1 & (1<<1)));  // wait for ADDR bit to set
		
		uint8_t temp = I2C1->SR1 | I2C1->SR2;  // read SR1 and SR2 to clear the ADDR bit
		
		while (remaining>2) {
			while (!(I2C1->SR1 & (1<<6)));  // wait for RxNE to set
			
			buffer[size-remaining] = I2C1->DR;  // copy the data into the buffer			
			
			I2C1->CR1 |= 1<<10;  // Set the ACK bit to Acknowledge the data received
			
			remaining--;
		}
		
		// Read the SECOND LAST BYTE
		while (!(I2C1->SR1 & (1<<6)));  // wait for RxNE to set
		buffer[size-remaining] = I2C1->DR;
		
		I2C1->CR1 &= ~(1<<10);  // clear the ACK bit 
		
		I2C1->CR1 |= (1<<9);  // Stop I2C
		
		remaining--;
		
		// Read the Last BYTE
		while (!(I2C1->SR1 & (1<<6)));  // wait for RxNE to set
		buffer[size-remaining] = I2C1->DR;  // copy the data into the buffer
	}	
	
}


void MPU_Write(uint8_t Address, uint8_t Reg, uint8_t Data) {
	i2c_Start();
	I2C_Address(Address);
	I2C_Write(Reg);
	I2C_Write(Data);
	I2C_Stop();
}

void MPU_Read(uint8_t Address, uint8_t Reg, uint8_t *buffer, uint8_t size) {
	i2c_Start();
	I2C_Address(Address);
	I2C_Write(Reg);
	i2c_Start();  // repeated start
	I2C_Read(Address+0x01, buffer, size);
	I2C_Stop();
}

void MPU6050_Init(void) {
	uint8_t check;
	uint8_t Data;

	// check device ID WHO_AM_I

	MPU_Read(MPU6050_ADDR,WHO_AM_I_REG, &check, 1);

	if (check == 104) {  // 0x68 will be returned by the sensor if everything goes well
		// power management register 0X6B we should write all 0's to wake the sensor up
		Data = 0;
		MPU_Write (MPU6050_ADDR, PWR_MGMT_1_REG, Data);

		// Set DATA RATE of 1KHz by writing SMPLRT_DIV register
		Data = 0x07;
		MPU_Write(MPU6050_ADDR, SMPLRT_DIV_REG, Data);

		// Set accelerometer configuration in ACCEL_CONFIG Register
		// XA_ST=0,YA_ST=0,ZA_ST=0, FS_SEL=0 -> ? 2g
		Data = 0x00;
		MPU_Write(MPU6050_ADDR, ACCEL_CONFIG_REG, Data);

		// Set Gyroscopic configuration in GYRO_CONFIG Register
		// XG_ST=0,YG_ST=0,ZG_ST=0, FS_SEL=0 -> ? 250 ?/s
		Data = 0x00;
		MPU_Write(MPU6050_ADDR, GYRO_CONFIG_REG, Data);
	}

}

void MPU6050_Read_Accel(float *Ax, float *Ay, float *Az) {
	uint8_t Rx_data[6];
	
	int16_t Accel_X_RAW = 0;
	int16_t Accel_Y_RAW = 0;
	int16_t Accel_Z_RAW = 0;

	// Read 6 BYTES of data starting from ACCEL_XOUT_H register

	MPU_Read(MPU6050_ADDR, ACCEL_XOUT_H_REG, Rx_data, 6);

	Accel_X_RAW = (int16_t)(Rx_data[0] << 8 | Rx_data [1]);
	Accel_Y_RAW = (int16_t)(Rx_data[2] << 8 | Rx_data [3]);
	Accel_Z_RAW = (int16_t)(Rx_data[4] << 8 | Rx_data [5]);

	
	*Ax	= (Accel_X_RAW)/16384.0;
	*Ay = (Accel_Y_RAW)/16384.0;
	*Az = (Accel_Z_RAW)/16384.0;
}


