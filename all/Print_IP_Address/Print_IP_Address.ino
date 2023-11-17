/*
 * Connects the ESP8266 NodeMCU board to wifi and prints the IP address
 * 
 * by:
 * ROBOSANS
 * based on ThingSpeak Library example
 * 
 * https://www.robosans.com/
 */

#include "ESP8266WiFi.h"

// WiFi parameters to be configured
const char* ssid = "DESKTOP-DB3900H 6599"; // Write here your router's username
const char* password = "8S6m506*"; // Write here your router's passward

void setup(void)
{ 
  Serial.begin(9600);
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
void loop() {
  // Nothing
}
