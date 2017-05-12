# LoRaFi API

## Table of Contents

#### [Include The Library](#include-the-library-1)
#### [Setup](#setup-1)
#### [Modes](#modes-1)
#### [Signal Quality](#signal-quality-1)
#### [Configuration](#configuration-1)
#### [Sending Data](#sending-data-1)
#### [Receiving Data](#receiving-data-1)
#### [Interrupt](#interrupt-1)
#### [Other Functions](#other-functions-1)


---------------------------------------------------------------------------------------------------

## Include The Library
```C++
 #include <LoRaFi.h>
 ```
 
 
## Setup
  
### Begin
 
Initialize the library with LoRa transceiver pins numbers 
 
 ```C++
 LoRaFi.begin(cs, rst, rx_sw, tx_sw, dio0);
 ```
 
- cs is the chip select pin number of LoRa transceiver 
- rst is the reset pin number of LoRa transceiver 
- rx_sw is the receiver switch pin number of LoRa transceiver
- tx_sw is the transmitter switch pin number of LoRa transceiver
- dio0 is the dio0 (digital input/output no. 0) pin number of LoRa transceiver

> Note 1: The pins numbers is the Arduino pins numbers that connected to the LoRa transceiver

> Note 2: If you use the LoRaFi board you do not need to pass any thing and the ```begin()``` function will be written as follows:

```C++
LoRaFi.begin();
```
### End

Stop the library and set LoRa transceiver to sleep mode

```C++
LoRaFi.end();
```
 
### Reset

Restore the defaults (*restore factory defaults*)
```C++
LoRaFi.reset();
```
-------------------------------------------------------
## Modes

### Sleep

Set LoRa transceiver to sleep mode
```C++
LoRaFi.sleep();
```

### Idle

Set LoRa transceiver to Standby mode

```C++
LoRaFi.idle();
```
### Mode

Set the previous modes and other modes if required

```C++
LoRaFi.Mode(mode);
```
The available modes:
- ```SLEEP```         : sleep mode
- ```LORA```          : lomg range mode
- ```STANDBY```       : standby or idle mode
- ```TX```            : transmitting mode
- ```RX```            : single byte receiving mode
- ```RX_CONTINUOUS``` : continuous receiving mode

> Note: FSK/OOK modes are not supported in this library

-------------------------------------------------------------
## Signal Quality

### RSSI

Get the RSSI (Recieved Signal Strength Indicator) value

```C++
int rssiValue = LoRaFi.Rssi();
```
This function return the value of RSSI in ``` int ``` data type

### Packet RSSI

Get the RSSI of the last received packet

```C++
int packetRssiValue = LoRaFi.PacketRssi();
```
This function return the value of RSSI in ``` int ``` data type

### SNR

Get the SNR (Signal-to-noise ratio) value
```C++
float snrValue = LoRaFi.SNR();
```
This function return the value of SNR in ``` float ``` data type

------------------------------------------------------------------------------

## Configuration

Configure the LoRa transceiver

> Note: If the configuration functions were called and the user didn't pass any value then the default value of the function will be set.


### Power

Set transmitting power
```C++
LoRaFi.TXpower(power);
```
The support power vlues:
- ``` NORMAL_POWER ``` : minimum transmitting power (13dBm) 
- ``` MAX_POWER ```    : maximum transmitting power (20dBm)

> Note: Default power is ``` NORMAL_POWER ```

### Synchronizing Word

Set synchronizing word

```C++
LoRaFi.SyncWord(sw);
```

- Supported values from ``` 0x00 ``` to ``` 0xFF ```

> Note: Default is ``` 0x34 ``` 

### Carrier Frequency

Set carrier frequency 

```C++
LoRaFi.ChannelFrequency(frequency);
```

The frequency in Hz

- Supported values from ``` 860E6 ``` Hz to ``` 1020E6 ``` Hz

> Note: Default value is ``` 915E6 ``` Hz

### Signal Bandwidth

Set Signal bandwidth 

```C++
LoRaFi.Bandwidth(bw);
```

The frequency in Hz

- Supported values are ``` 125E3, 250E3, 500E3 ``` Hz

> Note: Default value is ``` 125E3 ``` Hz

### Spreading Factor

Set spreading factor of LoRa transceiver

```C++
LoRaFi.SpreadingFactor(SF);
```

- Supported values are from ``` 6-12 ``` 

> Note: Default value is ``` 12 ``` 

### Preamble Length

Set preamble length of LoRa transceiver

```C++
LoRaFi.PreambleLength(length);
```

- supported values from ``` 6 ``` to ``` 65535 ``` 

> Note: Default value is ``` 8 ``` 

### Coding Rate

Set coding rate of LoRa transceiver

```C++
LoRaFi.CodingRate(C_Rate);
```

- Supported values from ``` 5 ``` to ``` 8 ``` 

> Note: Default value is ``` 5 ```

### Frequency Hopping

Enable or disable the frequency hopping

```C++
LoRaFi.FrequencyHopping(status);
```

The supported statuses are:

- ``` ON ``` : Enable the frequency hopping

- ``` OFF ``` : Disable the frequency hopping

> Note: Default status is ``` ON ```

### CRC

Enable or disable the cyclic redundancy check (CRC)

```C++
LoRaFi.CRC(status);
```

The supported statuses are:

- ``` ON ``` : Enable the frequency hopping

- ``` OFF ``` : disable the frequency hopping

> Note: Default status is ``` ON ```

### LDR Optimize

Enable or disable the low data rate (LDR) optimize

> Note: Mandated to enable the LDRoptimize when the sperading factor is 11 or 12 with bandwidth = 125 kHz

```C++
LoRaFi.LDRoptimize(status);
```
The supported statuses are:

- ``` ON ``` : Enable the frequency hopping

- ``` OFF ``` : disable the frequency hopping

> Note: Default status is ``` ON ```

--------------------------------------------
## Sending Data

### Send

Send data in many data types

```C++
LoRaFi.Send(data);
```

- Supported data types are ``` const String, const char, char, unsigned char, int, unsigned int, long, unsigned long, double ``` 


To send double or float, it possible to specify the number of digits after floating point, the Send function will look like follows:
```C++
LoRaFi.Send(data, digits);
```

> Note: The default value of "digits" is ``` 2 ```


### Send Package

Send data package

```C++
LoRaFi.SendPackage(*Package, packageLength);
```

- Supported data type is ``` char ``` array

Supported package length from ``` 1 ``` to ``` 255 ``` 

> Note: Default packageLength is ``` 1 ``` 

--------------------------------------------------------------------------

## Receiving Data

### Receive Char

Receiving single char

```C++
char character = LoRaFi.ReceiveChar();
```

This function return the received ``` char ```

### Receive Unsigned Char

Receiving single unsigned char

```C++
unsigned char LoRaFi.ReceiveUchar();
```
This function return the received ``` unsigned char ```


### Receive Integer

Receive integer value

```C++
int intValue = LoRaFi.ReceiveInt();
```
This function return the received ``` int ``` value

### Receive Unsigned Integer

Receive unsigned integer value 

```C++
unsigned int UintValue = LoRaFi.ReceiveUint();
```
This function return the received ``` unsigned int ``` value

### Receive Long

Receive long value

```C++
long longValue = LoRaFi.ReceiveLong();
```

This function return the received ``` long ``` value

### Receive Unsigned Long

Receive unsigned long value

```C++
unsigned long UlongValue = LoRaFi.ReceiveUlong();
```

This function return the received ``` unsigned long ``` value

### Receive Double

Receive double value

```C++
double doubleValue = LoRaFi.ReceiveDouble();
```

This function return the received ``` double ``` value

### Receive Package

Receive char array

```C++
LoRaFi.ReceivePackage(*Package, packageLength);
```

This function uses to receive ``` char ``` array

- Supported data type is ``` char ``` array
- Supported package length is from ``` 1-255 ``` 

## Interrupt 

Receiving data using interrupt instead of waiting for incoming data

### Receiving Interrupt

Set receiving interrupt

```C++
LoRaFi.ReceivingInterrupt(userFunction);
```

The "userFunction" is a callback function, it will be called when the data received 

> Note: This function must take no parameters and return nothing. This function is sometimes referred to as receiving interrupt service routine.

*For more information see the [examples](https://github.com/LoRaFi/LoRaFi/tree/master/examples)*

### Cancel Interrupt

Cancel receiving interrupt

```C++
LoRaFi.CancelInterrupt();
```

This function uses to cancel the interrupt service routine and stop calling the callback function


--------------------------------------------------------------------

## Other Functions

### Write to Register

Write data to specific register

```C++
LoRaFi.Write_Register(address, data);
```

- "address" : is the register address
- data : the data that will be written to the register

> Note: The supported data types for "address" and "data" are ``` uint8_t ``` and ``` unsigned char ```


### Read Register

Read data from specific register

```C++
uint8_t registerData = LoRaFi.Read_Register(address);
OR
unsigned char registerData = LoRaFi.Read_Register(address);
```

- "address" : is the register address

> Note: The supported data types for "address" are ``` uint8_t ``` and ``` unsigned char ``` 

-----------------------------------------------------------------------------------

-----------------------------------------------------------------------------------




