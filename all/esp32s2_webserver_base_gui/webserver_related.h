void blink() {
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(20);                         // wait 
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
}


#ifndef SSID
#define SSID "Konsoleta"
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

void handlePost() {
  if (server.hasArg("plain") == false) {
  }
  blink();
  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);
  String messageFromJSON = jsonDocument["message"];
  postMessage = messageFromJSON;
  Serial.println("POST: "+String(postMessage));
  Serial.println("POST body: "+String(body));
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
  server.on("/post", HTTP_POST, handlePost); 
  server.begin(); // Web server start
  Serial.println("HTTP server started");
  blink();
  delay(100);
  blink();
  delay(100);
  blink();
}