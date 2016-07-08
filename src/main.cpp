/*
 * main.cpp
 *
 * Author: Kitty Kwok
 * Copyright (c) 2015-2016 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cassert>
#include <libutil/pGrapher.h>
#include <cstring>
#include <libsc/system.h>
#include <stdint.h>
#include "car.h"
#include "RunMode.h"
#include <libutil/looper.h>



using namespace libsc;

using namespace libbase::k60;

using namespace libutil;


int main(void)
{

	System::Init();

	RunMode trytryhaha;

	Timer::TimerInt t = System::Time();

	while (1) {
		if (t == System::Time()) continue;
		t = System::Time();
		if (t % 500 == 0) {
			trytryhaha.switchLED(1);
			trytryhaha.switchLED(2);
			trytryhaha.switchLED(3);
			trytryhaha.switchLED(4);
		}
	}
// 	char message[22];
// 	int8_t encoderZeroCount =0;

// 	Timer::TimerInt cur_Time=0;
// 	Timer::TimerInt cur_Time1=0;
// 	Timer::TimerInt cur_Time2=0;
// 	Timer::TimerInt pastTime=0;
// 	int8_t counter=0;
// 	uint16_t distance;
// 	int8_t value;
// 	int8_t state;
// 	int16_t degree;
// 	int16_t looptime=0;
// 	trytryhaha.servo_control(820);
// 	trytryhaha.motor_control(100,1);
// 	int16_t count1=0;
// 	bool reverseFlag=0;
// 	int16_t count2=0;



// 	while(1){

// 		if(cur_Time!=System::Time()){

// 		//JUST PRINT RAW:
// 		cur_Time = System::Time();
// 		}

// 		if((int16_t)(cur_Time - cur_Time1) > 10){
// 			looptime++;
// 			cur_Time1 = cur_Time;
// //			grapher.sendWatchData();
// 			trytryhaha.get_raw_image();
// 			trytryhaha.imageCorrection(trytryhaha.data);
// //			if(!trytryhaha.getbutton(0)){
// 				trytryhaha.printRawCamGraph(0,0);
// //			}


// 			value = trytryhaha.CheckLightIndex(trytryhaha.data);
// 			//if state>=4 : beacon not found, run second camera
// 			state = trytryhaha.get_RState(value);

// 			reverseFlag = false;
// 			if(counter <1 && reverseFlag==false)trytryhaha.RUN_STATE(state);
// 			if(looptime>2699){
// 				looptime=0;
// 			}

// //			int16_t val = (int16_t) value;
// //			int16_t lightindex = (int16_t) trytryhaha.LightIndex[trytryhaha.rowIndex];
// 		}
// 			//PRINT MID, DEGREE & MARGIN
// 			sprintf(message, "L mid @%d", value);
// 			trytryhaha.printvalue(0, 68, 128,40, message);
// 			sprintf(message, "state %d", state);
// 			trytryhaha.printvalue(0, 132, 128,40, message);
// 			sprintf(message, "Degree @%d", trytryhaha.ideal_servo_degree);
// 			trytryhaha.printCar(message, 116);
// 			sprintf(message, "L%d R%d", trytryhaha.l_margin, trytryhaha.r_margin);
// 			trytryhaha.printvalue(0, 84, 128,40, message);
// 			//hahaha

// 		//System::DelayMs(10);
// 		//if((int16_t)(cur_Time-cur_Time2) > 30){
// 		if((looptime%3)==1){
// 			cur_Time2 = cur_Time;
// 			trytryhaha.update_encoder();
// 			if(counter >1){
// 				// use to delay and ignore code below
// 				counter--;

// 			}else if( counter == 1){
// 				// last loop to reverse,
// 				//problem:
// 				// encoder count is negative for next loop, as to pass into pid, need a "normal count"
// 				//method1:
// 				//set motor without pid with ideal speed
// 				trytryhaha.motor_control((trytryhaha.Ideal_encoder_count+345.504)/17.308,1);
// 				counter--;
// 			}
// 			else{
// 				if(trytryhaha.get_encoder_count()==0){
// 					counter=20;
// 					trytryhaha.motor_control(180,false);
// 					if(trytryhaha.ideal_servo_degree >820) trytryhaha.servo_control(820-150);
// 					else trytryhaha.servo_control(820+150);

// 				}
// 				else{
// 					trytryhaha.motor_control(trytryhaha.motorPID(trytryhaha.Ideal_encoder_count), 1);
// 				}
// 			}
// 		}

// 			//trytryhaha.encoder->Update();
// 	}


	return 0;
}
