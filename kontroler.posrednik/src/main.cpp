#include <Arduino.h>
#include "protocol.hpp"
Message msg;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200, SERIAL_8O1);
  Serial1.begin(115200, SERIAL_8O1);

  msg.init();

  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  //pinMode(13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
      if (Serial.available() > 0)  
        msg.check1(Serial.read());
      
      if (Serial1.available() > 0) {
        msg.check2(Serial1.read());
      }
            
}