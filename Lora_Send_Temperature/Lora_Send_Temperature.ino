
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







// Async delay
void _delay(int m);


// Get Theperature
#define pinThemp A1
float getTemp();
float temp = 0.0, last_temp = 0.0;





void setup() {
  // Setup Serial
  Serial.begin(115200);
  pinMode(pinThemp, INPUT);
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
    
  temp = getTemp();

  Serial.println(String(temp) + "°");
  
  LoRa.beginPacket();
  LoRa.print(temp);
  LoRa.endPacket(true);

}









// Real Time Delay
void _delay(int m) {
  unsigned long wait = millis()+m;
  while(millis() < wait);
}


// Get themperature   10 sample per second
float getTemp() {

  int sumples_len = 20;
  float average = 0;
  int wait = 2000;
  double wait_per_sample = wait/sumples_len;
  double currentTime = 0, previusTime = 0;
  
  int pointer = 0;
  
  long startTime = millis();
  while ( millis()-startTime <= wait ) {
    
    long nowTime = millis()-startTime;
    currentTime = millis();
    if ( (currentTime-previusTime) >= wait_per_sample ) {
      previusTime = currentTime;
      float val = analogRead(pinThemp);
      float voltage = val / 1024.0;
      float temp = (voltage - 0.55) * 100;
      average += temp;
      //Serial.println( "Time: " + String(nowTime) + "  Sample[" + String(pointer) + "]: " 
                                //+ String(val) + " Wait per Sample: " + String(wait_per_sample) );
      pointer++;
    }
    
    if (pointer == sumples_len) {
      break;
    }
    
  }

  average /= sumples_len;
  return average;
}
