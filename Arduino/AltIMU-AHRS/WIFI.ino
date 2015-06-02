/*

AltIMU-AHRS
Modified Pololu MinIMU-9 + Arduino AHRS (Attitude and Heading Reference System)

AltIMU-AHRS is based on MinIMU-9-Arduino-AHRS by Kevin-Pololu:
https://github.com/pololu/minimu-9-ahrs-arduino 
http://www.pololu.com/

MinIMU-9-Arduino-AHRS is based on sf9domahrs by Doug Weibel and Jose Julio:
http://code.google.com/p/sf9domahrs/

sf9domahrs is based on ArduIMU v1.5 by Jordi Munoz and William Premerlani, Jose
Julio and Doug Weibel:
http://code.google.com/p/ardu-imu/

AltIMU-AHRS is free software: you can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License as published by the
Free Software Foundation, either version 3 of the License, or (at your option)
any later version.

AltIMU-AHRS is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for
more details.

You should have received a copy of the GNU Lesser General Public License along
with MinIMU-9-Arduino-AHRS. If not, see <http://www.gnu.org/licenses/>.

*/


#define esp Serial1

bool useDefaultInit = false;

String serialMSG = "";
bool testSerialMSG = false;

String wifiMSG = "";
bool testWifiMSG = false;
bool wifiReady = false;
bool wifiStarted = false;
bool sendAccel = false;

bool test_Responce(char responce[]) {
  if (esp.available()) {
    if (esp.find(responce)) {
      return true;  
    } else {
      return false; 
    }
  }
}

void wifi_Init() {  
  Serial.println("Starting Wifi Module");
  digitalWrite(CH_PD, LOW);
  digitalWrite(RST, LOW);
  delay(1000);
  digitalWrite(CH_PD, HIGH);
  digitalWrite(RST, HIGH);
  
  Serial.println("Opening Connection to Wifi Module");
  esp.begin(WIFI_BAUD);
  delay(1000);
  while (esp.available()) {
    if (esp.find("ready")){
      Serial.println("Wifi Module Online");
    }  
  }

  wifiCustomInit ();
  wifiReady = true; 
  wifiStarted = true;
}

void sendCMD(String cmd) {
  esp.println(cmd);
  //esp.flush();
}

void chkSerial0() {
  // Reads the serial into a String buffer
  while (Serial.available()){
    serialMSG += char(Serial.read());
    wifiReady = false;
  } 
  delay(50);
  
  // Tests to see that there is a message when serial is done reading
  if (!Serial.available()){
    if (serialMSG.length() > 0) {
      testSerialMSG = true; 
    }
  }
  
  // Tests the input from serial for commands
  if (testSerialMSG) {
    bool other = true;
    
    if (serialMSG == "wifi baud 9600") {
      sendCMD("AT+CIOBAUD=9600");
      esp.flush();
      esp.end();
      esp.begin(9600);    
      other = false;
    }
    
    if (serialMSG == "wifi baud 57600") {
      sendCMD("AT+CIOBAUD=57600");
      esp.flush();
      esp.end();
      esp.begin(57600);     
      other = false;
    }
    
    if (serialMSG == "wifi baud 115200") {
      sendCMD("AT+CIOBAUD=115200"); 
      esp.flush();
      esp.end();
      esp.begin(115200);    
      other = false;
    }
    
    if (serialMSG == "wifi reset") {
      sendCMD("AT+RST");    
      other = false;
    }
      
    if (serialMSG == "wifi mode 3") {
      sendCMD("AT+CWMODE=3");
      other = false;
    }
      
    if (serialMSG == "wifi mode 2") {
      sendCMD("AT+CWMODE=2");
      other = false;
    }
      
    if (serialMSG == "wifi mode 1") {
      sendCMD("AT+CWMODE=1");
      other = false;
    }
      
    if (serialMSG == "wifi quit AP") {
      sendCMD("AT+CWQAP");
      other = false;
    }
      		
    if (serialMSG == "wifi list AP") {
      sendCMD("AT+CWLAP");
      other = false;
    }
      
    if (serialMSG == "wifi status") {
      sendCMD("AT+CIPSTATUS");
      other = false;
    }
      
    if (serialMSG == "wifi server stop") {
      sendCMD("AT+CIPSERVER=0");
      other = false;
    }
      
    if (serialMSG == "wifi server start") {
      sendCMD("AT+CIPMUX=1");
      delay(1000);
      sendCMD("AT+CIPSERVER=1," + port);
      other = false;
    }
      		
    if (serialMSG == "wifi multi true") {
      sendCMD("AT+CIPMUX=1");
      other = false;
    }
      		
    if (serialMSG == "wifi multi false") {
      sendCMD("AT+CIPMUX=0");
      other = false;
    }
    
    if (serialMSG == "wifi IP") {
      sendCMD("AT+CIFSR");
      other = false; 
    }
    
    if (serialMSG == "wifi echo off") {
      sendCMD("ATE0");
      other = false; 
    }
    
    if (serialMSG == "wifi echo on") {
      sendCMD("ATE1");
      other = false; 
    }
    
    
    if (serialMSG == "wifi setup AP") {
      sendCMD("AT+CWSAP=\"" + myAPName + "\",\"" + myAPPassword + "\"," + (String)APChannel + ",3");
      other = false; 
    }
    
    if (serialMSG == "wifi join AP") {
      sendCMD("AT+CWQAP");
      delay(1000);
      sendCMD("AT+CWJAP=\"" + APName + "\",\"" + APPassword + "\"");
      other = false;
    }
    
    if (serialMSG == "wifi ssid") {
      Serial.println("SSID " + myAPName + " PASSWORD " + myAPPassword);
      other = false;  
    }
    
    if (serialMSG == "wifi shutdown") {
      sendCMD("AT+CIPSERVER=0");
      delay(500);
      sendCMD("AT+CIPMUX=0");
      delay(500);
      sendCMD("AT+CWQAP");
      delay(500);
      Serial.println("Done closing connections");
      
      other = false;  
    }
    
    if (serialMSG == "help") {
      Serial.println("List of commands:");
      Serial.println("wifi reset");
      Serial.println("wifi status");
      Serial.println("wifi IP");
      Serial.println("wifi ssid");
      Serial.println("wifi list AP");
      Serial.println("wifi join AP");
      Serial.println("wifi quit AP");
      Serial.println("wifi setup AP");
      Serial.println("wifi mode 1");
      Serial.println("wifi mode 2");
      Serial.println("wifi mode 3");
      Serial.println("wifi multi true");
      Serial.println("wifi multi false");
      Serial.println("wifi server start");
      Serial.println("wifi server stop");
      Serial.println("wifi shutdown");
      Serial.println("wifi baud 9600");
      Serial.println("wifi baud 57600");
      Serial.println("wifi baud 115200");
      Serial.println("wifi echo off");
      Serial.println("wifi echo on");
      Serial.println("any text other than this should be automatically sent to the ESP8266 wifi. Use this for AT commands");
      other = false;  
    }
    
    if (serialMSG == "Gyro start") {
      sendAccel = true;
      other = false;  
    }
    
    if (serialMSG == "Gyro stop") {
      sendAccel = false;
      other = false;  
    }
      
    if (other != false) {
      if (serialMSG != "") {
        sendCMD(serialMSG);
      }
    }
    testSerialMSG = false;
    serialMSG = ""; 
    wifiReady = true;
  }
}

bool getWifiReady () {
  if (wifiReady != false && wifiStarted != false) {
    return true;
  } else {
    return false; 
  }
}

bool getSendAccel () {
 if (getWifiReady() != false && sendAccel != false) {
  return true;
 } else {
  return false; 
 }
}

void sendMessage (String msg) {
  wifiReady = false;
  String tmp = msg + "\n";
  //Serial.print(tmp);
  int DataLength = tmp.length();
  sendCMD("AT+CIPSEND=0," + String(DataLength));
  while (test_Responce(">") != true) {
    delay(5);
  }
  esp.print(tmp);
  esp.flush();
  while (test_Responce("SEND OK") != true) {
    delay(5);
  }
  wifiReady = true;
}

void chkesp() {
  if (wifiStarted == true) {
    while (esp.available()) {
      wifiMSG += char(Serial1.read());
    }
    delay(50);
    
    if (!esp.available()) {     
      if (wifiMSG != "") {
        Serial.println(wifiMSG);
        wifiMSG = wifiMSG.substring(0,4);
        if (wifiMSG == "Link") {
          Serial.println("Activating Gyro Print");
          sendAccel = true; 
      }
      
      if (wifiMSG == "Unli") {
        Serial.println("Deactivating Gyro Print");
        sendAccel = false; 
      }
      
      if (wifiMSG == ">\nw") {
        Serial.println("Error! Deactivating Gyro Print");
        sendAccel = false; 
      }
      
      wifiMSG = "";
    }  
  }
  }
}

void wifi_Loop() {
  chkSerial0();
  chkesp();  
}

