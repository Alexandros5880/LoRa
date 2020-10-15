#ifndef PINS_H
#include "pins.h"
#endif


#include "myLoRa.h"


#define DEBUG



// Create LoRa Object
long frequency = 433E6;
long bandwidth = 125E3;
int  spreading_fuctor = 7;
int tx_power = 20;
int sync_word = 0;
int coding_rate = 5;
long preamble_length = 0;

myLoRa * lora;


// Hellper Function fill the bufer and send the data (3486 Hz)
const int len = 250;
int buf[len];
void get_data();





void setup() {
  // Setup Serial
  Serial.begin(115200);
  pinMode(mic_pin, INPUT);
  pinMode(speacker_pin, OUTPUT);
  while ( ! Serial );
  // Setup Lora
  lora = &myLoRa( frequency, bandwidth, spreading_fuctor, tx_power, sync_word, coding_rate, preamble_length );
  //lora = &myLoRa( frequency );
}






void loop() {

  get_data();
  
}











void get_data() {
  long time_loop = 500000; //  0,5 second
  // Loop per 1 second
  long startTime = micros();
  int counter = 0;
  int i = 0;
  while ( (micros()-startTime) <= time_loop ) {

    // Fill buffer and then send it without LoRa
    buf[i] =  analogRead(mic_pin) >> 2;
    //analogWrite(speacker_pin, buf[i]);
    i++;
    if (i == len-1) {
      for (int j = 0; j < len; j++) {
        lora->lora_send( String(buf[j]) ); // Lora Send "i can comppes the value 2 bits down << 2"
      }
      i = 0;
    }
    
    counter++;
  }

  #if defined(DEBUG)
    Serial.println( String(counter*2) + " Hz" );
  #endif
}
