
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

**WARNING!**

![SerialMonitorSpeed_Image](https://github.com/IU5HKU/MiniWXStation/blob/master/Images/SerialMonitorSpeed.png)

The new ArduinoIDE Serial Monitor speed now is **74880bps**, not why i love weird numbers, but because this is the serial speed who permits to correctly see the core and systems messages of the sdk, the "weird" chars that you can read at startup or after a reset or a crash becomes in clear now, and this help me in understanding what happens under the hood when someone told me that his station doesn't works :-)

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

**KNOW BUGS:**

They are not properly bugs, but a "still to implement" status. The webpages looks nicely in the pc browsers, but not so nicely in the tablets and mobiles browsers, this because i set the container of the forms to be the 50% of the entire page, and this become a properly formatted page in big screens, but when using mobiles 50% isn't enough and the layout results completely mangled (or close to completely). In future release MiniWX will recognize correctly the browser and adapt the contents accordly, for the moment, if your prevalent use is by mobile, then you must manually change the form percentage in the "XX_Locale.h" file of your interest, search for the <fieldset> tags:

```javascript
  <fieldset style='width:49%'>
```

and change the percentage to 98%, (there are more than one in the file) this will made the webpages work correctly in mobiles but in pc browser the frame will occupy the whole page. To justify me, i wanna say that i'm not an html/javascript expert at all and that i'm learning while i'm writing this code, so be sympathetic with me....waiting for a new better working version :-)

There is a little problem at startup, the first time you flash this firmware may be the settings you typed in the various strings in the source code doesn't work well, the most common error is the USE_WUNDER flag that doesn't work properly and the static ip is not saved well, but the second time you run the sketch, after a power cycle, everything is fixed.
This thing makes me crazy for a while, i've decided to release the version despite this problems, it's not so severe and the station can be used without problems, anyway will be fixed in the future (i hope).
