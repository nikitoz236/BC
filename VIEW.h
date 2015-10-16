// VIEW.h

#ifndef _VIEW_h
#define _VIEW_h

#include "MODEL.h"
#include "MODEL.h"

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class DisplayObject
{
public:
	uint16_t position_x;
	uint16_t position_y;
	void draw(void);
	void update(void);
	unsigned char getUnitsType(obdValsEnum type);
};

class IntValueMedium : DisplayObject
{

};

class FloatValueMedium : DisplayObject
{
public:
	ObdTagFloat* tag;
	void draw(void);
	void update(void);

};




const unsigned char obdParametersNames[][] PROGMEM = {
	{ "RPM" },
	{ "SPEED" },
	{ "ENGINE t" },
	{ "INTAKE t" },
	{ "INTAKE PRESSURE" },
	{ "ATM PRESSURE" },
	{ "THROTTLE" },
	{ "OXY" },
	{ "INJECTION" },
	{ "FUEL CONS." },
	{ "FUEL CONS." },
	{ "FUEL TOTAL" }
};

const unsigned char UnitsResolution[] PROGMEM = {
	0, 1, 2, 2, 3, 3, 4, 5, 6, 7, 8, 9
};


const unsigned char obdParametersUnits[][] PROGMEM = {
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


#endif

