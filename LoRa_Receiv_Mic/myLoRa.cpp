#include "myLoRa.h"


// Static Vriable
static signed long myLoRa::pointer = 0;


// Constructor
myLoRa::myLoRa( long &frequency ) {
  while ( ! LoRa.begin(frequency) );  // GREECE: 433,050–434,040 MHz  434,040–434,790 MHz
  #if defined(DEBUG)
    Serial.println( "LoRa frequency: " + String(frequency) );
    Serial.println("\nLoRa Starts.\n");
  #endif
}



// Constructor
myLoRa::myLoRa( long &frequency, long &bandwidth, int &spreading_fuctor, 
                int &tx_power, int &sync_word, int &coding_rate, long &preamble_length ) {
  #if defined(DEBUG)
    Serial.println("LoRa Setup\n");
  #endif
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
  #if defined(DEBUG)
    Serial.println( "LoRa frequency: " + String( frequency ) );
    Serial.println( "LoRa bandwidth: " + String( bandwidth ) );
    Serial.println( "LoRa spreading_fuctor: " + String( spreading_fuctor ) );
    Serial.println( "LoRa tx_power: " + String( tx_power ) );
    Serial.println( "LoRa sync_word: " + String( sync_word ) );
    Serial.println( "LoRa coding_rate: " + String( coding_rate ) );
    Serial.println( "LoRa preamble_length: " + String( preamble_length ) );
    //delay(3000);
    Serial.println( "\nLoRa Starts.\n" );
  #endif
}



// Setup Lora
void myLoRa::lora_setup() {
  
}


// Lora Receive
void myLoRa::lora_receiving() {

  int packetSize = LoRa.parsePacket();
  if ( packetSize ) {
    String value = "";
    while ( LoRa.available() ) {
      char c = (char) LoRa.read();
      if (c != '&' ) {
        value += c;
      } else {
        myLoRa::pointer++;
        #if defined(DEBUG)
          Serial.print("Pointer: " + String(myLoRa::pointer) + "  Value: ");
        #endif
        int val = value.toInt();
        Serial.println(val);
        analogWrite(speacker, val);
        value = "";
      }
    } 
  }
  
}



// LoRa send
void myLoRa::lora_send( String val ) {
  LoRa.beginPacket();
  LoRa.print( val );
  LoRa.endPacket(true);
}
