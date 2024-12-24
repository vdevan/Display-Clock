#include <Arduino.h>
#line 1 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\DisplayClock.ino"
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
#include "Globals.h"


#line 61 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\DisplayClock.ino"
void setup();
#line 298 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\DisplayClock.ino"
String InitVariables(int index);
#line 351 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\DisplayClock.ino"
static bool PanelInit();
#line 402 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\DisplayClock.ino"
void BlinkLED();
#line 414 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\DisplayClock.ino"
void GetTemperature();
#line 461 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\DisplayClock.ino"
static void MonitorConfigPin();
#line 503 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\DisplayClock.ino"
void modeChange();
#line 533 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\DisplayClock.ino"
void loop();
#line 20 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\BGTask.ino"
void DispReq(void* param);
#line 470 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\BGTask.ino"
void ClkReq(void* param);
#line 35 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\CaptivePage.ino"
void handleRoot();
#line 105 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\CaptivePage.ino"
void handleWifiSave();
#line 173 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\CaptivePage.ino"
void handleNotFound();
#line 178 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\CaptivePage.ino"
void sendResponse();
#line 173 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\DCPage.ino"
void handleWeb();
#line 201 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\DCPage.ino"
void handleWebSocketMessage(uint8_t num, uint8_t *data, size_t len);
#line 240 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\DCPage.ino"
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
#line 329 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\DCPage.ino"
String GetTCString(String pIndex);
#line 369 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\DCPage.ino"
void handleSaveData();
#line 466 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\DCPage.ino"
void handleSettingsSave();
#line 535 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\DCPage.ino"
void broadcastWeatherChanges(int num);
#line 545 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\DCPage.ino"
void broadcastDailyChanges(int num);
#line 3 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\Network.ino"
static void connectWifi();
#line 47 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\Network.ino"
static bool tryConnect();
#line 71 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\Network.ino"
static void startServer();
#line 98 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\Network.ino"
static void DisplayServerScreen();
#line 136 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\Network.ino"
static String GETStreamRequest(String src);
#line 169 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\Network.ino"
static String GETRequest(String uri);
#line 196 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\Network.ino"
bool StoreFileFromWeb(char* dest,bool bCal);
#line 267 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\Network.ino"
String GetData(char* fName, int Lookup);
#line 315 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\Network.ino"
static bool SetTimeZone(Timezone *TZ, String location);
#line 348 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\Network.ino"
static String SecurePOSTRequest(String URL, String httpData);
#line 14 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\Panch2024-25.ino"
void CheckVersionChange();
#line 39 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\Panch2024-25.ino"
void CheckCalendarVersion();
#line 97 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\Panch2024-25.ino"
void RemoveCalendarFiles(int yr);
#line 111 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\Panch2024-25.ino"
bool SaveFile(char* fileName,String data);
#line 134 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\Panch2024-25.ino"
void ListDir(const char * dirname, uint8_t levels);
#line 180 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\Panch2024-25.ino"
bool CreateDir(char* dirName);
#line 211 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\Panch2024-25.ino"
void RemoveDir(char* dirName);
#line 254 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\Panch2024-25.ino"
bool GetDataFiles(String year,int month);
#line 330 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\Panch2024-25.ino"
String GetTC(String pIndex);
#line 18 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\Storage.ino"
static bool loadCredentials();
#line 75 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\Storage.ino"
void ReadNetworkData();
#line 91 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\Storage.ino"
void EraseStoredValue();
#line 104 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\Storage.ino"
static void saveCredentials();
#line 20 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\WeatherDateTime.ino"
static bool GetLocation();
#line 112 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\WeatherDateTime.ino"
static bool GetWeather();
#line 61 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\DisplayClock.ino"
void setup() 
{
  pinMode(ledPin, OUTPUT);
  BlinkLED();
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  DS18B20.begin();
  #ifndef ESP32D
  strcpy(ESPSSID, "DISPOLD");
  strcpy(HOSTNAME,"dclockold");
  IPAddress ESPIP(172, 20, 32, 1); 
  IPAddress dhcp(172, 20, 32, 5);
  #endif
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
  WiFi.setAutoReconnect(true); // Enable auto-reconnect
  WiFi.persistent(true);       // Save credentials to flash
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
      /* Old remnants to be removed
      if (LittleFS.exists("/TestFile.txt"))
      {
        LittleFS.remove("/TestFile.txt");
      }
      if (LittleFS.exists("/EndTest.txt"))
      {
        LittleFS.remove("/EndTest.txt");
      }
      if (LittleFS.exists("/UpTime.txt"))
      {
        LittleFS.remove("/UpTime.txt");
      }*/
      //#ifdef PROD //Remove Debug.txt
      if (LittleFS.exists("/Debug.txt"))
      {
        LittleFS.remove("/Debug.txt");
      }
      //#endif
    }
    //ListDir("/",2);

    //RemoveDir("/2025");  //used for testing
    CheckVersionChange();
    setInterval(CLOCKINTERVEL); //Check clock every 10 minutes
    events();
    bCaptive = false;
    dma_display->fillScreen(dma_display->color444(0, 0, 0));
    dma_display->setTextSize(1);
    dma_display->setCursor(2, 1);
    dma_display->setTextColor(myGREEN);
    dma_display->print("Initialising...");
    dma_display->setTextWrap(false);
    //dma_display->println("System configuring.");
    dma_display->setCursor(2, 24);
    dma_display->println("Please Wait");
    dma_display->setCursor(2, 34);
    dma_display->println("Checking Internet...");
    //dma_display->fillScreen(dma_display->color444(0, 0, 0));
    dma_display->setTextSize(1);
    dma_display->setCursor(2, 12);
    dma_display->setTextColor(myGREEN + myBLUE);
    
    InitVariables(0);
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
    //dma_display->fillScreen(dma_display->color444(0, 0, 0)); //All display activities will be now handled by processor 1
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


  weatherTime = millis();
  tempSense = millis();

  DayCh = 0;
  #ifndef PROD
    File file = LittleFS.open("/Debug.txt", FILE_APPEND);
    file.print("Forcing Day change @: ");
    file.println(ClkTZ.dateTime());
    file.flush();
    file.close();
  #endif
  //Serial.println("After deleting reset file");
  //ListDir("/",2); 
  netConnectDelay = millis(); //delay testing for network for 15 minutes 


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
       if (!SetTimeZone(&Ad1TZ,TZ))
          Serial.printf("Unable to set Time zone for location: %s\n",TZ.c_str());
        else
          Serial.println("Succesfully set location for tzID1");  

        logFile += "Additional Clock Zone Settings:\n" + String(tzID1) + ": " + Ad1TZ.getOlson() + "\n" + String(tzID2) + ": " + Ad2TZ.getOlson() + "\n"; 

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

        if (!SetTimeZone(&Ad2TZ,TZ))
          Serial.printf("Unable to set Time zone for location: %s\n",TZ.c_str());
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
  
  //Over 12 seconds then start Web server
  if (elapse > 12)
  {
    //bConnect = false;
    bCaptive = true;
    if (bgTask != null)
      vTaskDelete(bgTask);
    DisplayServerScreen();
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
  #ifdef ESP32D  
  MonitorConfigPin();  //need to work this out
  #endif
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
    if (!bCaptive)
    {
      //Added for persistent network connectivity Check every 15 minutes for network connectivity
      if ((millis() - netConnectDelay) > RESETDELAY) 
      {
        if (WiFi.status() != WL_CONNECTED) 
        {
          Serial.println("Wi-Fi disconnected, attempting reconnect...");
          connectWifi();
        }
        else
          Serial.println("Wi-Fi already connected, nothing to do.");
        netConnectDelay = millis(); //check again in 15 minutes
      }

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
      while (ClkTZ.day() != DayCh )//May need to Check every hour. Sometimes day change may be missed
      {
        if (bDisplay)
          vTaskSuspend(bgTask); //Wait till the calendar information obtained. only for Display and not clock which will correct itself
        Serial.print("Testing for Day Change. Suspending Display. DayCh current value: ");
        Serial.println(DayCh);
        GetWeather();
        CheckVersionChange();
        CheckCalendarVersion();
        broadcastDailyChanges(-1);
        //delay(SECOND * 3);
        TamilCalendar = GetTC(ClkTZ.dateTime("Ymd"));
        bDisplayChange = true; //Only need to change once a day
        //delay(SECOND * 2);
        if (bDisplay)
          vTaskResume(bgTask);
        weatherTime = millis();
      }
    }
  }
}

#line 1 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\BGTask.ino"


/******************************************************************
 * This is Display board procedure. Only fixed fonts are used.
 * Either Double size or single size can be used. 
 * If single size used then 
 * maximum of 20 characters per line with 6.4 points for Margin
 * total lines would be 7 with one point for Margin  
 * If double size used then
 * Maximum of 9 characters with 12 point for margin 
 * total lines would be 3 with one point for margin on top  
 *
 * Note: additional one point is used per line for both Single
 * and double size
 * Ver: 1001 Vasu Dated: 01-09-2024
 * Ver: 1002 Vertical Centering done. Shortcuts for dynamic update completed - Dated 26/09/2024
 * Ver: 1003 Clock display on one of the lines added 7/10/24
 * *****************************************************************/
#include "Globals.h"
void DispReq(void* param)
{
  //Variables for Display;
  String lineText;
  uint8_t lines;
  uint16_t lineColor;
  uint8_t lineFontSize;
  uint16_t start = 0;
  uint16_t end;
  uint8_t vCenterOffset=0;
  uint8_t fontSize;
 
 //settings to include %c or %C
  TIME dt;
  bool bClExist;
  char clockBuffer[10];
  String clockLineText;
  uint8_t clockLineXaxis;
  uint8_t clockLineYaxis;
  bool bAMClock;
  uint8_t clockLineRColor;
  uint8_t clockLineGColor;
  uint8_t clockLineBColor;
  uint8_t clockLineFSize;
  uint8_t clockLineFHeight;
  Serial.println("Entering Display Mode");

  // WEBPARAM* wp = (WEBPARAM*)param;
  //MatrixPanel_I2S_DMA* dma = dma_display;

  //Need to be removed except SCROLLCOLOR
  //Color Requirements for Cricket Active Score
  int scrollColor;

  
  //can be changed by user. Just a default settings
  scrollText = "You can even scroll display with colorful messages  *** Current Temperature: %t ***";
  //dma_display->setTextColor(0x83247f)
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  char colorbuf[5];
  uint8_t totalDoubles;
  uint8_t Vline;
  // Note 2 extra single size line could be squeezed for maximum utilization
  //if Full double size characters are used
  
  //Single Size Total Characters 21. Margin both sides = 1
  uint8_t LeftM = 1; // 21 characters with 1 point on either side
  uint8_t TopM = 1; // 7 lines add 1 extra space between lines - Last point will end up with one point

  // Double Size Total characters 10 with 4 points on either side if centering required
  int Left2C = 4; //if centering required
  int Top2C = 7; // 3 lines total with 8 points each on top and bottom for centering. 

  uint8_t NextX = LeftM;
  uint8_t NextY = TopM;
  uint16_t st;
  uint16_t ed;
  uint8_t cent;
  
  /* Note: Variables declared for scrolling text. Comment if Scrolling not required*/
 
    int xdisp;
    int ydisp;
    unsigned long scrollSpeed;
    bool bScroll;
    int Xax;
    int Yax;
    int scrSize;
    String scrText; //Scrolling text
    int track;
    int trackVal;
    bool bDelay = false;
    String dispText;
    unsigned long scrDelay;
    String storedScrollText;
    String temp;
    bool exitFlag = false;
  /* End of Scroll Variables */

  
  logFile += "Serving Display information\r\n";

  //Erase screen 
  while(true) //Will do this continuously will only actually used once unless DisplayData changes
  {
    Serial.println("Displaying Saved Texts");
    exitFlag = false;
    start = 0;
    end = 0;
    NextX = LeftM; //Start of First Line
    NextY = TopM;
    bClExist = false;
    dma_display->fillScreen(myBLACK); //Erase display
    dma_display->setTextWrap(false); // For the time being. Later we can change this    
    temp = ""; //force temperature replacement
    //Ensure we have the right data if fails will use previous 
    if (DisplayData=="")
    { 
      logFile += "display.txt file not found \r\n";
      exitFlag = true;
    }
    else 
    {
      end = DisplayData.indexOf('\n');
      if (end < 0)
      {
        logFile += "Display.txt configuration mismatch. Re-save the file; end Value: " + String(end) + "\r\n";
        exitFlag = true;
      }
      else
      {
        lineText = DisplayData.substring(start,end);
        lines = lineText.charAt(lineText.length()-1) - '0';
        Serial.printf("Line Text Read: %s; Line Nos Obtained: %d\n",lineText.c_str(),lines);
        //logFile += "First Line Text: " + lineText + " Total Lines Read: " + String(lines) + "\n";  
        
        start = end + 1;
        //Get the first line and total lines so we can work on Vertical Centering
        //int lines = DisplayData.substring(DisplayData.length()-1).toInt();  
        if (lines <= 0 || lines > 7)
        {
          logFile += "display.txt configuration mismatch. Re-save the file \r\n";
          exitFlag = true;
        }
      }
    }
    if (exitFlag)
    {
      dma_display->setTextSize(1);
      dma_display->setTextWrap(true);
      dma_display->setCursor(2, 12); //lets center it
      dma_display->setTextColor(dma_display->color565(255,150,150));
      dma_display->println("Invalid Display Text. Check log File. Unable to proceed"); //can be split here for showing runs in different color
      Serial.print("Errors in Display Text found. Check log files");
      break;
    }
    bDisplayChange = false; //We don't need to get data until data changes
    #ifndef PROD
      Serial.print("Possible Day change called from loop. Date Time Recorded: ");
      Serial.println(ClkTZ.dateTime());
      Serial.print("Tamil Calendar obtained: ");
      Serial.println(TamilCalendar.c_str());
    #endif
    //Store the Scroll information first. Note %t must be replaced by Current Temperature
    end = DisplayData.indexOf('\n',start);
    lineText = DisplayData.substring(start,end);
    //Serial.printf("Full Scroll line Read before any decoding: %s;\n",lineText.c_str());
    //Serial.printf("Values for RGB: %s, %s, %s\n", lineText.substring(3,5).c_str(),lineText.substring(5,7).c_str(),lineText.substring(7,9).c_str());
    totalDoubles = lineText.substring(0,1).toInt();
    red = (int)strtol(lineText.substring(3,5).c_str(), NULL, 16);
    green = (int)strtol(lineText.substring(5,7).c_str(), NULL, 16);
    blue = (int)strtol(lineText.substring(7,9).c_str(), NULL, 16);
    storedScrollText = lineText.substring(lineText.indexOf("\"") + 1,lineText.length()-1); //Remove quotes
    //String cal = TamilCalendar;
    st = 0;
    ed = TamilCalendar.indexOf(' ');
    while (ed > 0)
    {
      
      //Get year
      storedScrollText.replace("%y",TamilCalendar.substring(st, ed));
      st = ed +1;
      ed = TamilCalendar.indexOf(' ', st);
      //Get Month
      storedScrollText.replace("%m",TamilCalendar.substring(st, ed));
      st = ed +1;
      ed = TamilCalendar.indexOf(' ', st);
      //Get Date
      storedScrollText.replace("%d",TamilCalendar.substring(st, ed));
      st = ed +1;
      ed = TamilCalendar.indexOf(' ', st);
      //Get Tithi
      storedScrollText.replace("%T",TamilCalendar.substring(st, ed));
      st = ed +1;
      ed = TamilCalendar.indexOf(' ', st);
      //Get Star - need to remove the Paksham int
      storedScrollText.replace("%s",TamilCalendar.substring(st, TamilCalendar.length()-1));
      //Serial.printf("Last String returned %s\n",cal.substring(cal.length()-1).c_str());
     
      //Get Paksham
      if (TamilCalendar.substring(TamilCalendar.length()-1).toInt() == 1)
      {
        //Serial.println("Integer 1 returned");
        storedScrollText.replace("%p","Shukla");
      }
      else
      {
        //Serial.println("Integer 0 returned");
        storedScrollText.replace("%p","Krishna");
      }
      break;
    }
    //Serial.printf("storedScrollText: %s\n",storedScrollText.c_str());

    scrollColor = dma_display->color565(red,green,blue);
    //Serial.printf("Total Doubles: %d; RGB Values Read for Scroll text: %d; %d; %d\n",totalDoubles,red,green,blue);
    start = end +1; //Ready for next line
     //ScrollText already Stored
    //Display the lines
    //Serial.printf("Display Text before loop: %s", DisplayData.substring(start).c_str());
    
    if (storedScrollText == "") //No Scroll Text. We can use the entire area Also add 1 vertical space between lines
    {
      cent = PANEL_RES_Y - (((lines * SINGLEY) + (totalDoubles * SINGLEY )) + (lines - 2)); //Add 1 extra space between lines excluding first and last
    }
    else
    {
      lines--;
      cent = DISPLAYAREA - ((( (lines ) * SINGLEY) + (totalDoubles * SINGLEY)) + (lines -2) );
    }
    if (cent < 0)
      cent = 0;
    //Serial.printf("Display Area: %d; lines: %d; SINGLEY: %d; totalDoubles: %d; Total extra spaces: %d\n",DISPLAYAREA,lines,SINGLEY,totalDoubles,Tline);
    //Serial.printf("Value of Tline / lines: %d and Tline \ lines: %d\n",Tline/lines, Tline % lines);
    Tline = cent/lines;
    Vline = cent % lines;

    for (int i = 1; i <= lines; i++ )
    {
      
      end = DisplayData.indexOf('\n',start);
      lineText = DisplayData.substring(start,end);
      //Serial.printf("%d: %s \n",i,lineText.c_str());
      fontSize = lineText.substring(0,1).toInt();
      red = (int)strtol(lineText.substring(3,5).c_str(), NULL, 16);
      green = (int)strtol(lineText.substring(5,7).c_str(), NULL, 16);
      blue = (int)strtol(lineText.substring(7,9).c_str(), NULL, 16);
      lineText = lineText.substring(lineText.indexOf("\"") + 1,lineText.length()-1);
      //Serial.printf("LineText %d:%s\n",i,lineText.c_str());
      if ((lineText.indexOf("%c") >=0 || lineText.indexOf("%C") >=0 ) && !bClExist)
      {
        clockLineText = lineText;
        clockLineXaxis = NextX;
        clockLineYaxis = NextY;
        clockLineRColor = red;
        clockLineGColor = green;
        clockLineBColor = blue;
        clockLineFSize = fontSize + 1;
        clockLineFHeight = clockLineFSize == 1 ? SINGLEY : DOUBLEY;
        dt.minutes = ClkTZ.minute();
        dt.hour = ClkTZ.hour();
        if (lineText.indexOf("%c") >=0)
        {
          sprintf(clockBuffer,"%02d:%02d",dt.hour,dt.minutes);
          lineText.replace("%c",clockBuffer);        
          bAMClock = false;
        }
        else
        {
          dt.hour = dt.hour > 12 ? dt.hour - 12 : dt.hour;
          dt.hour = dt.hour == 0 ? 12 : dt.hour;
          sprintf(clockBuffer,"%02d:%02d %s",dt.hour,dt.minutes,ClkTZ.isAM() ? "AM" : "PM");
          lineText.replace("%C",clockBuffer);  
          bAMClock = true;
        }
        bClExist = true;
        Serial.printf("LineText on line No: %d is %s\n",i,lineText.c_str());
      }

      start = end +1; //Ready for next line
      //Text displayed
      //Serial.printf("X-Axis for Text: %d; Y-Axis for Text: %d\n",NextX,NextY);
      //Serial.printf("Line No: %d; Font size: %d; RGB Values: (%d, %d, %d) Text: %s\n",i,fontSize,red,green,blue,lineText.c_str());
      dma_display->setTextSize(fontSize + 1);
      dma_display->setCursor(NextX, NextY); //lets center it
      dma_display->setTextColor(dma_display->color565(red,green,blue));
      dma_display->println(lineText); //can be split here for showing runs in different color
      NextY += (SINGLEY * (fontSize + 1)) + Tline; //Need Tline to be worked out This is for vertical Centering

      if (Vline > 0)
      {
        NextY ++;
        Vline --;
      }
      //if (i == lines)
      //  NextY += Vline; //Leave the last line for scroller
    
      //We won't change NextX for current Version - That is required for Centering horizontal
    }
   
    //dma_display->setTextSize(1); //Always 1
    scrollSpeed = SCROLLSPEED; 
    bScroll = false;
    Xax = SCROLLX; //Always the last line
    Yax = SCROLLY;
    bDelay = false;  
    scrDelay = millis();

    xdisp = Xax;
    scrollText = storedScrollText;
    if (scrollText.length() > 21) //If we need scrolling
    { 
      while (!bDisplayChange) //Loop until DisplayText changes
      {
        //Serial.println ("Successfully entered loop");

        //dma_display->fillScreen(dma_display->color444(0, 0, 0)); //Clear Screen
        //We will read the ScrollText Value here. This is the scroll loop
        if (bClExist)
        {
          if (dt.minutes != ClkTZ.minute())
          {
            lineText = clockLineText;
            dma_display->fillRect(0, clockLineYaxis, PANEL_RES_X * PANEL_HCHAIN, clockLineFHeight, myBLACK);
            dt.minutes = ClkTZ.minute();
            dt.hour = ClkTZ.hour();
            if (bAMClock)
            {
              dt.hour = dt.hour > 12 ? dt.hour - 12 : dt.hour;
              dt.hour = dt.hour == 0 ? 12 : dt.hour;
              sprintf(clockBuffer,"%02d:%02d %s",dt.hour,dt.minutes,ClkTZ.isAM() ? "AM" : "PM");
              lineText.replace("%C",clockBuffer);  
            }
            else
            {
              sprintf(clockBuffer,"%02d:%02d",dt.hour,dt.minutes);
              lineText.replace("%c",clockBuffer);       
            } 
            dma_display->setTextSize(clockLineFSize);
            dma_display->setCursor(clockLineXaxis, clockLineYaxis); //lets center it
            dma_display->setTextColor(dma_display->color565(clockLineRColor,clockLineGColor,clockLineBColor));
            dma_display->println(lineText);
            //Serial.println(lineText);
          }
          
        }
        if ((millis() - scrDelay) > SCROLLSPEED)
        {
          if (!bScroll)
          {
            //Set Scroll variables
            if (temp != curTemp) //update current temperature
            {
              scrollText = storedScrollText; //"You can even scroll display with colorful messages  *** Current Temperature: %t ***";
              scrollText.replace("%t",curTemp);
              temp = curTemp;
            }
            scrText = scrollText;
            //delay(500); //Needed only if you need to pause at begining
            xdisp = Xax + 1;
            ydisp = Yax;

            // Uncomment this For single line display Once display reaches end of line
            //scrSize = -((scrText.length() * SINGLEX) - (PANEL_RES_X * PANEL_CHAIN) + (PANEL_RES_X/2)); //Let the scrolling go to half a screen

            //Uncomment for continuous scrolling - For now we will use continuous scrolling
            scrSize = -((scrText.length() * SINGLEX) - (PANEL_RES_X * PANEL_HCHAIN)); //Let the scrolling go continuous

            track = 2;
            trackVal = (PANEL_RES_X * PANEL_HCHAIN) - scrSize - 1;
            dispText = scrText;
            bScroll = true;
            //bDelay = true; //Only needed if not continuous scrolling
            //Serial.printf ("Successfully printed first line: %s\n",dispText.c_str());
          }
          else
          {
            if (scrSize < 0) //Only scroll if it is more than panel size
            {
              xdisp--;
              track--;

              if (xdisp == scrSize)
              {
                  dispText += dispText; //Needed for continuous Scrolling
                  //bScrollInit = false;
              }

              //Uncomment following lines if continuous scrolling required

              if (track == -trackVal)
              {
                  //delay(2000);
                  bScroll = false;
                  xdisp = Xaxis;
                  track = Xaxis;
              }
            }
            else
              bScroll = false; //Get the next info
          }
          // ScrollText(dispText, xdisp, ydisp) - Expansion
          dma_display->setTextSize(0);
          //dma_display->setTextWrap(false);
          dma_display->fillRect(0, SCROLLY, PANEL_RES_X * PANEL_HCHAIN, SINGLEY + 1, myBLACK);
          dma_display->setCursor(xdisp, ydisp);
          dma_display->setTextColor(scrollColor);
          dma_display->print(dispText);
          scrDelay = millis();
        }
        //bTaskInit = true;
      }
    }
    else
    {
      dma_display->setTextSize(0);
      dma_display->print(scrollText); 
      //Serial.println ("Not in the Scroll loop");
      while (!bDisplayChange)
      {
        if (bClExist)
        {
          if (dt.minutes != ClkTZ.minute())
          {
            lineText = clockLineText;
            dt.minutes = ClkTZ.minute();
            dt.hour = ClkTZ.hour();
            dma_display->fillRect(0, clockLineYaxis, PANEL_RES_X * PANEL_HCHAIN, clockLineFHeight, myBLACK);
            if (bAMClock)
            {
              dt.hour = dt.hour > 12 ? dt.hour - 12 : dt.hour;
              dt.hour = dt.hour == 0 ? 12 : dt.hour;
              sprintf(clockBuffer,"%02d:%02d %s",dt.hour,dt.minutes,ClkTZ.isAM() ? "AM" : "PM");
              lineText.replace("%C",clockBuffer);  
            }
            else
            {
              sprintf(clockBuffer,"%02d:%02d",dt.hour,dt.minutes);
              lineText.replace("%c",clockBuffer);       
            } 
            dma_display->setTextSize(clockLineFSize);
            dma_display->setCursor(clockLineXaxis, clockLineYaxis); //lets center it
            dma_display->setTextColor(dma_display->color565(clockLineRColor,clockLineGColor,clockLineBColor));
            dma_display->println(lineText);
            //Serial.println(lineText);
          }
        } //Only loop back if DisplayText changes. Else nothing to do
      }
    }
  }
}


/*************************************************************************
 * Background application for displaying Clock amd Tamil Calendar. To ensure
 * maximum utilisation of CPU cycles, only Seconds are updated every second. 
 * Minute and hour are updated as they change. AM / PM also updated when needed
 * This method avoids flickering and ensures smooth display. Runs on Processor 1.
 * Time used by eztinme.h gets updated every 10 minutes on processor 0. 
 * Global variable ClkTZ keeps track of date & time and this is copied locally
 * Weather is updated every 25 minutes. Global variable Weather, Rain and Cloud
 * are used for weather display. Weather variable is copied locally
 * All environmental requirements like WAN IP address, Latitude, Longitude and
 * Timezones are obtained from Web thus providing flexibility
 * 
 * Ver 1008 - Vasu deated 15/02/2024
 * *******************************************************************/

void ClkReq(void* param)
{   
  if (!bConnect)
    return;

  char tz1String[12];
  char tz2String[12];
  uint16_t TZ1COLOR = ClkTZ.getOffset() < Ad1TZ.getOffset() ? dma_display->color565(255, 190, 90) : dma_display->color565(90, 190, 255);
  uint16_t TZ2COLOR = ClkTZ.getOffset() < Ad2TZ.getOffset() ? dma_display->color565(255, 190, 90) : dma_display->color565(90, 190, 255); 
  char tz1Sign = ClkTZ.getOffset() < Ad1TZ.getOffset() ? '-' : '+';
  char tz2Sign = ClkTZ.getOffset() < Ad2TZ.getOffset() ? '-' : '+';

  //WEBPARAM* wp = (WEBPARAM*)param;
  Serial.println("Clock Service Started");
  sprintf(tz1String,"%s %02d:%02d%c ",tzID1,Ad1TZ.hour(),Ad1TZ.minute(),tz1Sign);
  sprintf(tz2String," %s %02d:%02d%c",tzID2,Ad2TZ.hour(),Ad2TZ.minute(),tz2Sign);
  Serial.printf("Clock Display String for %s: %s\n",tzID1,tz1String);
  Serial.printf("Clock Display String for %s: %s\n",tzID2,tz2String);
  Serial.printf("Offset of KN: %d,Offset of NZ: %d and current AU: %d\n",Ad1TZ.getOffset(),Ad2TZ.getOffset(),ClkTZ.getOffset());
  //MatrixPanel_I2S_DMA* dma = wp->dma_display;
  dma_display->fillScreen(dma_display->color444(0, 0, 0)); //Clear Screen
  String wr = Weather;
  bool bUpdate; 
  bool bSense;
  String TC = TamilCalendar;
  String temp = curTemp;
  //dma_display->setFont();
  //Used for first time display at start of the program and start of the day
  String dispTime;
  String dispDate;

  //Used for keeping track of change in date and time
  uint8_t min;
  uint8_t hr;
  uint8_t hour;
  TIME dt;

  //Various colors used
  int TIMECOLOR = myGREEN + myBLUE;
  int AMCOLOR = dma_display->color565(120, 150, 64);
  int PMCOLOR = dma_display->color565(72, 250, 160);
  int DATECOLOR = dma_display->color565(172, 250, 60);
  int WEATHERCOLOR = dma_display->color565(70, 201, 66);
  int TEMPCOLOR = dma_display->color565(220, 75, 75);
  int CALENDARCOLOR = dma_display->color565(255, 35, 225);
  int XSize = PANEL_RES_X * PANEL_HCHAIN;
  //Toggle between Cloud and Humidity
  constexpr auto TOGGLE = 5;
  bool bToggle = true;
  uint8_t clDisp = 0;

  //Co-ordinates for Time Display. Make any changes here
  int timeX = 6;
  int timeY = 1; 
  int TwocharWidth = 2 * DOUBLEX;
  int TwocharHeight = DOUBLEY;
  int hourStart = timeX;
  int minuteStart = timeX + (DOUBLEX * 3);
  int secondStart = timeX + (DOUBLEX * 6);

  //Co-ordinates for AM / PM display
  int amX = 106;
  int amY = 8;
  int pmY = timeY;

  //Co-ordinates for Date
  int dateX = 1;
  int dateY = timeY + DOUBLEY + 1;

  //Co-ordinates for current Temperature
  int tempX = dateX + SINGLEX * 14;
  int tempY = dateY;

  //Co-ordinates for Weather
  int weatherX = dateX;
  int weatherY = dateY + SINGLEY + 1;

  //Bitmap Co-ordinates for Rain
  int rainX = dateX + (SINGLEX * 10);
  int rainY = weatherY;    

  //Bitmap Size - Fixed 8px x 8px
  uint16_t bmpW = 8;
  uint16_t bmpH = 8;

  //Bitmap Co-ordinates for Cloud
  int cloudX = rainX + (SINGLEX * 3) + 20;//15; //Rain bitmap width is 8  + 2 & 5 for one space.
  int cloudY = weatherY;

  //Co-ordinates for Tamil Calendar
  int tcX = dateX;
  int tcY = weatherY + SINGLEY + 1;

  //Bitmap Co-ordinates for Tithi
  uint16_t tithiX = 118; //Last line, Last column
  uint16_t tithiY = tcY + (SINGLEY *2) + 3;

  //Offset required for Vertical centering 
  int weatherOffset = 0;
  int tcOffset = 0;
  //uint8_t tl;

  #ifndef PROD
    bool printFlag = true;
  #endif  

  //Set date / time for tracking
  String cd = ClkTZ.dateTime(RFC1036);
  //Serial.printf("Date Time Obtained: %s\n",cd.c_str());
  Serial.printf("UTC DATE-TIME: %s\n",ClkTZ.dateTime("Ymd"));

  dt.minutes = ClkTZ.minute();
  dt.hour = ClkTZ.hour();
  dt.ampm = ClkTZ.isAM();
  dt.date = ClkTZ.day();
  dispTime = cd.substring(15, 23);
  dispDate = cd.substring(0, 12);

  //Set Time to be done once - Updates every second
  dma_display->setCursor(timeX, timeY);
  dma_display->setTextSize(2);
  dma_display->setTextColor(TIMECOLOR);
  dma_display->print(dispTime);
  dma_display->setTextSize(1);

  //bDayChange = !bDayChange; //Notify to fetch Panchangam 
  delay(SECOND * 2);

  weatherOffset = Tline > 0 ? (Tline * 2 )+ 1 : 0 ;
  tcOffset = weatherOffset + (Tline * 2);

  //To be done once. - Updates every 12 hours - Display AM / PM
  if (dt.ampm == 1)
  {
    dma_display->fillRect(amX, timeY, SINGLEX * 2, DOUBLEY, myBLACK);
    dma_display->setCursor(amX, amY);
    dma_display->setTextColor(AMCOLOR);
    dma_display->print("AM");
  }
  else
  {
    dma_display->fillRect(amX, timeY, SINGLEX * 2, DOUBLEY, myBLACK);
    dma_display->setCursor(amX, timeY);
    dma_display->setTextColor(PMCOLOR);
    dma_display->print("PM");
  }
    
  //To be done once - updates once every day - Display Date
  dma_display->fillRect(0, dateY + Tline, tempX, SINGLEY, myBLACK);
  dma_display->setCursor(dateX, dateY + Tline);
  dma_display->setTextSize(1);
  dma_display->setTextColor(DATECOLOR);
  dma_display->print(dispDate);

  //Change Tamil Calendar Last character is the moon phase Needed every hour in case Time is obtained a bit late on reboot
  int index = TamilCalendar.length() - 1;
  TC = TamilCalendar.substring(0, index);
  int bmpIndex = TamilCalendar.substring(index).toInt();

  dma_display->fillRect(0, tcY + tcOffset, XSize, SINGLEY * 3, myBLACK);
  dma_display->setCursor(tcX, tcY + tcOffset);
  dma_display->setTextSize(1);
  dma_display->setTextWrap(true);
  dma_display->setTextColor(CALENDARCOLOR);
  dma_display->print(TC);

  //Draw Tithi bitmap - Waxing / Waning

  if (bmpIndex == 0 || bmpIndex == 3)
    dma_display->drawBitmap(tithiX, tithiY, bitmapArray[bmpIndex], bmpW, bmpH, myGRAY);
  else
    dma_display->drawBitmap(tithiX, tithiY, bitmapArray[bmpIndex], bmpW, bmpH, myWHITE);

  if (bClock)
  {
    dma_display->fillRect(0, weatherY + weatherOffset,  XSize, SINGLEY, myBLACK);
    dma_display->setCursor(weatherX, weatherY + weatherOffset);
    dma_display->setTextSize(1);
    dma_display->setTextColor(TZ1COLOR);
    dma_display->print(tz1String);
    dma_display->setTextColor(TZ2COLOR);
    dma_display->print(tz2String);
}
  //Reset to Clock display
  dma_display->setTextWrap(false);
  dma_display->setTextSize(2);
  dma_display->setCursor(timeX, timeY);
  dma_display->setTextColor(TIMECOLOR);

  bUpdate = !bNetUpdate; //Force displaying weather
  bSense = !bNetSense; //Force Temperature display
  hr = dt.hour;
  hour = 25; //dt.hour; //Force to display 12 @ hour!=dt.hour logic
  min = dt.minutes;
  hr = hr > 12 ? hr - 12 : hr;
  hr = hr == 0 ? 12 : hr;
 
  logFile += "Serving Calendar information to display\r\n";
  while (true)
  {
    /* For Vertical Centering Tline and Aline are used. This is worked out as follows:
     * First Line is TimeY, AmY and PmY - No change here
     * Second line dateY/tempY  = use + Tline 
     * Third line is weatherY/cloudY/rainY = Tline + Aline use offset WeatherOffset
     * Fourth line is tcY = (Tline * 2) + Aline use tcOffset
     * Sixth line is thithiY we can leave this as is 
     *
     */

    dt.minutes = ClkTZ.minute();

    #ifndef PROD
      if (dt.date != ClkTZ.day() && printFlag)
      {
        Serial.print("Entering date change initiated by BGTASK. DateTime: ");
        Serial.println(ClkTZ.dateTime());
        Serial.print("dt.date is: ");
        Serial.println(dt.date);
        Serial.print("Tline now is: ");
        Serial.println(Tline);
        printFlag = false; //Stop repetative printing
      } 
    #endif     
    //Check for date. Since the task is background chances are Panchangam & Tline might not reflect correct here. May use an old one
    //As we are calling this once a day, chances are this might be missed completely. Hence call this after 2 minutes. Date / Calendar
    // will not change for first 3 minutes. Unavoidable. Required to ensure Panchangam & Tline are properly loaded
    if (dt.date != ClkTZ.day() && dt.minutes > CALWAIT)
    {
      String cd = ClkTZ.dateTime(RFC1036);
      dispDate = cd.substring(0, 12);
      //If TLINE changes due to change from 2 to 3 line calendar display, then there will be some overlap. 
      //To avoid this first clear the entire screen. 
      dma_display->fillRect(0, dateY, PANEL_RES_X * PANEL_HCHAIN, PANEL_RES_Y * PANEL_YCHAIN, myBLACK);

      weatherOffset = Tline > 0 ? (Tline * 2 )+ 1 : 0 ;
      tcOffset = weatherOffset + (Tline * 2);

      dma_display->setCursor(dateX, dateY + Tline);
      dma_display->setTextSize(1);
      dma_display->setTextColor(DATECOLOR);
      dma_display->print(dispDate);
      dt.date = ClkTZ.day();
      #ifndef PROD
        Serial.print("Entering date change after 2 minutes. Time:  ");
        Serial.println(ClkTZ.dateTime());
        Serial.print("dt.date after 2 minutes is: ");
        Serial.println(dt.date);
        Serial.print("Computed Tline now is: ");
        Serial.println(Tline);
      #endif    

      //Print Panchangam now. Needed to do only once. Moved from hourly update. 
      //Change Tamil Calendar Last character is the moon phase Needed every hour in case Time is obtained a bit late on reboot
      int index = TamilCalendar.length() - 1;
      TC = TamilCalendar.substring(0, index);
      int bmpIndex = TamilCalendar.substring(index).toInt();

      //dma_display->fillRect(0, tcY, XSize, SINGLEY * 3, myBLACK); //Clear the entire screen - already done in earlier line
      dma_display->setCursor(tcX, tcY + tcOffset);
      dma_display->setTextSize(1);
      dma_display->setTextWrap(true);
      dma_display->setTextColor(CALENDARCOLOR);
      dma_display->print(TC);

      //Draw Tithi bitmap - Waxing / Waning
      if (bmpIndex == 0 || bmpIndex == 3)
        dma_display->drawBitmap(tithiX, tithiY, bitmapArray[bmpIndex], bmpW, bmpH, myGRAY);
      else
        dma_display->drawBitmap(tithiX, tithiY, bitmapArray[bmpIndex], bmpW, bmpH, myWHITE);

      bSense = !bNetSense; //Ensure Temperature is displayed by force
      bUpdate = !bNetUpdate; //Force weather display
    }

    //Print Seconds first
    dma_display->setTextSize(2);
    dma_display->fillRect(secondStart, timeY, TwocharWidth, TwocharHeight , myBLACK);
    dma_display->setCursor(secondStart, timeY);
    dma_display->setTextColor(TIMECOLOR);
    dma_display->printf("%02d",ClkTZ.second());
	
	  //Next Display Minute
    if (min != dt.minutes)
    {
		  dt.hour = ClkTZ.hour();
		  min = dt.minutes;
		  dma_display->fillRect(minuteStart, timeY, TwocharWidth, TwocharHeight, myBLACK);
      dma_display->setCursor(minuteStart, timeY);
      dma_display->setTextSize(2);
      dma_display->setTextColor(TIMECOLOR);
      dma_display->printf("%02d",dt.minutes);
		  dma_display->setTextSize(1);
      if (bClock) //If dual clocks selected, then update it
      {
        sprintf(tz1String,"%s %02d:%02d%c ",tzID1,Ad1TZ.hour(),Ad1TZ.minute(),tz1Sign);
        sprintf(tz2String," %s %02d:%02d%c",tzID2,Ad2TZ.hour(),Ad2TZ.minute(),tz2Sign);

        dma_display->fillRect(0, weatherY + weatherOffset,  XSize, SINGLEY, myBLACK);
        dma_display->setCursor(weatherX, weatherY + weatherOffset);
        dma_display->setTextSize(1);
        dma_display->setTextColor(TZ1COLOR);
        dma_display->print(tz1String);
        dma_display->setTextColor(TZ2COLOR);
        dma_display->print(tz2String);

        //Reset to Clock display
        dma_display->setTextWrap(false);
        dma_display->setTextSize(2);
        dma_display->setCursor(timeX, timeY);
        dma_display->setTextColor(TIMECOLOR);
      }
    }

    //Update Cloud and humudity if weather is selected every 5 secons     
    if (!bClock)
    {
      if (clDisp > TOGGLE)//We will alternate between Cloud and humidity every 5 secs
      {
        clDisp = 0;
        //Clear Screen
        dma_display->fillRect(rainX, rainY + weatherOffset, XSize, SINGLEY, myBLACK);
        dma_display->setTextSize(1);
        if (bToggle)
        {
          dma_display->drawBitmap(rainX, rainY + weatherOffset, bitmapArray[7], bmpW, bmpH, myYELLOW);
          dma_display->setCursor(rainX + 10, rainY + weatherOffset);
          dma_display->print(Windspeed);

          dma_display->drawBitmap(cloudX, cloudY + weatherOffset, bitmapArray[6], bmpW, bmpH, myYELLOW);
          dma_display->setCursor(cloudX + 10, cloudY + weatherOffset);
          dma_display->print(Humidity);
        }
        else
        {
          dma_display->drawBitmap(rainX, rainY + weatherOffset, bitmapArray[4], bmpW, bmpH, myYELLOW);
          dma_display->setCursor(rainX + 10, rainY + weatherOffset);
          dma_display->print(Rain);

          dma_display->drawBitmap(cloudX, cloudY + weatherOffset, bitmapArray[5], bmpW, bmpH, myYELLOW);
          dma_display->setCursor(cloudX + 10, cloudY + weatherOffset);
          dma_display->print(Cloud);
        }
        //Reset to Clock display
        dma_display->setTextWrap(false);
        dma_display->setTextSize(2);
        dma_display->setCursor(timeX, timeY);
        dma_display->setTextColor(TIMECOLOR);
        bToggle = !bToggle; //Alternate between Humidity and cloud
      }
    }
    
    //Next Hour Also check for AM / PM and date 
    if (hour != dt.hour)
    {
      hour = dt.hour;
      hr = dt.hour;
      dma_display->fillRect(hourStart, timeY, TwocharWidth, TwocharHeight, myBLACK);
      dma_display->setCursor(hourStart, timeY);
      dma_display->setTextColor(TIMECOLOR);
      hr = hr > 12 ? hr - 12 : hr;
      hr = hr == 0 ? 12 : hr;
      dma_display->setTextSize(2);
      dma_display->printf("%02d",hr);
      dma_display->setTextSize(1);

      //We will check for AM/PM
      if (dt.ampm != ClkTZ.isAM())
      {
        //bDayChange = !bDayChange; //Notify to fetch Panchangam 
        dt.ampm = ClkTZ.isAM();
        if (dt.ampm == 1)
        {
          dt.ampm = ClkTZ.isAM();
          dma_display->fillRect(amX, timeY, SINGLEX * 2, DOUBLEY, myBLACK);
          dma_display->setCursor(amX, amY);
          dma_display->setTextColor(AMCOLOR);
          dma_display->print("AM");
        }
        else
        {
          dma_display->fillRect(amX, timeY, SINGLEX * 2, DOUBLEY, myBLACK);
          dma_display->setCursor(amX, timeY);
          dma_display->setTextColor(PMCOLOR);
          dma_display->print("PM");
        }
      }
        
      //Reset to Clock display
      dma_display->setTextWrap(false);
      dma_display->setTextSize(2);
      dma_display->setCursor(timeX, timeY);
      dma_display->setTextColor(TIMECOLOR);
      
    }

	  //Now for the Temperature evry 5 mins
    if (bSense != bNetSense) 
    {
      bSense = bNetSense;
      temp = curTemp;
      dma_display->fillRect(tempX, tempY + Tline, SINGLEX * 7, SINGLEY, myBLACK);
      dma_display->setCursor(tempX, tempY + Tline);
      dma_display->setTextSize(1);
      dma_display->setTextColor(TEMPCOLOR);
      dma_display->print(temp);
    }
	
	  //Finally let us change weather - every 25 mins
    if(!bClock)
    {
      //Changes Weather and refreshes Tamil Calendar every 25 minutes set by timeDelay = 1500 seconds = 25 minutes
      if (bUpdate != bNetUpdate)
      {
        bUpdate = bNetUpdate;

        //Make a copy of the variable
        wr = Weather;
        
        dma_display->fillRect(0, weatherY + weatherOffset, cloudX, SINGLEY, myBLACK);


        dma_display->setCursor(weatherX, weatherY + weatherOffset);
        dma_display->setTextSize(1);
        dma_display->setTextColor(WEATHERCOLOR);
        dma_display->print(wr);


        //Reset to Clock display
        dma_display->setTextWrap(false);
        dma_display->setTextSize(2);
        dma_display->setCursor(timeX, timeY);
        dma_display->setTextColor(TIMECOLOR);
      }
    }
    clDisp++;
    vTaskDelay(SECOND);
  }

}
#line 1 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\CaptivePage.ino"
#include "Globals.h"
char CaptivePage[] PROGMEM = R"rawliteral(
  <!DOCTYPE html5>
  <html lang="en">
  <head>
<meta charset="utf-8" name="viewport" content="width=device-width, initial-scale=1, user-scalable=no" />
<script src='https://code.jquery.com/jquery-2.1.1.min.js' type="text/javascript"></script>
<script>
window.addEventListener('load', onLoad);
function onLoad(){var elements = document.getElementsByClassName("btn");for (var i = 0; i < elements.length; i++) {elements[i].addEventListener('click', pressed);}
var net = document.getElementsByClassName("n");for (var i = 0; i < net.length; i++) {net[i].addEventListener('click', setnet);}}
function pressed(){var name = this.name;console.log(name + "pressed");if (name == "del"){document.getElementById('ret').innerHTML = "Deleting Network. Please wait";}
if (name == "save"){document.getElementById('ret').innerHTML = "Saving Information. Please wait";}
if (name == "reset"){document.getElementById('ret').innerHTML = "Restarting device. Please wait";}setTimeout(1000);}
function setnet(){document.getElementById('s').value = this.innerText; $("#p").focus();}</script><title>WiFi Config</title>
  )rawliteral";    

char Stub[] PROGMEM = R"rawliteral(
  </head>
  <body>
    <div class="container">
      <h2>WiFi Settings</h2>
      <p>You are connected to the device: <b>
  )rawliteral";


char HTTP_SUBMIT[] PROGMEM = R"literal(
</table><div style="text-align:center"><p><span>
<button id = "btns" class = "btn" name = "save" type = "Submit">Save and Restart</button>&nbsp
<button id = "btnr" class = "btn" name = "reset" type = "submit">Restart without Save</button>
</span></p></div><h4>Stored Networks</h4><div class="strnet"><table>
)literal";

//Root - Portal
void handleRoot()
{
  String Page1="";
  Serial.println("Serving Captive Page");
  String Page = String(ESPSSID) + "</b></p>"; //Add ESPSSID;
  Page += ("<h4>Available Networks</h4><div class=\"scnnet\">");

  int n = WiFi.scanNetworks(); 
  //Now add the Scanned Networks
  if (n > 0)
  {
    for (int i = 0; i < n; i++)
    {
        Page += String(F("<div class=\"inf\"><a href=\"#p\" class=\"n\">")) + WiFi.SSID(i) + F("</a>&nbsp;<span class=\"q") + ((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? F("\">") : F(" l\">")) + abs(WiFi.RSSI(i)) + " %";
        Page += F("</span></div>");
    }
  }
  else
    Page += F("<div class=\"inf\"><p>No WLAN found</p>");

  Page += "</div><form method=\"POST\" action=\"wifisave\"><table style=\"table-layout:fixed;\">";

  if (storedNetworks >= MAXNETWORK)
  {
    Page += "<tr><td colspan=\"3\"><h4 style=\"color:red;\">Please DELETE a Network to store new one</h4></td></tr>";
  }
  else
  {
    Page += F("<tr><td width=\"35%\"><span><label>SSID: </label><br /><input id=\"s\" class=\"txt\" name=\"s\" length=32 placeholder=\"SSID\"></span></td><td width=\"15%\"></td>"
                "<td width=\"50%\"><span><label>Password: </label><br /><input id=\"p\" class=\"txt\" name=\"p\" length=32 placeholder=\"password\"></span></td></tr>");
  }

  for (int i = 0; i < storedNetworks; i++)
  {
    Page1 += F("<tr><td class=\"stbtn\"><label>");
    Page1 += Networks[i].SSID;
    Page1 += F("</label></td><td class=\"stbtn\"><button id=\"btn\" class=\"btn\" name=\"del\" value=\"");
    Page1 += Networks[i].SSID;
    Page1 += F("\" type=\"Submit\">Delete</button></td></tr>");
  }
  
  Page1 += "</table></div></form><p>Note: <span id=\"ret\" style=\"color:red; font-weight:800\"></span></p><table style=\"width: 70%;\"><tr>";
  Page1 += F("<td><button id=\"btin\" class=\"btn\" onclick=\"location.href = '/update'\" name=\"OTA Update\">Update Program</button></td>");
  
  if (bConnect)
    Page1 += "<td><button id=\"btca\" class=\"btn\" onclick=\"location.href = 'WebPage'\" name=\"Captive\">Display Page</button></td>";
  
  Page1 += "</tr></table><p>&nbsp;</p></div></body></html>";


  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  // here begin chunked transfer
  server.send(200, "text/html", "");
  server.sendContent(CaptivePage); //
  server.sendContent(Style); 
  server.sendContent(Stub);
  server.sendContent(Page);  
  server.sendContent(HTTP_SUBMIT);
  server.sendContent(Page1);
  Page=String();
  Page1=String();
  server.sendContent(F("")); // this tells web client that transfer is done
  server.client().stop();
  return;
}

//WiFi Save
void handleWifiSave()
{
  Serial.printf("Server Argument 2: %s\n",server.argName(2));

  String st;
  int offset = 0;
  if (server.hasArg("reset"))
  {
    sendResponse();
    Serial.println("Reset Request Initiated");
    ESP.restart();
    return;
  }

  if (server.hasArg("del")) //handle delete
  {
    st = server.arg("del");
    st.trim();
    Serial.printf("Delete Arg: %s,\n",st);
    int j;
    for (j=0; j < storedNetworks; j++)
    {
        //Serial.printf("READ From Storage[%d]: SSID: %s, Password: %s\n", j, Networks[j].SSID, Networks[j].Password);
        if (st == Networks[j].SSID)
        break;
    }
    Serial.printf("Index to delete found: %d\n",j);
    // J has the index for deleting. Let us zero it first
    memset(Networks[j].SSID, '\0', sizeof(Networks[j].SSID));
    memset(Networks[j].Password, '\0', sizeof(Networks[j].Password));

    //Rearrange storage
    for (int i = j; i < MAXNETWORK -1; i++)
    {
        memcpy(Networks[i].SSID, Networks[i+1].SSID, sizeof(Networks[i+1].SSID));
        memcpy(Networks[i].Password, Networks[i+1].Password, sizeof(Networks[i+1].Password));
    }
    //Erase the last storage
    memset(Networks[MAXNETWORK -1].SSID, '\0', sizeof(Networks[MAXNETWORK -1].SSID));
    memset(Networks[MAXNETWORK -1].Password, '\0', sizeof(Networks[MAXNETWORK -1].Password));
  }
  else
  {
    st = server.arg("s");
    st.trim();
    if (st != "")
    {
      //handle network storage
      //Pick the first empty storage
      for (int i = 0; i < MAXNETWORK; i++)
      {
        if (Networks[i].SSID[0] == 0)
        {
          offset = i;
          break;
        }
      }
      st.toCharArray(Networks[offset].SSID, sizeof(Networks[offset].SSID));
      server.arg("p").toCharArray(Networks[offset].Password, sizeof(Networks[offset].Password));
      Serial.printf("SSID: %s  Password: %s\n",st.c_str(),server.arg("p").c_str());
      Serial.printf("Stored at Location %d\n",offset);
    }

  }      
  sendResponse();
  saveCredentials();    
}

void handleNotFound()
{
    server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}

void sendResponse()
{
  server.sendHeader("Location", "/", true);
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send(302, "text/plain", "");    
  server.client().stop(); // Stop is needed because we sent no content length
}


#line 1 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\DCPage.ino"
#include "Globals.h"

char TempPage[] PROGMEM = R"rawliteral(
<!doctype html>
<html lang="en">
<head>
<script src='http://ajax.googleapis.com/ajax/libs/jquery/1.3.2/jquery.min.js' type="text/javascript" charset="utf-8"></script>
<script>
var websock;
function settings(data){var dt = data.split(';');document.getElementById('disp').value = dt[0];document.getElementById('tzs').value = dt[1];}
function displayValue(data){if (data == "")return;var val = data.split('\n');var lines = Number(val[0].substring(val[0].length - 1));
for (let i = 1; i <= lines; i++){var textline = val[i];var text = textline.substring(textline.indexOf('\"') + 1);text = text.substring(0, text.length - 1);
var data = textline.substring(0, textline.indexOf('\"'));var srgb = data.split(',');if (i == 1) {document.getElementById('scrl').value = text;document.getElementById('clr7').value = srgb[1];}
else {var j = i - 1;var tid = "clr" + j;var fsize = "10pt";if (srgb[0] == "1")fsize = "14pt";document.getElementById(tid).value = srgb[1];tid = "ln" + j;document.getElementById(tid).value = text;
document.getElementById(tid).style.fontSize = fsize;tid = "siz" + j;document.getElementById(tid).value = j + srgb[0];}}}
function setTZ(data){var dt = data.split(';');var tz = dt[0].split(':');document.getElementById('id1').value = tz[0];document.getElementById('tz1').value = tz[1];tz = dt[1].split(':');document.getElementById('id2').value = tz[0];document.getElementById('tz2').value = tz[1];}
function start() {websock = new WebSocket('ws://' + window.location.hostname + ':81/');websock.onopen = function (evt){console.log('websock open');};
websock.onerror = function (evt) { console.log(evt); };
websock.onmessage = function (evt) {let state = evt.data;var txtarea = document.getElementById('li');
switch (state[0]) {
case '0': document.getElementById('dt').innerHTML = state.slice(1);break;
case '1': document.getElementById('rt').innerHTML=state.slice(1);break;
case '2': document.getElementById('tc').innerHTML = state.slice(1);break;
case '3': document.getElementById('wr').innerHTML = state.slice(1);break;
case '4': txtarea.innerHTML = state.slice(1);txtarea.scrollTop = txtarea.scrollHeight;break;
case '5': displayValue(state.slice(1));break;
case '6': alert(state.slice(1));break;
case '7': txtarea.value += state.slice(1);txtarea.scrollTop = txtarea.scrollHeight;break;
case '8': settings(state.slice(1));break;
case '9': setTZ(state.slice(1));break;
case 'a': tcs(state.slice(1));break;
case 'b': document.getElementById('pts').value = state.slice(1); break;
case 'c': document.getElementById('ver').innerHTML = state.slice(1); break;
default:  document.getElementById('ret').innerHTML = evt.data;}}}

window.addEventListener('load', onLoad);
function onLoad(event) {start();initButton();document.getElementById('id1').addEventListener('input', txtid);document.getElementById('id2').addEventListener('input', txtid);}
function txtid() {if (this.value.length > 2)this.value = this.value.slice(0, 2);}
function initButton() {
document.getElementById('btsv').addEventListener('click', pressed);
document.getElementById('btdf').addEventListener('click', pressed);
document.getElementById('btns').addEventListener('click', pressed);
document.getElementById('btnr').addEventListener('click', pressed);
for (let i = 1; i <= 6; i++)
{
document.getElementById('siz' + i).addEventListener('change', lbchange);
document.getElementById('ln' + i).addEventListener('input', txtin);
document.getElementById('ln' + i).addEventListener('focus', txtin);
}
document.getElementById('scrl').addEventListener('input', function (){if (this.value.length > 120){this.value = this.value.slice(0, 120);}});
document.getElementById('UDS').style.display = "block";
document.getElementById('btnU').className += " active";
}
function openTZ(evt, tzName) 
{var i, tabcontent, tablinks;
tabcontent = document.getElementsByClassName("tabcontent");
for (i = 0; i < tabcontent.length; i++) 
{tabcontent[i].style.display = "none";}
tablinks = document.getElementsByClassName("tablinks");
for (i = 0; i < tablinks.length; i++) 
{tablinks[i].className = tablinks[i].className.replace(" active", "");}
document.getElementById(tzName).style.display = "block";
evt.currentTarget.className += " active";}

function txtin()
{var txtarea = document.getElementById('li');
var name = this.name;
var sizId = "siz" + name.substring(2);
var size = document.getElementById(sizId).value.slice(1);
if (size == "1"){if (this.value.length > 10){this.value = this.value.slice(0, 10);
txtarea.value += "Limiting the text to 10 for Double Size selection\n";
txtarea.scrollTop = txtarea.scrollHeight;}}
else{if (this.value.length > 21){this.value = this.value.slice(0, 21);
txtarea.value += "Limiting the text to 21 for Single Size selection\n";
txtarea.scrollTop = txtarea.scrollHeight;}}}

function lbchange() {var txtarea = document.getElementById('li');
var name = this.name;
var txtId = "ln" + name.substring(3);var size = this.options[this.selectedIndex].text;
var text = document.getElementById(txtId).value;var fsize = "10pt";
if (size == "Double"){fsize = "14pt";document.getElementById(txtId).length = 10;document.getElementById(txtId).value = text.slice(0,10);}
else{document.getElementById(txtId).length = 21;document.getElementById(txtId).value = text.slice(0, 21);}
document.getElementById(txtId).style.fontSize = fsize;
txtarea.value += "Setting Fontsize for text on Line "  + name.substring(3) + " to " +  fsize + "\n";
if (fsize == "10pt")txtarea.value += "Text on Line: " + name.substring(3) + " is limited to 21 characters only\n";
else txtarea.value += "Text on Line: " + name.substring(3) + " is limited to 10 characters only and one line will be reduced\n";
txtarea.scrollTop = txtarea.scrollHeight;}

function tcs(data){var sel = document.getElementById('pts');var opt=data.split('\n');for (var i=0; i < opt.length; i++){if (opt[i].length < 10) break;
var dt = opt[i].split(',');var option = document.createElement("option");var index = dt[0];var val = dt[1].split(';');
option.innerHTML = val[0];option.value = index + ";" + val[1] + ";" + val[2];sel.appendChild(option);}}
function pressed() {var name = this.name;var txtarea = document.getElementById('li');
if (name == 'save') {txtarea.value += "Saving the Texts\n";}
else {txtarea.value += "Loading Defaults\n";}
txtarea.scrollTop = txtarea.scrollHeight;
if (name == "sv"){document.getElementById('ret').innerHTML = "Saving Information. Please wait";}
if (name == "reset"){document.getElementById('ret').innerHTML = "Restarting device. Please wait";}setTimeout(1000);}
</script>
<meta charset="utf-8">
<title>Display Page</title>
  )rawliteral";

char TempStub[] PROGMEM = R"rawliteral(
</head>
<body>
<div class="container">
<h2>Display Page</h2>
<p><span id="ver" style="color:red; font-weight:800"></span></p>
<div class="tab">
  <button id="btnU" class="tablinks" onclick="openTZ(event, 'UDS')">User Defined Settings</button>
  <button id="btnS" class="tablinks" onclick="openTZ(event, 'CLS')">Clock Settings</button>
</div>
<div id="UDS" class="tabcontent">
<h3>User Defined Text Settings</h3>
<form method="POST" action="savedata" enctype="multipart/form-data" id="fop" name="fop" target="_self">
<div class="scnnet">
<table border="1">
<tr class="dp"><td class="hd" colspan="4"><span class="attn">Your Text for Display</span></td></tr>
<tr class="dp"><td class="hd">Line</td><td class="hd">Size</td><td class="hd">Text</td><td class="hd">Color</td></tr>
)rawliteral";

char TempStub1[] PROGMEM = R"rawliteral(
<tr class="dp">
  <td colspan="3"><span>Scroll Text: <input class="scrlb" id="scrl" type="text" name="scrl" maxlength=128 placeholder="Your scroller text goes here"></span></td>
  <td><input type="color" id="clr7" name="clr7" value="#13e7d9"></td>
</tr>
</table>
</div>
<table style="width: 50%; float:left;">
<tr class="dp">
  <td><button id="btsv" class="btn" name="save" type="Submit">Save Texts</button></td>
  <td><button id="btdf" class="btn" name="defa" type="Submit">Load Default</button></td>
</tr>
</table>
</form>
<table style="width: 50%; float:right"><tr>
<td><button id="btin" class="btn" onclick="location.href = '/update'" name="OTA Update">Update Program</button></td>
<td><button id="btca" class="btn" onclick="location.href = '/captive'" name="Captive">WiFi Page</button></td>
</tr>
</table>
<table style="background: #FDF5E6"><tr><td>
  <p class="attn">Note: Use following Short cuts for Scrolling Text: </p>
  <p class="inf">%t: Temperature; %y: Tamil year; %m: Tamil month; %d: Tamil date; %T: Tithi; %s: Star; %p: Paksham <br/>
  Note: %c for clock with 24 hr format and %C for 12 hr format with AM PM indication can be used on any of the Text lines other than scroller</p>
</td></tr></table>
<table><tr>
<td colspan="3"><span>Log Info:</span><textarea class="log" readonly="readonly" id="li"></textarea></td>
</tr></table><div id="logtxt"><table>
<tr class="ti"><td>Current Date: </td><td width="30%"><span id="dt" class="inf"></span></td><td>Room Temperature: <span id="rt" class="inf"></span></td></tr>
<tr class="ti"><td>Tamil Calendar: </td><td colspan="2"><span id="tc" class="inf"></span></td></tr>
<tr class="ti"><td>Weather Report: </td><td colspan="2"><span id="wr" class="inf"></span></td></tr></table>
</div></div>
<div id="CLS" class="tabcontent">
<h3>Clock Settings - Device will reset on Save</h3>
<form method="POST" action="settingssave">
<table style="table-layout:fixed;"><tr>
<td><span><label>Display: </label><br /><select class="scrl" name="disp" id="disp"><option value="clk">Clock</option><option value="dis">User Defined</option></select></span></td>
<td style="width:15%;"><span><label>Id: </label><br /><input id="id1" class="idtxt" name="id1" length=2 placeholder="AU"></span></td>
<td><span><label>Timezone String </label><br /><input id="tz1" class="txt" name="tz1" length=32 placeholder="Australia/Sydney"></span></td>
</tr><tr>
<td><span><label>Configuration:</label><select class="scrl" name="tzs" id="tzs"><option value="dcl">Additional Clock</option><option value="wth">Weather</option></select></span></td>
<td><span><label>Id: </label><br /><input id="id2" class="idtxt" name="id2" length=2 placeholder="AU"></span></td>
<td><span><label>Timezone String </label><br /><input id="tz2" class="txt" name="tz2" length=32 placeholder="Australia/Sydney"></span></td>
</tr><tr><td><span><label>Available Calendars:</label><select class="scrl" name="pts" id="pts"></select></span></td>
<td colspan="2"> <a href='https://panchangam.brahas.com/timezones/tz.html' target="_blank">For Timezone String check this page</a></td>
</tr></table><div style="text-align:center"><p><span><button id="btns" class="btn" name="sv" type="Submit">Save and Restart</button>&nbsp
<button id="btnr" class="btn" name="reset" type="submit">Restart without Save</button>
</span></p></div></form><p>Note: <span id="ret" style="color:red; font-weight:800"></span></p></div><p>&nbsp;</p></div></body></html>
)rawliteral";

//&deg;
//This is the index page. /update will automatically bring OTA page
void handleWeb()
{
  Serial.println("Serving DisplayWeb now");
  char buf[400];
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  // here begin chunked transfer
  server.send(200, "text/html", "");
  server.sendContent(TempPage); //
  server.sendContent(Style);
  server.sendContent(TempStub);
  for (int i = 1; i < 7; i ++)
  {
    sprintf(buf,
"<tr class=\"dp\"><td>%d</td><td><select class=\"scrl\"name=\"siz%d\"id=\"siz%d\">\
<option value=\"%d0\" selected>Single</option><option value=\"%d1\">Double</option></select></td>\
<td><input id=\"ln%d\" class=\"blk\" type=\"text\" name=\"ln%d\" length=21 placeholder=\"Line %d text\"></td>\
<td><input type=\"color\" id=\"clr%d\" name=\"clr%d\" value=\"#db4ddb\"></td></tr>\n",i,i,i,i,i,i,i,i,i,i);
    server.sendContent(buf);            
  }
  server.sendContent(TempStub1);
  server.sendContent(F("")); // this tells web client that transfer is done
  server.client().stop();
  return;
}

void handleWebSocketMessage(uint8_t num, uint8_t *data, size_t len) 
{
  IPAddress ip = ws.remoteIP(num);
  data[len] = 0; //terminate the string
  String sdat = (char*)data;
  int offset = sdat.indexOf('_'); 
  char dat;
  String dt;
  if (offset >= 0)
  {
    //logFile += ClkTZ.dateTime("H:i:s~ ") + "Message from: " + ip.toString() + "\n";
    dat = sdat[0];    
    sdat = sdat.substring(offset+1);

    switch (dat)
    {
      case 'L': //Centigrade requested
      dt = "5" + DisplayData;
      ws.sendTXT(num,dt.c_str(),dt.length());
      break;    

      default:
        //logFile += " Unable to decode the message\r\n";  
        dt += "z"+sdat;
        ws.sendTXT (num,dt.c_str(),dt.length());  
    }

    if (logFile.length() >= 2048)
    {
      //String fn = "/WM-" + ClkTZ.dateTime("Y/F/F-d") + ".log";
      //writeFile(fn.c_str(),logFile.c_str()); 
      logFile = String();
    }
  }
  
}

//handle all events here connect Except for WS_EVT_DATA rest are house keeping.
//num is the IPAddress, type Typeof Event, Payload and length of payload. Will need a terminate '0' to payload
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) 
{
  
  IPAddress ip = ws.remoteIP(num);
  String tr = curTemp;
  tr.replace(" ","&deg; ");
  String dt;
  switch (type) 
  {
    case WStype_CONNECTED:

      Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\r\n", num, ip[0], ip[1], ip[2], ip[3], payload);
      //Send log info
      dt = "4" + logFile;
      ws.sendTXT(num,dt.c_str(),dt.length());
      dt = "5" + DisplayData;
      ws.sendTXT(num,dt.c_str(),dt.length());
      Serial.printf("DisplayData Sent: %s\n",dt.c_str());
      broadcastDailyChanges(num);
      //Send Temperature for quick refresh, lets send the web temperature first. in 10 mins this will get replaced by internal recorded temperature
      dt = String("1") + webTemp +  " °C";
      ws.sendTXT(num,dt.c_str(),dt.length());
      //Send Weather
      broadcastWeatherChanges(num);
      //sprintf(dw,"3Weather prediction for today - Rain now is: %s; Cloud cover: %d; Relative humidity: %d; Wind speed recorded: %d. Expected Minimum / Maximum Temperature: %s&deg; C",Rain.c_str(),Cloud,Humidity,Windspeed,Weather.c_str());
      //ws.sendTXT(num,dw,strlen(dw));
      tr= "7" + ClkTZ.dateTime("H:i:s~ ")  + "Total Connected Clients: " + String(ws.connectedClients()) + "\r\n";
      ws.sendTXT(num,tr.c_str(),tr.length());
      
      //Send Header Settings
      dt = "8" + String(bDisplay ? "dis" : "clk") + ";" + String(bClock ? "dcl" : "wth");
      ws.sendTXT(num,dt.c_str(),dt.length());

      //Send Timezone Settings
      dt = "9" + GetData(CLOCKZONES,-1); 
      ws.sendTXT(num,dt.c_str(),dt.length());

      dt = "a" + GETStreamRequest(PANCHANGAMURL + String(CALENDARFILE));
      ws.sendTXT(num,dt.c_str(),dt.length());

      dt = "b" + String(header.calIndex) + ";" + String(header.calCode) + ";" + String(header.calVer);
      Serial.printf("Value sent for Display Page: %s\n",dt.c_str());
      ws.sendTXT(num,dt.c_str(),dt.length());
      
      dt = String("7") + "Compiled Date Time: " + String(CTIME) + "\n";
      ws.sendTXT(num,dt.c_str(),dt.length());

      if (bVersionChange)
      {
        dt = "cCurrent Program Ver: " + String(header.ver) + String(". A new version ") + String(pVer) + String(" is available.");
        ws.sendTXT(num,dt.c_str(),dt.length());
      }
      ListDir("/",3);
      if (LittleFS.exists("/Debug.txt"))
      {
        dt = "7" + GetData("/Debug.txt", -1);
        //Serial.printf("Contents of Debug.txt: %s\n",dt.c_str());
        ws.sendTXT(num,dt.c_str(),dt.length());
      }
      break;

    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\r\n", num);
      break;

    case WStype_TEXT:
      Serial.printf("[%u] get Text: %s\r\n", num, payload);
      handleWebSocketMessage(num, payload,length); //call the event data to handle message
      break;

    case WStype_BIN:
      Serial.printf("[%u] get binary length: %u\r\n", num, length);
      #ifdef ESP8266
        hexdump(payload, length);
      #endif  
      ws.sendBIN(num, payload, length);//send the message back to client
      break;

    default:
      Serial.printf("Invalid WStype [%d]\r\n", type);
      break;
  }

  if (logFile.length() >= 2048)
  {
    logFile = String();
  }
}

String GetTCString(String pIndex)
{
  uint8_t start;
  uint8_t end;
  String retVal ="";
  String tc;
  if (pIndex != "")
    tc = GetTC(pIndex);
  else
    tc = TamilCalendar;
  start = 0;
  end =  tc.indexOf(" "); //Get Year     
  retVal = tc.substring(start,end);
  
  retVal.concat(" year, Month: " );
  start = end + 1;
  end = tc.indexOf(" ",start); //Get Month
  retVal.concat(tc.substring(start,end)); 
  
  retVal.concat(" Date: ");
  start = end + 1;
  end = tc.indexOf(" ",start); //Get Day
  retVal.concat(tc.substring(start,end)); 

  retVal.concat(", Thithi: ");
  start = end + 1;
  end = tc.indexOf(" ",start); //Get Thithi
  retVal.concat(tc.substring(start,end));

  retVal.concat(", Star: ");
  start = end + 1;
  end = tc.length()-1; //Get Star - last
  retVal.concat(tc.substring(start,end));
  
   //Get Paksham last character
  Serial.printf("Paksham Returned: %s\n",tc.substring(end).c_str());
  retVal.concat((tc.substring(end)=="1" ? ", Shukla Paksham" : ", Krishna Paksham"));
  return retVal;
}

void handleSaveData()
{ 
  if (server.hasArg("defa"))
  {
    Serial.println("default called - Saving Default Text");
    StoreFileFromWeb(DISPLAYTEXT,false);
    bDisplayChange = true;
    Serial.println("- file written");
    DisplayData = GetData(DISPLAYTEXT, -1);
    ws.broadcastTXT("6Successflly Loaded Default texts");
    sendResponse();
    return;
  }

  if (server.hasArg("save"))
  {
    int lineNos;
    char line[5];
    String size;
    char clr[10];
    Serial.println("Save Called");
   
    char data[150];
    int count = 0;  
    uint8_t sz;
    String text;
    String fileText;
    uint8_t totalDoubles = 0;
    for (int i=1;i<7; i++)
    {
      //check for texts and fill up lines
      sprintf(line,"ln%d",i);
      //sprintf(size,"siz%d",i);
      text = server.arg(line);
      size = "siz" + String(i);
      size = server.arg(size);
      
      //Serial.printf("Line: %s, Size: %d Value of Size: %s and substring: %shas Spae?\n",line,(i-1)*3,size.c_str(), size.substring(1).c_str());
      //text.trim(); //do not trim. Could be wantonly added
      if(text.length() > 0) //we will only consider if text is available
      {
        sprintf(clr,"clr%d",i);
        //Serial.printf("Color - %s\n",clr);
        count ++; sz = 0;
        if (size.substring(1) == "1")
        {
          sz++;
          totalDoubles++;
        }
        text = text.substring(0,sz == 0 ? 21 : 10);
        sprintf(data,"%d,%s,\"%s\"\n",sz,server.arg(clr).c_str(),text.c_str());  
        Serial.printf("Data for line: %d is %s\n",i,data);
        fileText.concat(data);
      }
    }
    //Make sure the whole thing will fit in the display area before saving
    
    text = server.arg("scrl");
    //text.trim();
    if (text != "")
      count++;

    //Max lines allowed only 7. Doubles will occupy 2 lines!
    if (count + totalDoubles > MAXLINES)
    {
        Serial.println("More lines than that can fit");
        ws.broadcastTXT("6More lines than that can fit display. Consider reducing Double Size Texts");
        server.send(204, "text/plain", ""); 
        return;
    }

    text = text.substring(0,120);
    sprintf(data,"%d,%s,\"%s\"\n",totalDoubles,server.arg("clr7").c_str(),text.c_str());  
    DisplayData = DISPCONFIG; //Save the head
    DisplayData.concat(String(count) + "\n"); //Save total lines
    DisplayData.concat(data); //Save scroller text
    DisplayData.concat(fileText); //Save all other texts
    Serial.printf("File to be sent:\n%s\n",DisplayData.c_str());

    if(!SaveFile(DISPLAYTEXT,DisplayData))
    {
      ws.broadcastTXT("6Failed to Save Texts. Please Retry again");
      Serial.println("- write failed");
      server.send(204, "text/plain", ""); 
      return;
    }
    else
    {
      Serial.println("- file written");
      ws.broadcastTXT("6Successflly saved the texts");
      bDisplayChange = true;
    }
  }
  sendResponse();
}

//WiFi Save
void handleSettingsSave()
{
  String id;  
  String st;
  Serial.println("Save & Restart Request Initiated");
  Serial.printf("Value obtained for Display is %s\n",server.arg("disp"));

  if (server.arg("disp") == "clk")
    header.bDisplay = false;
  else
    header.bDisplay = true;          

  Serial.printf("Value obtained for Timezone is %s\n",server.arg("tzs"));

  if (server.arg("tzs") == "dcl")
    header.bClock = true;
  else
    header.bClock = false;   

  //Save the calendar information
  st = server.arg("pts");
  Serial.printf("Calendar Argument Received: %s\n",st.c_str());
  int index = st.indexOf(';');
  header.calIndex = st.substring(0,index).toInt();
  index++;
  int nIndex = st.indexOf(';',index);
  if (strcmp(header.calCode,st.substring(index,nIndex).c_str() )!=0)
  {
    strcpy(header.calCode, st.substring(index,nIndex).c_str());
    nIndex++;
    header.calVer = st.substring(nIndex).toInt();
    Serial.printf("Calendar code to be saved: %s and Version int: %d\n",header.calCode,header.calVer);
    RemoveCalendarFiles(ClkTZ.dateTime("Y").toInt());
  }
  //Serial.printf("Header file to be saved with Index: %d, Code: %s, Version: %d and the index:%d\n",header.calIndex,header.calCode,header.calVer,index);
  id = server.arg("id1");
  id.trim();
  st = id + ":";
  id = server.arg("tz1");
  id.trim();
  st += id + ";";
  id = server.arg("id2");
  id.trim();
  st += id + ":";
  id = server.arg("tz2");
  id.trim();
  st += id +  ";\n";
  Serial.printf("Value to be saved: %s\n",st.c_str());
  File file = LittleFS.open(CLOCKZONES, FILE_WRITE);
  if(!file)
  {
    ws.broadcastTXT("6Failed to open file system. Please Retry again");
    Serial.println("- write failed");
    server.send(204, "text/plain", ""); 
    return;
  }
  if(!file.print(st.c_str()))
  {
    ws.broadcastTXT("6Failed to Save Texts. Please Retry again");
    Serial.println("- write failed");
    server.send(204, "text/plain", ""); 
  }
  file.close();

  Serial.printf("Timezone file that was written: %s",st.c_str());
  sendResponse();
  saveCredentials(); 
}

void broadcastWeatherChanges(int num)
{
  char dw[180];
  sprintf(dw,"3Weather prediction for today - Rain now is: %s; Cloud cover: %d; Relative humidity: %d; Wind speed recorded: %d. Expected Minimum / Maximum Temperature: %s&deg; C",Rain.c_str(),Cloud,Humidity,Windspeed,Weather.c_str());
  if (num < 0)
    ws.broadcastTXT(dw,strlen(dw));
  else
    ws.sendTXT(num,dw,strlen(dw));
}
 
void broadcastDailyChanges(int num)
{
  String dt;
  if (num < 0)
  {
    //Send Date     
    dt= "0" + ClkTZ.dateTime("l, jS ~o~f F Y");
    ws.sendTXT(num,dt.c_str(), dt.length()); 
    //Send Tamil Calendar
    dt = "2" + GetTCString("");
    ws.sendTXT(num,dt.c_str(),dt.length());
  }
  else
  {
    //Send Date     
    dt= "0" + ClkTZ.dateTime("l, jS ~o~f F Y");
    ws.broadcastTXT(dt.c_str(),dt.length()); 
    //Send Tamil Calendar
    dt = "2" + GetTCString("");
    ws.broadcastTXT(dt.c_str(), dt.length()); 
  }
}

#line 1 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\Network.ino"
#include "Globals.h"

static void connectWifi()
{
    WiFi.mode(WIFI_AP);
    Serial.print("Scanning Networks...found Network: ");
    int n = WiFi.scanNetworks();
    Serial.println(n);
    unsigned long tick = millis();
    storageIndex = -1;
    Serial.printf("Stored Networks: %d\n",storedNetworks);
    int j;
    for (int i = 0; i < storedNetworks; i++)
    {
      for (j = 0; j < n; j++)
      {
        Serial.printf("Now checking the SSID %s at storage index %d with Scanned Network: %s\n",
              Networks[i].SSID, i, WiFi.SSID(j).c_str());

        if (WiFi.SSID(j).compareTo(String(Networks[i].SSID)) == 0)
        {
            storageIndex = i;
            break;
        }
      }
      if (j < n)  //storageIndex >= 0
      {
        if (tryConnect())
            break;
      }
    }

    if (bConnect)
    {          
      Serial.printf("Connected to Network, IPAddress: %s\n",WiFi.localIP().toString().c_str());
      logFile += "Connected to Network, IPAddress: " + WiFi.localIP().toString() + "\n";
    }
    else
    {
      Serial.println("Not Connected to Network");
    }

    return;
}

//First 
static bool tryConnect()
{
    int i = 0;
    bConnect = false;
    Serial.printf("Trying to connect with SSID: %s, Password: %s\n", Networks[storageIndex].SSID, Networks[storageIndex].Password);

    WiFi.begin(Networks[storageIndex].SSID, Networks[storageIndex].Password);

    int status = WiFi.status();
    int startTime = millis();
    while (status != WL_CONNECTED && status != WL_NO_SSID_AVAIL && status != WL_CONNECT_FAILED && (millis() - startTime) <= WIFI_TIMEOUT * 1000)
    {
        delay(WHILE_LOOP_DELAY);
        status = WiFi.status();
    }

    if (WiFi.status() == WL_CONNECTED)
        bConnect = true;

    Serial.printf("Connection status of SSID: %s is %d\n", Networks[storageIndex].SSID, WiFi.status());
    return bConnect;
}

//Server start when connection fails
static void startServer()
{
  server.begin(); // Web server start
  Serial.println("HTTP server started");
  Serial.printf("BGTask handle: %d\n", bgTask);
  // Setup MDNS responder
  
  if (!MDNS.begin(HOSTNAME))
      Serial.println("Error setting up MDNS responder!");
  else
  {
      Serial.println("mDNS responder started");
      MDNS.addService("http", "tcp", 80);
      MDNS.addService("_http", "_tcp", 80);
      MDNS.addServiceTxt("_http", "_tcp", "board", "ESP32");
  }

  if (!bConnect)
  {
      //bServer = true;
    bCaptive = true;
    if (bgTask != null)
      vTaskDelete(bgTask);
   DisplayServerScreen();
  }
}

static void DisplayServerScreen()
{
    dma_display->setFont();
    dma_display->fillScreen(myBLACK);
    Serial.println("Cleared Screen");
    int line = 3;
    int col = 5;
    
    dma_display->setTextSize(1);
    dma_display->setCursor(col, line);
    dma_display->setTextColor(myGREEN);
    dma_display->println("Web Server Started");
        
    line += SINGLEY + 2;
    dma_display->setCursor(col, line);
    dma_display->setTextColor(myBLUE + myGREEN);
    dma_display->print("IP: ");
    dma_display->println(ESPIP.toString().c_str());
        
    line += SINGLEY + 4;
    dma_display->setCursor(col, line);
    dma_display->setTextColor(myBLUE + myRED);
    dma_display->printf("SSID = %s", ESPSSID);

    line += SINGLEY + 4;
    dma_display->setCursor(col, line);
    dma_display->setTextColor(myGREEN + myRED);
    dma_display->printf("Password = %s", WIFIPWD);
    line += SINGLEY + 4;
    dma_display->setCursor(col, line);
    dma_display->setTextColor(myGREEN);
    dma_display->printf("http://%s/",HOSTNAME);
    Serial.printf("Web Server Started...SSID = %s; Password = %s\n",ESPSSID,WIFIPWD);
    Serial.printf("http://%s/\n",HOSTNAME);
    return;
}


static String GETStreamRequest(String src)
{
  Serial.printf("HTTPClient will use URL:\n%s\n", src.c_str());
  HTTPClient httpclient;
  httpclient.begin(src.c_str());
  String payload = "";  
  int httpCode = httpclient.GET();
  Serial.printf("Received http code as: %d\n", httpCode);
  if (httpCode !=200)
    return payload;
  WiFiClient* stream = httpclient.getStreamPtr();

  bool bUpload = false;
  size_t downloadedDataSize = 0;
  size_t FILE_SIZE = httpclient.getSize();
  if (FILE_SIZE <= 0)
    return payload;
  Serial.printf("File size Received: %d\n",FILE_SIZE);
  while (downloadedDataSize < FILE_SIZE) 
  {
    size_t availableDataSize = stream->available();
    if (availableDataSize > 0) 
    {
      payload += stream->readString();
      downloadedDataSize += availableDataSize;
    }
  }
  httpclient.end();
  //Serial.printf("Payload from StreamRequest: %s",payload.c_str());
  return payload;
}


static String GETRequest(String uri)
{
    HTTPClient http;
    WiFiClient wifiClient;
    Serial.printf("HTTPClient will use URL:\n%s\n", uri.c_str());
    http.begin(wifiClient, uri.c_str());
    int httpResponseCode = http.GET();

    String payload = "{}";

    if (httpResponseCode > 0)
    {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        payload = http.getString();
    }
    else
    {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
    }
    http.end();
    //Serial.printf("Payload from HTTP Client: %s\n", payload.c_str());
    return payload;
}
 

bool StoreFileFromWeb(char* dest,bool bCal)
{
  String src;
  if (!bFS)
    return false;
  if(bCal)  
    src = String(PANCHANGAMURL) + String("/") +  String(header.calCode) + "/" + String(header.calVer)  + String(dest);  //Source is the URL + destination file. Destination file to provide leading /
  else
    src = PANCHANGAMURL + String(dest);  //Source is the URL + destination file. Destination file to provide leading /
 
  Serial.printf("Source for Download: %s; Destination: %s\n",src.c_str(),dest);
  HTTPClient httpclient;
  httpclient.begin(src.c_str());
  int httpCode = httpclient.GET();
  Serial.printf("Received http code as: %d\n", httpCode);
 
  if (httpCode != 200 )
  {
    Serial.printf("Unable to get File: %s\n",src.c_str());
    httpclient.end();
    return false;
  }

  WiFiClient* stream = httpclient.getStreamPtr();  

  String payload;
  File fw = LittleFS.open(dest,FILE_WRITE);
  if (!fw)
  {
    Serial.printf("Unable to open %s file for write\n", dest);
    httpclient.end();
    return false;
  }

  int size = 0;
  while (httpclient.connected() && stream->available()) 
  {
    payload = stream->readString();
    fw.print(payload);
    Serial.printf("File chunk stored: %d\n",(size_t)payload.length());
    size += payload.length();
  }
  httpclient.end();

  Serial.printf("File size received: %d\n",size);
  fw.close();
  httpclient.end();

  //Read the Downloaded file
  String buffer = "";
  File fr = LittleFS.open(dest,FILE_READ);
  if (!fr)
  {
    Serial.println("Unable to open file for read");
    return false;
  }
  else
  {
    if (fr.size() != size)
    {
      fr.close();
      return false;
    }
    Serial.printf("Source File size: %d, Stored File size: %d\n",size,fr.size());
    fr.close();
  }
  return true;
}



String GetData(char* fName, int Lookup)
{
  String data = "";
  if (!bFS)
    return data;
  Serial.printf ("File Name Received: %s; Lookup index: %d\n",fName,Lookup);
  File fr = LittleFS.open(fName,FILE_READ);
  int count = 0;
  if (!fr)
  {
    Serial.printf("Unable to open file for read: %s\n",fName);
    return data;
  }
  if (Lookup < 0)
  {
    while(fr.available())
    {
      data += fr.readString();
    }
    return data;
  }
  else
  {
    while(fr.available())
    {
      for (count = 0;count < Lookup; count ++)
      {
        data = fr.readStringUntil('\n');
      }
      break;
    }
  }
  fr.close();

  if (count == Lookup)  
    return data;
   
  return "";
    
}


/****************************************************************
 * This procedure will first attempt to use our own AWS server 
 * to set the Timezone. If it fails then use the internal plug-in
 * of eztime server. Returns true if it succeeds. Else false
 * Modified: 10th Nov 2024 
 *****************************************************************/
static bool SetTimeZone(Timezone *TZ, String location)
{
    if (TZ->setLocation(location))
      return true;
    Serial.println("Native plug-in failed. Using Brahas plug-in");
    String PosixString = location;
    JSONVar myObj;
    PosixString.toLowerCase();
    PosixString = "{\"filedata\":\"timezones/timezones.json\",\"key\":\"" + PosixString + "\"}";
    Serial.printf("Sending to POST Server: %s\n",PosixString.c_str());
    PosixString = SecurePOSTRequest(TZURL, PosixString);
    Serial.printf("Secure POST returned: %s\n",PosixString.c_str());

    if (PosixString != "" )
    {
      myObj=JSON.parse(PosixString);
      //Serial.printf("TZURL Script succeeded. Setting POSIXString: %s\n",JSON.stringify(myObj["value"]).c_str());
      return (TZ->setPosix(myObj["value"]));
    }
    return false;
}


/****************************************************************
 * This will securely POST to AWS server. This method has been tested to work.
 * https://t7lcs31js3.execute-api.us-east-1.amazonaws.com/production/timezones
 *  is the POST URL. This will refer to Timezone file which is stored at URL:
 *  https://panchangam.brahas.com/timezones/timezones.json
 * Server Family has the utility update to update this JSON file
 * 
 * Created on 10th Feb 2024 - Vasu Prog Ver 1008 - for Scoreboard
 * Modified to suit current requirement on 10th Nov 2024
 ***************************************************************/
static String SecurePOSTRequest(String URL, String httpData)
{
    HTTPClient httpclient;
    WiFiClientSecure wificlient;
    wificlient.setInsecure();

    String payload = "";
    //Serial.printf("Data for POST: %s Host: %s\n",httpData.c_str(),URL.substring(URL.indexOf("//")+ 2).c_str());
    if (httpclient.begin(wificlient, URL))
    {
        httpclient.addHeader("Host", URL.substring(URL.indexOf("//")+ 2));
        httpclient.addHeader("content-type", "application/json");
        httpclient.addHeader("Content-Length", String(httpData.length()).c_str());
        int response = httpclient.POST(httpData);
        payload = httpclient.getString();
        //Serial.printf("Response obtained from server: %d and payload returning: %s\n", response,payload.c_str());
        httpclient.end();
    }
    else
    {
        Serial.println("Unable to connect using httpclient");
    }
    if (payload.indexOf("error")>=0)
      return ""; 

    return payload;
}
#line 1 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\Panch2024-25.ino"
/**********************************************************************
* Tamil Panchangam. Temp storage. Should be in a database and stored in Web
* API call to be used to retrieve information - For future consideration
* Currently embedded. Good until 30th April 2025. Will update every year
* This is the only file required for Tamil Calendar. Can be replace and 
* the main program compiled, without any changes to the main program
* Last updated 14-02-2024. Tamil Calendar based on Sydney time
* ********************************************************************/
#include "Globals.h"

const char Month[][4] = {"NUL","Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};


void CheckVersionChange()
{
  //StoreFile(DATAVERSION); //for checking. to be commented out
  //Serial.println("Checking Version now");
  if (!bFS)  //No File System. Unable to proceed
    return;
  String payload = GETStreamRequest(PANCHANGAMURL+String(VERSIONFILE));

  if (payload == "") //Nothing can be done if no internet
    return;

  uint8_t pStart = payload.indexOf(':');
  uint8_t pEnd = payload.indexOf(';',pStart);
  pVer = payload.substring(pStart+1,pEnd).toInt();
  Serial.printf("Version details: %d Header Version:%d\n",pVer,header.ver);

  if (pVer != header.ver)
    bVersionChange = true;

  return;

}  

//Checks CALVER internet Calendar version and if change detected, 
//stores the newVersion in Config.txt and returns true. 
void CheckCalendarVersion()
{
  String payload = GETStreamRequest(PANCHANGAMURL + String(CALENDARFILE));
  Serial.printf("Calendar.txt returned:\n%s",payload.c_str());
  if (payload == "")
    return;
  int length = payload.indexOf('\n');
  String retVal = ""; 
  int start = 0;

  while (length >0 )
  {
    retVal = payload.substring(start,length);
    //Serial.printf("Calendar.txt line read: %s; Calendar line no: %d; Header.calIndex: %d\n",retVal.c_str(), retVal.substring(0,retVal.indexOf(',')).toInt(),header.calIndex);

    if (retVal.substring(0,retVal.indexOf(',')).toInt() == header.calIndex)
      break;
    start = length + 1;
    length = payload.indexOf('\n',start);    
  }
  if (length < 0) //No data available
    return;

  int codeIndex=retVal.indexOf(',');
  int calIndex = (retVal.substring(0,codeIndex)).toInt();
  codeIndex ++;
  retVal = retVal.substring(codeIndex);  
  //Serial.printf("Calendar Index Obtained: %d & line to manipulate: %s\n",calIndex, retVal.c_str());
  int calVer;
  char calCode[6];
  codeIndex = retVal.indexOf(';');
  codeIndex ++;
  int verIndex = retVal.indexOf(';',codeIndex);
  strcpy(calCode,retVal.substring(codeIndex,verIndex).c_str());  
  calVer = (retVal.substring(verIndex+1)).toInt();
  //Serial.printf("calCode obtained from retVal %s  Version Obtained: %d\n",calCode,calVer);
  

  Serial.printf("Header calCode: %s File CalCode: %s; Header calVer: %d; File calVer: %d\n",header.calCode,calCode,header.calVer,calVer);

  if (calVer != header.calVer || (strcmp(calCode,header.calCode) != 0) )
  {
    Serial.println("Version change detected. Need to reboot");
    Serial.printf("Calendar Index Obtained: %d\n",calIndex);
    //verIndex = retVal.lastIndexOf(';');
    //retVal = String("CalendarURL:") + retVal.substring(0,verIndex)+ String("/") + retVal.substring(verIndex + 1);
    strcpy(header.calCode,calCode);
    header.calVer=calVer;
    header.calIndex = calIndex;
    
    //Remove all Calendar files. Current Year, Previous Year and Next Year. 
    RemoveCalendarFiles(ClkTZ.dateTime("Y").toInt());
    Serial.printf("calVer to Store: %d\n",header.calVer);
    ListDir("/",2);
    saveCredentials();
  }
}

void RemoveCalendarFiles(int yr)
{
  Serial.printf("Year directory for deletion: %d\n",yr);
  char buf[16];
  sprintf(buf, "/%d", yr-1);  //Previous year if exist
  RemoveDir(buf);
  sprintf(buf, "/%d", yr);  //Current year if exist
  RemoveDir(buf);
  sprintf(buf, "/%d", yr+1);  //Next year if exist
  RemoveDir(buf);
  Serial.println("Successfully removed all directories. New files will be loaded on reboot");
  //ListDir("/",2);
}

bool SaveFile(char* fileName,String data)
{
  File file = LittleFS.open(fileName, FILE_WRITE);
  if(!file)
  {
      Serial.println("- failed to open file for writing");
      return false;
  }
  if(file.print(data))
  {
    Serial.println("- file written");
    ws.broadcastTXT("6Successflly saved the texts");
    file.close();
  }   
  else 
  {
    Serial.println("- write failed");
    file.close();
    return false;
  }
  return true;
}

void ListDir(const char * dirname, uint8_t levels)
{
  //Serial.printf("Listing directory: %s\r\n", dirname);
  File root = LittleFS.open(dirname);
  if(!root){
      Serial.println("- failed to open directory");
      ws.broadcastTXT("7-failed to open directory\n");
      return;
  }

  File file = root.openNextFile();
  while(file)
  {
    if(file.isDirectory())
    {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      ws.broadcastTXT(String("7---Dir--- ") + file.name() + String("- \n"));

      if(levels)
      {
          ListDir(file.path(), levels -1);
      }
    } 
    else 
    {
      Serial.print("  FILE: ");
      if (dirname == "/")
      {
        Serial.printf("/%s",file.name());
        ws.broadcastTXT(String("7/") + String(file.name()));
      }
      else
      {
         Serial.printf("%s/%s",dirname,file.name());
         ws.broadcastTXT(String("7") + String(dirname) + String("/") + String(file.name()));
      }
       
      Serial.print("\tSIZE: ");
      Serial.println(file.size());
      ws.broadcastTXT(String("7 SIZE: ") + String(file.size()) + "\n");
    }
    file = root.openNextFile();
  }
}

bool CreateDir(char* dirName)
{
    //Check if directory is available
  Serial.printf("Checking for file: %s\n", dirName);
  if (LittleFS.exists(dirName)) 
  {
    File dr = LittleFS.open(dirName);
    if (!dr.isDirectory())  //If a file with year name exist delete it.
    {
      dr.close();
      LittleFS.remove(dirName);
    }
    else
    {
      dr.close();
      Serial.printf("The said directory %s already exists\n", dirName);
      return true;
    }
  }
  //Check if directory is available
  Serial.printf("Creating Directory as it doesnot exists: %s\n", dirName);
  if (!LittleFS.mkdir(dirName))
  {
    Serial.printf("Unable to create Directory: %s; Unable to proceed\n", dirName);
    return false;
  }
  
  return true;
}

//Make sure you pass the leading '/' in dirName
void RemoveDir(char* dirName)
{
  if (dirName == "/")
  {
    Serial.println("Cannot Remove Root Directory");
    return;
  }
  if (!LittleFS.exists(dirName))
  {
    Serial.printf("Directory does not exist: %s\n", dirName);
    return;
  }

  Serial.printf("Checking for Directory to remove: %s\n", dirName);
  File root = LittleFS.open(dirName);
  String storedFiles = "";
  if(!root)
  {
    Serial.println("- failed to open directory");
    return;
  }
  File file = root.openNextFile();
  char buf[16];
  while(file)
  {
    sprintf(buf,"%s/%s\n",dirName,file.name());
    storedFiles += buf;
    file = root.openNextFile();
  }
  Serial.printf("List of files in the Directory: %s\n%s\n",dirName,storedFiles.c_str());
  int start = 0;
  int end = storedFiles.indexOf("\n");
  while (end > 0)
  {
    Serial.printf("Removing File: %s\n",storedFiles.substring(start,end).c_str());
    LittleFS.remove(storedFiles.substring(start,end).c_str());
    start = end + 1;
    end = storedFiles.indexOf("\n",start);
  }
  //Now remove the directory
  LittleFS.rmdir(dirName);
}

bool GetDataFiles(String year,int month) 
{

  Serial.printf("Year: %s and Month: %d passsed as parameters for CheckFiles\n",year,month);
  //Check for Year file. If not download it
  if (!bFS)  //No File System. Unable to proceed
    return false;

  char buf[16];
  sprintf (buf,"/%s",year.c_str());

  if (!CreateDir(buf))
    return false;

  //Look for Data file. If available download.
  sprintf(buf, "/%s/%s.txt",year,Month[month]);
  Serial.printf("Now looking for file data: %s\n", buf);
  if (!LittleFS.exists(buf)) 
  {
    Serial.printf("Storing data: %s\n",buf);
    if (!StoreFileFromWeb(buf,true))
    {
      //If we just created a directory then we should delete it
      year = String("/") + year;
      LittleFS.rmdir(year);
      return false;  //Current month data essential
    }
  }

  int yr = year.toInt();
  //If first year, remove previous dir and file
  
  if (month == 1)
  {
    sprintf(buf, "/%d", yr-1);  //Need to handle year change
    RemoveDir(buf);
  }
  else
  {  
    //Look for next month Data file. We will only keep two months file
    //First delete previous month data
    //int mnt = ClkTZ.dateTime("m").toInt() - 1;
    sprintf(buf, "/%s/%s.txt", year, Month[month-1]);  
    Serial.printf("Now looking for Previous month file to delete: %s\n", buf);
    if (LittleFS.exists(buf)) 
    {
      Serial.println("Here we are but should not be");
      LittleFS.remove(buf);
    }
  }

  if (month == 12)
  {
    month = 1;
    sprintf(buf,"/%d",yr+1);
    Serial.printf("Creating direcotory: %s\n", buf);
    CreateDir (buf);
    sprintf(buf, "/%d/%s.txt", yr +1, Month[month]);  
  }
  else
  {
    //Now get the next month data
    sprintf(buf, "/%s/%s.txt", year, Month[month + 1]);  
  }

  Serial.printf("Now looking for Next month file to download: %s\n", buf);
  if (!LittleFS.exists(buf)) 
  {
    Serial.printf("Storing data: %s\n",buf);
    StoreFileFromWeb(buf,true);
  }

  return true;
}


String GetTC(String pIndex) 
{
  DayCh = ClkTZ.day(); //if the ClkTZ changes while getting calendar, this will ensure that this procedure is called again
  #ifndef PROD
  //Testing for missing day when calendar version changes
  File file = LittleFS.open("/Debug.txt", FILE_APPEND);
  file.print("TC called @");
  file.print(ClkTZ.dateTime());
  file.print(" String Passed: ");
  file.println(pIndex);
  file.flush();
  file.close();
  //End of Test
#endif
  Tline = 2;
  
  String year = pIndex.substring(0,4);
  int month = pIndex.substring(4,6).toInt();
  if (!GetDataFiles(year,month))
    return DEFAULTDATA;
  int day = pIndex.substring(6).toInt();
  Serial.printf("String of Day: %s and int of day: %d\n",pIndex.substring(6),day);
  char buf[64];
  sprintf(buf, "/%s/%s.txt", year,Month[month]);
  String data = GetData(buf,day);
  Serial.printf("GetData Returned for current date: %s\n", data.c_str());
  if (data == "")
  {
    return DEFAULTDATA;
  }
  //Serial.printf("Calendar Data obtained: %s\n",data.c_str());

  if (data.indexOf("Amavasya") > 0)
  {
    data.setCharAt(data.length()-1, '3');
  }
  else 
  {
    if (data.indexOf("Pournima")>0)
    {  
      data.setCharAt(data.length()-1, '2');
    }
  }
  //Needed for Vertical Centering
  if (data.length() > 41) //Two lines is 42. Last character is for bitmap. So 41
  { 
    //data = data.substring(data.indexOf(" ") +1 );
    Tline = 0;
  }
  //Serial.printf("Calendar Data returning: %s\n",data.c_str());

  #ifndef PROD
  //Testing for missing day when calendar version changes
  File file1 = LittleFS.open("/Debug.txt", FILE_APPEND);
  file1.print(ClkTZ.dateTime());
  file1.print(": Tline Computed: ");
  file1.print(Tline);
  file1.print(" Day changed to: ");
  file1.println(DayCh);
  file1.flush();
  file1.close();
  //End of Test
#endif
  return data;  
}
#line 1 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\Storage.ino"
/** Load WLAN credentials from EEPROM 
* There are 512 byte storage buffer available. We will use 
* this to store up to 6 WiFi configurations 32 byte for SSID
* and 32 byte for password. That will occupy 33*6*2 = 396 bytes in total
* to ensure we have the right data we will store header info hich will be
* a Prog ID and a version info both as UINT16. We will have two flags on the
* header - one for Scoreboard or clock flag and the other for using Porportional 
* font or fixed-pitch font. There is also 21 byte char array to store 
* Game ID - currently unusedT his will be the first
* header information on the EEPROM. Followed by NETWORK array
* We are looking at 16+16+8+8+21 a Total of 69 bytes for header and 396 bytes 
* Stored networks - well under 512 bytes
* 11/02/2024 - Vasu
* ***/
#include "Globals.h"

//If valid data exists in EEPROM read it and return true. else return false
static bool loadCredentials()
{
  EEPROM.begin(512);
  int offset = 0;
  EEPROM.get(offset, header);
 
    //Fresh Program. Needs complete Erase
  if (header.id != PROGID)
  {
    Serial.println("Valid header not found");
    header.id = PROGID;
    header.ver = VERSION;
    header.calIndex = CALINDEX; //Always the first until changed
    strcpy(header.calCode, CALCODE);
    header.calVer = CALVER;
    header.bDisplay = bDisplay;
    header.bClock = bClock;
    EraseStoredValue();
    return true;
  }

  //Only version change. Make modifications to Header file if required
  //Leave the stored network as-is
  ReadNetworkData();

  if (header.ver != VERSION)
  {
    header.id = PROGID;
    header.ver = VERSION;
    header.calIndex = CALINDEX; //Always the first until changed
    strcpy(header.calCode, CALCODE);
    header.calVer = CALVER;
    header.bDisplay = bDisplay;
    header.bClock = bClock;
    if (bFS)
    {   
      if (LittleFS.exists(VERSIONFILE))
        LittleFS.remove(VERSIONFILE);

      if (LittleFS.exists(CONFIG)) 
        LittleFS.remove(CONFIG);
    }
    saveCredentials();  
  }
        
  bDisplay = header.bDisplay;
  bClock = header.bClock;
  Serial.printf("After Version Change bDisplay Value: %d\n", bDisplay);
  Serial.printf ("Header Values read: ID: %d, Version: %d, Calendar Index: %d Calendar Code: %s Calendar Version: %d Display: %d Clock/Weather: %d\n", header.id,header.ver,header.calIndex,header.calCode,header.calVer, header.bDisplay,header.bClock);
  logFile += ("Prog Id: ") + String(header.id) + "; Version: " + String(header.ver) + "; Display: ";
  logFile += String(bDisplay ? "Display Mode" : "Calendar Mode") + " selected\r\n";
  logFile += String("Calendar Index: ") + String(header.calIndex) + " Version: " + String(header.calVer) + "\r\n";
  logFile += "Stored Network: " + String(storedNetworks) + "\r\n";
  Serial.printf("Stored Network: %d\n",(int)storedNetworks );
  return true;   
}

void ReadNetworkData()
{
    int offset = NETWORKOFFSET;
    storedNetworks = 0;
    for (int i = 0; i < MAXNETWORK; i++)
    {
      EEPROM.get(offset, Networks[i]);
      offset += sizeof(NETWORK);
      Serial.printf("READ From Storage[%d]: SSID: %s, Password: %s\n", i + 1, Networks[i].SSID, Networks[i].Password);
      if (strlen(Networks[i].SSID) > 0)
        storedNetworks++;
      
    }
    EEPROM.end();
}

void EraseStoredValue()
{
  for (int i = 0; i < MAXNETWORK; i++)
  {
      memset(Networks[i].SSID, '\0', sizeof(Networks[i].SSID));
      memset(Networks[i].Password, '\0', sizeof(Networks[i].Password));
  }

  Serial.println("Erasing the stored contents");
  saveCredentials();
}

/** Store WLAN credentials to EEPROM */
static void saveCredentials()
{
  Serial.println("Saving Credentials");
  EEPROM.begin(512);

  int offset = 0;
  EEPROM.put(offset, header);
  offset = NETWORKOFFSET;

  for (int i = 0; i < MAXNETWORK; i++)
  {
      EEPROM.put(offset, Networks[i]);
      offset += sizeof(NETWORK);
  }
  EEPROM.commit();
  EEPROM.end();
  delay(5000);
  ESP.restart();
}


#line 1 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\WeatherDateTime.ino"
#include "Globals.h"

/*********************************************************************
/* Following function will get the Gateway address, Location and time zone
 * Note time zone must be constructed from "country_name" / "city_name"
 * Location details are "latitude=-34.47&longitude=150.42&" 
 * Timezone = Australia/Sydney which are to 
 * be stored in Location and TimeZone Global variable 
 * This program is called at the start of the day and also once every day 
 * This program is the lifeline for clock. If there is no connection or
 * unable to get details, the program will revert back to server mode
 * Hence 5 attempts are made to make to the site
 * 
 * Site is free for personal usage. URL: https://api.ip2location.io/?key= + key obtained on registering
 * Result Obtained: {"ip":"122.151.141.222","country_code":"AU","country_name":"Australia",
 * "region_name":"New South Wales","city_name":"Sydney","latitude":-33.86778,"longitude":151.207052,
 * "zip_code":"2000","time_zone":"+11:00","asn":"9443","as":"Vocus Retail","is_proxy":false}
 * Changed on 9th Feb 2024 - Vasu
 *****************************************************************/
static bool GetLocation()
{
    Serial.printf("Getting Location URL: %s, Key= %s\n", LocationUri.c_str() , LocationKey.c_str());
    int count = 0;
    String jsonArray;
    JSONVar myObj;


    while (count < 5)
    {
        count++;
        jsonArray = GETRequest(LocationUri + LocationKey);
        myObj = JSON.parse(jsonArray);
        if (JSON.typeof(myObj) == "undefined" || jsonArray == "")
        {
            delay(RETRY); //Wait for 5 second and try again
            continue;
        }
        String l = JSON.stringify(myObj["ip"]);
        l.replace("\"","");
        Serial.printf("ip: %s\n", l.c_str());

        if (l.c_str() == "null")
        {
          Serial.printf("ip null detected: %s\n", JSON.stringify(myObj["ip"]).c_str());
          delay(RETRY); //Wait for 2 seconds and try again
          continue;
        }

        //Used by older URL. Not used
        //Location = "latitude=" + JSON.stringify(myObj["latitude"]);
        //Location += "&longitude=" + JSON.stringify(myObj["longitude"]) + String("&");
        //Location.replace("\"", "");

        l = JSON.stringify(myObj["loc"]);
        l.replace("\"", "");
        int index = l.indexOf(',');

        Location =  "latitude="   + l.substring(0, index);
        Location += "&longitude=" + l.substring(index + 1) + String("&");

        
        //Older URL. Not used
        //TimeZone = JSON.stringify(myObj["country_name"]) + "/" + JSON.stringify(myObj["city_name"]);
        //TimeZone.replace("\"", "");
        
        TimeZone = JSON.stringify(myObj["timezone"]);
        TimeZone.replace("\"", "");
       
        WeatherUri = "";  //Reset Weather uri in case some things have changed
        break;
        

    }
    waitForSync();
    Serial.printf("Location: %s, TimeZone: %s obtained\n",Location.c_str(), TimeZone.c_str());
    ws.broadcastTXT(String("7TimeZone from Web: " )+ TimeZone + String("\r\n"));
    
    if (!SetTimeZone(&ClkTZ,TimeZone))
        Serial.printf("Unable to set Time zone for location: %s\n",TimeZone.c_str());
    else
        Serial.printf("Time Now for Location: %s is %s\n",TimeZone.c_str(), ClkTZ.dateTime().c_str());

    delay(SECOND); //Wait to get the time
    #ifndef PROD
        //Testing for missing day when calendar version changes
        File file = LittleFS.open("/Debug.txt", FILE_APPEND);
        file.print("Setting time after Reset: ");
        file.println(ClkTZ.dateTime());
        file.flush();
        file.close();
    #endif
    Serial.printf("Time obtained for %s now is: %s\n",Ad1TZ.getOlson().c_str(), Ad1TZ.dateTime(RFC1036).c_str());    
    delay(SECOND); //Wait to get the time  
    Serial.printf("Time obtained for %s now is: %s\n",Ad2TZ.getOlson().c_str(), Ad2TZ.dateTime(RFC1036).c_str());    

    if (count < 5)
        return true;
    else
        return false;

}
/***********************************************************************
 * Weather is called every 25 minutes This can be changed.
 * This function will construct WeatherURI before calling. So ensure Location and TimeZone
 * are properly filled. If not this will automatically call Get Location
 * Requirements for WeatherUri are:
 * WUri  + Location  + TimeZone + "&current=temperature_2m,rain,cloud_cover&hourly=temperature_2m,rain,
 * cloud_cover&daily=weather_code,temperature_2m_max,temperature_2m_min,rain_sum&forecast_days=1";
 * 
 * Modified on 9th February 2024 - Vasu
 **************************************************************************/
static bool GetWeather()
{
    Serial.printf("Weather URI in the system: %s\n",WeatherUri.c_str());
    if (Location == "" || TimeZone == "")
    {
      //Everytime we get Weather, we will get location as well. This will ensure the time zone to correct itself
        if (!GetLocation())
        {
            delay(TIMEDELAY);
            ESP.restart();
        }
    }
    if (WeatherUri == "")
    {
        WeatherUri =  WUri + Location + TimeZone;
        //Current temperature is removed and actual sensor is used - ver 1014 change
        WeatherUri += "&current=temperature_2m,rain,cloud_cover,relative_humidity_2m,wind_speed_10m";

        //WeatherUri += "&cloud_cover,relative_humidity_2m,wind_speed_10m";
        WeatherUri += "&daily=weather_code,temperature_2m_max,temperature_2m_min,&minutes=15"; 
        // https://api.open-meteo.com/v1/forecast?
        // latitude=-33.86778&longitude=151.207052&Australia/Sydney&
        // current=temperature_2m,rain,cloud_cover,relative_humidity_2m,wind_speed_10m
        // &daily=weather_code,temperature_2m_max,temperature_2m_min,rain_sum&forecast_days=1

    }

    Serial.printf("Weather URI that will be used is: %s\n", WeatherUri.c_str());
    int count = 0;
    String jsonArray;
    JSONVar myObj;

    //try 4 times to ensure we get result. If not reboot
    while (count < 5)
    {
        count++;
        jsonArray = GETRequest(WeatherUri);
        myObj = JSON.parse(jsonArray);
        if (JSON.typeof(myObj) == "undefined" || jsonArray == "")
        {
            delay(RETRY);
            continue;
        }

        if (myObj["error"])
        {
            delay(RETRY);
            continue;
        }

        break;

    }
    if (count >= 5)
    {
        delay(TIMEDELAY);
        ESP.restart();
    }

    /* **********
     * The following Temperature variable is used if temperature sensor fails
     * **********/

    webTemp = JSON.stringify(myObj["current"]["temperature_2m"]);
    webTemp.replace("\"", "");

    if (webTemp == null || webTemp == "null") //Occassionally we get null result from the web. If that is the case, we will ignore and use old values
    {
      webTemp = "";
      Serial.println("webTemp failed");
      return false;
    }    

    Serial.printf("Current web Temperature: %s\n", webTemp.c_str());
      
    if (logFile.length() >= 2048)
    {
      logFile = "Message Overload. Old Messages deleted\r\n";
    }
    
    String wr = JSON.stringify(myObj["daily"]["temperature_2m_min"]);
    Weather = wr.substring(1,wr.indexOf(','));
    Weather += "/";
    wr = JSON.stringify(myObj["daily"]["temperature_2m_max"]);
    Weather += wr.substring(1,wr.indexOf(','));

    Serial.printf("Weather now is: %s\n", Weather.c_str());

    Rain = JSON.stringify(myObj["current"]["rain"]);
    Rain.replace("\"", "");
    Serial.printf("Rain now is: %s\n", Rain.c_str());
    Cloud = myObj["current"]["cloud_cover"];
    Serial.printf("Cloud Cover now is: %d\n", Cloud);
    Humidity = myObj["current"]["relative_humidity_2m"];
    Serial.printf("Current Humidity is: %d\n", Humidity);
    Windspeed = myObj["current"]["wind_speed_10m"];
    Serial.printf("Wind speed now is: %d\n", Windspeed);
    Weather.replace("[", "");
    Weather.replace("]", "");
    bNetUpdate = !bNetUpdate;
    broadcastWeatherChanges(-1); 
    return true;
}
