#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h> //for sda, scl define
#include <OneWire.h> //for ds18b20
#include <DallasTemperature.h> //for ds18b20

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
long counter = 0;
//time counter

//sleep time setup-------------------------------------
int sleep_time=10; // in seconds
//sleep time setup-------------------------------------

void setup() {
  // Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT); //for led blinking

  // OLED setup ---------------------
    Wire.begin(4, 5); //Wire.begin(I2C_SDA, I2C_SCL); sda -> D2 , scl -> D1 on nodemcu esp8266
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { //See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  // OLED setup ---------------------
  
displayTransformersLogo();
}

void loop() {
  read_temperature();
  display_temp();
  blik_LED_BUILTIN(3);
  delay(sleep_time * 1000);
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
    display.display(); // Update screen
}

void display_time(){
    display.setTextSize(1); // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(1, 30);
    display.println(F("Czas:")); 
    display.setCursor(35, 30);
    display.println(String((counter*sleep_time)/3600)+F("h"));
    display.setCursor(60, 30);
    display.println(String(((counter*sleep_time)%3600)/60)+F("m"));
    display.setCursor(85, 30);
    display.println(String((counter*sleep_time)%60)+F("s"));
    calculate_counter_and_time();
}

void display_counter(){
    display.setCursor(1, 45);
    display.println(F("Nr pom.:")); 
    display.setCursor(53, 45);
    display.println(counter);
}

void calculate_counter_and_time(){
    counter=counter+1;
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





