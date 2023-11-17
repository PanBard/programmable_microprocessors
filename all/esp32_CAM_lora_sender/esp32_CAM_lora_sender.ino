#ifndef ESP32
  #error This code is designed to run on ESP32 platform, not Arduino nor ESP8266! Please check your Tools->Board setting.
#endif
#include <SPI.h>
#include <LoRa.h>


//PINOUT -------------------------------------start
#include <Wire.h> //for custom sda, scl define
//---lora 
#define SCK 13
#define MISO 2
#define MOSI 15
#define SS 14

#define RST 23
#define DIO0 19

//---lora 

//PINOUT --------------------------------------end


int counter = 0;
int memory_value_x =0; //save value to save lora sender
int memory_value_y =0; //save value to save lora sender


//joystick
 int x_axis = 0;
 int y_axis = 0;
//joystick

void setup() {

  Serial.begin(9600);
  Serial.println("LoRa Sender");
  Serial.println(counter);
  //SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  LoRa.setPins(SS, RST, DIO0);

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
   };
  }
}

void loop() {
  sendData();
  delay(4000);
}




void sendData(){
  Serial.print("Sending packet: ");
  Serial.println(counter);  
  counter++;
 // send packet
  LoRa.beginPacket();
  LoRa.print("Pakiet numer:");
  LoRa.endPacket();
}





