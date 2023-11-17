#ifndef ESP32
  #error This code is designed to run on ESP32 platform, not Arduino nor ESP8266! Please check your Tools->Board setting.
#endif
#include <SPI.h>
#include <LoRa.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//PINOUT -------------------------------------start
#include <Wire.h> //for custom sda, scl define
//---lora 
#define ss 12  //MOSI: 11, MISO: 9, SCK: 7
#define rst 16
#define dio0 2
//---lora 
#define pinLED1 38
#define pinLED2 36
#define I2C_SDA 1 // oled sda
#define I2C_SCL 2 // oled scl
#define joystickPinAxisX A2 //analogpin x (3)
#define joystickPinAxisY A4 //analogpin y (5)
#define joystickPinPullup 4 //joystick switch  y
//PINOUT --------------------------------------end


// OLED setup ---------------------
Adafruit_SSD1306 display(128, 64, &Wire, -1); // On ESP32 s2 mini:   pin11=(SDA),  pin12=(SCL); VCC=3.3v; Reset pin # (or -1 if sharing Arduino reset pin)
// OLED setup ---------------------


int counter = 0;
int memory_value_x =0; //save value to save lora sender
int memory_value_y =0; //save value to save lora sender


//joystick
 int x_axis = 0;
 int y_axis = 0;
//joystick

void setup() {
pinMode(pinLED1,OUTPUT);
pinMode(pinLED2,OUTPUT);
pinMode(joystickPinPullup,INPUT_PULLUP);
// OLED setup ---------------------
    Wire.begin(I2C_SDA, I2C_SCL); //Wire.begin(I2C_SDA, I2C_SCL); 
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { //See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  // OLED setup ---------------------
displayTransformersLogo();

  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  

  Serial.println("LoRa Sender");
  Serial.println(counter);

  LoRa.setPins(ss, rst, dio0);

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1){  Serial.print("MOSI: ");
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
}

void loop() {
  x_axis = analogRead(joystickPinAxisX);// pin 3
  y_axis = analogRead(joystickPinAxisY);// pin 5
  x_axis = map(x_axis,0,8191,0,1023);
  y_axis = map(y_axis,0,8191,0,1023);
  runLed();
  loraSenderEngine();
  display_content();
  // delay(100);
}

void loraSenderEngine(){
  if(x_axis>700){sendData();}
  if(y_axis>700){sendData();}
  if(x_axis<500){sendData();}
  if(y_axis<500){sendData();}
}

void runLed(){
  if(x_axis>700){
    digitalWrite(pinLED1,HIGH);
  }
  if(x_axis<500){digitalWrite(pinLED1,LOW);}

   if(y_axis>700){
    digitalWrite(pinLED2,HIGH);
  }
   if(y_axis<500){digitalWrite(pinLED2,LOW);}

   if(digitalRead(joystickPinPullup)==LOW){
        if(digitalRead(joystickPinPullup)==LOW){
     digitalWrite(pinLED1,HIGH);
     digitalWrite(pinLED2,HIGH);
   }
   else{
     digitalWrite(pinLED1,LOW);
     digitalWrite(pinLED2,LOW);
   }
   }


}

void sendData(){
  if(abs(x_axis-memory_value_x)>20 || abs(y_axis-memory_value_y)>20){
      Serial.print("Sending packet: ");
  Serial.println(counter);
  Serial.print(" Szybkosc: ");
  Serial.print(x_axis);
  Serial.print(" skret: ");
  Serial.println(y_axis);
  
  counter++;
 // send packet
  LoRa.beginPacket();
  LoRa.print("Pakiet numer:");
  LoRa.print(counter);
  LoRa.print("szybkosc:");
  LoRa.print(x_axis);
  LoRa.print("skret:");
  LoRa.print(y_axis);
  LoRa.endPacket();

 digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  memory_value_x = x_axis;
  memory_value_y = y_axis;
  }
}




void display_content(){
  display.clearDisplay(); // Clear display buffer
  display.drawLine(0, 32, 128,32, SSD1306_WHITE); // line (x1,y1,x2,y2,color)
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.clearDisplay(); // Clear display buffer

  display.setCursor(1, 5);
  display.println(F("x_axis:")); 
  display.setCursor(50, 5);
  display.println(x_axis);


  display.setCursor(1, 30);
  display.println(F("y_axis: ")); 
  display.setCursor(50, 30);
  display.println(y_axis);

  display.display(); // Update screen

 
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