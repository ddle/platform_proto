#include "lib4LCD.h"

/*++++++++++++++++++ CONSTRUCTOR +++++++++++++++++++++++++*/
lib4LCD::lib4LCD(int numCols, int numRows, int rs, int enable,int d4, int d5, int d6, int d7){
	init(rs, enable, d4, d5, d6, d7);	
	begin(numCols, numRows);	 
}


/*++++++++++++++++++ INITIALIZE ++++++++++++++++++++++++++*/
void lib4LCD::init(int rs, int enable,int d4, int d5, int d6, int d7){

 	data_pins[0] = d4;
  	data_pins[1] = d5;
 	data_pins[2] = d6;
 	data_pins[3] = d7; 

	rs_pin = rs;
  	enable_pin = enable;
  	
  	for(int i=0; i<4 ;i++) {pinMode(data_pins[i], OUTPUT);}
  	pinMode(rs_pin, OUTPUT);
  	pinMode(enable_pin, OUTPUT);  	
}


void lib4LCD::begin(int numCols, int numRows){
	
	nrows = numRows;
	ncols = numCols;

	delayMicroseconds(50000); 				//we need at least 40ms after power rises above 2.7V
  
	// try to set 4 bit mode.  This is according to the hitachi HD44780 datasheet (figure 24, pag 46)
    	sendCommand(0x03);
	delayMicroseconds(4500); // wait at least 4.1ms

    	// second try
    	sendCommand(0x03);
    	delayMicroseconds(150); // wait at least 100us 
    
    	// finally third go!
   	sendCommand(0x03); 

    	// finally, set to 4-bit interface
    	sendCommand(0x02); 
  
 	//Function set: set data interface length (mode), number of lines, font size
  	uint8_t functionset = FOURBITMODE | SINGLELINE | FONT5X8;
  	if (nrows > 1) {functionset |= TWOLINE;}
  	sendCommand(CMD_FUNCTIONSET | functionset );  

  	//Display control: turn the display on with no cursor or blinking 
  	sendCommand(CMD_DISPLAYCONTROL | DISPLAYON | CURSOROFF | BLINKOFF  );

  	//Entry mode set: set default text direction (from left to right)
  	sendCommand(CMD_ENTRYMODESET | ENTRYLEFT | ENTRYSHIFTDECREMENT);
  	
  	clear();
}


/*+++++++++++++++ HIGH LEVEL COMMANDS FOR THE USER ++++++++++++*/

void lib4LCD::clear()
{
  sendCommand(CMD_CLEARDISPLAY);  // clear display, set cursor position to zero
  delayMicroseconds(2000);  	  // this command it's very slowly
}

void lib4LCD::home()
{
  sendCommand(CMD_RETURNHOME);  // clear display, set cursor position to zero
  delayMicroseconds(2000);  	  // this command it's very slowly
}

void lib4LCD::setCursor(int col, int row)
{
  int row_offsets[] = { 0x00, 0x40, 0x14, 0x54}; 	//offsets for first,second,third and fourth row
  if (row >= nrows)  {row = nrows-1;}    		// on the last row because we count rows starting w/0
  if (col >= ncols)  {col = ncols-1;}    		// on the last col because we count cols starting w/0
  sendCommand (CMD_SETDDRAMADDR | (col + row_offsets[row]));
}


void lib4LCD::newCustomChar(int location, uint8_t pattern[]) {	//Fill the first 8 CGRAM locations
  location &= 0x7; 	//if location>7 or location<0	
  sendCommand(CMD_SETCGRAMADDR | (location << 3));
  for (int i=0; i<8; i++) {
    sendData(pattern[i]);
  }
  sendCommand (CMD_SETDDRAMADDR);	//re-position DDRAM address counter in the current position
}


void lib4LCD::print(char string[]) {	//print an entire string on LCD
  int i;  
  for (i=0;i < strlen(string);i++){
    write(string[i]);
  }
}

void lib4LCD::print(char c) {		//overloading print for receving a single char as parameter
    write(c);
}


void lib4LCD::printCustomChar(int location){
   sendData(location);
}


void lib4LCD::write(char c) {		//print a single char on LCD
   sendData(c);
}



/*++++++++++++++++++++++ LOW LEVEL COMMANDS ++++++++++++++++++++*/

void lib4LCD::sendCommand(uint8_t value){

	digitalWrite(rs_pin, LOW);
	
	//first nibble
	push4Bits(value>>4);				//the data transfer is completed after the 4-bit data has been transferred twice		
	enablePinPulse();
	
	//second nibble
	push4Bits(value);						
	enablePinPulse();	
}

void lib4LCD::sendData(uint8_t value){

	digitalWrite(rs_pin, HIGH);			//the only difference with sendCommand routine	
	
	//first nibble
	push4Bits(value>>4);				//the data transfer is completed after the 4-bit data has been transferred twice		
	enablePinPulse();
	
	//second nibble
	push4Bits(value);						
	enablePinPulse();	
}

void lib4LCD::push4Bits(uint8_t value){			
	for (int i = 0; i < 4; i++) {
  		digitalWrite(data_pins[i], (value >> i) & 0x01);
	}
}

void lib4LCD::enablePinPulse (){ 
	digitalWrite(enable_pin,LOW);
	delayMicroseconds(1);
	digitalWrite(enable_pin,HIGH);
	delayMicroseconds(1);   			 // high level must be at least 450ns
	digitalWrite(enable_pin,LOW);
	delayMicroseconds(1);  				 // at least 37us to settle
}



