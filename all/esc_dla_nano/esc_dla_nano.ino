#include <Servo.h>
Servo myservo;  // create servo object to control a servo
int temp_val = 0;
 int op = 0;
 int value = 0;
 int obr=0;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
     myservo.attach(2);
}

// the loop routine runs over and over again forever:
void loop() {

  value = analogRead(A0);
  // obr = (value *180)/1023;
  op = map(value,0,1023,0,180);
// Serial.println(value);
  
  // read the input on analog pin 0:
  if(op > temp_val){
    digitalWrite(LED_BUILTIN, HIGH);
  // Serial.println(op);
  temp_val = op;
  }
   myservo.write(value);
  // print out the value you read:
    Serial.println(value);
 temp_val = op;

      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(1000); 
  // delay(1);  // delay in between reads for stability
}
