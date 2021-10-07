#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <HTTPClient.h>
#include "AzureIotHub.h"
#include "Esp32MQTTClient.h"

const char* ssid = "JOSEPH";
const char* password = "00000000";
const char* host = "maker.ifttt.com";
const char* apiKey = "pEbV6M8QVTmc9S43r2jfrWmy488E8hi61OqZr4lun4B";
String serverName = "https://maker.ifttt.com/trigger/door_status/with/key/pEbV6M8QVTmc9S43r2jfrWmy488E8hi61OqZr4lun4B";
static const char* connectionString = "HostName=IOT-HUB-ESP.azure-devices.net;DeviceId=ESP-02;SharedAccessKey=3K/Nc5tebS7PLGbIjh6Eh7/WqAj0ZyNCZz2vzDzYjOc=";

static bool hasIoTHub = false;

String ledon = "on";
String ledoff = "off";


char homepage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
   <head>
      <meta name="viewport" content="width=device=width, initial-scale=1\">
      <title>WEB AUTOMATION</title>
      <style>
        html{font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center; background-color: yellow;}
        body{margin-top: 50px;}
        h1{color: #444444; margin: 50px auto 30px;}
        h3{margin-bottom: 50px; color: #444444;}
        .button{background-color: green; }
        footer{font-size: 15px; color: blue;}
      </style>
      <script>
       
      </script>
    </head>
  <body>
    <h1>HOME AUTOMATION </h1>
    <h3>This is An Automation Project</h3>
    <p>This is a simple esp32 project to control some devices via the web</P>
    <p>Turn on or off the FAN</p>
    <p id="fan"></p>
    <a href="/fanon"><button type="button" class="button button-on redclr" onclick="onled1()">ON</button></a>
    <a href="/fanoff"><button type="button" class="button button-off" onclick="offled1()">OFF</button></a>
    <p>Turn on or off the AC</p>
    <p id="AC"></p>
    <a href="/ACon"><button type="button" class="button button-on greenclr" onclick="onled2()">ON</button></a>
    <a href="/ACoff"><button type="button" class="button button-off" onclick="offled2()">OFF</button></a>
    <p>Turn on or off the BULB</p>
    <p id="bulb"></p>
    <a href="/bulbon"><button type="button" class="button button-on greenclr" onclick="onled3()">ON</button></a>
    <a href="/bulboff"><button type="button" class="button button-off" onclick="offled3()">OFF</button></a>
    <p>Turn on or off the FRIDGE</p>
    <p id="fridge"></p>
    <a href="/fridgeon"><button type="button" class="button button-on greenclr" onclick="onled3()">ON</button></a>
    <a href="/fridgeoff"><button type="button" class="button button-off" onclick="offled3()">OFF</button></a>
    <p>Turn on or off all Appliance</p>
    <p id="all"></p>
    <a href="/allon"><button type="button" class="button button-on greenclr" onclick="onled3()">ON</button></a>
    <a href="/alloff"><button type="button" class="button button-off" onclick="offled3()">OFF</button></a>
    <br>
    <br>
    <br>
    <footer><B>AUTOMATION BY JOSEPH</B></footer>
  </body>
  </html>)=====";


WebServer server(80);

const int led =  2;
const int fan =  12;
const int AC =  26;
const int fridge =  14;
const int bulb = 27;


String azureiot(String Status){
 
  Serial.println("start sending events.");
  if (hasIoTHub)
  {
    char buff[128];

    // replace the following line with your data sent to Azure IoTHub
    snprintf(buff, 128, "{\"DEVICE_ID\":\"JOSEPH_ESP32\", \"LOCATION\":\"PORT HARCOURT\", \"STATUS\":\Status\}");
    
    if (Esp32MQTTClient_SendEvent(buff))
    {
      Serial.println("Sending data succeed");
    }
    else
    {
      Serial.println("Failure...");
    }
    delay(5000);
  }
  
  }

void sendemail(){
  
      HTTPClient http;

      String serverPath = serverName;
      
      // Your Domain name with URL path or IP address with path
      http.begin(serverPath.c_str());
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
  }

void allon(){
    digitalWrite(fan, HIGH);
    delay(500);
    digitalWrite(AC, HIGH);
    delay(500);
    digitalWrite(bulb, HIGH);
    delay(500);
    digitalWrite(fridge, HIGH);
    delay(500);
    Serial.println("the appliances are all turned on");
    sendemail();
    server.send(200, "text/html", homepage);
    azureiot("ALL APPLIANCE = ON");
    }

void fanon(){
    digitalWrite(fan, HIGH);
    Serial.println("the fan is being turned on");
    sendemail();
    server.send(200, "text/html", homepage);
    azureiot("FAN = ON");
    }
    
void ACon(){
    digitalWrite(AC, HIGH);
    Serial.println("the AC is being turned on");
    sendemail();
    server.send(200, "text/html", homepage);
    azureiot("AC = ON");
    }

void bulbon(){
    digitalWrite(bulb, HIGH);
    Serial.println("the bulb is being turn on");
    sendemail();
    server.send(200, "text/html", homepage);
    azureiot("BULB = ON");
    }

void fridgeon(){
    digitalWrite(fridge, HIGH);
    Serial.println("the fridge is being turned on");
    sendemail();
    server.send(200, "text/html", homepage);
    azureiot("FRIDGE = 0N ");
    }
void alloff(){
    digitalWrite(fan, LOW);
    delay(500);
    digitalWrite(AC, LOW);
    delay(500);
    digitalWrite(bulb, LOW);
    delay(500);
    digitalWrite(fridge, LOW);
    delay(500);
    Serial.println("the appliances are all turned oFF");
    sendemail();
    server.send(200, "text/html", homepage);
    azureiot("ALL APPLIANCE = OFF");
    }

void fanoff(){
    digitalWrite(fan, LOW);
    Serial.println("the fan is being turned off");
    sendemail();
    server.send(200, "text/html", homepage);
    azureiot("FAN = OFF");
    }
    
void ACoff(){
    digitalWrite(AC, LOW);
    Serial.println("the AC is being turned off");
    sendemail();
    server.send(200, "text/html", homepage);
    azureiot("AC = OFF");
    }

void bulboff(){
    digitalWrite(bulb, LOW);
    Serial.println("the bulb is being turned off");
    sendemail();
    server.send(200, "text/html", homepage);
    azureiot("BULB = OFF");
    }

void fridgeoff(){
    digitalWrite(fridge, LOW);
    Serial.println("the fridge is being turned off");
    sendemail();
    server.send(200, "text/html", homepage);
    azureiot("FRIDGE = OFF");
    }

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/html", homepage);
  digitalWrite(led, 0);
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void) {
  pinMode(fan, OUTPUT);
  pinMode(AC, OUTPUT);
  pinMode(fridge, OUTPUT);
  pinMode(bulb, OUTPUT);
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("webautomation")) {
    Serial.println("MDNS responder started");
  }

    if (!Esp32MQTTClient_Init((const uint8_t*)connectionString))
  {
    hasIoTHub = false;
    Serial.println("Initializing IoT hub failed.");
    return;
  }
  hasIoTHub = true;


  server.on("/", handleRoot);
  server.on("/fanon", fanon);
  server.on("/fanoff", fanoff);
  server.on("/ACon", ACon);
  server.on("/ACoff", ACoff);
  server.on("/bulbon", bulbon);
  server.on("/bulboff", bulboff);
  server.on("/fridgeon", fridgeon);
  server.on("/fridgeoff", fridgeoff);
   server.on("/allon", allon);
  server.on("/alloff", alloff);


  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}
