#include <Arduino.h>
#include "silnik.hpp"
#include "protocol.hpp"

#define timeImp 200
#define LEVEL_EN_ON HIGH
#define LEVEL_EN_OFF LOW

#define ENPIN 18 //LPT 14

#define DIR1PIN 15 //LPT 8
#define DIR2PIN 14 //LPT 17
#define DIR3PIN 19 //LPT 4
#define DIR4PIN 16 //LPT 3
#define DIR5PIN 17 //LPT 6

#define PULSE1PIN 12 //LPT 16
#define PULSE2PIN 11 //LPT 1
#define PULSE3PIN 8 //LPT 7
#define PULSE4PIN 7 //LPT 9
#define PULSE5PIN 6 //LPT 5

#define LIMIT1PIN 5
#define LIMIT2PIN 4
#define LIMIT3PIN 3
#define LIMIT4PIN 2
#define LIMIT5PIN 21

//1 - LPT 14
//2 - GND LPT 9 
//3 - LPT 8
/*
enum Dozowniknr {
  X = 0,
  Y = 1,
  Z = 2,
  C = 3,
  D = 4,
};
*/


Silnik s[5] = {
  {DIR1PIN, PULSE1PIN, LIMIT1PIN, HIGH}, 
  {DIR2PIN, PULSE2PIN, LIMIT2PIN, HIGH},
  {DIR3PIN, PULSE3PIN, LIMIT3PIN, HIGH},
  {DIR4PIN, PULSE4PIN, LIMIT4PIN, HIGH},
  {DIR5PIN, PULSE5PIN, LIMIT5PIN, LOW},
};


Message msg;

void setup() {
  // put your setup code here, to run once:
  pinMode(ENPIN, OUTPUT);
  pinMode(LIMIT1PIN, INPUT_PULLUP);
  pinMode(LIMIT2PIN, INPUT_PULLUP);
  pinMode(LIMIT3PIN, INPUT_PULLUP);
  pinMode(LIMIT4PIN, INPUT_PULLUP);
  pinMode(LIMIT5PIN, INPUT_PULLUP);


  digitalWrite(ENPIN, LEVEL_EN_OFF);
  s[0].init();
  s[1].init();
  s[2].init();
  s[3].init();
  s[4].init();

  Serial1.begin(115200, SERIAL_8O1);
  Serial.begin(115200);
  delay(100);
  msg.setResetDone();
  Silnik::maxSteps = 55000;

  pinMode(13, OUTPUT);
}

void loop() {
  
  // put your main code here, to run repeatedly:
  if (Serial1.available() > 0) {
    if (msg.check(Serial1.read())) {
      if (msg.getStatusWork() == msg.RETURN_HOME) {
        digitalWrite(ENPIN, LEVEL_EN_ON);
        Serial.println("Home");
        digitalWrite(13, HIGH);
        uint32_t steps = s[msg.getNrDozownika()].home(); 
        msg.setHomeDone(steps);
        Serial.print("Done Home=");
        Serial.println(steps);
        digitalWrite(13, LOW);
        digitalWrite(ENPIN, LEVEL_EN_OFF);
      } else if (msg.getStatusWork() == msg.POS_START) {
        digitalWrite(ENPIN, LEVEL_EN_ON);
        Serial.print("Pos=");
        Serial.println(msg.getSteps());
        digitalWrite(13, HIGH);
        uint32_t steps = s[msg.getNrDozownika()].start(msg.getSteps()); 
        msg.setPosDone(steps);
        Serial.print("Done pos=");
        Serial.println(steps);
        digitalWrite(13, LOW);
        digitalWrite(ENPIN, LEVEL_EN_OFF);
      } else if (msg.getStatusWork() == msg.SETTING) {
        for (int i=0; i<5; ++i) {
          s[i].setHomeLevel(msg.getReverse(i) ? HIGH : LOW);
          Serial.print("Silnik ");
          Serial.print(i+1,DEC);
          Serial.print(" reverse=");
          Serial.println(msg.getReverse(i),DEC);
        }
      } else if (msg.getStatusWork() == msg.ECHO) {
          uint8_t lp1 = 0, lp2 = 0;
          uint8_t tryN = 10;
          do {
              lp1 |= digitalRead(LIMIT1PIN) << 0;
              lp1 |= digitalRead(LIMIT2PIN) << 1;
              lp1 |= digitalRead(LIMIT3PIN) << 2;
              lp1 |= digitalRead(LIMIT4PIN) << 3;
              lp1 |= digitalRead(LIMIT5PIN) << 4;
              delay(50);
              lp2 |= digitalRead(LIMIT1PIN) << 0;
              lp2 |= digitalRead(LIMIT2PIN) << 1;
              lp2 |= digitalRead(LIMIT3PIN) << 2;
              lp2 |= digitalRead(LIMIT4PIN) << 3;
              lp2 |= digitalRead(LIMIT5PIN) << 4;
              delay(50);
          } while (lp1 != lp2 && tryN-- > 0);
          msg.setEcho(lp1);
          Serial.print("Echo Message : ");
          Serial.println(lp1, BIN);
      } else if (msg.getStatusWork() == msg.NOP) {

      } else {
        Serial.println("Nieznany rozkaz");
      }
    }
  }  
}