/*
 * komendy_at.c  
 *
 *  Created on: 13-03-2012
 *      Author: Miros�aw Karda�
 */
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>

#include "MK_USART/mkuart.h"
#include "DISPLAY/display.h"

#include "komendy_at.h"

#define AT_CNT 	10			// ilo� polece� AT


//----------- tablica z poleceniami AT i wska�nikami funkcji do ich obs�ugi --------------------
const TATCMD polecenia_at[AT_CNT] PROGMEM = {
	// { at_cmd } , { wska�nik do funkcji obs�ugi at },
		{"AT", 		at_service},			//oddaje "OK"
		{"AT+STR", 	at_str_service},		//zapisuje string do bufora
		{"ATI", 	ati_service},			//oddaje nazwę urządzenia
		{"AT+RST",	at_rst_service},		//resetuje uC
		{"AT+DEF",	at_def_service},		//pozwala zapisać znak do eepromu
		{"AT+BTM",	at_btm_service},		//pozwala skonfigurować BTM-222 przez uC
		{"AT+CLS",	at_cls_service},		//czyści bufor
		{"AT+HELP",	at_help_service},		//oddaje zbiór komend
		{"AT+SPD",	at_spd_service},		//pozwala ustawić prędkość przewijania się napisów
		{"AT+SET", 	at_set_service},		//pozwala ustawić znaki asoii konkretnych znaków dodatkowych
};



//----------------- funkcja do analizowania danych odebranych z UART ------------------------------
void parse_uart_data( char * pBuf ) {

	int8_t (*_at_srv)(uint8_t inout, char * data);

	char * cmd_wsk;
	char * reszta;
	uint8_t i=0, len;



	if ( strpbrk(pBuf, "=?"))	{
		// obs�uga polece� AT we/wy + parametry

		if ( strpbrk(pBuf, "="))	{
			// ustawienia uk�adu w postaci: AT+CMD=parametry

						cmd_wsk = strtok_r(pBuf, "=", &reszta);
						len = strlen(cmd_wsk);
						for(i=0;i<AT_CNT;i++) {
							if ( len && 0 == strncasecmp_P(cmd_wsk, polecenia_at[i].polecenie_at, len) ) {
								if( pgm_read_word(polecenia_at[i].polecenie_at) ) { // <--- UWAGA! w tek�cie ksi��ki zabrak�o pgm_read_word()
									_at_srv = (void *)pgm_read_word( &polecenia_at[i].at_service );
									if( _at_srv && ! _at_srv( 1, reszta ) ) uart_puts_P(PSTR("OK\r\n"));
									else uart_puts_P(PSTR("ERROR\r\n"));
								}
								break;
							}
						}


		} else {
			// zapytania do uk�adu w postaci: AT+CMD?

						cmd_wsk = strtok_r(pBuf, "?", &reszta);
						len = strlen(cmd_wsk);
						for(i=0;i<AT_CNT;i++) {
							if ( len && 0 == strncasecmp_P(cmd_wsk, polecenia_at[i].polecenie_at, len) ) {
								if( pgm_read_word(polecenia_at[i].polecenie_at) ) { // <--- UWAGA! w tek�cie ksi��ki zabrak�o pgm_read_word()
									_at_srv = (void *)pgm_read_word( &polecenia_at[i].at_service );
									if( _at_srv) {
										if( _at_srv( 0, reszta ) < 0 ) uart_puts_P(PSTR("ERROR\r\n"));
									}
								}
								uart_puts_P(PSTR("\r\n"));
								break;
							}
						}
		}

	} else {
		// obs�uga polece� AT bez parametr�w

		if( 0 == pBuf[0] ) uart_puts_P(PSTR("\r\n"));	// reakcja na znak CR lub CRLF z terminala
		else {
			for(i=0;i<AT_CNT;i++) {
				if ( 0 == strncasecmp_P(pBuf, polecenia_at[i].polecenie_at, strlen(pBuf)) ) {
					if( pgm_read_word(polecenia_at[i].polecenie_at) ) { // <--- UWAGA! w tek�cie ksi��ki zabrak�o pgm_read_word()
						_at_srv = (void *)pgm_read_word( &polecenia_at[i].at_service );
						if( _at_srv) _at_srv(2,0);
					}
					break;
				}
			}
		}
	}

	if( AT_CNT == i ) uart_puts_P(PSTR("ERROR\r\n"));
}



//----------------- obs�uga poszczeg�lnych komend AT ----------------------------------
int8_t at_service(uint8_t inout, char * params) {
	uart_puts_P(PSTR("OK\r\n"));
	return 0;
}

int8_t ati_service(uint8_t inout, char * params) {
	uart_puts_P(PSTR("Display 5x8 Aleksy Walczak\r\nuse AT+HELP\r\n"));
	return 0;
}

int8_t at_str_service(uint8_t inout, char * params)
{
	/* Formy:
	 * AT+STR
	 * AT+STR=Ala ma kota
	 * AT+STR=Ala ma kota,5
	 * AT+STR=5 ---> nic nie robi
	 * AT+STR?
	 */
	if(1 == inout)
	{
		string2buf(params ,0);
	}
	else if(2 == inout)
	{
		uart_puts_P(PSTR("AT+STR=text\r\n"));
	}
	return 0;
}

int8_t at_def_service(uint8_t inout, char * params)
{
	uint8_t tab[] = {0xf0, 0xff, 0x0f, 0x88, 0x08, 0x80};
	defchar(0, tab);
	uart_puts("S");
	return 0;
}

int8_t at_btm_service(uint8_t inout, char * params) {
	//baudrate
	//name
	//pin
	//ustawienia fabryczne

	uart_puts("OK\r\n");
	return 0;
}

int8_t at_cls_service(uint8_t inout, char * params)
{
	displayCls();
	uart_puts("OK\r\n");
	return 0;
}

int8_t at_rst_service(uint8_t inout, char * params) {
	uart_puts_P(PSTR("re"));

	//****** RESET UK�ADU NA POTRZEBY BOOTLOADERA (MkBootloader) ***********
	cli();			// wy��cz przerwania
	wdt_enable(0);  // ustaw watch-dog
	while(1);		// czekaj na RESET

	return 0;
}

int8_t at_spd_service(uint8_t inout, char * params)
{

	return 0;
}

int8_t at_set_service(uint8_t inout, char * params)
{
	return 0;
}

int8_t at_help_service(uint8_t inout, char * params)
{
	return 0;

}

