#ifndef SILNIK_H
#define SILNIK_H
#pragma once
	
#include "Arduino.h"	

class Silnik  
{
	private:

	public:

		/**
		 * @brief Construct a new Silnik object
		 * 
		 * @param en - pin dla enable silnika
		 * @param dir - pin dla dir silnika
		 * @param pls - pin dla pulse
		 * @param limit - pin dla limit
		 */

		Silnik(uint16_t mls_motor, uint8_t dir, uint8_t pulse, uint8_t limit);
		~Silnik();

		void init();
		uint32_t start(uint32_t steps);
		void stop();
		uint32_t home();

		inline void interruptFun() { noInterrupts(); ++stopNow; }
		static unsigned int maxSteps;
		static bool reverse;
		uint8_t goBack(bool val) const;
		uint8_t goForward(bool val) const { return ((goBack(val) == LOW) ? HIGH : LOW); } 
private:
	const uint16_t mls_motor;
	const uint8_t dirPin;
	const uint8_t pulsePin;
	const uint8_t limitPin;
	uint32_t stopNow;


	
	

};
#endif