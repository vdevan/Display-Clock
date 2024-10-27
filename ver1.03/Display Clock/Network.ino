
static void connectWifi()
{
    //Serial.println("Connecting as wifi client...");
    int n = WiFi.scanNetworks();
    unsigned long tick = millis();
    storageIndex = -1;
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
    if (bgTask != null)
      vTaskDelete(bgTask);
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
  }
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