#ifndef ESP32
#error This code is designed to run on ESP32 platform (ESP32 S2 mini [LOLIN S2 Mini]), not Arduino nor ESP8266! Please check your Tools->Board setting.
#endif

#include <WiFi.h>
#include <HTTPClient.h>
#include "wifi_related.h"
#include "joystick_related.h"


void setup() 
{
  wifi_start();
  joystick_start();
}

void loop() 
{
  read_joystick_value_and_manage_comunication();
}
