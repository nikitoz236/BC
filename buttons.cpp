

#include "buttons.h"



unsigned char current_buttons;
unsigned char previous_buttons;

unsigned char get_buttons(void)
{
	return ((PIND >> 1) & 0x0078) + (PINB & 0x03) + ((PINC & 0x01) << 2);
}

void update_buttons(void)
{
	previous_buttons = current_buttons;
	current_buttons = get_buttons();
}