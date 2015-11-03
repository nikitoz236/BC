// VIEW.h

#ifndef _VIEW_h
#define _VIEW_h

#include "MODEL.h"
#include <MultiLCD.h>

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

extern LCD_ILI9341 lcd;
extern MODEL* comp;



const char obdParametersNames[][16] PROGMEM = {
//     1234567890ABCDEF
	{ "RPM" },						//0
	{ "SPEED" },					//1
	{ "ENGINE t" },					//2
	{ "INTAKE t" },					//3
	{ "INTAKE PRESSURE" },			//4
	{ "ATM PRESSURE" },				//5
	{ "THROTTLE" },					//6
	{ "OXY" },						//7
	{ "INJECTION" },				//8
	{ "FUEL CONS." },				//9
	{ "FUEL CONS." },				//10
	{ "FUEL TOTAL" }				//11
};

const unsigned char UnitsResolution[] PROGMEM = {
//	0  1  2  3  4  5  6  7  8  9 10 11
	0, 1, 2, 2, 3, 3, 4, 5, 6, 7, 8, 9
};


const char obdParametersUnits[][8] PROGMEM = {
	{ " " },		// 0
	{ "KM/H" },		// 1
	{ "*C" },		// 2
	{ "KPA" },		// 3
	{ "%" },		// 4
	{ "OXY" },		// 5
	{ "MS" },		// 6
	{ "L/H " },		// 7
	{ "L/100" },	// 8
	{ "L" },		// 9
};




class DisplayObject
{
public:
	uint16_t position_x_text;
	uint16_t position_x_value;
	uint16_t position_y;
	virtual void draw(void);
	virtual void update(void);
	unsigned char getUnitsType(unsigned char type);
};

class DisplayObjectInteger : public DisplayObject
{
public:
	ObdTagInteger* tag;
	void draw(void);
	void update(void);

	DisplayObjectInteger(ObdTagInteger* tag_);
};

class DisplayObjectFloat : public DisplayObject
{
public:
	ObdTagFloat* tag;
	void draw(void);
	void update(void);

	DisplayObjectFloat(ObdTagFloat* tag_);
};



class Layout
{
public:
	DisplayObject* layout_array[7];
	Layout(void);
	void draw(void);
	void update(void);
	Layout(uint16_t position);
};




#endif

