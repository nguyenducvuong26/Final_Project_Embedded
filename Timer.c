#include "Timer.h"

void TIM2Config(void) {
	RCC->APB1ENR |= (1<<0); // enable clock for TIM2 
  	TIM2->CNT = 0;			// Manually reset CNT
	TIM2->ARR = 0xffff-1;  	// ARR value
	TIM2->PSC = 72-1;      	// Prescalar value
	TIM2->CR1 |= (1<<0);  	// enable timer
	TIM2->EGR |= (1<<0);    // Reset timer counter registers
}

void TIM2IRTConfig(void) {
	RCC->APB1ENR |= (1<<0);	// enable clock for TIM2 
	TIM2->CNT = 0;			// Manually reset CNT
	TIM2->ARR = 5000-1;     // ARR value
	TIM2->PSC = 7200-1;     // Prescalar value
	TIM2->DIER |= (1<<0);	// Enable timer interrupt generation
	
	NVIC_SetPriority(TIM2_IRQn, 1);
	NVIC_EnableIRQ(TIM2_IRQn);

	TIM2->SR &= ~(1<<0);
  	TIM2->EGR |= (1<<0);
  	TIM2->CR1 |= (1<<0); 
}


