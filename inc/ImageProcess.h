/*
 * ImageProcess.h
 *
 *  Created on: 14 Jun 2016
 *      Author: kittylgnkwok
 */

#ifndef INC_IMAGEPROCESS_H_
#define INC_IMAGEPROCESS_H_

#include <libbase/k60/mcg.h>
#include <libsc/k60/ov7725.h>
#include <libsc/button.h>
#include <libsc/st7735r.h>
#include <libsc/lcd_console.h>
#include <libsc/lcd_typewriter.h>
#include <string>

class ImageProcess{
public:


private:

protected:
	libsc::St7735r* LCD=nullptr;
	libsc::LcdConsole* LCDconsole=nullptr;
	libsc::LcdTypewriter* LCDwriter=nullptr;
	libsc::k60::Ov7725* cam=nullptr;

};

#endif /* INC_IMAGEPROCESS_H_ */
