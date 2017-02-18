//LoRaFi Receiver example

#include <LoRaFi.h>

  //creat object to call functions of LoRaFi library
  LoRaFi LoRaFi;

  char message[11];

void setup() {
  
  //initialize serial communication
  Serial.begin(9600);
  delay(100);

  //initialize LoRa module
  LoRaFi.begin();
  delay(100);
}

void loop() {

  Serial.print("received Message: ");
  
  int len = 11;

  //Receive message
  LoRaFi.ReceivePackage(message,len);

  //Print received message
  int i;
  for(i=0; i<len; i++)
  {
  Serial.print(message[i]);
  }
  Serial.println();

  delay(1000);

}

