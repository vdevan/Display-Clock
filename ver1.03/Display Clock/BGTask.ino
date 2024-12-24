

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