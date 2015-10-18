// MODEL.h

#ifndef _MODEL_h
#define _MODEL_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "OBD.h"

#define temp_formula(a) (double)(155.04149 - (a*3.0414878) + (a*a*0.03952185) - (a*a*a*0.00029383913) + (a*a*a*a*0.0000010792568) - (a*a*a*a*a*0.0000000015618437))


#define INJECTOR_PERFOMANCE		235


#define OBD_WAITING_TIMEOUT				1000
#define ANALOG_READING_TIMEOUT			500



#define analog_voltage_pin A6
#define analog_tank_pin A7

#define TAGS_AMOUNT FUEL_TOTAL + 1

enum obdValsEnum
{
	RPM,
	SPEED,
	ENGINE_T,
	INTAKE_T,
	INTAKE_PRESSURE,
	ATM_PRESSURE,
	THROTTLE,
	OXY,
	INJECTION,
	FUEL_CONS_H,
	FUEL_CONS_KM,
	FUEL_TOTAL
};



class ObdTag
{
public:
	obdValsEnum val_type;	
	bool isFloat;
	uint16_t tag_value;
	unsigned char digits;
};

class ObdTagIntger : ObdTag
{
public:
	uint16_t tag_value;
	ObdTagIntger(obdValsEnum type);
};

class ObdTagFloat : ObdTag
{
public:

	double tag_value;
	ObdTagFloat(obdValsEnum type, unsigned char dig);
};


class Timer
{
public:
	Timer(unsigned long timeout);
	bool isOver(void);
	void setTimeout(unsigned long timeout);
	void reset(void);
	unsigned long getTime(void);

private:
	unsigned long start_;
	unsigned long timeout_;
};





class MODEL
{
public:
	unsigned long obd_update_period;


	MODEL(void);
	void calculateTags(unsigned char page, unsigned char buffer[]);
	void calculateOtherTags(void);
	void routine(void);

	bool connection_established;

	ObdTag* tags_array[TAGS_AMOUNT];

	//ObdTagIntger* rpm;
	//ObdTagIntger* speed;
	//ObdTagIntger* throttle;
	//ObdTagFloat* temp_engine;
	//ObdTagFloat* temp_intake;
	//ObdTagFloat* pressure_atm;
	//ObdTagFloat* pressure_intake;
	//ObdTagFloat* injection;
	//ObdTagFloat consumption_h;
	//ObdTagFloat fuel_total;

private:

	unsigned char memory_offset;

	OBD* ecu;
	Timer* analog_reading_timer;
	Timer* obd_waiting_timer;
	Timer* obd_period_timer;


};








#endif

