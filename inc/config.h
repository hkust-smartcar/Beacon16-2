/*
 * config.h
 *
 *  Created on: 
 *      Author: yungc
 */

#pragma once

//put all config here

#include <functional>
#include <libsc/system.h>
#include <libbase/k60/mcg.h>
#include <libsc/ab_encoder.h>
#include <libsc/trs_d05.h>
#include <libsc/k60/ov7725.h>
#include <libsc/st7735r.h>
#include <libsc/dir_motor.h>
#include <libsc/us_100.h>
#include <libsc/config.h>

using namespace libsc;

namespace libbase
{
	namespace k60
	{

		Mcg::Config Mcg::GetMcgConfig()
		{
			Mcg::Config config;
			config.external_oscillator_khz = 50000;
			config.core_clock_khz = 150000;
			return config;
		}

	}
}

using namespace libbase::k60;
using namespace libsc;

// LED
Gpo::Config GetLedConfig(uint8_t id) {
	Gpo::Config config;
	config.is_high = false;
	switch (id) {
		case 0: config.pin = LIBSC_LED0; break;
		case 1: config.pin = LIBSC_LED1; break;
		case 2: config.pin = LIBSC_LED2; break;
		case 3: config.pin = LIBSC_LED3; break;
		default: config.pin = LIBSC_LED0; break;
	}
	return config;
}

// servo
TrsD05::Config GetServoConfig()
{
	TrsD05::Config config;
	config.id = 0;
	return config;
}

// //encoder
// AbEncoder::Config GetAbEncoderConfig()
// {
// 	AbEncoder::Config EncoderConfig;
// 	EncoderConfig.id = 0;
// 	return EncoderConfig;
// }

// //AlternateMotor::Config GetAltmotorConfig()
// //{
// //	AlternateMotor::Config AltmotorConfig;
// //	AltmotorConfig.id = 0;
// //	return AltmotorConfig;
// //}


// DirMotor::Config GetDirmotorConfig()
// {
// 	DirMotor::Config DirmotorConfig;
// 	DirmotorConfig.id = 0;
// 	return DirmotorConfig;
// }

// Us100::Config GetUs100Config()
// {
// 	Us100::Config Us100Config;
// 	Us100Config.id=0;
// 	return Us100Config;
// }


// uint8_t GetCcdConfig()
// {

// 	return 0;
// }

// Button::Config GetButton1Config()
// {

// 	Button::Config Button1Config;
// 	Button1Config.id = 0;
// 	Button1Config.is_active_low = true;
// 	Button1Config.is_use_pull_resistor = false;
// //	Button1Config.listener_trigger = Button::Config::Trigger::kDown;
// //	Button1Config.listener = &blinkLed;
// 	//TODO add ISR for Button
// 	return Button1Config;
// }

// Button::Config GetButton2Config()
// {
// 	Button::Config Button2Config;
// 	Button2Config.id = 1;
// 	Button2Config.is_active_low = true;
// 	Button2Config.is_use_pull_resistor = false;
// 	return Button2Config;
// }

// Joystick::Config GetJoystickConfig()
// {
// 	Joystick::Config JoystickConfig;
// 	JoystickConfig.id = 0;
// 	JoystickConfig.is_active_low = true;
// //	JoystickConfig.listener_triggers[static_cast<int>(Joystick::State::kDown)] =
// //			Joystick::Config::Trigger::kDown;
// //	JoystickConfig.listeners[static_cast<int>(Joystick::State::kDown)] =
// //						[&](const uint8_t)
// //				{};
// 	//TODO add ISR for every joystick state
// 	return JoystickConfig;
// }

// St7735r::Config GetLcdConfig()
// {
// 	St7735r::Config LcdConfig;
// 	LcdConfig.is_revert = false;
// 	LcdConfig.is_bgr = false;
// 	return LcdConfig;
// }

// SimpleBuzzer::Config GetBuzzerConfig()
// {
// 	SimpleBuzzer::Config BuzzerConfig;
// 	BuzzerConfig.id = 0;
// 	BuzzerConfig.is_active_low = false;
// 	return BuzzerConfig;
// }

// k60::Ov7725::Config GetCamConfig(){
// 	k60::Ov7725::Config camcfg;
// 	camcfg.id=1;
// 	camcfg.w=80;
// 	camcfg.h=60;
// 	camcfg.fps=k60::Ov7725::Config::Fps::kHigh;
// 	camcfg.contrast = 0x35;
// 	camcfg.brightness=0x00;
// 	return camcfg;
// }
