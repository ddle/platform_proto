#ifndef lib4LCD_h
#define lib4LCD_h

#include <string.h>
#include "WProgram.h"

// Commands (Uncustomizable)			// DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0	//FLAGS

#define CMD_FUNCTIONSET 0x20			//  0	0   1   DL  N   F   *	*	interface data lenght( DL) ;  number of lines (N) ; font size (F)

#define CMD_ENTRYMODESET 0x04			//  0	0   0   0   0   1  I/D	S	cursor move direction (I/D)  ;  display shift (S)

#define CMD_DISPLAYCONTROL 0x08			//  0	0   0   0   1   D   C	B	display on/off (D) ;  cursor on/off (C)  ; blink on/off (B)

#define CMD_CLEARDISPLAY 0x01			//  0	0   0   0   0   0   0	1       

#define CMD_RETURNHOME 0x02			//  0	0   0   0   0   0   1	0	

#define CMD_SETDDRAMADDR 0x80			//  1  ADD ADD ADD ADD ADD ADD ADD 	ADD=DDRAM address and corresponds to cursor address 
 
#define CMD_SETCGRAMADDR 0x40			//  0   1  ACG ACG ACG ACG ACG ACG	ACG=CGRAM address


// Flags for function set command (Customizable)
#define FOURBITMODE 0x00			// DL=0 (interface data lenght = 4 bits)
#define TWOLINE 0x08				// N=1 (numbers of lines=2)
#define SINGLELINE 0x00				// N=0 (numbers of lines=1)
#define FONT5X8 0x00				// F=0 (font size=5x8)

// Flags for display entry mode command (Customizable)
#define ENTRYLEFT 0x02				// I/D=1 (increment cursor position by 1 after print a char on lcd)
#define ENTRYSHIFTDECREMENT 0x00		// S=0 (no display shift)

// Flags for display on/off control command (Customizable)
#define DISPLAYON 0x04				// D=1
#define CURSOROFF 0x00				// C=0
#define BLINKOFF 0x00				// B=0

class lib4LCD{

public:
  lib4LCD(int , int , int , int ,int , int , int , int );
  void print(char []);
  void print(char);
  void clear();
  void setCursor(int , int);
  void home();
  void newCustomChar(int , uint8_t[]) ;
  void printCustomChar (int);
  
private:
  void init(int , int ,int , int , int , int);   
  void begin(int, int);
  void sendCommand(uint8_t);
  void sendData(uint8_t);
  void push4Bits(uint8_t);
  void enablePinPulse();
  void write(char);


int rs_pin; 			// Selects registers. LOW: instruction register(command) ; HIGH: data register(data)
int enable_pin; 		// Starts data and command read/write
int data_pins[4];		// In 4-bit mode used only D4,D5,D6,D7
int nrows;			// Number of rows
int ncols;			// Number of columns

};

#endif



