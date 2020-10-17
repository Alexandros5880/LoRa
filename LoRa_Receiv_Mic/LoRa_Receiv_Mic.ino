#ifndef SETUP_H
#include "setup.h"
#endif


#include "myLoRa.h"



// Create LoRa Object
long frequency = 433E6;
long bandwidth = 125E3;
int  spreading_fuctor = 7;
int tx_power = 0;
int sync_word = 0;
int coding_rate = 5;
long preamble_length = 0;

myLoRa * lora;








void setup() {
  // Setup Serial
  Serial.begin(115200);
  while ( ! Serial );
  // Setup Lora
  lora = &myLoRa( frequency, bandwidth, spreading_fuctor, tx_power, sync_word, coding_rate, preamble_length );
  //lora = &myLoRa( frequency );
}







void loop() {  
  // Get data from Lora
  String data = lora->lora_receiving();
  if ( data != "" ) {
    Serial.println( data + "\n\n" );
  }
}
