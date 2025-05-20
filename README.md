# IoT25-HW05: ESP32 BLE Client for Environmental Sensor Display

This project demonstrates how to create a Bluetooth Low Energy (BLE) connection between two ESP32 devices: one acting as a **BLE Server** sending environmental sensor data (temperature and humidity), and the other as a **BLE Client** receiving and displaying that data on an **OLED screen**.

---

## 🧾 Objectives

- Establish a BLE connection between ESP32 devices (client ↔ server)
- Receive real-time sensor data via BLE (temperature & humidity)
- Display values on an OLED (SSD1306)
- Use BLE notify characteristics for efficient data transfer

---

## 🧰 Components Used

- 2× ESP32 DevKit boards  
- OLED Display (SSD1306 128×64, I2C)  
- Temperature & Humidity Sensor (on server side)  
- Jumper wires  
- Breadboard  
- Micro USB cables  

---

## 💡 Project Structure

- **BLE Server (not shown in code)** sends temperature and humidity readings via BLE characteristics.
- **This Client Code**:
  - Scans for the BLE server with name `ESP32_BLE_Server`
  - Connects using known UUIDs for temperature and humidity characteristics
  - Receives data via BLE notifications
  - Displays the readings on an OLED screen using Adafruit SSD1306 library

---

## 📄 Code Summary (Client)

- Temperature Characteristic UUID:  
  `cba1d466-344c-4be3-ab3f-189f80dd7518` (°C)  
- Humidity Characteristic UUID:  
  `ca73b3ba-39f6-4ab3-91ae-186dc9577d99`  

Main logic:
- Uses `BLEAdvertisedDeviceCallbacks` to detect the server
- Uses `registerForNotify()` to subscribe to live updates
- Renders readings on OLED using `Adafruit_SSD1306`

---

## 📸 Media

**Hardware Setup Photos**:

- ![Photo 1](./media/hw%205-1.png)  
- ![Photo 2](./media/hw%205-2.png)  
- ![Photo 3](./media/hw%205-3.png)  
- ![Photo 4](./media/hw%205-4.jpg)  
- ![Photo 5](./media/hw%205-5.jpg)  
- ![Photo 6](./media/hw%205-6.jpg)

---

## ▶ Demo

- ![GIF Demo](./media/IoT25-HW05.gif)

(Also available as [video](./media/IoT25-HW05.mp4))

---

## ✅ Results

- [x] ESP32 client successfully scans and connects to server  
- [x] Receives temperature and humidity data via BLE notify  
- [x] OLED screen refreshes with updated data in real time  
- [x] BLE connection automatically starts once server is detected

---

## 🔗 References

- [ESP32 BLE Server-Client Tutorial](https://randomnerdtutorials.com/esp32-ble-server-client/)
- [Adafruit SSD1306 OLED Guide](https://learn.adafruit.com/monochrome-oled-breakouts/arduino-library-and-examples)
