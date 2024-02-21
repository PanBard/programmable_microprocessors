#ifndef ESP32
#error This code is designed to run on ESP32 platform (ESP32 S2 mini [LOLIN S2 Mini]), not Arduino nor ESP8266! Please check your Tools->Board setting.
#endif
#include <SPI.h>
#include <LoRa.h> // by Sandeep Mistry
#include <Wire.h>  //for custom sda, scl define
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <ArduinoJson.h> // by Benoit Blanchon
#include "index.h"  //Web page header file
#include "lora_protocol_functions.h"  

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

//---lora
#define ss 12  // for esp32 LOLIN S2 Mini-> SCK:7, MISO: 9, MOSI: 11, NSS: 12, RST: 18
#define rst 16
#define dio0 2
String inString = "";  // string to hold input from lora
int counter = 0; //count received lora packets 
long intervalTime = 0; //for time measurement between received lora packet
String message = ""; // save message from lora
//---lora

//-----json
StaticJsonDocument<250> jsonDocument; //for deserialize json
String postMessage = ""; //message from POST request
String jsonString = ""; //json string for get request from webapp
//-----json

String previousMessage = ""; //to dont replicate  message send to server
String logs = ""; //log all lora message to one string
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

void blink() {
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(20);                         // wait 
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
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

void change_parameter(){

 if(String(postMessage) == "Bandwidth[125E3]")
  {
    Serial.println("Bandwidth = default [125E3]");
    lora_send_message("Bandwidth[125E3]");
    LoRa.setSignalBandwidth(125E3);
    Serial.println("bandwidth_state ="+String(message));
    Serial.println("[control]Message: "+String(message));
    bandwidth = 125;
    
  }
  
  else  if(String(postMessage)  == "Bandwidth[250E3]")
  {
    Serial.println("Bandwidth = default [250E3]");
    lora_send_message("Bandwidth[250E3]");
    LoRa.setSignalBandwidth(250E3);
    Serial.println("bandwidth_state ="+String(message));
    Serial.println("[control]Message: "+String(message));
    bandwidth=250;
  }

  else    if(String(postMessage)  == "Bandwidth[500E3]")
  {
    Serial.println("Bandwidth =  [500E3]");
    lora_send_message("Bandwidth[500E3]");
    LoRa.setSignalBandwidth(500E3);
    Serial.println("bandwidth_state ="+String(message));
    Serial.println("[control]Message: "+String(message));
    bandwidth=500;
  }

   else    if(String(postMessage)  == "Power[17]")
  {
    Serial.println("Power = default 17");
    lora_send_message("Power[17]");
    LoRa.setTxPower(17);
    Serial.println("power_state ="+String(message));
    Serial.println("[control]Message: "+String(message));
    power =17;
  }
  else    if(String(postMessage)  == "Power[19]")
  {
    Serial.println("Power = default 19");
    lora_send_message("Power[19]");
    LoRa.setTxPower(19);
    Serial.println("power_state ="+String(message));
    Serial.println("[control]Message: "+String(message));
    power=19;
  }
  else    if(String(postMessage)  == "Power[20]")
  {
    Serial.println("Power = default 20");
    lora_send_message("Power[20]");
    LoRa.setTxPower(20);
    Serial.println("power_state ="+String(message));
    Serial.println("[control]Message: "+String(message));
    power=20;
  }

}

void lora_send_message(String message) {
  String final_message = return_text_with_checksum(message);
  Serial.println("Sending message: "+ final_message);
  // send packet
  LoRa.beginPacket();
  LoRa.print(final_message);
  LoRa.endPacket();
  blink();
  logs += ("[LORA] Sending: "+message+"\n");
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
    Serial.print(inString);
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
    bool message_status = validate_message(inString);
    if(message_status)
    {
      message = return_message_without_checksum(inString) + " RSSI=" + String(LoRa.packetRssi())+ " [OK]";
      logs += ("[LORA] Receive: "+message+"\n");
      if(return_message_without_checksum(inString) != "OK")
      {
        lora_send_message(String(checksum_calculator(inString)));
      }
    }
    else
    {
      message = return_message_without_checksum(inString) + " RSSI=" + String(LoRa.packetRssi())+ " [BAD CHECKSUM !!!]";
      logs += ("[LORA] Receive: "+message+"\n");
      lora_send_message("BAD");
    }
    
    inString = ""; // clear received message
    message = "";
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

