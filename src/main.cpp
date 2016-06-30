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

//code for ploting graph for a equation of y = mx +c, where y and x are encoder counting or motor PWM
//uncomment for usage
/*
	//tune encoder here
	//to uncomment this code, comment all pVarManager object
	JyMcuBt106::Config config;
	config.id = 0;
	config.baud_rate = libbase::k60::Uart::Config::BaudRate::k115200;
	config.rx_irq_threshold = 2;
	JyMcuBt106 fuck(config);
	char *PWM_buffer = new char[120]{0};
	float encoder_counting = 0;
	int motor_speed =0;
 while(1){
	 motor_speed += 1;
	 Run.motor_control(motor_speed,true);
	 Run.update_encoder();
	 System::DelayMs(30);
	 Run.update_encoder();

	 encoder_counting = Run.get_encoder_count();
	 int n = sprintf(PWM_buffer,"%d %d \n",(int)motor_speed,(int)encoder_counting);
	 fuck.SendBuffer((Byte*)PWM_buffer,n);
	 memset(PWM_buffer,0,n);
	 if (motor_speed > 500) {	 Run.motor_control(0,true);while(1);}
	 System::DelayMs(20);
 }
 */


	//-------------------------------------your code below----------------------------------------//

	System::Init();
	//must init for using LCD and anything that contain function inside "System"
	//use tick
	//...
	//Car *trycar;
	RunMode trytryhaha;
	Timer::TimerInt cur_Time=0;
	Timer::TimerInt pastTime=0;
	char message[22];
	uint16_t distance;
	int8_t value;
	int8_t state;
	int16_t degree;
	trytryhaha.servo_control(900);
	int16_t count1=0;
	int16_t count2=0;

//Test Ticks & Servo
	while(1){
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
		trytryhaha.printvalue(0, 68, 128,40, "820-160");
		trytryhaha.servo_control(820-160);
		System::DelayMs(1200);
		trytryhaha.printvalue(0, 68, 128,40, "820");
		trytryhaha.servo_control(820);
		System::DelayMs(1200);
		trytryhaha.printvalue(0, 68, 128,40, "820+170");
		trytryhaha.servo_control(820+170);
		System::DelayMs(1200);
		trytryhaha.printvalue(0, 68, 128,40, "820");
		trytryhaha.servo_control(820);
		System::DelayMs(1200);

	}

	while(1){

		/*
		int16_t count1=0;
		int16_t count2=0;
		if(cur_Time!=System::Time()){
			cur_Time = System::Time();
			if((int32_t) (cur_Time - pastTime) >=20){
				pastTime = cur_Time;
				count1+=1;
				sprintf(message, "count20 %d", count1);
				trytryhaha.printvalue(0, 132, 128,40, message);
			}
			if((int32_t) (cur_Time - pastTime) >=10){
				pastTime= cur_Time;
				count2+=1;
				sprintf(message, "count10 %d", count2);
				trytryhaha.printvalue(0, 132, 128,40, message);
			}

		}
		*/
		if(cur_Time!=System::Time()){

		//JUST PRINT RAW:
		cur_Time = System::Time();
		if((int16_t)(cur_Time%10)==0){
			trytryhaha.get_raw_image();
			trytryhaha.imageCorrection(trytryhaha.data);
			trytryhaha.printRawCamGraph(0,0);

			value = trytryhaha.CheckLightIndex(trytryhaha.data);
			state = trytryhaha.get_RState();

			trytryhaha.RUN_STATE(state);

			int16_t val = (int16_t) value;
			int16_t lightindex = (int16_t) trytryhaha.LightIndex[trytryhaha.rowIndex];

			//PRINT MID, DEGREE & MARGIN
			sprintf(message, "L mid @%d", value);
			trytryhaha.printvalue(0, 68, 128,40, message);
			sprintf(message, "state %d", state);
			trytryhaha.printvalue(0, 132, 128,40, message);
	//		sprintf(message, "Degree @%d", degree);
	//		trytryhaha.printCar(message, 132);
			sprintf(message, "L%d R%d", trytryhaha.l_margin, trytryhaha.r_margin);
			trytryhaha.printvalue(0, 84, 128,40, message);

		//System::DelayMs(10);
		}
		}

	}

//	for(;;){}
	trytryhaha.~RunMode();
	//trycar->cam->Stop();
	return 0;
}
