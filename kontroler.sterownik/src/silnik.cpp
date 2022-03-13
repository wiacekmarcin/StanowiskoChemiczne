#include "silnik.hpp"  

unsigned int Silnik::maxSteps = 0;
bool Silnik::reverse;

Silnik::Silnik(uint16_t mlstime, uint8_t en, uint8_t dir, uint8_t pulse, uint8_t limit, void (*intFun)())
: mls_motor(mlstime), enPin(en), dirPin(dir), pulsePin(pulse), limitPin(limit), stopNow(false), intFunPtr(intFun)

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
    //digitalWrite(enPin, LOW);
    stopNow = false;
    if (steps < 0)
        steps = -steps;
    uint32_t s = 0;    
    digitalWrite(13, HIGH);
    
    for (; s < steps && s < maxSteps /* && !stopNow */; s++) {
        digitalWrite(pulsePin, HIGH);    
        delay(mls_motor);
        digitalWrite(pulsePin, LOW);        
        delay(mls_motor);
    }
    digitalWrite(13, LOW);
    stop();
    digitalWrite(dirPin , goForward(false));
    return s;
}

uint32_t Silnik::home()
{
    attachInterrupt(digitalPinToInterrupt(limitPin), intFunPtr, FALLING);
    digitalWrite(dirPin , goBack(true));
    //digitalWrite(enPin, LOW);
    stopNow = false;
    uint32_t s = 0;
    digitalWrite(13, HIGH);
    for (; s < maxSteps && !stopNow; s++) {
        digitalWrite(pulsePin, HIGH);
        delay(mls_motor);
        digitalWrite(pulsePin, LOW);
        delay(mls_motor);
    }
    digitalWrite(13, LOW);
    stop();
    digitalWrite(dirPin , goBack(false));
    return s;
}

void Silnik::stop()
{
    detachInterrupt(digitalPinToInterrupt(limitPin));
    //digitalWrite(enPin, HIGH);
}

uint8_t Silnik::goBack(bool val) const
{
    return ((reverse ^ val) ? HIGH : LOW);
}

