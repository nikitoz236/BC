// 
// 
// 

#include "VIEW.h"



unsigned char DisplayObject::getUnitsType(obdValsEnum type)
{
	return pgm_read_byte(&UnitsResolution[type]);
}



void checkType(uint16_t a)
{
	lcd.print("INT");
}

void checkType(double a)
{
	lcd.print("FLO");
}

DisplayObjectInteger::DisplayObjectInteger(ObdTagInteger* tag)
{
	tag_ = tag;
}

void DisplayObjectInteger::draw(void)
{
	lcd.setXY(position_x_text, position_y);
	lcd.setFontSize(FONT_SIZE_MEDIUM);
	lcd.setColor(0x5555);
	lcd.print((__FlashStringHelper *)obdParametersNames[tag_->val_type]);

	this->update();
}

void DisplayObjectInteger::update(void)
{
	lcd.setXY(position_x_value, position_y);
	lcd.setColor(VGA_YELLOW);

	lcd.print(tag_->tag_value, DEC);

	lcd.print(" ");
	lcd.print((__FlashStringHelper* ) obdParametersUnits[getUnitsType(tag_->val_type)]);
	lcd.print("    ");
}



DisplayObjectFloat::DisplayObjectFloat(ObdTagFloat* tag)
{
	tag_ = tag;
}

void DisplayObjectFloat::draw(void)
{
	lcd.setXY(position_x_text, position_y);
	lcd.setFontSize(FONT_SIZE_MEDIUM);
	lcd.setColor(0x5555);
	lcd.print((__FlashStringHelper *)obdParametersNames[tag_->val_type]);

	this->update();
}

void DisplayObjectFloat::update(void)
{
	lcd.setXY(position_x_value, position_y);
	lcd.setColor(VGA_YELLOW);

	lcd.print(tag_->tag_value, tag_->digits);
	
	lcd.print(" ");
	lcd.print((__FlashStringHelper *)obdParametersUnits[getUnitsType(tag_->val_type)]);
	lcd.print("    ");
}





Layout::Layout(uint16_t position)
{
	layout_array[0] = new DisplayObjectInteger(comp->rpm);
	layout_array[1] = new DisplayObjectInteger(comp->speed);
	layout_array[2] = new DisplayObjectFloat(comp->temp_engine);
	layout_array[3] = new DisplayObjectFloat(comp->temp_intake);
	layout_array[4] = new DisplayObjectFloat(comp->injection);
	layout_array[5] = new DisplayObjectFloat(comp->fuel_consumption_h);
	layout_array[6] = new DisplayObjectFloat(comp->fuel_total);

	for (int i = 0; i < 7; i++)
	{
		layout_array[i]->position_x_text = 12;
		layout_array[i]->position_x_value = 160;
		layout_array[i]->position_y = i * 24 + position;
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