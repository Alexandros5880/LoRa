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










void loop() {

  // Get dimmer frequensy
  int Hz = 700;//map( analogRead(dimmer), 0, 1023, 200, 700 ); // 200 to 700 Hz
  long time_loop = 1000000; //  1 second in micros
  long samples_num = (time_loop * Hz)/1000000; // micros == 1 second
  long wait_freq = (time_loop/samples_num)-223;
  // buffer
  int buff[Hz];


  // Get data from Lora
  int packetSize = LoRa.parsePacket();
  if ( packetSize ) {
    
    // Receiving data
    String message = "";
    int counter = 0;
    while ( LoRa.available() ) {
      int value = String(LoRa.read()).toInt();
      Serial.println( value );
      buff[counter] = value;
      if (counter < Hz) {
        counter++;
      } else {
        break;
      }
    }

    
    // Play Data
    int i = 0;
    while ( i < Hz ) {
      //Serial.println( buff[i] );
      //analogWrite( speacker_pin, buff[i] );
      buff[i] = 0;
      i++;
      long start = micros();
      while ( (micros()-start) <= wait_freq );
    }
    
 
  }

  
}
