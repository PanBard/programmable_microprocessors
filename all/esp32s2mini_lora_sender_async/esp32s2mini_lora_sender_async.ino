#include <SPI.h>
#include <LoRa.h>

int counter = 0;
int bandwidth_state = 0; //for button to change bantwith state

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

//-----async
long lastSendTime = 0;        // last send time
int INTERVAL = 3000;          // interval between sends (ms)                              <<<<------ MANUAL SETUP
//-----async

void setup() {
  Serial.begin(9600);
  lora_start();
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  lora_send_message();
  check_button_state_debounce();
}

void lora_send_message() {
  if (millis() - lastSendTime > INTERVAL)
  {
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
    lastSendTime = millis(); //set new send time
  }
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



void do_it_if_button_pressed() 
{
  blink();
  Serial.println("The button is pressed");
    if(bandwidth_state == 0)
  {
    Serial.println("Bandwidth = default [125E3]");
    LoRa.setSignalBandwidth(125E3);
    bandwidth_state ++;
    Serial.println("bandwidth_state ="+String(bandwidth_state));
  }
  
  else  if(bandwidth_state == 1)
  {
    Serial.println("Bandwidth = default [250E3]");
    LoRa.setSignalBandwidth(250E3);
    bandwidth_state ++;
    Serial.println("bandwidth_state ="+String(bandwidth_state));
  }

  else    if(bandwidth_state == 2)
  {
    Serial.println("Bandwidth = default [500E3]");
    LoRa.setSignalBandwidth(500E3);
    bandwidth_state = 0;
    Serial.println("bandwidth_state ="+String(bandwidth_state));
  }
}

void check_button_state_debounce() 
  {
    currentState = digitalRead(BUTTON_PIN);// read the state of the switch/button:
  
    if (currentState != lastFlickerableState) { // If the switch/button changed, due to noise or pressing:
      lastDebounceTime = millis(); // reset the debouncing timer
      lastFlickerableState = currentState; // save the the last flickerable state
    }

    if (check_if_button_has_been_pressed()) { // whatever the reading is at, it's been there for longer than the debounce delay, so take it as the actual current state:
    
      if(lastSteadyState == HIGH && currentState == LOW)// if the button state has changed:
      { 
        do_it_if_button_pressed();
      }
      else if(lastSteadyState == LOW && currentState == HIGH)
        Serial.println("The button is released");
    
      lastSteadyState = currentState; // save the the last steady state
    }
  }

bool check_if_button_has_been_pressed() 
  {
    if ((millis() - lastDebounceTime) > DEBOUNCE_TIME)
    {return true;}
    else
    {return false;}
  }

