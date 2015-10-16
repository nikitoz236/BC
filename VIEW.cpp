// 
// 
// 

#include "VIEW.h"



unsigned char DisplayObject::getUnitsType(obdValsEnum type)
{
	return UnitsResolution[type];
}

void FloatValueMedium::draw(void)
{
	lcd.setXY(position_x_text, position_y);
	lcd.setFontSize(FONT_SIZE_MEDIUM);
	lcd.setColor(VGA_YELLOW);
	lcd.print(obdParametersNames[tag->val_type]);
}

void FloatValueMedium::update(void)
{
	lcd.setXY(position_x_value, position_y);
	lcd.setColor(0x5555);
	lcd.print(tag->tag_value, tag->digits);
	lcd.print(" ");
	lcd.print(obdParametersUnits[getUnitsType(tag->val_type));
	lcd.print("    ");
}

void IntValueMedium::draw(void)
{
	lcd.setXY(position_x_text, position_y);
	lcd.setFontSize(FONT_SIZE_MEDIUM);
	lcd.setColor(VGA_YELLOW);
	lcd.print(obdParametersNames[tag->val_type]);
}

void IntValueMedium::update(void)
{
	lcd.setXY(position_x_value, position_y);
	lcd.setColor(0x5555);
	lcd.print(tag->tag_value, DEC);
	lcd.print(" ");
	lcd.print(obdParametersUnits[getUnitsType(tag->val_type));
	lcd.print("    ");
}



FloatValueMedium::FloatValueMedium(ObdTagFloat* arg)
{
	tag = arg;
}

IntValueMedium::IntValueMedium(ObdTagIntger* arg)
{
	tag = arg;
}

VIEW::VIEW(void)
{
	IntValueMedium* rpm = new IntValueMedium(comp->rpm);
	IntValueMedium* speed = new IntValueMedium(comp->speed);
	FloatValueMedium* temp_engine = new FloatValueMedium(comp.temp_engine);
	FloatValueMedium* temp_intake = new FloatValueMedium(comp.temp_intake);
	FloatValueMedium* injection = new FloatValueMedium(comp.injection);
	FloatValueMedium* consumption_h = new FloatValueMedium(comp.consumption_h);
	FloatValueMedium* fuel_total = new FloatValueMedium(comp.fuel_total);

}