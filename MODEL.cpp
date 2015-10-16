// 
// 
// 

#include "MODEL.h"


MODEL::MODEL(void)
{

	ObdTagIntger* rpm = new ObdTagIntger(RPM);
	ObdTagIntger* speed = new ObdTagIntger(SPEED);
	ObdTagIntger throttle(THROTTLE);
	ObdTagFloat temp_engine(ENGINE_T, 2);
	ObdTagFloat temp_intake(INTAKE_T, 2);
	ObdTagFloat pressure_atm(ATM_PRESSURE, 2);
	ObdTagFloat pressure_intake(INTAKE_PRESSURE, 2);
	ObdTagFloat injection(INJECTION, 2);

	ObdTagFloat consumption_h(FUEL_CONS_H, 2);
	ObdTagFloat fuel_total(FUEL_TOTAL, 2);

	Timer analog_reading_timer(ANALOG_READING_TIMEOUT);
	Timer obd_waiting_timer(OBD_WAITING_TIMEOUT);
	Timer obd_period_timer(0);

}

void MODEL::calculateTags(unsigned char page, unsigned char buffer[])
{
	switch(page)
	{
	case 0 :
		rpm.tag_value = (256 * buffer[0] + buffer[1]) / 4;
		speed.tag_value = buffer[2];
		break;

	case 1 :
		temp_engine.tag_value = (double)temp_formula(buffer[0]);
		temp_intake.tag_value = (double)temp_formula(buffer[1]);
		break;

	case 2 :
		injection.tag_value = (double)(256 * buffer[4] + buffer[5]) / 250;
		break;

	default:
		break;
	}

}

void MODEL::calculateOtherTags(void)
{
	consumption_h = injection * rpm * INJECTOR_PERFOMANCE * 0.000002;
	fuel_total += consumption_h * obd_update_period / 3600000;	
}

void MODEL::routine(void)
{
	if(analog_reading_timer.isOver())
	{

	}

	if (obd_waiting_timer.isOver)
	{
		speed.tag_value= 0;
		rpm.tag_value = 0;
		injection.tag_value = 0;

		memory_offset = 0;
		connection_established = false;
		obd_waiting_timer.reset();
		ecu.readMemoryRequest(memory_offset, 16);
	}

	if (ecu.available())
	{
		connection_established = true;
		calculateTags(memory_offset, ecu.obd_buffer);
		memory_offset++;
		if (memory_offset > 16)
		{
			memory_offset = 0;
			obd_update_period = obd_period_timer.getTime();
			obd_period_timer.reset();
			calculateOtherTags();
		}
		ecu.readMemoryRequest(memory_offset, 16);
		obd_waiting_timer.reset();
	}

	ecu.routine();
}


//=========================================================================================================================


ObdTagIntger::ObdTagIntger(obdValsEnum type)
{
	val_type = type;
	tag_value = 0;
}


ObdTagFloat::ObdTagFloat(obdValsEnum type, unsigned char dig)
{
	val_type = type;
	digits = dig;
	tag_value = 0;
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