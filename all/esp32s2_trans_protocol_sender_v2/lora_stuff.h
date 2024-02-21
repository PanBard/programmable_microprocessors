#include <LoRa.h>

//---lora
#define ss 12  // for esp32 LOLIN S2 Mini-> SCK:7, MISO: 9, MOSI: 11, NSS: 12, RST: 18
#define rst 16
#define dio0 2
//---lora

//-----async
long lastSendTime = 0;        // last send time for test sender
long respondTime = 0;
int responseWaitingTime = 1000; //time between repeat seding packet in case of failure                      <<<<------ MANUAL SETUP
int interval_in_miliseconds = 3000; //                                                                     <<<<------ MANUAL SETUP
//-----async

String inString = "";  // string to hold input from lora
int counter = 0; //count received lora packets 
long intervalTime = 0; //for time measurement between received lora packet
String message = ""; // save message from lora
String rssi = ""; // save rssi for message
String global_message_status = "free_mode";
String current_checksum = "";
String send_message_buffer = "";
String received_message_buffer = "";


void blink() {
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(20);                         // wait 
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
}

int checksum_calculator(String str) 
{
  // Length (with one extra character for the null terminator)
  int str_len = str.length() + 1; 
  // Prepare the character array (the buffer) 
  char char_array[str_len];
  // Copy it over 
  str.toCharArray(char_array, str_len);
  int checksum = 0;
  for(char h: char_array){
      checksum += int(h);
    }
  return checksum;
}

String return_text_with_checksum(String text)
{
  text += "*"+String(checksum_calculator(text));
  return text ;
}

boolean validate_message(String message)
{
   // Length (with one extra character for the null terminator)
  int str_len = message.length() + 1; 
  // Prepare the character array (the buffer) 
  char char_array[str_len];
  // Copy it over 
  message.toCharArray(char_array, str_len);

  String only_message = "";
  String checksum_from_message = "";
  boolean check = false;
  for(char x : char_array)
    {
      if(x == '*')
      {
        check = true;
      }
      if(!check)
      {
        only_message+=String(x);
      }
      if(check)
      {
        if(x != '*')
        {
          checksum_from_message+=String(x);
        }        
      }
      
    }
  int calculated_chesum = checksum_calculator(only_message);
  int checksum_from_message_int = checksum_from_message.toInt();
  if(calculated_chesum == checksum_from_message_int)
  {
      return true;
  }
  else
  {
    return false;
  }
}

String return_message_without_checksum(String message)
{
    // Length (with one extra character for the null terminator)
  int str_len = message.length() + 1; 
  // Prepare the character array (the buffer) 
  char char_array[str_len];
  // Copy it over 
  message.toCharArray(char_array, str_len);
  String only_message = "";
  String checksum_from_message = "";
  boolean check = false;
  for(char x : char_array)
    {
      if(x == '*')
      {
        check = true;
      }
      if(!check)
      {
        only_message+=String(x);
      }
    }
  return only_message;
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

void lora_send_message(String message) 
{
  send_message_buffer = message;
  String final_message = return_text_with_checksum(message);
  current_checksum = String(checksum_calculator(final_message));
  Serial.println("Sending message: "+ final_message);
  // send packet
  LoRa.beginPacket();
  LoRa.print(final_message);
  LoRa.endPacket();
  global_message_status = "waiting_for_response";
  respondTime = millis(); //set new  time - waiting for response
  blink();
}

void lora_send_response(String response)
{ 
  // send packet
  LoRa.beginPacket();
  LoRa.print(String(checksum_calculator(inString)));
  LoRa.endPacket();
  blink();
}

void lora_send_test_message_repetitively() 
{

   if (millis() - lastSendTime > interval_in_miliseconds)
   {
      if(global_message_status == "free_mode")
      { 
        counter++;
        lora_send_message("jazda "+String(counter));
        lastSendTime = millis(); //set new send time
      }
      else if (global_message_status == "send_failure")
      {
        lora_send_message("jazda "+String(counter));
        lastSendTime = millis(); //set new send time
        global_message_status="";
      }
      
    }
}

void check_response( String message)
{
   if(message == current_checksum )
    {
      global_message_status = "free_mode";
    }
    else 
    {
      if (millis() - lastSendTime > responseWaitingTime)
      {
          global_message_status = "free_mode";
          lora_send_message(send_message_buffer);
      }
    }
}

void loraEngineReceiver() 
{
  int packetSize = LoRa.parsePacket();

  if (packetSize) 
  {
    blink();
    // read packet
    while (LoRa.available()) //loop for lora message receiver
    { 
      int inChar = LoRa.read();
      inString += (char)inChar;
    }
    bool message_status = validate_message(inString);
    if(message_status && (global_message_status =="waiting_for_response"))
    {
      message = return_message_without_checksum(inString);
      check_response(message);
      message="";
    }

    else if(message_status && (global_message_status =="free_mode"))
    {
      message = return_message_without_checksum(inString); 
      received_message_buffer = message;
    }
    
    rssi = String(LoRa.packetRssi());
    inString = ""; // clear received message
    intervalTime = millis();

    Serial.print("Received packet: ");
    Serial.print(message);
    Serial.println(" (with RSSI "+String(rssi)+")");
  }
}


void change_parameter(){

 if(String(message) == "Bandwidth[125E3]")
  {
    Serial.println("Bandwidth = default [125E3]");
    LoRa.setSignalBandwidth(125E3);
    Serial.println("bandwidth_state ="+String(message));
    message ="";
    Serial.println("[control]Message: "+String(message));
    lora_send_message("Bandwidth[125E3]OK");
  }
  
  else  if(String(message)  == "Bandwidth[250E3]")
  {
    Serial.println("Bandwidth = default [250E3]");
    LoRa.setSignalBandwidth(250E3);
    Serial.println("bandwidth_state ="+String(message));
    message ="";
    Serial.println("[control]Message: "+String(message));
    lora_send_message("Bandwidth[250E3]OK");
  }

  else    if(String(message)  == "Bandwidth[500E3]")
  {
    Serial.println("Bandwidth = default [500E3]");
    LoRa.setSignalBandwidth(500E3);
    Serial.println("bandwidth_state ="+String(message));
    message ="";
    Serial.println("[control]Message: "+String(message));
    lora_send_message("Bandwidth[500E3]OK");
  }
  else    if(String(message)  == "Power[17]")
  {
    Serial.println("Power = default 17");
    LoRa.setTxPower(17);
    Serial.println("power_state ="+String(message));
    message ="";
    Serial.println("[control]Message: "+String(message));
    lora_send_message("Power[17]OK");
  }
  else    if(String(message)  == "Power[19]")
  {
    Serial.println("Power = default 19");
    LoRa.setTxPower(19);
    Serial.println("power_state ="+String(message));
    message ="";
    Serial.println("[control]Message: "+String(message));
    lora_send_message("Power[19]OK");
  }
  else    if(String(message)  == "Power[20]")
  {
    Serial.println("Power = default 20");
    LoRa.setTxPower(20);
    Serial.println("power_state ="+String(message));
    message ="";
    Serial.println("[control]Message: "+String(message));
    lora_send_message("Power[20]OK");
  }


}


