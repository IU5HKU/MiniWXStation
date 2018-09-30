
# MiniWX Station V1.0
ESP8266 IoT and BME280 sensor for a minimalist weather station

Compiled with:

**ArduinoIDE 1.8.6**
**SDK:2.2.1(cfd48f3)/Core:2.4.2/lwIP:2.0.3(STABLE-2_0_3_RELEASE/glue:arduino-2.4.1-13-g163bb82)/BearSSL:6d1cefc**

- upload to weather underground
- added aprs telemetry packets
- using Ticker() for counting transmit delay
- blinking leds for infos
- ESP.getVcc() in telemetry
- more robust NTP sync
- Implemented 'SoftClock' and less aggressive ntpsync (user defined delay)
- Extended comment in APRS packet
- realtime APRS packets instead of 'timestamp-ed' ones
- Dynamics webpages for infos and config parameters
- Added OTA firmware flashing
- Added more verbose WiFi Scan routine
- **Added webconfig page**
- **Works with BMP280 too (only pressure and temperature obviously)**
- **Localization (Spanish by EA1CDV - Italian by IZ5MMH)**
- **Modifieds functions (faster) for read/write files in SPIFFS**
- **In "Config wifi access point menu" added new entry for reset DNS over StaticIP**
- **More verbosity at first time use**
- **Checks for browser type and serve better formatted pages in mobile devices**
- **fixed "exit" button issues for non-firefox users**
- **new css "MiniWX" style for buttons**
- **setting for Serial Console baudrate speed**

**WARNING!**

![SerialMonitorSpeed_Image](https://github.com/IU5HKU/MiniWXStation/blob/master/Images/SerialMonitorSpeed.png)

The new ArduinoIDE Serial Monitor speed now is **74880bps**, not why i love weird numbers, but because this is the serial speed who permits to correctly see the core and systems messages of the sdk, the "weird" chars that you can read at startup or after a reset or a crash becomes in clear now, and this help me in understanding what happens under the hood when someone told me that his station doesn't works :-) Please set "Newline" as carriage return too, because it's really important when you insert datas through serial console, websetting page are not affected by this parameter.
Anyway, as requested by some users, i've left the possibility to choose the serial console communication baudrate, as usual at the beginning of the source code you can find this:

```javascript
//**** CHOOSE SERIAL MONITOR BAUD RATE
//#define SER_MON_BAUDRATE 115200
#define SER_MON_BAUDRATE 74880
```
and is up to you let's make that this baudrate match the one you use in your terminal emulator program (ArduinoIDE's Serial Monitor is more than enough). If you are under Linux, using ArduinoIDE as development environment, remember to 'chmod'-ding your serial device, otherwise you can encounter problems in changing serial speed, it's only matter to type in a shell:

```javascript
    sudo chmod a+x /dev/ttyUSB0
```
....or whatever 'USB' is your esp8266 connected to...

There is localization now, choose as usual with #defines, from this three languages:

```javascript
    //**** CHOOSE WEBPAGES LANGUAGE
    #define LANG_ENGLISH
    //#define LANG_SPANISH
    //#define LANG_ITALIAN
```
The sketch now recognizes correctly Bosch BME280 and BMP280 sensors (this last one lacks the relative humidity measure) and shows the corrispondents values accordly to the sensor (no more "nan" and unwanted output). Also the aprs packet is changed accordly. In "Config wifi access point menu" now there is a new entry for reset DNS over StaticIP, just in case you make mistakes and don't wanna reflash the entire sketch to correct an error in typing IP or gateway and so on.
The new station can be configured through webpage now, and there is a more useful and pleasant main page to check bme280 values and some other infos:

![WebMainPage_Image](https://github.com/IU5HKU/MiniWXStation/blob/master/Images/WebMainPage.png)

only the first three parameters are read from BME280 (two in case of BMP280) the last two are derived from calculus.
The page use a jquery script and refresh only the values, every second, in this way there are only few data to exchange from esp8266 to browser, without need to reload and refresh the whole page.
The clock who ticks on top of the page is the esp8266 inner "soft" clock, so could be not properly in sync with your pc clock, but it's just a matter of few seconds, less of 5 usually. This isn't a problem at all, the clock is not used anymore for sending packets, that are sent as "realtime data", substantially are timeless packets, the same as you aprs tracker sends.
In the bottom section of the page you can find a control panel, to mimic some of the functions you've find in the serial menu, the most important is the "settings" button, who lands you here:

![WebSettings_Image](https://github.com/IU5HKU/MiniWXStation/blob/master/Images/WebSettings.png)

i think that this page is self explanatory, you can change most of the station settings, and save them through the "save" button, or simply check your current configuration and then "exit" to main page without saving.
A little bug when you change values: the refresh of the main page doesn't work, my javascript fails....need to be checked and fixed in the new version, yes, there will be a new version, with graphs and simple forecasts (like an analog barometer or can do) and with aprs bullettin if my forecasts algo will work quite well...
Others buttons are self explanatory, you can't see the output at moment, the related info page will close and come back automagically, but in the near future i think to implement something called "SSE" (Server Sent Events) to update the page contents with some useful infos....work in progress for the moment.

Now MiniWX check the client's browser for Android type, serving a more readable main webpage in case of mobiles, just in case the new sizes doesn't match your needs you can change the percentage directly in code, at row 603, inside the AdjustFieldsets() function you find this two strings:
```javascript
        const char* mobile_fieldsizes[] = {"97%","44%","32%","18%","98%","18%","78%","98%","*"};
        const char* pc_fieldsizes[] = {"49%","50%","22%","18%","50%","28%","68%","50%","*"}; 
```
the sizes are expressed in sequential way, from top to bottom, feel free to change values as you want.

The webpages are entirely stored in flash, "no ram is abused in this program", thanks to the PROGMEM directive, who tell the linker to put vars in flash:

```javascript
    const char PAGE_Main_ES[] PROGMEM = "<!DOCTYPE html><html><head>.....
```
and to be retrieved by means of FPSTR:

```javascript
    String page = FPSTR(PAGE_Main_EN);
```
this, side to side with "F" macro, allows to save a lot of precious ram.
The page are created and filled dynamically using placeholders enclosed in double braces, as this:

```javascript
    <legend style='text-shadow: 2px 1px grey;'> {{ChipModel}} </legend>\
```
in this case {{ChipModel}} is the placeholder and just before serving the page is changed with the proper value thanks to the strings manipulation functions:

```javascript
    page.replace(F("{{ChipModel}}"), String("BMP280"));
```
this is a powerful way for deliver dynamics contents in your webpages, with a very low footprint in cpu and ram resources.
To be honest i've read around the web that the 'String' type is an evil beast in Arduino and Arduino-like environment as ESP8266 is, seems that using concatenation operands, like this ones:

```javascript
    message += FPSTR(HTTP_REBOOT_SCRIPT);
    message += FPSTR(HTTP_BODY);
```
concurr to the fragmentation of the ram, who lead to a crash in the long time use. I need to better understand and read about this potential issue, maybe that in the future MiniWX will use others methods to concatenate string (transparently to the final user obviously), but for the moment the entire system seems to be quite stable, i mark this thing as "todo when the whole program will be finished".

**KNOW BUGS:**

There is a little problem at startup, the first time you flash this firmware may be the settings you typed in the various strings in the source code doesn't work well, the most common error is the USE_WUNDER flag that doesn't work properly and the static ip is not saved well, but the second time you run the sketch, after a power cycle, everything is fixed.
This thing makes me crazy for a while, i've decided to release the version despite this problems, it's not so severe and the station can be used without problems, anyway will be fixed in the future (i hope).
Another issue is the REBOOT button, for an esp8266, not my fault, after you have flashed the chip the issued software commands doesn't reboot properly the whole system who hangs in an infinite loop. The remedy is an hardware reset just after flashed the MiniWX, after that all the successive reboots working as expected.
