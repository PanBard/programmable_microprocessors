
// RTC CONNECTIONS (NodeMCU esp8266):
//    DS1302 CLK/SCLK --> D4 
//    DS1302 DAT/IO --> D3 
//    DS1302 RST/CE --> D2 
//    DS1302 VCC --> 3.3v - 5v
//    DS1302 GND --> GND

#include <ThreeWire.h>  //lib: Rtc by Makuna (by Michael C. Miller)
#include <RtcDS1302.h>  //lib: Rtc by Makuna (by Michael C. Miller)

ThreeWire myWire(D3,D4,D2); //DS1302 -> DAT, SCLK, RST
RtcDS1302<ThreeWire> Rtc(myWire);
char datestring[20];

void setup () 
{
    Serial.begin(9600);

    Serial.print("compiled: ");
    Serial.print(__DATE__);
    Serial.println(__TIME__);

    Rtc.Begin();
}

void loop () 
{
    RtcDateTime now = Rtc.GetDateTime();    
    Serial.println(printDateTime(now));
    delay(10000); // ten seconds
}

char* printDateTime(const RtcDateTime& dt)
{
    snprintf_P(datestring, 
            countof(datestring),
            PSTR("%02u-%02u-%04u %02u:%02u:%02u"),            
            dt.Day(),
            dt.Month(),
            dt.Year(),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );        
    return datestring;
}
