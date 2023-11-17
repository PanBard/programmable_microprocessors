#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h> //for sda, scl define
#include <OneWire.h> //for ds18b20
#include <DallasTemperature.h> //for ds18b20

#include "ESP8266WiFi.h"
#include "ESP8266HTTPClient.h"
#include "WiFiClient.h"

// wifi setup
const char* ssid = "DESKTOP-DB3900H 6599"; // Write here your router's username
const char* password = "8S6m506*"; // Write here your router's passward
String URL = "http://192.168.0.185:88"; //adress to your API http://localhost:88/
HTTPClient http;
WiFiClient client;
// wifi setup

// OLED setup ---------------------
Adafruit_SSD1306 display(128, 64, &Wire, -1); // On nodeMCU esp8266 v3:   pin D2=(SDA),  pin D1=(SCL); VCC=3.3v; Reset pin # (or -1 if sharing Arduino reset pin)
// OLED setup ---------------------

//DS18B20 setup
#define ONE_WIRE_BUS 14 // Data wire is plugged into port D5 on the nodemcu esp8266
OneWire oneWire(ONE_WIRE_BUS); // Pass our oneWire reference to Dallas Temperature. // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensors(&oneWire); // Pass our oneWire reference to Dallas Temperature. 
float tempC; //temp value in celsius
//DS18B20 setup

//time counter
long data_packet_counter = 0;
long time_loop_counter = 0;
//time counter

//wifisender sleep time setup------------------------------------- <<<<<< SETUP
int sleep_time_for_data_sender = 600; // in seconds - interval between POST message to server
int sleep_time_for_program_loop = 5;  //in seconds - refresh temp mesaurement
//wifisender sleep time setup------------------------------------- <<<<<< SETUP

//wifisender  counter
int wifi_sleep = sleep_time_for_data_sender; //initial value for sending first data packet after reset
//wifisender counter

void setup() {
  // Serial.begin(9600);
  // OLED setup ---------------------
  Wire.begin(4, 5); //Wire.begin(I2C_SDA, I2C_SCL); sda -> D2 , scl -> D1 on nodemcu esp8266
  oledConnectionCheck();
  // OLED setup ---------------------
  

  displayTransformersLogo();
  
  connectToWIFI();
  pinMode(LED_BUILTIN, OUTPUT); //for led blinking
  blik_LED_BUILTIN(3);

}

void loop() {
  oledConnectionCheck();
  read_temperature();
  display_temp();
  checkWifiConnection();
  data_sender_time_sleep_update();
  sendData();
  delay(sleep_time_for_program_loop * 1000);
}

void read_temperature(){
  sensors.requestTemperatures(); // Send the command to get temperatures
  tempC = sensors.getTempCByIndex(0);//get the temperature from the first sensor only ByIndex
  // if(tempC != DEVICE_DISCONNECTED_C){ // Check if reading was successful
  //   Serial.print("Temperature: ");
  //   Serial.println(tempC);
  // } 
  // else
  // { Serial.println("Error: Could not read temperature data"); }
}

void oledConnectionCheck(){
 display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
}

void checkWifiConnection(){
   if(WiFi.status() != WL_CONNECTED){
      connectToWIFI();
      }
}

void connectToWIFI(){
    // Connect to WiFi
    display_wifi_connecting_message();
  WiFi.begin(ssid, password);

  // while wifi not connected yet, print '.'
  // then after it connected, get out of the loop
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
     Serial.println(WiFi.status());
  }
  //print a new line, then print WiFi connected and the IP address
  Serial.println("");
  Serial.println("WiFi connected");
  // Print the IP address
  Serial.println(WiFi.status());
  Serial.println(WiFi.localIP());
}

void sendData(){
  if(wifi_sleep>=sleep_time_for_data_sender){
    String data = "temperature="+String(tempC);
    http.begin(client, URL);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int httpCode = http.POST(data);
    String payload = http.getString();
    // Serial.print("data: "); Serial.println(data);
    // Serial.print("http: "); Serial.println(httpCode);
    // Serial.print("payload: "); Serial.println(payload);
    calculate_data_package_sent();
    data_sender_time_sleep_reset();
    blik_LED_BUILTIN(3);
  }
    
}

void data_sender_time_sleep_reset(){
  wifi_sleep = 0;
}

void calculate_data_package_sent(){
    data_packet_counter=data_packet_counter+1;
}

void data_sender_time_sleep_update(){
  wifi_sleep = wifi_sleep + sleep_time_for_program_loop; //
}



void display_temp(){
   display.clearDisplay(); // Clear display buffer
    display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
    display.setCursor(1, 5);
    display.println(F("Temp.:")); 
    display.setCursor(50, 5);
    display.setTextSize(2); // Draw 2X-scale text
    display.println(tempC);
    display_time();
    display_counter();
    display_transmission_interval();
    display.display(); // Update screen
}

void calculate_time(){
  time_loop_counter = time_loop_counter + 1;
}

void display_time(){
    display.setTextSize(1); // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(1, 25);
    display.println(F("Time:")); 
    display.setCursor(35, 25);
    display.println(String((time_loop_counter*sleep_time_for_program_loop)/3600)+F("h"));
    display.setCursor(60, 25);
    display.println(String(((time_loop_counter*sleep_time_for_program_loop)%3600)/60)+F("m"));
    display.setCursor(85, 25);
    display.println(String((time_loop_counter*sleep_time_for_program_loop)%60)+F("s"));
     calculate_time();
}

void display_counter(){
    display.setCursor(1, 40);
    display.println(F("Data pkt.:")); 
    display.setCursor(60, 40);
    display.println(data_packet_counter);
}

void display_transmission_interval(){
    display.setCursor(1, 55);
    display.println(F("Interval:")); 
    display.setCursor(58, 55);
    display.println(String(sleep_time_for_data_sender/60)+"m");
}

void display_wifi_connecting_message(){
   display.clearDisplay(); // Clear display buffer
    display.setTextSize(1); // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(1, 30);
    display.println(F("Connecting to wifi...")); 
    display.display(); // Update screen
}

void blik_LED_BUILTIN(int msec){
  digitalWrite(LED_BUILTIN, LOW);  // turn the LED on (HIGH is the voltage level)
  delay(msec);                      // wait for a second
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off by making the voltage LOW
}


const unsigned char Autobot_logo [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 
	0xff, 0xbf, 0xff, 0xff, 0xff, 0xfc, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 
	0xbf, 0xff, 0xff, 0xff, 0xfd, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xfd, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xdf, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xdf, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xdf, 0xfc, 0x00, 0x3f, 
	0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xef, 0xff, 0x00, 0x7f, 0xfb, 
	0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xf7, 0xff, 0x81, 0xff, 0xef, 0xff, 
	0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf9, 0xf9, 0xff, 0xe3, 0xff, 0xdf, 0xdf, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xfe, 0xff, 0xf7, 0xff, 0x3f, 0x3f, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x3f, 0x3f, 0xff, 0xfe, 0xfe, 0x7f, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x9f, 0xdf, 0xff, 0xf9, 0xfd, 0xff, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x7f, 0xe7, 0xe7, 0xff, 0xf7, 0xf3, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x7d, 0xf3, 0xfb, 0xff, 0xcf, 0xcf, 0xbe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x7e, 0x7d, 0xf8, 0xff, 0x9f, 0x9f, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x3f, 0xbe, 0x7e, 0x3e, 0x7f, 0x7c, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x3f, 0xcf, 0xff, 0x9d, 0xfe, 0xfb, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 
	0xe7, 0xfd, 0xe3, 0xff, 0xf7, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xf9, 
	0xfd, 0xff, 0xff, 0xcf, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xfe, 0xfd, 
	0xff, 0xff, 0x3f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0x7d, 0xff, 
	0xfe, 0x7f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0b, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xd0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0xff, 0xfe, 0xff, 0xbf, 0xff, 
	0xb0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x7f, 0xfe, 0xff, 0xbf, 0xfe, 0x70, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xfe, 0xff, 0xbf, 0xff, 0xf0, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xe0, 0x7e, 0xff, 0xbf, 0x03, 0xf0, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xe0, 0x02, 0xff, 0x20, 0x03, 0xf0, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xe0, 0x00, 0xff, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x0f, 0xe0, 0x02, 0xff, 0x40, 0x07, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x0f, 0xf8, 0x0e, 0xff, 0x70, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x07, 0xfe, 0x3f, 0xff, 0x7e, 0x3f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x07, 0xfe, 0xff, 0xff, 0x7f, 0x7f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x07, 0xfe, 0xff, 0xff, 0x7f, 0x7f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 
	0xfe, 0xff, 0xff, 0x7f, 0x7f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfe, 
	0xff, 0xff, 0x7f, 0x7f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfe, 0xff, 
	0xff, 0x7f, 0x7f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfe, 0xff, 0xff, 
	0x7f, 0x7f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfe, 0xff, 0xff, 0x7f, 
	0x7f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfe, 0xff, 0xff, 0x7f, 0x7f, 
	0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfe, 0xff, 0xff, 0x7f, 0x7f, 0xf0, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfe, 0xff, 0xff, 0x7f, 0x7f, 0xf0, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfe, 0xff, 0x7f, 0x7f, 0x7f, 0xe0, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfe, 0xff, 0xff, 0xff, 0x7f, 0xe0, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfe, 0xff, 0xff, 0xff, 0x7f, 0xe0, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x03, 0xfe, 0xff, 0xff, 0xff, 0x7f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0xff, 0xff, 0x7f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x7e, 0xff, 0xff, 0xff, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x3e, 0xfe, 0x00, 0x3f, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x1e, 0xfd, 0xff, 0xbf, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x06, 0xfd, 0xff, 0xff, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 
	0xfd, 0xff, 0xdf, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfb, 
	0xff, 0xdf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfb, 0xff, 
	0xef, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3b, 0xff, 0xee, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x17, 0xff, 0xec, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xf0, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00
};

void displayTransformersLogo(){
  display.clearDisplay();
  display.drawBitmap(0,0,Autobot_logo, 120, 60, 1);//(x,y,bitmap_name,width,height,1)
  display.display();
  delay(1000);
}
