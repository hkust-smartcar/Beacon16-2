/*
 * ultrasonic.h
 *
 *  Created on: 21 Jun 2016
 *      Author: kittylgnkwok
 */

#pragma once

#include <car.h>

class ultrasonic {
public:
	ultrasonic();
	virtual ~ultrasonic();

	uint16_t GetDistance();

	uint16_t U_DISTANCE;



protected:

};

