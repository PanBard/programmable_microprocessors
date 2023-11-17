#include <SPI.h>
#include <LoRa.h>

#define ss 12
#define rst 16
#define dio0 2

int counter = 0;

//serwo
 int value = 0;
 int obr=0;
//serwo

void setup() {
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
  Serial.print("Sending packet: ");
  Serial.println(counter);
  counter++;
 // send packet
  LoRa.beginPacket();
  LoRa.print("madafaka");
  LoRa.print(counter);
  LoRa.endPacket();

 digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW

  delay(1000);
}
