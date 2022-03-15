#include "silnik.hpp"  

unsigned int Silnik::maxSteps = 0;
bool Silnik::reverse;

Silnik::Silnik(uint16_t mlstime, uint8_t dir, uint8_t pulse, uint8_t limPin)
: mls_motor(mlstime), dirPin(dir), pulsePin(pulse), limitPin(limPin), stopNow(false)

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
    //pinMode(limitPIN, INPUT_PU);
}

uint32_t Silnik::start(uint32_t steps)
{
    Serial.print("dirPin=");
    Serial.println(goForward(true));
    digitalWrite(dirPin , goForward(true));
    if (steps < 0)
        steps = -steps;
    uint32_t s = 0;    
    for (; s < steps && s < maxSteps /* && !stopNow */; s++) {
        digitalWrite(pulsePin, HIGH);    
        delayMicroseconds(mls_motor);
        digitalWrite(pulsePin, LOW);        
        delayMicroseconds(mls_motor);
    }
    stop();
    return s;
}

uint32_t Silnik::home()
{
    //interrupts();
    Serial.print("dirPin=LOW");
    //Serial.println(goBack(true));
    digitalWrite(dirPin , HIGH);
    //digitalWrite(enPin, LOW);
    stopNow = 0;
    uint32_t s = 0;
    Serial.print("stopNow=");
    Serial.println(stopNow,DEC);
    for (; s < maxSteps /*&& digitalRead(limitPin) == HIGH*/; s++) {
        digitalWrite(pulsePin, HIGH);
        delayMicroseconds(mls_motor);
        digitalWrite(pulsePin, LOW);
        delayMicroseconds(mls_motor);
    }
    Serial.print("stopNow=");
    Serial.println(stopNow,DEC);
    stop();
    //noInterrupts();
    return s;
}

void Silnik::stop()
{
    stopNow = true;
}

uint8_t Silnik::goBack(bool val) const
{
    return ((reverse ^ val) ? HIGH : LOW);
}

