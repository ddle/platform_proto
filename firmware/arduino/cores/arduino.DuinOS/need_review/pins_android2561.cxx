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

  $Id: pins_arduino.c 254 2007-04-20 23:17:38Z mellis $
*/

#ifndef _AVR_IO_H_
	#include <avr/io.h>
#endif
//#include "wiring_private.h"
#ifndef Wiring_h
	#include "wiring.h"
#endif
#ifndef Pins_Arduino_h
	#include "pins_arduino.h"
#endif

#undef PE

#define PA 1
#define PB 2
#define PC 3
#define PD 4
#define PE 5
#define PF 6
#define PG 7

// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)
const uint16_t PROGMEM port_to_mode_PGM[] =
{
	NOT_A_PORT,
	&DDRA,
	&DDRB,
	&DDRC,
	&DDRD,
	&DDRE,
	&DDRF,
	&DDRG,
};

const uint16_t PROGMEM port_to_output_PGM[] =
{
	NOT_A_PORT,
	&PORTA,
	&PORTB,
	&PORTC,
	&PORTD,
	&PORTE,
	&PORTF,
	&PORTG,
};

const uint16_t PROGMEM port_to_input_PGM[] =
{
	NOT_A_PIN,
	&PINA,
	&PINB,
	&PINC,
	&PIND,
	&PINE,
	&PINF,
	&PING,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] =
{
	PF,	 /* 0 */
	PF,
	PF,
	PF,
	PF,
	PF,
	PF,
	PF,

	PE,		//*	PE 0 8
	PE,		//*	PE 1 
	PE,		//*	PE 2 
	PE,		//*	PE 3 
	PE,		//*	PE 4 
	PE,		//*	PE 5 
	PE,		//*	PE 6 
	PE,		//*	PE 7 

	PB,	 	/* 16 */
	PB,
	PB,
	PB,
	PB,
	PB,
	PB,
	PB,
	
	
	PD, 	//* PD 0	24	
	PD,
	PD,
	PD,
	PD,
	PD,
	PD,
	PD,

	PC,		//*	PC 0	32
	PC,
	PC,
	PC,
	PC,
	PC,
	PC,
	PC,

	PA,		//* PA 0	40
	PA,		//* PA 1	41
	PA,		//* PA 2	42
	PA,		//* PA 3	43
	PA,
	PA,
	PA,
	PA,

	PG,	 /* 48 */
	PG,
	PG,
	PG,
	PG,
	PG,
	PG,
	PG,
	
	

};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] =
{
	_BV(0), /* 0, port F */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),

	_BV(0), /* 8, port E */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),

	_BV(0), /* 16, port B */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),

	_BV(0), /* 24, port D */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),

	_BV(0), /* 32, port C */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),

	_BV(0), /* 40, port A */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),

	_BV(0), /* 48, port G */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),

};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] =
{
	NOT_ON_TIMER,	/* 0	- PF0 */
	NOT_ON_TIMER,	/* 1	- PF1 */
	NOT_ON_TIMER,	/* 2	- PF2 */
	NOT_ON_TIMER,	/* 3	- PF3 */
	NOT_ON_TIMER,	/* 4	- PF4 */
	NOT_ON_TIMER,	/* 5	- PF5 */
	NOT_ON_TIMER,	/* 6	- PF6 */
	NOT_ON_TIMER,	/* 7	- PF7 */
	
	NOT_ON_TIMER,	/* 8	- PE0 */
	NOT_ON_TIMER,	/* 9	- PE1 */
	NOT_ON_TIMER,	/* 10	- PE2 */
	TIMER3A,		/* 11	- PE3 */
	TIMER3B,		/* 12	- PE4 */
	TIMER3C,		/* 13	- PE5 */
	NOT_ON_TIMER,	/* 14	- PE6 */
	NOT_ON_TIMER,	/* 15	- PE7 */
	
	NOT_ON_TIMER,	/* 16	- PB0 */
	NOT_ON_TIMER,	/* 17	- PB1 */
	NOT_ON_TIMER,	/* 18	- PB2 */
	NOT_ON_TIMER,	/* 19	- PB3 */
	TIMER2A,		/* 20	- PB4 */
	TIMER1A,		/* 21	- PB5 */
	TIMER1B,		/* 22	- PB6 */
	TIMER0A,		/* 23	- PB7 */
	
	NOT_ON_TIMER,	/* 24	- PD0 */
	NOT_ON_TIMER,	/* 25	- PD1 */
	NOT_ON_TIMER,	/* 26	- PD2 */
	NOT_ON_TIMER,	/* 27	- PD3 */
	NOT_ON_TIMER,	/* 28	- PD4 */
	NOT_ON_TIMER,	/* 29	- PD5 */
	NOT_ON_TIMER,	/* 30	- PD6 */
	NOT_ON_TIMER,	/* 31	- PD7 */
	
	NOT_ON_TIMER,	/* 32	- PC0 */
	NOT_ON_TIMER,	/* 33	- PC1 */
	NOT_ON_TIMER,	/* 34	- PC2 */
	NOT_ON_TIMER,	/* 35	- PC3 */
	NOT_ON_TIMER,	/* 36	- PC4 */
	NOT_ON_TIMER,	/* 37	- PC5 */
	NOT_ON_TIMER,	/* 38	- PC6 */
	NOT_ON_TIMER,	/* 39	- PC7 */
	
	NOT_ON_TIMER,	/* 40	- PA0 */
	NOT_ON_TIMER,	/* 41	- PA1 */
	NOT_ON_TIMER,	/* 42	- PA2 */
	NOT_ON_TIMER,	/* 43	- PA3 */
	NOT_ON_TIMER,	/* 44	- PA4 */
	NOT_ON_TIMER,	/* 45	- PA5 */
	NOT_ON_TIMER,	/* 46	- PA6 */
	NOT_ON_TIMER,	/* 47	- PA7 */
	
	NOT_ON_TIMER,	/* 48	- PG0 */
	NOT_ON_TIMER,	/* 49	- PG1 */
	NOT_ON_TIMER,	/* 50	- PG2 */
	NOT_ON_TIMER,	/* 51	- PG3 */
	NOT_ON_TIMER,	/* 52	- PG4 */
	TIMER0B,		/* 53	- PG5 */
	NOT_ON_TIMER,	/* 54	- PG6 */
	NOT_ON_TIMER,	/* 55	- PG7 */

};
