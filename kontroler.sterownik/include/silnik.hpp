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

		Silnik(uint8_t en, uint8_t dir, uint8_t pulse, uint8_t limit, void (*intFun)());
		~Silnik();

		void init();
		void start(int steps);
		void stop();

		inline void interruptFun() { stopNow = true; }
		static unsigned int maxSteps;
private:
	const uint8_t enPin;
	const uint8_t dirPin;
	const uint8_t pulsePin;
	const uint8_t limitPin;
	volatile bool stopNow;
	void (*intFunPtr)();
	
	

};
#endif