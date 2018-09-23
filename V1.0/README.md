
# MiniWX Station V1.0
ESP8266 IoT and BME280 sensor for a minimalist weather station

Compiled with:

-***ArduinoIDE 1.8.6***-
-***SDK:2.2.1(cfd48f3)/Core:2.4.2/lwIP:2.0.3(STABLE-2_0_3_RELEASE/glue:arduino-2.4.1-13-g163bb82)/BearSSL:6d1cefc***-

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

**WARNING:**

The new ArduinoIDE Serial Monitor speed now is **74880bps**, not why i love weird numbers, but because this is the serial speed who permits to correctly see the core and systems messages of the sdk, the "weird" chars that you can read at startup or after a reset or a crash becomes in clear now, and this help me in understanding what happens under the hood when someone told me that his station doesn't works :-)


The new station can be configured through webpage now, and there is a more useful and pleasant main page to check bme280 values and some other infos:

![WebMainPage_Image](https://github.com/IU5HKU/MiniWXStation/blob/master/Images/WebMainPage.png)

only the first three parameters are read from BME280 (two in case of BMP280) the last two are derived from calculus.
The page use a jquery script and refresh only the values, every second, in this way there are only few data to exchange from esp8266 to browser, without need to reload and refresh the whole page.
The clock who ticks on top of the page is the esp8266 inner "soft" clock, so could be not properly in sync with your pc clock, but it's just a matter of few seconds, less of 5 usually. This isn't a problem at all, the clock is not used anymore for sending packets, that are sent as "realtime data", substantially are timeless packets, the same as you aprs tracker sends.
In the bottom section of the page you can find a control panel, to mimic some of the functions you've find in the serial menu, the most important is the "settings" button, who lands you here:

![WebSettings_Image](https://github.com/IU5HKU/MiniWXStation/blob/master/Images/WebSettings.png)

i think that this page is self explanatory, you can change most of the station settings, and save them through the "save" button, or simply check your current configuration and then "exit" to main page without saving.
A little bug when you change values: the refresh of the main page doesn't work, my javascript fails....need to be checked and fixed in the new version, yes, there will be a new version, with graphs and simple forecasts (like an analog barometer or can do) and with aprs bullettin if my forecasts algo will work quite well...
Others buttons are self explanatory, you can't see the output at moment, the related info page will close and come back automagically, but in the near future i think to implement something called "SSE" (Server Side Event) to update the page contents with some useful infos....work in progress for the moment.

