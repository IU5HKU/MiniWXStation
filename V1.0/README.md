
# MiniWX Station V1.0
ESP8266 IoT and BME280 sensor for a minimalist weather station

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
- **Added webconfig page
- Works with BMP280 too (only pressure and temperature obviously)
- Localization (Spanish by EA1CDV - Italian by IZ5MMH)
- Modifieds functions (faster) for read/write files in SPIFFS
- In "Config wifi access point menu" added new entry for reset DNS over StaticIP
- More verbosity at first time use**

The new station can be configured through webpagw now, and there is a more useful and pleasant main page to check bme280 values and some other infos:


