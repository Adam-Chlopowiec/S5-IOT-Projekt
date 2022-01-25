/*
 * komendy_at.h  
 *
 *  Created on: 13-03-2012
 *      Author: Miros�aw Karda�
 */

#ifndef KOMENDY_AT_H_
#define KOMENDY_AT_H_

// definicja typu strukturalnego
typedef struct {
	char polecenie_at[8];
	int8_t (* at_service)(uint8_t inout, char * params);
} const TATCMD;


// deklaracje zmiennych zewn�trznych
extern TATCMD polecenia_at[] PROGMEM;
extern uint8_t ir_enable;


// deklaracje funkcji
void parse_uart_data( char * pBuf );

int8_t at_service(uint8_t inout, char * params);					//AT OK
int8_t ati_service(uint8_t inout, char * params);					//name of device
int8_t at_str_service(uint8_t inout, char * params);				//= text to display
int8_t at_def_service(uint8_t inout, char * params);				//= new defined characters
int8_t at_btm_service(uint8_t inout, char * params);				//=/? bluetooth settings
int8_t at_cls_service(uint8_t inout, char * params);				//clear screen
int8_t at_rst_service(uint8_t inout, char * params);				//reset microcontroler
int8_t at_help_service(uint8_t inout, char * params);				//displays a list of commands
int8_t at_spd_service(uint8_t inout, char * params);				//= sets a speed of displaying words
int8_t at_set_service(uint8_t inout, char * params);				//= sets an actual bufor area

#endif /* KOMENDY_AT_H_ */
