#include <Arduino.h>
#include "protocol.hpp"
Message msg;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(115200);

  msg.init();
}

void loop() {
  // put your main code here, to run repeatedly:
      if (Serial.available())  
        msg.check1(Serial.read());
      
      if (Serial1.available()) {
        msg.check2(Serial1.read());
      }

            
}