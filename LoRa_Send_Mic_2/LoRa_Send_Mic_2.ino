#include <SPI.h>
#include <LoRa.h>



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










void setup() {
  // Setup Serial
  Serial.begin(115200);
  while ( ! Serial );
  // Setup Lora
  while ( ! LoRa.begin(433E6) );  // GREECE: 433,050–434,040 MHz  434,040–434,790 MHz
  LoRa.setSpreadingFactor(7); // 6 - 12
  LoRa.setSignalBandwidth(125E3); // 125, 250, 500
  LoRa.setTxPower(16);  // Max 20
  //LoRa.setSyncWord(3); // Big delay 
  LoRa.setCodingRate4(5); // 5,6,7,8
  //LoRa.setPreambleLength(3);
  //LoRa.crc();
  Serial.println("LoRa Sender Starts");
}








// 500 Hz
int wait = 2000;
long previusTime = 0, currentTime = 0;
const int len = 100;
String data[len];



void loop() {

  //long startTime = micros();

  // 500 Hz analog Read create buffer
  int counter = 0;
  while ( counter < len ) {
    currentTime = micros();
    if ( (currentTime - previusTime) >= wait ) {
      previusTime = currentTime;
      //analogWrite( speacker_pin, analogRead(mic_pin) >> 2 );
      data[counter] = String(analogRead(mic_pin) >> 2) + "&";
      counter++;
    }
  }

  // LoRa send
  LoRa.beginPacket();
  for ( int i = 0; i < len; i++ ) {
    Serial.println(data[i]);
    LoRa.print( data[i] );
  }
  LoRa.endPacket();

  //Serial.println( "Time: " + String(micros()-startTime) );
}
