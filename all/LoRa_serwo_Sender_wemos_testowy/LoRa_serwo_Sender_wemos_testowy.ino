#include <SPI.h>
#include <LoRa.h>
#include <Ra01S.h>

#define ss 15
#define rst 16
#define dio0 2

int counter = 0;

//serwo
 int value = 0;
 int obr=0;
//serwo

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Sender");
  Serial.println(counter);

  LoRa.setPins(ss, rst, dio0);

  if (!LoRa.begin(410E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);

 

  counter++;

  value = analogRead(A0);
  obr = (value *180)/1024;
  Serial.println(obr);
 // send packet
  LoRa.beginPacket();
  LoRa.print(obr);
  LoRa.endPacket();


  delay(1000);
}
