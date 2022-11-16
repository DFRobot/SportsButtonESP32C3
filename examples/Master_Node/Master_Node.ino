/*!
 * @file Master_Node.ino
 * @brief This is the master node program of the C3 device, which has three preset modes
         Mode_One Whac-A-Mole Mode
         Mode_Two Relay Race Mode
         Mode_Three Random Picker Mode
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @author Pai Da Xing
 * @version  V1.0
 * @date  2022-11-01
 * @url 
 */

#include <Arduino.h>
#include <U8g2lib.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
/*
 * Display I2C interface constructor of hardware
 *@param rotation：U8G2_R0 has no rotation and keeps horizontal, draw from left to right
           U8G2_R1 rotate 90 degrees clockwise, draw from top to bottom
           U8G2_R2 rotate 180 degrees clockwise, draw from right to left
           U8G2_R3 rotate 270 degrees clockwise, draw from bottom to top
*/
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R2, /* reset=*/U8X8_PIN_NONE);

#include <Adafruit_NeoPixel.h>
#define PIN_LED 0
//#define PIN_LED 1
#define NUM_LED 1
Adafruit_NeoPixel RGB_Strip = Adafruit_NeoPixel(NUM_LED, PIN_LED, NEO_GRB + NEO_KHZ800);


#include <WifiEspNow.h>
#include <WiFi.h>

int randNumber;
int fraction, Judgment, key = 0;

unsigned long last_button_time = 0;
unsigned long last_button_time1 = 0;
unsigned long last_button_time2 = 0;
static unsigned long timepoint = 0;
char key_num = 0;
String Mode = "one";

bool button_press = false;
bool quit = true;
bool interval = true;


// static uint8_t PEER0[]{ 0x84, 0xF7, 0x03, 0x42, 0x00, 0x15 };  //A
// static uint8_t PEER1[]{ 0x84, 0xF7, 0x03, 0x5F, 0x96, 0xB9 };  //B
// static uint8_t PEER2[]{ 0x84, 0xF7, 0x03, 0xA9, 0xDA, 0x65 };  //C

static uint8_t PEER0[]{ 0x34, 0xB4, 0x72, 0x87, 0x21, 0x81 };  //MAC address of the slave node
static uint8_t PEER1[]{ 0x60, 0x55, 0xF9, 0xC7, 0x3E, 0xDD };  //
static uint8_t PEER2[]{ 0x60, 0x55, 0xF9, 0xB0, 0x57, 0x65 };  //



int btn_Buttons = 5;  //Define button pin
int Buzzer = 6;       //Defin buzzer pin
// int btn_Buttons = 4;
// int Buzzer = 6;



//width:30,height:30
const uint8_t col[] U8X8_PROGMEM = {
  0x00, 0xc0, 0x00, 0x00, 0x00, 0xe0, 0x01, 0x00, 0x00, 0xe0, 0x01, 0x00, 0x00,
  0xc0, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0xe0,
  0x01, 0x00, 0x00, 0xf8, 0x07, 0x00, 0x06, 0xfe, 0x1f, 0x18, 0x07, 0xff, 0x3f,
  0x38, 0xdf, 0xff, 0xff, 0x3e, 0xfa, 0xff, 0xff, 0x17, 0xf0, 0xff, 0xff, 0x03,
  0xe0, 0xff, 0xff, 0x01, 0xe0, 0xff, 0xff, 0x01, 0xe0, 0xff, 0xff, 0x01, 0xe0,
  0xff, 0xff, 0x01, 0x20, 0x00, 0x00, 0x01, 0xa0, 0xff, 0x7f, 0x01, 0xa0, 0x01,
  0x60, 0x01, 0x20, 0x07, 0x38, 0x01, 0xe0, 0x0c, 0xcc, 0x01, 0xe0, 0x39, 0xe7,
  0x01, 0xe0, 0xe7, 0xf9, 0x01, 0xc0, 0x1f, 0xfe, 0x00, 0x80, 0xff, 0x7f, 0x00,
  0x00, 0xfe, 0x1f, 0x00, 0x00, 0xf8, 0x07, 0x00, 0x00, 0xe0, 0x01, 0x00, 0x00,
  0xc0, 0x00, 0x00
};










void setup() {
  Serial.begin(115200);
  Serial.println();
  u8g2.begin();
  u8g2.enableUTF8Print();
  IO_init();
  SetAP();
  Init_ESP_NOW();
  fengmingqi();
  RGB_Strip.begin();
  RGB_Strip.show();  //Disable all the pixel by default when initialized
  RGB_Strip.setBrightness(255);//Set brightness


  u8g2.setFont(u8g2_font_bracketedbabies_tr);
  u8g2.firstPage();
  do {
    u8g2.drawXBMP(/* x=*/0, /* y=*/0, /* width=*/30, /* height=*/30, col);  //Draw images
    u8g2.setFont(u8g2_font_sticker_mel_tr);
    u8g2.drawStr(/* x=*/30, /* y=*/42, " DFROBOT!");

  } while (u8g2.nextPage());
}

void loop() {
  btn_Event();
}


//Init config of pin
void IO_init() {
  pinMode(btn_Buttons, INPUT_PULLUP);
  pinMode(Buzzer, OUTPUT);
  digitalWrite(Buzzer, 0);
}




void btn_Event() {

  if (digitalRead(btn_Buttons) == 0) {
    key_num++;
    if (key_num == 2) {
      key_num = 1;
    }
  }

  if (key_num == 1) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_wqy16_t_gb2312);
    u8g2.setCursor(0, 10);
    u8g2.print("Mode setting");
    u8g2.setFont(u8g2_font_wqy16_t_gb2312);
    u8g2.setCursor(0, 48);
    u8g2.print("pattern");
    u8g2.setCursor(70, 48);
    u8g2.print(Mode);
    u8g2.sendBuffer();
  }

  unsigned long time = millis();
  if (digitalRead(btn_Buttons) == 0) {
    if (!button_press) {
      button_press = true;
      last_button_time = time;
    }
  } else if (button_press) {
    button_press = false;

    //Click the button to select the mode
    if ((time - last_button_time) >= 20 && (time - last_button_time) < 1000) {
      key++;
      delay(500);
      if (key == 4) key = 1;
      if (key == 3) {
        Mode = "three";
      }
      if (key == 2) {
        Mode = "two";
      }
      if (key == 1) {
        Mode = "one";
      }
    }

    //Long press the button to enter the corresponding mode
    if ((time - last_button_time) >= 1000) {
      quit = true;
      button_press = false;
      key_num = 0;
      fraction = 0;


      for (int f = 3; f >= 1; f--) {

        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_wqy16_t_gb2312);
        u8g2.setCursor(30, 30);
        u8g2.print(f);
        u8g2.sendBuffer();
        delay(1000);
      }
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_wqy16_t_gb2312);
      u8g2.setCursor(30, 30);
      u8g2.print("GO");
      u8g2.sendBuffer();





      //Mode One
      if (Mode == "one") {
        Send_data3("one");
        colorWipe(RGB_Strip.Color(0, 255, 0));
        unsigned long time = millis();
        last_button_time1 = time;


        int c1 = 0;
        while (quit) {

          unsigned long time = millis();
          if (digitalRead(btn_Buttons) == 0) {
            if (!button_press) {
              button_press = true;
              last_button_time = time;
            }
          } else if (button_press) {
            button_press = false;
            if ((time - last_button_time) >= 2000) {
              quit = false;
              u8g2.clearBuffer();
              u8g2.setFont(u8g2_font_wqy16_t_gb2312);
              u8g2.setCursor(30, 30);
              u8g2.print("quit");
              u8g2.sendBuffer();
              delay(1000);
            }
          }
          if (interval) {
            last_button_time2 = time;
            interval = false;
          }
          if ((time - last_button_time2) >= 1000) {
            interval = true;
            //Generate a random number and assign it to randNumber
            randNumber = random(0, 4);
            Serial.println(randNumber);
            c1++;
            if (randNumber == 0) {
              Send_data0();
            }


            if (randNumber == 1) {
              Send_data1();
            }

            if (randNumber == 2) {
              Send_data2();
            }



            if (randNumber == 3) {
              timepoint = millis();
              colorWipe(RGB_Strip.Color(255, 0, 0));
              fengmingqi();

              while (1 < 2) {


                if (millis() - timepoint < 2000U) {

                  if (digitalRead(btn_Buttons) == 0) {
                    colorWipe(RGB_Strip.Color(0, 255, 0));
                    fraction++;

                    break;
                  }
                }
                if (millis() - timepoint > 2000U) {
                  timepoint = millis();
                  colorWipe(RGB_Strip.Color(0, 255, 0));
                  break;
                }
              }
            }


            Serial.println(fraction);
            u8g2.clearBuffer();
            u8g2.setFont(u8g2_font_wqy16_t_gb2312);
            u8g2.setCursor(0, 15);
            u8g2.print("integral");
            u8g2.setCursor(65, 15);
            u8g2.print(fraction);
            u8g2.sendBuffer();
          }
        }

        Serial.println(fraction);
        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_wqy16_t_gb2312);
        u8g2.setCursor(0, 15);
        u8g2.print("integral");
        u8g2.setCursor(65, 15);
        u8g2.print(fraction);

        u8g2.setCursor(0, 35);
        u8g2.print("Time use");
        u8g2.setCursor(78, 35);
        u8g2.print((millis() - last_button_time1) / 1000);

        int hit = fraction * 100 / c1;
        u8g2.setCursor(0, 55);
        u8g2.print("hit rate");
        u8g2.setCursor(78, 55);
        u8g2.print(hit);

        u8g2.sendBuffer();
      }

      //Mode Two
      if (Mode == "two") {
        Send_data3("two");

        while (Mode == "two" && quit) {

          unsigned long time = millis();

          if (digitalRead(btn_Buttons) == 0) {
            if (!button_press) {
              button_press = true;
              last_button_time = time;
            }
          } else if (button_press) {
            button_press = false;
            if ((time - last_button_time) >= 2000) {
              quit = false;
              Send_data3("gb");
              u8g2.clearBuffer();
              u8g2.setFont(u8g2_font_wqy16_t_gb2312);
              u8g2.setCursor(30, 30);
              u8g2.print("quit");
              u8g2.sendBuffer();
              delay(1000);
            }
          }

          if (digitalRead(btn_Buttons) == 0 && fraction % 4 == 0) {
            fraction++;


            colorWipe(RGB_Strip.Color(0, 0, 0));
            delay(500);
          }

          if (Judgment != fraction) {
            int quyu;
            quyu = fraction % 4;
            if (quyu == 1) {
              Send_data0();
            }

            Judgment = fraction;
          }
        }
      }

      //Mode Three
      if (Mode == "three") {
        Send_data3("three");
        while (Mode == "three") {
          for (int xh = 0; xh <= 2; xh++) {
            if (xh == 0) {
              Send_data0();
            }
            if (xh == 1) {
              Send_data1();
            }
            if (xh == 2) {
              Send_data2();
            }
          }

          colorWipe(RGB_Strip.Color(0, 0, 255));
          fengmingqi();
          colorWipe(RGB_Strip.Color(0, 0, 0));

          if (digitalRead(btn_Buttons) == 0) {
            Send_data3("threes");

            randNumber = random(0, 4);
            if (randNumber == 0) {
              Send_data0();
            }
            if (randNumber == 1) {
              Send_data1();
            }
            if (randNumber == 2) {
              Send_data2();
            }
            if (randNumber == 3) {
              colorWipe(RGB_Strip.Color(0, 0, 255));
            }
            break;
          }
        }
      }
    }
  }
}


//Configure AP
void SetAP() {
  WiFi.persistent(false);
  WiFi.mode(WIFI_AP);
  WiFi.disconnect();
  WiFi.softAP("ESPNOW", nullptr, 3);
  WiFi.softAPdisconnect(false);
}


//Initialize ESPNOW
void Init_ESP_NOW() {
  bool ok0 = WifiEspNow.begin();
  bool ok1 = WifiEspNow.begin();
  bool ok2 = WifiEspNow.begin();
  if (!ok0 && !ok1 && !ok2) {
    Serial.println("WifiEspNow.begin() Failure");
    ESP.restart();
  }
  WifiEspNow.onReceive(printReceivedMessage, nullptr);
  ok0 = WifiEspNow.addPeer(PEER0);
  ok1 = WifiEspNow.addPeer(PEER1);
  ok2 = WifiEspNow.addPeer(PEER2);
  if (!ok0 && !ok1 && !ok2) {
    Serial.println("WifiEspNow.addPeer() Failure");
    ESP.restart();
  }
}


//Send data to slave node A
void Send_data0() {
  String str = "a";
  char data[60];
  int len = str.length();
  for (int i = 0; i < len; i++) {
    if (i > 60) {
      break;
    }
    data[i] = str[i];
  }
  bool h = WifiEspNow.send(PEER0, reinterpret_cast<const uint8_t*>(data), len);
  Serial.println("Sending data：" + str);
  if (h) {
    Serial.println("Send successfully");
  } else {
    Serial.println("Sending failure");
  }

  delay(200);
}


//Send data to slave node B
void Send_data1() {
  String str = "a";
  char data[60];
  int len = str.length();
  for (int i = 0; i < len; i++) {
    if (i > 60) {
      break;
    }
    data[i] = str[i];
  }
  bool f = WifiEspNow.send(PEER1, reinterpret_cast<const uint8_t*>(data), len);
  Serial.println("Sending data：" + str);
  if (f) {
    Serial.println("Send successfully");
  } else {
    Serial.println("Sending failure");
  }
  delay(200);
}



//Send data to slave node C
void Send_data2() {
  String str = "a";
  char data[60];
  int len = str.length();
  for (int i = 0; i < len; i++) {
    if (i > 60) {
      break;
    }
    data[i] = str[i];
  }
  bool g = WifiEspNow.send(PEER2, reinterpret_cast<const uint8_t*>(data), len);
  Serial.println("Sending data：" + str);
  if (g) {
    Serial.println("Send successfully");
  } else {
    Serial.println("Sending failure");
  }

  delay(200);
}


//Send data to 3 slave nodes simultaneously
void Send_data3(String str) {
  char data[60];
  int len = str.length();
  for (int i = 0; i < len; i++) {
    if (i > 60) {
      break;
    }
    data[i] = str[i];
  }
  bool h = WifiEspNow.send(PEER0, reinterpret_cast<const uint8_t*>(data), len);
  bool f = WifiEspNow.send(PEER1, reinterpret_cast<const uint8_t*>(data), len);
  bool g = WifiEspNow.send(PEER2, reinterpret_cast<const uint8_t*>(data), len);
  Serial.println("Sending data：" + str);
  if (g && f && g) {
    Serial.println("Send successfully");
  } else {
    Serial.println("Sending failure");
  }
  delay(200);
}



//Data receiving callback function
void printReceivedMessage(const uint8_t mac[WIFIESPNOW_ALEN], const uint8_t* buf, size_t count,
                          void* arg) {
  String data = "";
  for (int i = 0; i < static_cast<int>(count); ++i) {
    data += char(buf[i]);
  }
  Handler(data);
}



//Data receiving decision & processing function
void Handler(String read_data) {
  if (read_data != "") {
    Serial.println("Data received:" + read_data);

    if (read_data == "on") {
      fraction++;


      Serial.println(fraction);
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_wqy16_t_gb2312);
      u8g2.setCursor(30, 30);
      u8g2.print(fraction);
      u8g2.sendBuffer();
    }

    if (Mode == "two") {
      int quyu;
      quyu = fraction % 4;
      if (quyu == 2) {
        Send_data1();
      }
      if (quyu == 3) {
        Send_data2();
      }
      if (quyu == 0) {

        colorWipe(RGB_Strip.Color(0, 255, 0));
        fengmingqi();
      }
    }
  }
}

//Buzzer driving function
void fengmingqi() {
  digitalWrite(Buzzer, 1);
  delay(200);
  digitalWrite(Buzzer, 0);
  delay(100);
  digitalWrite(Buzzer, 1);
  delay(200);
  digitalWrite(Buzzer, 0);
}

//Fill the dots with one color
void colorWipe(uint32_t c) {
  for (uint16_t i = 0; i < RGB_Strip.numPixels(); i++) {
    RGB_Strip.setPixelColor(i, c);
    RGB_Strip.show();
  }
}