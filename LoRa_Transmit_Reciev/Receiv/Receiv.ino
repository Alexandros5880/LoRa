#ifndef SETUP_H
#include "setup.h"
#endif

#include "myLoRa.h"

// Create LoRa Object
long frequency = 43305E4;
long bandwidth = 125E3;
int  spreading_fuctor = 6; // 6 - 12
int tx_power = 0;  // Max 20
int sync_word = 127;
int coding_rate = 5; // 5,6,7,8
long preamble_length = 0;

myLoRa * lora;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting Reciever\n\n");
  // Setup Lora
  lora = &myLoRa( frequency, bandwidth, spreading_fuctor, tx_power, sync_word, coding_rate, preamble_length );
  //lora = &myLoRa( frequency );
}

void loop() {
  String data = lora->lora_receiving();
  if (data.length() > 0) {
    Serial.print("Recieved: ");
    Serial.println(data);
  }
}
