#ifndef screens_h
#define screens_h





#define COLOR_TITLE VGA_LIME
#define COLOR1 VGA_YELLOW
#define COLOR2 VGA_OLIVE
#define BG_COLOR 0
#define dump_base_Y 4


#define analogs_base_Y 26
#define analogs_color 0x5555

#define V_REF 2.95
#define voltage_scale_factor 6.138

#define voltage_scale(a) ((V_REF / 1024) * a * voltage_scale_factor)

double voltage = 0;




void prepare_map(void)
{
  lcd.fill(0, 320, 0, TFT_LINE_HEIGHT * (analogs_base_Y-1));
  lcd.setCursor(0, 0);
  lcd.setFontSize(FONT_SIZE_MEDIUM);
  lcd.setColor(COLOR_TITLE);
  lcd.print("HONDA B16 OBD memory map");

  lcd.setFontSize(FONT_SIZE_SMALL);

  lcd.setCursor(0, dump_base_Y);
  lcd.print("      00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F");

  for(int i = 0; i < 16; i++)
  {
    if(i & 1) lcd.setColor(COLOR1);
    else lcd.setColor(COLOR2);
    
    lcd.setCursor(0, dump_base_Y + 2 + i);
    
    lcd.print("0x");
    lcd.print(i, HEX);
    lcd.print("0:");
  }
}



void update_map(unsigned char offset)
{
  lcd.setCursor(36, dump_base_Y + 2 + offset);
  
  lcd.setFontSize(FONT_SIZE_SMALL);    
  if(offset & 1) lcd.setColor(COLOR1);
  else lcd.setColor(COLOR2);

  for(int i = 0; i < 16; i++)
  {
    if(obd_buffer[i] < 15) lcd.print("0");
    lcd.print(obd_buffer[i], HEX);
    lcd.print(" ");
  }
}

void draw_analogs(void)
{
  lcd.setFontSize(FONT_SIZE_MEDIUM);  
  lcd.setColor(analogs_color);
  lcd.setCursor(80, analogs_base_Y);
  voltage = voltage_scale(analogRead(analog_voltage_pin));
  lcd.print(voltage, 1);
  lcd.print("   ");  
  lcd.setCursor(240, analogs_base_Y);
  lcd.print(analogRead(analog_tank_pin), DEC);  
  lcd.print("  ");   
  lcd.setCursor(0, analogs_base_Y + 2);
  lcd.print(millis(), DEC);  
}

void update_period(unsigned long t)
{
  lcd.setFontSize(FONT_SIZE_MEDIUM);   
  lcd.setColor(analogs_color);
  lcd.setCursor(240, analogs_base_Y + 2);
  lcd.print(t, DEC);
  lcd.print("  ");
}




void prepare_analogs(void)
{
  
  lcd.setColor(COLOR1);  
  lcd.setFontSize(FONT_SIZE_MEDIUM);  
  lcd.setCursor(0, analogs_base_Y);
  lcd.print("VOLTAGE: ");
  lcd.setCursor(160, analogs_base_Y);
  lcd.print("TANK: ");
  
  lcd.setCursor(160, analogs_base_Y + 2);
  lcd.print("PERIOD: ");
}



void prepare_parameters(void)
{
  lcd.fill(0, 320, 0, TFT_LINE_HEIGHT * (analogs_base_Y-1));
  lcd.setCursor(0, 0);
  lcd.setFontSize(FONT_SIZE_MEDIUM);
  
  lcd.setColor(COLOR_TITLE);
  lcd.print("HONDA B16 OBD Parameters");
  
  lcd.setColor(COLOR1);
  lcd.setXY(12, dump_base_Y * 8);
  lcd.print("RPM:");  
  lcd.setXY(12, dump_base_Y * 8 + 20 * 1);
  lcd.print("SPEED:");    
  lcd.setXY(12, dump_base_Y * 8 + 20 * 2);
  lcd.print("ENGINE t:");  
  lcd.setXY(12, dump_base_Y * 8 + 20 * 3);
  lcd.print("INTAKE t:");
  lcd.setXY(12, dump_base_Y * 8 + 20 * 4);
  lcd.print("INJECTION:");  
  lcd.setXY(12, dump_base_Y * 8 + 20 * 5);
  lcd.print("FUEL CONS.:");  
  lcd.setXY(12, dump_base_Y * 8 + 20 * 6);
  lcd.print("FUEL TOTAL:");
}


void update_parameters(void)
{
  lcd.setFontSize(FONT_SIZE_MEDIUM);
  lcd.setColor(analogs_color);
  lcd.setXY(160, dump_base_Y * 8);
  lcd.print(obdval_rpm, DEC);  
  lcd.print("     ");  
  
  lcd.setXY(160, dump_base_Y * 8 + 20 * 1);
  lcd.print(obdval_speed, DEC);  
  lcd.print(" KM/h    ");  
  
  lcd.setXY(160, dump_base_Y * 8 + 20 * 2);
  lcd.print(obdval_temp_engine);  
  lcd.print(" *C    ");  

  lcd.setXY(160, dump_base_Y * 8 + 20 * 3);
  lcd.print(obdval_temp_intake);  
  lcd.print(" *C    ");  

  lcd.setXY(160, dump_base_Y * 8 + 20 * 4);
  lcd.print(obdval_inj);  
  lcd.print(" ms    ");  
  
  lcd.setXY(160, dump_base_Y * 8 + 20 * 5);
  if(obdval_speed)
  {
    obdval_consumption_km = obdval_consumption_h / obdval_speed;
    lcd.print(obdval_consumption_km);  
    lcd.print(" L/KM    ");    
  }
  else
  {
    lcd.print(obdval_consumption_h);  
    lcd.print(" L/h    ");    
  }
  
  lcd.setXY(160, dump_base_Y * 8 + 20 * 6);
  lcd.print(obdval_total_fuel);  
  lcd.print(" L    ");  
}




class Form
{
  uint16_t width;
  uint16_t height;
  uint16_t bg_color;
  uint16_t bd_color;
  uint16_t text_color;
  const PROGMEM byte* text;
  
  public:
  Form(uint16_t width_val, uint16_t height_val, uint16_t bg_color_val, uint16_t bd_color_val, uint16_t text_color_val)
  {
    width = width_val;
    height = height_val;
    bg_color = bg_color_val;
    bd_color = bd_color_val;
    text_color = text_color_val;
  }
  
  void draw(uint16_t x, uint16_t y,  const __FlashStringHelper* text)
  {
    lcd.fill(x, x + width, y, y + height, bd_color);
    lcd.fill(x + 2, x + width - 2, y + 2, y + height - 2, bg_color);
    lcd.setXY(x + 4, y + 4);
    lcd.setBackColor(bg_color);
    lcd.setColor(text_color);
    lcd.print(text);   
    lcd.setXY(x + 10, y + 10);
    lcd.print(text);   

    lcd.setColor(VGA_WHITE);
    lcd.setBackColor(0);
  }
};

Form window(200, 50, 0x5678, 0x2345, VGA_RED);



#endif
