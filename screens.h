#ifndef screens_h
#define screens_h


#include "MODEL.h"
#include <MultiLCD.h>

extern LCD_ILI9341 lcd;



#define COLOR_TITLE VGA_LIME
#define COLOR1 VGA_YELLOW
#define COLOR2 VGA_OLIVE
#define BG_COLOR 0
#define dump_base_Y 4


#define analogs_base_Y 26
#define analogs_color 0x5555


void draw_analogs(void);
void update_period(unsigned long t);
void prepare_analogs(void);



//void prepare_map(void)
//{
//  lcd.fill(0, 320, 0, TFT_LINE_HEIGHT * (analogs_base_Y-1));
//  lcd.setCursor(0, 0);
//  lcd.setFontSize(FONT_SIZE_MEDIUM);
//  lcd.setColor(COLOR_TITLE);
//  lcd.print("HONDA B16 OBD memory map");
//
//  lcd.setFontSize(FONT_SIZE_SMALL);
//
//  lcd.setCursor(0, dump_base_Y);
//  lcd.print("      00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F");
//
//  for(int i = 0; i < 16; i++)
//  {
//    if(i & 1) lcd.setColor(COLOR1);
//    else lcd.setColor(COLOR2);
//    
//    lcd.setCursor(0, dump_base_Y + 2 + i);
//    
//    lcd.print("0x");
//    lcd.print(i, HEX);
//    lcd.print("0:");
//  }
//}
//
//
//
//void update_map(unsigned char offset)
//{
//  lcd.setCursor(36, dump_base_Y + 2 + offset);
//  
//  lcd.setFontSize(FONT_SIZE_SMALL);    
//  if(offset & 1) lcd.setColor(COLOR1);
//  else lcd.setColor(COLOR2);
//
//  for(int i = 0; i < 16; i++)
//  {
//    if(obd_buffer[i] < 15) lcd.print("0");
//    lcd.print(obd_buffer[i], HEX);
//    lcd.print(" ");
//  }
//}







#endif
