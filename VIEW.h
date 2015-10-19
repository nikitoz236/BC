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
	{ "RPM" },
	{ "SPEED          " },
	{ "ENGINE t       " },
	{ "INTAKE t       " },
	{ "INTAKE PRESSURE" },
	{ "ATM PRESSURE   " },
	{ "THROTTLE       " },
	{ "OXY            " },
	{ "INJECTION      " },
	{ "FUEL CONS.     " },
	{ "FUEL CONS.     " },
	{ "FUEL TOTAL     " }
};

const unsigned char UnitsResolution[] PROGMEM = {
	0, 1, 2, 2, 3, 3, 4, 5, 6, 7, 8, 9
};


const char obdParametersUnits[][8] PROGMEM = {
	{ 0 },			// 0
	{ "KM/H" },		// 1
	{ "*C" },		// 2
	{ "KPA" },		// 3
	{ "%" },		// 4
	{ "OXY" },		// 5
	{ "MS" },		// 6
	{ "L/100" },	// 7
	{ "L/H " },		// 8
	{ "L" },		// 9
};






class DisplayObject
{
public:
	uint16_t position_x_text;
	uint16_t position_x_value;
	uint16_t position_y;
	void draw(void);
	void update(void);
	unsigned char getUnitsType(obdValsEnum type);
};

class DisplayObjectInteger : public DisplayObject
{
public:
	ObdTagInteger* tag_;
	void draw(void);
	void update(void);

	DisplayObjectInteger(ObdTagInteger * tag);
};

class DisplayObjectFloat : public DisplayObject
{
public:
	ObdTagFloat* tag_;
	void draw(void);
	void update(void);

	DisplayObjectFloat(ObdTagFloat * tag);
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

