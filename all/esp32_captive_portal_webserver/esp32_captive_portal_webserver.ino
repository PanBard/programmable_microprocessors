#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include "index.h"  //Web page header file

#ifndef SSID
#define SSID "open"
#endif

const char *softAP_ssid = SSID;

// The access points IP address and net mask
// It uses the default Google DNS IP address 8.8.8.8 to capture all 
// Android dns requests
IPAddress apIP(8, 8, 8, 8);
IPAddress netMsk(255, 255, 255, 0);

// DNS server 
const byte DNS_PORT = 53; 
DNSServer dnsServer;

// Web server
WebServer server(80);

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

// checks if the request is for the controllers IP, if not we redirect automatically to the
// captive portal 
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

  // String p;
  // p += F(
  //           "<html><head></head><body>"
  //           "<h1>HELLO WORLD!!</h1>");
  // p += F("</body></html>");
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

// void handleRoot() {
//  String page = MAIN_page; //Read HTML contents
//  server.send(200, "text/html", page); //Send web page
// }

void sendLoraMessage() {
 server.send(200, "text/plane", "message"); //Send data only to client ajax request
}

void sendCounter() {
 server.send(200, "text/plane", "counter"); //Send data  only to client ajax request
}

void setup() {
  delay(1000);
  Serial.begin(9600);
  Serial.println();
  Serial.println("Configuring access point...");
  WiFi.softAPConfig(apIP, apIP, netMsk);
  // its an open WLAN access point without a password parameter
  WiFi.softAP(softAP_ssid);
  delay(1000);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  /* Setup the DNS server redirecting all the domains to the apIP */
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", apIP);

  /* Setup the web server */
  server.on("/", handleRoot);
  server.on("/generate_204", handleRoot);
  server.on("/readLoraMessage", sendLoraMessage);//To get update of ADC Value only
  server.on("/readCounter", sendCounter);//To get update of ADC Value only
  server.onNotFound(handleNotFound);
  server.begin(); // Web server start
  Serial.println("HTTP server started");

}

void loop() {
  // put your main code here, to run repeatedly:
  //DNS
  dnsServer.processNextRequest();
  //HTTP
  server.handleClient();
}

