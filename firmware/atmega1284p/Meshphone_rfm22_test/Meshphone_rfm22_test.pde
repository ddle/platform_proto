// ===============================================================================//
// Meshphone Prototype I Firmware v1.4
// by Le Dang Dung
// ===============================================================================//
// Hardware:
// + atmega1284p 16MHz
// + RFM22 433 via SPI
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
// (SDO) -> MOSI (D 5) PB5  6|        |35  PA5 (AI 5 / D26) <- KP5
// (SDI) <- MISO (D 6) PB6  7|        |34  PA6 (AI 6 / D25) <- KP6
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
//                      +-------------------+
//                 ANT 1|      RFM22B       |9  GND
//                     2|                   |10 SDN->GND
//                     3|                   |11 NIRQ
//                     4|            --     |12 NSEL
//                 VCC 5|           |  |    |13 SCK
//                     6|     ----  |  |    |14 SDI
//                     7|    | IC |  --     |15 SDO
//                     8|     ----          |16 
//                      +-------------------+
// Note: 
// + see Mike 's RF22 lib for more details on RF22 wireup
// + outputs from ATmega1284p (5V) voltage divided (~3V) before fetched to RFM12B
// + LCD contrast control via Vo (0~3.3V) using a potentiometer 
// =============================================================================//
// Library:
// + Ported DuinOS for ATmega1284p (Sanguino & DuinOS v0.3)
// + Ported RF22 lib (Mike McCauley) for ATmega1284p 
// + lib4LCD (DevCorner http://www.stefanomanni.it/arduino/lib4lcd/)
// Note:
// + before compiling, under Tools->Board select Multiplo.Brain.M1284 + DuinOS
// + standard delay function (any kind) does not work and was redefined in 
//   DuinOs(see DuinOS.h). There 's also a delayMicroSeconds() that should be 
//   used with care. DO NOT USE DEALY() OUTSIDE ANY TASK!
// =============================================================================//

//#include <RF12.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <stdlib.h>
#include <lib4LCD.h>
#include <Keypad.h>

#include <SPI.h>
#include <RF22.h>

// Singleton instance of the radio
// SS pin on pin D4, attachInterrupt on pin D2
RF22 rf22(4,0);

#define ledPinRed 14
#define ledPinGreen 0
#define jumperRadio 1

uint8_t myID[] = "P1";
uint8_t pw[] = "123456";
uint8_t hello_msg[32] = " v1.9 hello ";

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

int recvCount = 0;
int txCount = 0;
unsigned char rssi;
char rssiString[3]; // one byte, max 255
uint8_t key_input[4] = {10,11,12,13};
uint8_t last_key[4] = {LOW,LOW,LOW,LOW};
boolean inputMode = false;
//char currentText = '_';
uint8_t currentMsg[100]; // random buffer size for current version !
unsigned char currentMsgLength = 0;
uint8_t sndString[128] = ""; // random buffer size for current version !
uint8_t sndBuff[100] = ""; // random buffer size for current version !
unsigned char sndLength = 0;
char recvString[128]; // random buffer size for current version !
unsigned char recvLength = 0;
char serialBuff[100] = "";
volatile boolean _serialOut = false;
unsigned char serialLength = 0;
boolean _dispSuspended = false; // warning: this resource is shared among tasks
boolean _dataInSndBuff = false;   // warning: this resource is shared among tasks
boolean _startup = true;
boolean _autoTx = false;
void tx(uint8_t* data, uint8_t length);
void rx();

// a custom string concat that does strOne = strOne + strTwo
// return : resulted string_length, including '\0'
// strTwo must be NULL terminated, boundaries must be provided
static unsigned int stringConcat(char* strOne, char* strTwo, unsigned int strOneLength, unsigned int strOneMaxLength)
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
  String one = String((char*)strOne);
  String two = String((char*)strTwo);
  return one.equals(two);
}
//========================== TASK DECLARATION  & DEFINITION =================================//
declareTaskLoop(disp);
declareTaskLoop(keypad);
declareTaskLoop(radio_t);
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
  delay(50); // slow this task down, but fast enough to poll key press
  uint8_t key = kpd.getKey();
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
            sndLength = stringConcat((char*)sndBuff,(char*)currentMsg,0,100);
            currentMsgLength = 0;      
            _dataInSndBuff = true; // ready to send,  race condition may be...need revised 
            if (sndLength == 0)
            {
              if(_autoTx == true)
              {
                _autoTx = false;
                 _dataInSndBuff = false;
              }
            }
            digitalWrite(ledPinRed,LOW);          
          }
          else //============= input mode
          {            
            inputMode = true;
            if (_dispSuspended)
            {
              resumeTask(disp);
              _dispSuspended = false; // race condition may be...need revised
            }
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
  }
}

//====== HANDLE DISPLAY IN TWO MODES: MONITOR & INPUT ======//
taskLoop(disp)
{  
  delay(500);
  if(inputMode ==  false) //============= monitor mode
  {
    lcd.clear();
    char countString[8]; // one byte, max 255 
    itoa(recvCount, countString, 10); // base 10
    lcd.print(countString);
    lcd.print(" ");
    
    for (byte s = 0; s < recvLength; s++)
      lcd.print(recvString[s]);       
    //recvLength = 0;
    
    itoa(rssi, rssiString, 10); // base 10
    lcd.setCursor(0,1);
    lcd.print(rssiString);    
    _dispSuspended = true; // race condition may be...need revised
    suspend(); // suspend until next data arrived, see receiveMsg()
  }
  else  //================ input mode
  {
    //delay(500);
    lcd.clear();
    for (byte i = 0; i < currentMsgLength ; i++)  // print "typing" characters
      lcd.print(currentMsg[i]);   
  }  
}


//=============================== RADIO TASKs =================================
// radio behaviors defined here
int countRadioLoop = 100;
taskLoop (radio_t)
{        
    rx(); // this function needed to be called frequently enough, otherwise data will be lost.
    delay(10);
    if(_autoTx == false)
    {
      if(_dataInSndBuff)
      {
        if (stringCompare((char*)sndBuff,(char*)pw) == true)
        {
          _autoTx = true;
           _dataInSndBuff = false;
           
        }
        else if(sndLength == 0)
        {
          tx(hello_msg,11);
          _dataInSndBuff = false;
          //_autoTx = false;
        }
        else
        {
          tx((uint8_t*)sndBuff,sndLength+1);
          _dataInSndBuff = false;
          //Serial.println((char*)sndBuff);
          //Serial.println(sndLength,DEC);
        }      
      }
    }
    else
    {
      //delay(900);
      if (countRadioLoop == 0)
      {
        countRadioLoop = 100;
        txCount++;
        char tmp[32];
        char txCountString[8];
        itoa(txCount,(char*)txCountString,10);
        uint8_t txlen = stringConcat(tmp, txCountString,0,32);
        txlen = stringConcat(tmp,(char*)hello_msg,txlen, 32);
        tx((uint8_t*)tmp,txlen);  
        //Serial.println(txlen,DEC);
        //Serial.println(tmp);
        //delay(40);
      }
      else
      {
        countRadioLoop--;
      }
    }    
}
boolean radio_init = false;

//============= SETUP ==================//
void setup()
{  
  Serial.begin(9600);  
  pinMode(ledPinRed, OUTPUT);
  pinMode(ledPinGreen, OUTPUT);

  lcd.clear();
  lcd.print("rf22 ");      

  // hook up tasks
  createTaskLoop(keypad, NORMAL_PRIORITY);
  createTaskLoop(disp, NORMAL_PRIORITY);
  createTaskLoopWithStackSize(serial, NORMAL_PRIORITY, 200); // more RAM, default is configMINIMAL_STACK_SIZE
  initMainLoopPriority(NORMAL_PRIORITY); 
  
  //suspendTask(radio_t); 
  
}
//=============================== MAIN TASK ==================================
void loop()
{
  if(radio_init == false)
  {
      // init radio
      if (!rf22.init())           
        lcd.print("init failed");        
//      else if (!rf22.setFrequency(434.0))
//        lcd.print("setFreq failed");
      else if (!rf22.setModemConfig(RF22::FSK_Rb9_6Fd45))
        lcd.print("setConf failed");
      else   
        lcd.print("ok");          
      //delay(100);    

      rf22.setTxPower(RF22_TXPOW_17DBM);  
      radio_init = true;
  }
  createTaskLoopWithStackSize(radio_t, NORMAL_PRIORITY,400); // need more ram!!!
  //resumeTask(radio_t);
  suspend();
  //radio();

}


void tx(uint8_t* data, uint8_t length)
{  
  digitalWrite(ledPinGreen,HIGH);
  rf22.send(data, length);
  
  rf22.waitPacketSent();
  digitalWrite(ledPinGreen,LOW);
 // lcd.clear();
 // lcd.print("pkg sent");
}


void rx()
{
  //delay(1000);
  if (rf22.available())
  {
    rssi = rf22.rssiRead();
    uint8_t buf[RF22_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    //rf22.waitAvailable(); // block until recv
    //lcd.clear();        
    // Serial.print("avail: ");
    if (rf22.recv(buf, &len))
    {              
       recvCount++;       
       int z = 0;       
       //Serial.println((char*)buf);       
       for(z ; z < len-1; z++)
       {
         recvString[z] = (char)buf[z];
         serialBuff[z] = (char)buf[z];         
       }   
       serialLength = z;
       recvLength = z; 
       buf[z] = '\0';
       //Serial.println((char*)buf);      
       if (_dispSuspended)
       {
         resumeTask(disp);
         _dispSuspended = false; // race condition may be...need revised
       }
       _serialOut = true;
    }
    else
    {
       //Serial.println("recv failed");
    }
  }
  else
  {
       //Serial.println("nothing");
  }
  
}







