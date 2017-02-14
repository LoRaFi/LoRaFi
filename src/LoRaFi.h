/*
 * LoRaFi.h version 1.0
 *
 *  Created on: Feb 8, 2017
 *      Authors: KHUDHUR ABDULLAH ALFARHAN and Dr. Ammar Zakaria
 *      Email: Qudoren@gmail.com , ammarzakaria@unimap.edu.my
 *
 *  This library licensed under GNU General Public License (GPL)
 *  and supported by Centre of Excellence for Advanced Sensor Technology (CEASTech)
 *  https://ceastech.com/
 *  Universiti Malaysia Perlis (UniMAP)
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

#ifndef LORAFI_H
#define LORAFI_H

//Include required libraries
#include <Arduino.h>
#include <SPI.h>
#include <string.h>
#include <WString.h>
#include <stdlib.h>
#include<stdio.h>
#include<math.h>

using namespace std;

//Define modes
#define SLEEP			0x00
#define STANDBY			0x01
#define LORA			0x80
#define TX				0x03
#define RX_CONTINUOUS	0x05
#define RX				0x06

//Define Registers
#define POWER_REGISTER	0x09

//Define defult pins
#define CS		10
#define RST		9
#define RX_SW	8
#define TX_SW	7
#define DIO0	3

//Define other variables 
#define DEFAULT_SPREADING_FACTOR	7
#define DEFAULT_BW					125000
#define DEFAULT_PACKAGE_LENGTH		10
#define DEFAULT_FREQUENCY			915000000
#define DEFAUL_C_RATE				5
#define DEFAULT_PREAMBLE_LENGTH		8
#define DEFAULT_SW					0x34
#define MAX_POWER					0x8F
#define NORMAL_POWER				0x0F
#define DEFAULT_DIGIT_NO			2


class LoRaFi
{
   public:
		LoRaFi();
		//~LoRaFi();
		
        void begin(uint8_t cs = CS, uint8_t rst = RST, uint8_t rx_sw = RX_SW, uint8_t tx_sw = TX_SW, uint8_t dio0 = DIO0);
        
		void Write_Register(uint8_t address, uint8_t data);
		
		uint8_t Read_Register(uint8_t address);
		
		void Mode(uint8_t mode = SLEEP);
		
		void TXpower(uint8_t pow = NORMAL_POWER);
		
		void SendPackage(char *Package, uint8_t packageLength = DEFAULT_PACKAGE_LENGTH);
		
		void ReceivePackage(char *Package, uint8_t packageLength = DEFAULT_PACKAGE_LENGTH);
		
		void ChannelFrequency(long frequency = DEFAULT_FREQUENCY);
		
		void end(void);
		
		void SpreadingFactor(uint8_t SF = DEFAULT_SPREADING_FACTOR);
		
		void Bandwidth(long bw = DEFAULT_BW);
		
		void CodingRate(uint8_t C_Rate = DEFAUL_C_RATE);
		
		void PreambleLength(uint16_t length = DEFAULT_PREAMBLE_LENGTH);
		
		void SyncWord(uint8_t sw = DEFAULT_SW);

		void reset(void);

		int16_t Rssi(void);

		float SNR(void);

		void ReceivingInterrupt(void (*userFunction)(void));

		void CancelInterrupt(void);

		//Habdling data for send
		//void Send(const);
		//void Send(const String);
		void Send(const char[]);
		void Send(char);
		void Send(unsigned char);
		void Send(int);
		void Send(unsigned int);
		void Send(long);
		void Send(unsigned long);
		void Send(double package, uint8_t digit = DEFAULT_DIGIT_NO);
		
		//Habdling received data
		int ReceiveInt(void);			//receive int value
		unsigned int ReceiveUint(void);	//receive unsigned int value
		double ReceiveDouble(void);	//receive double value
		char ReceiveChar(void);  // receive char 
	
	
private: 
	uint8_t LoRaPins[5];
	bool interrupt = false;

		void SetInterrupt(void);

	
};




#endif
