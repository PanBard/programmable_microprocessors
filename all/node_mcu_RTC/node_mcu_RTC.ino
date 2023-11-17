
// RTC CONNECTIONS (NodeMCU esp8266):
//    DS1302 CLK/SCLK --> D5
//    DS1302 DAT/IO --> D4
//    DS1302 RST/CE --> D2
//    DS1302 VCC --> 3.3v - 5v
//    DS1302 GND --> GND

#include <ThreeWire.h>  //lib: Rtc by Makuna (by Michael C. Miller)
#include <RtcDS1302.h>  //lib: Rtc by Makuna (by Michael C. Miller)

ThreeWire myWire(D6,D5,D7); //DS1302 -> DAT, SCLK, RST
RtcDS1302<ThreeWire> Rtc(myWire);

void setup () 
{
    Serial.begin(9600);

    Serial.print("compiled: ");
    Serial.print(__DATE__);
    Serial.println(__TIME__);

    Rtc.Begin();

    // RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    // printDateTime(compiled);
    // Serial.println();
}

void loop () 
{
    RtcDateTime now = Rtc.GetDateTime();

    printDateTime(now);
    Serial.println();

    delay(10000); // ten seconds
}

#define countof(a) (sizeof(a) / sizeof(a[0]))

void printDateTime(const RtcDateTime& dt)
{
    char datestring[20];

    snprintf_P(datestring, 
            countof(datestring),
            PSTR("%02u-%02u-%04u %02u:%02u:%02u"),            
            dt.Day(),
            dt.Month(),
            dt.Year(),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );
    Serial.print(datestring);
}
