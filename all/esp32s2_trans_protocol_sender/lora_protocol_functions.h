
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

