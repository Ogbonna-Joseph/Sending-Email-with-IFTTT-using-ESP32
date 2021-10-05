#include <WiFi.h>


// Auxiliary variables (it will only detect changes that are 1500 milliseconds apart)
unsigned long previousMillis = 0; 
const long interval = 1500;

const char* ssid = "JOSEPH";
const char* password = "00000000";
const char* host = "maker.ifttt.com";
const char* apiKey = "pEbV6M8QVTmc9S43r2jfrWmy488E8hi61OqZr4lun4B";


void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  pinMode(14, OUTPUT);


  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");  
}

void onandoff(){
  digitalWrite(14, HIGH);
  delay(10000);
  digitalWrite(14, LOW);
  delay(10000);
  }

void loop() {
//  if (changeState){
    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      onandoff();
      // If a state has occured, invert the current door state  
       
     

      //Send email
      Serial.print("connecting to ");
      Serial.println(host);
      WiFiClient client;
      const int httpPort = 80;
      if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;
      }

      String url = "/trigger/door_status/with/key/";
      url += apiKey;

      Serial.print("Requesting URL: ");
      Serial.println(url);
      client.print(String("POST ") + url + " HTTP/1.1\r\n" +
                     "Host: " + host + "\r\n" + 
                     "Content-Type: application/x-www-form-urlencoded\r\n" + 
                     "Content-Length: 13\r\n\r\n" +
                     "value1=" + "myrequest" + "\r\n");
    }  
  }
