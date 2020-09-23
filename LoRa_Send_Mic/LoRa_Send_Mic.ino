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

const byte mic_pin = 0;  // A0
const int MAX_RESULTS = 20;
volatile int results[MAX_RESULTS];
volatile int resultNumber;

String buff = "";




// ADC complete ISR Function
ISR (ADC_vect) {
  if (resultNumber >= MAX_RESULTS) {
    ADCSRA = 0;  // Turn off ADC
  } else {
    results[resultNumber++] = ADC >> 2;
  }
}
  
EMPTY_INTERRUPT (TIMER1_COMPB_vect);





// Send DATA
long previusTime = 0;
int wait = 150;
//int wait = 5000;

void send_data() {
  
  
  // Play Local  76 kHz
  for (int i = 0; i < MAX_RESULTS; i++) {
    Serial.println( results[i] );
    analogWrite( speacker_pin, results[i] );
  }
  

  /*
  // Send an array with len 20 and in the recever play the array with < 76 kHz >
  if ( (millis() - previusTime) >= wait ) {
    previusTime = millis();
    for (int i = 0; i < MAX_RESULTS; i++) {
      buff += String(results[i]) + "&";
    }
    Serial.println(buff);
    Serial.println("\n\n");
    LoRa.beginPacket();
    LoRa.print( buff );
    LoRa.endPacket(true);
    buff = "";
  }
  */
  
}

  





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

  
  // Over Clocking , ADC Free Running mode
  // Reset Timer 1
  TCCR1A  = 0;
  TCCR1B  = 0;
  TCNT1   = 0;
  TCCR1B  = bit (CS11) | bit (WGM12);  // CTC, prescaler of 8
  TIMSK1  = bit (OCIE1B); 
  OCR1A   = 39;    
  OCR1B   = 39; // 20 uS - sampling frequency 50 kHz  
  ADCSRA  =  bit (ADEN) | bit (ADIE) | bit (ADIF); // turn ADC on, want interrupt on completion
  ADCSRA |= bit (ADPS2);  // Prescaler of 16  ==  ~76 kHz sampling rate
  ADMUX   = bit (REFS0) | ( mic_pin & 7 );
  ADCSRB  = bit (ADTS0) | bit (ADTS2);  // Timer/Counter1 Compare Match B
  ADCSRA |= bit (ADATE);   // turn on automatic triggering
  

    
}










void loop() {

  
  
  while (resultNumber < MAX_RESULTS) {  }

  
  send_data();
  /*
  for (int i = 0; i < MAX_RESULTS; i++) {
    //buff += String(results[i]) + "&";
    Serial.println( results[i] << 2 );
    analogWrite( speacker_pin, results[i] << 2 );
  }
  buff = "";
  */
  
 
  resultNumber = 0; // reset counter
  ADCSRA =  bit (ADEN) | bit (ADIE) | bit (ADIF)| bit (ADPS2) | bit (ADATE); // turn ADC ON
  




  /*
  // Count max frquency
  long t0, t;
  t0 = micros();
  for(int i=0; i<1000; i++) {
    analogRead(A0);
  }
  t = micros()-t0;  // calculate elapsed time

  Serial.print("Time per sample: ");
  Serial.print((float)t/1000);
  Serial.println(" millis");
  Serial.print("Frequency: ");
  Serial.println((float)1000*1000000/t);
  Serial.println();
  delay(2000);
  */
  

}
