# 🚀 No-Code-Platform for ESP32
🔹 **And the best part?** If you understand my code, you can easily add any featuere you want to esp32 to have and make it happen by directly changing the code.
## 📌 Overview

This platform allows you to control your **ESP32**, manage **TFT Display**, interface with **DHT Sensors**, **MPU6500 series**, and even **older sensors**, while plotting real-time graphs 📊. The entire system is **RTOS-based**, ensuring efficient task management and real-time updates.
## ScreenShots of the Website 
# Index Page
<img width="800" alt="Screenshot 2025-03-28 at 3 48 56 PM" src="https://github.com/user-attachments/assets/623a9086-f528-4e9b-8193-b93911170aad" />

# User Control
<img width="800" alt="Screenshot 2025-03-28 at 3 43 21 PM" src="https://github.com/user-attachments/assets/0c8d64b4-8c40-44ff-a92a-1f94b8f695f4" />
<img width="800" alt="Screenshot 2025-03-28 at 3 38 12 PM" src="https://github.com/user-attachments/assets/f2a1af67-bcf2-4203-b823-b15d19fa4082" />

# WiFi
<img width="800" alt="Screenshot 2025-03-28 at 3 38 56 PM" src="https://github.com/user-attachments/assets/5833091e-fc49-4d7c-96f3-0b84f82b757a" />

## Playground
<img width="800" alt="Screenshot 2025-03-28 at 3 39 31 PM" src="https://github.com/user-attachments/assets/34e74684-5a2c-40bd-a4ba-3e4127155c0a" />

# Playground with block
<img width="800" alt="Screenshot 2025-03-28 at 3 41 51 PM" src="https://github.com/user-attachments/assets/4be11ad4-56b1-4572-abcb-34a4c33b28c3" />

# Playground with Plots (RealTime)
<img width="800" alt="Screenshot 2025-03-28 at 3 40 33 PM" src="https://github.com/user-attachments/assets/ccbd979a-16ae-479a-ab48-23a55834fd5c" />

### 🔥 Features

- **Real-time control of ESP32** ⏳ *(Max delay: 500ms on AP, varies with external router)*
- **Drag & Drop UI Playground** 🎨 *(No need to write code! Just arrange blocks.)*
- **Graphs for Sensor Readings** 📈 *(View real-time plots of data.)*
- **WiFi Connection & Scanning** 📿 *(Switch between AP & Station mode.)*
- **Minimal Power Testing Mode** 🥪 *(Test with limited features.)*

## 📞 ESP32 Setup

### 🌐 WiFi Configuration

1. ESP32 starts as an **Access Point (AP)** with credentials:
   - **SSID:** `esp32-iot`
   - **Password:** `Sai12345` *(You can change this in ****`esp32_main.cpp`****)*
2. The ESP32 AP IP address is **192.168.4.1**.
3. Once connected, send requests via this URL, and ESP32 will respond.
4. If ESP32 switches to **Station Mode (STA)**, it gets a **dynamic IP**. The **current IP** is stored in `/dev/file_esp32_ip.txt` using PHP.

---

## 🛠 Playground UI

> The **No-Code Playground** is where the magic happens! ✨

The interface is divided into **4 sections**:

- **🛠 Tools** - Essential blocks for controlling ESP32
- **🎮 Playground** - Drag & Drop blocks here to create logic
- **📂 Variables** - View & manage declared variables
- **📊 Graphs** - Click the Graphs icon to visualize sensor data

### 🎯 How It Works

- **Drag blocks** from Tools to Playground. Each block is assigned a class: `playground_element`.
- **Variables update** Variables created by user dynamically updates the **Variables** part every 1 sec using JavaScript.
- **Submit Button** 🔘 triggers a `fetch` request to PHP, which communicates with ESP32 via **POST requests** (No GET requests used!).
- **PHP** 🎥 acts as a bridge between ESP32 and the web client using `cURL`.

---

## 🔗 PHP Server Setup

### 🌍 Local Server for Client

To start a **PHP Server**, run:

```bash
php -S localhost:8000
```

This allows the client to connect and control the ESP32.

### 🗰 ESP32 as a Network Device

To enable the ESP32 to be accessed over a **Local Area Network (LAN)**, use `esp32_php_server_start.php` in `wifiConnect`:

```php
<?php
if (isset($_POST["ip"]) == 1 && isset($_POST["wifi-set"]) == 1) {
    $ip = shell_exec("ipconfig getifaddr en0 ");
    $ip = preg_replace("/[^a-zA-Z0-9-_\.]/", "", $ip);
    $pid = shell_exec("cd ../ \n php -S $ip:8000 > /dev/null 2>&1 & echo $!");
    header("Location: http://$ip:8000/../esp32_user_control.html");
} else {
    echo "Failed to start php server";
}
```

🔹 This works for **macOS**. Windows users may need to manually adjust the server location.

---

## 📂 File Structure

- **`/dev`** - Main directory
  - **`/playground`** - Everything about the **drag & drop UI**
  - **`/wifiConnect`** - Handles **WiFi scanning & AP/Station switching**
  - **`/exp`** - Experimental folder *(Ignore it!)*

---

## 📌 Libraries Required

### 🏆 **For MPU6500 I2C**

```bash
arduino-cli lib install "MPU6050_tockn"
arduino-cli lib install "I2Cdev"
```

Supports **MPU6050 & MPU6500**.

### 🖥 **For TFT Display**

```bash
arduino-cli lib install "olikraus/Ucglib@^1.5.2"
```

### 🌧 **For DHT Sensors**

```bash
arduino-cli lib install "adafruit/Adafruit Unified Sensor@^1.1.15"
arduino-cli lib install "adafruit/DHT sensor library@^1.4.6"
```

---

## 🎯 Final Thoughts

This framework is built to **minimize ESP32 computation** while maximizing **expandability**.

- Want to **add new features**? Just tweak a few lines of code! 🛠️
- **Communication is Serial**: All blocks are **forms** with unique tags; a **universal submit button** processes all requests.
- The **JS response** stores data, updates variables, and plots graphs dynamically!

### 🔄 Visualizing the Process

```
[ESP32]  <--->  [PHP (Server)]  <--->  [JS (Client)]
 |                  |                      |
 |  Handles WiFi    |  Sends & Receives     |  Updates UI, Variables & Graphs
 |  Controls Sensors|  Handles Logic        |  Manages Playground Elements
```

🔹 **How it all connects:**
- The **Playground UI** creates forms representing the logic.
- The **Universal Submit Button** sends data to PHP using **fetch()**.
- **PHP processes** the request and relays it to the **ESP32**.
- **ESP32 executes the command** and **sends back data**.
- PHP then **returns the data to JS**, which updates variables and graphs.

🔹 **And the best part?** You can easily extend it by adding new sensor support or logic blocks without modifying core structures!

🔹 **This is just the beginning!** The No-Code Platform is designed to be scalable and extensible—so go ahead, experiment, and build amazing IoT projects! 🚀

---

## 👨‍💼 Contributors

Here are the amazing people who have contributed to this project:

| Name          | Contribution             |
|--------------|-------------------------|
| [Dhanush]  | [TFT display, features]      |
| [Martin]  | [Added Wifi Scan, improved the GUI of user control and index and wifi login page]      |
| [Korak]  | [DHT sensor, MPU6500 features and helped in ploting of graphs of sensor data]      |
| [Puneeth]  | [Helped in debugging the esp32 code and hardware support]      |








