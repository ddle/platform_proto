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
#define PH 8
#define PJ 10
#define PK 11
#define PL 12

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
	&DDRH,
	NOT_A_PORT,
	&DDRJ,
	&DDRK,
	&DDRL,
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
	&PORTH,
	NOT_A_PORT,
	&PORTJ,
	&PORTK,
	&PORTL,
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
	&PINH,
	NOT_A_PIN,
	&PINJ,
	&PINK,
	&PINL,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] =
{
	PA,	 /* 0 */
	PA,
	PA,
	PA,
	PA,
	PA,
	PA,
	PA,

	PB,	 /* 8 */
	PB,
	PB,
	PB,
	PB,
	PB,
	PB,
	PB,
	
	PC,	 /* 16 */
	PC,
	PC,
	PC,
	PC,
	PC,
	PC,
	PC,

	PD, /* 24 */
	PD,
	PD,
	PD,
	PD,
	PD,
	PD,
	PD,

	PE,		//*	PE 0 32
	PE,		//*	PE 1 33
	PE,		//*	PE 2 34
	PE,		//*	PE 3 35
	PE,		//*	PE 4 36
	PE,		//*	PE 5 37
	PE,		//*	PE 6 38
	PE,		//*	PE 7 39

	PF,	 /* 40 */
	PF,
	PF,
	PF,
	PF,
	PF,
	PF,
	PF,
	
	PG,	 /* 48 */
	PG,
	PG,
	PG,
	PG,
	PG,
	PG,
	PG,
	
	PH,	 /* 56 */
	PH,
	PH,
	PH,
	PH,
	PH,
	PH,
	PH,
	
	PJ,	 /* 64 */
	PJ,
	PJ,
	PJ,
	PJ,
	PJ,
	PJ,
	PJ,
	
	PK,	 /* 72 */
	PK,
	PK,
	PK,
	PK,
	PK,
	PK,
	PK,
	
	PL,	 /* 80 */
	PL,
	PL,
	PL,
	PL,
	PL,
	PL,
	PL,
	

};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] =
{
	_BV(0), /* 0, port A */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),

	_BV(0), /* 8, port B */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),

	_BV(0), /* 16, port C */
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

	_BV(0), /* 32, port E */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),

	_BV(0), /* 40, port F */
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


	_BV(0), /* 56, port H */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),

	_BV(0), /* 64, port J */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),

	_BV(0), /* 72, port K */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),

	_BV(0), /* 80, port L */
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
	NOT_ON_TIMER,	/* 0	- PA0 */
	NOT_ON_TIMER,	/* 1	- PA1 */
	NOT_ON_TIMER,	/* 2	- PA2 */
	NOT_ON_TIMER,	/* 3	- PA3 */
	NOT_ON_TIMER,	/* 4	- PA4 */
	NOT_ON_TIMER,	/* 5	- PA5 */
	NOT_ON_TIMER,	/* 6	- PA6 */
	NOT_ON_TIMER,	/* 7	- PA7 */
	
	NOT_ON_TIMER,	/* 8	- PB0 */
	NOT_ON_TIMER,	/* 9	- PB1 */
	NOT_ON_TIMER,	/* 10	- PB2 */
	NOT_ON_TIMER,	/* 11	- PB3 */
	TIMER2A,		/* 12	- PB4 */
	TIMER1A,		/* 13	- PB5 */
	TIMER1B,		/* 14	- PB6 */
	TIMER0A,		/* 15	- PB7 */
	
	NOT_ON_TIMER,	/* 16	- PC0 */
	NOT_ON_TIMER,	/* 17	- PC1 */
	NOT_ON_TIMER,	/* 18	- PC2 */
	NOT_ON_TIMER,	/* 19	- PC3 */
	NOT_ON_TIMER,	/* 20	- PC4 */
	NOT_ON_TIMER,	/* 21	- PC5 */
	NOT_ON_TIMER,	/* 22	- PC6 */
	NOT_ON_TIMER,	/* 23	- PC7 */
	
	NOT_ON_TIMER,	/* 24	- PD0 */
	NOT_ON_TIMER,	/* 25	- PD1 */
	NOT_ON_TIMER,	/* 26	- PD2 */
	NOT_ON_TIMER,	/* 27	- PD3 */
	NOT_ON_TIMER,	/* 28	- PD4 */
	NOT_ON_TIMER,	/* 29	- PD5 */
	NOT_ON_TIMER,	/* 30	- PD6 */
	NOT_ON_TIMER,	/* 31	- PD7 */
	
	NOT_ON_TIMER,	/* 32	- PE0 */
	NOT_ON_TIMER,	/* 33	- PE1 */
	NOT_ON_TIMER,	/* 34	- PE2 */
	TIMER3A,		/* 35	- PE3 */
	TIMER3B,		/* 36	- PE4 */
	TIMER3C,		/* 37	- PE5 */
	NOT_ON_TIMER,	/* 38	- PE6 */
	NOT_ON_TIMER,	/* 39	- PE7 */
	
	NOT_ON_TIMER,	/* 40	- PF0 */
	NOT_ON_TIMER,	/* 41	- PF1 */
	NOT_ON_TIMER,	/* 42	- PF2 */
	NOT_ON_TIMER,	/* 43	- PF3 */
	NOT_ON_TIMER,	/* 44	- PF4 */
	NOT_ON_TIMER,	/* 45	- PF5 */
	NOT_ON_TIMER,	/* 46	- PF6 */
	NOT_ON_TIMER,	/* 47	- PF7 */
	
	NOT_ON_TIMER,	/* 48	- PG0 */
	NOT_ON_TIMER,	/* 49	- PG1 */
	NOT_ON_TIMER,	/* 50	- PG2 */
	NOT_ON_TIMER,	/* 51	- PG3 */
	NOT_ON_TIMER,	/* 52	- PG4 */
	TIMER0B,		/* 53	- PG5 */
	NOT_ON_TIMER,	/* 54	- PG6 */
	NOT_ON_TIMER,	/* 55	- PG7 */

	NOT_ON_TIMER,	/* 56	- PH0 */
	NOT_ON_TIMER,	/* 57	- PH1 */
	NOT_ON_TIMER,	/* 58	- PH2 */
	TIMER4A,		/* 59	- PH3 */
	TIMER4B,		/* 60	- PH4 */
	TIMER4C,		/* 61	- PH5 */
	TIMER2B,		/* 62	- PH6 */
	NOT_ON_TIMER,	/* 63	- PH7 */
	

	NOT_ON_TIMER,	/* 64	- PJ0 */
	NOT_ON_TIMER,	/* 65	- PJ1 */
	NOT_ON_TIMER,	/* 66	- PJ2 */
	NOT_ON_TIMER,	/* 67	- PJ3 */
	NOT_ON_TIMER,	/* 68	- PJ4 */
	NOT_ON_TIMER,	/* 69	- PJ5 */
	NOT_ON_TIMER,	/* 70	- PJ6 */
	NOT_ON_TIMER,	/* 71	- PJ7 */
	

	NOT_ON_TIMER,	/* 72	- PK0 */
	NOT_ON_TIMER,	/* 73	- PK1 */
	NOT_ON_TIMER,	/* 74	- PK2 */
	NOT_ON_TIMER,	/* 75	- PK3 */
	NOT_ON_TIMER,	/* 76	- PK4 */
	NOT_ON_TIMER,	/* 77	- PK5 */
	NOT_ON_TIMER,	/* 78	- PK6 */
	NOT_ON_TIMER,	/* 79	- PK7 */
	

	NOT_ON_TIMER,	/* 80	- PL0 */
	NOT_ON_TIMER,	/* 81	- PL1 */
	NOT_ON_TIMER,	/* 82	- PL2 */
	TIMER5A,		/* 83	- PL3 */
	TIMER5B,		/* 84	- PL4 */
	TIMER5C,		/* 85	- PL5 */
	NOT_ON_TIMER,	/* 86	- PL6 */
	NOT_ON_TIMER,	/* 87	- PL7 */
	
};
