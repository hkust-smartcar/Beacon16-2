/*
 * car.h
 *
 *  Created on: 2016?~1??16??
 *      Author: Kitty Kwok
 */

#pragma once

#include <libsc/led.h>
#include <libbase/k60/mcg.h>
#include <libsc/ab_encoder.h>
#include <libsc/trs_d05.h>
#include <libsc/k60/ov7725.h>
#include <libsc/button.h>
#include <libsc/joystick.h>
#include <libsc/st7735r.h>
#include <libsc/simple_buzzer.h>
#include <string>
#include <libsc/alternate_motor.h>
#include <libsc/dir_motor.h>
#include <libsc/lcd_console.h>
#include <libsc/lcd_typewriter.h>
#include <libsc/tower_pro_mg995.h>
#include <libsc/us_100.h>
#include <array>
#include <cstring>
#include <sstream>
#include <functional>
#include <math.h>
#include <vector>


struct pixel{
	float x;
	float y;
};

enum RunState{
		LOCATING=0, FAR, CLOSE, AVOID
	};
		//RunState STATES = LOCATING;

class Car{
public:

	Car();

	~Car();


	void printvalue(int16_t value, int16_t color);		//print number to up-right corner in LCD
	void printvalue(int x,int y,int w,int h,int16_t value, int16_t color);		//print number to specific location, (x,y,w,h,value-to-print) respectively
	void printvalue(std::string);		//print string to up-right corner in LCD
	void printCar(std::string, uint8_t);
	void printvalue(int x,int y,int w,int h,std::string Result);	//print string to to specific location, (x,y,w,h,value-to-print) respectively
	void startcam();
	void stopcam();

	void printRawCamGraph(uint x, uint y);

	void clearLcd (uint16_t);		// its very slow

	void printline(int16_t value,uint16_t color);
	// print a horizontal line, could be for threshold

	void blinkLED(int8_t id, int delay_time, int persist_time);
	//Internal delay inside

	void beepbuzzer(bool tobeep);
	void switchLED(int8_t id);
	bool getbutton(int8_t id);
	//return true when the specific button has been pressed
	libsc::Joystick::State getjoystick();
	//return state, use with ticks

	int16_t otsu_threshold();
	void get_raw_image(void);
	void get2dimage(const int8_t, const int8_t, const bool[80][60]);
	bool GetPixel(const Byte*, const int8_t, const int8_t);
	void rawimgprocess(void);
	void bytetobit(Byte*, bool);
	uint16_t getmidline(bool[78][58]);

	void avgimage(const Byte*, bool[78][58]);
	void printavgimage(const Byte*, bool[58][78]);
	void longestwhiteline(bool[78][58]);

	//BEACON specific
	int8_t get_RState();
	void imageCorrection(const Byte*, bool[78][58]);
	void findLight(bool [78][58], bool [78][58], uint8_t);
	uint16_t findLightbyByte(const Byte*, int8_t);
	uint8_t DiverseSearch(const Byte*, uint8_t, uint8_t);
	void avoid(int8_t);
	int16_t turn(int8_t);
	int8_t CheckLightIndex(const Byte*);
	void imageCorrection(const Byte*);
	void RUN_STATE(int8_t);
	uint16_t u_distance();

	virtual int16_t turningPID (int16_t, int16_t){return 0; };
	//PID = kp *error +kd *(error_prev - error) + ki * sum of error
	virtual int16_t motorPID (int16_t){return 0; };
	//PID = kp *error +kd *(error_prev - error) + ki * sum of error
	virtual void motor_control(uint16_t power, bool is_clockwise_rotating){ };	//0~1000
	//apply motor PID to improve the acceleration
	virtual void servo_control(int16_t degree){ }; //-90 to 90
	//apply turning PID to improve the turning delay & to have a smooth turning
	virtual int32_t get_encoder_count(){return 0; };


	enum LightCond{
		DEFAULT =0, BLACK, WHITE, POSSIBLEW, COMBINE
	};
	LightCond lightCond = DEFAULT;

	Byte data[600];
	char msg[22];
	bool image[80][60];
	// bool map[78][58];
	bool bitmap[78][58];
	bool processmap[58][78];
	bool lightFound = false;
	bool BYTEMAP[8];
	//bool SECONDBYTEMAP[8];
	//bool THIRDBYTEMAP[8];

	int8_t l_margin = 0;
	int8_t r_margin = 0;
//	int8_t margin[4][3];
	int8_t lightCondition = 111;
	int8_t BEACON_SIZE = 0;
	int8_t LightX;
	int8_t LightY;
	int8_t LightH;
	int8_t LightW;
	int8_t rowIndex;
	int8_t LightIndex[60];
	uint16_t distance_to_beacon;
	int8_t temp=0;

	int8_t mid = 0;

	//std::vector<pixel> pixvector;


private:



protected:

	libsc::Led* Led1=nullptr;
	libsc::Led* Led2=nullptr;
	libsc::Led* Led3=nullptr;
	libsc::Led* Led4=nullptr;
	libsc::AbEncoder* encoder=nullptr;
	libsc::TrsD05* servo=nullptr;
	libsc::DirMotor* motor=nullptr;
	libsc::Us100* ultrasonic=nullptr;
	libsc::Button* button1=nullptr;
	libsc::Button* button2=nullptr;
	libsc::Joystick* joystick=nullptr;
	libsc::St7735r* LCD=nullptr;
	libsc::LcdConsole* LCDconsole=nullptr;
	libsc::LcdTypewriter* LCDwriter=nullptr;
	libsc::SimpleBuzzer* buzzer=nullptr;
	libsc::k60::Ov7725* cam=nullptr;

};
