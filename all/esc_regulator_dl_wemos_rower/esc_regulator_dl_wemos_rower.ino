

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
 int op = 0;
 int value = 0;
 int obr=0;

void setup() {
  myservo.attach(2);  // attaches the servo on GIO2 to the servo object (D4 on wemos)
  pinMode(4, INPUT_PULLUP);
  myservo.write(0); //set serwo to max position on start
  Serial.begin(9600);
}

void loop() {
 int pos;

  value = analogRead(A0);
  Serial.println(value);
  obr = (value *180)/1024;
   myservo.write(obr);
Serial.println(obr);

}
