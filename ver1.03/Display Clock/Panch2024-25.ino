/**********************************************************************
* Tamil Panchangam. Temp storage. Should be in a database and stored in Web
* API call to be used to retrieve information - For future consideration
* Currently embedded. Good until 30th April 2025. Will update every year
* This is the only file required for Tamil Calendar. Can be replace and 
* the main program compiled, without any changes to the main program
* Last updated 14-02-2024. Tamil Calendar based on Sydney time
* ********************************************************************/

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
  //Serial.printf("Calendar.txt returned:\n%s",payload.c_str());
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

  retVal = retVal.substring(retVal.indexOf(',') +1);  
  
  //Serial.printf("Calendar line to manipulate: %s\n",retVal.c_str());
  uint16_t calVer;
  char calCode[6];
  int codeIndex = retVal.indexOf(';');
  codeIndex ++;
  int verIndex = retVal.indexOf(';',codeIndex);
  strcpy(calCode,retVal.substring(codeIndex,verIndex).c_str());
  //Serial.printf("calCode obtained from retVal %s  Variable calCode: %s\n",retVal.substring(codeIndex,verIndex).c_str(),calCode);
  
  calVer = retVal.substring(verIndex+1).toInt();
  //Serial.printf("calVer obtained from retVal %s  Variable calVer: %d\n",retVal.substring(verIndex + 1).c_str(),calVer);

  if ( calVer != header.calVer || (strcmp(calCode,header.calCode) != 0) )
  {
    Serial.println("Version change detected. Need to reboot");
    //verIndex = retVal.lastIndexOf(';');
    //retVal = String("CalendarURL:") + retVal.substring(0,verIndex)+ String("/") + retVal.substring(verIndex + 1);
    strcpy(header.calCode,calCode);
    header.calVer=calVer;
    
    //Remove all Calendar files. Current Year, Previous Year and Next Year. 
    RemoveCalendarFiles(ClkTZ.dateTime("Y").toInt());
    /*Serial.printf("Year directory for deletion: %d\n",yr);
    char buf[16];
    sprintf(buf, "/%d", yr-1);  //Previous year if exist
    RemoveDir(buf);
    sprintf(buf, "/%d", yr);  //Current year if exist
    RemoveDir(buf);
    sprintf(buf, "/%d", yr+1);  //Next year if exist
    RemoveDir(buf);
    Serial.println("Successfully removed all directories. New files will be loaded on reboot");
    ListDir("/",2);*/
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
      return;
  }

  File file = root.openNextFile();
  while(file)
  {
    if(file.isDirectory())
    {
      Serial.print("  DIR : ");
      Serial.println(file.name());
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
        //ws.broadcastTXT(String("7/") + String(file.name()));
      }
      else
      {
         Serial.printf("%s/%s",dirname,file.name());
          //ws.broadcastTXT(String("7") + String(dirname) + String("/") + String(file.name()));
      }
       
      Serial.print("\tSIZE: ");
      Serial.println(file.size());
      //ws.broadcastTXT(String("7 SIZE: ") + String(file.size()) + "\n");
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
  if (data.length() > 43) //Two lines is 42. Last character is stripped out. So 43
  { 
    Tline = 0;
  }
  //Serial.printf("Calendar Data returning: %s\n",data.c_str());
  return data;  
}