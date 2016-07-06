/*
 * RunMode.cpp
 *
 *  Created on: 26-03-2016
 *      Author: Kyle
 */
#include "car.h"
#include "RunMode.h"
#define SERVO_MID 820
#define REVERSE_SPEED 180


RunMode::RunMode():
servoErr(0), servoPrevErr(0), ideal_motor_speed(100), maxMotorSpeed(350), minMotorSpeed(80), ideal_servo_degree(SERVO_MID), encoder_count(0), motorErr(0), motorPrevErr(0), maxServoAngle(990), minServoAngle(580),encoderZeroCount(0){
	//can initialize the variable here,
	//servo
	s_lkp = 11.0f;
	s_lkd = 0.0f;
	s_rkp = 1.30f;
	s_rkd = 6.0f;
	servoTurn = 0;
	//motor
	m_Kp = 0.45f;
	m_Ki = 0.05f;
	m_kd = 0;
}

RunMode::~RunMode(){

}

int16_t RunMode::turningPID (int16_t mid_line, int16_t value){
	//mid_line: current beacon mid, value: ideal turning position
	//int16_t degree; //max: 800+265, min: 800-265
	//int16_t temp;

	servoPrevErr = servoErr;
	servoErr = 40 - mid_line;
	//float kp, ki, kd;
	//FORMULA: degree = kp*servoErr + kd*servoPrevErr
	servoTurn = s_lkp*servoErr + s_lkd*(servoErr-servoPrevErr);
	ideal_servo_degree = SERVO_MID - servoTurn;

//	if(value >= 0)	servo_control(ideal_servo_degree);
//	else  servo_control(ideal_servo_degree+50);
//	servo->SetDegree(libutil::Clamp(minServoAngle, ideal_servo_degree, maxServoAngle));

	servo_control(ideal_servo_degree);
	return ideal_servo_degree;
	//return 0;//your implementation
}

int16_t RunMode::motorPID (int16_t ideal_encoder_count){
	encoder_count = get_encoder_count();

//	if(!encoder_count){
//		encoderZeroCount+=1;
//	}else
//		encoderZeroCount=0;

	//CHECK IF MOTOR STOPPED
//	(!encoder_count) ? encoderZeroCount+=1 : encoderZeroCount=0;
//
//	if(encoderZeroCount>1){
//
//		// REVERSE CAR AND EXIT MOTORPID TO AVOID MOTORERR MISS COUNT
//		is_clockwise = false;
//		motor_control(REVERSE_SPEED, is_clockwise);
//		servo_control((ideal_servo_degree > 820) ? (SERVO_MID-150) : (SERVO_MID+150));
//		//(ideal_servo_degree > 820) ? servo_control(SERVO_MID-150) : servo_control(SERVO_MID+150);
//		motor_control(REVERSE_SPEED, is_clockwise);
//		return REVERSE_SPEED;
//
//	}

	//IF MOTOR GONE CRAZY RUN MOTOR BACK AT SPEED 80
	if(abs(encoder_count)>5500){
			return 80;
		}

//	if(abs(encoder_count)>2500){
//		encoder_count = ideal_encoder_count;
//	}
	is_clockwise=true;
	motorPrevErr = motorErr;
	motorErr = (ideal_encoder_count - encoder_count);
	motorSum += motorErr;
	//relationship between motorerr & servoerr?
	//(m_varset.ideal_encoder_count- (m_varset.ideal_encoder_count == 0 ? 0 : m_varset.KDec * abs(ServoErr))) - encodercount);
	//17.308 * motor - 345.504
	actual_encoder_target = ideal_encoder_count +m_Kp *motorErr+m_Ki*motorSum + m_kd*(motorErr-motorPrevErr);
	temp = (actual_encoder_target + 345.504)/17.308;
	ideal_motor_speed = (uint16_t) temp;
//	ideal_motor_speed = ideal_motor_speed + m_Kp * (motorErr - motorPrevErr) + m_Ki * (motorErr);

	motor_control(ideal_motor_speed,is_clockwise);

	return ideal_motor_speed;
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
	//ideal_servo_degree = degree + 900;
	//servo->SetDegree(ideal_servo_degree);
	ideal_servo_degree = degree;
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
