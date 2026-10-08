# 🌊 Open-Source Contactless Wireless Water Level Monitor

An open-source, non-contact water level monitoring system using **ESP32** microcontrollers and the **ESP-NOW** wireless protocol. 

Commercial non-contact water level sensors often cost over **₹700 ($8+)**. This project demonstrates how to build a reliable, open-source, non-contact sensor module for under **₹150 ($1.80)** that detects liquid directly through plastic, glass, or ceramic tank walls **up to 20mm thick** without ever touching the water.

---

## ✨ Features

* **Zero Water Contact:** Prevents sensor corrosion, wiring degradation, and water contamination.
* **Deep Wall Penetration:** Detects liquid levels through glass, plastic, or ceramic walls up to **20 mm** thick.
* **Dual Output Modes (NPN / PNP):** Configurable `MODE` pin allows toggling between active-HIGH (NPN) and active-LOW (PNP) output signals.
* **Adjustable Sensitivity:** Built-in potentiometer (POT) for fine-tuning wall thickness thresholds.
* **Zero-Lag ESP-NOW Protocol:** Direct board-to-board radio communication without needing a Wi-Fi router.
* **Modular Design:** Swappable wireless transceiver support (ESP-NOW, LoRa, or NRF24L01).

---

## 🛠️ Hardware Requirements

| Component | Quantity | Description |
| :--- | :--- | :--- |
| **ESP32 Development Board** | 2 | 1 Transmitter (TX) + 1 Receiver (RX) |
| **Contactless Water Sensor** | 1 | Open-source 4-pin module |
| **Jumper Wires & Power** | - | Micro-USB cables or 5V Power Supply |

---

## 📌 Sensor Pinout & Configuration

The contactless sensor features a 4-pin header:

| Pin Name | Function | Connection / Description |
| :--- | :--- | :--- |
| **VIN** | Power Supply | Connect to **5V** or **3.3V** |
| **GND** | Ground | Common Ground |
| **OUT** | Signal Output | Digital Signal Output (Connect to ESP32 GPIO 13) |
| **MODE** | Output Select | **Floating:** Active-HIGH (NPN mode)<br>**GND:** Active-LOW (PNP mode) |

> 💡 **Sensitivity Adjustment:** Turn the onboard trimpot clockwise/counter-clockwise to adjust the sensitivity based on tank wall material and thickness.

---

## 🔌 Wiring Diagram

### 1. Transmitter Node (TX - Tank Side)
```cpp
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



### Receiver Code (`Receiver.ino`)

The Receiver ESP32 resides inside the house. It listens for incoming ESP-NOW radio packets from the Transmitter and processes the water level status.

```cpp
#include <esp_now.h>
#include <WiFi.h>

// Matching data structure with Transmitter
typedef struct struct_message {
  bool liquidDetected;
} struct_message;

struct_message incomingData;

// ESP-NOW Receive Callback (ESP32 Core v3.x compliant)
void OnDataRecv(const esp_now_recv_info *info, const uint8_t *data, int len) {
  memcpy(&incomingData, data, sizeof(incomingData));

  if (incomingData.liquidDetected) {
    Serial.println("Received: Liquid Detected 💧");
  } else {
    Serial.println("Received: No Liquid Available ❌");
  }
}

void setup() {
  Serial.begin(115200);

  // Set ESP32 to Station mode
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register receive callback function
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // Asynchronous execution: callback handles incoming data automatically
}
