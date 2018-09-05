/*
esp8266 with BME280
- config menu with serial
- ntp update
- upload weather data on aprs servers
- local web page for weather informations and configuration
 f4goh@orange.fr

compiler options:
ESP8266 SDK 2.4.2
lwIP V2
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

 iu5hku (mrcodemail@gmail.com)

*/

#include <Wire.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ArduinoOTA.h>
#include <Ticker.h>

#include <NTPtimeESP.h>
#include "FS.h"

#include <SparkFunBME280.h>
// Global sensor object
BME280 mySensor;

// webserver for parameters page
ESP8266WebServer server(80);

//**** INTERNAL USE (SOFTWARE VERSION)
const char* SOFT_VER = "v0.9";

//**************************************************************************************************
//* ADDED FEATURES  - SWITCHES
//**************************************

//**** How the station is named in your NET
const char* WiFi_hostname = "IU5HKU-13";

//**** APRS PASSWORD (use -1 if you are using a CWOP callsign)
const char* AprsPassw = "YourAprsNumericalPASS";

//**** APRS COMMENT, you can set this string as you want (max 43 chars)
const char* APRS_CMNT = "MiniWX Station .:.ESP8266.:.";

//**** APRS_PRJ, Telemetry Project Title (max 23 chars)
const char* APRS_PRJ = "MiniWX Station";

//**** Comment this for ESP.getVcc() value in telemetry
//**** getVcc function (system_get_vdd33) is only available when A0 pin17 is suspended (floating), 
//**** this function measure the power voltage of VDD3P3 pin 3 and 4 (in the ESP8266 chip)
//#define HAVE_BATTERY

//**** uncomment this for weatherunderground upload,remember to set ID and PASSWORD of your account
//#define USE_WUNDER
//* change ID and PASSWORD with yours
char ID [] = "YourWunderID";                      
char PASSWORD [] = "YourWunderPASSW";

//**** show BME280 registers in Serial Output;
//#define DISPLAY_BME_REGS
//**** show BME280 values in Serial Output;
#define DISPLAY_BME_VAL

//**** blinking led to show that into the 10 minutes the system is still alive WILL BE ELIMINATED IN BATTERY POWERED VERSION (1" blink)
//**** NOTE: WEMOS D1 Mini doens't have this led, NodeMCU V0.9 & V1.0 have it.
#define BLINK_RED_LED

//**** blinking led to show that ESP8266 is transmitting WILL BE ELIMINATED IN BATTERY POWERED VERSION (0.5" blink)
#define BLINK_BLUE_LED

//**** show (annoying) animated clock in the serial output 
//#define SHOW_TICKS

//**** Sync the soft clock every 12 hours
#define NTPSYNC_DELAY  12*60*60

//**** Set credential for OTA firmware upgrade <<--->>
//*uncomment the #define if you wanna use this handy feature
//#define USE_OTA_UPGRADE
#ifdef USE_OTA_UPGRADE
const char* OTA_hostname = "MINIWX";
//PLEASE CHANGE THIS PASSWORD WITH YOUR OWN FOR SECURITY REASON
const char* OTA_passw = "esp8266";  
#endif

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
//**************************************************************************************************

//* time related structures and vars
//* time server for ntp function
//NTPtime NTPch("time.nist.gov"); 
NTPtime NTPch("ntp1.inrim.it");   //italian national institute for measures
strDateTime dateTime;
byte nextMinTx;
byte nextHour;
byte nextSecTx;
//flags for Time counters
Ticker TkAlarm;
bool bSendFlag;
Ticker TkSeconds;
bool bSecsFlag;
Ticker TkBlueLed;
bool bNtpSyncFlag;
Ticker TkNtpSync;

typedef struct {
  long rssi;
  unsigned int vbat;
} Telemetry;
Telemetry tl; // declare telemetry structure

typedef struct  {
  float temperatureC;
  float temperatureF;
  float pression ;
  float humidity;
  double fdewptf;
} WeatherStruct;
WeatherStruct wx;    //declare la structure

typedef struct  {
  char ssid[50];
  char password[50];
} configStruct;
configStruct internet;    //declare la structure

//for connections to clients (APRS servers and WUNDER server)
//better keep this declaration global (as seen in forums)
WiFiClient client;

typedef struct  {
  char callsign[10];
  char tlm_callsign[10];
  char longitude[10];
  char latitude[10];
  char clientAdress[20];
  int clientPort;
  long transmitDelay;
  byte logger;
  int altitude;
} positionStruct;
positionStruct station;    //declare la structure

char car;

//**************************************
//* WUNDERGROUND
//**************************************
char WGserver [] = "weatherstation.wunderground.com";  
char WEBPAGE [] = "GET /weatherstation/updateweatherstation.php?";

void calcDewPoint(){
  // Calculate dew Point
  double A0= 373.15/(273.15 + wx.temperatureF);
  double SUM = -7.90298 * (A0-1);
  SUM += 5.02808 * log10(A0);
  SUM += -1.3816e-7 * (pow(10, (11.344*(1-1/A0)))-1) ;
  SUM += 8.1328e-3 * (pow(10,(-3.49149*(A0-1)))-1) ;
  SUM += log10(1013.246);
  double VP = pow(10, SUM-3) * wx.humidity;
  double T = log(VP/0.61078);   
  wx.fdewptf = (241.88 * T) / (17.558-T);
}

#ifdef USE_WUNDER
void Send2Wunder(){
  // Calculate dew Point
  calcDewPoint();
  // connect to wunderground
  if (!client.connect(WGserver, 80)) {
    Serial.println("Send2Wunder Fail");
    return;
  }
    Serial.print("WeatherUnderground page updating....");
    client.print(WEBPAGE); 
    client.print("ID=");
    client.print(ID);
    client.print("&PASSWORD=");
    client.print(PASSWORD);
    client.print("&dateutc=");
    client.print("now");    
    client.print("&tempf=");
    client.print(wx.temperatureF);
    client.print("&dewptf=");
    client.print(wx.fdewptf);
    client.print("&humidity=");
    client.print(wx.humidity);
    client.print("&baromin=");
    client.print((wx.pression/100)*0.02953f); // 1 mbar = 0.02953 inHg
    //more compliant WU output by EA1CDV Antonio
    client.print("&softwaretype=MiniWX%20Station%20");
    client.print(SOFT_VER);
    client.print("&action=updateraw");
    client.println();
    delay(2500); 
    //Serial.println("done!");
    
    //print server reply
    Serial.print("server reply:");
    while (client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
    
    client.stop();
}
#endif

//******************************************
//* ARDUINO OTA SET HANDLERS
//* for flashing the firmware through TCP/IP
//******************************************
#ifdef USE_OTA_UPGRADE
void SetOTA(){

  ArduinoOTA.setHostname(OTA_hostname);
  ArduinoOTA.setPassword(OTA_passw);

  ArduinoOTA.onStart([]() {
      Serial.println("Start");
    });
    
  ArduinoOTA.onEnd([]() {
      Serial.println("\nEnd");
    });
    
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
  
  ArduinoOTA.onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });
  
  ArduinoOTA.begin();
  Serial.println("OTA ready");
}
#endif
//**************************************
//* TICKERS INTERRUPT ROUTINES
//* keep as short as posssible!
//**************************************
void SetSendFlag(void){
  bSendFlag=true;
}

//Soft Clock, quite precise indeed...
void SetSecsFlag(void){
  bSecsFlag=true;
  dateTime.second = (dateTime.second + 1) % 60;

    if (dateTime.second == 0) {
      dateTime.minute = (dateTime.minute + 1) % 60;
      if (dateTime.minute == 0) {
        dateTime.hour = (dateTime.hour + 1) % 24;
      }
    }
}

void SetNtpSyncFlag(void){
  bNtpSyncFlag=true;
}

#ifdef BLINK_BLUE_LED
void BlinkBlueLed(){
  digitalWrite(D4, digitalRead(D4)^1); // turn the ESP-12 LED off and on (HIGH is the voltage level and meaning OFF)
}
#endif

//**************************************
//* MAIN SETUP ROUTINE
//**************************************
void setup(void)
{  
  strcpy(station.clientAdress, "cwop.aprs.net");  // default, reccomended over rotate.aprs.net
  station.clientPort = 14580;
  station.transmitDelay = 10;
  station.logger = 0;

  Serial.begin(115200);
  Serial.println();
  delay(10);

  SPIFFS.begin();
  if (SPIFFS.exists("/ssid.txt") == 0) {
    configMenu();
  }
  else
  {
    readSsidFile();
  }
  if (SPIFFS.exists("/station.txt") == 0) {
    configMenu();
  }
  else
  {
    readStationFile();
  }
  if  (detectMenu() == 1) configMenu();
  
  ssidConnect();
  initBme();
  printBme();

#ifdef USE_OTA_UPGRADE
  //** Init the required handlers for OTA firmware flashing
  SetOTA();
#endif

  server.on("/", handleRoot);
  server.on("/submit", handleSubmit);
  /*
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });
  server.on ( "/test.svg", drawGraph );
*/
  server.onNotFound(handleNotFound);
  server.begin();

  // First and most important sync with NTP Server (next sync will be done after 12 hours)
  ntp();
  
  //set bSecsFlag for tickin'the clock
  bSecsFlag=false;
  TkSeconds.attach( 1, SetSecsFlag);

  //initialize random seed
  randomSeed(analogRead(A0));

  //NodeMCU V0.9 pinout for onboard LED
#ifdef BLINK_BLUE_LED
  pinMode(D4, OUTPUT);     // Initialize GPIO2 pin as an output
  digitalWrite(D4, HIGH);  // Led OFF
#endif
#ifdef BLINK_RED_LED
  pinMode(D0, OUTPUT);     // Initialize GPIO16 pin as an output WARNING: DO NOT USE in deepsleep() mode!!!
  digitalWrite(D0, HIGH);  // Led OFF
#endif
#ifdef HAVE_BATTERY
    pinMode(A0, INPUT);   // read from A0
#endif

  //set bSendFlag to true every transmitDelay seconds
  bSendFlag=false;
  TkAlarm.attach( station.transmitDelay*60, SetSendFlag);

  //set bNtpSyncFlag for NTPsync() every 'NTPSYNC_DELAY' seconds
  bNtpSyncFlag=false;
  TkNtpSync.attach( NTPSYNC_DELAY, SetNtpSyncFlag);
}

int sysUpTimeMn;
int sysUpTimeHr;
int sysUpTimeDy;

void SystemUpTime() {
  long millisecs = millis();
  sysUpTimeMn = int((millisecs / (1000 * 60)) % 60);
  sysUpTimeHr = int((millisecs / (1000 * 60 * 60)) % 24);
  sysUpTimeDy = int((millisecs / (1000 * 60 * 60 * 24)) % 365);
}

//***********************************************************
//* MINIWX STATION  - MAIN INFO PAGE
//* improved readability on portable devices by EA1CDV Antonio
//* WebPages in FLASH and smart dynamics pages from:
//* http://blog.tldnr.org/2017/10/25/how-to-deliver-larger-web-pages-with-an-esp8266/
//***********************************************************
const char PAGE_infos[] PROGMEM = "<html>\
  <head>\
    <meta http-equiv='refresh' content='5'/>\
    <title>{{callsign1}} Weather Station</title>\
    <style>\
      body { background-color: #000000; font-family: Arial, Helvetica, Sans-Serif; Color: #13F70C; font-size: 32px;}\
      table { display: table; border: 1px solid green; border-collapse: separate; border-spacing: 1px; border-color: green; }\
      th, td {border: 1px solid green; border-collapse: separate; text-align: center; font-size: 28px;}\
      th {border-left: 30px solid green; border-right: 30px solid green; background: green; Color: black; font-size: 32px;}\
    </style>\
  </head>\
  <body>\
    <hr/>\
    <h3>{{callsign2}} Weather Station - {{time}}</h3>\
    <h4>\
    <fieldset>\
      <legend> BME280 Values </legend>\
      <div><table><thead><tr><th>Parameter</th><th>Value (realtime)</th><th>Unit</th></tr></thead><tbody>\
      <tr><td style='text-align:left'>Temperature</td><td>{{degC}}</td><td>&#8451;</td></tr>\
      <tr><td style='text-align:left'>Pressure</td><td>{{mBar}}</td><td>mBar</td></tr>\
      <tr><td style='text-align:left'>Relative Humidity</td><td>{{rHum}}</td><td>%</td></tr>\
      <tr><td style='text-align:left'>Dew point</td><td>{{DPdegC}}</td><td>&#8451;</td></tr>\
      </tbody></table></div>\
      </br>\
      <button disabled name='Trend' type='submit' value='true'> Graphics Trends </button>\
    </fieldset>\
    </h4>\
    <h6>\
    <fieldset>\
      <legend> System Infos </legend>\
      <p>Uptime Days:{{days}} Hrs:{{hrs}} Min:{{min}}</p>\
      <p>Next TX at: {{nexttx}}</p>\
      <p>SSID: {{SSID}}</p>\
      <p>RSSI: {{RSSI}} dbm</p>\
      <p>Position: {{lat}} {{long}}</p>\
    </fieldset>\
    </br>\
    <form action='http://{{myip}}/submit' method='POST'>\
      <fieldset>\
      <legend> Control Panel </legend>\
      <div>\
        <button name='Reboot' type='submit' value='true'> ESP8266 Reboot </button>\
        <button name='SendAPRS' type='submit' value='true'> Send APRS Packet </button>\
        <button name='SendWUNDER' type='submit' value='true'> Send WUNDER Packet </button>\
        <button name='NTPSync' type='submit' value='true'> NTP Sync </button>\
        <button name='WiFiScan' type='submit' value='true'> WiFi Scan </button>\
      </div>\
    </fieldset>\
    </form>\
    <p>MiniWX Station{{TM}} ({{SOFT_VER}})</p>\
    <p>Click <a href='https://github.com/IU5HKU/MiniWXStation'>here</a> and check for latest version</p>\
    </h6>\
  </body>\
</html>";

//*** HTML PAGE ELEMENTS
const char HTTP_HEAD[] PROGMEM = "<!DOCTYPE html><html><head lang='en'><title>{{callsign}} Weather Station</title>";
const char HTTP_SCRIPT[] PROGMEM = "<script></script>";
const char HTTP_STYLE[] PROGMEM = "<style> body { background-color: #000000; font-family: Arial, Helvetica, Sans-Serif; Color: #13F70C; font-size: 32px;}\
                                           table { display: table; border: 1px solid green; border-collapse: separate; border-spacing: 1px; border-color: green; }\
                                           th, td {border: 1px solid green; border-collapse: separate; text-align: center; font-size: 18px;}\
                                           th {border-left: 15px solid green; border-right: 15px solid green; background: green; Color: black; font-size: 20px;}\
                                   </style>";
const char HTTP_BODY[] PROGMEM = "<body>";
const char HTTP_FOOT[] PROGMEM = "</body></html>";

//*** HTML SYMBOLS
const char up_arrow[] = "&#8593;";
const char down_arrow[] = "&#8595;";
const char black_sun[] = "&#9728;";
const char cloud[] = "&#9729;";
const char umbrella[] = "&#9730;";
const char degreeC[] = "&#8451;";
const char degreeF[] = "&#8457;";
const char unlock[] = "&#128275;";
const char lock[] = "&#128274;";
const char trademark[] = "&#8482;";

void handleRoot() {
  String page = FPSTR(PAGE_infos);
  char buffer[20];

  page.replace("{{callsign1}}", station.callsign);
  page.replace("{{callsign2}}", station.callsign);
  page.replace("{{lat}}", station.latitude);
  page.replace("{{long}}", station.longitude);
  
  SystemUpTime();
  page.replace("{{days}}", String(sysUpTimeDy));
  page.replace("{{hrs}}", String(sysUpTimeHr));
  page.replace("{{min}}", String(sysUpTimeMn));

  getBmeValues();
  calcDewPoint();
  page.replace("{{degC}}", String((wx.temperatureC),2));
  page.replace("{{mBar}}", String((wx.pression/100),2));
  page.replace("{{rHum}}", String(wx.humidity,2));
  float degc = (wx.fdewptf-32.0f)*0.55f;// Fahrenheit to Celsius
  page.replace("{{DPdegC}}", String(degc,2));  

  sprintf(buffer, "%02d:%02d:%02d", nextHour, nextMinTx, nextSecTx);
  page.replace("{{nexttx}}", buffer);

  sprintf(buffer, "%02d:%02d:%02d", dateTime.hour, dateTime.minute, dateTime.second);
  page.replace("{{time}}", buffer);

  page.replace("{{SSID}}", internet.ssid);
  page.replace("{{RSSI}}", String(WiFi.RSSI()));
  page.replace("{{myip}}", WiFi.localIP().toString());

  page.replace("{{TM}}", trademark);
  page.replace("{{SOFT_VER}}", SOFT_VER);

  // Serve page actually
  server.sendHeader("Content-Length", String(page.length()));
  server.send ( 200, "text/html", page );
}

//Handle the buttons
void handleSubmit(){
  String message;
  
  if (server.args() > 0 ) {
    //** common elements of the various pages
    message += FPSTR(HTTP_HEAD);
    message.replace("{{callsign}}", station.callsign);
    message += FPSTR(HTTP_SCRIPT);
    message += FPSTR(HTTP_STYLE);
    message += FPSTR(HTTP_BODY);
      
    if (server.argName(0) == "Reboot" && server.arg(0) == "true"){
      // Serve page actually
      message += "<p>ESP8266 reboot, please press 'back' and wait for reconnect...</p>";
      message += "<form><input type='button' value='back' onclick='history.back()'></form>";
      message += FPSTR(HTTP_FOOT);

      server.sendHeader("Content-Length", String(message.length()));
      server.send ( 200, "text/html", message );
      delay(1000);
      
      ESP.restart();
    }
    
    if (server.argName(0) == "SendAPRS" && server.arg(0) == "true"){
      //Display some useful info's in a new page and come back to main page
      getBmeValues();  
    
      message += "<p>Packet sent to APRS server</p>";
      message += "<form><input type='button' value='back' onclick='history.back()'></form>";
      message += FPSTR(HTTP_FOOT);
    
      server.sendHeader("Content-Length", String(message.length()));
      server.send ( 200, "text/html", message );
      Send2APRS();
    }
    
    if (server.argName(0) == "SendWUNDER" && server.arg(0) == "true"){
      //Display some useful info's in a new page and come back to main page
      getBmeValues();
    
      message += "<p>Packet sent to Weather Underground server</p>";
      message += "<form><input type='button' value='back' onclick='history.back()'></form>";
      message += FPSTR(HTTP_FOOT);

      server.sendHeader("Content-Length", String(message.length()));
      server.send ( 200, "text/html", message );
#ifdef USE_WUNDER  
      Send2Wunder();
#endif
    }
    
    if (server.argName(0) == "NTPSync" && server.arg(0) == "true"){

      message += "<p>Sent NTP sync request to server</p>";
      message += "<form><input type='button' value='back' onclick='history.back()'></form>";
      message += FPSTR(HTTP_FOOT);
      
      server.sendHeader("Content-Length", String(message.length()));
      server.send ( 200, "text/html", message );
      ntp();
    }

     // WiFi Scan and put results in a table with CSS Style
    if (server.argName(0) == "WiFiScan" && server.arg(0) == "true"){
      String stations;
      int i=0;
      int AvailNets = WiFi.scanNetworks();//Scan for total networks available
      // and prepare the table to show in webpage
      stations = "<div><table><thead><tr><th>SSID</th><th>RSSI (dbm)</th><th>CHANNEL</th><th>BSSID</th><th>ENCRYPTION TYPE</th></tr></thead><tbody>";

      for ( i; i < AvailNets; ++i)
      {
        // Print SSID and RSSI for each network found
        stations += "<tr>";
        stations += "<td <td style='text-align:left'>";
        stations += WiFi.SSID(i);
        stations += "</td><td>";
        stations += WiFi.RSSI(i);
        stations += "</td><td>";
        stations += WiFi.channel(i);
        stations += "</td><td>";
        stations += WiFi.BSSIDstr(i);
        stations += "</td><td style='text-align:left'>";
        switch(WiFi.encryptionType(i))
        {
          case ENC_TYPE_WEP:
            stations += String(lock) + " WEP ";
            break;
          case ENC_TYPE_TKIP:
            stations += String(lock) + " WPA/PSK ";
            break;
          case ENC_TYPE_CCMP:
            stations += String(lock) + " WPA2/PSK ";
            break;
          case ENC_TYPE_NONE:
            stations += String(unlock) + " OPEN ";
            break;
          case ENC_TYPE_AUTO:
            stations += String(lock) + " WPA/WPA2/PSK ";
            break;
        }
        stations += "</td>";
      }
      stations += "</tr>";

      message += "<p>WiFi Scanning founds ";
      message += String(i);
      message += " networks</p>";
      message += stations;
      message += "</tbody></table></div>";
      message += "<form><input type='button' value='back' onclick='history.back()'></form>";
      message += FPSTR(HTTP_FOOT);
      
      server.sendHeader("Content-Length", String(message.length()));
      server.send ( 200, "text/html", message );
    }
  }
}

//*************************************************
//* PAGE NOT FOUND
//*************************************************
void handleNotFound() {
  String message = "File Not Found\n\n";
  
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  message += "\n";
  message += "FreeHeap.....: " + String(ESP.getFreeHeap()) + "\n";
  message += "ChipID.......: " + String(ESP.getChipId()) + "\n";
  message += "FlashChipId..: " + String(ESP.getFlashChipId()) + "\n";
  message += "FlashChipSize: " + String(ESP.getFlashChipSize()) + " bytes\n";
  message += "getCycleCount: " + String(ESP.getCycleCount()) + " Cycles\n";
  message += "Milliseconds.: " + String(millis()) + " Milliseconds\n";
  server.send(404, "text/plain", message);
}

//*************************************************
//* draw graphics
//* need to be adapted & tested
//*************************************************
/*
void drawGraph() {
  String out = "";
  char temp[100];
  out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"400\" height=\"150\">\n";
  out += "<rect width=\"400\" height=\"150\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
  out += "<g stroke=\"black\">\n";
  int y = rand() % 130;
  for (int x = 10; x < 390; x += 10) {
    int y2 = rand() % 130;
    sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", x, 140 - y, x + 10, 140 - y2);
    out += temp;
    y = y2;
  }
  out += "</g>\n</svg>\n";
  server.send ( 200, "image/svg+xml", out);
}
*/
//*************************************************
//* MAIN PROGRAM LOOP
//*************************************************
void loop()
{
#ifdef USE_OTA_UPGRADE
  //** this for upgrade the firmware OTA
  ArduinoOTA.handle();
#endif
  
  server.handleClient();

  updateTime();

  if (bSendFlag) {
    
#ifdef BLINK_BLUE_LED
  TkBlueLed.attach(0.25, BlinkBlueLed);
#endif

    updateServer();

#ifdef BLINK_BLUE_LED
      TkBlueLed.detach();
      digitalWrite(D4, HIGH);  // Led OFF
#endif
    bSendFlag=false;
  }
  
  if (Serial.available() > 0) {
    car= Serial.read();
    if (car == 'm') {
      while (Serial.read() != '\n') {};
      configMenu();
      ntp();
    }
    // immediately sends datas
    if (car == 'f') {
      while (Serial.read() != '\n') {};
#ifdef BLINK_BLUE_LED
      TkBlueLed.attach(0.25, BlinkBlueLed);
#endif
      updateServer();
#ifdef BLINK_BLUE_LED
      TkBlueLed.detach();
      digitalWrite(D4, HIGH);  // Led OFF
#endif

    }
  }
}

//*************************************************************************
//*** Support function to keep lightweight the Soft Clock interrupt routine
//*************************************************************************
void updateTime()
{ 
  char currentTime[10];

  if (bSecsFlag) {

#ifdef SHOW_TICKS    
    sprintf(currentTime, "%02d:%02d:%02d", dateTime.hour, dateTime.minute, dateTime.second);
    Serial.println(currentTime);
#endif

#ifdef BLINK_RED_LED
  digitalWrite(D0, digitalRead(D0)^1); // turn the ESP-12 LED off and on (HIGH is the voltage level and meaning OFF)
#endif
    
    bSecsFlag=false;
  }
}

//********************************************************************
//*** Sends the packets to desired servers and to log file, if enabled
//********************************************************************
void updateServer()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    printBme();
    calcNextTX();
    if(bNtpSyncFlag) {
      Serial.println("Syncing softclock with NTP time");
      ntp();   // NTP Sync every 12 hours is more than enough
      bNtpSyncFlag=false;
    }
    
    Send2APRS();
#ifdef USE_WUNDER
    Send2Wunder();
#endif
  }
  
  if (station.logger == 1) {
    if (SPIFFS.exists("/logger.txt") == 1) {
      String s;
      //long sizefile;
      File f = SPIFFS.open("/logger.txt", "a");
      if (!f) {
        Serial.println("file open failed");
      }
      Serial.println("====== add data logger =========");
      char buffer[50];
      sprintf(buffer, "%02d/%02d/%04d;", dateTime.day, dateTime.month, dateTime.year);
      f.print(buffer);
      sprintf(buffer, "%02d:%02d:%02d;", dateTime.hour, dateTime.minute, dateTime.second);
      f.print(buffer);
      sprintf(buffer, "%03d;%02d;%05d\n", (int)(wx.temperatureC), (int)(wx.humidity), (int)(wx.pression));
      f.print(buffer);
      f.close();
    }
  }
}

//****************************************************
//* SYNCRONIZE SYSTEM TIME with NTP SERVERS
//* need to be modified, obsolete library in use...
//****************************************************
#define SEND_INTV     10
#define RECV_TIMEOUT  10

void calcNextTX(){

  nextMinTx = (dateTime.minute + station.transmitDelay) % 60;
  nextSecTx = dateTime.second;
  if(nextMinTx<dateTime.minute) (nextHour=dateTime.hour + 1) % 24;
  else nextHour=dateTime.hour;
}

void ntp()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    // first parameter: Time zone in floating point (for India); second parameter: 1 for European summer time; 2 for US daylight saving time (not implemented yet)

    //**** BIG ISSUE: in case of poor connection, we risk to remain in this loop forever
    NTPch.setSendInterval(SEND_INTV);
    NTPch.setRecvTimeout(RECV_TIMEOUT);
    do
    {
      dateTime = NTPch.getNTPtime(1.0, 1);
      delay(1);
    }
    while(!dateTime.valid);
    
    NTPch.printDateTime(dateTime); 
    //dateTime.second = (dateTime.second + RECV_TIMEOUT) % 60;  //adjust the setRecvTimeout delay;
    calcNextTX();
    Serial.print(" ->>>>> next tx at : " );
    char buffer[20];
    sprintf(buffer, "%02d:%02d:%02d", nextHour, nextMinTx, nextSecTx);
    Serial.println(buffer);
   }
}

//****************************************************
//* READ BATTERY VOLTAGE
//* 0.004V resolution for 4.2V (R100K)
//* 0.003V resolution for 3.3V (R10K)
//* If you are using NodeMCU v0.9 then you need
//* a resistor if you wanna measure voltage greater than 3.3V
//* https://forum.arduino.cc/index.php?topic=445538.0
//* 
//****************************************************
unsigned int raw=0;
float volt=0.0;

#ifndef HAVE_BATTERY
  ADC_MODE(ADC_VCC);      //needed for ESP.getVcc()
#endif

unsigned int ReadVBAT (){
  raw = analogRead(A0);
  volt=raw/1023.0;
  //volt=volt*4.2f; //100Kohm resistor
  volt=volt*3.3f;   //10kohm resistor
  return ((unsigned int)(volt*100)); // two decimals after the comma
}
//****************************************************

void Send2APRS()
{
  char login[60];
  char sentence[150];
  unsigned int rndnum=random(1000);  // random number here (telemetry packet ID)
  unsigned int len;     // for padding callsign

  //MANDATORY: CWOP doesn't need password, but need to register to the CWOP program, and obtain a valid callsign
  //sprintf(login, "user %s pass -1 vers VERSION ESP8266", station.callsign); 
  
  sprintf(login, "user %s pass %s vers VERSION ESP8266", station.callsign, AprsPassw); // user must be "yourcallsign-13" if you are an hamradio operator, otherwise request and use a CWOP callsign....
   
  //retrieve telemetry infos
  tl.rssi = abs(WiFi.RSSI());   // strenght of WiFi AP signal

#ifdef HAVE_BATTERY
  tl.vbat = ReadVBAT();         // Read 0-4.2Volts from A0 (if you use 100kohm res)
#else
  tl.vbat = (unsigned int) (ESP.getVcc()*0.1f);   // Read the power voltage of VDD3P3 pin 3 and 4 (in the ESP8266 chip)
#endif

  Serial.print("Connecting to APRS server...");
  int retr=20;
  while (!client.connect(station.clientAdress, station.clientPort) && (retr > 0)) {
    delay(50);
    --retr;
  }
  
  if(!client.connected()){
    Serial.println("connection failed");
    client.stop();
    return;
  }
  else
  {  
    Serial.println("done");
  
    client.println(login);
    Serial.println(login);

    delay(3000); //as reccomended, 3" between login and sends packet
  }
  
  //print server reply
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  // Send Position Lat/Long/Alt Report message
  // WORKAROUND: i'm using an hack, WX Station can not have the 43 chars wide comment field
  // and the ALTITUDE field, so i first send a "red dot icon" position report and after that
  // i change the report icon in WX again....dirty, but fully functional :-)
  Serial.println("** POSITION PACKET **");
  sprintf(sentence, "%s>APRS,TCPIP*:=%s/%s&%s (%s)",station.callsign, 
                                                    station.latitude, 
                                                    station.longitude, 
                                                    APRS_CMNT, SOFT_VER);
  client.println(sentence);
  Serial.println(sentence);

  Serial.println("** WX PACKET **");
  sprintf(sentence, "%s>APRS,TCPIP*:=%s/%s_.../...g...t%03dr...p...P...h%02db%05d", station.callsign, 
                                                                                    station.latitude, 
                                                                                    station.longitude, 
                                                                                    (int)(wx.temperatureF), 
                                                                                    (int)(wx.humidity), 
                                                                                    (int)(wx.pression/10));
  client.println(sentence);
  Serial.println(sentence);

  //print server reply, but the server here doesn't reply at all......
  //delay(500);
  //while (client.available()) {
  //  String line = client.readStringUntil('\r');
  //  Serial.print(line);
  //}

  // adjust callsign to 9 char as requested by tlm packet (see APRS101.pdf)
  strcpy(station.tlm_callsign, station.callsign);
  
  if((len=strlen(station.callsign))<9)
  {
    do{// pad with spaces
      station.tlm_callsign[len++] = 0x20; 
    }
    while (len < 9);
  }

  // Send telemetry sentences, refer to APRS101.pdf
  Serial.println("** TELEMETRY PACKETS **");
  sprintf(sentence, "%s>APRS,TCPIP*:T#%03d,%03d,%03d,000,000,000,00000000", station.callsign, 
                                                                            rndnum, 
                                                                            tl.rssi, 
                                                                            tl.vbat);
  client.println(sentence);
  Serial.println(sentence);

  //Define telemetry parameters (labels)
  sprintf(sentence, "%s>APRS,TCPIP*::%s:PARM.RSSI,VBAT", station.callsign, 
                                                         station.tlm_callsign);
  client.println(sentence);
  Serial.println(sentence);

  //Define telemetry units
  sprintf(sentence, "%s>APRS,TCPIP*::%s:UNIT.dbm,V", station.callsign, 
                                                     station.tlm_callsign);
  client.println(sentence);
  Serial.println(sentence);

  //Add telemetry coefficient so the APRS protocol can convert your raw values into real value.
  sprintf(sentence, "%s>APRS,TCPIP*::%s:EQNS.0,-1,0,0,0.01,0,0,0,0,0,0,0,0,0,0", station.callsign, 
                                                                                 station.tlm_callsign);
  client.println(sentence);
  Serial.println(sentence);

  //Send bits and project comment
  sprintf(sentence, "%s>APRS,TCPIP*::%s:BITS.00000000,%s", station.callsign, 
                                                           station.tlm_callsign, 
                                                           APRS_PRJ);
  client.println(sentence);
  Serial.println(sentence);

  Serial.println();
  Serial.print("closing connection...");
  client.stop();
  Serial.println("closed!");
  
}

void initBme()
{
  //***Driver settings********************************//
  //commInterface can be I2C_MODE or SPI_MODE
  //specify chipSelectPin using arduino pin names
  //specify I2C address.  Can be 0x77(default) or 0x76

  //For I2C, enable the following and disable the SPI section
  mySensor.settings.commInterface = I2C_MODE;
  mySensor.settings.I2CAddress = 0x76;

  //For SPI enable the following and dissable the I2C section
  //mySensor.settings.commInterface = SPI_MODE;
  //mySensor.settings.chipSelectPin = 10;


  //***Operation settings*****************************//

  //renMode can be:
  //  0, Sleep mode
  //  1 or 2, Forced mode
  //  3, Normal mode
  mySensor.settings.runMode = 3; //Normal mode

  //tStandby can be:
  //  0, 0.5ms
  //  1, 62.5ms
  //  2, 125ms
  //  3, 250ms
  //  4, 500ms
  //  5, 1000ms
  //  6, 10ms
  //  7, 20ms
  mySensor.settings.tStandby = 0;

  //filter can be off or number of FIR coefficients to use:
  //  0, filter off
  //  1, coefficients = 2
  //  2, coefficients = 4
  //  3, coefficients = 8
  //  4, coefficients = 16
  mySensor.settings.filter = 0;

  //tempOverSample can be:
  //  0, skipped
  //  1 through 5, oversampling *1, *2, *4, *8, *16 respectively
  mySensor.settings.tempOverSample = 1;

  //pressOverSample can be:
  //  0, skipped
  //  1 through 5, oversampling *1, *2, *4, *8, *16 respectively
  mySensor.settings.pressOverSample = 1;

  //humidOverSample can be:
  //  0, skipped
  //  1 through 5, oversampling *1, *2, *4, *8, *16 respectively
  mySensor.settings.humidOverSample = 1;


  Serial.print("Program Started\n");
  Serial.print("Starting BME280... result of .begin(): 0x");

  //Calling .begin() causes the settings to be loaded
  delay(10);  //Make sure sensor had enough time to turn on. BME280 requires 2ms to start up.
  Serial.println(mySensor.begin(), HEX);
  
#ifdef DISPLAY_BME_REGS
  Serial.print("Displaying ID, reset and ctrl regs\n");

  Serial.print("ID(0xD0): 0x");
  Serial.println(mySensor.readRegister(BME280_CHIP_ID_REG), HEX);
  Serial.print("Reset register(0xE0): 0x");
  Serial.println(mySensor.readRegister(BME280_RST_REG), HEX);
  Serial.print("ctrl_meas(0xF4): 0x");
  Serial.println(mySensor.readRegister(BME280_CTRL_MEAS_REG), HEX);
  Serial.print("ctrl_hum(0xF2): 0x");
  Serial.println(mySensor.readRegister(BME280_CTRL_HUMIDITY_REG), HEX);

  Serial.print("\n\n");


  Serial.print("Displaying all regs\n");
  uint8_t memCounter = 0x80;
  uint8_t tempReadData;
  for (int rowi = 8; rowi < 16; rowi++ )
  {
    Serial.print("0x");
    Serial.print(rowi, HEX);
    Serial.print("0:");
    for (int coli = 0; coli < 16; coli++ )
    {
      tempReadData = mySensor.readRegister(memCounter);
      Serial.print((tempReadData >> 4) & 0x0F, HEX);//Print first hex nibble
      Serial.print(tempReadData & 0x0F, HEX);//Print second hex nibble
      Serial.print(" ");
      memCounter++;
    }
    Serial.print("\n");
  }

  Serial.print("\n\n");

  Serial.print("Displaying concatenated calibration words\n");
  Serial.print("dig_T1, uint16: ");
  Serial.println(mySensor.calibration.dig_T1);
  Serial.print("dig_T2, int16: ");
  Serial.println(mySensor.calibration.dig_T2);
  Serial.print("dig_T3, int16: ");
  Serial.println(mySensor.calibration.dig_T3);

  Serial.print("dig_P1, uint16: ");
  Serial.println(mySensor.calibration.dig_P1);
  Serial.print("dig_P2, int16: ");
  Serial.println(mySensor.calibration.dig_P2);
  Serial.print("dig_P3, int16: ");
  Serial.println(mySensor.calibration.dig_P3);
  Serial.print("dig_P4, int16: ");
  Serial.println(mySensor.calibration.dig_P4);
  Serial.print("dig_P5, int16: ");
  Serial.println(mySensor.calibration.dig_P5);
  Serial.print("dig_P6, int16: ");
  Serial.println(mySensor.calibration.dig_P6);
  Serial.print("dig_P7, int16: ");
  Serial.println(mySensor.calibration.dig_P7);
  Serial.print("dig_P8, int16: ");
  Serial.println(mySensor.calibration.dig_P8);
  Serial.print("dig_P9, int16: ");
  Serial.println(mySensor.calibration.dig_P9);

  Serial.print("dig_H1, uint8: ");
  Serial.println(mySensor.calibration.dig_H1);
  Serial.print("dig_H2, int16: ");
  Serial.println(mySensor.calibration.dig_H2);
  Serial.print("dig_H3, uint8: ");
  Serial.println(mySensor.calibration.dig_H3);
  Serial.print("dig_H4, int16: ");
  Serial.println(mySensor.calibration.dig_H4);
  Serial.print("dig_H5, int16: ");
  Serial.println(mySensor.calibration.dig_H5);
  Serial.print("dig_H6, uint8: ");
  Serial.println(mySensor.calibration.dig_H6);

  Serial.println();
#endif
}

//*** Retrieve values from BME280 and fill in the structure
void getBmeValues(){
    float pres = mySensor.readFloatPressure(); // read pressure where we are (altitude related)

    wx.temperatureC = mySensor.readTempC();
    
    //*** calc pression at sea level (standardized)
    pres= pres * ( pow(1.0 -(0.0065 * (float) station.altitude * -1 /(273.15+wx.temperatureC)), 5.255));
    wx.pression = pres;
        
    wx.temperatureF = mySensor.readTempF();
    wx.humidity =  mySensor.readFloatHumidity();
  
}

//*** Print BME280 values in Serial Monitor
void printBme()
{
  getBmeValues();

  float pres = mySensor.readFloatPressure(); // read pressure where we are (altitude related)

#ifdef DISPLAY_BME_VAL
  Serial.print("Pressure at home level: ");
  Serial.print((pres/100), 2);//(Pa->mBar)
  Serial.println(" Pa");

  Serial.print("Pressure at sea level: ");
  Serial.print((wx.pression/100), 2);//(Pa->mBar)
  Serial.println(" Pa");


  Serial.print("Temperature: ");
  Serial.print(mySensor.readTempC(), 2);
  Serial.println(" degrees C");

  Serial.print("Temperature: ");
  Serial.print(mySensor.readTempF(), 2);
  Serial.println(" degrees F");
  
  Serial.print("%RH: ");
  Serial.print(mySensor.readFloatHumidity(), 2);
  Serial.println(" %");

  Serial.println();
#endif
}

//****************************************************************************
//* WHOLE SYSTEM MENU
//****************************************************************************

byte detectMenu()
{
  long previousMillisSerial = 0;
  long currentMillisSerial;
  long EcratMillisSerial;
  int countDown = 0;
  Serial.println(F("m for boot menu"));
  previousMillisSerial = millis();
  do {
    currentMillisSerial = millis();
    EcratMillisSerial = currentMillisSerial - previousMillisSerial;
    if (Serial.available() > 0) {
      if (Serial.read() == 'm') {
        while (Serial.read() != '\n') {};
        return 1;
      }
    }
    if ((EcratMillisSerial / 1000) != countDown) {
      countDown++;
      Serial.write(countDown + 0x30);
    }
  }
  while (EcratMillisSerial < 10000);
  Serial.println();
  return 0;
}

void configMenu()
{
  char carMenu;
  do {
    carMenu = 0;
    Serial.println(F("-----------"));
    Serial.println(F("Config menu"));
    Serial.println(F("0 Quit menu"));
    Serial.println(F("1 format file system"));
    Serial.println(F("2 config wifi access point"));
    Serial.println(F("3 config weather station"));
    Serial.println(F("4 test ntp"));
    Serial.println(F("5 test bme 280"));
    Serial.println(F("6 test server upload"));
    Serial.println(F("7 print weather data logger (historic)"));
    Serial.println(F("8 create and erase weather data logger"));
    Serial.println(F("-----------"));
    carMenu = readCarMenu();
    switch (carMenu) {
      case '1' :
        Serial.println("Please wait 30 secs for SPIFFS to be formatted");
        SPIFFS.format();
        Serial.println("Spiffs formatted");
        break;
      case '2' :  configAcessPoint();
        break;
      case '3' :  configWeather();
        break;
      case '4' :  ssidConnect(); 
                  ntp(); //prévoir un test de connexion
        break;
      case '5' :  initBme(); 
                  printBme();
        break;
      case '6' :  initBme(); 
                  printBme();  
                  ssidConnect(); 
                  ntp; 
                  Send2APRS();
#ifdef USE_WUNDER
                  Send2Wunder();
#endif
        break;
      case '7' :  showlogger();
        break;
      case '8' :  createEraselogger();
        break;
      case '0' :
        break;
      default : Serial.println(F("error"));
    }
  } while (carMenu != '0');
}

void configAcessPoint()
{
  if (SPIFFS.exists("/ssid.txt") == 1) {
    readSsidFile();
  }
  else
  {
    Serial.println(F("no ssid config file"));
  }
  char carMenu;
  do {
    carMenu = 0;
    Serial.println(F("-----------"));
    Serial.println(F("Config wifi access point menu"));
    Serial.println(F("0 Save and exit acess point menu"));
    Serial.println(F("1 ssid list"));
    Serial.println(F("2 set ssid"));
    Serial.println(F("3 set ssid password"));
    Serial.println(F("4 show ssid config"));
    Serial.println(F("5 test ssid"));
    Serial.println(F("-----------"));
    carMenu = readCarMenu();
    switch (carMenu) {
      case '1' :
        wifiScan();
        break;
      case '2' :
        Serial.println(F("type your ssid"));
        readCharArray(internet.ssid);
        break;
      case '3' :
        Serial.println(F("type your password"));
        readCharArray(internet.password);
        break;
      case '4' :
        Serial.println(F("your wifi ssid config is"));
        Serial.println(internet.ssid);
        Serial.println(internet.password);
        break;
      case '5' :
        Serial.println(F("test ssid internet access"));
        ssidConnect();
        break;
      default : Serial.println(F("error"));
    }
  } while (carMenu != '0');
  writeSsidFile();
}

void configWeather()
{
  if (SPIFFS.exists("/station.txt") == 1) {
    readStationFile();
  }
  else
  {
    Serial.println(F("no station config file"));
  }
  char carMenu;
  char buffer[10];
  char len;
  do {
    carMenu = 0;
    Serial.println(F("-----------"));
    Serial.println(F("Config weather station"));
    Serial.println(F("0 Save and exit weather station menu"));
    Serial.println(F("1 set callsign station"));
    Serial.println(F("2 set longitude"));
    Serial.println(F("3 set latitude"));
    Serial.println(F("4 set altitude"));
    Serial.println(F("5 set server address"));
    Serial.println(F("6 set server port"));
    Serial.println(F("7 set transmit delay"));
    Serial.println(F("8 logger enable"));
    Serial.println(F("9 show weather config"));
    Serial.println(F("-----------"));
    carMenu = readCarMenu();
    switch (carMenu) {
      case '1' :
        Serial.println(F("type your callsign station ex: FWxxxx"));
        len=readCharArray(station.callsign);
        break;
      case '2' :
        Serial.println(F("type your longitude ex: 00012.21E"));
        readCharArray(station.longitude);
        break;
      case '3' :
        Serial.println(F("type your latitude ex: 4759.75N"));
        readCharArray(station.latitude);
        break;
      case '4' :
        Serial.println(F("type your altitude (meters) ex: 78"));
        readCharArray(buffer);
        station.altitude=atoi(buffer);
        break;
      case '5' :
        Serial.println(F("type your server address, default : cwop.aprs.net"));
        readCharArray(station.clientAdress);
        break;
      case '6' :
        Serial.println(F("type your server port, default : 14580"));
        readCharArray(buffer);
        station.clientPort = atoi(buffer);
        break;
      case '7' :
        Serial.println(F("type transmit delay, default 10 minutes"));
        readCharArray(buffer);
        station.transmitDelay = atoi(buffer);
        break;
      case '8' :
        Serial.println(F("logger enable 0/1, defaut 0"));
        readCharArray(buffer);
        station.logger = atoi(buffer);
        break;
      case '9' :
        Serial.print(F("callsign : "));
        Serial.println(station.callsign);
        Serial.print(F("longitude : "));
        Serial.println(station.longitude);
        Serial.print(F("latitude : "));
        Serial.println(station.latitude);
        Serial.print(F("altitude : "));
        Serial.println(station.altitude);
        Serial.print(F("server address : "));
        Serial.println(station.clientAdress);
        Serial.print(F("server port : "));
        Serial.println(station.clientPort);
        Serial.print(F("tx delay : "));
        Serial.println(station.transmitDelay);
        Serial.print(F("logger enable : "));
        Serial.println(station.logger);
        break;
      case '0' :
        break;
      default : Serial.println(F("error"));
    }
  } while (carMenu != '0');
  writeStationFile();
}

//********************************
//* READ CHARS FROM PC SERIAL COMM
//********************************
int readCharArray(char *buffer)
{
  char car;
  int ptr = 0;
  
  do
  {
    if (Serial.available() > 0) {
      car = Serial.read();
      if (car != '\n') {
        buffer[ptr++] = car;
      }
    }
  }
  while (car != '\n');
  
  buffer[ptr] = 0;
  // return the number of char read
  return ptr;
}
//********************************

char readCarMenu()
{
  char car = 0;
  char ret = 0;
  while (car != '\n')
  {
    if (Serial.available() > 0) {
      car = Serial.read();
      if ((car >= '0') && (car <= '9')) {
        ret = car;
      }
    }
  }
  return ret;
}

//******************************************
//* WiFi Scan (every parameter i can obtain)
//******************************************
void wifiScan()
{
  Serial.println(F("scan start"));
  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println(F("scan done"));
  if (n == 0)
    Serial.println(F("no networks found"));
  else
  {
    Serial.print(n);
    Serial.println(F(" networks found"));
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(F(": "));
      Serial.print(WiFi.SSID(i));
      Serial.print(F(" ("));
      Serial.print(WiFi.RSSI(i));
      Serial.print("dbm");
      Serial.print(F(")"));
      Serial.print(F(" CH:"));
      Serial.print(WiFi.channel(i));
      Serial.print(F(" BSSID:"));
      Serial.print(WiFi.BSSIDstr(i));
      switch(WiFi.encryptionType(i))
        {
          case ENC_TYPE_WEP:
            Serial.println(F(" WEP "));
            break;
          case ENC_TYPE_TKIP:
            Serial.println(F(" WPA/PSK "));
            break;
          case ENC_TYPE_CCMP:
            Serial.println(F(" WPA2/PSK "));
            break;
          case ENC_TYPE_NONE:
            Serial.println(F(" OPEN "));
            break;
          case ENC_TYPE_AUTO:
            Serial.println(F(" WPA/WPA2/PSK "));
            break;
        }
      delay(10);
    }
  }
  Serial.println("");
}

void writeSsidFile()
{
  File f = SPIFFS.open("/ssid.txt", "w+");
  if (!f) {
    Serial.println(F("file open failed"));
    return;
  }
  Serial.println(F("====== Writing to ssid.txt file ========="));
  for (int i = 0; i < sizeof(configStruct); i++) {
    f.write(*((char*)&internet + i));
  }
  f.close();
  return;
}


void readSsidFile()
{
  File f = SPIFFS.open("/ssid.txt", "r+");
  if (!f) {
    Serial.println(F("file open failed"));
    return;
  }
  Serial.println(F("====== Reading ssid.txt file ========="));
  for (int i = 0; i < sizeof(configStruct); i++) {
    *((char*)&internet + i) = f.read();
  }
  f.close();
  return;
}

void writeStationFile()
{
  File f = SPIFFS.open("/station.txt", "w+");
  if (!f) {
    Serial.println(F("file open failed"));
    return;
  }
  Serial.println(F("====== Writing to station.txt file ========="));
  for (int i = 0; i < sizeof(positionStruct); i++) {
    f.write(*((char*)&station + i));
  }
  f.close();
  return;
}

void readStationFile()
{
  File f = SPIFFS.open("/station.txt", "r+");
  if (!f) {
    Serial.println(F("file open failed"));
    return;
  }
  Serial.println(F("====== Reading station.txt file ========="));
  for (int i = 0; i < sizeof(positionStruct); i++) {
    *((char*)&station + i) = f.read();
  }
  f.close();
  return;
}

void createEraselogger()
{
  File f = SPIFFS.open("/logger.txt", "w");
  if (!f) {
    Serial.println("file open failed");
  }
  Serial.println("====== new logger file =========");
  f.println("date;time;temperature;humidity;pressure");
  f.close();
}

void showlogger()
{
  if (SPIFFS.exists("/logger.txt") == 1) {
    String s;
    //long sizefile;
    File f = SPIFFS.open("/logger.txt", "r");
    if (!f) {
      Serial.println("file open failed");
    }
    //sizefile=f.size()-42;
    Serial.println("====== read logger file =========");
    do {
      s = f.readStringUntil('\n');
      Serial.println(s);
    }
    while (s.length() > 0);
    f.close();
  }
}

void ssidConnect()
{
  Serial.println(internet.ssid);
  Serial.println(internet.password);
  
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(F("Connecting to "));
    Serial.println(internet.ssid);
    WiFi.persistent(false);       // WiFi config isn't saved in flash
    //WiFi.disconnect();             
    WiFi.hostname(WiFi_hostname);
    WiFi.mode(WIFI_STA);          // use WIFI_AP_STA if you want an AP
    WiFi.begin(internet.ssid, internet.password);

//****************************************************
//* STATIC IP DEFINITION, comment these lines for DNS
//****************************************************
#ifdef USE_STATIC_IP
    ip.fromString(stat_ip);        // STATIC IP
    gateway.fromString(stat_gateway);   // GATEWAY
    subnet.fromString(stat_subnet);    // SUBNET MASK
    dns1.fromString(stat_dns1);            // DNS1
    dns2.fromString(stat_dns2);            // DNS2
    WiFi.config(ip, gateway, subnet, dns1, dns2);
#endif
//****************************************************

    // Endlessly Wait for connection
    // WILL BE CHANGED IN BATTERY POWERED VERSION (use of deepsleep() after 'x' retries..)
    while (WiFi.status() != WL_CONNECTED) {
       delay(500);
      Serial.print(F("."));
     }
  }
     
    Serial.println();
    Serial.print(F("Connected to "));
    Serial.println(internet.ssid);
    Serial.print(F("IP address: "));
    Serial.println(WiFi.localIP());
}
