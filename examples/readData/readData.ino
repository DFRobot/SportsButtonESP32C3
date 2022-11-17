#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>

#if defined ARDUINO_SAM_ZERO
#define OLED_DC  7
#define OLED_CS  5
#define OLED_RST 6
/*ESP32 */
#elif defined(ESP32)
#define OLED_DC  D2
#define OLED_CS  D6
#define OLED_RST D3
/*ESP8266*/
#elif defined(ESP8266)
#define OLED_DC  D4
#define OLED_CS  D6
#define OLED_RST D5
/*AVR series board*/
#else
#define OLED_DC  2
#define OLED_CS  3
#define OLED_RST 4
#endif
U8G2_SSD1309_128X64_NONAME2_1_4W_HW_SPI u8g2(/* rotation=*/U8G2_R0, /* cs=*/ OLED_CS, /* dc=*/ OLED_DC,/* reset=*/OLED_RST);
char key_num =1; 
const uint8_t col[] U8X8_PROGMEM= {0x00,0x00,0x00,0xFE,0xFF,0x07,0xFE,0xFF,0x07,0x06,0x00,0x06,0x06,0x00,0x06,0x06,
0x00,0x06,0x16,0x05,0x07,0x16,0x8D,0x07,0x16,0x89,0x06,0x16,0x89,0x06,0x16,0xD9,
0x06,0x16,0x51,0x06,0x16,0x71,0x06,0xF6,0x21,0x06,0x06,0x00,0x06,0x06,0x00,0x06,
0x06,0x00,0x06,0xFE,0xFF,0x07,0xFE,0xFF,0x07,0x00,0x00,0x00}; 

const uint8_t col1[] U8X8_PROGMEM= {0x00,0x00,0x00,0xFE,0xFE,0x01,0x10,0xFE,0x01,0x38,0x86,0x01,0x54,0x86,0x01,0x10,
0x86,0x01,0x00,0x86,0x01,0x00,0x86,0x01,0x44,0x86,0x01,0x44,0x86,0x01,0x7C,0x86,
0x01,0x44,0x86,0x01,0x44,0x86,0x01,0x00,0x86,0x01,0x10,0x86,0x01,0x54,0x86,0x01,
0x38,0x86,0x01,0x10,0xFE,0x01,0xFE,0xFE,0x01,0x00,0x00,0x00}; 

const uint8_t col2[] U8X8_PROGMEM= {0x00,0x00,0x00,0x00,0x04,0x00,0x08,0x04,0x02,0x10,0x04,0x01,0x20,0x84,0x00,0x00,
0x00,0x00,0x80,0x3F,0x00,0x80,0x20,0x00,0x80,0x20,0x00,0xBC,0xA0,0x07,0x80,0x20,
0x00,0x80,0x20,0x00,0x80,0x3F,0x00,0x20,0x80,0x00,0x10,0x04,0x01,0x08,0x04,0x02,
0x00,0x04,0x00,0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; 

const uint8_t col3[] U8X8_PROGMEM= {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x44,0x00,0x20,0x22,0x00,0x10,
0x11,0x00,0x20,0x22,0x00,0x40,0x44,0x00,0x20,0x22,0x00,0x10,0x11,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0xFC,0xFF,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0xFF,0x03,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; 

#include "DFRobot_EnvironmentalSensor.h"
#if defined(ARDUINO_AVR_UNO)||defined(ESP8266)
#include <SoftwareSerial.h>
#endif

#define MODESWITCH        /*UART:*/1 /*I2C: 0*/

#if MODESWITCH
#if defined(ARDUINO_AVR_UNO)||defined(ESP8266)
  SoftwareSerial mySerial(/*rx =*/4, /*tx =*/5);
  DFRobot_EnvironmentalSensor environment(/*addr =*/SEN050X_DEFAULT_DEVICE_ADDRESS, /*s =*/&mySerial);
#else
  DFRobot_EnvironmentalSensor environment(/*addr =*/SEN050X_DEFAULT_DEVICE_ADDRESS, /*s =*/&Serial1); 
#endif
#else
DFRobot_EnvironmentalSensor environment(/*addr = */SEN050X_DEFAULT_DEVICE_ADDRESS, /*pWire = */&Wire);
#endif
void setup()
{
#if MODESWITCH
  
#if defined(ARDUINO_AVR_UNO)||defined(ESP8266)
  mySerial.begin(9600);
#elif defined(ESP32)
  Serial1.begin(9600, SERIAL_8N1, /*rx =*/D7, /*tx =*/D5);
#else
  Serial1.begin(9600);
#endif
#endif
  Serial.begin(115200);

  while(environment.begin() != 0){
    Serial.println(" Sensor initialize failed!!");
    delay(1000);
  }
  Serial.println(" Sensor  initialize success!!");
          u8g2.begin();
 u8g2.setFontPosTop();
}

void loop()
{
 if(touchRead(4)<=20)
 {
    key_num++;
    if(key_num==3)key_num=1;
 }

  
  Serial.println("-------------------------------");
  Serial.print("Temp: ");
  Serial.print(environment.getTemperature(TEMP_C));
  Serial.println(" ℃");
  Serial.print("Humidity: ");
  Serial.print(environment.getHumidity());
  Serial.println(" %");
  Serial.print("Ultraviolet intensity: ");
  Serial.print(environment.getUltravioletIntensity());
  Serial.println(" mw/cm2");
  Serial.print("LuminousIntensity: ");
  Serial.print(environment.getLuminousIntensity());
  Serial.println(" lx");
  Serial.print("Atmospheric pressure: ");
  Serial.print(environment.getAtmospherePressure(HPA));
  Serial.println(" hpa");
  Serial.print("Elevation: ");
  Serial.print(environment.getElevation());
  Serial.println(" m");
  Serial.println("-------------------------------");

if(key_num==1)
{ 
  u8g2.firstPage();   
  do
  {
       u8g2.setFont(u8g2_font_HelvetiPixelOutline_tr);
 
    u8g2.setCursor(/* x=*/2, /* y=*/2);
    u8g2.print("T :");
    u8g2.setCursor(/* x=*/35, /* y=*/2);
    u8g2.print(environment.getTemperature(TEMP_C));
    u8g2.setCursor(/* x=*/85, /* y=*/2);
    u8g2.print("*C");

    u8g2.setCursor(/* x=*/2, /* y=*/27);
    u8g2.print("H :");
    u8g2.setCursor(/* x=*/35, /* y=*/27);
    u8g2.print(environment.getHumidity());
    u8g2.setCursor(/* x=*/85, /* y=*/27);
    u8g2.print("%");




    u8g2.sendBuffer();          

  } while( u8g2.nextPage() );
}



if(key_num==2)
{
  u8g2.firstPage(); 
  do {
     u8g2.drawXBMP( /* x=*/0 , /* y=*/1 , /* width=*/20 , /* height=*/20 , col );
     u8g2.drawXBMP( /* x=*/0 , /* y=*/30 , /* width=*/20 , /* height=*/20 , col1 );

u8g2.setFont(u8g2_font_HelvetiPixelOutline_tr);
u8g2.setCursor(/* x=*/40, /* y=*/1);
    u8g2.print(environment.getUltravioletIntensity());
        u8g2.setCursor(/* x=*/25, /* y=*/1);
    u8g2.print(":");
            u8g2.setCursor(/* x=*/90, /* y=*/1);
    u8g2.print("mw/cm");

u8g2.setCursor(/* x=*/40, /* y=*/30);
    u8g2.print(environment.getElevation());
            u8g2.setCursor(/* x=*/25, /* y=*/30);
    u8g2.print(":");
            u8g2.setCursor(/* x=*/102, /* y=*/30);
    u8g2.print("m");
      
    
  }while(u8g2.nextPage()); 
    
  delay(2000);
  
    u8g2.firstPage(); 
  do {
     u8g2.drawXBMP( /* x=*/0 , /* y=*/1 , /* width=*/20 , /* height=*/20 , col2 );
     u8g2.drawXBMP( /* x=*/0 , /* y=*/30 , /* width=*/20 , /* height=*/20 , col3 );
   

u8g2.setFont(u8g2_font_HelvetiPixelOutline_tr);
u8g2.setCursor(/* x=*/40, /* y=*/1);
    u8g2.print(environment.getLuminousIntensity());
            u8g2.setCursor(/* x=*/25, /* y=*/1);
    u8g2.print(":");
            u8g2.setCursor(/* x=*/103, /* y=*/1);
    u8g2.print("lx");

u8g2.setCursor(/* x=*/40, /* y=*/30);
    u8g2.print(environment.getAtmospherePressure(HPA));
            u8g2.setCursor(/* x=*/25, /* y=*/30);
    u8g2.print(":");
            u8g2.setCursor(/* x=*/79, /* y=*/30);
    u8g2.print("hpa");
       
  }while(u8g2.nextPage());   
}
  delay(1000);
}
