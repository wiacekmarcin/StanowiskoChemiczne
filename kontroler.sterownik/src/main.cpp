#include <Arduino.h>
#include "silnik.hpp"
#include "protocol.hpp"

#define timeImp 200
#define LEVEL_EN HIGH


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
void interS1();
void interS2();
void interS3();
void interS4();
void interS5();




Silnik s[5] = {
  {timeImp, DIR1PIN, PULSE1PIN, LIMIT1PIN}, 
  {timeImp, DIR2PIN, PULSE2PIN, LIMIT2PIN},
  {timeImp, DIR3PIN, PULSE3PIN, LIMIT3PIN},
  {timeImp, DIR4PIN, PULSE4PIN, LIMIT4PIN},
  {timeImp, DIR5PIN, PULSE5PIN, LIMIT5PIN},
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


  digitalWrite(ENPIN, LEVEL_EN);
  s[0].init();
  s[1].init();
  s[2].init();
  s[3].init();
  s[4].init();

/*
  attachInterrupt(digitalPinToInterrupt(LIMIT1PIN),interS1, FALLING);
  attachInterrupt(digitalPinToInterrupt(LIMIT2PIN),interS2, FALLING);
  attachInterrupt(digitalPinToInterrupt(LIMIT3PIN),interS3, FALLING);
  attachInterrupt(digitalPinToInterrupt(LIMIT4PIN),interS4, FALLING);
  attachInterrupt(digitalPinToInterrupt(LIMIT5PIN),interS5, FALLING);
*/
  Serial1.begin(9600, SERIAL_8O2);
  Serial.begin(115200);
  delay(100);
  msg.setResetDone();
  Silnik::maxSteps = 100000;

  pinMode(13, OUTPUT);
  interrupts();
}
#define STR(X) #X
//#define STR(X) _STR(X)
//#define _STR2(X,Y) X##Y##PIN
#define STR2(X,Y) X ## #Y ## PIN

void RUN(uint8_t X) 
{
  const uint8_t pulses[] = {PULSE1PIN, PULSE2PIN, PULSE3PIN, PULSE4PIN, PULSE5PIN};
  for(int i=0;i<1000;i++) {
        digitalWrite(pulses[X-1],HIGH);
        delayMicroseconds(200);
        digitalWrite(pulses[X-1],LOW);
        delayMicroseconds(200);
        }
}

void LOWP(uint8_t X) 
{
  const uint8_t dirs[] = {DIR1PIN, DIR2PIN, DIR3PIN, DIR4PIN, DIR5PIN};
  digitalWrite(dirs[X-1],LOW);
  //RUN(X);
}
void HIGHP(uint8_t X)
{
  const uint8_t dirs[] = {DIR1PIN, DIR2PIN, DIR3PIN, DIR4PIN, DIR5PIN};
  digitalWrite(dirs[X-1],HIGH);
  //RUN(X);
}
void loop() {
  bool firstChar = false;
  if (Serial.available() > 0) {
    switch(Serial.read()) {
      case 'A':
        LOWP(1);
        break;
      case 'a':
        HIGHP(1);
        break;
      case 'B':
        LOWP(2);
        break;
      case 'b':
        HIGHP(2);
        break;
      case 'C':
        LOWP(3);
        break;
      case 'c':
        HIGHP(3);
        break;
      case 'D':
        LOWP(4);
        break;
      case 'd':
        HIGHP(4);
        break;
      case 'E':
        LOWP(5);
        break;
      case 'e':
        HIGHP(5);
        break;
      case '1':
        RUN(1);
        break;
      case '2':
        RUN(2);
        break;
      case '3':
        RUN(3);
        break;
      case '4':
        RUN(4);
        break;
      case '5':
        RUN(5);
        break;
      case 'Z':
        LOWP(1);
        break;
      case 'z':
        HIGHP(1);
        break;
      case 'Y':
        LOWP(2);
        break;
      case 'y':
        HIGHP(2);
        break;
      case 'X':
        LOWP(3);
        break;
      case 'x':
        HIGHP(3);
        break;
      case 'U':
        LOWP(4);
        break;
      case 'u':
        HIGHP(4);
        break;
      case 'V':
        LOWP(5);
        break;
      case 'v':
        HIGHP(5);
        break;
      default:
        break;
    }
  }
  return;
  digitalWrite(DIR2PIN, LOW);
  for (int i = 0; i < 1000; i++) {
        digitalWrite(PULSE3PIN, HIGH);    
        delayMicroseconds(200);
        digitalWrite(PULSE3PIN, LOW);        
        delayMicroseconds(200);
  }

  digitalWrite(DIR2PIN, HIGH);
  for (int i = 0; i < 1000; i++) {
        digitalWrite(PULSE3PIN, HIGH);    
        delayMicroseconds(400);
        digitalWrite(PULSE3PIN, LOW);        
        delayMicroseconds(400);
  }


  // put your main code here, to run repeatedly:
  if (Serial1.available() > 0) {
    if (msg.check(Serial1.read())) {
      Serial.print("Max steps");
      Serial.println(Silnik::maxSteps);
      if (msg.getStatusWork() == msg.RETURN_HOME) {
        Serial.println("Home");
        digitalWrite(13, HIGH);
        uint32_t steps = s[msg.getNrDozownika()].home(); 
        msg.setHomeDone(steps);
        Serial.print("Done Home=");
        Serial.println(steps);
        digitalWrite(13, LOW);
      } else if (msg.getStatusWork() == msg.POS_START) {
        Serial.print("Pos=");
        Serial.println(msg.getSteps());
        digitalWrite(13, HIGH);
        uint32_t steps = s[msg.getNrDozownika()].start(msg.getSteps()); 
        msg.setPosDone(steps);
        Serial.print("Done pos=");
        Serial.println(steps);
        
        digitalWrite(13, LOW);
      } else {
        Serial.println("Nieznany rozkaz");
      }
    }
  } else {
    //Serial.print(".");
    Serial.println(digitalRead(LIMIT1PIN),DEC);
    delay(500);
  }
  
  #if 0
digitalWrite(DIR1PIN, LOW);
digitalWrite(DIR2PIN, LOW);
digitalWrite(DIR3PIN, LOW);
digitalWrite(DIR4PIN, LOW);
digitalWrite(DIR5PIN, LOW);

digitalWrite(ENPIN, HIGH);

  Serial.println("Silnik 5");
  for (int i = 0; i < 3000 ; i++) {
    digitalWrite(PULSE5PIN, HIGH);
    delayMicroseconds(200);
    digitalWrite(PULSE5PIN, LOW);
    delayMicroseconds(200);
  }
  delay(3000);
/*
  Serial.println("Silnik 2");
  for (int i = 0; i < 300 ; i++) {
    digitalWrite(PULSE2PIN, HIGH);
    delay(1);
    digitalWrite(PULSE2PIN, LOW);
    delay(1);
  }
  delay(300);

  Serial.println("Silnik 3");
  for (int i = 0; i < 300 ; i++) {
    digitalWrite(PULSE3PIN, HIGH);
    delay(1);
    digitalWrite(PULSE3PIN, LOW);
    delay(1);
  }
  delay(300);

  Serial.println("Silnik 4");
  for (int i = 0; i < 300 ; i++) {
    digitalWrite(PULSE4PIN, HIGH);
    delay(1);
    digitalWrite(PULSE4PIN, LOW);
    delay(1);
  }
  delay(300);

  Serial.println("Silnik 5");
  for (int i = 0; i < 300 ; i++) {
    digitalWrite(PULSE5PIN, HIGH);
    delay(1);
    digitalWrite(PULSE5PIN, LOW);
    delay(1);
  }
  delay(300);
*/
#endif
}


void interS1() {
  s[0].interruptFun();
}



void interS2() {
  //Serial.println("I2");
  s[1].interruptFun();
}

void interS3() {
  //Serial.println("I3");
  s[2].interruptFun();
}

void interS4() {
  //Serial.println("I4");
  s[3].interruptFun();
}

void interS5() {
  //Serial.println("I5");
  s[4].interruptFun();
}
