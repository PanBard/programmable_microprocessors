/*the circuit for esp8266_nodemcu:

SD card attached to SPI bus as follows:
    MOSI --> pin D7
    MISO --> pin D6
    CLK --> pin D5
    CS --> pin D8 

RTC CONNECTIONS:
    DS1302 CLK/SCLK --> D4 
    DS1302 DAT/IO --> D3 
    DS1302 RST/CE --> D2 
    DS1302 VCC --> 3.3v - 5v
    DS1302 GND --> GND
 */

#include <ThreeWire.h>  //lib: Rtc by Makuna (by Michael C. Miller)
#include <RtcDS1302.h>  //lib: Rtc by Makuna (by Michael C. Miller)
#include <SPI.h>
#include <SD.h>
#include <Wire.h> //for sda, scl define
#include <OneWire.h> //for ds18b20
#include <DallasTemperature.h> //for ds18b20 (rem about 4.7k ohm resistor)

ThreeWire myWire(D3,D4,D2); //DS1302 -> DAT, SCLK, RST
RtcDS1302<ThreeWire> Rtc(myWire);
char datestring[20];

//-------------------
File myFile;
String str;

int number = 0;
int delay_work_time = 1000;
int delay_error_time = 2000;
int count_current_time = 0;
int time_for_log_save = 3600; // interval for log save (in seconds)
String file_name = "temperatura_28_09_2023.txt";

//DS18B20 setup
#define ONE_WIRE_BUS 5 // Data wire is plugged into port D1  on the nodemcu esp8266
OneWire oneWire(ONE_WIRE_BUS); // Pass our oneWire reference to Dallas Temperature. // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensors(&oneWire); // Pass our oneWire reference to Dallas Temperature. 
float tempC; //temp value in celsius
//DS18B20 setup

void setup () 
{
    Serial.begin(9600);

    Serial.print("compiled: ");
    Serial.print(__DATE__);
    Serial.println(__TIME__);
       
    Rtc.Begin();
    SDCardInitialization();

    pinMode(LED_BUILTIN, OUTPUT); // Initialize the LED_BUILTIN pin as an output
    digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH   
}

void loop () 
{
  if(count_current_time>=time_for_log_save){
    writeToLog();   
  }
  count_current_time += 1;
  read_temperature();
  blink(10);
  Serial.println("Waiting... timer="+String(count_current_time)+"s "+"(waiting "+calculateTimeFromSecond()+ ")");
  delay(delay_work_time);
}

String getTimeFromRTC()
{
    RtcDateTime now = Rtc.GetDateTime();

    snprintf_P(datestring, 
            countof(datestring),
            PSTR("%02u-%02u-%04u %02u:%02u:%02u"),            
            now.Day(),
            now.Month(),
            now.Year(),
            now.Hour(),
            now.Minute(),
            now.Second() );   
         
    return String(datestring);
}


//////--------------------------------------------

void SDCardInitialization()
{
  Serial.print("Initializing SD card...");
  if (!SD.begin(D8)) {
    Serial.println("initialization failed!");
    return;
  }
  writeFirstLogToFile();
  Serial.println("initialization done.");
}

void writeFirstLogToFile(){
  myFile = SD.open(file_name, FILE_WRITE);
  
  if (myFile) { // if the file opened okay, write to it:              
    str = "Device started at " + getTimeFromRTC();   
    myFile.println(str);    
    myFile.close(); // close the file
    blink(10);
    Serial.println("Writing ***{"+ str +"}*** to --> "+ file_name + " is done.");
    
  } else {// if the file didn't open, print an error:    
    Serial.println("error opening "+file_name + " ; time: "+ getTimeFromRTC() +" waiting "+String(delay_error_time/1000) + "s");
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
    count_current_time = 0;
    Serial.println("Writing ***{"+ str +"}*** to --> "+ file_name + " is done.");
    
  } else {// if the file didn't open, print an error:    
    Serial.println("error opening "+file_name + " ; time: "+ getTimeFromRTC() +" waiting "+String(delay_error_time/1000) + "s");
    delay(delay_error_time);
  }
}

String getStringWithLogData() {
  number +=1;  
  return "[Log nr: " + String(number) +"] ;"+ " time: "+ getTimeFromRTC() +"|"+"temp: "+String(tempC)+"st.C";
}

void blink(int miliSeconds) {
    digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on by making the voltage LOW
    delayMicroseconds(miliSeconds);                      // Wait for a second
    digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH                       
}

String calculateTimeFromSecond() 
{
  return String(time_for_log_save/3600) + "h "+ String((time_for_log_save%3600)/60) + "min " + String(time_for_log_save%60) + "s "; 
}

void read_temperature(){
  sensors.requestTemperatures(); // Send the command to get temperatures
  tempC = sensors.getTempCByIndex(0);//get the temperature from the first sensor only ByIndex
  if(tempC != DEVICE_DISCONNECTED_C){ // Check if reading was successful
    Serial.print("Temperature: ");
    Serial.println(tempC);
  } 
  else
  { Serial.println("Error: Could not read temperature data"); }
}
