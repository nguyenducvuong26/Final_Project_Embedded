#include "lcd_1602.h"

// I2c adjust 4 wire data tx
void lcd_i2c_data(char i2c, unsigned char data) {
	PCF8574_tx_byte(i2c,0x09);
	Delay_us(10);
	PCF8574_tx_byte(i2c,0x0D);
	Delay_us(5);
	PCF8574_tx_byte(i2c, ((data & 0x00f0) | 0x0D));
	Delay_us(10);
	PCF8574_tx_byte(i2c, ((data & 0x00f0) | 0x09));
	Delay_us(20);
	PCF8574_tx_byte(i2c,0x0D);
	Delay_us(5);
	PCF8574_tx_byte(i2c, (((data << 4) & 0x00f0) | 0x0D));
	Delay_us(10);
	PCF8574_tx_byte(i2c, (((data << 4) & 0x00f0) | 0x09));
}

// I2c adjust 4 wire cmd tx
void lcd_i2c_cmd(char i2c, unsigned char data) {
	PCF8574_tx_byte(i2c,0x08);
	Delay_us(10);
	PCF8574_tx_byte(i2c,0x0C);
	Delay_us(5);
	PCF8574_tx_byte(i2c, ((data & 0x00f0) | 0x0C));
	Delay_us(10);
	PCF8574_tx_byte(i2c, ((data & 0x00f0) | 0x08));
	Delay_us(20);
	PCF8574_tx_byte(i2c,0x0C);
	Delay_us(5);
	PCF8574_tx_byte(i2c, (((data << 4) & 0x00f0) | 0x0C));
	Delay_us(10);
	PCF8574_tx_byte(i2c, (((data << 4) & 0x00f0) | 0x08));
}

// I2c adjust 4 wire init
void lcd_i2c_init(char i2c) {
  	i2c_init(i2c, i2c_FM);
	Delay_ms(20);
	PCF8574_tx_byte(i2c,0x08);
	Delay_us(10);
	
	PCF8574_tx_byte(i2c,0x0C);
	Delay_us(5); 
	PCF8574_tx_byte(i2c,0x3C);
	Delay_us(10);
	PCF8574_tx_byte(i2c,0x38);
	
	Delay_ms(10);
	PCF8574_tx_byte(i2c,0x08);
	Delay_us(10);
	PCF8574_tx_byte(i2c,0x0C);
	Delay_us(5);
	PCF8574_tx_byte(i2c,0x3C);
	Delay_us(10);
	PCF8574_tx_byte(i2c,0x38);
	
	Delay_ms(1);
	
	PCF8574_tx_byte(i2c,0x08);
	Delay_us(10);
	PCF8574_tx_byte(i2c,0x0C);
	Delay_us(5);
	PCF8574_tx_byte(i2c,0x3C);
	Delay_us(10);
	PCF8574_tx_byte(i2c,0x38);
	
	Delay_ms(1);
	
	PCF8574_tx_byte(i2c,0x08);
	Delay_us(10);
	PCF8574_tx_byte(i2c,0x0C);
	Delay_us(5);
	PCF8574_tx_byte(i2c,0x2C);
	Delay_us(10);
	PCF8574_tx_byte(i2c,0x28);
	
	lcd_i2c_cmd(i2c,0x2C); // 4 bit communication mode / 2 lines
	Delay_ms(5);
	lcd_i2c_cmd(i2c,0x0C); // Display ON
	Delay_ms(5);
	lcd_i2c_cmd(i2c,0x01); // Clear Display
	Delay_ms(5);
	lcd_i2c_cmd(i2c,0x02); // Get back to initial address
	Delay_ms(5);
}

void lcd_i2c_send(char i2c, char str[]) {
	int i = 0;
	while (str[i]) {
		lcd_i2c_data(i2c,str[i]);
		i++;
		Delay_us(100);
	}
}

void lcd_i2c_msg(char i2c, unsigned char line_1_2, unsigned char pos_0_16, char msg[]) {
	short pos = 0;
	if (line_1_2 == 1) {
		pos = 0;
	} else if (line_1_2 == 2) {
		pos = 0x40;
	}
	
	lcd_i2c_cmd(i2c, 0x80 + pos + pos_0_16);
	Delay_us(100);
	lcd_i2c_send(i2c, msg);
}