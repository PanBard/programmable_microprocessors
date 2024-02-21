#ifndef ESP32
#error This code is designed to run on ESP32 platform (ESP32 S2 mini [LOLIN S2 Mini]), not Arduino nor ESP8266! Please check your Tools->Board setting.
#endif
#include <SPI.h>
#include <Wire.h>  //for custom sda, scl define
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <ArduinoJson.h> // by Benoit Blanchon
#include "index.h"  //Web page header file
#include "lora_stuff_for_LoraDashboard.h"

#ifndef SSID
#define SSID "Lora-dashboard"
#endif

// ------------------wifi
const char *softAP_ssid = SSID;
IPAddress apIP(8, 8, 8, 8);//It uses the default Google DNS IP address 8.8.8.8 to capture all  Android dns requests
IPAddress netMsk(255, 255, 255, 0);
const byte DNS_PORT = 53; // DNS server 
DNSServer dnsServer; // DNS server 
WebServer server(80);// Web server
// ------------------wifi

//-----json
StaticJsonDocument<250> jsonDocument; //for deserialize json
String postMessage = ""; //message from POST request
String jsonString = ""; //json string for get request from webapp
//-----json

String previousMessage = ""; //to dont replicate  message send to server

int bandwidth = 0;
int power = 0;
long milis_time = 0; //current time in miliseconds
String current_time =""; //string for time format

// check if this string is an IP address
boolean isIp(String str) {
  for (size_t i = 0; i < str.length(); i++) {
    int c = str.charAt(i);
    if (c != '.' && (c < '0' || c > '9')) {
      return false;
    }
  }
  return true;
}

String toStringIp(IPAddress ip) {
  String res = "";
  for (int i = 0; i < 3; i++) {
    res += String((ip >> (8 * i)) & 0xFF) + ".";
  }
  res += String(((ip >> 8 * 3)) & 0xFF);
  return res;
}

// checks if the request is for the controllers IP, if not we redirect automatically to the captive portal 
boolean captivePortal() {
  if (!isIp(server.hostHeader())) {
    Serial.println(".");
    server.sendHeader("Location", String("http://") + toStringIp(server.client().localIP()), true);
    server.send(302, "text/plain", "");   
    server.client().stop(); 
    return true;
  }
  return false;
}

void handleRoot() {
  if (captivePortal()) { 
    return;
  }
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  String page = MAIN_page; //Read HTML contents
  server.send(200, "text/html", page);
}

void handleNotFound() {
  if (captivePortal()) { 
    return;
  }
  String message = F("File Not Found\n\n");
  message += F("URI: ");
  message += server.uri();
  message += F("\nMethod: ");
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += F("\nArguments: ");
  message += server.args();
  message += F("\n");

  for (uint8_t i = 0; i < server.args(); i++) {
    message += String(F(" ")) + server.argName(i) + F(": ") + server.arg(i) + F("\n");
  }
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send(404, "text/plain", message);
}

void sendLoraLogs(){
  server.send(200, "text/plane", logs); //Send lora message to client ajax request
}

String getCurrentTime() {
    milis_time = millis(); //take current time in miliseconds
    current_time = String((milis_time/1000)/3600) + "h "+ String(((milis_time/1000)%3600)/60) + "m " + String((milis_time/1000)%60) + "s ";
    return current_time;
}

String makeJSONString(){
  return  String("{")+
      "\"counter\":\""+String(counter)+"\","+
      "\"connections\":\""+String(WiFi.softAPgetStationNum())+"\","+
      "\"bandwidth\":\""+String(bandwidth)+"\","+
      "\"power\":\""+String(power)+"\","+
      "\"time\":\""+getCurrentTime()+"\""+
      "}";
}

void sendDataToWebServer(){
  jsonString = makeJSONString();
  Serial.println(jsonString);
  server.send(200, "application/json", jsonString);
  jsonString = "";
}
void handleLoraMessageSender(){
   if (server.hasArg("plain") == false) {
  }
    blink();
  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);
  String messageFromJSON = jsonDocument["message"];
  logs += ("[POST] request: "+messageFromJSON+"\n");
  Serial.println("POST: "+String(messageFromJSON));
  Serial.print("JSON POST: ");
  lora_send_message(String(messageFromJSON));
  server.send(200, "application/json", "{}");
}

void handlePost() {
  if (server.hasArg("plain") == false) {
  }
    blink();
  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);

  String messageFromJSON = jsonDocument["message"];
  postMessage = messageFromJSON;
  logs += ("[POST] request: "+postMessage+"\n");
  Serial.println("POST: "+String(postMessage));
  Serial.println("POST body: "+String(body));
  change_parameter();
  server.send(200, "application/json", "{}");
}

void wifi_start() {
  Serial.println();
  Serial.println("Configuring access point...");
  WiFi.softAPConfig(apIP, apIP, netMsk);
  // its an open WLAN access point without a password parameter
  WiFi.softAP(softAP_ssid);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
  /* Setup the DNS server redirecting all the domains to the apIP */
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", apIP);

  /* Setup the web server */
  server.on("/", handleRoot);
  server.on("/generate_204", handleRoot);
  server.on("/readLoraLogs", sendLoraLogs);//To get update ajax
  server.on("/getData", sendDataToWebServer);
  //------------------------------------------
    server.on("/post", HTTP_POST, handlePost); 
    server.on("/loraMessageSender", HTTP_POST, handleLoraMessageSender); 
  //-------------------------------------------
  server.onNotFound(handleNotFound);
  server.begin(); // Web server start
  Serial.println("HTTP server started");

  blink();
  delay(100);
  blink();
  delay(100);
  blink();
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  lora_start(); //LORA setup
  wifi_start(); //WIFI setup
}

void loop() {
  loraEngineReceiver();//lora
  dnsServer.processNextRequest();//DNS
  server.handleClient(); //HTTP
}

