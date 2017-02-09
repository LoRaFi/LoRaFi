//LoRaFi transmitter example
#include <LoRaFi.h>

LoRaFi LoRaFi;

void setup() {
  //initialize serial communication
  Serial.begin(9600);
  delay(100);

  //initialize LoRa module
  LoRaFi.begin();
  delay(100);

}

void loop() {

  Serial.println("Send Message");

  //Send message
  LoRaFi.Send("Hello World");
  
  delay(1000);

}
