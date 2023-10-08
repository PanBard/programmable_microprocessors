#ifndef ESP32
  #error This code is designed to run on ESP32 platform, not Arduino nor ESP8266! Please check your Tools->Board setting.
#endif
#include <SPI.h>
#include <LoRa.h>

//---------
#include "esp_camera.h"
#include "Arduino.h"
#include "FS.h"                // SD Card ESP32
#include "SD_MMC.h"            // SD Card ESP32
#include "soc/soc.h"           // Disable brownour problems
#include "soc/rtc_cntl_reg.h"  // Disable brownour problems
#include "driver/rtc_io.h"
#include <EEPROM.h>            // read and write from flash memory
#include "base64.h"
//---------

//PINOUT -------------------------------------start
#include <Wire.h> //for custom sda, scl define
//---lora 
#define SCK 13
#define MISO 2
#define MOSI 15
#define SS 14

#define RST 23
#define DIO0 19

//---lora 

//PINOUT --------------------------------------end

#define LED_BUILTIN 4
int counter = 0;

//----------
// define the number of bytes you want to access
#define EEPROM_SIZE 1

// Pin definition for CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22


uint32_t tTimer;     // used to time tasks 
//----------
String message;
void setup() {
  pinMode (LED_BUILTIN, OUTPUT);//Specify that LED pin is output
  digitalWrite(LED_BUILTIN, HIGH);
  delay(10);
  digitalWrite(LED_BUILTIN, LOW);
  //==========
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;//               160x120 (QQVGA), 128x160 (QQVGA2), 176x144 (QCIF), 240x176 (HQVGA),1024x768 (XGA), 1280x1024 (SXGA), 1600x1200 (UXGA)320x240 (QVGA), 400x296 (CIF), 640x480 (VGA, default), 800x600 (SVGA),
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;                                                                     
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
    // Init Camera
  esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
  //==========

  Serial.begin(9600);
  Serial.println("LoRa Sender");
  Serial.println(counter);
  //SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1){  Serial.print("MOSI: ");
  Serial.println(MOSI);
  Serial.print("MISO: ");
  Serial.println(MISO);
  Serial.print("SCK: ");
  Serial.println(SCK);
  Serial.print("SS: ");
  Serial.println(SS); 

  digitalWrite(LED_BUILTIN, HIGH);
  delay(1);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
   };
  }
}

void loop() {
  print_image();
  // sendData();
  delay(4000);
}




void sendData(){
  Serial.print("Sending packet: ");
  Serial.println(counter);  
  counter++;
 // send packet
  LoRa.beginPacket();
  LoRa.print("Pakiet numer:");
  LoRa.endPacket();
  Serial.println("Packet sent!");
}

void print_image(){
 
 tTimer = millis();   // store time that image capture started

  camera_fb_t * fb = NULL;
  // Take Picture with Camera
  fb = esp_camera_fb_get();  
  if(!fb) {
    Serial.println("Camera capture failed");
    // return;
  }
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);


  Serial.println("JPG image capture took " + String(millis() - tTimer) + " milliseconds");
  tTimer = millis();
  uint32_t imageLen = fb->len;
  String base64data = base64::encode(fb->buf, fb->len);      // convert buffer to base64
  Serial.println("JPG image conversion to base64 format took " + String(millis() - tTimer) + " milliseconds");
  Serial.println("Image resolution=" + String(fb->width) + "x" + String(fb->height));
  Serial.println("Image size=" + String(fb->len) + " bytes");
  Serial.println("Image format=" + String(fb->format)); 
  Serial.println("Free memory=" + String(ESP.getFreeHeap()) + " bytes");
  Serial.println("base64 lenght = " + String(base64data.length()));

  Serial.print("Sending packet: ");


  LoRa.beginPacket();
  LoRa.print("START");
  LoRa.endPacket();
  delay(1000);

int l = base64data.length();
int k1 = 0;
int k2 = 200;
 for(int i=0; i<= (l/200); i++){
    // send packet
  message = base64data.substring(k1,k2);
  LoRa.beginPacket();
  LoRa.print(message);
  LoRa.endPacket();
  k1 = k2;
  k2 = k2+200;

  digitalWrite(LED_BUILTIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
 }

  delay(500);
  LoRa.beginPacket();
  LoRa.print("SUM");
  LoRa.endPacket();
  delay(500);

  delay(500);
  LoRa.beginPacket();
  LoRa.print(String(base64data.length()));
  LoRa.endPacket();
  delay(500);

  delay(1000);
  LoRa.beginPacket();
  LoRa.print("END");
  LoRa.endPacket();
  delay(1000);
 // send packet
  // LoRa.beginPacket();
  // // LoRa.print("JPG image conversion to base64 format took " + String(millis() - tTimer) + " milliseconds");
  // // LoRa.print("Image resolution=" + String(fb->width) + "x" + String(fb->height));
  // // LoRa.print("Image size=" + String(fb->len) + " bytes");
  // // LoRa.print("Image format=" + String(fb->format));
  // // LoRa.print("Free memory=" + String(ESP.getFreeHeap()) + " bytes");
  // // LoRa.print("base64 lenght = " + String(base64data.length()));
  // // LoRa.print("Image format=" + String(fb->format));
  // LoRa.print(base64data);
  // LoRa.endPacket();
  // Serial.println("Packet sent!");



  // tTimer = millis();
  // Serial.println(base64data);
  // Serial.println("printing image as base64 in serial moonitor took " + String((millis() - tTimer)/1000) + " seconds");
  
  // Serial.println("[APP] Free memory: " + String(esp_get_free_heap_size()) + " bytes");
   esp_camera_fb_return(fb);        // return frame so memory can be released
  Serial.println("Memory released!");

}



