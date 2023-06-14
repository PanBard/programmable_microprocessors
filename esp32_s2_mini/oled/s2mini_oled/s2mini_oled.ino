#ifndef ESP32
  #error This code is designed to run on ESP32 platform, not Arduino nor ESP8266! Please check your Tools->Board setting.
#endif

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h> //for sda, scl define

// OLED setup ---------------------
Adafruit_SSD1306 display(128, 64, &Wire, -1); // On ESP32 s2 mini:   pin11=(SDA),  pin12=(SCL); VCC=3.3v; Reset pin # (or -1 if sharing Arduino reset pin)
// OLED setup ---------------------

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

void setup() {
  // OLED setup ---------------------
    Wire.begin(11, 12); //Wire.begin(I2C_SDA, I2C_SCL); 
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { //See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  // OLED setup ---------------------
displayTransformersLogo();
}

void loop() {
  display_content();
}

void displayTransformersLogo(){
  display.clearDisplay();
  display.drawBitmap(0,0,Autobot_logo, 120, 60, 1);//(x,y,bitmap_name,width,height,1)
  display.display();
  delay(1000);
}

void display_content(){
  display.clearDisplay(); // Clear display buffer
  display.drawLine(0, 32, 128,32, SSD1306_WHITE); // line (x1,y1,x2,y2,color)
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);

  

  for(int i=0; i<=10; i++){
    display.clearDisplay(); // Clear display buffer
    display.setCursor(1, 5);
    display.println(F("MOC:")); 
    display.setCursor(30, 5);
    display.println(i);
    display.display(); // Update screen
    delay(400);
  }
}



