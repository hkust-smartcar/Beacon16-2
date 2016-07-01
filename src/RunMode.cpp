/*
 * RunMode.cpp
 *
 *  Created on: 26-03-2016
 *      Author: Kyle
 */
#include "car.h"
#include "RunMode.h"
#define SERVO_MID 820


RunMode::RunMode():
servoErr(0), servoPrevErr(0), maxMotorSpeed(600), minMotorSpeed(0), ideal_servo_degree(0), encoder_count(0), motorErr(0){
	//can initialize the variable here,
}

RunMode::~RunMode(){

}

int16_t RunMode::turningPID (int16_t mid_line, int16_t value){
	//mid_line: current beacon mid, value: ideal turning position
	//int16_t degree; //max: 800+265, min: 800-265
	//int16_t temp;
	servoErr = mid_line-value;
	//float kp, ki, kd;

	s_lkp = 1.38f;
	s_rkp = 1.38f;
	s_lkd = 10.0f;
	s_rkd = 10.0f;

	//FORMULA: degree = kp*servoErr + kd*servoPrevErr
	if(mid_line >40){
		ideal_servo_degree = int16_t(SERVO_MID + s_lkp*abs(servoErr*servoErr) + s_lkd*(servoErr-servoPrevErr));
	}else if(mid_line < 41){
		ideal_servo_degree = int16_t(SERVO_MID - s_lkp*abs(servoErr*servoErr) - s_lkd*(servoErr-servoPrevErr));
	}

	servo->SetDegree(libutil::Clamp(minServoAngle, ideal_servo_degree, maxServoAngle));
	//OR servo_control(ideal_servo_degree);

	servoPrevErr = servoErr;

//	//value threshold shou be 30
//	if(value < 35){
//		motor_control(100,1);
//		if(mid_line > 80){
//			return SERVO_MID; //mid:800
//		}else{
//			if(mid_line > 40){
//				degree = 800 + 265 * mid_line / 40;
//			}
//			if(mid_line < 41){
//				degree = 800 - 265 * (40-mid_line)/40;
//				//degree = 650;
//			}
//		}
//	}else{
//		//BEACON IS INFRONT OF CAR
//		motor_control(0,1);
//		if(mid_line<40){
//			//turn very left in circle around beacon
//			degree = 800 - 265;
//
//		}else{
//			//turn very right in circle around beacon
//			degree = 800 + 265;
//
//		}
//	}

	sprintf(msg, "Degree @%d", ideal_servo_degree);
	printCar(msg, 132);
	return ideal_servo_degree;
	//return 0;//your implementation

}

int16_t RunMode::motorPID (int16_t ideal_encoder_count){

	m_Kp = 0.45f;
	m_Ki = 0.03f;

	encoder_count = get_encoder_count();
	if(encoder_count ==0){
		//Reverse turn at speed 80
		ideal_motor_speed=80;
		is_clockwise = false;
		ideal_servo_degree = (ideal_servo_degree > SERVO_MID) ? (SERVO_MID - (ideal_servo_degree-SERVO_MID)) : (SERVO_MID + ideal_servo_degree - SERVO_MID);
		servo_control(ideal_servo_degree);
		motor_control(ideal_motor_speed,is_clockwise);
		return ideal_motor_speed;
	}
	if(abs(encoder_count)>2200){
		encoder_count = ideal_motor_speed;
	}
	motorErr = (int16_t)(encoder_count - ideal_encoder_count);
	//relationship between motorerr & servoerr?

	ideal_motor_speed = ideal_motor_speed + m_Kp * (motorErr - motorPrevErr) + m_Ki * (motorErr);


	motorPrevErr = motorErr;

	return ideal_motor_speed;//your implementation
	// tips, remember to add something to protect your motor, for example:
	// e.g. 1) add a simple if-statement, if encoder count is near zero for 1~2s, stop the motor
	// e.g. 2) if the car are sure its crazy, stop motor
}

void RunMode::motor_control(uint16_t power, bool is_clockwise_rotating){
	if(power > maxMotorSpeed) power = maxMotorSpeed;
	if(power < minMotorSpeed) power = minMotorSpeed;
	motor->SetClockwise(is_clockwise_rotating);
	ideal_motor_speed = power;
	motor->SetPower(ideal_motor_speed);
}
//not a good method

void RunMode::servo_control(int16_t degree){
	if(degree > maxServoAngle ) degree = maxServoAngle;
	if(degree < minServoAngle ) degree = minServoAngle;
	ideal_servo_degree = degree + 900;
	//servo->SetDegree(ideal_servo_degree);
	servo->SetDegree(degree);
}

void RunMode::update_encoder(){
	encoder->Update();
}

int32_t RunMode::get_encoder_count(){
	encoder_count = -1 * encoder->GetCount();
	return encoder_count;
}

//RunMode::RunState RunMode::get_RunState(){
//	RunState state;
//	state = LOCATING;
//	if (distance_to_beacon){
//		if(distance_to_beacon < 15){
//			state = AVOID;
//		}else if(distance_to_beacon < 31){
//			state = CLOSE;
//		}else if (distance_to_beacon > 30){
//			state = FAR;
//		}
//	}else{
//		state = LOCATING;
//	}
//	return state;
//}

//
//void RunMode::turnavoid(RunMode::RunState state){
//	if(state==CLOSE){
//
//	}
//	if(state==AVOID){
//
//	}
//}
