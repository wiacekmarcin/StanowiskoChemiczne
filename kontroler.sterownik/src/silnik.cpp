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

uint32_t Silnik::start(uint32_t steps)
{
    attachInterrupt(digitalPinToInterrupt(limitPin), intFunPtr, FALLING);
    digitalWrite(dirPin , goForward(true));
    digitalWrite(enPin, LOW);
    stopNow = false;
    if (steps < 0)
        steps = -steps;
    uint32_t s = 0;    
    for (; s < steps && s < maxSteps /* && !stopNow */; s++) {
        digitalWrite(13, HIGH);
        digitalWrite(pulsePin, HIGH);
        delay(5);
        digitalWrite(13, LOW);
        digitalWrite(pulsePin, LOW);
        delay(5);
    }
    stop();
    digitalWrite(dirPin , goForward(false));
    return s;
}

uint32_t Silnik::home()
{
    attachInterrupt(digitalPinToInterrupt(limitPin), intFunPtr, FALLING);
    digitalWrite(dirPin , goBack(true));
    digitalWrite(enPin, LOW);
    stopNow = false;
    uint32_t s = 0;
    for (; s < maxSteps && !stopNow; s++) {
        digitalWrite(pulsePin, HIGH);
        delay(5);
        digitalWrite(pulsePin, LOW);
        delay(5);
    }
    stop();
    digitalWrite(dirPin , goBack(false));
    return s;
}

void Silnik::stop()
{
    detachInterrupt(digitalPinToInterrupt(limitPin));
    digitalWrite(enPin, HIGH);
}

uint8_t Silnik::goBack(bool val) const
{
    return ((reverse ^ val) ? HIGH : LOW);
}

