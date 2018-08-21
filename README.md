# MiniWX Station
ESP8266 IoT and BME280 sensor for a minimalistic weather station to put in your terrace :-)

This work started as a fork from F4GOH code, so you can visit his blog (https://hamprojects.wordpress.com/) for infos about
how to compile and configure this firmware trough his menu. This instead is his github repository: https://github.com/f4goh/Weather. I've added some features that, at moment, you can set at compile time using #defines.
Many thanks to Antonio EA1CDV for his support, beta testing and encouragement, you can read a detailed article in spanish language about this project in his blog: http://ea1cdv.blogspot.com/2018/08/estacion-meteorologica-aprs-via-wifi.html
In a next version i'll change the source code to use native ESP8266 ntp calls instead of using NTPTimeESP, but for the moment the source code needs this library, so you have to add it to your environment, https://github.com/SensorsIot/NTPtimeESP
Obviously you need the BME280 Library too: https://github.com/sparkfun/SparkFun_BME280_Arduino_Library

For the moment this project is running without problems over three flavours of ESP8266:
- NodeMCU V0.9  (ESP-12)
- NodeMCU V1.0  (ESP-12E)
- Wemos D1 mini (ESP-12F)

```javascript
//**** How the station is named in your NET
const char* WiFi_hostname = "IU5HKU-13";
```
Change with the name you wish.

```javascript
//**** APRS PASSWORD (use -1 if you are using a CWOP callsign)
const char* AprsPassw = "YourAprsNumericalPASS";
```
For a CWOP callsign you need to register, it's free and easy, refer to
instruction at http://wxqa.com/ to obtain one.

```javascript
//**** comment this for hardcoded 3.7V value in telemetry
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

# lwIP

If you are using ArduinoIDE for development, then you can choose between some linkage options, one of the most important is the "lwIP" one:

![lwIP_Image](https://github.com/IU5HKU/MiniWXStation/blob/master/lwIP.jpg)

lwIP is the "lightweigth TCP/IP stack" (https://savannah.nongnu.org/projects/lwip/) ported to the ESP8266 embedded platform, and is the library that permit the ease of use of the whole internet related system.
Everything seems to be ok with the more advanced port, V2.0 as you can see, if you are experiencing strange behaviour you could try to change in favour of the old V1.4.
NOTE: you can set here the serial speed for upload your firmware to ESP8266, i had no problem at all with maximum speed, but if encounter some issue you can lower the value, as someone reported in some forum.
Remember: this isn't the serial speed that you will use later for console serial communications, and the ESP8266 doesn't have problem to manage maximum speed (usually).

# NodeMCU V0.9 schematics

This schematics is useful for reference in hardware connections if you own an ESP-12,
also know as NodeMCU V0.9, refer to this for pinouts.
ESP8266 GPIO's names and NodeMCU pin naming are different, as example GPIO16, who is used
for wakeup the SoC from deepsleep() mode, in ESP-12 is called 'D0'. This pin is connected to the onboard red led too.
When you are using deepsleep() D0 must be connectd to 'RST', in this way the RTC who keep tickin' in deepsleep (is the only thing active when you are in this mode) can wakeup the chip when time is expired. Useful, ins't it?:-)

![Schematics_Image](https://github.com/IU5HKU/MiniWXStation/blob/master/NODEMCU_DEVKIT_SCH.png)

# Adding a phototransistor

A cheap way to expand the capability of our poor-man weather station is adding a phototransistor, and for who have tha possibility, to characterize it to obtain a more or less precise value of the lux. I've remembered that wandering around my lab there is an old FPT100, so i decided to connect this analog device to the ESP8266 10bit ADC (Pin A0). This is the simple scheme:

![phototransistor_Image](https://github.com/IU5HKU/MiniWXStation/blob/master/phototransistor.png)

and following the rules explained here, https://forum.arduino.cc/index.php?topic=445538.0 you can read a voltage value proportional to the light who hit the phototransistor.
The resistor determine the sensitivity of the device, who is claimed to be linear in his response, but for a more precise work you MUST use a luxmeter as reference. The following table is from this site: https://www.theremino.com/hardware/inputs/light-sensors where fpt100 is compared against bpw34, if you use 10Kohm the old FPT100 is still the more sensitive, with is 0.5lux of resolution, but i guess that nowaday there are other (and cheaper!) devices who perform surely better. I know, there are an i2c version dedicated for IoT, but hey, i wanna reuse my precious fpt100, i think that it's more or less 25 years old :-)

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
