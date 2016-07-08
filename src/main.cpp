/*
 * main.cpp
 *
 * Author: Thomas
 * Copyright (c) 2015-2016 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cassert>
#include <cstring>
#include <libsc/system.h>
#include <stdint.h>
#include "car.h"

using namespace libsc;

using namespace libbase::k60;

using namespace libutil;


int main(void)
{

	System::Init();

	Car car;

	Timer::TimerInt t = System::Time();

	while (1) {
		if (t == System::Time()) continue;
		t = System::Time();
		if (t % 500 == 0) {
			car.led0.Turn();
		}
	}
	return 0;
}
