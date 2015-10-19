// 
// 
// 

#include "VIEW.h"



DisplayObject::DisplayObject(ObdTag* tag)
{
	tag_ = tag;
}

unsigned char DisplayObject::getUnitsType(obdValsEnum type)
{
	return pgm_read_byte(&UnitsResolution[type]);
}

void DisplayObject::draw(void)
{
	lcd.setXY(position_x_text, position_y);
	lcd.setFontSize(FONT_SIZE_MEDIUM);
	lcd.setColor(0x5555);
	lcd.print((__FlashStringHelper *)obdParametersNames[tag_->val_type]);

	this->update();
}

void checkType(uint16_t a)
{
	lcd.print("INT");
}

void checkType(double a)
{
	lcd.print("FLO");
}



void DisplayObject::update(void)
{
	lcd.setXY(position_x_value, position_y);
	lcd.setColor(VGA_YELLOW);

	lcd.print(tag_->toString());
	
	lcd.print(" ");
	lcd.print((__FlashStringHelper *)obdParametersUnits[getUnitsType(tag_->val_type)]);
	lcd.print("    ");
}





Layout::Layout(uint16_t position)
{
	layout_array[0] = new DisplayObject(comp->tags_array[RPM]);
	layout_array[1] = new DisplayObject(comp->tags_array[SPEED]);
	layout_array[2] = new DisplayObject(comp->tags_array[ENGINE_T]);
	layout_array[3] = new DisplayObject(comp->tags_array[INTAKE_T]);
	layout_array[4] = new DisplayObject(comp->tags_array[INJECTION]);
	layout_array[5] = new DisplayObject(comp->tags_array[FUEL_CONS_H]);
	layout_array[6] = new DisplayObject(comp->tags_array[FUEL_TOTAL]);

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