#include <SPI.h>
#include <LoRa.h>



//---lora
#define ss 12  // for esp32 LOLIN S2 Mini-> SCK:7, MISO: 9, MOSI: 11, NSS: 12, RST: 18
#define rst 16
#define dio0 2
String inString = "";  // string to hold input from lora
int counter = 0; //count received lora packets 
long intervalTime = 0; //for time measurement between received lora packet
String message = ""; // save message from lora
String rssi = ""; // save rssi for message
//---lora

//-----async
long lastSendTime = 0;        // last send time
int INTERVAL = 3000;          // interval between sends (ms)                              <<<<------ MANUAL SETUP
//-----async

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

void lora_send_default_message() {

   if (millis() - lastSendTime > INTERVAL){
      Serial.print("Sending packet: ");
      Serial.println(counter);
      // send packet 
      LoRa.beginPacket();
      LoRa.print("jazda ");
      LoRa.print(counter);
      LoRa.endPacket();
      blink();
      counter++;
      lastSendTime = millis(); //set new send time
    }

}

void lora_send_message(String message) {
  Serial.println("Sending message: "+ message);
  // send packet
  LoRa.beginPacket();
  LoRa.print(message);
  LoRa.endPacket();
  blink();
}

void loraEngineReceiver() {
  int packetSize = LoRa.parsePacket();

  if (packetSize) {
    blink();
    // read packet
    while (LoRa.available()) { //loop for lora message
      int inChar = LoRa.read();
      inString += (char)inChar;
    }
    message = inString;
    rssi = String(LoRa.packetRssi());
    inString = ""; // clear received message
    counter++;
    intervalTime = millis();

    Serial.print("Received packet: ");
    Serial.print(message);
    Serial.println(" (with RSSI "+String(rssi)+")");
  }
}


void change_parameter(){

 if(String(message) == "Bandwidth[125E3]")
  {
    Serial.println("Bandwidth = default [125E3]");
    LoRa.setSignalBandwidth(125E3);
    Serial.println("bandwidth_state ="+String(message));
    message ="";
    Serial.println("[control]Message: "+String(message));
    lora_send_message("Bandwidth[125E3]OK");
  }
  
  else  if(String(message)  == "Bandwidth[250E3]")
  {
    Serial.println("Bandwidth = default [250E3]");
    LoRa.setSignalBandwidth(250E3);
    Serial.println("bandwidth_state ="+String(message));
    message ="";
    Serial.println("[control]Message: "+String(message));
    lora_send_message("Bandwidth[250E3]OK");
  }

  else    if(String(message)  == "Bandwidth[500E3]")
  {
    Serial.println("Bandwidth = default [500E3]");
    LoRa.setSignalBandwidth(500E3);
    Serial.println("bandwidth_state ="+String(message));
    message ="";
    Serial.println("[control]Message: "+String(message));
    lora_send_message("Bandwidth[500E3]OK");
  }
  else    if(String(message)  == "Power[17]")
  {
    Serial.println("Power = default 17");
    LoRa.setTxPower(17);
    Serial.println("power_state ="+String(message));
    message ="";
    Serial.println("[control]Message: "+String(message));
    lora_send_message("Power[17]OK");
  }
  else    if(String(message)  == "Power[19]")
  {
    Serial.println("Power = default 19");
    LoRa.setTxPower(19);
    Serial.println("power_state ="+String(message));
    message ="";
    Serial.println("[control]Message: "+String(message));
    lora_send_message("Power[19]OK");
  }
  else    if(String(message)  == "Power[20]")
  {
    Serial.println("Power = default 20");
    LoRa.setTxPower(20);
    Serial.println("power_state ="+String(message));
    message ="";
    Serial.println("[control]Message: "+String(message));
    lora_send_message("Power[20]OK");
  }


}



void setup() {
  Serial.begin(9600);
  lora_start();
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  lora_send_default_message();
  loraEngineReceiver();
  change_parameter();
}