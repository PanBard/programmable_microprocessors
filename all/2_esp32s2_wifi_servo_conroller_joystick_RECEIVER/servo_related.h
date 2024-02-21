//servoPin_left_right ESP32 s2 mini -> pin 1
//servoPin_up_down ESP32 s2 mini -> pin 2

int servo_position_left_right=90;    // variable to store the servo position
int servo_position_up_down=90;    // variable to store the servo position

int servo_steps = 1;

Servo myservo_left_right;  // create servo object to control a servo
Servo myservo_up_down;  // create servo object to control a servo

void servo_start() 
{
  int servoPin_left_right = 1; //ESP32 s2 mini -> pin 1
  myservo_left_right.setPeriodHertz(50);    // standard 50 hz servo
  myservo_left_right.attach(servoPin_left_right, 800, 2600); // attaches the servo on pin 1 to the servo object

  int servoPin_up_down = 2; //ESP32 s2 mini -> pin 2
  myservo_up_down.setPeriodHertz(50);    // standard 50 hz servo
  myservo_up_down.attach(servoPin_up_down, 800, 2600); // attaches the servo on pin 2 to the servo object

  myservo_left_right.write(90);    // tell servo to go to start position 
  myservo_up_down.write(90);    // tell servo to go to start position 
}

void servo_engine(String command)
{
  if(command == "right" || command == "left")
  {
     myservo_left_right.write(servo_position_left_right);    // tell servo to go to position 
  }
 
  if(command == "down" || command == "up")
  {
    myservo_up_down.write(servo_position_up_down);    // tell servo to go to position 
  }

  if(command == "position_zero" )
  {
    myservo_left_right.write(servo_position_left_right);    // tell servo to go to position 
    myservo_up_down.write(servo_position_up_down);    // tell servo to go to position 
  }

  if(command == "light_on_off" )
  {
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)  
    delay(2000);                        // wait 
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  }
  
}

void handle_servo_movement(String message)
{
    if(message == "right")
    { 
      if(servo_position_left_right<178)
      {
        servo_position_left_right +=servo_steps;
        servo_engine("right");
      }
    }

    if(message == "left")
    {
      if(servo_position_left_right>0)
      {
          servo_position_left_right -=servo_steps;
          servo_engine("left");
      }
    }

    if(message == "up")
    { 
      if(servo_position_up_down<178)
      {
        servo_position_up_down +=servo_steps;
        servo_engine("up");
      }
    }

    if(message == "down")
    {
      if(servo_position_up_down>0)
      {
          servo_position_up_down -=servo_steps;
          servo_engine("down");
      }
    }

    if(message == "position_zero")
    {
      servo_position_up_down = 90;      
      servo_position_left_right = 90; 
      servo_engine("position_zero");
    }

    if(message == "light_on_off")
    {
      servo_engine("light_on_off");
    }
}




