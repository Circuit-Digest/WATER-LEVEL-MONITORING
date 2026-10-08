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
