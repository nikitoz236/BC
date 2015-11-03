/*************************************************************************
 * Demo sketch for MultiLCD library
 * Distributed under GPL v2.0
 * (C)2013-2014 developed by Stanley Huang <stanleyhuangyc@gmail.com>
 * For more information, please visit http://arduinodev.com
 *************************************************************************/
//$00$13$EE$10$10$10$CC$1F$1F$1F$1F$1F$1F$1F$1F$1F$1F$1F$34
//$00$13$AA$AA$55$55$55$55$55$55$55$55$55$55$55$55$55$55$12

#include <Arduino.h>
#include <SPI.h>
#include <MultiLCD.h>
//#include <avr/io.h>
//#include <avr/interrupt.h>
//#include "console.h"
//#include "OBD.h"
#include "buttons.h"
#include "MODEL.h"
#include "VIEW.h"
#include "screens.h"


LCD_ILI9341 lcd;




MODEL* comp;
Layout* mylayout;

unsigned char a;


void setup()
{
  Serial.begin(9600);
  
  PORTD |= 0xF0;
  PORTB |= 0x03;
  PORTC |= 0x01;

  analogReference(DEFAULT);

  lcd.setBackLight(0);
  lcd.begin();
  lcd.clear();
  


  comp = new MODEL();
  mylayout = new Layout(32);


  lcd.setXY(0, 0);
  lcd.setColor(VGA_GREEN);
  lcd.print("HONDA B16 Engine OBD computer");
  mylayout->draw();

  prepare_analogs();


	  

  
  while(1)
  {
	  comp->routine();
	  if (comp->frame_recieved)
	  {
		  mylayout->update();
		  update_period(comp->obd_update_period);
		  comp->frame_recieved = false;
	  }
  }
}

//================================================================================================================================================================================================


//void scan_buttons(void)
//{  
//  update_buttons();
//  if(button(BTNF1) && button(BTNUP)) bgrcol++;
//  if(button(BTNF1) && button(BTNDN)) bgrcol--;
//  if(button(BTNF1) && button(BTNLE)) bgrcol-=0x0100;
//  if(button(BTNF1) && button(BTNRI)) bgrcol+= 0x0100;
//  if(button(BTNF2) && button(BTNUP)) txtcol++;
//  if(button(BTNF2) && button(BTNDN)) txtcol--;
//  if(button(BTNF2) && button(BTNLE)) txtcol-=0x0100; 
//  if(button(BTNF2) && button(BTNRI)) txtcol+= 0x0100; 
//
//  if(button(BTNUP) || button(BTNDN) || button(BTNLE) || button(BTNRI))redraw_color_test();
//
//  delay(10);
//
//}

//================================================================================================================================================================================================

void buttons_helper(void)
{
  lcd.setColor(VGA_YELLOW);
  lcd.setBackColor(0);
  lcd.setCursor(0, 24);

  a = get_buttons();
  
  if((a & BTNUP) == 0) lcd.print("UP ");
  if((a & BTNDN) == 0) lcd.print("DN ");
  if((a & BTNLE) == 0) lcd.print("LE ");
  if((a & BTNRI) == 0) lcd.print("RI ");
  if((a & BTNOK) == 0) lcd.print("OK ");
  if((a & BTNF1) == 0) lcd.print("F1 ");
  if((a & BTNF2) == 0) lcd.print("F2 ");
  lcd.print(a, HEX);
  lcd.print("                           ");
}

void loop(void)
{

}