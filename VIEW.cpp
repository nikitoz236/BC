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


DisplayPages::DisplayPages(void)
{
	pages[0] = new LayoutPage(32, 1, 7);
	pages[0]->arr[0] = new DisplayObjectInteger(comp->rpm);
	pages[0]->arr[1] = new DisplayObjectInteger(comp->speed);
	pages[0]->arr[2] = new DisplayObjectFloat(comp->temp_engine);
	pages[0]->arr[3] = new DisplayObjectInteger(comp->engine_load);
	pages[0]->arr[4] = new DisplayObjectFloat(comp->injection);
	pages[0]->arr[5] = new DisplayObjectFloat(comp->fuel_consumption);
	pages[0]->arr[6] = new DisplayObjectFloat(comp->voltage_adc);

	pages[1] = new LayoutPage(32, 2, 7);
	pages[1]->arr[0] = new DisplayObjectInteger(comp->rpm);
	pages[1]->arr[1] = new DisplayObjectFloat(comp->injection);
	pages[1]->arr[2] = new DisplayObjectInteger(comp->obd_period);
	pages[1]->arr[3] = new DisplayObjectFloat(comp->fuel_consumption);
	pages[1]->arr[4] = new DisplayObjectFloat(comp->fuel_average);
	pages[1]->arr[5] = new DisplayObjectFloat(comp->fuel_total);
	pages[1]->arr[6] = new DisplayObjectFloat(comp->tank);

	pages[2] = new LayoutPage(32, 3, 4);
	pages[2]->arr[0] = new DisplayObjectFloat(comp->voltage_obd);
	pages[2]->arr[1] = new DisplayObjectFloat(comp->voltage_adc);
	pages[2]->arr[2] = new DisplayObjectFloat(comp->current);
	pages[2]->arr[3] = new DisplayObjectFloat(comp->alternator_load);

	pages[3] = new LayoutPage(32, 4, 8);
	pages[3]->arr[0] = new DisplayObjectFloat(comp->temp_engine);
	pages[3]->arr[1] = new DisplayObjectFloat(comp->temp_intake);
	pages[3]->arr[2] = new DisplayObjectFloat(comp->pressure_atm);
	pages[3]->arr[3] = new DisplayObjectFloat(comp->pressure_intake);
	pages[3]->arr[4] = new DisplayObjectInteger(comp->correction_short);
	pages[3]->arr[5] = new DisplayObjectInteger(comp->correction_long);
	pages[3]->arr[6] = new DisplayObjectInteger(comp->throttle);
	pages[3]->arr[7] = new DisplayObjectInteger(comp->idling_valve);

	current_page = pages[0];
	current_page->draw();
}


void DisplayPages::routine(void)
{
	update_buttons();
	if ((current_buttons != previous_buttons) && (current_buttons != 0x7F))
	{
		if (button(BTNLE)) current_page = pages[0];
		if (button(BTNUP)) current_page = pages[1];
		if (button(BTNRI)) current_page = pages[2];
		if (button(BTNDN)) current_page = pages[3];
		current_page->draw();
		
	}

	if (comp->need_update)
	{
		current_page->update();
		comp->need_update = false;
	}
}





LayoutPage::LayoutPage(uint16_t position_, unsigned char page_number_, unsigned char items_amount_)
{
	position = position_;
	page_number = page_number_;
	items_amount = items_amount_;
}


void LayoutPage::draw(void)
{

	lcd.fill(0, 320, position, 235, VGA_BLACK);
	lcd.setXY(200, position);
	lcd.setFontSize(FONT_SIZE_MEDIUM);
	lcd.setColor(VGA_RED);
	lcd.print("PAGE: ");
	lcd.print(page_number, DEC);

	for (int i = 0; i < items_amount; i++)
	{
		arr[i]->position_x_text = 12;
		arr[i]->position_x_value = 160;
		arr[i]->position_y = i * 20 + position + 20;

		arr[i]->draw();
	}
}

void LayoutPage::update(void)
{
	for (int i = 0; i < items_amount; i++)
	{
		arr[i]->update();
	}
}