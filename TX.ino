#include <esp_now.h>
#include <WiFi.h>

const int SensePin = 13;

// Replace with your Receiver's MAC Address
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

typedef struct struct_message {
  bool liquidDetected;
} struct_message;

struct_message myData;
esp_now_peer_info_t peerInfo;

// Updated callback function signature for ESP32 Arduino Core v3.x
void OnDataSent(const wifi_tx_info_t *info, esp_now_send_status_t status) {
  Serial.print("Delivery Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
  Serial.begin(115200);
  pinMode(SensePin, INPUT);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  bool currentState = digitalRead(SensePin);
  myData.liquidDetected = currentState;

  if (currentState) {
    Serial.println("Liquid Detected - Sending state...");
  } else {
    Serial.println("No Liquid Available - Sending state...");
  }

  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

  delay(1000);
}
