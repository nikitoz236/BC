#ifndef buttons_h
#define buttons_h

#include "MODEL.h"
#include <MultiLCD.h>

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif




#define BTNUP (1 << 4)
#define BTNDN (1 << 3)
#define BTNLE (1 << 0)
#define BTNRI (1 << 6)
#define BTNOK (1 << 5)
#define BTNF1 (1 << 2)
#define BTNF2 (1 << 1)

#define button(a) ((current_buttons & a) == 0)



extern unsigned char current_buttons;
extern unsigned char previous_buttons;

unsigned char get_buttons(void);
void update_buttons(void);




#endif
