/*
 * pins.c
 *
 *  Created on: 21.08.2016
 *      Author: Aleksy Walczak
 */

#include <avr/io.h>
#include <avr/pgmspace.h>

#include "pins.h"

const volatile uint8_t * const _DDR[] PROGMEM =	//adresy rejestrów kierunku
{&DDRB, &DDRB, &DDRD, &DDRD, &DDRD, &DDRD, &DDRD, &DDRD, &DDRD, &DDRD, &DDRC, &DDRC, &DDRC, &DDRC, &DDRC, &DDRC};

const volatile uint8_t * const _PORT[] PROGMEM =	//adresy rejestrów wyjścia
{&PORTB, &PORTB, &PORTD, &PORTD, &PORTD, &PORTD, &PORTD, &PORTD, &PORTD, &PORTD, &PORTC, &PORTC, &PORTC, &PORTC, &PORTC, &PORTC};

const uint8_t _P[] PROGMEM =	//numery bitów w rejestrach
{2, 1, 7, 6, 5, 4, 3, 2, 1, 0, 5, 4, 3, 2, 1, 0};

const uint8_t pins[] =
{
		A0,A1,A2,A3,A4,A5,A6,A7,K0,K1,K2,K3,K4,SS
};
