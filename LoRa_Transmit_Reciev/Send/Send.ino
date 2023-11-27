#ifndef SETUP_H
#include "setup.h"
#endif

#include "myLoRa.h"

// Create LoRa Object
long frequency = 433E6;
long bandwidth = 125E3;
int  spreading_fuctor = 7;
int tx_power = 10;
int sync_word = 0;
int coding_rate = 5;
long preamble_length = 0;

myLoRa * lora;

String message = String("Hello Alexandros");
const int len = message.length();

void setup() {
  Serial.begin(115200);
  Serial.println("Starting Sender\n\n");
  // Setup Lora
  //lora = &myLoRa( frequency, bandwidth, spreading_fuctor, tx_power, sync_word, coding_rate, preamble_length );
  lora = &myLoRa( frequency );
}

void loop() {
  lora->lora_send( message );
  delay(5000);
}
