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

