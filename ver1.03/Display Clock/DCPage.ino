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

      if (bVersionChange)
      {
        dt = "cCurrent Program Ver: " + String(header.ver) + String(". A new version ") + String(pVer) + String(" is available.");
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
