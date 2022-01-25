/*
 * main.c
 *
 *  Created on: 21-08-2016
 *      Author: Aleksy
 */
//display_new

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>

#include "DISPLAY/display.h"
#include "PINS/pins.h"
#include "MK_USART/mkuart.h"
#include "komendy_at.h"

//TO DO:
//komentarze
//komendy at dokończyć
//zapisywanie kilku napisów w jednym buforze i przełączanie obszarów
//zapisywanie w dowolym miejscu didplayBuf; x != 4
//tryb nie-textowy
//kilka podstawowych znaków w eepromie

char bufor[100];

int main(void)
{

	displayInit();
	USART_Init(__UBRR);
	register_uart_str_rx_event_callback(parse_uart_data);
	sei();

	uart_puts_P(PSTR("start..."));
	changeAscii(0, 241);
	while(1)
	{
		UART_RX_STR_EVENT(bufor);
	}
}
