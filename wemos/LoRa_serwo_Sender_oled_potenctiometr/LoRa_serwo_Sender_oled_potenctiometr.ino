#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>

#include <U8g2lib.h>
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);

#define ss 15
#define rst 16
#define dio0 2


int counter = 0;
String myString = "";
//serwo

 int value = 0;
 int obr=0;
int obr_copy = 0;
//serwo

int a=1;

char b[5];
char count[5];

String str;



void setup() {
  Serial.begin(9600);
  while (!Serial);
  u8g2.begin();
  Serial.println("LoRa Sender");
  Serial.println(counter);

  LoRa.setPins(ss, rst, dio0);

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}



void loop() {
  
  
  value = analogRead(A0);
  obr = (value *180)/1023;
  

 if(obr != obr_copy){
   myString = String(obr);
  Serial.print("Sending packet = [ obroty: "+myString+" ] counter: ");
  Serial.println(counter);
 // send packet
  LoRa.beginPacket();
  LoRa.print(obr);
  LoRa.endPacket();
  counter++;
  obr_copy = obr;
 }
  


str=String(obr);
str.toCharArray(b,5);

str = String(counter);
str.toCharArray(count,5);


u8g2.firstPage();
do {
    //keep this the same as it pages through the data generating the screen
    u8g2.setFont(u8g2_font_ncenB10_tr);
    u8g2.drawStr(10,20,"Obroty:");
    u8g2.drawStr(80,20,b);
    u8g2.drawStr(10,40,"Send: ");
    u8g2.drawStr(80,40,count);
  } while ( u8g2.nextPage() );
 
  // delay(1000);
}


