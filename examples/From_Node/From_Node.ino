/*!
 * @file From_Node.ino
 * @brief This is the slave node program of the C3 device, which has three preset modes
         Mode_One Whac-A-Mole Mode
         Mode_Two Relay Race Mode
         Mode_Three Random Picker Mode
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @author Pai Da Xing
 * @version  V1.0
 * @date  2022-11-01
 * @url 
 */
#include <WifiEspNow.h>
#include <WiFi.h>
#include <Adafruit_NeoPixel.h>
#define PIN_LED 0  
#define NUM_LED 1  
Adafruit_NeoPixel RGB_Strip = Adafruit_NeoPixel(NUM_LED, PIN_LED, NEO_GRB + NEO_KHZ800);


bool Modetwo = false;
bool Modeone = false;
bool Singlenode = false;
bool Vocalization = false;
String Mode = "wu";
static unsigned long time = 0;
static uint8_t PEER[]{ 0x60, 0x55, 0xF9, 0xB1, 0x83, 0xD1 };  //MAC address of the master node
//static uint8_t PEER[]{ 0x60, 0x55, 0xF9, 0xB0, 0xB0, 0x49 };

int btn_Buttons = 5;//Define button pin
int Buzzer = 6; //Defin buzzer pin


void setup() {
  Serial.begin(115200);
  Serial.println();
  IO_init();       
  SetAP();         
  Init_ESP_NOW();  
  fengmingqi();
  RGB_Strip.begin();
  RGB_Strip.show();//Disable all the pixel by default when initialized
  RGB_Strip.setBrightness(255);//Set brightness  
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

//Mode One
  if (Modeone) {
    while (1 < 2) {
      if (millis() - time < 2000U) {
        if (digitalRead(btn_Buttons) == 0) {
          colorWipe(RGB_Strip.Color(0, 255, 0));  
          Send_data("on");
          delay(200);
        }
      }

      if (millis() - time > 2000U) {
        
        colorWipe(RGB_Strip.Color(0, 255, 0));  
                                              
        break;
      }
      Modeone = false;
    }
  }

//Mode Two
  if (Modetwo) {

    if (digitalRead(btn_Buttons) == 0) {
      colorWipe(RGB_Strip.Color(0, 0, 0));
      Send_data("on");
      delay(200);
      Modetwo = false;
    }
  }

//Single Node Mode
  if (!Singlenode) {
    if (digitalRead(btn_Buttons) == 0) {


      colorWipe(RGB_Strip.Color(0, 0, 255));
      fengmingqi();
      colorWipe(RGB_Strip.Color(0, 0, 0));
    }
  }


  if (Vocalization) {
    fengmingqi();
    Vocalization = false;
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
  bool ok = WifiEspNow.begin();
  if (!ok) {
    Serial.println("WifiEspNow.begin() Failure");
    ESP.restart();
  }
  WifiEspNow.onReceive(printReceivedMessage, nullptr);
  ok = WifiEspNow.addPeer(PEER);
  if (!ok) {
    Serial.println("WifiEspNow.addPeer() Failure");
    ESP.restart();
  }
}

//Send data function to master node
void Send_data(String str) {
  char data[60];
  int len = str.length();
  for (int i = 0; i < len; i++) {
    if (i > 60) {
      break;
    }
    data[i] = str[i];
  }
  bool f = WifiEspNow.send(PEER, reinterpret_cast<const uint8_t*>(data), len);
  Serial.println("Sending data：" + str);
  if (f) {
    Serial.println("Send successfully");
  } else {
    Serial.println("Sending failure");
  }
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


    if (read_data == "a" && Mode == "one")  
    {
      time = millis();
      colorWipe(RGB_Strip.Color(255, 0, 0));  
      fengmingqi();
      Modeone = true;
      Modetwo = false;
    }

    if (read_data == "a" && Mode == "two")  
    {
      colorWipe(RGB_Strip.Color(0, 255, 0));  
      fengmingqi();
      Modetwo = true;
      Modeone = false;
    }

    if (read_data == "a" && Mode == "three") 
    {

      colorWipe(RGB_Strip.Color(0, 0, 255));
      fengmingqi();
      colorWipe(RGB_Strip.Color(0, 0, 0));
    }

    if (read_data == "a" && Mode == "threes")  
    {

      colorWipe(RGB_Strip.Color(0, 0, 255));
      fengmingqi();
    }



    if (read_data == "one")  
    {
      colorWipe(RGB_Strip.Color(0, 255, 0)); 
      Vocalization = true;
      Mode = "one";
      Singlenode = true;
    }
    if (read_data == "two")  
    {
      colorWipe(RGB_Strip.Color(0, 0, 0));
      Vocalization = true;
      Mode = "two";
      Singlenode = true;
    }
    if (read_data == "three")  
    {
      Vocalization = true;
      Mode = "three";
      Singlenode = true;
    }
    if (read_data == "threes")  
    {
      Mode = "threes";
    }


    if (read_data == "gb")  
    {
      colorWipe(RGB_Strip.Color(0, 0, 0));
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