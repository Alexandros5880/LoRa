#include <SPI.h>
#include <LoRa.h>

#define speacker A3
#define mic_pin A0


// LoRa Pins
#define DI00 2
#define PIN_SPI_RST   9
#define PIN_SPI_SS    10
/*
#define PIN_SPI_MOSI  11
#define PIN_SPI_MISO  12
#define PIN_SPI_SCK   13
*/




// Create LoRa Object
long frequency = 433E6;
long bandwidth = 125E3;
int  spreading_fuctor = 7;
int tx_power = 10;
int sync_word = 0;
int coding_rate = 1;
long preamble_length = 0;






void setup() {
  // Setup Serial
  Serial.begin(115200);
  while ( ! Serial );
  // Setip Pins
  pinMode(mic_pin, INPUT);
  pinMode(speacker, OUTPUT);
  // LoRa Setup
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





const int len = 250;
uint8_t buf[len];

void loop() {

  // Loop per hulf of second
  long startTime = micros();
  int counter = 0;
  int i = 0;
  while ( 1 ) {
    int val = (int) analogRead(mic_pin) >> 2;
    Serial.println(val);
    analogWrite(speacker, val);
    buf[i] = (uint8_t) val;
    i++;
    if (i == 250) {
      LoRa.beginPacket();
      LoRa.write(buf,sizeof(buf));
      LoRa.endPacket(true);
      break;
    }
    counter++;
  }
  Serial.println( "millis: " + String( ((micros()-startTime)/1000)*9.4339 ) + "    " + String(counter*9.4339) + " Hz" );

}