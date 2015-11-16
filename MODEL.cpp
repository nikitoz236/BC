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
	fuel_average = new ObdTagFloat(FUEL_AVERAGE, 2);
	alternator_load = new ObdTagFloat(ALTERNATOR_LOAD, 2);
	engine_load = new ObdTagInteger(ENGINE_LOAD);
	correction_short = new ObdTagInteger(CORRECTION_SHORT);
	correction_long = new ObdTagInteger(CORRECTION_LONG);
	idling_valve = new ObdTagInteger(IDLING_VALVE);
	current = new ObdTagFloat(CURRENT, 2);
	voltage_obd = new ObdTagFloat(VOLTAGE_OBD, 2);
	voltage_adc = new ObdTagFloat(VOLTAGE_ADC, 2);
	tank = new ObdTagFloat(TANK, 2);
	obd_period = new ObdTagInteger(OBD_PERIOD);


	connection_established = false;
	need_update = false;

	obd_update_period = 0;

	analog_reading_timer = new Timer(ANALOG_READING_TIMEOUT);
	obd_waiting_timer = new Timer(OBD_WAITING_TIMEOUT);
	obd_period_timer = new Timer(0);

	ecu = new OBD();
}


double MODEL::temp_formula(double x)
{
	double result = 155.04149 - x*3.0414878 + x*x*0.03952185 - x*x*x*0.00029383913 + x*x*x*x*0.0000010792568 - x*x*x*x*x*0.0000000015618437;
	return result;
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
		temp_engine->value = temp_formula(buffer[0]);
		temp_intake->value = temp_formula(buffer[1]);
		pressure_intake->value = (buffer[2] * 0.716) - 5;
		pressure_atm->value = (buffer[3] * 0.716) - 5;
		throttle->value = (buffer[4] - 24) / 2;
		voltage_obd->value = buffer[7] / 10.45;
		alternator_load->value = buffer[8] / 2.55;
		current->value = 77.06 - (buffer[9] / 2.5371);

		break;

	case 2 :
		correction_short->value = (double)((buffer[0] / 128 - 1) * 100);
		correction_long->value = (double)((buffer[2] / 128 - 1) * 100);
		injection->value = (double)(256 * buffer[4] + buffer[5]) / 250;
		idling_valve->value = buffer[8] / 2.55;
		break;

	case 9 :
		engine_load->value = buffer[0xC] / 2.55;
		break;

	default:
		break;
	}

}

void MODEL::calculateAnalogTags(void)
{
	voltage_adc->value = voltage_scale(analogRead(analog_voltage_pin));
	tank->value = tank->value * 0.8 + (analogRead(analog_tank_pin)) * 0.2;
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

		fuel_average->value = fuel_average->value * 0.95 + fuel_consumption_km->value * 0.05;
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
		calculateAnalogTags();
		need_update = true;
		analog_reading_timer->reset();
	}

	if (obd_waiting_timer->isOver())
	{
//		Serial.print("\n\rtimeout");
		speed->value = 0;
		rpm->value = 0;
		injection->value = 0;

		memory_offset = 0;
		connection_established = false;
		need_update = false;
		obd_update_period = 0;
		ecu->readMemoryRequest(16 * memory_offset, 16);
		obd_waiting_timer->reset();
		obd_period_timer->reset();
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
			need_update = true;
			obd_period->value = obd_period_timer->getTime();
			obd_period_timer->reset();
			calculateOtherTags();
		}
		ecu->readMemoryRequest(16 * memory_offset, 16);
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

ObdTagBin::ObdTagBin(unsigned char arg)
{
	type = arg;
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
