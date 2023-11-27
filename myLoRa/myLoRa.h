#include "Arduino.h"
#include <SPI.h>
#include <LoRa.h>

class myLoRa {
	public:
		// Contsructor
    myLoRa( long &frequency );
		myLoRa( long &frequency, long &bandwidth, int &spreading_fuctor, 
                int &tx_power, int &sync_word, int &coding_rate, long &preamble_length );
		// Lora Receive
		String lora_receiving();
		// LoRa send
		void lora_send( byte (*func)() );
		// Helper function read pin
		byte read_pin();
};
