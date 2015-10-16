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
#define analog_voltage_pin A6
#define analog_tank_pin A7

#include <Arduino.h>
#include <SPI.h>
#include <MultiLCD.h>
//#include <avr/io.h>
//#include <avr/interrupt.h>
#include "console.h"
#include "OBD.h"
#include "screens.h"
#include "buttons.h"




LCD_ILI9341 lcd;
unsigned char a;

unsigned long obd_time = 0;
unsigned long analog_time = 0;

unsigned char memory_offset = 0;
unsigned char mode = 1;


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
  

  prepare_parameters();
  prepare_analogs();
  
  while(1)
  {
    update_buttons();
    if(button(BTNF2) && (mode != 1))
    { 
      mode = 1;
      prepare_parameters();
      memory_offset = 0;
    }
    
    if(button(BTNF1) && (mode != 0))
    {
      mode = 0;
      prepare_map();
      memory_offset = 0;
    }

    
    if((millis() - obd_time > OBD_WAITING_TIMEOUT) || (obd_available()))
    {
      if(mode == 0)
      {
        if(obd_available())
        {
          update_map(memory_offset);
        }
        else
        {
          lcd.setFontSize(FONT_SIZE_SMALL);
          lcd.setCursor(36, dump_base_Y + 2 + memory_offset); 
          lcd.print("                                               ");
        }        
        memory_offset++;
        if(memory_offset == 16)
        {
          memory_offset = 0;
          obd_period = millis() - obd_begin;
          obd_begin = millis();
        }
      } else if(mode == 1)
      {
        if(obd_available())
        {
          calculate_parameters(memory_offset);
        }
        else
        {
          obdval_speed = 0;
          obdval_rpm = 0;
          obdval_inj = 0;
        }
        
        memory_offset++;
        if(memory_offset == 5)
        {

          
          update_parameters();

          memory_offset = 0;
          obd_period = millis() - obd_begin;
          obd_begin = millis();
        }
      
      }
      obd_read_memory_request(16 * memory_offset, 16);
      obd_time = millis();
      update_period(obd_period);      
    }
    
    if(millis() - analog_time > ANALOG_READING_TIMEOUT)
    {
      draw_analogs();
      analog_time = millis();
      
    }
    
    if(millis() < 1000)
    {
      obd_time = 0;
      analog_time = 0;
    }
    
    obd_routine(); 
  }
  

//    redraw_color_test();
//
//  while(1)
//  {
//    scan_buttons();
//    buttons_helper();
//  }

  console cons;
  
  cons.print(F("Send request to ECU"));
  obd_read_memory_request(16, 16);
  
  
  while(!obd_available())
  {
    obd_routine();
  }
  cons.print(F("\n\rRecieved data:\r\n"));
  for(int i = 0; i < 16; i++)
  {
      if(obd_buffer[i] < 16)cons.print("0");
      cons.print(obd_buffer[i], HEX);
      cons.print(" ");
  }
  cons.print(F("\n\rErrors: "));  
  cons.print(obd_get_errors(), BIN);  
  


 
  
  



}

//================================================================================================================================================================================================


void print_items(int y_begin, int acive_item, int items_amount)
{
  lcd.setFontSize(FONT_SIZE_SMALL);
    lcd.setBackColor(0x3456);
  for(int i = 0; i < items_amount; i++)
  {
    lcd.setCursor(0, y_begin + i);
    if(i == acive_item) lcd.setColor(VGA_WHITE);
    else lcd.setColor(VGA_RED);
    lcd.print("item");
    lcd.print(i);    
  }

}





int   active  = 0;


void loop()
{

//  print_items(6, active, 11);
//  if((a & BTNUP) == 0)
//  { 
//    lcd.print("UP ");
//    if (active > 0) active--;
//  }
//  
//  if((a & BTNDN) == 0)
//  { 
//    lcd.print("DN ");
//    if (active < 10) active++;
//  }


}








//================================================================================================================================================================================================








uint16_t bgrcol = 0x0000;
uint16_t txtcol = 0xFFFF;

void redraw_color_test(void)
{

  lcd.fill(100, 200, 100, 131, bgrcol);
  lcd.setXY(100, 100);
  lcd.setColor(txtcol);
  lcd.setBackColor(bgrcol);
  lcd.setFontSize(FONT_SIZE_MEDIUM);
  lcd.print("test");
  lcd.setXY(100, 116);
  lcd.setFontSize(FONT_SIZE_SMALL);
  lcd.print(123, HEX);

  lcd.setBackColor(0x0000);
  lcd.setFontSize(FONT_SIZE_MEDIUM);
  lcd.setXY(100, 132);
  lcd.setColor(txtcol);
  lcd.print(txtcol, HEX);
  
  lcd.setXY(100, 148);
  lcd.setColor(bgrcol);
  lcd.print(bgrcol, HEX);
  
}

void scan_buttons(void)
{  
  update_buttons();
  if(button(BTNF1) && button(BTNUP)) bgrcol++;
  if(button(BTNF1) && button(BTNDN)) bgrcol--;
  if(button(BTNF1) && button(BTNLE)) bgrcol-=0x0100;
  if(button(BTNF1) && button(BTNRI)) bgrcol+= 0x0100;
  if(button(BTNF2) && button(BTNUP)) txtcol++;
  if(button(BTNF2) && button(BTNDN)) txtcol--;
  if(button(BTNF2) && button(BTNLE)) txtcol-=0x0100; 
  if(button(BTNF2) && button(BTNRI)) txtcol+= 0x0100; 

  if(button(BTNUP) || button(BTNDN) || button(BTNLE) || button(BTNRI))redraw_color_test();

  delay(10);

}

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
