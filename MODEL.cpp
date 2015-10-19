// 
// 
// 

#include "MODEL.h"


MODEL::MODEL(void)
{
	ObdTagInteger* rpm = new ObdTagInteger(RPM);
	ObdTagInteger* speed = new ObdTagInteger(SPEED);
	ObdTagInteger* throttle = new ObdTagInteger(THROTTLE);
	ObdTagFloat* temp_engine = new ObdTagFloat(ENGINE_T, 2);
	ObdTagFloat* temp_intake = new ObdTagFloat(INTAKE_T, 2);
	ObdTagFloat* pressure_atm = new ObdTagFloat(ATM_PRESSURE, 2);
	ObdTagFloat* pressure_intake = new ObdTagFloat(INTAKE_PRESSURE, 2);
	ObdTagFloat* injection = new ObdTagFloat(INJECTION, 2);
	ObdTagFloat* fuel_consumption_h = new ObdTagFloat(FUEL_CONS_H, 2);
	ObdTagFloat* fuel_total = new ObdTagFloat(FUEL_TOTAL, 2);

	Timer* analog_reading_timer = new Timer(ANALOG_READING_TIMEOUT);
	Timer* obd_waiting_timer = new Timer(OBD_WAITING_TIMEOUT);
	Timer* obd_period_timer = new Timer(0);

	OBD* ecu = new OBD();
}



void MODEL::calculateTags(unsigned char page, unsigned char buffer[])
{
	switch(page)
	{
	case 0 :
		rpm->value = (256 * buffer[0] + buffer[1]) / 4;
		speed->value = buffer[2];
		break;

	case 1 :
		temp_engine->value = (double)temp_formula(buffer[0]);
		temp_intake->value = (double)temp_formula(buffer[1]);
		break;

	case 2 :
		injection->value = (double)(256 * buffer[4] + buffer[5]) / 250;
		break;

	default:
		break;
	}

}

void MODEL::calculateOtherTags(void)
{
	fuel_consumption_h->value = injection->value * rpm->value * INJECTOR_PERFOMANCE * 0.000002;
	fuel_total->value += fuel_consumption_h->value * obd_update_period / 3600000;
}

void MODEL::routine(void)
{
	if(analog_reading_timer->isOver())
	{

	}

	if (obd_waiting_timer->isOver())
	{
		speed->value = 0;
		rpm->value = 0;
		injection->value = 0;

		memory_offset = 0;
		connection_established = false;
		obd_waiting_timer->reset();
		ecu->readMemoryRequest(memory_offset, 16);
	}

	if (ecu->available())
	{
		connection_established = true;
		calculateTags(memory_offset, ecu->obd_buffer);
		memory_offset++;
		if (memory_offset > 16)
		{
			memory_offset = 0;
			obd_update_period = obd_period_timer->getTime();
			obd_period_timer->reset();
			calculateOtherTags();
		}
		ecu->readMemoryRequest(memory_offset, 16);
		obd_waiting_timer->reset();
	}

	ecu->routine();
}


//=========================================================================================================================




ObdTagInteger::ObdTagInteger(obdValsEnum arg)
{
	typeR = arg;
	value = 0;
}

ObdTagFloat::ObdTagFloat(obdValsEnum arg, unsigned char dig)
{
	typeR = arg;
	digits = dig;
	value = 0;
}


//=========================================================================================================================

Timer::Timer(unsigned long timeout)
{
	start_ = millis();
	timeout_ = timeout;
}

bool Timer::isOver(void)
{
	if (millis() > start_ + timeout_) return 1;
	else return 0;
}

void Timer::setTimeout(unsigned long timeout)
{
	timeout_ = timeout;	
}

void Timer::reset(void)
{
	start_ = millis();
}

unsigned long Timer::getTime(void)
{
	return millis() - start_;
}
