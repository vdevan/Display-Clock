#line 1 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\Globals.h"
#ifndef _GLOBALS_H_
#define _GLOBALS_H_
//Select Either One
//#define PROD
#define CTIME 24122400 //Update everytime compiled and ready for upload
//#define ESP32D //The new ESP32-WROOM-32D has RGB interchanged this is with type-C connector

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
#include "Weather8x8px.h" //Change this every year before April 15


//Internal House Keping Declarations
//WiFi Client Connection change if too low
#define WHILE_LOOP_DELAY  200   //Connection Wait time
#define MAXNETWORK 6            //stored network
#define SECOND 1000
#define WIFI_TIMEOUT 12        //Time out in secs for Wifi connection. 

#define ONE_WIRE_BUS 21          // Temperature Sensor
#define RETRY SECOND * 5        //Retry for network intervel
#define NETWORKOFFSET 100; //From Ver 1009, Network stored in a separate space not to interfere with stored Network data
#define VERSION 10003 //Change from Scoreboard

#define PROGID 0x4443  //Hex for DC - display clock
#define SENSORCOMP 3
#define CALINDEX 1
#define CALVER 10001
#define CALWAIT 2 //Wait for 2 minutes after midnight before changing date and calendar

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
IPAddress ESPIP(172, 20, 30, 1);  
IPAddress dhcp(172, 20, 30, 5);
char ESPSSID[] = "DISPCLOCK";
char HOSTNAME[] = "displayclock";
#else
IPAddress ESPIP(172, 20, 31, 1);  
IPAddress dhcp(172, 20, 31, 5);
char ESPSSID[] = "DCTEST";
char HOSTNAME[] = "dctest";
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
uint8_t DayCh;
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
unsigned long netConnectDelay; //Time to check for connectivity
const int ledPin = 2;
String logFile = "";

WebSocketsServer ws = WebSocketsServer(81); 
bool bState = false; //need to be removed. Used by Sample LED screen page

//Variables for Panchangam Requirement
bool bFS = false;
char DEFAULTDATA[] = "No Panchangam Data Available";
bool bMonthChange = true; 
bool bDataAvailable = false;
char PANCHANGAMURL[] = "https://panchangam.brahas.com/tcalendar/dc";
const char TZURL[] = "https://panchangam.brahas.com/lookup.php";
char VERSIONFILE[] = "/version.txt";
char DISPLAYTEXT[] = "/display.txt";
char CALENDARFILE[] = "/calendar.txt";
char DISPCONFIG[] = "Display Text Config. Total Lines:"; //First line in display.txt
bool bVersionChange = false;
bool bDisplayChange = false;
String DisplayData;
String scrollText;

//Function Prototypes for Global access are declared here. Needed for Intellisense
void handleSettingsSave(); //Handles Saving of settings and reset if require
void handleWifiSave();
void handleNotFound();
void handleSaveData();
void handleWeb();
void handleRoot();
static bool loadCredentials();
static void connectWifi();
static void startServer();
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
bool StoreFileFromWeb(char* dest,bool bCal);
void CheckVersionChange();
void CheckCalendarVersion();
static bool GetLocation();
static bool GetWeather();
String GetTCString(String pIndex);
String GetTC(String pIndex);
void DispReq(void* param);
void ClkReq(void* param);
String GetData(char* fName, int Lookup);
void broadcastDailyChanges(int num);
static void saveCredentials();
static String GETRequest(String uri);
void broadcastWeatherChanges(int num);
void sendResponse();
static String GETStreamRequest(String src);
bool SaveFile(char* fileName,String data);
void RemoveCalendarFiles(int yr);
static bool SetTimeZone(Timezone *TZ, String location);
static void DisplayServerScreen(); //Displays Web server details on the display panel
#endif