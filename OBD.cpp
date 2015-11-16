#include "OBD.h"

#include "obdtestmap.h"






#if defined(TEST_OBD)

void OBD::routine(void)
{
}


void OBD::readMemoryRequest(unsigned char offset, unsigned char amount)
{
	for (int i = 0; i < amount; i++)
	{
		obd_buffer[i] = pgm_read_byte(&obdTestMap[offset + i]);
	}
	obd_errors = 0;
	obd_new_frame = 1;
}



#else

void OBD::routine(void)
{
  
  if((obd_serial_available()) && (!obd_new_frame))
  {
    unsigned char c = obd_read();
    obd_rx_cs += c;
    obd_state++;
 //   Serial.print(obd_state, DEC);
    switch(obd_state)
    {
      case 1:
        obd_rx_type = c;
        break;
        
      case 2:
        obd_rx_length = c;
        if(obd_amount != (obd_rx_length - 3)) obd_errors |= (1 << OBD_LENGTH_ERROR);
        break;
        
      case 101:
        if(obd_rx_cs != 0) obd_errors |= (1 << OBD_CS_ERROR);
        obd_state = 0;
        if(obd_rx_type != 0x20) obd_new_frame = 1;
        break;

      default:
        obd_buffer[obd_state - obd_data_begin] = c;
        if((obd_state == (obd_rx_length - 1)) || (obd_state == 0x12)) obd_state = 100;
        break;        
    }
  }
}


void OBD::readMemoryRequest(unsigned char offset, unsigned char amount)
{
  while(obd_serial_available()) obd_read();
  obd_send(0x20);
  obd_send(0x05);
  obd_send(offset);
  obd_send(amount);
  obd_send(0x0100 - (0x00FF & (0x20 + 0x05 + offset + amount)));


  
  obd_new_frame = 0;
  obd_rx_length = 0;
  obd_rx_cs = 0;
  obd_errors = 0;
  obd_state = 0;
  obd_amount = amount;


//  Serial.print("\n\rrequest");
}


#endif

bool OBD::available(void)
{
  return obd_new_frame;
}

unsigned char OBD::getErrors(void)
{
  return obd_errors;
}
