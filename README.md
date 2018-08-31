# MiniWX Station
ESP8266 IoT and BME280 sensor for a minimalist weather station

This work started as a fork from F4GOH code, so you can visit his blog (https://hamprojects.wordpress.com/) for infos about
how to compile and configure this firmware trough his menu. This instead is his github repository: https://github.com/f4goh/Weather. I've added some features that, at moment, you can set at compile time using #defines.
Many thanks to Antonio EA1CDV for his support, beta testing and encouragement, you can read a detailed article in spanish language about this project in his blog: http://ea1cdv.blogspot.com/2018/08/estacion-meteorologica-aprs-via-wifi.html
In a next version i'll change the source code to use native ESP8266 ntp calls instead of using NTPTimeESP, but for the moment the source code needs this library, so you have to add it to your environment, https://github.com/SensorsIot/NTPtimeESP
Obviously you need the BME280 Library too: https://github.com/sparkfun/SparkFun_BME280_Arduino_Library

For the moment this project is running without problems over three flavors of ESP8266:
- NodeMCU V0.9  (ESP-12)
- NodeMCU V1.0  (ESP-12E)
- Wemos D1 mini (ESP-12F)

This is a simple schematics to help connect items to the board, the pinout name is still valid for NodeMCU V0.9 and Wemos D1 mini, the "phototransistor" part can be completely omitted, continue reading if you wanna connect something of analogic to the A0 pin ADC.

![fritzing_Image](https://github.com/IU5HKU/MiniWXStation/blob/master/Images/Schematics.jpg)

These instead are the #defines that you can find in the beginning of the sourcecode, with a brief explanation of what
happens if you choose to use them, it's a bit rude as method, but there is place for a config webpage hosted completely by the ESP8266....it's in my todo list, not in near future, i need to better test some other features.

```javascript
//**** How the station is named in your NET
const char* WiFi_hostname = "IU5HKU-13";
```
Change with the name you wish.

```javascript
//**** APRS PASSWORD (use -1 if you are using a CWOP callsign)
//**** APRS COMMENT, you can set this string as you want (max 43 chars)
//**** APRS_PRJ, Telemetry Project Title (max 23 chars)
const char* AprsPassw = "YourAprsNumericalPASS";
const char* APRS_CMNT = "ESP8266+BME280 MiniWX Station";
const char* APRS_PRJ = "MiniWX Station"
```
For a CWOP callsign you need to register, it's free and easy, refer to
instruction at http://wxqa.com/ to obtain one.

```javascript
//**** Comment this for ESP.getVcc() value in telemetry
//**** getVcc function (system_get_vdd33) is only available when A0 pin17 is suspended (floating), 
//**** this function measure the power voltage of VDD3P3 pin 3 and 4 (in the ESP8266 chip)
#define HAVE_BATTERY
```

This is for reading analog values connected to A0, for the proper value of the resistor to put in series
refer to this thread: https://forum.arduino.cc/index.php?topic=445538.0

```javascript
//**** uncomment this for weatherunderground upload,remember to set ID and PASSWORD of your account
#define USE_WUNDER
//* change ID and PASSWORD with yours
char ID [] = "YourWunderID";                      
char PASSWORD [] = "YourWunderPASSW";
```

Major upgrade, sends data to Weahter Underground.

```javascript
//**** show BME280 registers in printbme();
//#define DISPLAY_BME_REGS
```

Uncomment for printout of registers values in serial console.

```javascript
//**** blinking led to show that into the 10 minutes the system is still alive (1" blink)
//**** NOTE: WEMOS D1 Mini doens't have this led, NodeMCU V0.9 & V1.0 have it.
#define BLINK_RED_LED
```

WILL BE ELIMINATED IN BATTERY POWERED VERSION, this is a "i'm still alive" indication.

```javascript
//**** blinking led to show that ESP8266 is transmitting  (0.5" blink)
#define BLINK_BLUE_LED
```

WILL BE ELIMINATED IN BATTERY POWERED VERSION, this is a "i'm sending data to servers" indication.

```javascript
//**** show (annoying) animated clock in the serial output 
//#define SHOW_TICKS
```

Left commented to eliminate the clock in serial console.

```javascript
//**** use static ip instead of dns one
//#define USE_STATIC_IP
//* change to reflect your net configuration
#ifdef USE_STATIC_IP
String stat_ip="192.168.0.200";        // STATIC IP
String stat_gateway="192.168.0.1";     // GATEWAY
String stat_subnet="255.255.225.0";    // SUBNET MASK
String stat_dns1="8.8.8.8";            // DNS1
String stat_dns2="4.4.2.2";            // DNS2
IPAddress ip,gateway,subnet,dns1,dns2;
#endif
```

Self explanatory, this settings are needed if you want a static ip for the station.

**ArduinoIDE SerialMonitor config **

WARNING: some users are experiencing issues in filling the required fields in the provided menù. Due to a lack in the sanity check in the user input routine, is MANDATORY to use only 'newline' as method, doens't use twho chars as EOL marker!

![SerialMonitor_Image](https://github.com/IU5HKU/MiniWXStation/blob/master/Images/Newline.jpg)

This is the correct setting if you are using ArduinoIDE's SerialMonitor.
In a future revision the user input will be checked against errors or different serial settings, but for the moment...


**V0.7f NOTE:**

Thanks to a talk with Antonio EA1CDV we have this minor upgrade to the project, the possibility to have a 43 chars wide comments. As reported at pag.20 in APRS101.pdf draft, every packet can have his comment simply adding it at the tail of the packet, but this rule isn't valid for wx reports packets, so i've used a dirty workaround, i send a position report packet with the same coords of the wx station (this kind of packets can have a 43 chars comment) and just after that i send the "real" weather info packet. As result you have a WX station with the possibility to insert a decent comment.
APRS101.pdf report the possibility to insert the altitude in feet too, but unluckly after sending the wx packet the server delete this parameter, i don't know why, but a WX station can't display its altitude, and this is in the standard, so don't blame me, isn't my fault. To change the comment refer as usual at the beginning of my source code, looking for APRS_CMNT string.
There is also an APRS_PRJ one, this is a separate comment that goes in the telemetry page.

**V0.7e NOTE:**

In this new version i've slightly modified the NTP sync routine that crash the ESP in case of poor signals.
I've tested mine till -90db and seems to be quite stable (seems....), let me know if you encounter some issue.
Now the hardcoded 3.7V value in telemetry has left space to ESP.getVcc() routine, in this way you read the cpu voltage, not so useful, i know, just to fill the telemetry packet if you haven't connected A0 to some sensor.

**ABOUT BME280 sensor:**

Someone ask about the possibility to calibrate this sensor, but honestly i don't think that it needs some sort of compensation, it's factory calibrated and really affordable and precise, and causes of inaccurate readings could be found in bad placement (direct sun, next a wall, etc.etc.) or, as Mr Kandrsmith says "...manufacturers of cheap integrated modules not having followed the Bosch Sensortec soldering profile properly and contaminated or damaged the device. The reconditioning procedure might resolve that....".
You can read his amazing article about BME280, related to temperature calibration, at this link:
https://www.kandrsmith.org/RJS/Misc/Hygrometers/absolutetemperature.html
I've added the datasheet from Bosch to this repository, a really interesting lecture, the accuracy of BME280 is at high levels of precision, great buy for the money.
Anyway in the datasheet is well explained the possibility to read and write the calibration registers used by the sensor to return a correct measure, and the SparkfunBME280Library offer methods to operate with them:

```javascript
//ReadRegisterRegion takes a uint8 array address as input and reads
	//a chunk of memory into that array.
    void readRegisterRegion(uint8_t*, uint8_t, uint8_t );
	//readRegister reads one register
    uint8_t readRegister(uint8_t);
    //Reads two regs, LSByte then MSByte order, and concatenates them
	//Used for two-byte reads
	int16_t readRegisterInt16( uint8_t offset );
	//Writes a byte;
void writeRegister(uint8_t, uint8_t);
```
my advice is to keep unchanged those values, really, the SparkfunBME280 Library already use the calibration registers values to output temperature, relative humidity and pressure with high precision, greater than what is needed for this kind of applications, the functions are the specular copy of what Bosch reccomended in his datasheet.

# lwIP

If you are using ArduinoIDE for development, then you can choose between some linkage options, one of the most important is the "lwIP" one:

![lwIP_Image](https://github.com/IU5HKU/MiniWXStation/blob/master/Images/lwIP.jpg)

lwIP is the "lightweigth TCP/IP stack" (https://savannah.nongnu.org/projects/lwip/) ported to the ESP8266 embedded platform, and is the library that permit the ease of use of the whole internet related system.
Everything seems to be ok with the more advanced port, V2.0 as you can see, if you are experiencing strange behavior you could try to change in favor of the old V1.4.
NOTE: you can set here the serial speed for upload your firmware to ESP8266, i had no problem at all with maximum speed, but if encounter some issue you can lower the value, as someone reported in some forum.
Remember: this isn't the serial speed that you will use later for console serial communications, and the ESP8266 doesn't have problem to manage maximum speed (usually).

HINT: the system save in flash memory the last successful connection parameters, and because we are in a multitasking environment, it try to connect before you send the connect() command, this may resolve in some trouble if your wifi is gone or if you try the system at home and then you change place...you can resolve this problem using the "Sketch+WiFi settings" flashing set (Erasing Flash voice, just under Upload Speed).

![WiFi_Image](https://github.com/IU5HKU/MiniWXStation/blob/master/Images/SketchWifiSets.jpg)

Someone experienced difficulties in connection to home WiFi, in some cases, if you aren't use the same ssid, the above hint can resolve. Someone have commented this call into source code:

```javascript
WiFi.persistent(false); // WiFi config isn't saved in flash
```
but i don't think that can be related in some way to the effective resolution, anyway give it a try, who knows...

# NodeMCU V0.9 schematics

This schematics is useful for reference in hardware connections if you own an ESP-12,
also know as NodeMCU V0.9, refer to this for pinouts.
ESP8266 GPIO's names and NodeMCU pin naming are different, as example GPIO16, who is used
for wakeup the SoC from deepsleep() mode, in ESP-12 is called 'D0'. This pin is connected to the onboard red led too.
When you are using deepsleep() D0 must be connected to 'RST', in this way the RTC who keep tickin' in deepsleep (is the only thing active when you are in this mode) can wakeup the chip when time is expired. Useful, ins't it?:-)

![Schematics_Image](https://github.com/IU5HKU/MiniWXStation/blob/master/Images/NODEMCU_DEVKIT_SCH.png)

# Adding a phototransistor

A cheap way to expand the capability of our poor-man weather station is adding a phototransistor, and for who have tha possibility, to characterize it to obtain a more or less precise value of the lux. I've remembered that wandering around my lab there is an old FPT100, so i decided to connect this analog device to the ESP8266 10bit ADC (Pin A0). This is the simple scheme:

![phototransistor_Image](https://github.com/IU5HKU/MiniWXStation/blob/master/Images/phototransistor.png)

and following the rules explained here, https://forum.arduino.cc/index.php?topic=445538.0 you can read a voltage value proportional to the light who hit the phototransistor.
The resistor determine the sensitivity of the device, who is claimed to be linear in his response, but for a more precise work you MUST use a luxmeter as reference. The following table is from this site: https://www.theremino.com/hardware/inputs/light-sensors where fpt100 is compared against bpw34, if you use 10Kohm the old FPT100 is still the more sensitive, with is 0.5lux of resolution, but i guess that nowadays there are other (and cheaper!) devices who perform surely better. I know, there are an i2c version dedicated for IoT, but hey, i wanna reuse my precious fpt100, i think that it's more or less 25 years old :-)

Resistor Value | Resolution | Full-scale (lux)                      
---------------|------------|-----------------                     
50 Kohm        |    0.1     |   1000                                
10 Kohm        |    0.5     |   5000                                
1 Kohm         |    5       |   50000                               
500 ohm        |    10      |   100000                              

Reference     |   values (lux)
--------------|----------------
Sun at midday |   100000
Surgery table |   10000 
Office        |   from 500 to 2000
Art Gallery   |   from 100 to 500
Full Moon     |   1

So play with the resistor based on where you wanna put the sensor, and adjust the value in the ReadVBAT() function accordly to have a valid full scale value, look:

```c_cpp
unsigned int ReadVBAT (){
  raw = analogRead(A0);
  volt=raw/1023.0;
  //volt=volt*4.2f; //100Kohm resistor
  volt=volt*3.3f;   //10kohm resistor
  return ((unsigned int)(volt*100)); // two decimals after the comma
}
```

NOTE: if you are using 10Kohm you can connect the Vcc to 3.3V to obtain a 0.003V resolution from the internal ESP8266 ADC
