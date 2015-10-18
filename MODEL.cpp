// 
// 
// 

#include "MODEL.h"


MODEL::MODEL(void)
{
	tags_array[RPM] = (ObdTag*)new ObdTagIntger(RPM);
	tags_array[SPEED] = (ObdTag*)new ObdTagIntger(SPEED);
	tags_array[THROTTLE] = (ObdTag*)new ObdTagIntger(THROTTLE);
	tags_array[ENGINE_T] = (ObdTag*)new ObdTagFloat(ENGINE_T, 2);
	tags_array[INTAKE_T] = (ObdTag*)new ObdTagFloat(INTAKE_T, 2);
	tags_array[ATM_PRESSURE] = (ObdTag*)new ObdTagFloat(ATM_PRESSURE, 2);
	tags_array[INTAKE_PRESSURE] = (ObdTag*)new ObdTagFloat(INTAKE_PRESSURE, 2);
	tags_array[INJECTION] = (ObdTag*)new ObdTagFloat(INJECTION, 2);
	tags_array[FUEL_CONS_H] = (ObdTag*)new ObdTagFloat(FUEL_CONS_H, 2);
	tags_array[FUEL_TOTAL] = (ObdTag*)new ObdTagFloat(FUEL_TOTAL, 2);

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
		tags_array[RPM]->tag_value = (256 * buffer[0] + buffer[1]) / 4;
		tags_array[SPEED]->tag_value = buffer[2];
		break;

	case 1 :
		tags_array[ENGINE_T]->tag_value = (double)temp_formula(buffer[0]);
		tags_array[INTAKE_T]->tag_value = (double)temp_formula(buffer[1]);
		break;

	case 2 :
		tags_array[INJECTION]->tag_value = (double)(256 * buffer[4] + buffer[5]) / 250;
		break;

	default:
		break;
	}

}

void MODEL::calculateOtherTags(void)
{
	tags_array[FUEL_CONS_H]->tag_value = tags_array[INJECTION]->tag_value * tags_array[RPM]->tag_value * INJECTOR_PERFOMANCE * 0.000002;
	tags_array[ENGINE_T]->tag_value += tags_array[FUEL_CONS_H]->tag_value * obd_update_period / 3600000;
}

void MODEL::routine(void)
{
	if(analog_reading_timer->isOver())
	{

	}

	if (obd_waiting_timer->isOver())
	{
		tags_array[SPEED]->tag_value = 0;
		tags_array[RPM]->tag_value = 0;
		tags_array[INJECTION]->tag_value = 0;

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


ObdTagIntger::ObdTagIntger(obdValsEnum type)
{
	val_type = type;
	tag_value = 0;
	isFloat = 0;
	digits = -1;
}


ObdTagFloat::ObdTagFloat(obdValsEnum type, unsigned char dig)
{
	val_type = type;
	digits = dig;
	tag_value = 0; 
	isFloat = 1;
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