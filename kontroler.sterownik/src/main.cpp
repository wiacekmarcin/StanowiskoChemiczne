#include <Arduino.h>
#include "silnik.hpp"
#include "protocol.hpp"

#define ENPIN 18 //LPT 14

#define DIR1PIN 39 //LPT 4
#define DIR2PIN 14 //LPT 17
#define DIR3PIN 15 //LPT 8
#define DIR4PIN 16 //LPT 3
#define DIR5PIN 17 //LPT 6

#define PULSE1PIN 12 //LPT 16
#define PULSE2PIN 11 //LPT 17
#define PULSE3PIN 8 //LPT 7
#define PULSE4PIN 7 //LPT 9
#define PULSE5PIN 6 //LPT 5

#define LIMIT1PIN 5
#define LIMIT2PIN 4
#define LIMIT3PIN 3
#define LIMIT4PIN 2
#define LIMIT5PIN 21

void interS1();
void interS2();
void interS3();
void interS4();
void interS5();


Silnik s[5] = {
  {ENPIN, DIR1PIN, PULSE1PIN, LIMIT1PIN, interS1}, 
  {ENPIN, DIR2PIN, PULSE2PIN, LIMIT2PIN, interS2},
  {ENPIN, DIR3PIN, PULSE3PIN, LIMIT3PIN, interS3},
  {ENPIN, DIR4PIN, PULSE4PIN, LIMIT4PIN, interS4},
  {ENPIN, DIR5PIN, PULSE5PIN, LIMIT5PIN, interS5},
};


Message msg;

void setup() {
  // put your setup code here, to run once:
  pinMode(ENPIN, OUTPUT);
  s[0].init();
  s[1].init();
  s[2].init();
  s[3].init();
  s[4].init();

  Serial1.begin(115200);
  Serial.begin(115200);
  delay(100);
  msg.setResetDone();
  Silnik::maxSteps = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial1.available() > 0) {
    if (msg.check(Serial1.read())) {
      if (msg.getStatusWork() == msg.RETURN_HOME) {
        msg.setHomeDone(s[msg.getNrDozownika()].home());
        Serial.print("Done Home");
      } else if (msg.getStatusWork() == msg.POS_START) {
        msg.setPosDone(s[msg.getNrDozownika()].start(msg.getSteps()));
        Serial.print("Done pos");
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
