#include <esp_now.h>
#include <WiFi.h>

// Matching data structure
typedef struct struct_message {
  bool liquidDetected;
} struct_message;

struct_message incomingData;

// Data receive callback
void OnDataRecv(const esp_now_recv_info *info, const uint8_t *data, int len) {
  memcpy(&incomingData, data, sizeof(incomingData));

  if (incomingData.liquidDetected) {
    Serial.println("Received: Liquid Detected");
  } else {
    Serial.println("Received: No Liquid Available");
  }
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // Callback handles data as it arrives
}
