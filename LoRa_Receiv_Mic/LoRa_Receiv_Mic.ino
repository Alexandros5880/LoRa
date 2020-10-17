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
  pinMode(speacker, OUTPUT);
  // Setup Lora
  lora = &myLoRa( frequency, bandwidth, spreading_fuctor, tx_power, sync_word, coding_rate, preamble_length );
  //lora = &myLoRa( frequency );
}






void loop() {

  
  // Get data from Lora
  char baff[len1][len2];
  lora->lora_receiving(baff);

  
  // Print buffer
  for (int i = 0; i < len1; i++) {
    for (int j = 0; j < len2; j++) {
      //int val = String(baff[i][j]).toInt();
      //Serial.print( val );
      //analogWrite(speacker, val);
      Serial.print( String(baff[i][j]) );
    }
    Serial.println();
  }
  


}
