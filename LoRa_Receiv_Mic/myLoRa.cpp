#include "myLoRa.h"




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
void myLoRa::lora_receiving(char buf[len1][len2]) {
  
  String value = "";
  int counter = 0;
  
  int packetSize = LoRa.parsePacket();
  if ( packetSize ) {
    while ( LoRa.available() ) {
      char c = (char) LoRa.read();
      if ( (c != '|') && (c != '&' ) ) {
        value += c;
      } else {
        if (value != "") {
          for (int i = 0; i < len2-1; i++) {
            buf[counter][i] = value[i];
          }
          counter++;
          if (counter == len1-1) {
            break;
          }
        }
        value = "";
      }
    }
    
    /*
    // Print buffer
    for (int i = 0; i < len1; i++) {
      for (int j = 0; j < len2; j++) {
        Serial.print(String(buf[i][j]));
      }
      Serial.println();
    }
    */  
      
  }
  
}



// LoRa send
void myLoRa::lora_send( String val[], int len ) {
  // dividing my array in smaller arrays
  for (int j = 10; j < 50; j += 10) {
    if ( (len % j) == 0 ) {
      int divider = len/j;
      String line = "";
      for (int i = 0; i < len; i++) {
        line += val[i];
        if ( (i % divider) == 0 ) {
          LoRa.beginPacket();
          LoRa.print( line );
          //Serial.println(line);
          LoRa.endPacket(true);
          line = "";
        }
      }
    }
  }
}
