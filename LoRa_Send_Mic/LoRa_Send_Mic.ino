#include <SPI.h>
#include <LoRa.h>


#define dimmer A2


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
  delay(3000);
  Serial.println("LoRa Sender Starts");   
}





int Hz_last = 0;

void loop() {

    
    // Get dimmer frequensy
    int Hz = map( analogRead(dimmer), 0, 1023, 200, 500 ); // 200 to 500 Hz
    long time_loop = 1000000; //  1 second in micros
    long samples_num = (time_loop * Hz)/1000000; // micros == 1 second
    long wait_freq = (time_loop/samples_num)-336;

    // If Frequensy chanched send the value
    if ( (Hz > Hz_last+1) || (Hz < Hz_last-1) ) {
      Hz_last = Hz;
      Serial.println( "Send Frequensy: H" + String(Hz) );
      LoRa.beginPacket();
      LoRa.print("H" + String(Hz));
      LoRa.endPacket();
      delay(1000);
    }

    
    // Loop per 1 second
    long startTime = micros();
    int counter = 0;
    LoRa.beginPacket();
    while ( (micros()-startTime) <= time_loop ) {
      byte val = (byte) analogRead(mic_pin) >> 2;  // 112 micros delay
      analogWrite(speacker_pin, val);
      //LoRa.beginPacket();
      LoRa.print( "&" + String(val) );
      //LoRa.endPacket(true);
      long start = micros();
      while ( (micros()-start) <= wait_freq );
      counter++;
    }
    LoRa.endPacket(true);
    Serial.println( String(counter) + " smaples per second,    " 
                                    + String(Hz) + " Hz,   "  
                                    + String(wait_freq) 
                                    + " micros delay per circle." );
    
    
}
