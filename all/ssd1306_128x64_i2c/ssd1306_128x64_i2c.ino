#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>

// OLED setup ---------------------
Adafruit_SSD1306 display(128, 64, &Wire, -1); // On wemos:   D2(SDA),  D1(SCL);  Reset pin # (or -1 if sharing Arduino reset pin)
// OLED setup ---------------------

void setup() {

  // OLED setup ---------------------
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { //See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
// OLED setup ---------------------

  gaz();

}

void loop() {
// gaz();

}

void gaz(){
  display.clearDisplay(); // Clear display buffer
  display.drawLine(0, 32, 128,32, SSD1306_WHITE); // (x1,y1,x2,y2,color)
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(1, 5);
  display.println(F("MOC:"));
  display.setCursor(1, 22);
  display.println(F("min"));
  display.setCursor(110, 22);
  display.println(F("max"));
  display.fillRect(25, 28, 10, 5,SSD1306_WHITE);//(x,y,width,height,color)
  display.fillRect(40, 23, 10, 10,SSD1306_WHITE);//(x,y,width,height,color)
  display.fillRect(55, 18, 10, 15,SSD1306_WHITE);//(x,y,width,height,color)
  display.fillRect(70, 13, 10, 20,SSD1306_WHITE);//(x,y,width,height,color)
  display.fillRect(85, 8, 10, 25,SSD1306_WHITE);//(x,y,width,height,color)
  pozycjaSerwa();
  display.display(); // Update screen
}

void pozycjaSerwa(){
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(1, 45);
  display.println(F("DOCISK:"));
  display.setCursor(45, 45);
  display.println(100);
}

