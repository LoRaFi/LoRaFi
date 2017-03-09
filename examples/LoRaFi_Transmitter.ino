/*
 * This example to send the temperature and humidity values from DHT11 sensor to the LoRaFi receiver
 */

#include <LoRaFi.h>

//Include DHT11 temperature/humidity sensor library
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11   
DHT dht(DHTPIN, DHTTYPE);

LoRaFi LoRaFi;

 int h, t;

void setup() {
  
  LoRaFi.begin();
  delay(100);

  //initialize DHT11
  dht.begin();


}

void loop() {
  //read DHT 11
  h = dht.readHumidity();
  t = dht.readTemperature();
//send character '#' to inform the receiver that the next data is humdity
 LoRaFi.Send('#');
 delay(200);

 LoRaFi.Send(h);
 delay(200);

//send character '*' to inform the receiver that the next data is temperature
 LoRaFi.Send('*');
 delay(200);

 LoRaFi.Send(t);
 delay(200);
 

}
