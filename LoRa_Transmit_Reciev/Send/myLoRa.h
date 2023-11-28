#include "Arduino.h"
#include <SPI.h>
#include <LoRa.h>

#ifndef SETUP_H
#include "setup.h"
#endif

class myLoRa {
	public:
		// Contsructor
    myLoRa( long &frequency );
		myLoRa( long &frequency, long &bandwidth, int &spreading_fuctor, 
                int &tx_power, int &sync_word, int &coding_rate, long &preamble_length );
		// Lora Receive
		String lora_receiving();
    String lora_receiving_str();
		// LoRa send
		void lora_send( String val[], int len, int implicitHeader = false );
    void lora_send( String val, int implicitHeader = false );
    void lora_send( const char * data, int implicitHeader = false );
		// Helper function read pin
		byte read_pin();
};
