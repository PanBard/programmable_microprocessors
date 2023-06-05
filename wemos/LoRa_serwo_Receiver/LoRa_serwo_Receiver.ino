#include <SPI.h>
#include <LoRa.h> 
#include <Servo.h>

#define ss 15
#define rst 16
#define dio0 2

//serwo
Servo myservo;  // create servo object to control a servo
 int obr=0;
//serwo

int LED = 3;
String inString = "";    // string to hold input
int val = 0;
 
void setup() {
   myservo.attach(2);  // attaches the servo on GIO2 to the servo object
   myservo.write(180); //set serwo to max position on start
  Serial.begin(9600);
  LoRa.setPins(ss, rst, dio0);
  // pinMode(LED,OUTPUT);
  
  while (!Serial);
  Serial.println("LoRa Receiver");
  if (!LoRa.begin(433E6)) { // or 915E6
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}
 
void loop() {
  
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) { 
    // read packet    
    while (LoRa.available())
    {
      int inChar = LoRa.read();
      inString += (char)inChar;
      val = inString.toInt();       
    }
    inString = "";     
    LoRa.packetRssi();    
  }
  
  Serial.println(val);  
  myservo.write(val);

if(val == 1){
  Serial.println("playyyyyyyyyyy");
}

  // analogWrite(LED, val);
}