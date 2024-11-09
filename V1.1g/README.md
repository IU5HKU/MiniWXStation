# MiniWX Station V1.1g
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
- Added webconfig page
- Works with BMP280 too (only pressure and temperature obviously)
- Localization (Spanish by EA1CDV - Italian by IZ5MMH - Català by EA3BIN)
- Modifieds functions (faster) for read/write files in SPIFFS
- In "Config wifi access point menu" added new entry for reset DNS over StaticIP
- More verbosity at first time use
- Checks for browser type and serve better formatted pages in mobile devices
- Fixed "exit" button issues for non-firefox users
- New css "MiniWX" style for buttons
- Setting for Serial Console baudrate speed
- TIME ZONE setting
- Fixed uptime refresh in main page
- Interactive Graphs of the last sampled data (4 hours)
- Link to APRS-Direct to see your station in map (courtesy of SM4WJF Per Qvarforth)
- **New Locale in Catalan Language (thanks to EA3BIN David Torres)**
- **New Locale in Turkish Language (thanks to TA5ITK Kaya Taniel)**
- **New Locale in German Language (thanks to DL9SEC Thorsten Godau)** 
- **Send2APRS() routine now uses a smarter and less invasive method to send packets (thanks to IW1CGW Giovanni)**
- **Some minor bugfixes (thanks to DL9SEC Thorsten Godau)**

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

**New Features in this release**

Now you can check the last 4hours of sampled data in a separate webpage, with a series of fancy graphs.
The data are sampled every 2.5 minutes and contained in a circular buffer, so don't worry, you doesn't need to reset the board or do some magics for prevent the memory to become full. After 4 hours the buffer is full and the new datum is overwritten over the first taken 4 hours before. I know, you can see graphs of your data everywhere, in aprs.fi or in your weatherunderground page, these ones are "duplicated" of something that already exists, but i've got a lot of fun in programming this feature, and i've learnt a lot so why don't release it? 

There is another button in the control panel now, **is a link to [APRS-Direct](https://www.aprsdirect.com/) website, owned by SM4WJF Per Qvarforth**. In his site you can see you station on the map, but most important is the fact that Per was so kind to add a feature to his search engine, and now you can query the entire aprs database by comment!! For what i've see till now APRS-Direct is the unique website that allows this kind of extened query, aprs.fi can't, i'm sure. 
You can write something like 'CMT:*MiniWX*' in the search box on the top left corner, and see all the station around the world that are using MiniWX to report weather data. Obviously if you have cancelled 'MiniWX' from the comment field inside the source code you miss the opportunity to be listed in the results. Thank you so much again Per for your kindness and availability.

Hope you enjoy this release as much as i've got fun in programming it, at moment i'm quite busy in other projects for my primary job, but i think to have still some spare time to mantain this project, but new releases will be published with more delay for sure....sorry.
