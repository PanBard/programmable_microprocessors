//connect joystick to 3.3v power source

#define joystickPinAxisX 3 //analogpin x pin 3
#define joystickPinAxisY 5 //analogpin y pin 5
#define joystickPinPullup 4 //joystick switch  y

int x_axis = 0;
int y_axis = 0;
int buttonState = 0; //1 = button not pressed, 0 = button pressed


void joystick_start()
{
  pinMode(joystickPinPullup,INPUT_PULLUP);
}

void read_joystick_value_and_manage_comunication()
{
  x_axis = analogRead(joystickPinAxisX);// pin 3
  y_axis = analogRead(joystickPinAxisY);// pin 5
  buttonState = digitalRead(joystickPinPullup); // pin 4

  x_axis = map(x_axis,0,8191,0,1023); //map value from analiog pin  (0 to 1023)
  y_axis = map(y_axis,0,8191,0,1023); //map value from analiog pin  (0 to 1023)

  if(x_axis>700){send_data("http://8.8.8.8/up");} //cables in front of joystick
  if(y_axis>700){send_data("http://8.8.8.8/left");}
  if(x_axis<500){send_data("http://8.8.8.8/down");}
  if(y_axis<500){send_data("http://8.8.8.8/right");}
  if(buttonState == 0) {send_data("http://8.8.8.8/position_zero");}
}
