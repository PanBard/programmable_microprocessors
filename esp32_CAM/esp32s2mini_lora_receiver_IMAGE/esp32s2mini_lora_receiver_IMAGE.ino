#ifndef ESP32
  #error This code is designed to run on ESP32 platform, not Arduino nor ESP8266! Please check your Tools->Board setting.
#endif
#include <SPI.h>
#include <LoRa.h>

//PINOUT -------------------------------------start
#include <Wire.h> //for custom sda, scl define
//---lora 
#define ss 12  //MOSI: 11, MISO: 9, SCK: 7
#define rst 16
#define dio0 2
//---lora 
//PINOUT --------------------------------------end

String inString = "";    // string to hold current packet data
String image_data = "";    // string to hold whole receive image
String image_data_copy = "";    // string to hold whole receive image
String check_sum = "";    // string to hold check_sum
bool gate_for_checksum = false;
uint32_t tTimer;     // used to time tasks 
int current_count = 0;
int total_count = 0;
int image_number = 0;
void setup() {
 pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  
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
  delay(3000);
  Serial.println("LoRa receiver XD");
}

void loop() {
loraEngineReceiver();
  // delay(100);
}

void loraEngineReceiver(){
   int packetSize = LoRa.parsePacket();

    if (packetSize) { 
    
    // digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    // delay(2);                      // wait for a second
    // digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW

    // read packet    
    while (LoRa.available())
    {
      int inChar = LoRa.read();
      inString += (char)inChar;
      // val = inString.toInt(); 
         
    }
    if(inString == "START"){
      image_number ++;
      Serial.println("Receive image "+String(image_number)); 
      tTimer = millis();
      image_data = "";
    }

    if(gate_for_checksum){ //after SUM 
      check_sum = inString;
      gate_for_checksum=false; //enter inside only one time
    }

    if(inString == "SUM" ){
      image_data_copy = image_data;
      gate_for_checksum=true; // to catch data sent after SUM
    }

    if(inString == "END"){
      Serial.println("\nReceiving image number "+String(image_number)+" took " + String((millis() - tTimer)/1000) + " seconds.");
      Serial.println("Number of packets for this image: "+ String(current_count));
      Serial.println("Total number of packets: "+ String(total_count));  
      Serial.println("Checksum from camera: "+ check_sum);  
      Serial.println("Checksum from received data: "+ String(image_data_copy.length())); 
      Serial.println(image_data_copy);
      image_data = "";
      image_data_copy="";
      current_count = 0;
      gate_for_checksum=false;
    }

    if(inString != "END" && inString != "START" )
    {
      image_data += inString;
    }

    Serial.print("."); 
    inString = "";     
    current_count ++;
    total_count ++;
    LoRa.packetRssi();    
}
}


