#ifndef ESP32
#error This code is designed to run on ESP32 platform (ESP32 S2 mini [LOLIN S2 Mini]), not Arduino nor ESP8266! Please check your Tools->Board setting.
#endif
#include <SPI.h>
#include <LoRa.h> // by Sandeep Mistry
#include <Wire.h>  //for custom sda, scl define
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include "index.h"  //Web page header file

#ifndef SSID
#define SSID "open"
#endif

// ------------------wifi
const char *softAP_ssid = SSID;
IPAddress apIP(8, 8, 8, 8);//It uses the default Google DNS IP address 8.8.8.8 to capture all  Android dns requests
IPAddress netMsk(255, 255, 255, 0);
const byte DNS_PORT = 53; // DNS server 
DNSServer dnsServer; // DNS server 
WebServer server(80);// Web server
// ------------------wifi

//---lora
#define ss 12  // for esp32 LOLIN S2 Mini-> SCK:7, MISO: 9, MOSI: 11, NSS: 12, RST: 18
#define rst 16
#define dio0 2
String inString = "";  // string to hold input from lora
int counter = 0; //count received lora packets 
long intervalTime = 0; //for time measurement between received lora packet
String message = ""; // save message from lora
//---lora

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
    Serial.println("Request redirected to captive portal");
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

void blink() {
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(20);                         // wait 
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
}

void sendLoraMessage() {
 server.send(200, "text/plane", message); //Send lora message to client ajax request
}

void sendCounter() {
 server.send(200, "text/plane", String(counter)); //Send packet number to client ajax request
}

void readConnection() {
  server.send(200, "text/plane", String(WiFi.softAPgetStationNum())); //Send number of connected devices to client ajax request
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
  server.on("/readLoraMessage", sendLoraMessage);//To get update ajax
  server.on("/readCounter", sendCounter);//To get update ajax
  server.on("/readConnection", readConnection);//To get update ajax
  server.onNotFound(handleNotFound);
  server.begin(); // Web server start
  Serial.println("HTTP server started");

  blink();
  delay(100);
  blink();
  delay(100);
  blink();
}

void lora_start() {
  LoRa.setPins(ss, rst, dio0);
   if (!LoRa.begin(433E6)) {
    while (1) {      
      Serial.println("Starting LoRa failed!");
      Serial.print("MOSI: ");
      Serial.println(MOSI);
      Serial.print("MISO: ");
      Serial.println(MISO);
      Serial.print("SCK: ");
      Serial.println(SCK);
      Serial.print("SS: ");
      Serial.println(SS);
      digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
      delay(1000);                      // wait for a second
      digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
      delay(10000);
    };
  }
  Serial.println("LoRa status: ok");
}

void loraEngineReceiver() {
  int packetSize = LoRa.parsePacket();

  if (packetSize) {
    blink();
    // read packet
    while (LoRa.available()) { //loop for lora message
      int inChar = LoRa.read();
      inString += (char)inChar;
    }
    Serial.print(" Packet: ");
    Serial.println(inString);
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
    message = inString + " RSSI=" + String(LoRa.packetRssi());
    inString = ""; // clear received message
    counter++;
    intervalTime = millis();
  }
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

