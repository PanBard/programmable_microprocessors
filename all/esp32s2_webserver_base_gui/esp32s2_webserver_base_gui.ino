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
#include "webserver_related.h"


void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  wifi_start(); //WIFI setup
}

void loop() {
  dnsServer.processNextRequest();//DNS
  server.handleClient(); //HTTP
}

