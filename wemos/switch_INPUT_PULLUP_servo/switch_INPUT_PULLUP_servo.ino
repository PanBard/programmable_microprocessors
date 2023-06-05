#include <Servo.h>

Servo myservo;  // create servo object to control a servo
 int op = 0;

void setup() {
  myservo.attach(2);  // attaches the servo on GIO2 to the servo object
  pinMode(4, INPUT_PULLUP); //set PULLUP switch on pin GIO4 (D4) on wemos
  myservo.write(0); //set serwo to min position on start
}

void loop() {
 int pos;
myservo.write(2);//set servo to min position
delay(1000);
  if((digitalRead(4) == LOW) && (op==0)){
    myservo.write(180);//set servo to max position
    op=1;
    delay(500);
  }
  else{
    if((digitalRead(4) == LOW) && (op==1)){
      myservo.write(0);//set servo to min position
      op=0;
      delay(500);
    }
  }

}
