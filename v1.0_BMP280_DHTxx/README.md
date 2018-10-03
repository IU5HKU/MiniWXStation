# MiniWX Station BMP280 + DHT11
ESP8266 IoT and BME280 plus DHTxx sensors for a minimalist weather station

Someone of you askme for this mod to the original MiniWX, the possiiblity to use two sensors side by side, Bosch BMP280 for reading temperature and barometric pressure, and DHT11 to read realtive humidity.
Despite the DHTxx family of sensor is way more inaccurate then the Bosch ones, this is anyway an opportunite for someone who still have this sensors in his hands to employ them in something useful.
Thanks to the help of LU7EHR Hernan we have change a little bit the code and makes possible to MiniWX the use of DHT sensors.
To compile this "inner fork" of the project you need the necessary DHT Library, please download it and install as already explained in the main page of MiniWX.
After that you need to connect the DHT11 to the board, this two images will help you in identify pins.

![DHT11_Image](https://github.com/IU5HKU/MiniWXStation/blob/master/Images/DHT11.jpg)

and remember that there is no direct corrispondence between labels printed ontop your NodeMCU (or generic ESP8266) board and the numbers of the GPIOs used by the system to identify the pin, refer to this schema for the correct corrispondence:

![NodeMCU_GPIOs_Image]("https://github.com/IU5HKU/MiniWXStation/blob/master/Images/NodeMCU GPIOs.png")

These instead are the #defines that you can find in the beginning of the sourcecode, with a brief explanation of what
happens if you choose to use them, it's a bit rude as method, but there is place for a config webpage hosted completely by the ESP8266....it's in my todo list, not in near future, i need to better test some other features.

```javascript
//**** How the station is named in your NET
const char* WiFi_hostname = "IU5HKU-13";
```
