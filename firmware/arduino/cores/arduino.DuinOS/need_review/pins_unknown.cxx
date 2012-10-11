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
//*******************************************************************************************
//*	Sep  5, 2010	<MLS> Added pins_unknown.cxx
//*******************************************************************************************
//*	This file will work for ANY avr atmega cpu that is current defined.
//*	it has everyting in port order, i.e. a0, a1, a2,... b0, b1, b2 b3...
//*	if a port is omitted from the particular cpu, that portion will be left out
//*******************************************************************************************

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

//--------------------
//*	Port A
#if defined(DDRA)
	&DDRA,
#else
	NOT_A_PORT,
#endif
//--------------------
//*	Port B
#if defined(DDRB)
	&DDRB,
#else
	NOT_A_PORT,
#endif
//--------------------
//*	Port C
#if defined(DDRC)
	&DDRC,
#else
	NOT_A_PORT,
#endif
//--------------------
//*	Port D
#if defined(DDRD)
	&DDRD,
#else
	NOT_A_PORT,
#endif
//--------------------
//*	Port E
#if defined(DDRE)
	&DDRE,
#else
	NOT_A_PORT,
#endif
//--------------------
//*	Port F
#if defined(DDRF)
	&DDRF,
#else
	NOT_A_PORT,
#endif
//--------------------
//*	Port G
#if defined(DDRG)
	&DDRG,
#else
	NOT_A_PORT,
#endif
//--------------------
//*	Port H
#if defined(DDRH)
	&DDRH,
#else
	NOT_A_PORT,
#endif
//--------------------
//*	Port I is not defined for any Atmega chip
	NOT_A_PORT,
//--------------------
//*	Port J
#if defined(DDRJ)
	&DDRJ,
#else
	NOT_A_PORT,
#endif
//--------------------
//*	Port K
#if defined(DDRK)
	&DDRK,
#else
	NOT_A_PORT,
#endif
//--------------------
//*	Port L
#if defined(DDRL)
	&DDRL,
#else
	NOT_A_PORT,
#endif

};

const uint16_t PROGMEM port_to_output_PGM[] =
{

	NOT_A_PORT,

//--------------------
//*	Port A
#if defined(PORTA)
	&PORTA,
#else
	NOT_A_PORT,
#endif
//--------------------
//*	Port B
#if defined(PORTB)
	&PORTB,
#else
	NOT_A_PORT,
#endif
//--------------------
//*	Port C
#if defined(PORTC)
	&PORTC,
#else
	NOT_A_PORT,
#endif
//--------------------
//*	Port D
#if defined(PORTD)
	&PORTD,
#else
	NOT_A_PORT,
#endif
//--------------------
//*	Port E
#if defined(PORTE)
	&PORTE,
#else
	NOT_A_PORT,
#endif
//--------------------
//*	Port F
#if defined(PORTF)
	&PORTF,
#else
	NOT_A_PORT,
#endif
//--------------------
//*	Port G
#if defined(PORTG)
	&PORTG,
#else
	NOT_A_PORT,
#endif
//--------------------
//*	Port H
#if defined(PORTH)
	&PORTH,
#else
	NOT_A_PORT,
#endif
//--------------------
//*	Port I is not defined for any Atmega chip
	NOT_A_PORT,
//--------------------
//*	Port J
#if defined(PORTJ)
	&PORTJ,
#else
	NOT_A_PORT,
#endif
//--------------------
//*	Port K
#if defined(PORTK)
	&PORTK,
#else
	NOT_A_PORT,
#endif
//--------------------
//*	Port L
#if defined(PORTL)
	&PORTL,
#else
	NOT_A_PORT,
#endif


};

const uint16_t PROGMEM port_to_input_PGM[] =
{
	NOT_A_PIN,
//--------------------
//*	Port A
#if defined(PINA)
	&PINA,
#else
	NOT_A_PORT,
#endif
//--------------------
//*	Port B
#if defined(PINB)
	&PINB,
#else
	NOT_A_PORT,
#endif
//--------------------
//*	Port C
#if defined(PINC)
	&PINC,
#else
	NOT_A_PORT,
#endif
//--------------------
//*	Port D
#if defined(PIND)
	&PIND,
#else
	NOT_A_PORT,
#endif
//--------------------
//*	Port E
#if defined(PINE)
	&PINE,
#else
	NOT_A_PORT,
#endif
//--------------------
//*	Port F
#if defined(PINF)
	&PINF,
#else
	NOT_A_PORT,
#endif
//--------------------
//*	Port G
#if defined(PING)
	&PING,
#else
	NOT_A_PORT,
#endif
//--------------------
//*	Port H
#if defined(PINH)
	&PINH,
#else
	NOT_A_PORT,
#endif
//--------------------
//*	Port I is not defined for any Atmega chip
	NOT_A_PIN,
//--------------------
//*	Port J
#if defined(PINJ)
	&PINJ,
#else
	NOT_A_PORT,
#endif
//--------------------
//*	Port K
#if defined(PINK)
	&PINK,
#else
	NOT_A_PORT,
#endif
//--------------------
//*	Port L
#if defined(PINL)
	&PINL,
#else
	NOT_A_PORT,
#endif
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] =
{
#if defined(PORTA)
	PA,	 /* 0 */
	PA,
	PA,
	PA,
	PA,
	PA,
	PA,
	PA,
#endif

#if defined(PORTB)
	PB,	 /* 8 */
	PB,
	PB,
	PB,
	PB,
	PB,
	PB,
	PB,
#endif
	
#if defined(PORTC)
	PC,	 /* 16 */
	PC,
	PC,
	PC,
	PC,
	PC,
	PC,
	PC,
#endif
	
#if defined(PORTD)
	PD, /* 24 */
	PD,
	PD,
	PD,
	PD,
	PD,
	PD,
	PD,
#endif
	
#if defined(PORTE)
	PE,		//*	PE 0 32
	PE,		//*	PE 1 33
	PE,		//*	PE 2 34
	PE,		//*	PE 3 35
	PE,		//*	PE 4 36
	PE,		//*	PE 5 37
	PE,		//*	PE 6 38
	PE,		//*	PE 7 39
#endif
	
#if defined(PORTF)
	PF,	 /* 40 */
	PF,
	PF,
	PF,
	PF,
	PF,
	PF,
	PF,
#endif
	
#if defined(PORTG)
	PG,	 /* 48 */
	PG,
	PG,
	PG,
	PG,
	PG,
	PG,
	PG,
#endif
	
#if defined(PORTH)
	PH,	 /* 56 */
	PH,
	PH,
	PH,
	PH,
	PH,
	PH,
	PH,
#endif
	
#if defined(PORTJ)
	PJ,	 /* 64 */
	PJ,
	PJ,
	PJ,
	PJ,
	PJ,
	PJ,
	PJ,
#endif
	
#if defined(PORTK)
	PK,	 /* 72 */
	PK,
	PK,
	PK,
	PK,
	PK,
	PK,
	PK,
#endif
	
#if defined(PORTL)
	PL,	 /* 80 */
	PL,
	PL,
	PL,
	PL,
	PL,
	PL,
	PL,
#endif

};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] =
{
#if defined(PORTA)
	_BV(0), /* 0, port A */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),
#endif

#if defined(PORTB)
	_BV(0), /* 8, port B */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),
#endif

#if defined(PORTC)
	_BV(0), /* 16, port C */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),
#endif

#if defined(PORTD)
	_BV(0), /* 24, port D */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),
#endif

#if defined(PORTE)
	_BV(0), /* 32, port E */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),
#endif

#if defined(PORTF)
	_BV(0), /* 40, port F */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),
#endif

#if defined(PORTG)
	_BV(0), /* 48, port G */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),
#endif

#if defined(PORTH)
	_BV(0), /* 56, port H */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),
#endif

#if defined(PORTJ)
	_BV(0), /* 64, port J */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),
#endif

#if defined(PORTK)
	_BV(0), /* 72, port K */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),
#endif

#if defined(PORTL)
	_BV(0), /* 80, port L */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),
#endif
};

//*******************************************************************************************
//*	as much as I would like to do this register based, I cant
#if defined(__AVR_ATmega644P__ )
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
	TIMER0A,		/* 11	- PB3 */
	TIMER0B,		/* 12	- PB4 */
	NOT_ON_TIMER,	/* 13	- PB5 */
	NOT_ON_TIMER,	/* 14	- PB6 */
	NOT_ON_TIMER,	/* 15	- PB7 */

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
	TIMER1B,		/* 28	- PD4 */
	TIMER1A,		/* 29	- PD5 */
	TIMER2B,		/* 30	- PD6 */
	TIMER2A,		/* 31	- PD7 */
	
};

#else
const uint8_t PROGMEM digital_pin_to_timer_PGM[] =
{
#if defined(PORTA)
	NOT_ON_TIMER,	/* 0	- PA0 */
	NOT_ON_TIMER,	/* 1	- PA1 */
	NOT_ON_TIMER,	/* 2	- PA2 */
	NOT_ON_TIMER,	/* 3	- PA3 */
	NOT_ON_TIMER,	/* 4	- PA4 */
	NOT_ON_TIMER,	/* 5	- PA5 */
	NOT_ON_TIMER,	/* 6	- PA6 */
	NOT_ON_TIMER,	/* 7	- PA7 */
#endif
	
#if defined(PORTB)
	NOT_ON_TIMER,	/* 8	- PB0 */
	NOT_ON_TIMER,	/* 9	- PB1 */
	NOT_ON_TIMER,	/* 10	- PB2 */
	NOT_ON_TIMER,	/* 11	- PB3 */
	TIMER2A,		/* 12	- PB4 */
	TIMER1A,		/* 13	- PB5 */
	TIMER1B,		/* 14	- PB6 */
	TIMER0A,		/* 15	- PB7 */
#endif
	
#if defined(PORTC)
	NOT_ON_TIMER,	/* 16	- PC0 */
	NOT_ON_TIMER,	/* 17	- PC1 */
	NOT_ON_TIMER,	/* 18	- PC2 */
	NOT_ON_TIMER,	/* 19	- PC3 */
	NOT_ON_TIMER,	/* 20	- PC4 */
	NOT_ON_TIMER,	/* 21	- PC5 */
	NOT_ON_TIMER,	/* 22	- PC6 */
	NOT_ON_TIMER,	/* 23	- PC7 */
#endif
	
#if defined(PORTD)
	NOT_ON_TIMER,	/* 24	- PD0 */
	NOT_ON_TIMER,	/* 25	- PD1 */
	NOT_ON_TIMER,	/* 26	- PD2 */
	NOT_ON_TIMER,	/* 27	- PD3 */
	NOT_ON_TIMER,	/* 28	- PD4 */
	NOT_ON_TIMER,	/* 29	- PD5 */
	NOT_ON_TIMER,	/* 30	- PD6 */
	NOT_ON_TIMER,	/* 31	- PD7 */
#endif
	
#if defined(PORTE)
	NOT_ON_TIMER,	/* 32	- PE0 */
	NOT_ON_TIMER,	/* 33	- PE1 */
	NOT_ON_TIMER,	/* 34	- PE2 */
	TIMER3A,		/* 35	- PE3 */
	TIMER3B,		/* 36	- PE4 */
	TIMER3C,		/* 37	- PE5 */
	NOT_ON_TIMER,	/* 38	- PE6 */
	NOT_ON_TIMER,	/* 39	- PE7 */
#endif
	
#if defined(PORTF)
	NOT_ON_TIMER,	/* 40	- PF0 */
	NOT_ON_TIMER,	/* 41	- PF1 */
	NOT_ON_TIMER,	/* 42	- PF2 */
	NOT_ON_TIMER,	/* 43	- PF3 */
	NOT_ON_TIMER,	/* 44	- PF4 */
	NOT_ON_TIMER,	/* 45	- PF5 */
	NOT_ON_TIMER,	/* 46	- PF6 */
	NOT_ON_TIMER,	/* 47	- PF7 */
#endif
	
#if defined(PORTG)
	NOT_ON_TIMER,	/* 48	- PG0 */
	NOT_ON_TIMER,	/* 49	- PG1 */
	NOT_ON_TIMER,	/* 50	- PG2 */
	NOT_ON_TIMER,	/* 51	- PG3 */
	NOT_ON_TIMER,	/* 52	- PG4 */
	TIMER0B,		/* 53	- PG5 */
	NOT_ON_TIMER,	/* 54	- PG6 */
	NOT_ON_TIMER,	/* 55	- PG7 */
#endif
	
#if defined(PORTH)
	NOT_ON_TIMER,	/* 56	- PH0 */
	NOT_ON_TIMER,	/* 57	- PH1 */
	NOT_ON_TIMER,	/* 58	- PH2 */
	TIMER4A,		/* 59	- PH3 */
	TIMER4B,		/* 60	- PH4 */
	TIMER4C,		/* 61	- PH5 */
	TIMER2B,		/* 62	- PH6 */
	NOT_ON_TIMER,	/* 63	- PH7 */
#endif
	
#if defined(PORTJ)
	NOT_ON_TIMER,	/* 64	- PJ0 */
	NOT_ON_TIMER,	/* 65	- PJ1 */
	NOT_ON_TIMER,	/* 66	- PJ2 */
	NOT_ON_TIMER,	/* 67	- PJ3 */
	NOT_ON_TIMER,	/* 68	- PJ4 */
	NOT_ON_TIMER,	/* 69	- PJ5 */
	NOT_ON_TIMER,	/* 70	- PJ6 */
	NOT_ON_TIMER,	/* 71	- PJ7 */
#endif
	
#if defined(PORTK)
	NOT_ON_TIMER,	/* 72	- PK0 */
	NOT_ON_TIMER,	/* 73	- PK1 */
	NOT_ON_TIMER,	/* 74	- PK2 */
	NOT_ON_TIMER,	/* 75	- PK3 */
	NOT_ON_TIMER,	/* 76	- PK4 */
	NOT_ON_TIMER,	/* 77	- PK5 */
	NOT_ON_TIMER,	/* 78	- PK6 */
	NOT_ON_TIMER,	/* 79	- PK7 */
#endif
	
#if defined(PORTL)
	NOT_ON_TIMER,	/* 80	- PL0 */
	NOT_ON_TIMER,	/* 81	- PL1 */
	NOT_ON_TIMER,	/* 82	- PL2 */
	TIMER5A,		/* 83	- PL3 */
	TIMER5B,		/* 84	- PL4 */
	TIMER5C,		/* 85	- PL5 */
	NOT_ON_TIMER,	/* 86	- PL6 */
	NOT_ON_TIMER,	/* 87	- PL7 */
#endif
	
};
#endif
