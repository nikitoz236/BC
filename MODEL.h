// MODEL.h

#ifndef _MODEL_h
#define _MODEL_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "OBD.h"



#define INJECTOR_PERFOMANCE		235


#define OBD_WAITING_TIMEOUT				2000
#define ANALOG_READING_TIMEOUT			500



#define analog_voltage_pin A6
#define analog_tank_pin A7



#define log(a, b)   Serial.print(a);	\
					Serial.print(b, DEC)


enum obdValsEnum
{
	RPM = 0,
	SPEED = 1,
	ENGINE_T = 2,
	INTAKE_T = 3,
	INTAKE_PRESSURE = 4,
	ATM_PRESSURE = 5,
	THROTTLE = 6,
	OXY = 7,
	INJECTION = 8,
	FUEL_CONS_H = 9,
	FUEL_CONS_KM = 10,
	FUEL_TOTAL = 11
};



//class ObdTag
//{
//public:
//	obdValsEnum type;
//};

class ObdTagInteger
{
public:
	uint16_t value;
	unsigned char type;
	ObdTagInteger(unsigned char arg);
};

class ObdTagFloat
{
public:
	double value;
	unsigned char type;
	unsigned char digits;
	ObdTagFloat(unsigned char arg, unsigned char dig);
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
	double temp_formula(double x);
	void calculateTags(unsigned char page, unsigned char buffer[]);
	void calculateOtherTags(void);
	void routine(void);

	bool connection_established;
	bool frame_recieved;

	ObdTagInteger* rpm;
	ObdTagInteger* speed;
	ObdTagInteger* throttle;
	ObdTagFloat* temp_engine;
	ObdTagFloat* temp_intake;
	ObdTagFloat* pressure_atm;
	ObdTagFloat* pressure_intake;
	ObdTagFloat* injection;
	ObdTagFloat* fuel_consumption_h;
	ObdTagFloat* fuel_consumption_km;
	ObdTagFloat* fuel_consumption;
	ObdTagFloat* fuel_total;

private:

	unsigned char memory_offset;

	OBD* ecu;
	Timer* analog_reading_timer;
	Timer* obd_waiting_timer;
	Timer* obd_period_timer;


};








#endif

