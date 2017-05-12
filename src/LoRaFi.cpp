/*
 * LoRaFi.cpp version 1.3.0
 *
 *  Created on: May 13, 2017
 *      Authors: KHUDHUR ABDULLAH ALFARHAN and Dr. Ammar Zakaria
 *      Email: Qudoren@gmail.com , ammarzakaria@unimap.edu.my
 *
 *  This library licensed under GNU General Public License (GPL)
 *  and supported by Centre of Excellence for Advanced Sensor Technology (CEASTech)
 *  https://ceastech.com/
 *  Universiti Malaysia Perlis (UniMAP)
 *  For more information about LoRaFi plase visit the following links
 *  www.lorafi.tk
 *  www.loraficeastech.wixsite.com/lorafi
 *  LoRaFi forum:
 *  www.loraficeastech.wixsite.com/lorafi/forum
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 */

#include "LoRaFi.h"
#include <stdio.h>
#include <Arduino.h>
#include <stdlib.h>



LoRaFi::LoRaFi()
{
}

//initialize LoRa module
void LoRaFi::begin(uint8_t cs, uint8_t rst, uint8_t rx_sw, uint8_t tx_sw, uint8_t dio0)
{
	//Initialize the SPI communication 
 	SPI.begin();
	SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
	delay(100);

	//Store pins numbers in array to be used in other functions 
        LoRaPins[0] = cs;
        LoRaPins[1] = rst;
        LoRaPins[2] = rx_sw;
        LoRaPins[3] = tx_sw;
        LoRaPins[4] = dio0;

	//Set chip select, rest, RX switch and Tx switch pins
	pinMode(cs, OUTPUT);
	pinMode(rst, OUTPUT);
	pinMode(rx_sw, OUTPUT);
	pinMode(tx_sw, OUTPUT); 
	pinMode(dio0, INPUT);

	//Rest LoRa chip
	digitalWrite(rst, HIGH);
	delay(1);
	digitalWrite(rst, LOW);
	delay(10);

	// Set Chip select pin high to stop communication 
	digitalWrite(cs, HIGH);    
	delay(10);

	Mode(LORA);
	delay(10);
	CRC();
	FrequencyHopping();
	SpreadingFactor(12);

delay(100);
}




//Set LoRa Mode (SLEEP, LORA, STANDBY, TX, RX, RX_CONTINUOUS), FSK/OOK not available in this library
void LoRaFi::Mode(uint8_t mode)
{

	if(mode == LORA) 
	{
		Write_Register(0x01, SLEEP);			// enter sleep mood to select other modes
		Write_Register(0x01, LORA);  			//enter user selected mode
		Write_Register(0x01, LORA | STANDBY); 	//Wake up LoRa module
	}
	else { Write_Register(0x01, LORA | mode);}

}




//Sleep Mode
void LoRaFi::sleep(void)
{
	Mode();
}



// Idle mode
void LoRaFi::idle(void)
{
	Mode(STANDBY);
}



//Stop LoRa module
void LoRaFi::end(void)
{
  Mode(SLEEP);

  SPI.end();

  // Set Chip select pin high to stop communication
  digitalWrite(LoRaPins[0], HIGH);     
  digitalWrite(LoRaPins[2], LOW);
  digitalWrite(LoRaPins[3], LOW);

}



//Reset LoRa module, after reset, the setting will return to default and it should be configured
void LoRaFi::reset(void)
{
	//Reset LoRa chip
	digitalWrite(LoRaPins[1], HIGH);
	delay(1);
	digitalWrite(LoRaPins[1], LOW);
	delay(10);
}



//Get RSSI value in dBm
int16_t LoRaFi::Rssi(void)
{
	return -139 + Read_Register(0x1b);
}



// Get the RSSI of the last received packet
int16_t LoRaFi::PacketRssi(void)
{
	if((SNR()) >= 0)
		{
		 return -139 + ((int8_t)Read_Register(0x1A));
		}
	else
		{
		 return -139 + ((int8_t)Read_Register(0x1A)) + SNR();
		}
}



//Get SNR value
float LoRaFi::SNR(void)
{
	return ((int8_t)Read_Register(0x19)) * 0.25;
}




// Set transmitting power ==> NORMAL_POWER = 13dBm ==> MAX_POWER = 20dBm
void LoRaFi::TXpower(uint8_t pow)
{
  //Enter standby mode to set transmitting power
  Mode(STANDBY);
  
  Write_Register(POWER_REGISTER, pow);

}



// set channel frequency
void LoRaFi::ChannelFrequency(long frequency)
{
  Mode(SLEEP);

  uint64_t freq = ((uint64_t)frequency << 19) / 32000000;

  Write_Register(0x06, (uint8_t)(freq >> 16));
  Write_Register(0x07, (uint8_t)(freq >> 8));
  Write_Register(0x08, (uint8_t)(freq >> 0));
  
  Mode(STANDBY);

}



//Set Spreading Factor, the supported Spreading Factor values from 6 to 12, default to 7
void LoRaFi::SpreadingFactor(uint8_t SF)
{ 
    Mode(STANDBY);

//To make the value of SF from 6 to 12
	if(SF < 6) {SF = 6;}
	if(SF > 12) {SF = 12;}

    if (SF == 6) 
    {
    Write_Register(0x31, 0xc5);
    Write_Register(0x37, 0x0c);
    } 
    else {
    Write_Register(0x31, 0xc3);
    Write_Register(0x37, 0x0a);
         }

  Write_Register(0x1E, (Read_Register(0x1E) & 0x0f) | ((SF << 4) & 0xf0));

}




//Set the signal bandwidth of the radio "supported values are 125E3, 250E3, 500E3" default value is 125E3
void LoRaFi::Bandwidth(long bw)
{
  uint8_t set_bw;
  Mode(STANDBY);

  if (bw <= 125E3) 
  { set_bw = 0;}
   
  else if (bw <= 250E3) 
  { set_bw = 1;} 
  
  else { set_bw = 2;}

  Write_Register(0x1d, (Read_Register(0x1d) & 0x0f) | (set_bw << 6));

}



//Set the coding rate of LoRa radio "supported values are from 5 to 8"  Default value is 5
void LoRaFi::CodingRate(uint8_t C_Rate)
{
  Mode(STANDBY);
  
	if (C_Rate < 5) { C_Rate = 5;} 

	else if (C_Rate > 8) { C_Rate = 8;}

  	uint8_t CR = C_Rate - 4;

  	Write_Register(0x1d, (Read_Register(0x1d) & 0xf1) | (CR << 3));
}



//Set the preamble length of LoRa radio " supported values from 6 to 65535 default value is 8
void LoRaFi::PreambleLength(uint16_t length)
{
    Mode(STANDBY);

    if (length < 6) {length = 6;}

    Write_Register(0x20, (uint8_t)(length >> 8));
    Write_Register(0x21, (uint8_t)(length >> 0));

}



//Set the sync word of LoRa radio it is one-byte "Default is 0x34"
void LoRaFi::SyncWord(uint8_t sw)
{
	Mode(STANDBY);

	Write_Register(0x39, sw);
}



// Set the frequency hopping ON/OFF
void LoRaFi::FrequencyHopping(uint8_t FH)
{
	if(FH == ON)
		{
		 Write_Register(0x24, 0x11);
		}
	if(FH == OFF)
		{
		 Write_Register(0x24, 0x00);
		}
}


// Set CRC checksum ON/OFF
void LoRaFi::CRC(uint8_t crc)
{
	if(crc == ON)
		{
		 Write_Register(0x1d, (Read_Register(0x1d) & 0xff) | (0x1 << 1));
		}
	if(crc == OFF)
		{
		 Write_Register(0x1d, (Read_Register(0x1d) & 0xff) | (0x0 << 1));
		}
}




// Set Low Data Rate Optimize ON/OFF
void LoRaFi::LDRoptimize(uint8_t ldr)
{
	if(ldr == ON)
		{
		 Write_Register(0x1d, (Read_Register(0x1d) & 0xff) | (0x1));
		}
	if(ldr == OFF)
		{
		 Write_Register(0x1d, (Read_Register(0x1d) & 0xff) | (0x0));
		}
}




//Set Receiving Interrupt
void LoRaFi::ReceivingInterrupt(void (*userFunction)(void))
{
	if(!interrupt)
	{	
		interrupt = true;	
		SetInterrupt();
		attachInterrupt(digitalPinToInterrupt(LoRaPins[4]), userFunction, RISING);	

	}

}




//Cancel receiving interrupt
void LoRaFi::CancelInterrupt(void)
{
	if(interrupt)
	{
	detachInterrupt(LoRaPins[4]);
	interrupt = false;
	}
}




//set receiving interrupt regesters
void LoRaFi::SetInterrupt()
{

	digitalWrite(LoRaPins[0], HIGH);
	
  	//Switch LoRa to standby
  	Mode(RX_CONTINUOUS);
  
    //Load FIFO tx pointer
    uint8_t value = Read_Register(0x0E);
    Write_Register(0x0D, value);
  
    // Receive Mode
  	digitalWrite(LoRaPins[3], LOW);		//TX_SW = 0;
  	digitalWrite(LoRaPins[2], HIGH);	//RX_SW = 1;
  
    //IRD mask
    Write_Register(0x11, 0x87);

    //clear IRQ
    Write_Register(0x12, 0xFF);
}




//Write data to specific register 
void LoRaFi::Write_Register(uint8_t address, uint8_t data)
{
  	// Set Chip select pin low to start communication with LoRa module
  	digitalWrite(LoRaPins[0], LOW);

  	// Send address with MSB 1 to make it a write command
  	SPI.transfer(address | 0x80);

  	// Send data
  	SPI.transfer(data);

  	// Set CS pin high to stop communication
  	digitalWrite(LoRaPins[0], HIGH);
}



//Read data from specific register
uint8_t LoRaFi::Read_Register(uint8_t address)
{ 
  uint8_t RegData;

  	// Set Chip select pin low to start communication with LoRa module
  	digitalWrite(LoRaPins[0], LOW);

  	// Send register address
  	SPI.transfer(address);

  	// Send 0x00 to receive the answer from LoRa module 
  	RegData = SPI.transfer(0x00);

  	// Set Chip select pin high to stop communication with LoRa module
  	digitalWrite(LoRaPins[0], HIGH);
	
	return RegData;
}





//send package
void LoRaFi::SendPackage(char *Package, uint8_t packageLength)
{
  uint16_t i;
  uint8_t Value;

  	//Switch LoRa to standby mode
  	Mode(STANDBY);
  	
  	//Set payload length
    Write_Register(0x22, packageLength);

  	//Load FIFO tx pointer
  	Value = Read_Register(0x0E);	//get FIFO address pointer value
  	Write_Register(0x0D, Value);	//set SPI interface address pointer in FIFO data buffer.

  	//Transmit Mode
  	digitalWrite(LoRaPins[3], HIGH); //TX_SW = 1;
  	digitalWrite(LoRaPins[2], LOW);  //RX_SW = 0;

  	//IRQ mask
  	Write_Register(0x11, 0x87);

  	//IRQ flag
  	Write_Register(0x12, 0xFF);

  	//Write payload in to the FIFO
  	for (i = 0; i < packageLength; i++)
  	{
   	 Write_Register(0x80, *Package);
   	 delay(100);
   	 Package++;
  	}

  	//Close TX LoRa
  	Write_Register(0x01, 0x82);
  	Write_Register(0x01, 0x83);

  	//Wait until tx done
  	do
  	{
   		Value = Read_Register(0x12);

    	Value = Value & 0x08;

  	} while (Value == 0);

  	delay(1);

	//Check for interrupt
	if(!interrupt)
	{
  	//stop sending and receiving 
  	digitalWrite(TX_SW, LOW);  //TX_SW = 0;
  	digitalWrite(RX_SW, LOW); //RX_SW = 0;
	}
	else 
	{
	 // Receive Mode
  	 digitalWrite(LoRaPins[3], LOW);	//TX_SW = 0;
  	 digitalWrite(LoRaPins[2], HIGH);	//RX_SW = 1;
	}

}




//Receive package
void LoRaFi::ReceivePackage(char *Package, uint8_t packageLength)
{
  uint16_t i;
  uint8_t LoRa_Interrupt;
  uint8_t PackageLocation;
  
  	digitalWrite(LoRaPins[0], HIGH);
	
	//check for interrupt
	if(!interrupt)
	{
  
	SetInterrupt();

    
    while (digitalRead(LoRaPins[4]) == 0)
    {
      delay(1);
    }

	}
  	//Get interrupt register
  	LoRa_Interrupt = Read_Register(0x12);
  
  	//Clear interrupt register
  	Write_Register(0x12, 0x60);

  	//Get package location
  	PackageLocation = Read_Register(0x10);

  	//Set SPI pointer to PackageLocation
  	Write_Register(0x0D, PackageLocation);

  	//Get message
  	for (i = 0; i < packageLength; i++)
  	{
    	*Package = Read_Register(0x00);
    	Package++;
  	}



  	//stop sending and receiving 
  	digitalWrite(TX_SW, LOW);  //TX_SW = 0;
  	digitalWrite(RX_SW, LOW); //RX_SW = 0;
	
	//Check for interrupt
	if(interrupt)
	{ SetInterrupt();}

}





//handling data for send


//handling String
void LoRaFi::Send(const String &s)
{
	SendPackage((char *)s.c_str(), (uint8_t) s.length());   
}



//handling constant char
void LoRaFi::Send(const char package[])
{
	SendPackage((char *) package, strlen(package));
}


//handling char
void LoRaFi::Send(char package)
{
	uint8_t len = sizeof(package);
	SendPackage( &package, len);
}

//handling unsigned char
void LoRaFi::Send(unsigned char package)
{
	uint8_t len = sizeof(package);
	SendPackage((char*) &package, len);
}





//handling int
void LoRaFi::Send(int package)
{
	char buffer[10];
	itoa(package, buffer, 10);
	SendPackage(buffer, 10);
}


//Handling unsigned int
void LoRaFi::Send(unsigned int package)
{
	uint8_t len = sizeof(package);
	char buffer[10];
	utoa(package, buffer, 10);
	SendPackage(buffer, 10);

}



//Handling long
void LoRaFi::Send(long l)
{
	long_conv.union_long = l;
	SendPackage(long_conv.union_char, 4);
}



//Handling unsigned long
void LoRaFi::Send(unsigned long l)
{
	Ulong_conv.union_Ulong = l;
	SendPackage(Ulong_conv.union_char, 4);
}



//Handling double
void LoRaFi::Send(double package, uint8_t digit)
{
	uint8_t len = sizeof(package);

	char convertedChar[20];

	dtostrf(package, 4, digit, convertedChar);

	SendPackage(convertedChar, 20);
	
}



//Habdling received data


//Handling received int
int LoRaFi::ReceiveInt(void)
{
	char receivedChar[10];
	//Get received data in char type
	ReceivePackage(receivedChar,10); 
	
	//convert char data to int
	return atoi(receivedChar);
}


//Handling received unsigned int
unsigned int LoRaFi::ReceiveUint(void)
{
	char receivedUint[10];
	//Get received data in char type
	ReceivePackage(receivedUint,10);
	
	//convert char data to unsigned int and return it 
	return strtoul(receivedUint,NULL,10);
}


//Handling received long
long LoRaFi::ReceiveLong(void)
{
	char receivedLong[4];
	//Get received data in char type
	ReceivePackage(receivedLong,4);

	long_conv.union_char[0] = receivedLong[0];
	long_conv.union_char[1] = receivedLong[1];
	long_conv.union_char[2] = receivedLong[2];
	long_conv.union_char[3] = receivedLong[3];

	return long_conv.union_long;
}



//Handling received unsigned long
unsigned long LoRaFi::ReceiveUlong(void)
{
	char receivedUlong[4];
	//Get received data in char type
	ReceivePackage(receivedUlong,4);

	Ulong_conv.union_char[0] = receivedUlong[0];
	Ulong_conv.union_char[1] = receivedUlong[1];
	Ulong_conv.union_char[2] = receivedUlong[2];
	Ulong_conv.union_char[3] = receivedUlong[3];

	return Ulong_conv.union_Ulong;
}


//Handling received double
double LoRaFi::ReceiveDouble(void)
{
	char receivedDouble[20];
	//Get received data in char type
	ReceivePackage(receivedDouble,20);
	
	return atof(receivedDouble);
}



//Receive char
char LoRaFi::ReceiveChar(void)
{
	char receivedchar[1];

	ReceivePackage(receivedchar,1);
	
	return receivedchar[0];
}



//Receive unsigned char
unsigned char LoRaFi::ReceiveUchar(void)
{
	char receivedUchar[1];

	ReceivePackage(receivedUchar,1);
	
	return (unsigned char) receivedUchar[0];
}















