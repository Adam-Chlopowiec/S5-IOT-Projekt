/*
 * display.c
 *
 *  Created on: 21-08-2016
 *      Author: Aleksy
 */
//display_new/DISPLAY/display.c

#include <avr/io.h>
#include <avr/eeprom.h>
#include <stdlib.h>

#include "../PINS/pins.h"
#include "display.h"
#include "../MK_USART/mkuart.h"

//-----------FUNKCKJE POMOCNICZE-----------
void pgm2buf(uint8_t * diody, uint8_t startIdx, uint8_t len);
uint8_t eem2buf(uint8_t * diody, uint8_t startIdx);
FONT ascii2font(char c);

void copy_ram_eem(void);
void copy_eem_ram(void);
void check_and_load_eeprom();

/* W pliku display.c istnieją dwie zmienne typu TCFG z przydomkami: volatile, gdyż używane będą w przerwaniach
 * oraz !static!, ponieważ będą widoczne !tylko! w tym pliku i dostęp do nich mają tylko funkcje z tego pliku, a więc !biblioteczne!
 */

static volatile TCFG ram_cfg;
static volatile TCFG eem_cfg EEMEM;

static volatile uint8_t addedFonts[ADDED_FONTS_NUMBER * ADDED_FONTS_MAX_WIDTH] EEMEM;
static volatile uint8_t addedAscii[ADDED_FONTS_NUMBER] EEMEM;

#if FUNKCJE_POMOCNICZE == 1
void ledOn(uint8_t nr)		//funkcja zapalająca !jedną! diodę o numerze od 0 do 39
{
	static uint8_t dioda;			//zmienna dioda jest statyczna i przechgowuje numer diody, który !jest! zapalona

	SET(pins[(dioda%5) + 8]);		//stan wysoki na katodzie zapalonej diody
	CLR(pins[(uint8_t)(dioda/5)]);	//stan niski na anodzie zapalonej diody		||		dzięki zmiennej dioda nie marnujemy czasu na gaszenie wszystkich diod

	CLR(pins[(nr%5) + 8]);			//stan niski na katodzie diody o numerze nr
	SET(pins[(uint8_t)(nr/5)]);		//stan wysoki na anodzie diody o numerze nr

	dioda = nr;						//teraz pali się dioda nr, więc taką zapamiętujemy
}
#endif

void displayInit()			//funkcja inicjalizująca bibliotekę wywietlacza
{
	TCCR2 |= (1<<WGM21);						//Timer2 przełącza diody w trybie CTC
	TCCR2 |= (1<<CS22)|(1<<CS21)|(1<<CS20); 	//prescaler = 1024;
	OCR2 = 3;									//dodatkowy podział przez 4
	TIMSK |= (1<<OCIE2);						//odpalamy przerwanie COMPARE MATCH f ~~ 2kHz	||	50Hz na diodę

	TCCR1B |= (1<<WGM12);						//Timer1 przełącza znaki w buforach w trybie CTC
	TCCR1B |= (1<<CS12)|(1<<CS10);				//prescaler = 1024
	OCR1A = 1953;								//dodatkowy podział przez 7812
	TIMSK |= (1<<OCIE1A);						//odpalamy przerwanie COMPARE MATCH T ~~ 1s

	for(uint8_t i=0; i<13; i++) OUT(pins[i]);	//anody i katody jako wyjcie
	for(uint8_t i=0; i<5; i++) SET(pins[i+8]);	//na katodach stan wysoki
	check_and_load_eeprom();					//wczytuje eeprom(więcej przy szczegółowym opisie funkcji)
}

void string2buf(char * s, uint8_t x)
{
	FONT font;			//zmienna lokalna typu FONT (width, offset)
	uint8_t bufIndex = x;
	s--;
	while(*(++s))			//dopóki nie skończy się string(nie nastąpi NULL)
	{
		if(*s < 127)	//jeśli nastąpi zwykły znak ascii
		{
			font = ascii2font(*s);										//font odczytany z tablicy deskryptorów
			if(font.width < 0 || font.offset < 0) continue;				//jeśli funkcja zwróciła ujemne wynik, takiego charu nie ma w katalogu, więc go nie wywietlamy
			pgm2buf((uint8_t *)fonts, x++, 1);							//zapisujemy do bufora znak spacji, zwróć uwagę na x++
			pgm2buf((uint8_t *)&fonts[font.offset], x, font.width);		//zapisujemy do bufora z FLASHAa znak, którego offset w tablicy fontów mamy z tablicy deskryptorów
			x += font.width;											// x = x + 1(spacja) + font.width
		}

		else			//jeśli to jeden ze znaków przez nas definiowalnych
		{
			for(uint8_t i=0; i< ADDED_FONTS_NUMBER; i++)
			{
				if(eeprom_read_byte((uint8_t *)&addedAscii[i]) == (uint8_t)*s)				//jeśli to ten addedAscii
				{
					pgm2buf((uint8_t *)fonts, x++, 1);										//zapisz spację
					x += eem2buf((uint8_t *)&addedFonts[i * ADDED_FONTS_MAX_WIDTH], x);		//x zwiększamy o długość tego znaku, który wczytujemy do bufora z EEPROMa
					break;			//przerywamy pętlę, bo znaleźliśmy znak ===> jeli znak się powtórzy, wyświetli ten, który jest pierwszy w tablicy
				}
			}
		}
	}
	ram_cfg.bufTail = x;			//ustawiamy koniec  istotnego napisu na wyliczony indeks
	ram_cfg.bufIdx = bufIndex;		//ustawiamy początek na przechowane w zmiennej bufIndex miejsce zapisu pierwszego znaku
	copy_ram_eem();					//kopiujemy cały bufor do eemproma
}
void pgm2buf(uint8_t * diody, uint8_t startIdx, uint8_t len)
{
	if(startIdx > DISPLAY_BUF_MASK || len > DISPLAY_BUF_SIZE ) return;

	for(uint8_t i=startIdx; len; i = (i+1) & DISPLAY_BUF_MASK, len--)
		ram_cfg.displayBuf[i] = pgm_read_byte(diody++);
}

uint8_t eem2buf(uint8_t * diody, uint8_t startIdx)
{
	uint8_t d=0, len=ADDED_FONTS_MAX_WIDTH;
	if(startIdx > DISPLAY_BUF_MASK || len > DISPLAY_BUF_SIZE ) return 0;

	for(uint8_t i=startIdx; len; i = (i+1) & DISPLAY_BUF_MASK, len--)
	{
		d = eeprom_read_byte(diody++);
		if(d) ram_cfg.displayBuf[i] = d;
		else break;
	}
	return ADDED_FONTS_MAX_WIDTH - len;
}

FONT ascii2font(char c)				//funkcja zwracająca strukturę typu FONT odpowiadającą znakowi c(w tablicy deskryptorów)
{
	FONT font = {-1,-1};
	if(c > 31 && c < 97) c -= 32;
	else if(c > 96 && c < 123) c -= 64;
	else if(c > 122 && c < 127) c -= 58;
	else if(c == 163) c = 101;
	else if(c == 165) c = 102;
	else if(c == 202) c = 103;
	else c = 255;

	if(c != 255)		//jeli znaleziono taki znak w katalogu tablicy deskryptorów
	{
		font.width = pgm_read_byte(&descriptors[(uint8_t)c].width);
		font.offset = pgm_read_word(&descriptors[(uint8_t)c].offset);
	}

	return font;		//!!!ACHTUNG!!! jeli nie ma takiego znaku w tablicy deskryptorów funkcja zwraca {-1,-1}
}

void copy_eem_ram( void ) {eeprom_read_block( (uint8_t *)&ram_cfg, (uint8_t *)&eem_cfg, sizeof(ram_cfg) ); }
void copy_ram_eem( void ) {eeprom_write_block( (uint8_t *)&ram_cfg, (uint8_t *)&eem_cfg, sizeof(ram_cfg) ); }

void check_and_load_eeprom()
{
	uint8_t i, len = sizeof( ram_cfg );
	uint8_t * ram_wsk = (uint8_t*)&ram_cfg;

	copy_eem_ram();
	for(i=0; i<len; i++)
	{
		if( 0xff == *ram_wsk++ ) continue;
		break;
	}
	if( i == len ) displayCls();
}

void changeAscii(uint8_t number, uint8_t newAscii)
{
	eeprom_write_byte((uint8_t *)&addedAscii[number], newAscii);
	uart_putint(eeprom_read_byte((uint8_t *)addedAscii), 10);
}

void defchar(uint8_t number, uint8_t * sign)
{
	eeprom_write_block(sign, (uint8_t *)&addedFonts[number], ADDED_FONTS_MAX_WIDTH);
}

void displayCls()
{
	displayClear(0,DISPLAY_BUF_SIZE);
}

void displayClear(uint8_t startIdx, uint8_t len)
{
	if(startIdx > DISPLAY_BUF_MASK || len > DISPLAY_BUF_SIZE ) return;

	for(uint8_t i=startIdx; len; i = (i+1) & DISPLAY_BUF_MASK, len--)
		ram_cfg.displayBuf[i] = 0;

	copy_ram_eem();
}

ISR(TIMER1_COMPA_vect)
{
	ram_cfg.bufIdx++;
	if(ram_cfg.bufIdx > DISPLAY_BUF_MASK) ram_cfg.bufIdx=0;
	if(ram_cfg.bufIdx > ram_cfg.bufTail) ram_cfg.bufIdx= 0;
}

ISR(TIMER2_COMP_vect)			//procedura obsługi przerwania COMAPRE MATCH Timer2
{
	static uint8_t counter;						//statyczny licznik przechowuje, którą diodę zapalamy
	uint8_t poprzednia = (counter + 39)%40;		//zmienna poprzednia przechowuje wartoć poprzednio ewentualnie zapalonej diody

	SET(pins[ (poprzednia%5) + 8 ]);			//stan wysoki na katodzie i
	CLR(pins[(uint8_t)(poprzednia/5)]);			//stan niski na anodzie poprzedniej diody

	if(ram_cfg.displayBuf[(ram_cfg.bufIdx + counter%5) & DISPLAY_BUF_MASK] & (1<<(uint8_t)(counter/5)))	//jeśli w odpowiedniu miejsca bufora jest 1 to
	{
		CLR(pins[(counter%5) + 8]);							//ustawiamy stan niski na katodzie i
		SET(pins[(uint8_t)(counter/5)]);					//stan wysoki na anodzie kolejnej diody
	}

	if(++counter > 39) counter = 0;			//zwiększamy counter 0,1,2...38,39,0
}
