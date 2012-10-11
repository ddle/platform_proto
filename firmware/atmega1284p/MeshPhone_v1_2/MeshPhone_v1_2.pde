// ===============================================================================
// Meshphone Prototype I Firmware 
// by Le Dang Dung
// ===============================================================================
// Hardware:
// + atmega1284p 16MHz
// + RFM12 433/868MHz via SPI
// + 16x2 LCD parallel interface (using Hitachi HD44780 controller)
// + 4 press buttons for input 
// Fuse setting: FD D9 F6
//                           ATMega1284P
//                           +---\/---+
//        LED -> (D 0) PB0  1|        |40  PA0 (AI 0 / D31) <- RSSI
//               (D 1) PB1  2|        |39  PA1 (AI 1 / D30)
// (nIRQ)-> INT2 (D 2) PB2  3|        |38  PA2 (AI 2 / D29)
//               (D 3) PB3  4|        |37  PA3 (AI 3 / D28)
// (nSEL)<-   SS (D 4) PB4  5|        |36  PA4 (AI 4 / D27)
// (SDO) -> MOSI (D 5) PB5  6|        |35  PA5 (AI 5 / D26)
// (SDI) <- MISO (D 6) PB6  7|        |34  PA6 (AI 6 / D25)
// (SCK) <-> SCK (D 7) PB7  8|        |33  PA7 (AI 7 / D24)
//                     RST  9|        |32  AREF <---||---| GND
//      5V |-------->  VCC 10|        |31  GND      22p 
//            22p      GND 11|        |30  AVCC <--------| 5V
//      GND|--||---> XTAL2 12|        |29  PC7 (D 23)
//      GND|--||---> XTAL1 13|        |28  PC6 (D 22)
//          RX0 (D 8)  PD0 14|        |27  PC5 (D 21) 
//          TX0 (D 9)  PD1 15|        |26  PC4 (D 20) <- LCD data d7
//      key1 -> (D 10) PD2 16|        |25  PC3 (D 19) <- LCD data d6
//      key2 -> (D 11) PD3 17|        |24  PC2 (D 18) <- LCD data d5
//      key3 -> (D 12) PD4 18|        |23  PC1 (D 17) <- LCD data d4
//      key4 -> (D 13) PD5 19|        |22  PC0 (D 16) <- LCD enable
//       LED -> (D 14) PD6 20|        |21  PD7 (D 15) <- LCD rs
//                           +--------+
//
//                      +------------------+
//                 SDO 1|      -==-|->RSSI |8  nSEL
//                nIRQ 2|        (R)       |9  SCK
//                     3|                  |10 SDI
//                     4|      RFM12B      |11 
//                     5|                  |12 GND
//                     6|                  |13 3.3V
//                     7|                  |14 ANT
//                      +------------------+
// Note: 
// + analog RSSI output : on resistor
// + outputs from ATmega1284p (5V) voltage divided (~3V) before fetched to RFM12B
// + LCD contrast control via Vo (0~3.3V) using a potentiometer 
// =============================================================================
// Library:
// + Ported DuinOS for ATmega1284p (Sanguino & DuinOS v0.3)
// + Ported RF12 lib for ATmega1284p (Jeelabs 2009-02-09), add RSSI reading
// + lib4LCD (DevCorner http://www.stefanomanni.it/arduino/lib4lcd/)
// Note:
// + before compiling, under Tools->Board select Multiplo.Brain.M1284 + DuinOS
// + standard delay function (any kind) does not work and was redefined in 
//   DuinOs(see DuinOS.h). There 's also a delayMicroSeconds() that should be 
//   used with care.
// =============================================================================

#include <RF12.h>
#include <global.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <stdlib.h>
#include <lib4LCD.h>

#define ledPinRed 0
#define ledPinGreen 14
#define jumperRadio 1
//lib4LCD lcd(numCol,numRow,rs, enable, d4, d5, d6, d7);
lib4LCD lcd(16, 2, 15, 16, 17, 18, 19, 20);

unsigned char rssi;
uint8_t key_input[4] = {10,11,12,13};
uint8_t last_key[4] = {LOW,LOW,LOW,LOW};
boolean inputMode = false;
char currentText = 'a';
char currentMsg[8];
unsigned char currentMsgLength = 0;
char queue[8];
unsigned char queueLength = 0;
char recvString[16] = "";
unsigned char recvLength = 0;
boolean _dispSuspended = false;
boolean _dataInQueue = false;
void receiveMsg();
void sendMsg();

//========================== TASK DECLARATION  & DEFINITION =================================
declareTaskLoop(disp);
declareTaskLoop(keypad);

// HANDLE KEYPAD
taskLoop(keypad)
{  
  delay(50); // slow this task down, but fast enough to poll key press
  for (byte m = 0; m < 4 ; m++)
  {
    uint8_t d;
    d = digitalRead(key_input[m]);
    if (d != last_key[m]) 
    {
      if (d == HIGH) // a LOW-to-HIGH signal (press)
      {
        if (m == 0) // change mode
        {        
          if(inputMode == true) // submit msg and switch to monitor mode
          {
            inputMode = false;            
            queueLength = currentMsgLength + 1;  
            byte q;
            for (q = 0; q < currentMsgLength;  q++) // copy msg to queue
              queue[q] = currentMsg[q];          
            queue[q] = currentText;
            currentMsgLength = 0;      
            _dataInQueue = true; // ready to send           
            currentText = 'a';  
            digitalWrite(ledPinRed,LOW);          
          }
          else // input mode
          {            
            inputMode = true;
            if (_dispSuspended)
            {
              resumeTask(disp);
              _dispSuspended = false;
            }
            digitalWrite(ledPinRed,HIGH);
          }
        }
        else if (m == 1) // down
        {          
          if(inputMode == true)
          {   
            currentText--;
          }
        }
        else if (m == 2) // up
        {
          if(inputMode == true)
          {            
            currentText++;
          }
        }
        else // select
        {
          if(inputMode == true)
          {
            currentMsg[currentMsgLength] = currentText;
            currentMsgLength++;
            currentText = 'a';
          }
        }
      } 
      else // a HIGH-to-LOW signal (release)
      {        
        //nothing yet
      }
      last_key[m] = d;
    }
  }
}

// HANDLE DISPLAY IN TWO MODES: MONITOR & INPUT
taskLoop(disp)
{  
  if(inputMode ==  false) // monitor mode
  {
    lcd.clear();
    for (byte s = 0; s < recvLength; s++)
      lcd.print(recvString[s]);        
    char rssiString[3]; // one byte, max 255
    itoa(rssi, rssiString, 10); // base 10
    lcd.setCursor(0,1);
    lcd.print(rssiString);    
    _dispSuspended = true; // may have conflict here
    suspend(); // suspend until next data arrived, see receiveMsg()
  }
  else  // input mode
  {
    delay(500);
    lcd.clear();
    for (byte i = 0; i < currentMsgLength ; i++)  // print "typing" characters
      lcd.print(currentMsg[i]);
    lcd.print(currentText);    
  }  
}
//=============================== SET UP =====================================
void setup()
{
  Serial.begin(9600);  
  
  // init keys
  pinMode(key_input[0], INPUT);
  pinMode(key_input[1], INPUT);
  pinMode(key_input[2], INPUT);
  pinMode(key_input[3], INPUT);
  
  pinMode(ledPinRed, OUTPUT);
  pinMode(ledPinGreen, OUTPUT);
  pinMode(jumperRadio, INPUT);
  
  // init radio
  rf12_initialize(1, RF12_433MHZ, 33);
  
  // print welcome
  // lcd.print("MeshPhone");  
  
  // hook up tasks
  createTaskLoop(keypad, NORMAL_PRIORITY);
  createTaskLoop(disp, NORMAL_PRIORITY);
  initMainLoopPriority(NORMAL_PRIORITY);   
}
//=============================== MAIN TASK ==================================
void loop()
{
  radio();
}

//=============================== RADIO TASK =================================
void receiveMsg()
{
  if (rf12_recvDone() && rf12_crc == 0)
  {
    digitalWrite(ledPinGreen,HIGH);
    byte j;
    for (j = 0;  j < rf12_len; j++) // copy msg from buffer (to send to display)
    {
        recvString[j] = (char)rf12_data[j];
    }
    recvLength = rf12_len;
    if(_dispSuspended)
      resumeTask(disp);
    delay(50);
    digitalWrite(ledPinGreen, LOW);
  }
}

void sendMsg(char * msg, byte size)
{
  rf12_recvDone(); // from author note, to keep the transceiving "logic" going
  if (rf12_canSend()) 
  {       
    rf12_sendStart(0, msg, size);  
  }
}

void radio()
{  
    // always listening
    receiveMsg();
    if(_dataInQueue)
    {
      sendMsg(queue,queueLength);    
      _dataInQueue = false;
    }    
  
  //delay(50); // not sure if this could cause losing data 
}
//============================== ADC CALL BACK =================================
// interrupt routine when ADC finished, hooked while receiving, see rf12.cpp
// ADCH reg then can be read for received signal strength
ISR(ADC_vect) 
{ 
  rssi = ADCH;
  stopADC();
}






