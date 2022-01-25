/*
 * pins.h
 *
 *  Created on: 21.08.2016
 *      Author: Aleksy Walczak
 */

#ifndef PINS_H_
#define PINS_H_
//Wygenerowano dla mikrokontrolera MEGA8-P
//wskaźniki w pamięci FLASH

#include <avr/pgmspace.h>

//definicjenazw ścieżek jako kolejnych indeksów tablic
#define SS 0
#define A1 1
#define A7 2
#define A6 3
#define A5 4
#define A4 5
#define A3 6
#define A2 7
#define UART_TX 8
#define UART_RX 9
#define A0 10
#define K4 11
#define K3 12
#define K2 13
#define K1 14
#define K0 15

//deklaracje tablic przechowujących adresy rejestrów

extern const volatile uint8_t * const _DDR[] PROGMEM;
extern const volatile uint8_t * const _PORT[] PROGMEM;
extern const uint8_t _P[] PROGMEM;
extern const uint8_t pins[];

//funkcja ustawiająca stan wysoki w rejestrze wyjścia
inline void SET(uint8_t nr)
{
	*((uint8_t *)pgm_read_word(&_PORT[nr])) |= (1<<pgm_read_byte(&_P[nr]));
}

//funkcja ustawiająca stan niski w rejestrze wyjścia
inline void CLR(uint8_t nr)
{
	*((uint8_t *)pgm_read_word(&_PORT[nr])) &= ~(1<<pgm_read_byte(&_P[nr]));
}

//funkcja ustawiająca wyjście w rejestrze kierunku
inline void OUT(uint8_t nr)
{
	*((uint8_t *)pgm_read_word(&_DDR[nr])) |= (1<<pgm_read_byte(&_P[nr]));
}

#endif /* PINS_H_ */
