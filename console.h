#ifndef console_h
#define console_h

#include <inttypes.h>
#include <MultiLCD.h>


#define console_max_x	50
#define console_max_y	10
#define console_position 3

extern LCD_ILI9341 lcd;

class console : public Print
{
  uint16_t console_x;
  uint16_t console_y;
  char console_buffer[console_max_x][console_max_y];
  
  public:  
  console()
  {
    console_x = 0;
    console_y = 0;
    lcd.setCursor(0, 0);
    lcd.setFontSize(FONT_SIZE_MEDIUM);
    lcd.print("Console");
    lcd.setCursor(0, console_position);
    lcd.setFontSize(FONT_SIZE_SMALL);
    
  }    
  size_t write(uint8_t character);
};

#endif
