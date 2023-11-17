/*the circuit for esp8266_nodemcu:
    SD card attached to SPI bus as follows:
    MOSI - pin D7
    MISO - pin D6
    CLK - pin D5
    CS - pin D8 
 */

#include <SPI.h>
#include <SD.h>

File myFile;
String str;
String currentTime;

int number = 0;
int delay_work_time = 1000;
int delay_error_time = 2000;
int battery_time_log_save = 0;
int time_for_log_save = 10; // interval for log save

unsigned long myTime;

String file_name = "rol16.txt";

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT); // Initialize the LED_BUILTIN pin as an output
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH       
  Serial.print("Initializing SD card...");

  if (!SD.begin(D8)) {
    Serial.println("initialization failed!");
    return;
  }
  writeFirstLogToFile();
  Serial.println("initialization done.");
}

void loop() {

  
  if(battery_time_log_save>=time_for_log_save){
    writeToLog();    
  }
  battery_time_log_save += 1;
  blink(10);
  Serial.println("Waiting... timer="+String(battery_time_log_save)+"s");
  delay(delay_work_time);

}

void writeFirstLogToFile(){
  myFile = SD.open(file_name, FILE_WRITE);
  currentTime = getStringWithCurrentTime();
  if (myFile) { // if the file opened okay, write to it:              
    str = "Device started at " + currentTime;   
    myFile.println(str);    
    myFile.close(); // close the file
    blink(10);
    Serial.println("Writing ***{"+ str +"}*** to --> "+ file_name + " is done.");
    
  } else {// if the file didn't open, print an error:    
    Serial.println("error opening "+file_name + " ; time: "+ getStringWithCurrentTime() +" waiting "+String(delay_error_time/1000) + "s");
    delay(delay_error_time);
  }
}

void writeToLog(){ 
  myFile = SD.open(file_name, FILE_WRITE);
  if (myFile) { // if the file opened okay, write to it:              
    str = getStringWithLogData();   
    myFile.println(str);    
    myFile.close(); // close the file
    blink(10);
    battery_time_log_save = 0;
    Serial.println("Writing ***{"+ str +"}*** to --> "+ file_name + " is done.");
    
  } else {// if the file didn't open, print an error:    
    Serial.println("error opening "+file_name + " ; time: "+ getStringWithCurrentTime() +" waiting "+String(delay_error_time/1000) + "s");
    delay(delay_error_time);
  }
}

String getStringWithLogData() {
  number +=1;
  currentTime = getStringWithCurrentTime();
  return "[Log nr: " + String(number) +"] ;"+ " time: "+ currentTime +"|";
}

String getStringWithCurrentTime() {
  myTime = millis()/1000;
  return  String(myTime/3600) + "h "+ String((myTime%3600)/60) + "min " + String(myTime%60) + "s " + "; total --> ("+ String(millis()) +" ms) ";                 
}

void blink(int miliSeconds) {
    digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on by making the voltage LOW
    delayMicroseconds(miliSeconds);                      // Wait for a second
    digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH                       
}