#include "screens.h"

void draw_analogs(void)
{
	double voltage = 0;

	lcd.setFontSize(FONT_SIZE_MEDIUM);
	lcd.setColor(analogs_color);
	lcd.setCursor(80, analogs_base_Y);
	voltage = voltage_scale(analogRead(analog_voltage_pin));
	lcd.print(voltage, 1);
	lcd.print("   ");
	lcd.setCursor(240, analogs_base_Y);
	lcd.print(analogRead(analog_tank_pin), DEC);
	lcd.print("  ");
	lcd.setCursor(0, analogs_base_Y + 2);
	lcd.print(millis(), DEC);
}

void update_period(unsigned long t)
{
	lcd.setFontSize(FONT_SIZE_MEDIUM);
	lcd.setColor(analogs_color);
	lcd.setCursor(240, analogs_base_Y + 2);
	lcd.print(t, DEC);
	lcd.print("  ");
}




void prepare_analogs(void)
{

	lcd.setColor(COLOR1);
	lcd.setFontSize(FONT_SIZE_MEDIUM);
	lcd.setCursor(0, analogs_base_Y);
	lcd.print("VOLTAGE: ");
	lcd.setCursor(160, analogs_base_Y);
	lcd.print("TANK: ");

	lcd.setCursor(160, analogs_base_Y + 2);
	lcd.print("PERIOD: ");
}

