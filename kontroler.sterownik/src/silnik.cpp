#include "silnik.hpp"  

uint32_t Silnik::maxSteps = 55000;
uint16_t Silnik::mls_motor = 200;

Silnik::Silnik(uint8_t dir, uint8_t pulse, uint8_t limPin, uint8_t hLevel)
: dirPin(dir), pulsePin(pulse), limitPin(limPin), homeLevel(hLevel)

{
	stepsPos = 0;
}
	
Silnik::~Silnik()
{
	
}

void Silnik::init() 
{
    pinMode(dirPin, OUTPUT);
    pinMode(pulsePin, OUTPUT);
}

uint32_t Silnik::start(uint32_t steps)
{
    digitalWrite(dirPin , !homeLevel);
    uint32_t s = 0;    
    for (; s < steps && stepsPos < maxSteps; ++s, ++stepsPos) {
        digitalWrite(pulsePin, HIGH);    
        delayMicroseconds(mls_motor);
        digitalWrite(pulsePin, LOW);        
        delayMicroseconds(mls_motor);
    }
    return s;
}

uint32_t Silnik::home()
{
    digitalWrite(dirPin , homeLevel);
    uint8_t pinT[10] = {1,1,1,1,1,1,1,1,1,1};
    uint32_t s = 0;
    for (int i=0; i<8; i++) {
        pinT[i] = digitalRead(limitPin);
        delayMicroseconds(2*mls_motor);
    }

    for (; (s>>1) < maxSteps &&check(pinT, 8); ++s) {
        pinT[s & 0x7] = digitalRead(limitPin);
        digitalWrite(pulsePin, HIGH);
        delayMicroseconds(mls_motor);
        digitalWrite(pulsePin, LOW);
        delayMicroseconds(mls_motor);
    }
    stepsPos = 0;
    return s;
}

bool Silnik::check(uint8_t tab[], uint8_t s) {
  uint8_t * ptr = tab;
  uint8_t sum = 0;
  for (int i = 0; i < s; ++i) {
    sum+= *ptr++;
  }
  return sum > 0;
}


