#include <SPI.h>
#include <LoRa.h> 

#define ss 12
#define rst 16
#define dio0 2

int LED = 3;
String inString = "";    // string to hold input
int val = 0;
 
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  LoRa.setPins(ss, rst, dio0);
  
  // while (!Serial);
  Serial.println("LoRa Receiver");
  if (!LoRa.begin(433E6)) { // or 915E6
    Serial.println("Starting LoRa failed!");
    while (1){ 
      Serial.print("MOSI: ");
      Serial.println(MOSI);
      Serial.print("MISO: ");
      Serial.println(MISO);
      Serial.print("SCK: ");
      Serial.println(SCK);
      Serial.print("SS: ");
      Serial.println(SS); 
      delay(10000);};
  }
}
 
void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) { 
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(1000);                      // wait for a second
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
    // read packet    
    while (LoRa.available())
    {
      int inChar = LoRa.read();
      inString += (char)inChar;
      val = inString.toInt();       
    }
    inString = "";     
    LoRa.packetRssi();    
    Serial.println(val); 
  }
  
}