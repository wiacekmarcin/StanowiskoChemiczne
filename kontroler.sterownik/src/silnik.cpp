#include "silnik.hpp"  

unsigned int Silnik::maxSteps = 0;
Silnik::Silnik(uint8_t en, uint8_t dir, uint8_t pulse, uint8_t limit, void (*intFun)())
: enPin(en), dirPin(dir), pulsePin(pulse), limitPin(limit), stopNow(false), intFunPtr(intFun)

{
	//interrupt = &Silnik::interruptFun;
}
	
Silnik::~Silnik()
{
	
}

void Silnik::init() 
{
    pinMode(dirPin, OUTPUT);
    pinMode(pulsePin, OUTPUT);
    pinMode(limitPin, INPUT);
    //attachInterrupt(digitalPinToInterrupt(limitPin), [this](){ interruptFun(); }, CHANGE);
}

void Silnik::start(int steps)
{
    attachInterrupt(digitalPinToInterrupt(limitPin), intFunPtr, FALLING);
    digitalWrite(dirPin , steps > 0 ? HIGH : LOW);
    digitalWrite(enPin, HIGH);
    stopNow = false;
    if (steps < 0)
        steps = -steps;
    for (int s = 0; s < steps && s < maxSteps && !stopNow; s++) {
        digitalWrite(pulsePin, LOW);
        delay(100);
        digitalWrite(pulsePin, HIGH);
        delay(100);
    }
}

void Silnik::stop()
{
    detachInterrupt(digitalPinToInterrupt(limitPin));
    digitalWrite(enPin, LOW);
}