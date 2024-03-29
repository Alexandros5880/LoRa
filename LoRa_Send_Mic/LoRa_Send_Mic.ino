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

// Hellper Function fill the bufer and send the data (3486 Hz)
const int len = 60;
String buf[len];
void get_data();

void setup() {
  // Setup Serial
  #if defined(DEBUG)
    Serial.begin(115200);
    while ( ! Serial );
  #endif
  // Setip Pins
  pinMode(mic_pin, INPUT);
  pinMode(speacker, OUTPUT);
  // Setup Lora
  lora = &myLoRa( frequency, bandwidth, spreading_fuctor, tx_power, sync_word, coding_rate, preamble_length );
  //lora = &myLoRa( frequency );
  #if defined(DEBUG)
    Serial.println("START\n\n");
  #endif
}

void loop() {
  get_data();
}

void get_data() {
  // Loop per hulf of second
  long time_loop = 500000;
  long startTime = micros();
  int counter = 0;
  int i = 0;
  while ( (micros()-startTime) <= time_loop ) {
    // Fill buffer and then send it without LoRa
    int val = analogRead(mic_pin) >> 2;
    //analogWrite(speacker, val);
    //Serial.println(val);
    buf[i] =  String(val) + "&";
    i++;
    if (i == len-1) {
      lora->lora_send( buf, len );
      i = 0;
    }
    counter++;
  }

  #if defined(DEBUG)
    Serial.println( String(counter*2) + " Hz" );
  #endif
}
