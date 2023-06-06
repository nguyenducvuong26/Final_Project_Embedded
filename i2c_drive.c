#include "i2c_drive.h"
#include "gpio_stm32.h"

/*
I2C2
PB10 -> SCL
PB11 -> SDA

I2C1
PB6 -> SCL
PB7 -> SDA

*/

void i2c_init(char i2c,unsigned short speed_mode) {
	RCC->APB2ENR |= 1; 
	
	if (i2c == 1) {
		RCC->APB1ENR |= 0x200000;
		// Pin enable 
		Init_GPIO(PB,6,OUT_50,O_AF_OD);
		Init_GPIO(PB,7,OUT_50,O_AF_OD);
		I2C1->CR1 |= 0x8000;
		I2C1->CR1 &= ~0x8000;
		I2C1->CR2 =0x8;
		I2C1->CCR = speed_mode;
		I2C1->TRISE = 0x9;
		I2C1->CR1 |= 1;
	} else if (i2c == 2) {
		RCC->APB1ENR |= 0x400000;
		// Pin enable 
		Init_GPIO(PB,10,OUT_50,O_AF_OD);
		Init_GPIO(PB,11,OUT_50,O_AF_OD);
		I2C2->CR1 |= 0x8000;
		I2C2->CR1 &= ~0x8000;
		I2C2->CR2 =0x8;
		I2C2->CCR = speed_mode;
		I2C2->TRISE = 0x9;
		I2C2->CR1 |= 1;
	}
}

// Start step
void i2c_start(char i2c) {
	if (i2c == 1) {
		I2C1->CR1 |= 0x100;
		while (!(I2C1->SR1 & 1)) {};
	} else if (i2c == 2) {
		I2C2->CR1 |= 0x100;
		while (!(I2C2->SR1 & 1)) {};
	}
}

// Sending the address + R or Write	
void i2c_add(char i2c, char address,char RW) {
	volatile int tmp;
	if (i2c == 1) {
		I2C1->DR = (address|RW);
		while ((I2C1->SR1 & 2)==0) {};
		while ((I2C1->SR1 & 2)) {
			tmp = I2C1->SR1;
			tmp = I2C1->SR2;
			
			if ((I2C1->SR1 & 2) == 0) break;
		}
	} else if (i2c == 2) {
		I2C2->DR = (address|RW);
		while ((I2C2->SR1 & 2)==0) {};
		while ((I2C2->SR1 & 2)) {
			tmp = I2C2->SR1;
			tmp = I2C2->SR2;
			if((I2C2->SR1 & 2) == 0) break;
		}
	}
}

// Sending data step
void i2c_data(char i2c,char data) {
	if (i2c == 1) {
		while ((I2C1->SR1 & 0x80) == 0) {}
		I2C1->DR = data;
		while ((I2C1->SR1 & 0x80) == 0) {}
	}
	else if (i2c == 2) {
		while ((I2C2->SR1 & 0x80) == 0) {}
		I2C2->DR = data;
		while ((I2C2->SR1 & 0x80) == 0) {}
	}
}

// Stop step
void i2c_stop(char i2c) {
	I2C1->CR1 |= 0x200;
}