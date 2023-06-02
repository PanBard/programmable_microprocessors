#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>

// OLED setup --------------------- 
Adafruit_SSD1306 display(128, 64, &Wire, -1); // On wemos:   D2(SDA),  D1(SCL); VCC=3.3v; Reset pin # (or -1 if sharing Arduino reset pin)
// OLED setup ---------------------

//SERWO --------------------------
Servo myservo;  // create servo object to control a servo
Servo engine_power;  // create servo object to control a motor
int value = 0; //for read the analog value frompotentiometer
int obr=0; //for serwo movement
int moc=0; //for motor pwm
int bieg=0; //for motor speed change
//SERWO --------------------------

void setup() {
  //ALL CONNECTED TO 3.3V
  myservo.attach(16);  // attaches the servo pwm  on GIO16  (D0 on wemos)
  engine_power.attach(14); // attaches the motor pwm on  GIO14 (D5 on wemos)
  pinMode(2, INPUT_PULLUP); // attaches pullupp button on GIO2 (D4 on wemos) and to GND
  pinMode(0, INPUT_PULLUP); // attaches pullupp button on GIO0 (D3 on wemos) and to GND
  myservo.write(0); //set serwo to min position on start
  engine_power.write(0);//set motor to min pwm value on start
  Serial.begin(9600); //start serial monitor 

  // OLED setup ---------------------
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { //See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
// OLED setup ---------------------

  updateScreen();

}

void loop() {
  // Serial.println(digitalRead(2));
  if(digitalRead(2) == LOW ){
    dpeeddown();
  }

  if(digitalRead(0) == LOW ){
    speedup();
  }

  updateScreen();
  value = analogRead(A0);
  obr = (value *180)/1024;
  myservo.write(obr); //set serwo pwm
  moc = bieg * 36;
  engine_power.write(moc);//set motor pwm
  //  Serial.print("obr: ");
  //  Serial.println(obr);
  //  Serial.print("moc: ");
  //  Serial.println(moc);

  delay(200);
}

void speedup(){
  if(bieg != 5){
    bieg =bieg + 1;
  }
}

void dpeeddown(){
  if(bieg != 0){
    bieg =bieg - 1;
  }
}

void updateScreen(){
  display.clearDisplay(); // Clear display buffer
  display.drawLine(0, 32, 128,32, SSD1306_WHITE); // (x1,y1,x2,y2,color)
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(1, 5);
  display.println(F("MOC:"));
  display.setCursor(30, 5);
  display.println(bieg);
  display.setCursor(1, 22);
  display.println(F("min"));
  display.setCursor(110, 22);
  display.println(F("max"));
  if(bieg==1){display.fillRect(25, 28, 10, 5,SSD1306_WHITE);}//(x,y,width,height,color)}
  if(bieg==2){
    display.fillRect(25, 28, 10, 5,SSD1306_WHITE);
    display.fillRect(40, 23, 10, 10,SSD1306_WHITE);
  }
  if(bieg==3){
    display.fillRect(25, 28, 10, 5,SSD1306_WHITE);
    display.fillRect(40, 23, 10, 10,SSD1306_WHITE);
    display.fillRect(55, 18, 10, 15,SSD1306_WHITE);
  }
  if(bieg==4){
    display.fillRect(25, 28, 10, 5,SSD1306_WHITE);
    display.fillRect(40, 23, 10, 10,SSD1306_WHITE);
    display.fillRect(55, 18, 10, 15,SSD1306_WHITE);
    display.fillRect(70, 13, 10, 20,SSD1306_WHITE);
  }
  if(bieg==5){
    display.fillRect(25, 28, 10, 5,SSD1306_WHITE);
    display.fillRect(40, 23, 10, 10,SSD1306_WHITE);
    display.fillRect(55, 18, 10, 15,SSD1306_WHITE);
    display.fillRect(70, 13, 10, 20,SSD1306_WHITE);
    display.fillRect(85, 8, 10, 25,SSD1306_WHITE);
  }
  pozycjaSerwa();
  display.display(); // Update screen
}

void pozycjaSerwa(){
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(1, 45);
  display.println(F("DOCISK:"));
  display.setCursor(45, 45);
  display.println(obr);
}

