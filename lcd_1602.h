#include "gpio_stm32.h"
#include "systick_time.h"
#include "stm32f10x.h"
#include "PCF8574_drive.h"

void lcd_i2c_data(char i2c, unsigned char data);
void lcd_i2c_cmd(char i2c, unsigned char data);
void lcd_i2c_init(char i2c);
void lcd_i2c_send(char i2c, char str[]);
void lcd_i2c_msg(char i2c, unsigned char line_1_2, unsigned char pos_0_16, char msg[]);

