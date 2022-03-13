#include <Arduino.h>
#include "silnik.hpp"
#include "protocol.hpp"

#define timeImp 100
#define LEVEL_EN HIGH


#define ENPIN 18 //LPT 14

#define DIR1PIN 16 //LPT 4
#define DIR2PIN 14 //LPT 17
#define DIR3PIN 15 //LPT 8
#define DIR4PIN 39 //LPT 3
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
void interS1();
void interS2();
void interS3();
void interS4();
void interS5();




Silnik s[5] = {
  {timeImp, ENPIN, DIR1PIN, PULSE1PIN, LIMIT1PIN, interS1}, 
  {timeImp, ENPIN, DIR2PIN, PULSE2PIN, LIMIT2PIN, interS2},
  {timeImp, ENPIN, DIR3PIN, PULSE3PIN, LIMIT3PIN, interS3},
  {timeImp, ENPIN, DIR4PIN, PULSE4PIN, LIMIT4PIN, interS4},
  {timeImp, ENPIN, DIR5PIN, PULSE5PIN, LIMIT5PIN, interS5},
};


Message msg;

void setup() {
  // put your setup code here, to run once:
  pinMode(ENPIN, OUTPUT);
  digitalWrite(ENPIN, LEVEL_EN);
  s[0].init();
  s[1].init();
  s[2].init();
  s[3].init();
  s[4].init();

  Serial1.begin(9600, SERIAL_8O2);
  Serial.begin(9600, SERIAL_8O2);
  delay(100);
  msg.setResetDone();
  Silnik::maxSteps = 0;

  pinMode(13, OUTPUT);
}


void loop() {
  // put your main code here, to run repeatedly:
  if (Serial1.available() > 0) {
    if (msg.check(Serial1.read())) {
      if (msg.getStatusWork() == msg.RETURN_HOME) {
        digitalWrite(13, HIGH);
        msg.setHomeDone(s[msg.getNrDozownika()].home());
        Serial.println("Done Home");
        digitalWrite(13, LOW);
      } else if (msg.getStatusWork() == msg.POS_START) {
        digitalWrite(13, HIGH);
        msg.setPosDone(s[msg.getNrDozownika()].start(msg.getSteps()));
        Serial.println("Done pos");
        digitalWrite(13, LOW);
      }
    }
  }
}


void interS1() {
  s[0].interruptFun();
}

void interS2() {
  s[1].interruptFun();
}

void interS3() {
  s[2].interruptFun();
}

void interS4() {
  s[3].interruptFun();
}

void interS5() {
  s[4].interruptFun();
}
