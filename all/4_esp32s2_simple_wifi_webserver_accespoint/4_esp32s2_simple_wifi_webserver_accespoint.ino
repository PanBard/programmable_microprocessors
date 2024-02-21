#include <WiFi.h>
#include <WebServer.h>

/* Put your SSID & Password */
const char* ssid = "Konsoleta";  // Enter SSID here
const char* password = "87654321";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(8,8,8,8);
IPAddress subnet(255,255,255,0);
WebServer server(80);

void setup() {
  WiFi.softAP(ssid);
  WiFi.softAPConfig(local_ip, local_ip, subnet);
  delay(1000);
  server.on("/up", handle_up_movement);
  server.on("/down", handle_down_movement);
  server.on("/left", handle_left_movement);
  server.on("/right", handle_right_movement);
  server.begin();
  Serial.println("HTTP server started");
  pinMode(LED_BUILTIN, OUTPUT);
  blink();
  delay(100);
  blink();
  delay(100);
  blink();
}
void loop() {
  server.handleClient();
}

void handle_up_movement() {
  Serial.println("up");
  server.send(200, "text/html", "up"); blink();
}

void handle_down_movement() {
  Serial.println("down");
  server.send(200, "text/html", "down"); blink();
}

void handle_left_movement() {
  Serial.println("left");
  server.send(200, "text/html", "left"); blink();
}

void handle_right_movement() {
  Serial.println("right");
  server.send(200, "text/html", "right"); blink();
}

void blink() {
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(20);                         // wait 
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
}




