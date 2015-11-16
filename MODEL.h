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

#define V_REF 2.95
#define voltage_scale_factor 6.138

#define voltage_scale(a) ((V_REF / 1024) * a * voltage_scale_factor)


#define log(a, b)   Serial.print(a);	\
					Serial.print(b, DEC)


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
	FUEL_TOTAL,
	FUEL_AVERAGE,
	ALTERNATOR_LOAD,
	ENGINE_LOAD,
	CORRECTION_SHORT,
	CORRECTION_LONG,
	IDLING_VALVE,
	CURRENT,
	VOLTAGE_OBD,
	VOLTAGE_ADC,
	TANK,
	OBD_PERIOD


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

class ObdTagBin
{
public:
	bool value;
	unsigned char type;
	ObdTagBin(unsigned char arg);
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
	void calculateAnalogTags(void);
	void calculateOtherTags(void);
	void routine(void);

	bool connection_established;
	bool need_update;

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
	ObdTagFloat* fuel_average;
	ObdTagFloat* alternator_load;
	ObdTagInteger* engine_load;
	ObdTagInteger* correction_short;
	ObdTagInteger* correction_long;
	ObdTagInteger* idling_valve;
	ObdTagFloat* current;
	ObdTagFloat* voltage_obd;
	ObdTagFloat* voltage_adc;
	ObdTagFloat* tank;
	ObdTagInteger* obd_period;


private:

	unsigned char memory_offset;

	OBD* ecu;
	Timer* analog_reading_timer;
	Timer* obd_waiting_timer;
	Timer* obd_period_timer;


};








#endif

