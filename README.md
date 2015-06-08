# Connect Arduino Due + Pololu AltIMU-10 v4 to Unity3D over WIFI directly through TCP Socket IO. #

----------

This is how I obtain Euler angles from a AltiIMU-10 v4 inside the Unity3D game engine on any platform that has a WIFI connection or access to a wireless router using a TCP stream socket IO connection.

**Components:**

- 	Arduino Due
- 	Pololu AltIMU-10 v4
- 	ESP8266 WIFI Serial Transceiver version ESP-01
- 	Two pull-up resistors for i2c (1k Ohm to 4.7k Ohm. Recommend higher resistance)

----------

# Arduino Sketch for reading AltIMU-10 v4 Values #

First I used a modified version of pololu's MinIMU-9 + Arduino AHRS application's Arduino Sketch to calculate estimated roll, pitch and yaw angles from raw sensor readings through the Arduino's I2C pins. The modified code is available in this GitHub. The original code is based on work of Jordi Munoz, William Premerlani, Jose Julio, and Doug Weibel and is available at: [https://github.com/pololu/minimu-9-ahrs-arduino](https://github.com/pololu/minimu-9-ahrs-arduino).

I then modified it to be able to use the ESP8266 Wifi serial transeiver to send the pitch, roll and yaw values over a TCP stream through the WIFI sketch. The WIFI sketch will automatically start sending values over the TCP stream when a connection to a remote device over the TCP stream is obtained. A custom initialiser code for the WIFI sketch can be set using the CustomPrefsWIFI sketch (Two options provided by default. One for connecting to a router and the other for using the ESP8266 as a hotspot). This allows you to define your own connection setups. Please note that the WIFI Access Point Name and password reference can be found in AltIMU-AHRS. If you wish to manually configure the module, set the value of the "WifiChoice" constant to an unused number. This will allow for manual input of commands through a serial monitor using the Arduino Due's USB serial port. Commands are relayed to the ESP8266 automatically if they do not match the commands available by sending the command "wifi help". **Note** commands are case sensitive.

When configuring the ESP8266 baud rate, use the baud rate commands provided (see "wifi help" command). Please note that the WIFI_BAUD value in the AltIMU-AHRS sketch must be changed to match the updated speed for the next time the program is restarted. This is because the Due doesn't support saving values at runtime. The ESP8266 will save the updated baud rate and will not work unless the Arduino has the matching baud rate.

The modified code is dependent on the following libraries which are provided by Pololu.

- 	L3G:	[https://github.com/pololu/L3G](https://github.com/pololu/L3G)
- 	LSM303: [https://github.com/pololu/LSM303](https://github.com/pololu/LSM303)

The unity code is a standard C# TCP socket connection with data streams. See comments inside the code if you get lost. For some reason I could only find the MSDN reference information in German.
[https://msdn.microsoft.com/de-de/bb979228(de-de)](https://msdn.microsoft.com/de-de/bb979228(de-de))

# Wires #

## AltiIMU ##

![](https://a.pololu-files.com/picture/0J5507.200.jpg?a8bb72ea91434199548bc046156e97e6)

**Arduino Due to AltIMU-10 v4:** The AltIMU is built to run with 3.3 volts to 5 volts on all pins except VDD.


	Arduino ->    AltIMU	
	 3.3V   ->     VIN
	 GND    ->     GND
	 SDA    ->     SDA
	 SCL    ->     SCL

Please note that i2c connections require pull-up resistors. Please add a 1k - 4.7k Ohm resistor from 3.3V to the SDA and SCL lines.

## ESP8266 ##

See Pin-Out layout below for more details on the ESP8266 pins.

	Arduino  ->    ESP8266
	3.3V	 ->	   VCC
	GND	 	 ->    GND
	TX1		 ->	   RXD
	RX1	     ->	   TXD
	30		 ->	   RST		(Digital pin configurable in AltIMU-AHRS)
	31  	 ->	   CH_PD	(Digital pin configurable in AltIMU-AHRS)

Please note that the ESP8266 can't handle 5V as the circuite does not have logic level conerters. It can only handle 3.3V on all pins.

----------

# ESP8266 Pin-out layout #

The ESP8266 is a tough little general purpose wireless serial transceiver. With a large range for its size.

![](http://playground.boxtec.ch/lib/exe/fetch.php/wireless/esp8266-pinout_etch_copper_top.png?cache=)

	VCC		3.3V (max 3.6V) Versorgungsspannung
	GND 	Ground
	TXD		Transmit Data (3.3V Level)
	RXD		Receive Data (3.3V Level)
	CH_PD	Chip Power down: (LOW = Power down active)
	GPIO0	General Purpose I/O 0
	GPIO2	General Purpose I/O 2
	RST		Reset (LOW = Reset active)

**Important Notes** that all pin-outs on the ESP8266 are only 3.3 volt tolerant. Any more than this will damage the circuit. The ESP8266 uses the following mA quantities at 3.3V during operation.

	Mode:												Consumption:
	Transmit 802.11b, CCK 1Mbps, POUT=+19.5dBm			215 mA
	Transmit 802.11b, CCK 11Mbps, POUT=+18.5dBm			197 mA
	Transmit 802.11g, OFDM 54Mbps, POUT =+16dBm			145 mA
	Transmit 802.11n, MCS7, POUT=+14dBm					135 mA
	Receive 802.11b, packet length=1024 byte, -80dBm	60 mA
	Receive 802.11g, packet length=1024 byte, -70dBm	60 mA
	Receive 802.11n, packet length=1024 byte, -65dBm	62 mA
	Standby												0.9 mA
	Deep sleep											10 uA
	Power save mode DTIM 1								1.2 mA
	Power save mode DTIM 3								0.86 mA
	Total shutdown										0.5 uA

For more details on the ESP8266 including the source of the above data see:

- [http://wiki.iteadstudio.com/ESP8266_Serial_WIFI_Module](http://wiki.iteadstudio.com/ESP8266_Serial_WIFI_Module)
- [https://nurdspace.nl/ESP8266](https://nurdspace.nl/ESP8266)
- [https://www.pololu.com/product/2470/resources](https://www.pololu.com/product/2470/resources)
- [http://playground.boxtec.ch/doku.php/wireless/esp8266](http://playground.boxtec.ch/doku.php/wireless/esp8266)



This repository is a student project by Nicholas Roodt. Student number 120008.