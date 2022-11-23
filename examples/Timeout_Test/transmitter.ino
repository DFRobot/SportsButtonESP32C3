#include <esp_now.h>
#include <WiFi.h>

uint8_t MAC1[] = {0x84, 0xF7, 0x03, 0x41, 0xAF, 0x48};

typedef struct struct_message {
  float a;
} struct_message;

uint8_t flag = 0;

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
  flag = 1;
  memcpy(&receData, Data, sizeof(receData));
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
  sendData.a = 16.5;
}
 
void loop() {
  if(flag == 1){
    esp_err_t result = esp_now_send(MAC1, (uint8_t *)&sendData, sizeof(sendData));
    flag = 0;
    Serial.print("Data: ");
    Serial.println(receData.a);
    Serial.println("---------");
  }
}