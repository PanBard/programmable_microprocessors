#include "ESP8266WiFi.h"
#include "ESP8266HTTPClient.h"
#include "WiFiClient.h"
// WiFi parameters to be configured
const char* ssid = "DESKTOP-DB3900H 6599"; // Write here your router's username
const char* password = "8S6m506*"; // Write here your router's passward

int temperature = 50;

String URL = "http://192.168.0.185/esp8266/index.php";

    HTTPClient http;
    WiFiClient client;

void setup(void)
{ 
  Serial.begin(9600);
  connectToWIFI();

}
void loop() {
 checkWifiConnection();
sendData();

  delay(5000);
}

void checkWifiConnection(){
   if(WiFi.status() != WL_CONNECTED){
      connectToWIFI();
      }
}

void sendData(){
    String data = "temperature="+String(temperature);

    http.begin(client, URL);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int httpCode = http.POST(data);
    String payload = http.getString();

    Serial.print("data: "); Serial.println(data);
    // Serial.print("http: "); Serial.println(httpCode);
    // Serial.print("payload: "); Serial.println(payload);
}

void connectToWIFI(){
    // Connect to WiFi
  WiFi.begin(ssid, password);

  // while wifi not connected yet, print '.'
  // then after it connected, get out of the loop
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
     Serial.println(WiFi.status());
  }
  //print a new line, then print WiFi connected and the IP address
  Serial.println("");
  Serial.println("WiFi connected");
  // Print the IP address
  Serial.println(WiFi.status());
  Serial.println(WiFi.localIP());
}