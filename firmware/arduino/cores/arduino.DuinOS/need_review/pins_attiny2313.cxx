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

  $Id: pins_arduino.c 522 2008-11-01 22:06:13Z mellis $
  
  modified for Atmel ATTiny2313 mcu by René Bohne
*/

#include <avr/io.h>
#include "wiring_private.h"
#include "pins_arduino.h"

// On the Arduino board, digital pins are also used
// for the analog output (software PWM).  Analog input
// pins are a separate set.

// ATMEL ATTINY2313
//
//                  +-\/-+
//            PA2  1|    |29  VCC
// RX   (D 0) PD0  2|    |19  PB7 (D 16)
// TX   (D 1) PD1  3|    |18  PB6 (D 15)
//      (D 2) PA1  4|    |17  PB5 (D 14)
//      (D 3) PA0  5|    |16  PB4 (D 13)
// INT0 (D 4) PD2  6|    |15  PB3 (D 12)
// INT1 (D 5) PD3  7|    |14  PB2 (D 11)
//      (D 6) PD4  8|    |13  PB1 (D 10)
//     *(D 7) PD5  9|    |12  PB0 (D 9)
//            GND 10|    |11  PD6 (D 8)
//                  +----+
//
// * indicates PWM port


#define PA 1
#define PB 2
#define PD 4

// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)
const uint16_t PROGMEM port_to_mode_PGM[] = {
	NOT_A_PORT,
	&DDRA,
	&DDRB,
	NOT_A_PORT,
	&DDRD,
};

const uint16_t PROGMEM port_to_output_PGM[] = {
	NOT_A_PORT,
	&PORTA,
	&PORTB,
	NOT_A_PORT,
	&PORTD,
};

const uint16_t PROGMEM port_to_input_PGM[] = {
	NOT_A_PORT,
	&PINA,
	&PINB,
	NOT_A_PORT,
	&PIND,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
	PD, /* 0 */
	PD,
	PA,
	PA,
	PD,
	PD,
	PD,
	PD,
	PD, /* 8 */
	PB,
	PB,
	PB,
	PB,
	PB,
	PB, /* 14 */
	PB,
	PB,
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
	_BV(0), /* 0 */
	_BV(1),
	_BV(1),
	_BV(0),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6), /* 8 */
	_BV(0),
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5), /* 14 */
	_BV(6),
	_BV(7),
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
	NOT_ON_TIMER, 
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	TIMER0B,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	TIMER0A,
	TIMER1A,
	TIMER1B,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
};

