#ifndef ESP32
#error This code is designed to run on ESP32 platform (ESP32 S2 mini [LOLIN S2 Mini]), not Arduino nor ESP8266! Please check your Tools->Board setting.
#endif
#include <SPI.h>
#include <LoRa.h> // by Sandeep Mistry
#include <Wire.h>  //for custom sda, scl define
#include <WiFi.h>
#include <WebServer.h>
#include "index.h"  //Web page header file

//---wifi 
const char* ssid = "ESP32+";  // Enter your custom SSID here
const char* password = "12345678";  //Enter your custom Password here
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);
WebServer server(80);
//---wifi

//---lora
#define ss 12  // for esp32 LOLIN S2 Mini-> SCK:7, MISO: 9, MOSI: 11, NSS: 12, RST: 18
#define rst 16
#define dio0 2
//---lora

String inString = "";  // string to hold input from lora
int counter = 0; //count received lora packets 
long intervalTime = 0; //for time measurement between received lora packet
String message = ""; // save message from lora
void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  wifi_start(); //WIFI setup
  lora_start(); //LORA setup
  
}

void loop() {
  loraEngineReceiver();
  server.handleClient();
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
    // server.send(200, "text/html", generateHTML());
    message = inString + " RSSI=" + String(LoRa.packetRssi());
    inString = ""; // clear received message
    counter++;
    intervalTime = millis();
  }
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

void blink() {
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(20);                         // wait 
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
}

void wifi_start() {
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  server.on("/", handleRoot);      //This is display page
  server.on("/readLoraMessage", sendLoraMessage);//To get update of ADC Value only
  server.on("/readCounter", sendCounter);//To get update of ADC Value only
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP server started");
  blink();
  delay(100);
  blink();
  delay(100);
  blink();
}

void handleRoot() {
 String page = MAIN_page; //Read HTML contents
 server.send(200, "text/html", page); //Send web page
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

void sendLoraMessage() {
 server.send(200, "text/plane", message); //Send data only to client ajax request
}

void sendCounter() {
 server.send(200, "text/plane", String(counter)); //Send data  only to client ajax request
}
