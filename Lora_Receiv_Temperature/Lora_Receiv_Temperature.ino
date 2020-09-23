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




void setup() { 
  // Setup Serial
  Serial.begin(115200);
  while ( ! Serial );
  // Setup Lora
  while ( ! LoRa.begin(433E6) );  // GREECE: 433,050–434,040 MHz  434,040–434,790 MHz
  LoRa.setSpreadingFactor(7); // 6 - 12
  LoRa.setSignalBandwidth(125E3); // SMALL DATA, BIG RANGE <-- 125, 250, 500 --> BIG DATA, SMALL RANGE
  //LoRa.setTxPower(16);  // Max = 20
  //LoRa.setSyncWord(3); // Big delay 
  LoRa.setCodingRate4(5); // 5,6,7,8
  //LoRa.setPreambleLength(3);
  //LoRa.crc();
  Serial.println("LoRa Receiver Starts");
}




void loop() {

  // try to parse packet
  int packetSize = LoRa.parsePacket();
  
  if (packetSize) {
    // Read packet
    String data = "";
    
    while ( LoRa.available() ) {
      data += (char) LoRa.read();
    }

    Serial.println(data + "°");

    /*
    // Print RSSI of packet
    Serial.print("With RSSI: ");
    Serial.println(LoRa.packetRssi());
    Serial.println();
    */
  }
  
}
