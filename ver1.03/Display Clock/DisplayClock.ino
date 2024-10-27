/************VERY IMPORTANT COMPILING INFORMATION**************************
 * This program is compiled with NodeMCU-32S in Arduino IDE 
 * Ver 2.3.2. Use Board Manager in IDE search for ESP32 
 * and ensure esp32 by Espressif Systems Version 2.0.17 
 * installed. Do not update to Ver 3.0 as I2S changes 
 * in the version are not compatible with this program. If 3.0 is installed, downgrade it to 2.0.17
 * A compiled bin is stored with the program. 
 *
 * Check URL for older versions:
 *   https://sourceforge.net/projects/arduino-core-esp32.mirror/files/2.0.17/
 * 
 * Another method (Not tried)
 * A zip file 'esp32-2.0.17.zip' is stored in Additional files directory
 *
 * Unzip this file which will unzip to folder esp32-2.0.17
 * Rename the folder to 2.0.17 and copy it to the directory:
 * C:\Users\vdeva\AppData\Local\Arduino15\packages\esp32\hardware\esp32
 *
 * The main display is based on Development code from the website:
 * https://www.waveshare.com/wiki/RGB-Matrix-P2.5-64x64
 *
 * Libraries used:
 * For ESP32/ESP8266 libraries Use Arduino IDE Preference/Additional Board libraries
 * and have these two links saved separated by a comma: 
 *  http://arduino.esp8266.com/stable/package_esp8266com_index.json,
 *  https://dl.espressif.com/dl/package_esp32_index.json
 *  
 * Adafruit GFX Library
 * Adafruit BusIO
 * Arduino_JSON
 * RGB Matrix Panel by Adafruit
 * ezTime by RopGonggrijp https://github.com/ropg/ezTime
 * DallasTemperature by Miles Burton 
 * OneWire by Jim Studt. Tom Pollard... https://www.pjrc.com/teensy/td_libs_OneWire.html
 * ESP32 Hub75 LED Matrix Panel DMA Dispay: https://github.com/mrcodetastic/ESP32-HUB75-MatrixPanel-DMA
 *
 * ElegantOTA is used 
 * Details of ElegantOTA use the library by Ayush Sharma URL:
 *     https://github.com/ayushsharma82/ElegantOTA - 
 * ElegantOTA is always at localhost:/update. 
 * There are 11 files that are to be stored locally apart from .ino files
 * The display resolution and dma buffers are handled by these 11 files. 
 * Leave them in the same directory as the INO files.
 * Version Changes:
 * Ver 1000 - Dated: 24/08/2024
 * Copied from Scoreboard Version 1016. Modified for Display clock
  * Removed Scoreboard options and replaced with Display option 
 * Telnet removed. No need to use this. We will use web interface for logs
 * Ver 1001 - Date started:26/08/2024 Completed: 
 * Little FS is used for storing downloaded files. Panchang data downloaded from web from this version +
 * User defined Display Page incorporated 
 * Ver 1002 - Date started: 20/09/2024 Completed: 15/09/2024
 * Incorporating config page and provide options for two additional clocks instead of weather on 3rd line of display
 * Ver 1.03 (10003) - Major changes to include multiple versions of Panchangam and additional data to be used as
 * Scroller text. Configuration file config.txt is the only file saved locally. See Specification.txt
 * for ver 1.03 details. completed on 7/10/24. Testing with display pending.
 ****************************************************************************************** */

//Select Either One
#define PROD
 
#define ESP32D //The new ESP32-WROOM-32D has RGB interchanged this is with type-C connector
//#define EZTIME_NETWORK_ENABLE
#include <ezTime.h> //Manage Date / Time
#include "Style.h"
#include <WiFi.h>        //ESP8266 WiFi
#include <WebServer.h>
#include <ESPmDNS.h>      //Needed for Web server
#include <EEPROM.h>             //WiFi info are stored. Up to 6 networks can be stored
//#include <WiFiClientSecure.h>
#include <Arduino_JSON.h>       //JSON library
#include <HTTPClient.h>
#include "ESP32-HUB75-MatrixPanel-I2S-DMA.h"

//Following are required for Temperature Sensor
#include <OneWire.h>
#include <DallasTemperature.h>

//Include different fonts here Fonts are at "C:\Users\vdeva\AppData\Local\Arduino15\libraries\Adafruit-GFX-Library\Fonts"
//#include "FreeSans9pt7b.h"
//#include "FreeSans18pt7b.h"
//#include "Panch2024-25.h"
#include <ElegantOTA.h> 
#include <WebSocketsServer.h>

#ifdef ESP32D
#define SERVERTRIGGER 35     // Trigger Server Request 
#else
#define SERVERTRIGGER 34 
#endif

//File System for Calendar storage
#include <FS.h>
#include <LittleFS.h>
//Internal House Keping Declarations
//WiFi Client Connection change if too low
#define WHILE_LOOP_DELAY  200   //Connection Wait time
#define MAXNETWORK 6            //stored network
#define SECOND 1000
#define WIFI_TIMEOUT 8         //Time out in secs for Wifi connection. 

#define ONE_WIRE_BUS 21          // Temperature Sensor
#define RETRY SECOND * 2        //Retry for network intervel
#define NETWORKOFFSET 100; //From Ver 1009, Network stored in a separate space not to interfere with stored Network data
#define VERSION 10003 //Change from Scoreboard

#define PROGID 0x4443  //Hex for DC - display clock
#define SENSORCOMP 3
#define CALINDEX 1
#define CALVER 10001

//parameters for getting time and Weather
String Location; //to be evaluated as : "latitude=-34.47&longitude=150.42&timezone=Australia/Sydney  / is replaced with %2F
String TimeZone; //Used by Clock and also WUri
char CLOCKZONES[] = "/clockzones.txt";
char CALCODE[] = "tampp";
char CONFIG[] = "/config.txt";
//Provides GatewayIP, location and Time zone. 
String LocationUri = "http://ipinfo.io/?token=";
String LocationKey = "81603eda566497";  //to be deleted before posting on public domain

//Do not call WUri without setting Location & TimeZone. Call Getlocation first
//Parameters needed are: // + Location  + TimeZone + "&current=temperature_2m,rain,cloud_cover&hourly=temperature_2m,rain,
// cloud_cover&daily=weather_code,temperature_2m_max,temperature_2m_min,rain_sum&forecast_days=1";
//These are filled up in WeatherUri - split is required to ensure if the location changes, it is accommodated

String WUri = "http://api.open-meteo.com/v1/forecast?"; 
String WeatherUri = ""; 

//End of Clock, Weather and Tamil Calendar declaration

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);
char tempCString[7];
		

struct HEADER
{
  uint16_t id; //Program ID this will not change
  uint16_t ver; //Program version
  uint16_t calIndex; //Index into calendar
  char calCode[6];
  uint16_t calVer;
  uint8_t  bDisplay;
  uint8_t bClock;
} header;


bool bDisplay = false; //Determine whether clock (false) or user defined text(true) to be displayed
bool bClock = false; //use Weather(false) instead of dual time zones(true)
const char WIFIPWD[9] = "pass1234";  //Host password - None
uint16_t pVer;

//LAN NOLANbyte array is used for displaying the signal 
// strength / connection status at the top right corner

struct NETWORK
{
    char SSID[33];
    char Password[33];
};

//WiFi, UDP, Server & DNS. Change constants as per requirement
WebServer server(80);// Web server
#ifdef PROD
IPAddress ESPIP(172, 20, 30, 1);  //192.168.18.1 Defalt server IP. Safe on class C Network //test network - 192.168.17.1 - Production: 192.168.18.1
IPAddress dhcp(172, 20, 30, 5);
char ESPSSID[] = "DISPCLOCK";
const char HOSTNAME[] = "displayclock";
#else
IPAddress ESPIP(172, 20, 31, 1);  //192.168.18.1 Defalt server IP. Safe on class C Network //test network - 192.168.17.1 - Production: 192.168.18.1
IPAddress dhcp(172, 20, 31, 5);
char ESPSSID[] = "DCTEST";
const char HOSTNAME[] = "dctest";
#endif
IPAddress netMask(255, 255, 255, 0); //by using 0 in 4th octet, (254 - 5) network connectivity possible
NETWORK Networks[MAXNETWORK];
WiFiClientSecure secureClient;

//Internal Network Variables
uint8_t storedNetworks = 0;
int storageIndex;

//Connection status of Server / client and WiFi retry 
bool bConnect = false;
bool bCaptive = false;

//End of Network Declarations


//Panel Display Definitions
//MatrixPanel_I2S_DMA dma_display;
MatrixPanel_I2S_DMA* dma_display = nullptr;

#define E_PIN_DEFAULT  32  //Required for 64x64 Matrix
#define PANEL_RES_X 64     // Number of pixels wide of each INDIVIDUAL panel module. 
#define PANEL_RES_Y 64     // Number of pixels tall of each INDIVIDUAL panel module.
#define PANEL_HCHAIN 2      // Total number of panels chained one to another
#define SINGLEX 6
#define DOUBLEX 12
#define SINGLEY 8
#define DOUBLEY 16
#define PANEL_YCHAIN 1


#define SCROLLX 1 //Always at the last line
#define SCROLLY (PANEL_RES_Y * PANEL_YCHAIN) -1 - SINGLEY //64-1-8 = @55 on a single panel //Top margin 1 pixel
#define DISPLAYAREA  SCROLLY -1  //55-1 @54  We will give one pixel for top line
#define MAXLINES 7

//Note Color565 is defined as 5 bit for Red, 6 bit for Green and 5 bit for Blue. Hence R&B can have only values 0~32 while Green 0~64
uint16_t myBLACK = dma_display->color565(0, 0, 0);
uint16_t myWHITE = dma_display->color565(255, 255, 255);
uint16_t myRED = dma_display->color565(255, 0, 0);
uint16_t myGREEN = dma_display->color565(0, 255, 0);
uint16_t myBLUE = dma_display->color565(0, 0, 255);
uint16_t myYELLOW = dma_display->color565(255,255, 0);
uint16_t myGRAY = dma_display->color565(75, 75, 75);

uint16_t colorWheel(uint8_t pos)
{
    if (pos < 85)
    {
        return dma_display->color565(pos * 3, 255 - pos * 3, 0);
    }
    else if (pos < 170)
    {
        pos -= 85;
        return dma_display->color565(255 - pos * 3, 0, pos * 3);
    }
    else
    {
        pos -= 170;
        return dma_display->color565(0, pos * 3, 255 - pos * 3);
    }
}

uint8_t Xaxis = 2;
uint8_t Yaxis = 2;
uint8_t Tline = 0;

//End of Display Panel Declarations

//Global Variables Used by Calendar
struct TIME
{
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hour;
    uint8_t date;
    uint8_t ampm;
}dtTime;


String curTemp;
String webTemp = ""; //If sensor fails, this is used
String Rain;
uint8_t Cloud;
uint8_t Humidity;
int Windspeed;
String TamilCalendar;

String Weather = "";
//bool bDayChange = false;
uint8_t bDay;
Timezone ClkTZ;
Timezone Ad1TZ;
Timezone Ad2TZ;
char tzID1[3];
char tzID2[3];

//End of Calendar Variables Declaration

/*
//Background Task Variable
struct WEBPARAM
{
  //Web Request Parameters
  MatrixPanel_I2S_DMA* dma_display;
  int Xaxis;
  int Yaxis;
  int scrollSpeed;
  int textSize;
  char win[10];
  //Result Variables
}webParam; */

TaskHandle_t bgTask = null;
bool bNetUpdate = false;
bool bNetSense = false;
//End of Background Task Variables


//Timing Variables change as per requirement
#define TIMEDELAY  30000 //30 secs will be used in multiples for next fetchunsigned long weatherTime = 0;
#define SCROLLSPEED 40   //40 milli second. Changed from 80 milli second

//Variables for Time Delays
#define WEATHERTIME TIMEDELAY * 50 //25 minutes
#define CLOCKINTERVEL TIMEDELAY * 20 //10 minutes
#define TEMPSENSOR TIMEDELAY * 10 //5 minutes
#define RESETDELAY TIMEDELAY * 30 //15 minutes


//variables associated with Timing
unsigned long weatherTime;
unsigned long clockIntervel;
unsigned long tempSense; //Time to check Temperature
unsigned long resetDelay;

const int ledPin = 2;
String logFile = "";

WebSocketsServer ws = WebSocketsServer(81); 
bool bState = false; //need to be removed. Used by Sample LED screen page

//Variables for Panchangam Requirement
bool bFS = false;
char DEFAULTDATA[] = "No Panchangam Data Available";
bool bMonthChange = true; 
bool bDataAvailable = false;
char PANCHANGAMURL[] = "https://panchangam.brahas.com/tcalendar/ver103";;
char VERSIONFILE[] = "/version.txt";
char DISPLAYTEXT[] = "/display.txt";
char CALENDARFILE[] = "/calendar.txt";
char DISPCONFIG[] = "Display Text Config. Total Lines:"; //First line in display.txt
bool bVersionChange = false;
bool bDisplayChange = false;
String DisplayData;
String scrollText;


void setup() 
{
  pinMode(ledPin, OUTPUT);
  BlinkLED();
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  DS18B20.begin();

  PanelInit();  //Show Initial Screen
  #ifdef PROD
    Serial.println ("Working in PROD mode");
  #else
    Serial.println ("Working in TEST mode");
  #endif
  Serial.println("Configuring access point...");

  pinMode(SERVERTRIGGER, INPUT); //Monitor for Server Trigger  logInfo = "";
  //pinMode(SERVERTRIGGER, INPUT_PULLDOWN); 
  if(!LittleFS.begin(true))
  {
    Serial.println("LittleFS Mount Failed");
    logFile += "LittleFS Mount Failed\n";
  }
  else
  {
    Serial.printf("Total heap: %d\n",ESP.getHeapSize());
    Serial.printf("Free heap: %d\r\n",ESP.getFreeHeap());
    //Serial.printf("Total PSRAM: %d\r\n",ESP.getPsramSize());
    //Serial.printf("Free PSRAM: %d\r\n",ESP.getFreePsram());
    bFS = true;
    //ListDir("/",2);
  }

  WiFi.softAPConfig(ESPIP, ESPIP, netMask, dhcp);
  WiFi.softAP(ESPSSID, WIFIPWD);
  delay(500);
  Serial.print("ESP Device IP address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/settingssave",handleSettingsSave);
  server.on("/wifisave", handleWifiSave);
  server.onNotFound(handleNotFound);
  server.on("/savedata", handleSaveData);

  //Following is for moving from LED to Captive & viceVersa
  server.on("/captive",handleRoot);
  server.on("/WebPage",handleWeb);

  if (loadCredentials()) // Load WLAN credentials from EPROM if not valid or no network stored then we need to start webserver
    connectWifi();
  else
  {
    Serial.println("Unable to continue");
    delay(30000);
    ESP.restart();
  }
  
  if (!bConnect)
  {
    server.on("/", handleRoot);
    InitVariables(0);
  }
  else  
    server.on("/", handleWeb);

  startServer();
  ElegantOTA.begin(&server); 
  MDNS.addService("ws", "tcp", 81);
  ws.begin();
  ws.onEvent(webSocketEvent);   

  if (!bConnect)
  {
    bCaptive = true;
    logFile += "No Connectivity. Wifi (Captive) mode in Operation";
    Serial.println("No Connectivity. Wifi (Captive) mode in Operation");
  }
  else
  {
    if (bFS)
    {
      if (!LittleFS.exists(DISPLAYTEXT))
      {
        StoreFileFromWeb(DISPLAYTEXT,false);
      }
      if (!LittleFS.exists(CLOCKZONES))
      {
        StoreFileFromWeb(CLOCKZONES,false);
      }

    }
    //ListDir("/",2);
    InitVariables(0);
    //RemoveDir("/2025");  //used for testing
    CheckVersionChange();
    setInterval(CLOCKINTERVEL); //Check clock every 10 minutes
    events();
    bCaptive = false;
    dma_display->fillScreen(dma_display->color444(0, 0, 0));
    dma_display->setTextSize(1);
    dma_display->setCursor(2, 12);
    dma_display->setTextColor(myGREEN);
    dma_display->setTextWrap(false);
    dma_display->println("System configuring.");
    dma_display->setCursor(2, 24);
    dma_display->println("Please Wait");
    dma_display->setCursor(2, 34);
    dma_display->println("Checking Internet...");
    dma_display->fillScreen(dma_display->color444(0, 0, 0));
    dma_display->setTextSize(1);
    dma_display->setCursor(2, 12);
    dma_display->setTextColor(myGREEN + myBLUE);
    

    if (bDisplay) 
    {
      Serial.println("Display mode selected.");
      dma_display->println("Getting your Messages");
      dma_display->setCursor(2, 24);
      dma_display->println("Please Wait...");
      GetLocation(); //Only for information. Can proceed even without web link
    }
    else
    {
      Serial.println("Connected. Calendar mode in Operation");
      dma_display->println("Getting Location");
      dma_display->setCursor(2, 24);
      dma_display->println("Please Wait...");

      if (!GetLocation()) //Force Web Captive server - No connection. cannot proceed
      {
        dma_display->setCursor(2, 44);
        dma_display->println("No Internet found");
        dma_display->setCursor(2, 54);
        dma_display->println("Starting Web Server");
        delay(SECOND * 5);
        handleRoot();
        return;
      }
      else
      {
        dma_display->setCursor(2, 44);
        dma_display->println("Getting Weather");
      }
    }
    //Wait 5 seconds before starting display process. This is required to ensure all variables from web are obtained
    //Change this value if it is further delayed to get details from web. Common for both Score and calendar. 
    delay(SECOND * 5);
    GetWeather();
    CheckCalendarVersion();//Must be checked everyday
    TamilCalendar = GetTC(ClkTZ.dateTime("Ymd"));
 
    //Last of display handling 
    dma_display->fillScreen(dma_display->color444(0, 0, 0)); //All display activities will be now handled by processor 1
    scrollText = "Connecting *** ";
    GetTemperature();    

    /*webParam.dma_display = dma_display;
    webParam.scrollSpeed = SCROLLSPEED;
    webParam.Xaxis = Xaxis;
    webParam.Yaxis = Yaxis;
    webParam.textSize = 1;
    memset(webParam.win, '\0', sizeof(webParam.win));*/
    
    if (bDisplay)
    {
        xTaskCreatePinnedToCore(DispReq, "bgTask", 10000, (void*)NULL, 1, &bgTask, 1);
    }
    else
    {
        xTaskCreatePinnedToCore(ClkReq, "bgTask", 15000, (void*)NULL, 1, &bgTask, 1);
    }
  }

  Serial.printf("Connection Status: %s\n",bConnect ? "TRUE" : "FALSE");

  BlinkLED();
  if (!bConnect)
    resetDelay = millis();

  //For Testing
  //Serial.printf("Panchangam obtained for 1 Dec 2024: %s\n",GetTC("20250101").c_str());
  //ListDir("/",3);



  //Checking for Amavasya and Pournima data
  //Serial.printf("Checking for Amavasya data: %s\n", GetTC("20240902").c_str());
  //Serial.printf("Checking for Pournima data: %s\n", GetTC("20240918").c_str());
  //Serial.printf("Checking for Krishna data: %s\n", GetTC("20240919").c_str());
  //Serial.printf("Checking for Shukla data: %s\n", GetTC("20240903").c_str());

  bDay = ClkTZ.day();
  weatherTime = millis();
  tempSense = millis();

  //Serial.println("After deleting reset file");
  //ListDir("/",2);  


}

String InitVariables(int index)
{
  if (bFS)
  {
    DisplayData = GetData(DISPLAYTEXT, -1);
    String clz = GetData(CLOCKZONES,-1);
    //Serial.printf("ClockZone File contents: %s\n",clz.c_str());
    String TZ;
    if (clz != "")
    {
      int in = clz.indexOf(';');
      if (in > 0)
      {
        TZ = clz.substring(0,in);
        strcpy(tzID1,TZ.substring(0,2).c_str());
        TZ = TZ.substring(3,TZ.length());
        //Serial.printf("First TZ ID: %s and TZ string: %s\n",tzID1,TZ.c_str());

        if (!Ad1TZ.setLocation(TZ))
          logFile += "Unable to Set location for ID: " + String (tzID1) + ": " + TZ + "\n";    
        else
          Serial.println("Succesfully set location for tzID1");  

        if (index == 1)  
          return TZ;
      }
      int sec = clz.indexOf(';',in+1);
      if (sec > 0)
      {
        TZ = clz.substring(in+1,sec);
        strcpy(tzID2,TZ.substring(0,2).c_str());
        TZ = TZ.substring(3,TZ.length());
        //Serial.printf("Second TZ ID: %s and TZstring: %s\n",tzID2,TZ.c_str());

        if (!Ad2TZ.setLocation(TZ))
          logFile += "Unable to Set location for ID: " + String (tzID2) + ": " + TZ + "\n";    
        else
          Serial.println("Succesfully set location for tzID2"); 

        if (index == 2)
          return TZ;  
      }
    } 
    logFile += DisplayData;
    logFile += "Additional Clock Zone Settings:\n" + String(tzID1) + ": " + Ad1TZ.getOlson() + "\n" + String(tzID2) + ": " + Ad2TZ.getOlson() + "\n";
  }
  return String();
}


/* Copied from original example. Used as a test for all the LEDs */
static bool PanelInit()
{
  // Module configuration
  HUB75_I2S_CFG mxconfig(
      PANEL_RES_X,   // module width
      PANEL_RES_Y,   // module height
      PANEL_HCHAIN    // Chain length   
  );

  mxconfig.gpio.e = 32;
  mxconfig.clkphase = false;
  mxconfig.driver = HUB75_I2S_CFG::FM6124;

  // Display Setup
  dma_display = new MatrixPanel_I2S_DMA(mxconfig);
  dma_display->begin();
  dma_display->setBrightness8(90); //0-255
  dma_display->clearScreen();
  dma_display->fillScreen(myWHITE);

  // fix the screen with green
  dma_display->fillRect(0, 0, dma_display->width(), dma_display->height(), dma_display->color444(0, 15, 0));
  delay(500);

  // draw a box in yellow
  dma_display->drawRect(0, 0, dma_display->width(), dma_display->height(), dma_display->color444(15, 15, 0));
  delay(500);

  // draw an 'X' in red
  dma_display->drawLine(0, 0, dma_display->width() - 1, dma_display->height() - 1, dma_display->color444(15, 0, 0));
  dma_display->drawLine(dma_display->width() - 1, 0, 0, dma_display->height() - 1, dma_display->color444(15, 0, 0));
  delay(500);

  // draw a blue circle
  dma_display->drawCircle(10, 10, 10, dma_display->color444(0, 0, 15));
  delay(500);

  // fill a violet circle
  dma_display->fillCircle(40, 21, 10, dma_display->color444(15, 0, 15));
  delay(500);

  // fill a violet circle
  dma_display->fillCircle(70, 31, 10, dma_display->color444(8, 12, 12));
  delay(500);

  // fill a violet circle
  dma_display->fillCircle(100, 41, 10, dma_display->color444(12, 15, 0));
  delay(500);
  return true;
}

void BlinkLED()
{
  for (int i = 0; i < 10 ; i++)
  {
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
    delay(100);
  }

}

void GetTemperature() 
{
  float tempC;
  int count = 0;
  do 
  {
    DS18B20.requestTemperatures(); 
    tempC = DS18B20.getTempCByIndex(0);
    delay(100);
    if (count >10) //Do not Wait for more than 1 second
      break;
    else
      count ++;

  } while (tempC == 85.0 || tempC == (-127.0)); 
    
  char timeStr[20];     
  sprintf(timeStr, "%02d-%02d-%02d %02d:%02d:%02d", ClkTZ.year(), ClkTZ.month(), ClkTZ.day(), ClkTZ.hour(), ClkTZ.minute(), ClkTZ.second());
 
  if (count < 10|| (tempC > -127.0  && tempC < 85.0) )
  {
    dtostrf(tempC - SENSORCOMP, 2, 1, tempCString);
    curTemp = tempCString;

    Serial.printf("Sensor Temperature: %s\n",curTemp);
    sprintf(timeStr, "%02d-%02d-%02d %02d:%02d:%02d", ClkTZ.year(), ClkTZ.month(), ClkTZ.day(), ClkTZ.hour(), ClkTZ.minute(), ClkTZ.second());
    //logFile += String(timeStr) + String("->");
    //logFile += curTemp+ " °C\r\n";
  }
  else
  {
    if (webTemp != "")
    {
      curTemp = webTemp; 
      strcpy(tempCString,webTemp.substring(0,webTemp.indexOf(' ')).c_str());
      Serial.println("Temperature sensor failed");
      Serial.printf("Web Temperature used: %s °C\n",curTemp);
    }
    //else Leave whatever tempr stored earlier
    //curTemp = "NIL";
  }
  curTemp += " C";
  ws.broadcastTXT("7" + String(timeStr) + "-> " + tempCString + " °C\n");
  ws.broadcastTXT(String("1") +  tempCString + " °C");
}

//Monitor the config pin and call Config function if required
static void MonitorConfigPin()
{
  //return; //Switch not connected yet
  if (digitalRead(SERVERTRIGGER) == LOW)
  {
    //Serial.println("High detected");
    return;
  }
  int elapse = 0;
  Serial.printf("Detected Config Request\n");
  while (digitalRead(SERVERTRIGGER) == HIGH && elapse < 12)
  {
    elapse++;
    delay(SECOND);
    Serial.println("Trigger detected");
  }
  if (elapse < 3) //discard spikes
    return;

  ws.broadcastTXT("7Config Change Requested\n");
  
  //Over 8 seconds then start Web server
  if (elapse > 8)
  {
    //bConnect = false;
    bCaptive = true;
    dma_display->setFont();
    //startServer();
    handleRoot();
    return;
  }

    //Over 2 seconds and less than 8 seconds? then Toggle between Clock & Scoreboard

  if (elapse > 2)
  {    
    modeChange();
  }

}

void modeChange()
{
  bCaptive = true; //This will exit the loop

  //Now you can kill it
  if (bgTask != null)
    vTaskDelete(bgTask);

  dma_display->setFont();

  dma_display->fillScreen(dma_display->color444(0, 0, 0));
  dma_display->setTextSize(1);
  dma_display->setCursor(2, 12);
  dma_display->setTextColor(myGREEN);
  dma_display->setTextWrap(true);
  dma_display->println("System configuring.");
  dma_display->setCursor(2, 24);
  dma_display->println("Please Wait");
  dma_display->setTextWrap(false);

  bDisplay = !bDisplay; //Toggle Scoreboard switch

  header.bDisplay = bDisplay;
  Serial.println("Getting into Save Credentials");

  saveCredentials(); //Save it to EEPROM
  Serial.println("Back from Save Credentials");
  ESP.restart();
}

void loop() 
{
    
  MonitorConfigPin();  //need to work this out
  server.handleClient();
  ElegantOTA.loop();
  ws.loop();
  server.handleClient(); 
  events();
  if (!bConnect)
  {

    if ((millis() - resetDelay) > RESETDELAY) 
    {
      ESP.restart();
    }
  }
  else
  {
    
    //Day change Test
   // if (ClkTZ.day() != bDay)  
   //   bDay = !bDay;

    while ((millis() - tempSense) > TEMPSENSOR) //Check Temperature every 5 minutes
    {
      GetTemperature();
      tempSense = millis();
      bNetSense = !bNetSense;
    }
    /* If bDisplay messages to be obtained from web UI. This should be only when change occur. No need in loop */
    events();
    while ((millis() - weatherTime) > WEATHERTIME) //Get Weather every 25 minutes 30 * 50 timedelay is defined as 30seconds
    {
      GetWeather();
      weatherTime = millis();   
    }
    while (ClkTZ.day() != bDay )//One day passed. Update Panchangam 
    {
      if (bDisplay)
        vTaskSuspend(bgTask); //Wait till the calendar information obtained. only for Display and not clock which will correct itself
      Serial.println("Testing for Day Change. Suspending Display");
      GetWeather();
      CheckVersionChange();
      CheckCalendarVersion();
      broadcastDailyChanges(-1);
      delay(SECOND * 3);
      TamilCalendar = GetTC(ClkTZ.dateTime("Ymd"));
      bDisplayChange = true; //Only need to change once a day
      delay(SECOND * 2);
      if (bDisplay)
        vTaskResume(bgTask);
      bDay = ClkTZ.day();
      weatherTime = millis();
    }
  }
}
