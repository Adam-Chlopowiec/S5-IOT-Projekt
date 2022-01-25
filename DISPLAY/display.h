/*
 * display.h
 *
 *  Created on: 21-08-2016
 *      Author: Aleksy
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <avr/interrupt.h>
#include <avr/eeprom.h>

#include "../FONTS/fonts.h"

#define FUNKCJE_POMOCNICZE 0

#define DISPLAY_BUF_SIZE 128			//DISPLAY_BUF_SIZE = 2^n  &&  n < 9
#define DISPLAY_BUF_MASK ( DISPLAY_BUF_SIZE - 1)

#define ADDED_FONTS_NUMBER 16
#define ADDED_FONTS_MAX_WIDTH 6

// typ strukturalny TCFG jest strukturą na potrzeby pamięci EEPROM
typedef struct
{
	volatile uint8_t displayBuf[DISPLAY_BUF_SIZE];	//główny bufor wywietlacza
	volatile uint8_t bufIdx;						//indeks miejsca w buforze, które jest wywietlane
	volatile uint8_t bufTail;						//indeks miejsca w buforze, w którym kończy się !istotny! zapis
} TCFG;

#if FUNKCJE_POMOCNICZE == 1
void ledOn(uint8_t nr);				//funkcja ledOn służy do włączenia diody o okrelonym numerze -> nieistotny przy wywietlaniu napisów
#endif

void displayInit();										//inicjalizacja wywietlacza, timery porty
void string2buf(char * s, uint8_t x);					//zapis stringu do bufora
void displayCls();										//czyszczenie całego wyświetlacza
void displayClear(uint8_t startIdx, uint8_t len);		//wyczyść bufor w określonym przedziale
void changeAscii(uint8_t number, uint8_t newAscii);		//zmień przypisany znak ascii znakowi innych znaków
void defchar(uint8_t number, uint8_t * sign);			//zmień grafikę znaku


#endif /* DISPLAY_H_ */
