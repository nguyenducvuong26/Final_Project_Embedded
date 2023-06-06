#include "systick_time.h"

void systick_init(void) {
	SysTick->CTRL = 0;
	SysTick->LOAD = 0x00FFFFFF;
	SysTick->VAL = 0;
	SysTick->CTRL = 5;
}

void Delaymicro(void) {
	SysTick->LOAD = 72;
	SysTick->VAL = 0;
	while ((SysTick->CTRL & 0x00010000) == 0);
}

void Delay_us(uint16_t us) {
	for (;us>0;us--) {
		Delaymicro();
	}
}

void DelayMillis(void) {
	SysTick->LOAD = 0x11940;
	SysTick->VAL = 0;
	while ((SysTick->CTRL & 0x00010000) == 0);
}

void Delay_ms(uint16_t ms) {
	for (;ms>0;ms--) {
		DelayMillis();
	}
}

