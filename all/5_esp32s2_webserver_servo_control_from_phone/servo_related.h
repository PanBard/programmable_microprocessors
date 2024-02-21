int servo_position_left_right;    // variable to store the servo position
int servo_position_up_down;    // variable to store the servo position


Servo myservo_left_right;  // create servo object to control a servo
Servo myservo_up_down;  // create servo object to control a servo

void servo_start() 
{
  int servoPin_left_right = 1;
  myservo_left_right.setPeriodHertz(50);    // standard 50 hz servo
  myservo_left_right.attach(servoPin_left_right, 800, 2600); // attaches the servo on pin 18 to the servo object

  int servoPin_up_down = 2;
  myservo_up_down.setPeriodHertz(50);    // standard 50 hz servo
  myservo_up_down.attach(servoPin_up_down, 800, 2600); // attaches the servo on pin 18 to the servo object
}

void servo_engine()
{
  myservo_left_right.write(servo_position_left_right);    // tell servo to go to position in variable 'pos'
  myservo_up_down.write(servo_position_up_down);    // tell servo to go to position in variable 'pos'
}

void move_x_servo(String message)
{
		myservo_left_right.write(message.toInt());    // tell servo to go to position
}

void move_y_servo(String message)
{ 
		myservo_up_down.write(message.toInt());    // tell servo to go to position 
}

void handle_servo_movement(String message)
{
    if(message == "right")
    { 
        if(servo_position_left_right<180)
        {
          servo_position_left_right +=20;
          servo_engine();
        }
    }

    if(message == "left")
    {
        if(servo_position_left_right>0)
        {
           servo_position_left_right -=20;
           servo_engine();
        }
       
    }

    if(message == "up")
    { 
        if(servo_position_up_down<180)
        {
          servo_position_up_down +=20;
          servo_engine();
        }
    }

    if(message == "down")
    {
        if(servo_position_up_down>0)
        {
           servo_position_up_down -=20;
           servo_engine();
        }
       
    }

    if(message == "position_zero")
    {
           servo_position_up_down = 0;      
           servo_position_left_right = 0; 
           servo_engine();
    }
}



