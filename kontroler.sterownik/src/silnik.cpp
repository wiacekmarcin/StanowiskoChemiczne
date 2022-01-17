#include "silnik.hpp"  

unsigned int Silnik::maxSteps = 0;
bool Silnik::reverse;

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

uint32_t Silnik::start(int32_t steps)
{
    attachInterrupt(digitalPinToInterrupt(limitPin), intFunPtr, FALLING);
    digitalWrite(dirPin , steps > 0 ? HIGH : LOW);
    digitalWrite(enPin, HIGH);
    stopNow = false;
    if (steps < 0)
        steps = -steps;
    uint32_t usteps = steps;    
    uint32_t s = 0;    
    for (; s < usteps && s < maxSteps && !stopNow; s++) {
        digitalWrite(pulsePin, LOW);
        delay(100);
        digitalWrite(pulsePin, HIGH);
        delay(100);
    }
    stop();
    return s;
}

uint32_t Silnik::home()
{
    attachInterrupt(digitalPinToInterrupt(limitPin), intFunPtr, FALLING);
    digitalWrite(dirPin , LOW);
    digitalWrite(enPin, HIGH);
    stopNow = false;
    uint32_t s = 0;
    for (; s < maxSteps && !stopNow; s++) {
        digitalWrite(pulsePin, LOW);
        delay(100);
        digitalWrite(pulsePin, HIGH);
        delay(100);
    }
    stop();
    return s;
}

void Silnik::stop()
{
    detachInterrupt(digitalPinToInterrupt(limitPin));
    digitalWrite(enPin, LOW);
}