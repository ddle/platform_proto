:::::::::::::::
::: LIB4LCD :::
:::::::::::::::

**AUTHOR** 

Stefano Manni (http://www.stefanomanni.it/arduino/lib4lcd)

**INTRODUCTION**

Lib4LCD is a simple but functional library allows an Arduino board to control LCDs based on the Hitachi HD44780 (or a compatible) chipset. The library works ONLY with 4 bit mode so uses only six pin on your Arduino (4 data lines in addition to the rs, and enable pin). 

**INSTALL**

After extracting archive move the entire contents in your libraries directory (tipically ~/hardware/libraries).

**CIRCUIT**

See example.

**FUNCTIONS**

- lib4LCD(int numCols, int numRows, int rs, int enable,int d4, int d5, int d6, int d7): Contructor
	Parameters:
	+ rs: the number of the Arduino pin that is connected to the RS pin on the LCD
	+ enable: the number of the Arduino pin that is connected to the enable pin on the LCD
	+ d4, d5, d6, d7: the numbers of the Arduino pins that are connected to the corresponding data pins on the LCD

- print(char or string): Prints a single char or a string to LCD.

- clear(): Clears the LCD screen and positions the cursor in the upper-left corner. 

- setCursor(int col, int row): Sets cursor position. First column/row is identified by 0.

- home(): Positions the cursor in the upper-left corner (0,0).

- newCustomChar (int location, uint8_t[] pattern): fill one of the eight location in CGRAM with a custom char.
	Parameters:
	+ location: indicates location in CGRAM and must be a number between 0 and 7 
	+ pattern: 5x8 pattern of custom char. See example called "heart" for clarification.
	
- printCustomChar (int location): print the custom char stored in location on the LCD.
