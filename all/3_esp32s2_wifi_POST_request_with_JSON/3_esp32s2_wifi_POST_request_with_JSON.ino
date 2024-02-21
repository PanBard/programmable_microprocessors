#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFiMulti.h>
 
const char *AP_SSID = "Konsoleta";
const char *AP_PWD = "---";
  
WiFiMulti wifiMulti;
 
void setup() {
  Serial.begin(9600);
  wifiMulti.addAP(AP_SSID);
 
  postDataToServer();
}
 
void loop() {
  // Not used in this example
   postDataToServer();
   delay(3000);
}
 
void postDataToServer() {
 
  Serial.println("Posting JSON data to server...");
  // Block until we are able to connect to the WiFi access point
  if (wifiMulti.run() == WL_CONNECTED) {
     
    HTTPClient http;   
     
    http.begin("http://8.8.8.8/post");  
    http.addHeader("Content-Type", "application/json");         
     
    StaticJsonDocument<200> doc;

    doc["message"] = "up";
   
   
    
    String requestBody;
    serializeJson(doc, requestBody);
     
    int httpResponseCode = http.POST(requestBody);
 
    if(httpResponseCode>0){
       
      String response = http.getString();                       
       
      Serial.println(httpResponseCode);   
      Serial.println(response);
     
    }
    else {
     
      Serial.printf("Error occurred while sending HTTP POST: %s\n");
       
    }
     
  }
}