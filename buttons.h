#ifndef buttons_h
#define buttons_h


#include "buttons.h"



#define BTNUP (1 << 4)
#define BTNDN (1 << 3)
#define BTNLE (1 << 0)
#define BTNRI (1 << 6)
#define BTNOK (1 << 5)
#define BTNF1 (1 << 2)
#define BTNF2 (1 << 1)

#define button(a) ((current_buttons & a) == 0)

unsigned char current_buttons;
  
unsigned char get_buttons(void)
{
  return ((PIND >> 1) & 0x0078) + (PINB & 0x03) + ((PINC & 0x01) << 2);
}

void update_buttons(void)
{
  current_buttons = get_buttons();
}







#endif
