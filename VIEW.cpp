// 
// 
// 

#include "VIEW.h"



unsigned char DisplayObject::getUnitsType(unsigned char type)
{
	return pgm_read_byte(&UnitsResolution[type]);
}

//=========================================================================================================================
DisplayObjectInteger::DisplayObjectInteger(ObdTagInteger* tag_)
{
	tag = tag_;
}

void DisplayObjectInteger::draw(void)
{
	lcd.setXY(position_x_text, position_y);
	lcd.setFontSize(FONT_SIZE_MEDIUM);
	lcd.setColor(0x5555);
	lcd.print((__FlashStringHelper *)obdParametersNames[tag->type]);
	update();
}

void DisplayObjectInteger::update(void)
{
	lcd.setXY(position_x_value, position_y);
	lcd.setFontSize(FONT_SIZE_MEDIUM);
	lcd.setColor(VGA_YELLOW);
	lcd.print(tag->value, DEC);

	lcd.print(" ");
	lcd.print((__FlashStringHelper *)obdParametersUnits[getUnitsType(tag->type)]);
	lcd.print("    ");
}



//=========================================================================================================================
DisplayObjectFloat::DisplayObjectFloat(ObdTagFloat* tag_)
{
	tag = tag_;
}

void DisplayObjectFloat::draw(void)
{
	lcd.setXY(position_x_text, position_y);
	lcd.setFontSize(FONT_SIZE_MEDIUM);
	lcd.setColor(0x5555);
	lcd.print((__FlashStringHelper *)obdParametersNames[tag->type]);

	this->update();
}

void DisplayObjectFloat::update(void)
{
	lcd.setXY(position_x_value, position_y);
	lcd.setFontSize(FONT_SIZE_MEDIUM);
	lcd.setColor(VGA_YELLOW);
	lcd.print(tag->value, tag->digits);

	lcd.print(" ");
	lcd.print((__FlashStringHelper *)obdParametersUnits[getUnitsType(tag->type)]);
	lcd.print("    ");
}

//=========================================================================================================================




Layout::Layout(uint16_t position)
{
	layout_array[0] = new DisplayObjectInteger(comp->rpm);
	layout_array[1] = new DisplayObjectInteger(comp->speed);
	layout_array[2] = new DisplayObjectFloat(comp->temp_engine);
	layout_array[3] = new DisplayObjectFloat(comp->temp_intake);
	layout_array[4] = new DisplayObjectFloat(comp->injection);
	layout_array[5] = new DisplayObjectFloat(comp->fuel_consumption);
	layout_array[6] = new DisplayObjectFloat(comp->fuel_total);

	for (int i = 0; i < 7; i++)
	{
		layout_array[i]->position_x_text = 12;
		layout_array[i]->position_x_value = 160;
		layout_array[i]->position_y = i * 20 + position;
	}
}


void Layout::draw(void)
{
	for (int i = 0; i < 7; i++)
	{
		layout_array[i]->draw();
	}
}

void Layout::update(void)
{
	for (int i = 0; i < 7; i++)
	{
		layout_array[i]->update();
	}
}