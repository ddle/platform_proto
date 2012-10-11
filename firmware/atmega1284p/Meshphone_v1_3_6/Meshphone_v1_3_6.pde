// ===============================================================================//
// Meshphone Prototype I Firmware v1.3
// by Le Dang Dung
// ===============================================================================//
// Hardware:
// + atmega1284p 16MHz
// + RFM12 433/868MHz via SPI
// + 16x2 LCD parallel interface (using Hitachi HD44780 controller)
// + 4x3 Matrix keypad for input 
// Fuse setting: FD D9 F6
// Connection:
//                           ATMega1284P
//                           +---\/---+
//        LED -> (D 0) PB0  1|        |40  PA0 (AI 0 / D31) <- RSSI
//               (D 1) PB1  2|        |39  PA1 (AI 1 / D30) <- KP1
// (nIRQ)-> INT2 (D 2) PB2  3|        |38  PA2 (AI 2 / D29) <- KP2
//               (D 3) PB3  4|        |37  PA3 (AI 3 / D28) <- KP3
// (nSEL)<-   SS (D 4) PB4  5|        |36  PA4 (AI 4 / D27) <- KP4
// (SDI) <- MOSI (D 5) PB5  6|        |35  PA5 (AI 5 / D26) <- KP5
// (SDO) -> MISO (D 6) PB6  7|        |34  PA6 (AI 6 / D25) <- KP6
// (SCK) <-> SCK (D 7) PB7  8|        |33  PA7 (AI 7 / D24) <- KP7
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
// + output logics from ATmega1284p (5V) voltage divided (~3V) before fetched to RFM12B
// + LCD contrast control via Vo (0~3.3V) using a potentiometer 
// =============================================================================//
// Library:
// + Ported DuinOS for ATmega1284p (Sanguino & DuinOS v0.3)
// + Ported RF12 lib for ATmega1284p (Jeelabs 2009-02-09), add RSSI reading
// + lib4LCD (DevCorner http://www.stefanomanni.it/arduino/lib4lcd/)
// Note:
// + before compiling, under Tools->Board select Multiplo.Brain.M1284 + DuinOS
// + standard delay function (any kind) does not work and was redefined in 
//   DuinOs(see DuinOS.h). There 's also a delayMicroSeconds() that should be 
//   used with care. DO NOT USE DEALY() OUTSIDE ANY TASK!
// =============================================================================//

#include <RF12.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <stdlib.h>
#include <lib4LCD.h>
#include <Keypad.h>

#define ledPinRed 14
#define ledPinGreen 0
#define jumperRadio 1

const char* myID = "P1";
int msgNum = 1;

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

// Keypad Wiring: 
// D30 D29 D28 D27 D26 D25 D24
// to 
// KP1 KP2 KP3 KP4 KP5 KP6 KP7
// Keypad Rows: KP2,KP7,KP6,KP4 == D29 D24 D25 D27
// Keypad Columns: KP3,KP1,KP5  == D28 D30 D26

// Connect keypad ROW0, ROW1, ROW2 and ROW3 
byte rowPins[ROWS] = {29, 24, 25, 27}; //connect to the row pinouts of the keypad 
// Connect keypad COL0, COL1 and COL2 
byte colPins[COLS] = {28, 30, 26}; 
// Create the Keypad
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

//lib4LCD lcd(numCol,numRow,rs, enable, d4, d5, d6, d7);
lib4LCD lcd(16, 2, 15, 16, 17, 18, 19, 20);

// put these in memory, not registers
// probably make it "slow", need revised
volatile unsigned char rssi;
char rssiString[3]; // one byte, max 255
// old keypad variables
uint8_t key_input[4] = {10,11,12,13};
uint8_t last_key[4] = {LOW,LOW,LOW,LOW};
boolean inputMode = false;
//char currentText = '_';
char currentMsg[100]; // random buffer size for current version !
unsigned char currentMsgLength = 0;
char sndString[128] = ""; // random buffer size for current version !
char sndBuff[100] = ""; // random buffer size for current version !
char serialBuff[100] = "";
unsigned char sndLength = 0;

unsigned char ACKsndLength = 0;
char ACKsndString[20] = ""; // random buffer size for current version !
int ACKNum = 0;

unsigned char serialLength = 0;
volatile char recvString[128]; // random buffer size for current version !
volatile unsigned char recvLength = 0;
volatile boolean _dispSuspended = false; // warning: this resource is shared among tasks
volatile boolean _dataInSndBuff = false;   // warning: this resource is shared among tasks
volatile boolean _startup = true;
volatile boolean _serialOut = false;
static void receiveMsg();
static void sendMsg();
static unsigned int stringConcat(char* strOne,const char* strTwo, unsigned int strOneLength, unsigned int strOneMaxLength);

//========================== TASK DECLARATION  & DEFINITION =================================//
declareTaskLoop(disp);
declareTaskLoop(keypad);
declareTaskLoop(serial);

//===== HANDLE SERIAL CONNECTION =====//
taskLoop(serial)
{
  // handle serial in
  if (Serial.available() > 0) 
  {
    // read the incoming byte:
    int incomingByte = Serial.read();

    // say what you got:    
    Serial.print(incomingByte, BYTE);    
  }
  else
  {
    delay(50);
  }
  // handle serial out
  if(_serialOut == true)
  {
    byte k = 0;
    for (k; k < serialLength; k++)
      Serial.print(serialBuff[k]);    
    if(k>0)
    {
      Serial.print(" rssi ");
      Serial.println(rssiString);
    }
    _serialOut = false;
  }
  //suspend();
}

//===== HANDLE KEYPAD =====//
taskLoop(keypad)
{  
  delay(100); // slow this task down, but fast enough to poll key press
  char key = kpd.getKey();
  if(key)  // same as if(key != NO_KEY)
  {
    switch (key)
    {
      case '*': //============ delete
        if(inputMode == true)
        {
          if(currentMsgLength > 0)
            currentMsgLength--;
        }
        break;
        
      case '#': //============ change mode
          if(inputMode == true) // submit msg and switch to monitor mode
          {
            inputMode = false;            
            currentMsg[currentMsgLength] = '\0';
            sndLength = stringConcat(sndBuff,currentMsg,0,100);
            currentMsgLength = 0;      
            _dataInSndBuff = true; // ready to send,  race condition may be...need revised 
            digitalWrite(ledPinRed,LOW);          
          }
          else //============= input mode
          {            
            inputMode = true;
//            if (_dispSuspended)
//            {
//              resumeTask(disp);
//              _dispSuspended = false; // race condition may be...need revised
//            }
            digitalWrite(ledPinRed,HIGH);
          }        
        break;
        
      default: //============= input characters
        if(inputMode == true)
        {
          currentMsg[currentMsgLength] = key;
          currentMsgLength++;
          
        }
    }
    if (_dispSuspended)
    {
      resumeTask(disp);
      _dispSuspended = false; // race condition may be...need revised
    }
  }
}

//====== HANDLE DISPLAY IN TWO MODES: MONITOR & INPUT ======//
taskLoop(disp)
{ 
  //delay(500);
  if(inputMode ==  false) //============= monitor mode
  {
    if (recvLength > 0)
    {
      lcd.clear();
      for (byte s = 0; s < recvLength; s++)
        lcd.print(recvString[s]);     
      
        lcd.setCursor(0,1);
        lcd.print(rssiString);    
    }
//    _dispSuspended = true; // race condition may be...need revised
//    suspend(); // suspend until next data arrived, see receiveMsg()
  }
  else  //================ input mode
  {    
    lcd.clear();
    for (byte i = 0; i < currentMsgLength ; i++)  // print "typing" characters
      lcd.print(currentMsg[i]);   
    //delay(500);
  }  
  _dispSuspended = true; // race condition may be...need revised
  suspend(); // suspend until next data arrived, see receiveMsg()
}

//============= SETUP ==================//
void setup()
{
  Serial.begin(9600);  
  pinMode(ledPinRed, OUTPUT);
  pinMode(ledPinGreen, OUTPUT);
  //pinMode(jumperRadio, INPUT);
  
  // digitalWrite(ledPinGreen,HIGH);
  
  // print welcome
  lcd.clear();
  lcd.print("MeshPhone v1.3");  
  Serial.println("MeshPhone v1.3");
  // init radio
  rf12_initialize(1, RF12_433MHZ, 33);

  // delay(500); //   do not use delay outside tasks! 
  // hook up tasks
  createTaskLoop(keypad, NORMAL_PRIORITY);
  createTaskLoop(disp, NORMAL_PRIORITY);
  createTaskLoopWithStackSize(serial, NORMAL_PRIORITY, 200); // more RAM, default is configMINIMAL_STACK_SIZE
  initMainLoopPriority(NORMAL_PRIORITY);  
  
}
//=============================== MAIN TASK ==================================
void loop()
{
//  Serial.println("test");
//  digitalWrite(ledPinGreen,HIGH);
//  delay(50);
//  digitalWrite(ledPinGreen,LOW);
//  delay(500);
// if (Serial.available() > 0) 
//  {
//    // read the incoming byte:
//    int incomingByte = Serial.read();
//    digitalWrite(ledPinGreen,HIGH);
//  delay(50);
//  digitalWrite(ledPinGreen,LOW);
//  
//    // say what you got:
//    Serial.print("I received: ");
//    Serial.println(incomingByte, DEC);
//    
//  }
  radio();
}

//=============================== RADIO TASKs =================================
// radio behaviors defined here
static void radio()
{  
    // simultaneously listening
    receiveMsg();
    if(_dataInSndBuff) 
    {
      // test version 
      // send a default string for testing purpose
      if (sndLength == 0 || stringCompare(sndBuff,"123456"))  
      {       
        char msgNumString[16];        
        itoa(msgNum, msgNumString, 10);
        //String defaultString = String(msgNum); // there are some issues with using String lib...        
        // make a default string... something like P#[phoneID]: ping [msg#]
        
        sndLength = stringConcat(sndString,myID,0,128);  
        sndLength = stringConcat(sndString," ping ",sndLength,128); 
        sndLength = stringConcat(sndString,msgNumString,sndLength,128);   
        
        sendMsg(sndString,sndLength);            
        msgNum++;
        // test version
        if(stringCompare(sndBuff,"123456"))
        {
          // delay(1000);
          // halt ping about one second
          // but still enable receiver
          int countRev = 0;
          for (countRev; countRev < 500; countRev++)
            receiveMsg();            
//          if (_dispSuspended)
//          {
//            resumeTask(disp);
//            _dispSuspended = false; // race condition maybe...need revised
//          }
        }
        else 
           _dataInSndBuff = false;   // race condition maybe...need revised 
      }
      else // send submitted msg
      { 
            
        sndLength = stringConcat(sndString,myID,0,128);      
        sndLength = stringConcat(sndString," ",sndLength,128);      
        sndLength = stringConcat(sndString,sndBuff,sndLength,128);     
        sendMsg(sndString,sndLength);   
        _dataInSndBuff = false;   // race condition may be...need revised   
      }
     
    }      
    //delay(100); // not sure if this could cause losing data 
}

// check incomming messages and put on display buffer
// see jeelabs http://jeelabs.net/projects/11/wiki/RF12 for details
static void receiveMsg()
{
  if (rf12_recvDone() && rf12_crc == 0)
  {
    
    byte j;
    for (j = 0;  j < rf12_len; j++) // copy msg from buffer
    {
        recvString[j] = (char)rf12_data[j];
        serialBuff[j] = (char)rf12_data[j];
    }
    recvLength = j;
    serialLength = j;
    itoa(rssi, rssiString, 10); // base 10
    
    if(_dispSuspended) // resume display task, to dispay data
      resumeTask(disp);
      
    //resumeTask(serial);
    _serialOut = true;
    //delay(25);
    
    // blink
    delay(15);
    digitalWrite(ledPinGreen,HIGH);
    delay(15);    
    digitalWrite(ledPinGreen, LOW);

    // sample ACK, the following code using for test purposes only, no more than 9 units
    if (recvString[3]!='A' || recvString[4]!='C' || recvString[5]!='K')
    {
      ACKNum++;
      char ACKNumString[16];
      itoa(ACKNum, ACKNumString, 10);      
      ACKsndLength = stringConcat(ACKsndString,myID,0,128);
      ACKsndLength = stringConcat(ACKsndString," ACK ",ACKsndLength,128);
      ACKsndLength = stringConcat(ACKsndString,ACKNumString,ACKsndLength,128);   
      sendMsg(ACKsndString,ACKsndLength);
    }    
  }
  delay(2); // spin delay for counting/timing, send/receive a 20 bytes per packet approximately takes ~ 3ms
}

// submit send message in current buffer to radio, see http://jeelabs.net/projects/11/wiki/RF12 for details
static void sendMsg(char * msg, byte size)
{
  rf12_recvDone(); // from author note, to keep the transceiving "logic" going
  if (rf12_canSend()) 
  { 
    delay(15);
    digitalWrite(ledPinGreen,HIGH);        
    delay(15);    
    digitalWrite(ledPinGreen, LOW); 
    rf12_sendStart(0, msg, size); 
  }
}

//============================================================
// a custom string concat that does strOne = strOne + strTwo
// return : resulted string_length, including '\0'
// strTwo must be NULL terminated, boundaries must be provided
static unsigned int stringConcat(char* strOne,const char* strTwo, unsigned int strOneLength, unsigned int strOneMaxLength)
{
  unsigned int strTwoIndex = 0;
  while(strTwo[strTwoIndex] != '\0' && (strTwoIndex + strOneLength) < strOneMaxLength)
  {
    strOne[strOneLength + strTwoIndex] = strTwo[strTwoIndex];
    strTwoIndex++;
  }
  strOne[strOneLength + strTwoIndex] = '\0';
  return strOneLength + strTwoIndex;
}

static boolean stringCompare(char* strOne,char* strTwo)
{
  String one = String(strOne);
  String two = String(strTwo);
  return one.equals(two);
}
char currentChar;
unsigned char timeOut = 0;
char keyMap [10][4] = {
  {' ','.',',','0'},
  {'a','b','c','1'},
  {'d','e','f','2'},
  {'g','h','i','3'},
  {'j','k','l','4'},
  {'m','n','o','5'},
  {'p','q','r','6'},
  {'s','t','u','7'},
  {'v','w','x','8'},
  {'y','z','&','9'}    
};

unsigned int counterOne = 0;
unsigned int counterOneMax = 10;
boolean keyTimeout = false;
boolean _startCounter = false; // warning: this resource is shared among tasks 
void keyCounter()
{
  if(_startCounter)
  {
    if(counterOne <  counterOneMax)
    {
      keyTimeout = false;
      counterOne++;
    }
    else
    {
      keyTimeout = true;
      counterOne = 0;
      _startCounter = false;
    }        
  }
}
void keyInterpreter()
{
  
}
//============================== ADC CALL BACK =================================
// interrupt routine when ADC finished, hooked while receiving, see rf12.cpp
// ADCH reg then can be read for received signal strength
ISR(ADC_vect) 
{ 
  rssi = ADCH;
  stopADC();
}







