#include "lora_stuff.h"

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  lora_start();
}

void loop() {
  lora_send_test_message_repetitively();
  loraEngineReceiver();
  change_parameter();
  Serial.println(message);
}