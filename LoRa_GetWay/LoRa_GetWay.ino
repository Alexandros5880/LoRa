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

// Functions declirations
void LoRa_sendMessage(String message);
void LoRa_rxMode();
void LoRa_txMode();
void onReceive(int packetSize);
void onTxDone();
boolean runEvery(unsigned long interval);

void setup() {
  Serial.begin(115200);
  while (!Serial);

  //LoRa.setPins(PIN_SPI_SS, PIN_SPI_RST, DI00);
  while ( ! LoRa.begin(433E6) );  // GREECE: 433,050–434,040 MHz  434,040–434,790 MHz

  /*
  LoRa.setSpreadingFactor(7); // 6 - 12
  LoRa.setSignalBandwidth(125E3); // 125, 250, 500
  LoRa.setTxPower(16);  // Max 20
  //LoRa.setSyncWord(3); // Big delay 
  LoRa.setCodingRate4(5); // 5,6,7,8
  //LoRa.setPreambleLength(3);
  //LoRa.crc();
  */
  
  LoRa.onReceive(onReceive);
  LoRa.onTxDone(onTxDone);
  LoRa_rxMode();
  
  Serial.println("LoRa GETWAY Starts");
}

void loop() {

  if ( runEvery(50) ) { 
    int packetSize = LoRa.parsePacket();
    onReceive(packetSize);
  }
  
}

// Enable tx mode and send the message
void LoRa_sendMessage(String message) {
  Serial.println("LoRa sends: " + message);
  LoRa_txMode();                        // set tx mode
  LoRa.beginPacket();                   // start packet
  LoRa.print(message);                  // add payload
  LoRa.endPacket(true);                 // finish packet and send it
}

// Enable receive mode, get and send the packet
void onReceive(int packetSize) {
  if ( packetSize ) {
    String message = "";
    while ( LoRa.available() ) {
      message += (char)LoRa.read();
    }
    Serial.print("Gateway Receive: ");
    Serial.println(message);
    LoRa_sendMessage(message);
  }
}

void LoRa_rxMode() {
  LoRa.disableInvertIQ();               // normal mode
  LoRa.receive();                       // set receive mode
}

void LoRa_txMode() {
  LoRa.idle();                          // set standby mode
  LoRa.enableInvertIQ();                // active invert I and Q signals
}

void onTxDone() {
  Serial.println("TxDone");
  LoRa_rxMode();
}

// Async delay function
boolean runEvery(unsigned long interval) {
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    return true;
  }
  return false;
}
