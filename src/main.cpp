/*
 * main.cpp
 *
 * Author: Kitty Kwok
 * Copyright (c) 2015-2016 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cassert>
#include <cstring>
#include <libsc/system.h>
#include <stdint.h>
#include "pVarManager.h"
#include "car.h"
#include "RunMode.h"
#include <libutil/looper.h>


using namespace libsc;

using namespace libbase::k60;

using namespace libutil;


int main(void)
{
	int8_t encoderZeroCount =0;
	System::Init();
	//must init for using LCD and anything that contain function inside "System"
	//use tick
	//...
	//Car *trycar;
	RunMode trytryhaha;
	char message[22];

//code for ploting graph for a equation of y = mx +c, where y and x are encoder counting or motor PWM
//uncomment for usage

	//tune encoder here
	//to uncomment this code, comment all pVarManager object
	JyMcuBt106::Config config;
	config.id = 1;
	config.baud_rate = libbase::k60::Uart::Config::BaudRate::k115200;
	config.rx_irq_threshold = 2;
	JyMcuBt106 fuck(config);
//	char *PWM_buffer = new char[120]{0};
//	float encoder_counting = 0;
//	int motor_speed =0;
//	char message[22];
//	System::DelayMs(5000);
// while(1){
//	 motor_speed += 1;
//	 trytryhaha.motor_control(motor_speed,true);
//	 trytryhaha.update_encoder();
//	 System::DelayMs(30);
//	 trytryhaha.update_encoder();
//
//	 encoder_counting = trytryhaha.get_encoder_count();
//	 int n = sprintf(PWM_buffer,"%d %d \n",(int)motor_speed,(int)encoder_counting);
//	 fuck.SendBuffer((Byte*)PWM_buffer,n);
//	 memset(PWM_buffer,0,n);
//	 if (motor_speed > 350) {	 trytryhaha.motor_control(0,true);while(1);}
//	 System::DelayMs(20);
// }

 /*
  * Encoder motor formula:
  * Encoder_count = 17.308 * motor - 345.504
  */


	//-------------------------------------your code below----------------------------------------//

//	System::Init();
	//must init for using LCD and anything that contain function inside "System"
	//use tick
	//...
	//Car *trycar;
//	RunMode trytryhaha;
	Timer::TimerInt cur_Time=0;
	Timer::TimerInt cur_Time1=0;
	Timer::TimerInt cur_Time2=0;
	Timer::TimerInt pastTime=0;
	uint16_t distance;
	int8_t value;
	int8_t state;
	int16_t degree;
	trytryhaha.servo_control(820);
	trytryhaha.motor_control(80,1);
	int16_t count1=0;
	int16_t count2=0;

//Test Ticks & Servo
//	while(1){
//		sprintf(message,"Time: %d", (System::Time())/1000);
//		trytryhaha.printvalue(0, 80, 128,40, message);
//
//		if(cur_Time!=System::Time()){
//			if((int16_t) (cur_Time%50) ==0){
//				//pastTime = cur_Time;
//				count1+=1;
//			}
//			if((int16_t) (cur_Time%20) ==0){
//				count2+=1;
//			}
//		}
//		820
//		trytryhaha.printvalue(0, 68, 128,40, "820-160");
//		trytryhaha.servo_control(820-160);
//		System::DelayMs(1200);
//		trytryhaha.printvalue(0, 68, 128,40, "820");
//		trytryhaha.servo_control(820);
//		System::DelayMs(1200);
//		trytryhaha.printvalue(0, 68, 128,40, "820+170");
//		trytryhaha.servo_control(820+170);
//		System::DelayMs(1200);
//		trytryhaha.printvalue(0, 68, 128,40, "820");
//		trytryhaha.servo_control(820);
//		System::DelayMs(1200);
//
//	}

	while(1){
		if(cur_Time!=System::Time()){

		//JUST PRINT RAW:
		cur_Time = System::Time();

		if((int16_t)(cur_Time - cur_Time1) > 10){
			cur_Time1 = cur_Time;
			trytryhaha.get_raw_image();
			trytryhaha.imageCorrection(trytryhaha.data);
			trytryhaha.printRawCamGraph(0,0);

			value = trytryhaha.CheckLightIndex(trytryhaha.data);
			state = trytryhaha.get_RState();

			trytryhaha.RUN_STATE(state);

			int16_t val = (int16_t) value;
			int16_t lightindex = (int16_t) trytryhaha.LightIndex[trytryhaha.rowIndex];

			//PRINT MID, DEGREE & MARGIN
//			sprintf(message, "L mid @%d", value);
//			trytryhaha.printvalue(0, 68, 128,40, message);
//			sprintf(message, "state %d", state);
//			trytryhaha.printvalue(0, 132, 128,40, message);
//			sprintf(message, "Degree @%d", trytryhaha.ideal_servo_degree);
//			trytryhaha.printCar(message, 116);
//			sprintf(message, "Speed %d", trytryhaha.ideal_motor_speed);
//			trytryhaha.printvalue(0, 84, 128,40, message);

		//System::DelayMs(10);
		}

		if((int16_t)(cur_Time-cur_Time2) > 30){
			cur_Time2 = cur_Time;
			trytryhaha.update_encoder();
			trytryhaha.motor_control(trytryhaha.motorPID(trytryhaha.Ideal_encoder_count), 1);

		if(!trytryhaha.get_encoder_count()){
			encoderZeroCount++;
		}else encoderZeroCount = 0;

		if (encoderZeroCount > 1){
			trytryhaha.motor_control(180,false);
			if(trytryhaha.ideal_servo_degree >820) trytryhaha.servo_control(820-150);
			else trytryhaha.servo_control(820+150);

		System::DelayMs(600);
		trytryhaha.update_encoder();
		trytryhaha.motor_control(trytryhaha.ideal_motor_speed,true);
		trytryhaha.servo_control(trytryhaha.ideal_servo_degree);

		}
			//trytryhaha.encoder->Update();
		}

	}
	}

//	for(;;){}
	//trycar->cam->Stop();
	return 0;
}
