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

		Silnik(uint8_t dir, uint8_t pulse, uint8_t limit, uint8_t home);
		~Silnik();

		void init();
		uint32_t start(uint32_t steps);
		void stop();
		uint32_t home();

		void setHomeLevel(uint8_t hlevel) { homeLevel = hlevel; }


		static uint32_t maxSteps;
		static uint16_t mls_motor;
		bool check(uint8_t tab[], uint8_t s);
private:
	const uint8_t dirPin;
	const uint8_t pulsePin;
	const uint8_t limitPin;
	uint8_t homeLevel;
	uint16_t stepsPos;

	
	

};
#endif