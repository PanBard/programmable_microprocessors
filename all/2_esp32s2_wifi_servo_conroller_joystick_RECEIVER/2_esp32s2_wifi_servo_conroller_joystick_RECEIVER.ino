#ifndef ESP32
#error This code is designed to run on ESP32 platform (ESP32 S2 mini [LOLIN S2 Mini]), not Arduino nor ESP8266! Please check your Tools->Board setting.
#endif

#include <SPI.h>
#include <Wire.h>  //for custom sda, scl define
#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>
#include "servo_related.h"
#include "webserver_related.h"

void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);
  servo_start(); //servo start
  wifi_start(); //WIFI setup
}

void loop() 
{
  server.handleClient(); //HTTP
}

