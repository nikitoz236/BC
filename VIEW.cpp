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


void IntValueMedium::IntValueMedium(ObdTagIntger* a)
{
	tag = a;
}


void VIEW::VIEW()
{
	IntValueMedium rpm(comp.rpm);
	IntValueMedium speed;
	FloatValueMedium temp_engine;
	FloatValueMedium temp_intake;
	FloatValueMedium injection;
	FloatValueMedium consumption_h;
	FloatValueMedium fuel_total;

}