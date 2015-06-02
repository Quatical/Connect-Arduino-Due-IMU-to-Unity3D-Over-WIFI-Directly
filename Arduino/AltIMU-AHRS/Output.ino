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


void sendOutput()
{    
    if (getSendAccel()  != false) {
        float tmpRoll = ToDeg(roll);
        float tmpYaw = ToDeg(yaw);
        float tmpPitch = ToDeg(pitch);
        String outputMSG = (String)tmpPitch + "," + (String)tmpYaw + "," + (String)tmpRoll;
        Serial.println(outputMSG);
        if (getWifiReady () == true){
          sendMessage (outputMSG);
        }
    }      
}

long convert_to_dec(float x)
{
  return x*10000000;
}

