/*
 * Lcd.c
 *
 * Created: 01.05.2021 21:25:52
 *  Author: Адам
 */ 

#define F_CPU 8000000UL // для функции задержки

#include <avr/io.h>

#include <avr/interrupt.h>

#include <util/delay.h>
#include <stdint.h>

#include "Lcd.h"



void LCD_init(void)	// инициализация для железа

{
	LED_DDR = (1<<RSpin)|(1<<Epin)|(1<<LCD_D4)|(1<<LCD_D5)|(1<<LCD_D6)|(1<<LCD_D7);	// все пины на выход
	LED_PORT &= ~((1<<RSpin)|(1<<Epin)|(1<<LCD_D4)|(LCD_D5)|(1<<LCD_D6)|(1<<LCD_D7));	// на всех пинах лог 0
	_delay_ms(40); //Ждем 40 мс
	
	Send_Byte(0b00000011, COMMAND);
	_delay_ms(5);
	Send_Byte(0b00000011, COMMAND);
	_delay_ms(5);
	Send_Byte(0b00000011, COMMAND);// Установка дисплея в начальное положение. Возврат курсора.
	_delay_ms(5);
	Send_Byte(0b00101000, COMMAND);
	_delay_ms(5);
	Send_Byte(0b00101000, COMMAND);// Параметры интерфейса: Разрядность 4 бита. 2 строки. Размер шрифта 5x7.
	_delay_ms(5);
	Send_Byte(0b00001000, COMMAND);// Выключение дисплея
	_delay_ms(5);
	Send_Byte(0b00000001, COMMAND);// Стирание дисплея
	_delay_ms(5);
	Send_Byte(0b00010000, COMMAND);// Запрет сдвига изображения
	_delay_ms(5);
	Send_Byte(0b00000110, COMMAND);// Автосмещение курсора вправо, после записи
	_delay_ms(5);
	Send_Byte(0b00000010, COMMAND);// Установка дисплея в начальное положение. Возврат курсора
	_delay_ms(5);
	Send_Byte(0b00101000, COMMAND);// Параметры интерфейса: Разрядность 4 бита. 2 строки. Размер шрифта 5x7.
	_delay_ms(5);
	Send_Byte(0b00001100, COMMAND);// Включение дисплея. Запрет видимости курсора. Запрет мерцания курсора.
	_delay_ms(5);
	
	
}

void SendHalfByte(unsigned char c)

{
	LED_PORT |= (1<<Epin); //формируем импульс на E
	_delay_us(50);
	c = (0b00001111&c); // сбрасываем старшие 4 разряда
	c <<= 4;

	//_delay_us(50);
	LED_PORT &= (0b00001111);	//сбрасываем пред. тетраду
	LED_PORT |= c;	// устанавливаем тетраду
	
	LED_PORT &= ~(1<<Epin);	// лог 0 на E

	_delay_us(50);
}

void Send_Byte(unsigned char byte, uint8_t param)		// param - COMMAND(0) or DATA(1)
{
	uint8_t data = 0;
	if(param==0) {
		LED_PORT &= ~(1<<RSpin);
	}
	else {
		LED_PORT |= (1<<RSpin);
	}
	
	//byte = INVERT_BYTE(byte);	//это я не правильно понял порядок байт
	
	data = (byte>>4);
	SendHalfByte(data);
	data = byte;
	SendHalfByte(data);

}

void Send_String(char* mass)
{
	char ch;
	while(*mass) {
		ch = *mass++;
		Send_Byte(ch, DATA);
	}
}



unsigned char INVERT_BYTE(unsigned char a) // инвертирует последовательность битов в байте
{
	  a = (((a&1)<<7) | ((a&2)<<5) | ((a&4)<<3) | ((a&8)<<1) | ((a&16)>>1) | ((a&32)>>3) | ((a&64)>>5) | ((a&128)>>7));
	  return a;   
}


void LCD_init_prot(void) // инициализация для proteus
{
	LED_DDR = (1<<RSpin)|(1<<Epin)|(1<<LCD_D4)|(1<<LCD_D5)|(1<<LCD_D6)|(1<<LCD_D7);	// все пины на выход
	LED_PORT &= ~((1<<RSpin)|(1<<Epin)|(1<<LCD_D4)|(LCD_D5)|(1<<LCD_D6)|(1<<LCD_D7));	// на всех пинах лог 0

	Send_Byte(0b00000010, COMMAND);
	Send_Byte(0b00101000, COMMAND);
	Send_Byte(0b00001100, COMMAND);
	
}


void SetPos(unsigned char x, unsigned y) // Нужно доработать; x - номер строки; y - смещение
{
	char adress;
	if(x==1){
		adress=(0x00+y)|0b10000000;
	} else if(x==2){
		adress=(0x40+y)|0b10000000;
	} else {
		adress=(0x00)|0b10000000;
	}
	Send_Byte(adress, COMMAND);
}


void LCD_Clear(void)
{
	Send_Byte(0b00000001, COMMAND);
	_delay_us(1500);
}
