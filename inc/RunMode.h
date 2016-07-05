/*
 * RunMode.h
 *
 *  Created on: 2016�~2��20��
 *      Author: yungc
 */

#pragma once
#include "car.h"

class RunMode: public Car
{
	// tips for choosing datatype, dont use too large( it will slow down your program)
	//and dont use too small (otherwise underflow occur)
	// you may check 8bit,16bit,and 32bit int & uint range fromthe link:
	//			http://www.societyofrobots.com/member_tutorials/book/export/html/341
	//or just google that if u are not sure
public:


	RunMode();
	~RunMode();

	int16_t turningPID (int16_t mid_line, int16_t value);
	//PID = kp *error +kd *(error_prev - error) + ki * sum of error, in smartcar, ki can be neglected as its too small

	int16_t motorPID (int16_t ideal_encoder_count);
	//PID = kp *error +kd *(error_prev - error) + ki * sum of error, in smartcar, ki can be neglected as its too small

	void motor_control(uint16_t power, bool is_clockwise_rotating);	//0~1000
	//apply motor PID to improve the acceleration

	void servo_control(int16_t degree); //-90 to 90

	void update_encoder();
	int32_t get_encoder_count();
	//for "GetCount()" of encoder, it return the difference of value between two update
	//i.e. update ->process-with-2s->update->GetCount, the result is the encoder count within that process with 2s

//	RunState get_RunState();
//	void turnavoid(RunState);

	//--------------------------variable below---------------------------//
	//to access the public variable, you can use (obj_name).(var_name) to access

	int16_t ideal_servo_degree, ideal_motor_speed;
	int16_t actual_encoder_target = 0;
	int32_t encoder_count;
	int16_t servoErr, servoPrevErr;
	int16_t motorErr, motorPrevErr;
	int16_t motorSum = 0;
	int16_t Ideal_encoder_count = 0;
	//int8_t encoderPrevZeroCount;
	int8_t encoderZeroCount;
	float s_lkp;
	float s_lkd;
	float s_rkp;
	float s_rkd;

	float m_Kp;
	float m_Ki;
	float m_kd;

	bool is_clockwise = true;
	float temp=0;



private: //yes, I add these variable as private, because they are not important
	// Moreover, variable can be declare in header(.h), and define in either header(.h) or source(.cpp)
	const int16_t maxServoAngle; 				//820+170
	const int16_t minServoAngle; 				//820-170;		// give a maximun& minimun angle for servo to turn
	const int16_t maxMotorSpeed;
	const int16_t minMotorSpeed; 					// give a maximun& minimun PWM for motor to run

};
