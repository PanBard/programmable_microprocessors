/* Put your SSID & Password */
const char* SSID = "Konsoleta";  // Enter SSID here
const char* PASSWORD = "sremrofsnart";  //Enter Password here

void blink() 
{
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delayMicroseconds(100);                        // wait 
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
}

void wifi_start() 
{
    pinMode(LED_BUILTIN, OUTPUT);
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID,PASSWORD);
    blink();
    delay(100);
    blink();

    while(WiFi.status() != WL_CONNECTED)
    {
      blink();
    }
}

void send_data(String url) 
{
    if((WiFi.status() == WL_CONNECTED)) 
    {
        HTTPClient http;
        http.begin(url); //HTTP
        int httpCode = http.GET();
        http.end();
        blink();
    }
    else
    {
        digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
        delay(1000);                        // wait 
        digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
    }
}

