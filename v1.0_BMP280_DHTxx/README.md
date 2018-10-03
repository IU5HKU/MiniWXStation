# MiniWX Station BMP280 + DHT11
ESP8266 IoT and BME280 or BMP280 plus DHTxx sensors for a minimalist weather station

Someone ask me for this mod to the original MiniWX, the possibility to use two sensors side by side, Bosch BMP280 for reading temperature and barometric pressure, and DHT11 to read relative humidity.
Despite the DHTxx family of sensor is way more inaccurate then the Bosch ones, this is anyway an opportunity for someone who still have this sensors in his hands to employ them in something useful....or for whatever reason should be.
Changing a bit the source code makes it possible to MiniWX to use DHT sensors, thanks goes to LU7EHR Hernan for support in remote testing and debugging.
To compile this "inner fork" of the project you need the necessary [DHT Library](https://github.com/adafruit/DHT-sensor-library), please download it and install as already explained in the main page of MiniWX.
After that you need to connect the DHT11 to the board, this two images will help you in identify pins.

![DHT11_Image](https://github.com/IU5HKU/MiniWXStation/blob/master/Images/DHT11.jpg)

and remember that there is no direct corrispondence between labels printed ontop your NodeMCU (or generic ESP8266) board and the numbers of the GPIOs used by the system to identify the pin, refer to this schema for the correct corrispondence:

![NodeMCU_GPIOs_Image](https://github.com/IU5HKU/MiniWXStation/blob/master/Images/NodeMCU_GPIOs.png)

These instead are the #defines that you can find, as usual, in the beginning of the sourcecode, with corrispondence labels/gpios. I choose D5 as the pin where DHT11 must be connected, but you can choose another one from D3 to D8:


```javascript
//***************************************************************************
// Uncomment one of the lines below for whatever DHT sensor type you're using
// Please follow this schema if you want change DHT sensor connection:
// Testing and Debugging by LU7EHR Hernan
//***************************************************************************
// D0 - RESERVED (Future Use)
// D1 - RESERVED (BMP280)
// D2 - RESERVED (BMP280)
// D3 - GPIO0 - DHTPin = 0
#define D3  0
// D4 - GPIO2 - DHTPin = 2
#define D4  2
// D5 - GPIO14 - DHTPin = 14
#define D5  14
// D6 - GPIO12 - DHTPin = 12
#define D6  12
// D7 - GPIO13 - DHTPin = 13
#define D7  13
// D8 - GPIO15 - DHTPin = 15
#define D8  15

#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
// DHT Sensor - Connect DHT11 PIN 2 TO ESP8266 PIN5 (GPIO14)

const int DHTPin = D5; 
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);

//***************************************************************************
```

i think it's quite self explanatory, remember to uncomment the #define for the DHT sensor you are using, also if the most common is DHT11, that i left as default. If you comment all three the defines then MiniWX will not use the DHT sensor at all, using only BMP280 and the two values who can retrieve from it, temperature and barometric pressure.
In this case, as advice, please use one of the "standard branch" releases.
