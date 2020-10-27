#include <SPI.h>
#include <LoRa.h>
#include <analogWrite.h>



// LoRa Pins
#define DI00 2
#define PIN_SPI_RST   14
#define PIN_SPI_SS    5

// Specker and microphone Pins
#define MIC_PIN 32
#define SPEACKER_PIN 33


// Create LoRa Object
long frequency = 433E6;
long bandwidth = 125E3;
int  spreading_fuctor = 7;
int tx_power = 10;
int sync_word = 0;
int coding_rate = 1;
long preamble_length = 0;




// Buffer
//#define BUFFER
const int buf_len = 250;
uint8_t buf[buf_len];
int pointer = 0;





void setup() {
  // Setup Serial
  Serial.begin(115200);
  while ( ! Serial );
  // Setup Pins
  pinMode(MIC_PIN, INPUT);
  pinMode(SPEACKER_PIN, OUTPUT);
  // SETUP LORA
  Serial.println("LoRa Sender");
  LoRa.setPins(PIN_SPI_SS, PIN_SPI_RST, DI00);
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
  Serial.println( "LoRa frequency: " + String( frequency ) );
  Serial.println( "LoRa bandwidth: " + String( bandwidth ) );
  Serial.println( "LoRa spreading_fuctor: " + String( spreading_fuctor ) );
  Serial.println( "LoRa tx_power: " + String( tx_power ) );
  Serial.println( "LoRa sync_word: " + String( sync_word ) );
  Serial.println( "LoRa coding_rate: " + String( coding_rate ) );
  Serial.println( "LoRa preamble_length: " + String( preamble_length ) );
  delay(1000);
  Serial.println( "\nLoRa Starts.\n" );
}









void loop() {
  #if defined(BUFFER)
    long start = micros();
    if ( pointer == buf_len-1 ) {
      // LoRa Send
      LoRa.beginPacket();
      LoRa.write(buf, sizeof(buf));
      LoRa.endPacket(true);
      pointer = 0;
    }
    //buf[pointer] = map(analogRead(mic), 0, 1023, 0, 250);
    buf[pointer] = (uint8_t) analogRead(mic) >> 4;
    analogWrite(speacker, (int)buf[pointer]);
    Serial.println((int)buf[pointer]);
    Serial.println( String( 1000000/(micros()-start) ) + " hz" );
    pointer++;
  #else
    long start = micros();
    int val = (int)analogRead(MIC_PIN) >> 4;
    analogWrite(SPEACKER_PIN, val);
    // LoRa Send
    LoRa.beginPacket();
    LoRa.print(String(val) + "&");
    LoRa.endPacket(true);
    Serial.println( "Val: " + String(val) + "   " + String( 1000000/(micros()-start) ) + " hz" );
  #endif
}
