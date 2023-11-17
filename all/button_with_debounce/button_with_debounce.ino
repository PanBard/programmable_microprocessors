//------denounce button
#define BUTTON_PIN     33 // pin connected to button
#define DEBOUNCE_TIME  50 // the debounce time in millisecond, increase this time if it still chatters
int lastSteadyState = LOW;       // the previous steady state from the input pin
int lastFlickerableState = LOW;  // the previous flickerable state from the input pin
int currentState;                // the current reading from the input pin
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
//------denounce button

void setup() {
  Serial.begin(9600);// initialize serial communication at 9600 bits per second:
  pinMode(LED_BUILTIN, OUTPUT);//builtin led
  pinMode(BUTTON_PIN, INPUT_PULLUP);// initialize the pushbutton pin as an pull-up input the pull-up input pin will be HIGH when the switch is open and LOW when the switch is closed.
}

void loop() {
  check_button_state_debounce();
}

void check_button_state_debounce() {
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