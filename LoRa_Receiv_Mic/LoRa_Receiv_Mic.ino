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





// Async Lora Dellay = 1937 micros
// Sync Lora Delay = 376647 micros
// Human voice frequency:
// Male: 85 - 180 Hz
// Femaly: 165 - 255 Hz

int wait = 1261; // micros
long previusTime = 0, currentTime = 0;


const int len = 1000;
byte data[len];








void setup() {
  // Setup Serial
  Serial.begin(115200);
  while ( ! Serial );
  // Setup Lora
  while ( ! LoRa.begin(433E6) );  // GREECE: 433,050–434,040 MHz  434,040–434,790 MHz
  LoRa.setSpreadingFactor(7); // 6 - 12
  LoRa.setSignalBandwidth(125E3); // 125, 250, 500
  //LoRa.setTxPower(16);  // Max 20
  //LoRa.setSyncWord(3); // Big delay
  LoRa.setCodingRate4(5); // 5,6,7,8
  //LoRa.setPreambleLength(3);
  //LoRa.crc();
  Serial.println("LoRa Receiver Starts");
}








int counter = 0;
boolean check_buffer = false;

void loop() {


  // Get data from Lora
  int packetSize = LoRa.parsePacket();
  if ( packetSize ) {

    // Receivin and data and push it in the buffer
    String message = "";
    boolean check = false;
    while ( LoRa.available() ) {
      message += (char) LoRa.read();
      check = true;
    }
    if ( check ) {
      data[counter] = (byte) message.toInt();
      //Serial.println( "data[" + String(counter) + "]: " + String(data[counter]) );
      counter++;
      if ( counter == len-1 ) {
        //Serial.println( "Buffer Ready" );
        check_buffer = true;
        counter = 0;
      }
    }

    
    if ( check_buffer ) {
      Serial.println( "Saving data" );
      check_buffer = false;
      // Save Data To SD
      for ( int i = 0; i < len; i++ ) {
        Serial.println( data[i] );
        analogWrite(speacker_pin, (int) data[i]);
        // SD card apend this data to file
                ////
      }
    }
    
      
  }


  



  /*
  Serial.println( data[counter] );
  analogWrite(speacker_pin, data[counter]);
  */

  
}
