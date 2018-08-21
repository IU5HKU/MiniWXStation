# MiniWXStation
ESP8266 IoT and BME280 sensor for a minimalistic weather station to put in your terrace :-)

This work started as a fork from F4GOH code, so you can visit his blog (https://hamprojects.wordpress.com/) for infos about
how to compile and configure this firmware trough his menu. This instead is his github repository: https://github.com/f4goh/Weather. I've added some features that, at moment, you can set at compile time using #defines.

//**** How the station is named in your NET
const char* WiFi_hostname = "IU5HKU-13";

Change with the name you wish more appropriate 

//**** APRS PASSWORD (use -1 if you are using a CWOP callsign)
const char* AprsPassw = "YourAprsNumericalPASS";

For a CWOP callsign you need to register, it's free and easy, refer to
instruction at https://http://wxqa.com/ to obtain one

//**** comment this for hardcoded 3.7V value in telemetry
#define HAVE_BATTERY

This is for reading analog values connected to A0, for the proper value of the resistor to put in series
refer to this thread: 

//**** uncomment this for weatherunderground upload,remember to set ID and PASSWORD of your account
#define USE_WUNDER
//* change ID and PASSWORD with yours
char ID [] = "YourWunderID";                      
char PASSWORD [] = "YourWunderPASSW";

Major upgrade, sends data to Weahter Underground

//**** show BME280 registers in printbme();
//#define DISPLAY_BME_REGS

Uncomment for printout of registers values in serial console.

//**** blinking led to show that into the 10 minutes the system is still alive (1" blink)
#define BLINK_RED_LED

WILL BE ELIMINATED IN BATTERY POWERED VERSION, this is a "i'm still alive" signal

//**** blinking led to show that ESP8266 is transmitting  (0.5" blink)
#define BLINK_BLUE_LED

WILL BE ELIMINATED IN BATTERY POWERED VERSION, this is a "i'm sending data to servers" signal

//**** show (annoying) animated clock in the serial output 
//#define SHOW_TICKS

Left commented to eliminate the clock in serial console

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

Self explanatory, this settings are needed if you want a static ip for the station
