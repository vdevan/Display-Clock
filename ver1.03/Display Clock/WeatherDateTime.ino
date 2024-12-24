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