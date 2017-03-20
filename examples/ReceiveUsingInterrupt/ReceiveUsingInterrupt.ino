// LoRaFi receiving data using interrupt

#include <LoRaFi.h>

  //creat object to call functions of LoRaFi library
  LoRaFi LoRaFi;

  const int messageLength = 11;
  char message[messageLength];


void setup() {
  
  //initialize serial communication
  Serial.begin(9600);
  delay(100);

  //initialize LoRa module
  LoRaFi.begin();
  delay(100);

  // activate the interrupt on LoRaFi receiving
  LoRaFi.ReceivingInterrupt(receiveMessage);

}

void loop() {

  //Just waiting the interrupt and do nothing
  delay(1000);

}

//callback function to receve the temperature and humidity using interrupt routine
void receiveMessage()
{
  Serial.print("Received Message: ");

  //Receive message
  LoRaFi.ReceivePackage(message,messageLength);

  //Print received message
  int i;
  for(i=0; i<messageLength; i++)
  {
  Serial.print(message[i]);
  }
  Serial.println();
}

