#include <SPI.h>
#include <LoRa.h>

int counter = 0;

//---lora
#define ss 12  //MOSI: 11, MISO: 9, SCK: 7
#define rst 16
#define dio0 2
//---lora

//------denounce button
#define BUTTON_PIN     33 // pin connected to button
#define DEBOUNCE_TIME  50 // the debounce time in millisecond, increase this time if it still chatters
int lastSteadyState = LOW;       // the previous steady state from the input pin
int lastFlickerableState = LOW;  // the previous flickerable state from the input pin
int currentState;                // the current reading from the input pin
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
//------denounce button

void setup() {
  Serial.begin(9600);
  lora_start();
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  lora_send_message();
  check_if_button_has_been_pressed();
  delay(1000);
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

bool check_if_button_has_been_pressed() 
  {
    if ((millis() - lastDebounceTime) > DEBOUNCE_TIME)
    {return true;}
    else
    {return false;}
  }

void do_it_if_button_pressed() 
{
  blink();
  Serial.println("The button is pressed");
}