#include <esp_now.h>
#include <WiFi.h>

//MAC
uint8_t MAC1[] = {0x34, 0xB4, 0x72, 0xEE, 0xFD, 0x68};

typedef struct struct_message {
  float a;
} struct_message;

unsigned long myTime1;

struct_message sendData;
struct_message receData;

esp_now_peer_info_t peerInfo;

// Callback when data is sent   
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  if(status == ESP_NOW_SEND_SUCCESS){
    //Serial.println("Send Success");
  }else{
    //Serial.println("Send Fail");
  }
}

// Callback when data is received   
void OnDataRecv(const uint8_t * mac, const uint8_t *Data, int len) {
  Serial.print("Time: ");
  Serial.print((millis()-myTime1)/2);
  Serial.println("ms");
  memcpy(&receData, Data, sizeof(receData));
  Serial.print("Data: ");
  Serial.println(receData.a);
  Serial.println("---------");
}
 
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  //Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing");
    return;
  }
  
  
  esp_now_register_send_cb(OnDataSent);
  
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  memcpy(peerInfo.peer_addr, MAC1, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  
  esp_now_register_recv_cb(OnDataRecv);

  sendData.a = 15.5;
}


 
void loop() {
  myTime1 = millis();
  esp_err_t result = esp_now_send(MAC1, (uint8_t *)&sendData, sizeof(sendData));
  delay(1000);
}