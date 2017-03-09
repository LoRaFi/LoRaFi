//LoRaFi WiFi+LoRa example (DHT11)
/*This example to receive the temperature and humidity from LoRaFi transmitter 
 * and show them on the WiFi server. 
 * The IP of the WiFi server is 192.168.4.1/DHT11
Connect your smartphone or PC WiFi to the LoRaFi_Receiver WiFi network 
and enter the mentioned IP address in you internet browser
*/
#include <LoRaFi.h>

//Include DHT11 temperature/humidity sensor library

#define BUFFER_SIZE 512          
char buffer[BUFFER_SIZE];
char aux_str[200];
String str;

//create object to call LoRaFi library
LoRaFi LoRaFi;

char message[40];

unsigned long starttime2;

int h, t;
bool H = false;
bool T = false;

void setup() {
  
  //initialize serial communication
  Serial.begin(9600);
  delay(100);

  //initialize LoRa module
  LoRaFi.begin();
  delay(100);
  
  //Initialize ESP12-E as Access Point
  setupWiFi();

  // activate the interrupt on LoRaFi receiving
  LoRaFi.ReceivingInterrupt(receiveTempHum);
}

void loop() {

  //Convert reading to Strings
  str="Temperature= "+String (int (t))+"*C"+", Humidity= "+String (int (h))+"%";

  //Receive request and create WebServer
  int ch_id, packet_len, lssid = 0, lpass = 0;
  char *pb;  
  if(read_till_eol()) {
    if(strncmp(buffer, "+IPD,", 5)==0) {
      // request: +IPD,ch,len:data
      sscanf(buffer+5, "%d,%d", &ch_id, &packet_len);
      if (packet_len > 0) {
        pb = buffer+5;
        while(*pb!=':') pb++;
        pb++;
        get_data(pb, ch_id);
      }
    }
  } 

  if ((millis() - starttime2) > 15000) {
  //pass Strings to array buffer
  str.toCharArray(message,40);

    //Send message
  //LoRaFi.Send(message);
  starttime2 = millis();
}
delay(10);
}

void get_data(char *pb, int ch_id){
  char *pget;
  if (strncmp(pb, "GET /", 5) == 0) 
  {
    if (strncmp(pb+5, "DHT11", 5) == 0) 
    {   
       serve_homepage(ch_id); 
    }    
  }
}

int8_t sendATcommand(char* ATcommand, char* expected_answer1, unsigned int timeout) {

  uint8_t x = 0,  answer = 0;
  char response[200];
  unsigned long previous;

  memset(response, '\0', 100);    // Initialize the string

  delay(100);

  while ( Serial.available() > 0) Serial.read();   // Clean the input buffer

  Serial.println(ATcommand);  // Send the AT command

  x = 0;
  previous = millis();

  do {
    if (Serial.available() != 0) {
      response[x] = Serial.read();
      x++;
      // check if the desired answer is in the response of the module
      if (strstr(response, expected_answer1) != NULL)
      {
        answer = 1;
      }
    }
  }
  while ((answer == 0) && ((millis() - previous) < timeout));
  return answer;
}

void setupWiFi() {
  
  while (sendATcommand("AT","OK",1000)==0);
  while (sendATcommand("AT+CWMODE=2","OK",1000)==0);
  while (sendATcommand("AT+CWSAP=\"LoRaFi_Receiver\",\"\",1,0","OK",1000)==0);
  while (sendATcommand("AT+RST","OK",3000)==0);
  delay(3000);
  while (sendATcommand("AT+CIPMUX=1","OK",1000)==0);
  while (sendATcommand("AT+CIPSERVER=1,80","OK",1000)==0);
}

void serve_homepage(int ch_id) {
  
  String content="";
  content += "HTTP/1.1 200 OK\r\n";
  content += "Content-Type: text/html\r\n\r\n";
  content += "<!DOCTYPE HTML>\r\n<html>\r\n";
  content += str;
  content += "</html>\n";

  int ss=content.length();
  snprintf(aux_str, sizeof(aux_str), "AT+CIPSEND=%d,%d", ch_id,ss+1);
  sendATcommand(aux_str,"> ",2000);
  content.toCharArray(aux_str,ss+1);
  sendATcommand(aux_str,"SEND OK",2000);
  snprintf(aux_str, sizeof(aux_str), "AT+CIPCLOSE=%d", ch_id);
  sendATcommand(aux_str,"OK",2000);
}

bool read_till_eol() {
  static int i=0;
  if(Serial.available()) {
    buffer[i++]=Serial.read();
    if(i==BUFFER_SIZE)  i=0;
    if(i>1 && buffer[i-2]==13 && buffer[i-1]==10) {
      buffer[i]=0;
      i=0;
      //dbg.print(buffer);
      return true;
    }
  }
  return false;
}



//callback function to receve the temperature and humidity using interrupt routine
void receiveTempHum()
{

  if(H)
  { h = LoRaFi.ReceiveInt();
    H = false;
  }

  if(T)
  { t = LoRaFi.ReceiveInt();
    T = false;
  }

  if(LoRaFi.ReceiveChar() == '#')
  { H = true;}

    if(LoRaFi.ReceiveChar() == '*')
  { T = true;}
}

