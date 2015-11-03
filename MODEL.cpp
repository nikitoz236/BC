// 
// 
// 

#include "MODEL.h"
#include "screens.h"


MODEL::MODEL(void)
{
	rpm = new ObdTagInteger(RPM);
	speed = new ObdTagInteger(SPEED);
	throttle = new ObdTagInteger(THROTTLE);
	temp_engine = new ObdTagFloat(ENGINE_T, 2);
	temp_intake = new ObdTagFloat(INTAKE_T, 2);
	pressure_atm = new ObdTagFloat(ATM_PRESSURE, 2);
	pressure_intake = new ObdTagFloat(INTAKE_PRESSURE, 2);
	injection = new ObdTagFloat(INJECTION, 2);
	fuel_consumption_h = new ObdTagFloat(FUEL_CONS_H, 2);
	fuel_consumption_km = new ObdTagFloat(FUEL_CONS_KM, 2);
	fuel_consumption = fuel_consumption_h;
	fuel_total = new ObdTagFloat(FUEL_TOTAL, 2);

	connection_established = false;
	frame_recieved = false;

	obd_update_period = 0;

	analog_reading_timer = new Timer(ANALOG_READING_TIMEOUT);
	obd_waiting_timer = new Timer(OBD_WAITING_TIMEOUT);
	obd_period_timer = new Timer(0);

	ecu = new OBD();
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
//	fuel_consumption_h->value = 15;
	fuel_consumption_h->value = injection->value * rpm->value * INJECTOR_PERFOMANCE * 0.000002;
	fuel_total->value += fuel_consumption_h->value * obd_update_period / 3600000;

	if (speed->value)
	{
		fuel_consumption_km->value = (fuel_consumption_h->value / speed->value) * 100;
		fuel_consumption->value = fuel_consumption_km->value;
		fuel_consumption->type = FUEL_CONS_KM;
	}
	else
	{
		fuel_consumption->value = fuel_consumption_h->value;
		fuel_consumption->type = FUEL_CONS_H;
	}

}


void MODEL::routine(void)
{
	if (analog_reading_timer->isOver())
	{
		draw_analogs();
		analog_reading_timer->reset();
		//if (speed->value) speed->value = 0;
		//else speed->value = 45;
		//calculateothertags();

	}

	if (obd_waiting_timer->isOver())
	{
//		Serial.print("\n\rtimeout");
		speed->value = 0;
		rpm->value = 0;
		injection->value = 0;

		memory_offset = 0;
		connection_established = false;
		frame_recieved = false;
		obd_update_period = 0;
		ecu->readMemoryRequest(memory_offset, 16);
		obd_waiting_timer->reset();
	}

	if (ecu->available())
	{
//		Serial.print("\n\rmessage");
		connection_established = true;
		calculateTags(memory_offset, ecu->obd_buffer);
		memory_offset++;
		if (memory_offset > 16)
		{
			memory_offset = 0;
			frame_recieved = true;
			obd_update_period = obd_period_timer->getTime();
			obd_period_timer->reset();
			calculateOtherTags();
		}
		ecu->readMemoryRequest(memory_offset, 16);
		obd_waiting_timer->reset();
	}

	if (millis() < 1000)
	{
		obd_period_timer->reset();
		analog_reading_timer->reset();
		obd_waiting_timer->reset();
	}


	ecu->routine();
}


//=========================================================================================================================




ObdTagInteger::ObdTagInteger(unsigned char arg)
{
	type = arg;
	value = 0;
}

ObdTagFloat::ObdTagFloat(unsigned char arg, unsigned char dig)
{
	type = arg;
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
