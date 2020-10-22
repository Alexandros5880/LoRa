#include <SPI.h>
#include <LoRa.h>


//#define ADC_T
//#defined BUFFER

#define speacker A2
#define mic A0


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




const int buf_len = 250;
uint8_t buf[buf_len];
int pointer = 0;



void clear_buff() {
  for (int i = 0; i < sizeof(buf); i++) {
    buf[i] = 0;
  }
}


void setup() {
  // Setup Serial
  Serial.begin(115200);
  while ( ! Serial );
  // Setip Pins
  pinMode(mic, INPUT);
  pinMode(speacker, OUTPUT);
  // SETUP ADC CLOCK
  #if defined(ADC_T)
    cli();//disable interrupts
    //clear ADCSRA and ADCSRB registers
    ADCSRA = 0;
    ADCSRB = 0;
    ADMUX |= (1 << REFS0); //set reference voltage
    ADMUX |= (1 << ADLAR); //left align the ADC value- so we can read highest 8 bits from ADCH register only
    ADCSRA |= (1 << ADPS2) | (1 << ADPS0); //set ADC clock with 32 prescaler- 16mHz/32=500kHz
    ADCSRA |= (1 << ADIE); //enable interrupts when measurement complete
    ADCSRA |= (1 << ADATE); //enabble auto trigger
    ADCSRA |= (1 << ADEN); //enable ADC
    ADCSRA |= (1 << ADSC); //start ADC measurements
    sei();//enable interrupts
  #endif
  // SETUP LORA
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


#if defined(ADC_T)
  // when new ADC value ready
  ISR(ADC_vect) {
    long start = micros();
    if ( pointer == buf_len-1 ) {
      // LoRa Send
      LoRa.beginPacket();
      LoRa.write(buf, sizeof(buf));
      LoRa.endPacket(true);
      pointer = 0;
    }
    buf[pointer] = (uint8_t) ADCH;
    analogWrite(speacker, (int) buf[pointer]);
    Serial.println( "Val: " + String((int) buf[pointer]) + "  " + String( 1000000/(micros()-start) ) + " hz\n" );
    pointer++;
  }
#endif





void loop() {

  #if ! defined(ADC_T) and defined(BUFFER)
    long start = micros();
    if ( pointer == buf_len-1 ) {
      // LoRa Send
      LoRa.beginPacket();
      LoRa.write(buff, sizeof(buf));
      LoRa.endPacket(true);
      pointer = 0;
    }
    //buf[pointer] = map(analogRead(mic), 0, 1023, 0, 250);
    dbs[pointer] = (uint8_t) analogRead(mic) >> 2;
    analogWrite(speacker, (int)dbs[pointer]);
    Serial.println((int)dbs[pointer]);
    Serial.println( String( 1000000/(micros()-start) ) + " hz" );
    pointer++;
  #else if ! defined(ADC_T) and ! defined(BUFFER)
    long start = micros();
    int val = (int)analogRead(mic) >> 2;
    analogWrite(speacker, val);
    // LoRa Send
    LoRa.beginPacket();
    LoRa.print(String(val) + "&");
    LoRa.endPacket(true);
    Serial.println( "Val: " + String(val) + "   " + String( 1000000/(micros()-start) ) + " hz" );
  #endif
  
}
