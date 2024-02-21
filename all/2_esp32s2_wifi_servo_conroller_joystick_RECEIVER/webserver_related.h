void blink() {
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delayMicroseconds(100);                        // wait 
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
}

// ------------------wifi
const char* SSID = "Konsoleta"; 
const char* PASSWORD = "sremrofsnart";  
IPAddress local_ip(8,8,8,8);
IPAddress subnet(255,255,255,0);
WebServer server(80);// Web server
// ------------------wifi

void handle_up_movement() 
{
  handle_servo_movement("up");
  blink();
}

void handle_down_movement() 
{
  handle_servo_movement("down");
  blink();
}

void handle_left_movement() 
{
  handle_servo_movement("left");
  blink();
}

void handle_right_movement() 
{
  handle_servo_movement("right");
  blink();
}

void handle_light_on_off() 
{
  handle_servo_movement("light_on_off");
  blink();
}

void handle_position_zero() 
{
  handle_servo_movement("position_zero");
  blink();
}

void wifi_start() 
{
  WiFi.softAP(SSID,PASSWORD,1,true);
  WiFi.softAPConfig(local_ip, local_ip, subnet);
  server.on("/up", handle_up_movement);
  server.on("/down", handle_down_movement);
  server.on("/left", handle_left_movement);
  server.on("/right", handle_right_movement);
  server.on("/light_on_off", handle_light_on_off);
  server.on("/position_zero", handle_position_zero);
  server.begin();
  Serial.println("HTTP server started");
  pinMode(LED_BUILTIN, OUTPUT);
  blink();
  delay(100);
  blink();
  delay(100);
  blink();
}