#include <SPI.h>
#include <LoRa.h>

int counter = 0;

//---lora
#define ss 12  // for esp32 LOLIN S2 Mini-> SCK:7, MISO: 9, MOSI: 11, NSS: 12, RST: 18
#define rst 16
#define dio0 2
//---lora

void setup() {
  Serial.begin(9600);
  lora_start();
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  lora_send_message();
  delay(2000);
}

void lora_send_message() {
  Serial.print("Sending packet: ");
  Serial.println(counter);
  // send packet
  LoRa.beginPacket();
  LoRa.print("jzd ");
  LoRa.print(counter);
  LoRa.print(" [");
  LoRa.print(LoRa.packetRssi());
  LoRa.print(" ]");
  LoRa.endPacket();
  blink();
  counter++;
}

void lora_start() {
  LoRa.setPins(ss, rst, dio0);
   if (!LoRa.begin(433E6)) {
    while (1) {      
      Serial.println("Starting LoRa failed!");
      Serial.print("MOSI: ");
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
  Serial.println("LoRa status: ok");
}

void blink() {
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(20);                         // wait 
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
}