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





// Async Lora Dellay = 1937 micros
// Sync Lora Delay = 376647 micros
// Human voice frequency:
// Male: 85 - 180 Hz
// Femaly: 165 - 255 Hz


// 792 Hz ==> 792 samples per 1000 millis ==> 1000 samples per 1261 millis
int sample_loop_time = 1261; // millis
const int samples_len = 1000;
byte data[samples_len]; // Buffer




/*
 *   !!!!!
 *          In Every Buffer Loop I lossing 38 Hz from LoRa
 *   !!!!!
 */





// Update Lora Data
void updateLoraData();





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







void loop() {

    // In every 1261 millis get 1000 samples
    long startTime = millis();
    int pointer = 0;
    while ( (millis()-startTime) <= sample_loop_time ) {
      // Get Data
      data[pointer] = (byte) analogRead(mic_pin) >> 2; // 110 micros blocking time
      //analogWrite(speacker_pin, (int) data[pointer]);
      //Serial.println( "data[" + String(pointer) + "]: " + data[pointer] );
      pointer++;
      if ( pointer ==  samples_len-1) {
        Serial.println("Pointer stacks");
        break;
      }
    }



    // Update Lora Data
    updateLoraData();

    
    
}









// Update Lora Data
void updateLoraData() {
  analogWrite(speacker_pin, 0);
  for ( int i = 0; i < samples_len; i++ ) {
    Serial.println( "data[" + String(i) + "]: " + data[i] );
    //analogWrite(speacker_pin, (int) data[i]);
    LoRa.beginPacket();
    LoRa.print( data[i] );
    LoRa.endPacket(true);
    long startTime = millis();
    while ( (millis()-startTime) <= 33 );
  }
}
