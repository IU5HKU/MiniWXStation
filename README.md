# MiniWXStation
ESP8266 IoT and BME280 sensor for a minimalistic weather station to put in your terrace :-)

This work started as a fork from F4GOH code, so you can visit his blog (https://hamprojects.wordpress.com/) for infos about
how to compile and configure this firmware trough his menu. This instead is his github repository: https://github.com/f4goh/Weather. I've added some features that, at moment, you can set at compile time using #defines.
Many thanks to Antonio EA1CDV for his support, beta testing and encouragement, you can read a detailed article in spanish language about this project in his blog: http://ea1cdv.blogspot.com/.
In a next version i'll change the source code to use native ESP8266 ntp calls instead of using NTPTimeESP, but for the moment the source code needs this library, so you have to add it to your environment, https://github.com/SensorsIot/NTPtimeESP
Obviously you nedd the BME280 Library too: https://github.com/sparkfun/SparkFun_BME280_Arduino_Library

<code>
  //**** How the station is named in your NET
  const char* WiFi_hostname = "IU5HKU-13";
</code>
Change with the name you wish more appropriate

<code>
//**** APRS PASSWORD (use -1 if you are using a CWOP callsign)
const char* AprsPassw = "YourAprsNumericalPASS";
</code>
For a CWOP callsign you need to register, it's free and easy, refer to
instruction at http://wxqa.com/ to obtain one

<code>
//**** comment this for hardcoded 3.7V value in telemetry
#define HAVE_BATTERY
</code>

This is for reading analog values connected to A0, for the proper value of the resistor to put in series
refer to this thread: https://forum.arduino.cc/index.php?topic=445538.0
<code>
//**** uncomment this for weatherunderground upload,remember to set ID and PASSWORD of your account
#define USE_WUNDER
//* change ID and PASSWORD with yours
char ID [] = "YourWunderID";                      
char PASSWORD [] = "YourWunderPASSW";
</code>

Major upgrade, sends data to Weahter Underground
<code>
//**** show BME280 registers in printbme();
//#define DISPLAY_BME_REGS
</code>

Uncomment for printout of registers values in serial console.
<code>
//**** blinking led to show that into the 10 minutes the system is still alive (1" blink)
#define BLINK_RED_LED
</code>

WILL BE ELIMINATED IN BATTERY POWERED VERSION, this is a "i'm still alive" signal
<code>
//**** blinking led to show that ESP8266 is transmitting  (0.5" blink)
#define BLINK_BLUE_LED
</code>

WILL BE ELIMINATED IN BATTERY POWERED VERSION, this is a "i'm sending data to servers" signal
<code>
//**** show (annoying) animated clock in the serial output 
//#define SHOW_TICKS
</code>

Left commented to eliminate the clock in serial console
<code>
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
</code>

Self explanatory, this settings are needed if you want a static ip for the station
