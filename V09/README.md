# MiniWX Station V09
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
