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

