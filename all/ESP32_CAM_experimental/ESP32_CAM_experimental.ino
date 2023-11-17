#include "esp_camera.h"
#include "Arduino.h"
#include "FS.h"                // SD Card ESP32
#include "SD_MMC.h"            // SD Card ESP32
#include "soc/soc.h"           // Disable brownour problems
#include "soc/rtc_cntl_reg.h"  // Disable brownour problems
#include "driver/rtc_io.h"
#include <EEPROM.h>            // read and write from flash memory
#include "base64.h"
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


void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
 
  Serial.begin(9600);

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
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG; 

                                                        //               160x120 (QQVGA), 128x160 (QQVGA2), 176x144 (QCIF), 240x176 (HQVGA),
                                                        //               320x240 (QVGA), 400x296 (CIF), 640x480 (VGA, default), 800x600 (SVGA),
                                                        //               1024x768 (XGA), 1280x1024 (SXGA), 1600x1200 (UXGA)
  

    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  
  
 
    // Init Camera
  esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

}

void loop() {
  print_image();
   delay(4000);
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
  tTimer = millis();
  Serial.println(base64data);
  Serial.println("printing image as base64 in serial moonitor took " + String((millis() - tTimer)/1000) + " seconds");
  
// tTimer = millis();
// Serial.print(fb->height); Serial.write(','); // write the height
// Serial.print(fb->width); Serial.write(','); // write the width
// for (int i = 0; i < fb->len; i++){ // dump the pixels' value
//   Serial.print(fb->buf[i]);
//   if (i != fb->len -1) Serial.write(',');
//   else Serial.println();
// }
// Serial.println("printing image as bytecode in serial moonitor took " + String((millis() - tTimer)/1000) + " seconds");


    // Turns off the ESP32-CAM white on-board LED (flash) connected to GPIO 4
  // pinMode(4, OUTPUT);
  // digitalWrite(4, LOW);
  // rtc_gpio_hold_en(GPIO_NUM_4);
  Serial.println("[APP] Free memory: " + String(esp_get_free_heap_size()) + " bytes");
   esp_camera_fb_return(fb);        // return frame so memory can be released
    Serial.println("Memory released!");

}