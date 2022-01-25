/*
 * fonts.h
 *
 *  Created on: 23-08-2016
 *      Author: Aleksy
 */

#ifndef FONTS_H_
#define FONTS_H_

typedef struct
{
	int8_t width;
	int16_t offset;
} FONT;

extern const FONT descriptors[] PROGMEM;
extern const uint8_t fonts[] PROGMEM;

#endif /* FONTS_H_ */
