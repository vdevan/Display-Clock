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
