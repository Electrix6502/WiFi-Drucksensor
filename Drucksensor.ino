#include <SPI.h>                  // SPI Low Level/Hardware Layer Lib
#include <WiFi.h>                 // WiFi Stack
#include <WiFiClient.h>           // WiFi Client Stack
#include <WiFiAP.h>               // WiFi AccessPoint
#include <WebServer.h>            // Webserver
#include <Update.h>               // Firmware Update via WIFI
#include <Preferences.h>          // Configuration Lib
#include <nvs_flash.h>            // NVS Flash Lib für Configuration
#include <FS.h>                   // File Lib
#include <SPIFFS.h>               // Flash Speicher Filesystem

#include <TFT_eSPI.h>             // Universal TFT Lib

#include "Button2.h"              // Buttonn Events (mit debounce und so weiter)
#include "esp_adc_cal.h"          // ADC Kalibrierung
#include "LargeFont.h"            // Font

#include "time.h"

// ESP32 RTC 
#include "esp32/rom/rtc.h"

#include <PubSubClient.h>

#define AA_FONT_LARGE NotoSansBold36

#define TFT_BLACK 0x0000        // black

#define ADC_BATT             34  // Batt ADC Pin
#define ADC_PIN             32  // ADC4
#define BUTTON_1            35  // Taster 1
#define BUTTON_2            0   // Taster 2

#define uS_TO_S_FACTOR 1000000   /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  360       /* Time ESP32 will go to sleep (in seconds) */

TFT_eSPI tft = TFT_eSPI();      // init TFT library, pins defined in User_Setup.h

WebServer server(80);           // init webserver

Preferences pref;               // init Preferences

WiFiClient espClient;
PubSubClient client(espClient);

// Variablen und co

String apssid = "";
String appassword = "";

String ssid = "";
String password = "";

String hostname = "WiFi-Druck";

String sensor1 = "Sensor1";

int ringbuffer[225];

byte pos = 0;
byte werte = 0;

int dmax = 0;
int dmin = 0;

unsigned long currmillis = 0;

int color = 0;

int colors[3] = {TFT_GREEN, TFT_RED, TFT_BLUE};

int storeinterval = 0;

int config_done = 0;

String fwversion = "Wifi-Drucksensor V1.0.0";
String tempdata = "";

String header = "";

String filename = "";

String mqttServer ="";
String mqttUser = "";
String mqttPass = "";

byte mqttEnable = 0;

//Seitenanzeige
int sec = 0;
int store = 0;
int page = 0;

int adc_zero = 0;
int adc_value = 0;
int presure_value = 0;
int adc_reading = 0;

// Setup Funkton (einmaliger Aufruf beim Start)
void setup(void) 
{   
  //Pins setzen
  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);
  
  //TFT initialisieren
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  digitalWrite(TFT_BL, HIGH);
  
  //Factory Reset wenn BUTTON_1 beim Einschalten gedrückt ist
  if(!digitalRead(BUTTON_1)){
    digitalWrite(TFT_BL, HIGH);               //TFT Backlight
    tft.fillScreen(TFT_BLACK);                //TFT Clear (fill black)
    tft.setTextColor(TFT_YELLOW,TFT_BLACK);   //TFT Text Color
    tft.setTextSize(2);                       //TFT Text Size      
    tft.setCursor(0, 0, 2);                   //TFT Cursor position      
    tft.println("Factory\r\nReset");  
    
    //Preferences (EEPROM ersatz) 
    pref.begin("config", false);
    pref.clear();
    pref.end();

    //NVS Flash für die WiFi Daten und so weiter
    nvs_flash_erase();                        // erase the NVS partition and...
    nvs_flash_init();                         // initialize the NVS partition.
    delay(1000);
    ESP.restart(); 
  }

  Serial.begin(115200);
  
  prefRead(); yield();

  if(config_done){
    wifisetup(); yield();
  } else {
    apsetup(); yield();
  }
  
  serversetup();    
  
  currmillis = millis();
  
  adc_reading = analogRead(ADC_PIN);

  ringbufferAdd();
  displayWifi();

  if(mqttServer.length() > 3){
    client.setServer(mqttServer.c_str(), 1883);
    client.setCallback(callback);
    mqttEnable = 1;
  }
}
 
void loop(void) 
{ 
  server.handleClient();
  yield();
  
  mqttLoop();

  if(millis() >= currmillis + 1000){
    currmillis = millis();
    sec++;
    
    adc_reading = (adc_reading + analogRead(ADC_PIN)) / 2;

    if(sec >= 5){
      sec = 0;
      store++;
      if(store == storeinterval){
        store = 0;
        ringbufferAdd();
        mqttPub();
      }

      if(page == 0){
        page = 1;
        displayData();  
      } else {
        page = 0;
        displayGraph();
      }   
    }
  }
}

void ringbufferAdd(){
  
  ringbuffer[pos] = map(adc_reading, adc_zero, adc_value, 0, presure_value); //Umrechnen map()

  pos++;
  
  if(pos >= 224){
    pos = 0;
  }

  if(werte < 224){
    werte++;
  }
    
  dmax = ringbuffer[0];
  dmin = ringbuffer[0];
  
  for(int i = 0; i < werte; i++){
    if(ringbuffer[i] > dmax){
      dmax = ringbuffer[i];
    }
    if(ringbuffer[i] < dmin){
      dmin = ringbuffer[i];
    }     
  }
}

int ringbufferGet(){
  int p = pos;
  if(p == 0){
    p = 223;
  } else {
    p = p - 1;
  }
  return ringbuffer[p];
}