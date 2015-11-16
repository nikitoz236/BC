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


#include "buttons.h"

extern LCD_ILI9341 lcd;
extern MODEL* comp;



const char obdParametersNames[][16] PROGMEM = {
//     1234567890ABCDEF
	{ "RPM" },						//0
	{ "SPEED" },					//1
	{ "ENGINE t" },					//2
	{ "INTAKE t" },					//3
	{ "INTAKE PRESS." },			//4
	{ "ATM PRESS." },				//5
	{ "THROTTLE" },					//6
	{ "OXY" },						//7
	{ "INJECTION" },				//8
	{ "FUEL CONS." },				//9
	{ "FUEL CONS." },				//10
	{ "FUEL TOTAL" },				//11
	{ "FUEL AVERAGE" },				//12
	{ "ALTERNATOR LOAD" },			//13
	{ "ENGINE LOAD" },				//14
	{ "CORR. SHORT" },				//15
	{ "CORR. LONG" },				//16
	{ "IDLING VALVE" },				//17
	{ "CURRENT" },					//18
	{ "VOLTAGE OBD" },				//19
	{ "VOLTAGE ADC" },				//20
	{ "TANK" },						//21
	{ "OBD PERIOD" }				//22
};

const unsigned char UnitsResolution[] PROGMEM = {
//	0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17  18  19  20 21 22 
	0, 1, 2, 2, 3, 3, 4, 5, 6, 7, 8, 9, 8, 4, 4, 4, 4, 4, 10, 11, 11, 0, 6
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
	{ "A" },		// 10
	{ "V" }			// 11
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

class DisplayObjectBin : public DisplayObject
{
public:
	ObdTagBin* tag;
	void draw(void);
	void update(void);

	DisplayObjectBin(ObdTagBin* tag_);
};




class LayoutPage
{
public:
	DisplayObject* arr[12];
	LayoutPage(uint16_t position_, unsigned char page_number_, unsigned char items_amount_);
	void draw(void);
	void update(void);
	uint16_t position;
	unsigned char page_number;
	unsigned char items_amount;
};



class DisplayPages
{
public:
	LayoutPage* pages[10];
	LayoutPage* current_page;
	DisplayPages(void);
	void routine(void);


};




#endif

