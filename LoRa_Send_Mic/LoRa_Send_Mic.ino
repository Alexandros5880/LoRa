#include "myLoRa.h"

#define DI00 2
#define PIN_SPI_RST   9
#define PIN_SPI_SS    10
/*
#define PIN_SPI_MOSI  11
#define PIN_SPI_MISO  12
#define PIN_SPI_SCK   13
*/



const int speacker_pin = 3;
const int mic_pin = A0;





// Async Lora Dellay = 1937 micros
// Sync Lora Delay = 376647 micros
// Human voice frequency:
// Male: 85 - 180 Hz
// Femaly: 165 - 255 Hz



// Create LoRa Object
long frequency = 433E6;
long bandwidth = 125E3;
int  spreading_fuctor = 7;
int tx_power = 0;
int sync_word = 0;
int coding_rate = 5;
long preamble_length = 0;

myLoRa * lora;





// Helper function read pin
byte read_pin();




void setup() {
  // Setup Serial
  Serial.begin(115200);
  while ( ! Serial );
  // Setup Lora
  lora = &myLoRa( frequency, bandwidth, spreading_fuctor, tx_power, sync_word, coding_rate, preamble_length );
  //lora = &myLoRa( frequency );
}






void loop() {
  lora->lora_send( read_pin );
}














// Helper function read pin
byte read_pin() {
  return (byte) analogRead(mic_pin) >> 2;  // 112 micros delay
}
