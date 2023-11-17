#include <Wire.h> //for sda, scl define
#include <OneWire.h> //for ds18b20
#include <DallasTemperature.h> //for ds18b20

//DS18B20 setup
#define ONE_WIRE_BUS 5 // Data wire is plugged into port D1  on the nodemcu esp8266
OneWire oneWire(ONE_WIRE_BUS); // Pass our oneWire reference to Dallas Temperature. // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensors(&oneWire); // Pass our oneWire reference to Dallas Temperature. 
float tempC; //temp value in celsius
//DS18B20 setup



void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT); //for led blinking
}

void loop() {
  read_temperature();
  Serial.println(tempC);
  blik_LED_BUILTIN(3);
  delay(1000);
}

void read_temperature(){
  sensors.requestTemperatures(); // Send the command to get temperatures
  tempC = sensors.getTempCByIndex(0);//get the temperature from the first sensor only ByIndex
  // if(tempC != DEVICE_DISCONNECTED_C){ // Check if reading was successful
  //   Serial.print("Temperature: ");
  //   Serial.println(tempC);
  // } 
  // else
  // { Serial.println("Error: Could not read temperature data"); }
}


void blik_LED_BUILTIN(int msec){
  digitalWrite(LED_BUILTIN, LOW);  // turn the LED on (HIGH is the voltage level)
  delay(msec);                      // wait for a second
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off by making the voltage LOW
}







