#include "myLoRa.h"




// Constructor
myLoRa::myLoRa( long &frequency ) {
  while ( ! LoRa.begin(frequency) );  // GREECE: 433,050–434,040 MHz  434,040–434,790 MHz
  Serial.println( "LoRa frequency: " + String(frequency) );
  Serial.println("LoRa Starts");
}



// Constructor
myLoRa::myLoRa( long &frequency, long &bandwidth, int &spreading_fuctor, 
                int &tx_power, int &sync_word, int &coding_rate, long &preamble_length ) {
  while ( ! LoRa.begin(frequency) );  // GREECE: 433,050–434,040 MHz  434,040–434,790 MHz
  if ( bandwidth != 0 ) {
    LoRa.setSignalBandwidth(bandwidth);
  }
  if ( spreading_fuctor > 5 ) {
    LoRa.setSpreadingFactor(spreading_fuctor); // 6 - 12
  }
  if ( tx_power != 0) {
    LoRa.setTxPower(tx_power);  // Max 20
  }
  if ( sync_word != 0) {
    LoRa.setSyncWord(sync_word); // Big delay
  }
  if ( coding_rate  > 4) {
    LoRa.setCodingRate4(coding_rate); // 5,6,7,8
  }
  if ( preamble_length != 0) {
    LoRa.setPreambleLength(preamble_length);
  }
  Serial.println( "LoRa frequency: " + String( frequency ));
  Serial.println( "LoRa bandwidth: " + String( bandwidth ));
  Serial.println( "LoRa spreading_fuctor: " + String( spreading_fuctor ));
  Serial.println( "LoRa tx_power: " + String( tx_power ));
  Serial.println( "LoRa sync_word: " + String( sync_word ));
  Serial.println( "LoRa coding_rate: " + String( coding_rate ));
  Serial.println( "LoRa preamble_length: " + String( preamble_length ));
  //delay(3000);
  Serial.println( "LoRa Starts.\n\n" );
}



// Setup Lora
void myLoRa::lora_setup() {
  
}


// Lora Receive
String myLoRa::lora_receiving() {
  String value = "";
  int packetSize = LoRa.parsePacket();
  //Serial.println( "Package: " + String(packetSize) );
  if ( packetSize ) {
    while ( LoRa.available() ) {
      value += (char) LoRa.read();
    }
  }
  return value;
}



// LoRa send
void myLoRa::lora_send( String val ) {
  LoRa.beginPacket();
  LoRa.print( val );
  LoRa.endPacket(true);
}