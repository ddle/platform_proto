/*
 Lib4LCD - Serial Monitor
 
 Demonstrates the use a 20x4 LCD display.  
 This sketch displays text sent over the serial port 
 (e.g. from the IDE Serial Monitor) on LCD.
 
  The circuit:
 * LCD --------------------------------- ARDUINO
 *
 * Vss pin (1)                           GND
 * Vdd pin (2)                           5V
 * VO/Contrast pin (3)                   5V through a  linear potentiometer 
 * RS pin (4)                            digital pin 12
 * Enable pin (6)                        digital pin 11
 * D4 pin (11)                           digital pin 5
 * D5 pin (12)                           digital pin 4
 * D6 pin (13)                           digital pin 3
 * D7 pin (14)                           digital pin 2
 * R/W pin (5)                           GND
 * Backlight Led+ pin (15)                         5V
 * Backlight Led- pin (16)                         GND
 
 http://www.stefanomanni.it/arduino/lib4lcd
 
*/

#include <lib4LCD.h>

#include "WProgram.h"
void setup();
void loop();
int numR = 4; //number of rows
int numC = 20; //number of columns
int incomingByte = 0;	// for incoming serial data
int  curR=0;
int  curC=0;

lib4LCD lcd(numC,numR,12,11,5,4,3,2);  //constructor lib4LCD xxx(numCols, numRows, RS, Enable, D4, D5, D6, D7)

void setup(){
Serial.begin(115200);
}

void loop(){
 
  while (Serial.available() > 0) {
  
                incomingByte = Serial.read();      
          
                if (incomingByte == 10)  break;   //newline character
                
                lcd.setCursor(curC,curR);
                lcd.print(char(incomingByte));    
                curC++;  
             
                if (curC==20) {curC=0; curR++;}
                if (curR==4) {lcd.clear(); curR=0;}   
	}
  
}

int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}

