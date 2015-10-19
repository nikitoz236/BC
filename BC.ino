/*************************************************************************
 * Demo sketch for MultiLCD library
 * Distributed under GPL v2.0
 * (C)2013-2014 developed by Stanley Huang <stanleyhuangyc@gmail.com>
 * For more information, please visit http://arduinodev.com
 *************************************************************************/
//$00$13$EE$10$10$10$CC$1F$1F$1F$1F$1F$1F$1F$1F$1F$1F$1F$34
//$00$13$AA$AA$55$55$55$55$55$55$55$55$55$55$55$55$55$55$12

#include "MODEL.h"
#include "VIEW.h"

#include <Arduino.h>
#include <SPI.h>
#include <MultiLCD.h>
//#include <avr/io.h>
//#include <avr/interrupt.h>
//#include "console.h"
//#include "OBD.h"
//#include "screens.h"
#include "buttons.h"




LCD_ILI9341 lcd;

class test_
{
public:
	ObdTagInteger* testtag2;
	test_(void)
	{
		ObdTagInteger* testtag2 = new ObdTagInteger(SPEED);
	}
};

test_* abraaca = new test_();


MODEL* comp = new MODEL();

//Layout* mylayout = new Layout(24);

ObdTagInteger* testtag = new ObdTagInteger(SPEED);
DisplayObjectInteger* di = new DisplayObjectInteger(comp->speed);

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
  
  lcd.setXY(0, 0);
  lcd.print("HONDA NEW MODEL COMPUTER");

  di->position_x_text = 12;
  di->position_x_value = 160;
  di->position_y = 24;
  di->draw();


  Serial.print("SPPED value:");
  Serial.print(comp->speed->typeR, DEC);

//  mylayout->draw();


//  lcd.setXY(0, 24);
//  lcd.print(obdParametersNames[3]);

	  

  
  while(1)
  {
//	  comp->routine();
//	  mylayout->update();
  
    
    //if(millis() - analog_time > ANALOG_READING_TIMEOUT)
    //{
    //  draw_analogs();
    //  analog_time = millis();
    //  
    //}
    
    //if(millis() < 1000)
    //{
    //  obd_time = 0;
    //  analog_time = 0;
    //}    
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