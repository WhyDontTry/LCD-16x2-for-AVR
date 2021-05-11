/*
 * Lcd.h
 *
 * Created: 01.05.2021 21:26:37
 *  Author: Адам
 */ 

/*
	D7-D4 - PC7-PC4
	E - PC3
	RS - PC2

*/

#ifndef LCD_H_
#define LCD_H_

#define COMMAND 0
#define DATA 1

#define RSpin	PORTC2
#define Epin	PORTC3

#define LCD_D4	PORTC4
#define LCD_D5	PORTC5
#define LCD_D6	PORTC6
#define LCD_D7	PORTC7

#define LED_PORT PORTC
#define LED_DDR DDRC





void LCD_init(void);
void LCD_init_prot(void);
void LCD_Clear(void);
void SendHalfByte(unsigned char c);
void Send_Byte(unsigned char byte, uint8_t param);
void SetPos(unsigned char x, unsigned y);
void Send_String(char* mass);
unsigned char INVERT_BYTE(unsigned char);



#endif /* LCD_H_ */