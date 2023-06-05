#include <U8g2lib.h>
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);//for Wemos: D2(SDA),  D1(SCL);

int value = 0;
char b[5];
char count[5];

String str;

void setup() {
  // Serial.begin(9600);
   u8g2.begin();
}

void loop() {
  int pos;
  value = analogRead(A0);
  // Serial.println(value);
  str=String(value);
  str.toCharArray(b,5);
  u8g2.firstPage();
  do {
      //keep this the same as it pages through the data generating the screen
      u8g2.setFont(u8g2_font_ncenB10_tr);
      u8g2.drawStr(10,20,"Pole:");
      u8g2.drawStr(80,20,b);
    } while ( u8g2.nextPage() );
  delay(100);
}
