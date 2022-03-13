#include <Arduino.h>
#include "protocol.hpp"
Message msg;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600, SERIAL_8O2);
  Serial1.begin(9600, SERIAL_8O2);

  msg.init();

  pinMode(A3, OUTPUT);
  digitalWrite(A3, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
      if (Serial.available() > 0)  
        msg.check1(Serial.read());
      
      if (Serial1.available() > 0) {
        msg.check2(Serial1.read());
      }
            
}