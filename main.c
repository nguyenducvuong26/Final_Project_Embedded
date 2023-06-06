#include "stm32f10x.h"                  // Device header
#include "gpio_stm32.h"
#include "systick_time.h"
#include "PCF8574_drive.h"
#include "lcd_1602.h"
#include "i2c_drive.h"
#include <stdio.h>
#include <math.h>
#include "MPU6050.h"
#include "Timer.h"

void EXTI1_IRQHandler();
void EXTI0_IRQHandler();
void counterStep();
float accTotal,accValue,delta;
float Ax, Ay, Az;
int signal =0;
int switchState =0;
volatile float prevAcc =0;
volatile int steps =0;
void delay(int t);

int main(void){
	SystemInit();
	
	Init_GPIO(PC,13,OUT_50,O_GP_PP);
	Init_GPIO(PA,5,OUT_50,O_GP_PP);
	Init_GPIO(PA,0,IN,I_PP);
	Init_GPIO(PA,1,IN,I_PP);
	
	AFIO->EXTICR[0] &= ~0xF; // Activating the interrupts for the Port A PIN 0
	EXTI->IMR |= 0x1; // Enable the interrupt for EXTI0
	EXTI->RTSR |= 0x1; // Generate a system interrupt at the rising edge
	NVIC_EnableIRQ(EXTI0_IRQn); // Enable the global interrupt function
	
	AFIO->EXTICR[0] &= ~0xF0;
	EXTI->IMR |= 0x2;
	EXTI->RTSR |= 0x2;
	NVIC_EnableIRQ(EXTI1_IRQn);
	
	systick_init();
	TIM2IRTConfig();
	I2C_Init();
	
	lcd_i2c_init(1);
	MPU6050_Init();
	
	
	while (1) {
		char c[16];
		
		if (signal) {
			W_GP(PA,5,0);
     	 	MPU6050_Read_Accel(&Ax, &Ay, &Az);
      		counterStep();
			Delay_ms(100);
		} else {
			W_GP(PA,5,1);
			W_GP(PC,13,1);
		} 
		
		sprintf(c, "%d", steps);
		Delay_ms(100);
		lcd_i2c_msg(1,1,0,"Buoc chan: ");
		lcd_i2c_msg(1,1,11,c);
	}
}

void EXTI0_IRQHandler() { // Interrupt Handler function for Port A pin 0
	EXTI->PR |=1;
	if (signal)
		signal = 0;
	else
		signal = 1;
}

void EXTI1_IRQHandler() {
	EXTI->PR |=1;
	steps = 0;
}

void TIM2_IRQHandler(void) {
	TIM2->SR &= ~(1u<<0);
	
	if (signal) GPIOC->ODR ^= GPIO_ODR_ODR13;
}

void counterStep() {
	MPU6050_Read_Accel(&Ax, &Ay, &Az);

	accTotal = sqrt((Ax * Ax) + (Ay * Ay) + (Az * Az));	
	
	delta = accTotal - prevAcc;
	
	prevAcc = accTotal;
	
	if (delta > 0.15 && delta < 0.9)
		steps++;
} 