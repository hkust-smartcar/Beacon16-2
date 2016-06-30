/*
 * ultrasonic.cpp
 *
 *  Created on: 21 Jun 2016
 *      Author: kittylgnkwok
 */

#include "ultrasonic.h"
#include "car.h"

ultrasonic::ultrasonic() {
	// TODO Auto-generated constructor stub
	U_DISTANCE=0;
}

ultrasonic::~ultrasonic() {

}

uint16_t ultrasonic::GetDistance(){

	return U_DISTANCE;
}

