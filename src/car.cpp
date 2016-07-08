/*
 * car.cpp
 *
 *  Created on: 2016?~1??16??
 *      Author: kitty kwok
 */
#include "car.h"
#include "config.h"
#define data_size 600
#define GET_BIT(x, n) (((x) >> (n)) & 1)
#define POWREGULATOR(lower, mpower, upper)(((mpower < lower) ? lower : ((mpower > upper))? upper : mpower))
#define SIZE_THRESHOLD 20
#define PI 3.14159

//#define bound(lower,mpower,upper) (((mpower < lower) ? lower : ((mpower > upper))? upper : mpower))

using namespace std;

Car::Car(): STATE1M(1100), STATE2M(1300), STATE3M(1500), STATE4M(1500), STATE5M(1000), DEFAULTM(1500){

	Led1 = new Led(GetLed1Config());
	Led2 = new Led(GetLed2Config());
	Led3 = new Led(GetLed3Config());
	Led4 = new Led(GetLed4Config());
	encoder = new AbEncoder(GetAbEncoderConfig());
	servo = new TrsD05(GetServoConfig());
	motor = new DirMotor(GetDirmotorConfig());
	motor->SetPower(0);
	ultrasonic = new Us100(GetUs100Config());

	button1 = new Button(GetButton1Config());
	button2 = new Button(GetButton2Config());
//	joystick = new Joystick(GetJoystickConfig());
	LCD = new St7735r(GetLcdConfig());
	buzzer = new SimpleBuzzer(GetBuzzerConfig());
	cam=new k60::Ov7725(GetCamConfig());
	cam->Start();

	LcdTypewriter::Config LcdWconfig;
	LcdWconfig.lcd = LCD;
	LCDwriter = new LcdTypewriter(LcdWconfig);

	memset(data, 0, 8 * 600);
	memset(image, false, true * 80 * 60);

}

Car::~Car(){

	delete Led1 ;
	delete Led2 ;
	delete Led3 ;
	delete Led4 ;
	delete encoder ;
	delete servo ;
	motor->SetPower(0);
	delete motor ;
	ultrasonic->Stop();
	delete ultrasonic;
	delete button1 ;
	delete button2 ;
//	delete joystick ;
	delete LCD;
	delete buzzer ;
	delete LCDwriter;
	delete LCDconsole;
	cam->Stop();
	delete cam;

}

void Car::printvalue(int16_t value, int16_t color){
	LCD->SetRegion(libsc::Lcd::Rect(0,0,128,40));
	std::string Result;
	std::ostringstream convert;
	convert << value;
	Result = convert.str();
	const char *s = Result.c_str();
	LCDwriter->SetTextColor(color);
	LCDwriter->WriteString(s);
}

void Car::printvalue(int x,int y,int w,int h,int16_t value, int16_t color){
	LCD->SetRegion(libsc::Lcd::Rect(x,y,w,h));
	std::string Result;
	std::ostringstream convert;
	convert << value;
	Result = convert.str();
	const char *s = Result.c_str();
	LCDwriter->SetTextColor(color);
	LCDwriter->WriteString(s);
}

void Car::printvalue(std::string Result){
	LCD->SetRegion(libsc::Lcd::Rect(0,0,128,40));
	const char *s = Result.c_str();
	LCDwriter->WriteString(s);
}

void Car::printCar(std::string Result, uint8_t line){
	LCD->SetRegion(libsc::Lcd::Rect(0,line,128,40));
	const char *s = Result.c_str();
	LCDwriter->WriteString(s);
	//68
	//84
	//100
	//116
	//132
	//148
}

void Car::printvalue(int x,int y,int w,int h,std::string Result){
	LCD->SetRegion(libsc::Lcd::Rect(x,y,w,h));
	const char *s = Result.c_str();
	LCDwriter->WriteString(s);
}

void Car::get2dimage(const int8_t x, const int8_t y, const bool (array)[80][60]){
	for(int8_t col=0; col<60; col++){
		for(int8_t row=0; row<80; row++){
			LCD->SetRegion(libsc::Lcd::Rect(row+x, col+y, 1,1));
			LCD->FillColor((array)[row][col] * 0xFFFF);
		}
	}
}

void Car::startcam(){
	cam->Start();
}

void Car::stopcam(){
	cam->Stop();
}

void Car::printRawCamGraph(uint x, uint y){//directly print Car's private 'data[600]' at(x,y)
	//80*60
	LCD->SetRegion(Lcd::Rect(x,y,80,60));
	LCD->FillBits(0,0xFFFF,data,80*60);
}

void Car::printline(int16_t value ,uint16_t color){
	LCD->SetRegion(libsc::Lcd::Rect(0,value * 160 / 255,128,1));
	LCD->FillColor(color);
}

bool Car::GetPixel(const Byte* src,const int8_t x, const int8_t y){
	const int offset  = x/8 + (y*80/8);
	return ((src[offset] << (x%8) & 0x80)) ? false : true;
	//if src[offset] == 1 -> return false
	//if src[offset] == 0 -> return true
	//if not get pixel --> white++

}


//**need improve
void Car::avgimage(const Byte* src, bool bitmap[78][58]){
	for (uint8_t col = 1; col < 59; col++){
		for (uint8_t row = 1; row < 79; row++){
			uint8_t white = 0;

			for (uint8_t count=0; count<3; count++){
				if(!GetPixel(src, row-1+count, col-1)){
					//if not (getpixel) white++
					white++;
				}
			}
			for(uint8_t count =0; count<3; count++){
				if(!GetPixel(src, row-1+count, col)){
					white++;
				}
			}
			for(uint8_t count=0; count<3; count++){
				if(!GetPixel(src, row-1+count, col+1)){
					white++;
				}
			}
			if(white > 4){
				Car::bitmap[col-1][row-1] = true;
			}else{
				Car::bitmap[col-1][row-1] = false;
			}
		}
	}

}

void Car::printavgimage(const Byte* src, bool processmap[58][78]){
	for (uint8_t col = 1; col < 59; col++){
			for (uint8_t row = 1; row < 79; row++){
				uint8_t white = 0;

				for (uint8_t count=0; count<3; count++){
					if(!GetPixel(src, row-1+count, col-1)){
						white++;
					}
				}
				for(uint8_t count =0; count<3; count++){
					if(!GetPixel(src, row-1+count, col)){
						white++;
					}
				}
				for(uint8_t count=0; count<3; count++){
					if(!GetPixel(src, row-1+count, col+1)){
						white++;
					}
				}
				if(white > 4){
					processmap[col-1][row-1] = false;
				}else{
					processmap[col-1][row-1] = true;
				}
			}
		}
	for(int8_t col=0; col<58; col++){
		for(int8_t row=0; row<78; row++){
			LCD->SetRegion(libsc::Lcd::Rect(row, col, 1,1));
			//lcd::Rect(row, col, 1, 1)
			LCD->FillColor(processmap[col][row] * 0xFFFF);
		}
	}

}

/*
//**need alter algo
void Car::longestwhiteline(bool whitemap[78][58]){
	int8_t row=18;
	int8_t left = 3; int8_t right= 5;
	int8_t whitelen=right-left;
	for(int8_t col = 3; col < 75 ; col++){
		if(!whitemap[col][row]){
			continue;
		}
		for(int8_t temp=col+1 ; temp<75 ; temp++){
			if(!whitemap[temp][row] && (temp-col)>whitelen){
				//if(temp-col > whitelen){
				left = col;
				right = temp;
				whitelen = temp-col;
				//}
				break;
			}
			else if(whitemap[col][row] && col == 74 && (temp - col > right - left )){
				right = 74;
				left = col;
				break;
			}
		}
	}
	sprintf(msg, "whitelen: %d", whitelen);
	printvalue(0,80,128,40, msg);
	lmargin = left;
	rmargin = right;
	sprintf(msg, "left: %d", lmargin);
	printvalue(0,96,128,40, msg);
	sprintf(msg, "right: %d", rmargin);
	printvalue(0,110,128,40, msg);
	mid = 0.5*(lmargin+rmargin);
	sprintf(msg, "mid: %d", mid);
	printvalue(0,126,128,40, msg);
}
*/

//MAJOR IMAGE PROCESSING
int8_t Car::CheckLightIndex(const Byte* src){
	rowIndex=0;
	uint32_t i, j, k, m;
	uint32_t pixelCounter = 0, s=0, prerow=0, row=0;

	for(j=0; j<600; j+=10){
		m=j/10;
		LightIndex[m] = 0;
		for(i=0; i<10; i++){
			s=data[i+j];
			for(k=0; k<8; k++){
				if(!((s>>(7-k))&1u)){
					LightIndex[m]++;
				}
			}
		}
		if(LightIndex[rowIndex] < LightIndex[m]){
			rowIndex=m;
		}
	}

//	for (uint32_t y=0; y<60; y++){
//		LightIndex[y]=0;
//			for (int8_t x = 0; x<80; x++){
//
//				//int16_t temp = src[10*(x)+(y/8)];//show the byte
//
//				if(GetPixel(src, x, y)){
//					LightIndex[y]+=1;
//				}
//			}
//			if(LightIndex[rowIndex] < LightIndex[y]){
//				rowIndex=y;
//			}
//	}




	if (rowIndex==0 && LightIndex[rowIndex]<2){
		//no white found
		distance_to_beacon=99;
		return 99;
	}

	distance_to_beacon = 60-rowIndex;

	int8_t tempDist =5;
	LightX = 0;
	LightW = 0;

//
//	 uint16_t count=77;
//	 uint16_t x=0, c=0, anoj=0;
//	 for (uint8_t i=0; i<10; i++){
//		 s=data[rowIndex*10+i];
//		 for(uint8_t j=0; j<8; j++){
//			 if(!(s>>(7-j))&1u){
//				 x = rowIndex*10+i*8+j;
//				 l_margin = x;
//				 x += 1;
//				while((!(data[rowIndex*10+c]>>(7-j))&1u) && x<78){
//					x+=1;
//					c+=1;
//					j+=1;
//
//					if(c==10) c=0;
//					if(anoj==8)	anoj=0;
//
//				}
//				r_margin = x;
//
//				if(r_margin < l_margin){
//					return 100;
//				}
//				if((r_margin - l_margin) < tempDist){
//					x = r_margin;
//				}else{
//					tempDist = r_margin - l_margin;
//					LightX = l_margin;
//					LightW = r_margin;
//				}
//			 }
//
//		 }
//	 }


	for(int8_t x=2; x<78; x++){

		 if(GetPixel(src, x, rowIndex)){
		 	l_margin = x;
			x+=1;
		 	while(GetPixel(src, x, rowIndex) && x<78){
		 		x+=1;

		 	}
		 	r_margin = x;

			if(r_margin < l_margin){
				//impossible
				//code 100
				return 100;
			}
			if((r_margin - l_margin) < tempDist){
				x = r_margin;

			}else{
				tempDist = r_margin - l_margin;
				LightX = l_margin;
				LightW = r_margin;
			}

		 }

	}


	if ( LightX ==0 && LightW ==0){
		//no W code 101
		distance_to_beacon=101;
		return 101;
	}
	mid = (LightX + LightW)/2;
	int8_t beacon_mid = mid;
	return distance_to_beacon;
}

void Car::imageCorrection(const Byte* src){
	for(uint16_t index = 0; index<600; index++){
		if(src[index]>230){
			data[index] = 255;
		}
	}
}

/*
uint16_t Car::u_distance(){
	uint16_t dist=1;
	ultrasonic->Start();
	//temp+=1;
	while(!ultrasonic->IsAvailable() && temp<1000){
		temp+=1;
	}
	dist = (uint16_t) ultrasonic->GetDistance();
	//dist = (uint16_t) ultrasonic->GetDistance();
	sprintf(msg,"Dist: %d %d", dist);
	printCar(msg,100);
	temp=0;
	ultrasonic->Stop();
	//dist = ultrasonic->GetDistance();

	return dist;
}
*/

int8_t Car::get_RState(int8_t Distance_to_beacon){
	//distance_to_beacon = (60-rowIndex);
	//RunState state;
	int8_t state=0;
	//STATE = LOCATING;
	if (Distance_to_beacon<81){
		if(Distance_to_beacon < 40){
			//state = AVOID;
			state=1;
		}else if(Distance_to_beacon < 50){
			//state = CLOSE;
			state=2;
		}else if (Distance_to_beacon > 51){
			//state = FAR;
			state=3;
		}
	}else{
		//STATE = LOCATING;
		state=4;
	}

//	if(prevState==1){
//		state=5;
//	}
//	if(u_distance()<100){
//			state=5;
//	}
	prevState= state;
	return state;
}
/*
 * Encoder motor formula:
 * Encoder_count = 17.308 * motor - 345.504
 *
 */

void Car::exitState(int8_t state){

}

void Car::RUN_2ndCAMSTATE(int8_t state){
	switch (state){

	case 1:
	{


		break;
	}
	case 2:
	{

		break;
	}

	case 3:
	{

		break;
	}

	case 4:
	{


		break;
	}

	case 5:
	{

		break;
	}

	default:
	{

	}




	}
}

void Car::RUN_STATE(int8_t state){
	switch (state){

	case 1:
	{
		//if right-> turn more right, if left->turn more left
//		if(mid<40){
//			//keep beacon on right hand side
//			//turningPID(mid, 10);
			turningPID(r_margin+3, r_margin+10);
//
//		}
//		if(mid>39){
//			//turningPID(mid,70);
//			turningPID(mid, l_margin-23);
//		}
		motorPID(STATE1M+100);
		//motor_control(150,1);
		break;
	}
	case 2:
		{
			//turn left / right around the beacon
//			if(mid<40){
//				//keep beacon on left hand side
//				turningPID(mid, 14);
//			}
//			if(mid>39){
//				turningPID(mid,66);
//			}
			turningPID(r_margin+3, r_margin+5);
			motorPID(STATE2M+100);
			//motor_control(130,1);
			break;
		}
	case 3:
	{
		//normal PID
		//turningPID(mid,40);
		turningPID(r_margin+1, r_margin+5);
		motorPID(STATE3M+100);
		//motor_control(165,1);
		break;
	}
	case 4:
	{
		//turn left / right slowly
//		if(temp==1){
//			temp=320;
//		}
//		if(temp>160){
			turningPID(l_margin+10, r_margin+5);
			//servo_control(670);
//			temp--;
//		}else if(temp>1){
//			turningPID(670, l_margin-5);
//			temp--;
//		}else if(temp==1){
//			temp=0;
//		}

		motorPID(STATE4M+100);
//		servo_control(950);
//		motor_control(100,1);

		break;
	}
	case 5:
	{
		servo_control(670);
		motorPID(DEFAULTM);
		break;
	}
	default:
	{
		servo_control(670);
		motorPID(DEFAULTM);
		//motor_control(130,1);
	}
	}
	prevState=state;
}

//modify the black value=0; white value = 1;
uint16_t Car::findLightbyByte(const Byte* src, int8_t fac){
	lightCond = DEFAULT;
	uint16_t byte_x, byte_y;
	uint8_t count_pix;
	uint8_t count=0;
	uint8_t countH=0;
	uint8_t times_count=0;
	uint16_t size = 111;
	bool consecutive_count = false;

	for (uint16_t i=2; i<598; i++){
		count_pix=0;
		count=0;

		if(src[i]< 240 && src[i]!=0){
			uint16_t coorx, coory;

			for(uint8_t offset=0; offset<8; offset++){
				//STORE EACH BIT BY BIT SHIFT
				BYTEMAP[offset]= ((src[i] >> (7-offset))) % 2;
				if (!BYTEMAP[offset]){
					count++;
				}
			}

			if (count<3){
				continue;
			}


			for(uint8_t offset =0; offset<8; offset++){

				if (!BYTEMAP[offset]){
					uint16_t tempx = (i*8) / 80;
					uint16_t tempy = (i*8) % 80;
//					byte_y = (i * 8) / 80;
//					byte_x = (i * 8) % 80;

//					for(uint8_t c=0; c<3; c++){
//						if(!GetPixel(src, tempx+count, tempy+1)){
//							count_pix++;
//						}
//					}
//					for(uint8_t c=0; c<3; c++){
//						if(!GetPixel(src, tempx-1+count, tempy+1)){
//							count_pix++;
//						}
//					}
					while(!GetPixel(src, tempx+1, tempy+times_count)){
						times_count++;
						countH++;
					}
					times_count=0;
					while(!GetPixel(src, tempx+times_count, tempy+countH/2)){
						times_count++;
						count_pix++;
					}

//
//					//x,y+1
//					if(!(((src[i+80] >> (7-offset))) % 2)){
//						count_pix++;
//					}
//					//x+1, y+1
//					if(offset!=7){
//						if(!((src[i+80] >> (7-offset+1)) % 2)){
//							count_pix++;
//						}
//					}else{
//						if(!((src[i+81] >> 7) % 2)){
//							count_pix++;
//						}
//					}
//
//					//x-1, y+1
//					if(offset!=0){
//						if(!((src[i+80] >> (7-offset-1)) % 2)){
//							count_pix++;
//						}
//					}else{
//						if(!(src[i+79] % 2)){
//							count_pix++;
//						}
//					}
					if(count_pix>2){
						//WHITE FOUND
						//byte_x= (i * 8) / 80;
						//byte_y= (i * 8) % 80;
						sprintf(msg,"SOME XY @%d,%d", tempx, tempy);
						printCar(msg, 116);
						size = count_pix * countH;
						return tempx;
					}
//					if(!consecutive_count){
//						if(BYTEMAP[offset]==0 && BYTEMAP[offset-1]==0){
//							consecutive_count = true;
//							count_pix++;
//						}else{
//							consecutive_count = false;
//						}
//					}else{ //IF CONSECUTIVE COUNT
//						count_pix++;
//					}
				}
			}


		}else if(src[i]==0){
			//READ WHITE BY BYTE
			//WHOLE WHITE CONDITION
			uint16_t tempi = i+80;
			LightH = 0;
			LightW = 0;
			lightCond = WHITE;
			//PROCESS TO LOCATE X & Y
			byte_y = (i * 8) / 80;
			byte_x = (i * 8) % 80;

			while(src[i+80] && i<519){
				i=i+80;
				LightH++;

			}
			//tempi = (tempi + i - 80)/2;
			while(src[tempi+1] && tempi<595){
				tempi++;
				LightW++;
			}
			if(LightH <2 || LightW<2){
				continue;
			}
			size = LightH * LightW;

			//BEACON_SIZE = DiverseSearch(src, byte_x, byte_y);
			LightX = byte_x;
			LightY = byte_y;
			sprintf(msg,"WW LightH %d", LightH);
			printCar(msg, 132);
			//printvalue(0,132,128,40, msg);
			return LightX;
		}else{
			printCar("No W found", 132);
		}
		//
	}

	return 900;
	//code for no light found: 111
}
/*
 * 68
 * 84
 * 100
 * 116
 * 132
 */

uint8_t Car::DiverseSearch(const Byte* src, uint8_t dir_x, uint8_t dir_y){
	//Search for white in 3 directions: x-1,y-1 ; x,y-1 ; x+1,y-1
	uint8_t count=0;
	uint8_t count_pix=0;
	while (!GetPixel(src, dir_x-1+count, dir_y-1)){
		if(!GetPixel(src, dir_x-1+count, dir_y-1)){
			count_pix++;
		}
		count++;
	}
	count=0;
	while (!GetPixel(src, dir_x, dir_y-1-count)){
		if(!GetPixel(src, dir_x, dir_y-1-count)){
			count_pix++;
		}
		count++;
	}
	count=0;
	while (!GetPixel(src, dir_x-1+count, dir_y-1)){
		if(!GetPixel(src, dir_x-1+count, dir_y-1)){
			count_pix++;
		}
		count++;
	}
	sprintf(msg,"White Count: %d", count);
	printvalue(0,116,128,40, msg);

	return count_pix;
}


void Car::avoid(int8_t mid_line){
	if(LightIndex[rowIndex]>40){
		//BEACON IS INFRONT OF CAR
		if(mid_line<40){
			//turn very left in circle around beacon

		}else{
			//turn very right in circle around beacon

		}
	}
}

int16_t Car::turn(int8_t value){
	int16_t degree=800; //max: 800+250, min: 800-250
	if(value > 40){
		degree = 800 + (250 * (value/40));
	}
	if(value < 41){
		//degree = 800 - (250 * ((40-value)/40));
		degree = 650;
	}
	sprintf(msg, "Degree @%d", degree);
	printCar(msg, 132);
	motor_control(100,1);
	return degree;
}

//**need alter algo
/*
uint16_t Car::getmidline(bool midmap[78][58]){

	int8_t countcol=0;
	int8_t countrow=0;
	int8_t row=29;
	//for(int8_t col=1; col<78;){
		countrow=0;
		//for()
		printvalue("getmidline in progress");
		for(int8_t col=3; col<75; col++){
//			if (countrow>2){
//				break;
//			}
			//if the neighbour 2 pixels are black, ignore, else mark in lmargin
			if(!midmap[col][row]){
				if(midmap[col-2][row]==midmap[col][row] && midmap[col-1][row]==midmap[col][row] && midmap[col][row]==midmap[col+1][row] && midmap[col][row]==midmap[col+2][row]){
					continue;
				}
				else if(midmap[col][row]!=midmap[col-3][row] || midmap[col][row]!= midmap[col+3][row]){
					lmargin = row;
					sprintf(msg, "left: %d", lmargin);
					printvalue(20,20,128,40, msg);
					//printvalue(113,0,128,40,"hahaleft");
				}else{
				}
				countrow+=1;
			}
			countrow=0;
			//vice versa in rmargin
			if(midmap[col][row]){
				if(midmap[col-2][row]==midmap[col][row] && midmap[col-1][row]==midmap[col][row] && midmap[col][row]==midmap[col+1][row] && midmap[col][row]==midmap[col+2][row]){
									continue;
				}
				else if(midmap[col][row]!=midmap[col-3][row] || midmap[col][row]!= midmap[col+3][row]){
					rmargin = row;
					sprintf(msg, "right: %d", rmargin);
					printvalue(20,50,128,40,msg);
					//printvalue(113,50,128,40,"haharight");

				}
				else{
				}
				countrow+=1;
			}
		}
		//col+=(78/3);
	//}

	mid=((rmargin + lmargin)/2)/58*128;
	printvalue("getmidline finish");

	return mid;
}
*/

/*
 * Concept:
1)		//collect row data: white pixels in row
		for(int pixels=RS; pixels<RE; pixels++){			<== if raw data[x][y] ==0 --> white
			if(!bitmap[row][pixels])
				data[row][WHITECOUNT]++;
		}

2) if midpoint = black, means off track and need to take prev midpoint

3)
Arrays for data:
 a)	rawdata[][]		====> 1||0
 b)	process_data[row][]  ==> store row info
 c) trackdiff[5] = 0;
 eg.
		//data info
		#define WHITECOUNT 0
		#define ISWHITE 1
		#define ISBLACK 2
		#define WHITEAT 3

		after handle image data

		//track info
		#define STRAIGHT	11		(inc small S turns)
		#define TURN		22
		#define SMALLTURN	33
		#define BIGTURN		44
		#define +track		55


4)
Take 5 rows for data, scan horizontally to de-noise first
		de-noise: if the neighbour 3 pixels are same color, not noise
		otherwise, correct that pixel to b same as neighbour pixel

after de-noise, take the margin by scanning horizontally
		if line1 have
		if line2

5) analyze track
by the difference from absolute midpt (64)
output and store the track array eg. [-5,-1, 3, 5, 7]  ==> right turn

6) calculate the angle difference by tanh(abs(trackdiff[i]), line distance)
	output the servo degree and pass to turnservo()

*/

void Car::clearLcd (uint16_t color){
	LCD->Clear(color);
}

void Car::blinkLED(int8_t id, int delay_time, int persist_time){
	libsc::Led* LedToBlink;
	switch(id){
	case 1:
		LedToBlink = Led1;
		break;
	case 2:
		LedToBlink = Led2;
		break;
	case 3:
		LedToBlink = Led3;
		break;
	case 4:
		LedToBlink = Led4;
		break;
	}
	LedToBlink->SetEnable(true);
	while ( (persist_time-delay_time) >= 0){
		LedToBlink->Switch();
		libsc::System::DelayMs(delay_time);
		persist_time -= delay_time;
	}
	LedToBlink->SetEnable(false);

}

void Car::beepbuzzer(bool tobeep){
	buzzer->SetBeep(tobeep);
}

void Car::switchLED(int8_t id){
	libsc::Led* LedToBlink;
	switch(id){
	case 1:
		LedToBlink = Led1;
		break;
	case 2:
		LedToBlink = Led2;
		break;
	case 3:
		LedToBlink = Led3;
		break;
	case 4:
		LedToBlink = Led4;
		break;
	}
	LedToBlink->Switch();
}

bool Car::getbutton(int8_t id){
	if(id ==1) return button1->IsDown();
	else if (id == 2) return button2->IsDown();
	else return 0;
}

//libsc::Joystick::State Car::getjoystick(){
//	return joystick->GetState();
//}


void Car::get_raw_image(){//get image in 1D-Byte, then put into Car's private 'data[600]'
	if (cam->IsAvailable()){
		std::memcpy(data, cam->LockBuffer(), data_size);
		cam->UnlockBuffer();
	}
}


void Car::rawimgprocess(){

	memcpy(data, cam->LockBuffer(), data_size);
	cam->UnlockBuffer();

	for (int8_t col=0; col<60; col++){
		for (int8_t row = 0; row<80; row++){
			image[col][row] = GetPixel(data, row, col);
		}
	}

}
