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
	//must init for using LCD and anything that contain function inside "System"
	//use tick
	//...
	//Car *trycar;
//	pGrapher grapher;
	RunMode trytryhaha;


	char message[22];
	int8_t encoderZeroCount =0;

//code for ploting graph for a equation of y = mx +c, where y and x are encoder counting or motor PWM
//uncomment for usage

	//tune encoder here
	//to uncomment this code, comment all pVarManager object
//	JyMcuBt106::Config config;
//	config.id = 0;
//	config.baud_rate = libbase::k60::Uart::Config::BaudRate::k115200;
//	config.rx_irq_threshold = 2;
//	JyMcuBt106 fuck(config);
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
	int8_t counter=0;
	uint16_t distance;
	int8_t value;
	int8_t state;
	int16_t degree;
	int16_t looptime=0;
	trytryhaha.servo_control(820);
	trytryhaha.motor_control(100,1);
	int16_t count1=0;
	bool reverseFlag=0;
	int16_t count2=0;

//
//	grapher.addSharedVar(&trytryhaha.s_lkp,"s_lkd");
//	grapher.addSharedVar(&trytryhaha.s_lkp,"s_lkp");
//	grapher.addSharedVar(&trytryhaha.m_Kp,"m_Kp");
//	grapher.addSharedVar(&trytryhaha.m_Ki,"m_Ki");
//	grapher.addSharedVar(&trytryhaha.m_kd,"m_kd");

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
		}

		if((int16_t)(cur_Time - cur_Time1) > 10){
			looptime++;
			cur_Time1 = cur_Time;
//			grapher.sendWatchData();
			trytryhaha.get_raw_image();
			trytryhaha.imageCorrection(trytryhaha.data);
//			if(!trytryhaha.getbutton(0)){
				trytryhaha.printRawCamGraph(0,0);
//			}


			value = trytryhaha.CheckLightIndex(trytryhaha.data);
			//if state>=4 : beacon not found, run second camera
			state = trytryhaha.get_RState(value);
//			if(state>3){
//				//run 2nd camera:
//				//CheckLightIndex on 2nd camera base on 2nd cam data;
//				int8_t Secondvalue = trytryhaha.CheckLightIndex(trytryhaha.data);
//
//				if(state<4){
//					//reverse car untill beacon not found i.e. until state>3
//					reverseFlag=true;
//					int8_t SecondState =0;
//					SecondState = trytryhaha.get_RState(Secondvalue);
//					//...2ndCameraRunState
//					trytryhaha.RUN_2ndCAMSTATE(SecondState);
//				}
//			}
			reverseFlag = false;
			if(counter <1 && reverseFlag==false)trytryhaha.RUN_STATE(state);
			if(looptime>2699){
				looptime=0;
			}

//			int16_t val = (int16_t) value;
//			int16_t lightindex = (int16_t) trytryhaha.LightIndex[trytryhaha.rowIndex];
		}
			//PRINT MID, DEGREE & MARGIN
			sprintf(message, "L mid @%d", value);
			trytryhaha.printvalue(0, 68, 128,40, message);
			sprintf(message, "state %d", state);
			trytryhaha.printvalue(0, 132, 128,40, message);
			sprintf(message, "Degree @%d", trytryhaha.ideal_servo_degree);
			trytryhaha.printCar(message, 116);
			sprintf(message, "L%d R%d", trytryhaha.l_margin, trytryhaha.r_margin);
			trytryhaha.printvalue(0, 84, 128,40, message);
			//hahaha

		//System::DelayMs(10);
		//if((int16_t)(cur_Time-cur_Time2) > 30){
		if((looptime%3)==1){
			cur_Time2 = cur_Time;
			trytryhaha.update_encoder();
			if(counter >1){
				// use to delay and ignore code below
				counter--;

			}else if( counter == 1){
				// last loop to reverse,
				//problem:
				// encoder count is negative for next loop, as to pass into pid, need a "normal count"
				//method1:
				//set motor without pid with ideal speed
				trytryhaha.motor_control((trytryhaha.Ideal_encoder_count+345.504)/17.308,1);
				counter--;
			}
			else{
				if(trytryhaha.get_encoder_count()==0){
					counter=20;
					trytryhaha.motor_control(180,false);
					if(trytryhaha.ideal_servo_degree >820) trytryhaha.servo_control(820-150);
					else trytryhaha.servo_control(820+150);

				}
				else{
					trytryhaha.motor_control(trytryhaha.motorPID(trytryhaha.Ideal_encoder_count), 1);
				}
			}
		}

			//trytryhaha.encoder->Update();
	}



//	for(;;){}
	//trycar->cam->Stop();
	return 0;
}
