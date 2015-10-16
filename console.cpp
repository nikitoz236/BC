


#include "console.h"

  
size_t console::write(uint8_t character) 
{
  if(console_x == console_max_x)
  {
    console_x = 0;
    console_y++;      
  }
  
  if(console_y == console_max_y)
  {
    for(int fy = 0; fy < console_max_y; fy++)
    {
      lcd.setCursor(0, fy + console_position); 
      for(int fx = 0; fx < console_max_x; fx++)
      {
        if(fy == (console_max_y - 1)) console_buffer[fx][fy] = ' ';
        else console_buffer[fx][fy] = console_buffer[fx][fy + 1]; 
        lcd.write(console_buffer[fx][fy]);
      }          
    }
    console_y--;
  }
  
  lcd.setCursor(console_x*6, console_y + console_position);

  if(character == '\n')
  {
    console_y++;
    
  } else if(character == '\r')
  {
    console_x = 0;
  } else
  {
    lcd.write(character);
    console_buffer[console_x][console_y] = character;
    console_x++;
  }
}

