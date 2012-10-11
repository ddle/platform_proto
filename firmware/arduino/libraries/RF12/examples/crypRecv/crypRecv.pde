// Test encrypted communication, receiver side
// 2010-02-21 <jcw@equi4.com> http://opensource.org/licenses/mit-license.php
// $Id: crypRecv.pde 4833 2010-02-21 21:44:24Z jcw $

#include <RF12.h>


//byte recvCount;

void setup () {
   // Serial.begin(57600);
   // Serial.println("\n[crypRecv]");
    rf12_initialize(1, RF12_433MHZ, 33);
    pinMode(3,INPUT);
}

// this test turns encryption on or off after every 10 received packets

void loop () {
    if (rf12_recvDone() && rf12_crc == 0) {
        digitalWrite(3,HIGH);
  delay(100);
  digitalWrite(3,LOW);
  delay(500);
        
    }
}
