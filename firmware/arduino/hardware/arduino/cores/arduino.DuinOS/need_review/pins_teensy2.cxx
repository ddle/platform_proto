/*
  pins_arduino.c - pin definitions for the Arduino board
  Part of Arduino / Wiring Lite

  Copyright (c) 2005 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  $Id: pins_arduino.c 565 2009-03-25 10:50:00Z dmellis $

  Modified 28-08-2009 for attiny84 R.Wiersma
  Modified 09-10-2009 for attiny45 A.Saporetti
*/

#ifndef _AVR_IO_H_
	#include <avr/io.h>
#endif
#ifndef Wiring_h
	#include "wiring.h"
#endif
#ifndef Pins_Arduino_h
	#include "pins_arduino.h"
#endif


#define PA 1
#define PB 2
#define PC 3
#define PD 4
#define PE 5
#define PF 6


// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing) tiny45 only port B 
const uint16_t PROGMEM port_to_mode_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	&DDRB,
	&DDRC,
	&DDRD,
	&DDRE,
	&DDRF,
};

const uint16_t PROGMEM port_to_output_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	&PORTB,
	&PORTC,
	&PORTD,
	&PORTE,
	&PORTF,
};

const uint16_t PROGMEM port_to_input_PGM[] = {
	NOT_A_PIN,
	NOT_A_PIN,
	&PINB,
	&PINC,
	&PIND,
	&PINE,
	&PINF,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
	PB,	//	0	PB0
	PB,	//	1	PB1
	PB,	//	2	PB2
	PB,	//	3	PB3
	PB,	//	4	PB7
	PD,	//	5	PD0
	PD,	//	6	PD1
	PD,	//	7	PD2
	PD,	//	8	PD3
	PC,	//	9	PC6
	PC,	//	10	PC7
	PD,	//	11	PD6	A10
	PD,	//	12	PD7	A9
	PB,	//	13	PB4	A8
	PB,	//	14	PB5	A7
	PB,	//	15	PB6	A6
	PF,	//	16	PF7	A5
	PF,	//	17	PF6 A4
	PF,	//	18	PF5 A3
	PF,	//	19	PF4 A2
	PF,	//	20	PF1 A1
	PF,	//	21	PF0 A0
	PD,	//	22	PD4	A11
	PD,	//	23	PD5
	PE,	//	24	PE6

};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
	_BV(0),	//	0	PB0
	_BV(1),	//	1	PB1
	_BV(2),	//	2	PB2
	_BV(3),	//	3	PB3
	_BV(7),	//	4	PB7
	_BV(0),	//	5	PD0
	_BV(1),	//	6	PD1
	_BV(2),	//	7	PD2
	_BV(3),	//	8	PD3
	_BV(6),	//	9	PC6
	_BV(7),	//	10	PC7
	_BV(6),	//	11	PD6	A10
	_BV(7),	//	12	PD7	A9
	_BV(4),	//	13	PB4	A8
	_BV(5),	//	14	PB5	A7
	_BV(6),	//	15	PB6	A6
	_BV(7),	//	16	PF7	A5
	_BV(6),	//	17	PF6 A4
	_BV(5),	//	18	PF5 A3
	_BV(4),	//	19	PF4 A2
	_BV(1),	//	20	PF1 A1
	_BV(0),	//	21	PF0 A0
	_BV(4),	//	22	PD4	A11
	_BV(5),	//	23	PD5
	_BV(6),	//	24	PE6

};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
	NOT_ON_TIMER,	//	0	PB0
	NOT_ON_TIMER,	//	1	PB1
	NOT_ON_TIMER,	//	2	PB2
	NOT_ON_TIMER,	//	3	PB3
	TIMER0A,		//	4	PB7
	TIMER0B,		//	5	PD0
	NOT_ON_TIMER,	//	6	PD1
	NOT_ON_TIMER,	//	7	PD2
	NOT_ON_TIMER,	//	8	PD3
	TIMER3A,		//	9	PC6
	TIMER4A,		//	10	PC7
	NOT_ON_TIMER,	//	11	PD6	A10
	TIMER4C,		//	12	PD7	A9		//	this should be 4D but its not defined
	NOT_ON_TIMER,	//	13	PB4	A8
	TIMER1A,		//	14	PB5	A7
	TIMER1B,		//	15	PB6	A6
	NOT_ON_TIMER,	//	16	PF7	A5
	NOT_ON_TIMER,	//	17	PF6 A4
	NOT_ON_TIMER,	//	18	PF5 A3
	NOT_ON_TIMER,	//	19	PF4 A2
	NOT_ON_TIMER,	//	20	PF1 A1
	NOT_ON_TIMER,	//	21	PF0 A0
	NOT_ON_TIMER,	//	22	PD4	A11
	NOT_ON_TIMER,	//	23	PD5
	NOT_ON_TIMER,	//	24	PE6

};
