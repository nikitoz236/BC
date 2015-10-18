#ifndef OBD_h
#define OBD_h

#include <Arduino.h>


#define obd_send(a) Serial.write(a)
#define obd_read Serial.read
#define obd_serial_available Serial.available

#define obd_data_begin 3

#define OBD_CS_ERROR     1
#define OBD_LENGTH_ERROR 2



class OBD
{
public:
	unsigned char obd_buffer[16];

	void routine(void);
	void readMemoryRequest(unsigned char offset, unsigned char amount);
	bool available(void);
	unsigned char getErrors(void);


private:
	bool obd_new_frame;
	unsigned char obd_amount;
	unsigned char obd_rx_length;
	unsigned char obd_rx_type;
	unsigned char obd_rx_cs;
	unsigned char obd_errors;
	unsigned char obd_state;
};




#endif
