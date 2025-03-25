# Contribution to ESP32 IoT No-Code Platform

## Overview
This document outlines my contributions to the ESP32 IoT No-Code Platform project. I focused on enhancing the user interface through HTML/CSS modifications and improving the WiFi functionality with various source code updates. The WiFiManager will appear instantly when you connected to "esp32-iot" ssid, you can configure the network of ESP32 module. where you can use it as "station".

## Changes Made
1. Improved backend and frontend of user control page (esp32_control_backend.php).
2. Recreated the wifi Validation Page, by adding wifimanager option for easy access in (esp32_wifi_credentials_validation.php),(clubbed all php, html, css in one file).
3. Front end & Designing Part for every page (html,CSS).

### Libraries Used
- WiFiManager.h
   - by tzapu, link goes here : https://github.com/tzapu/WiFiManager.git
- WiFi.h
- we can use these Libraries and add more parameters.
#### Installing Library
- step 1: Install Arduino CLI (if you haven't already)
- step 2: Follow the following commands..
```
arduino-cli config init
arduino-cli lib search "WiFiManager"
arduino-cli lib install "WiFiManager"
arduino-cli lib install "WiFiManager@latest"
arduino-cli lib list
```
- Step 5: Upload Code to Your Arduino (optional)
```
arduino-cli board list
arduino-cli compile --fqbn arduino:avr:uno /path/to/your/sketch
arduino-cli upload -p /dev/ttyUSB0 --fqbn arduino:avr:uno /path/to/your/sketch
```

### HTML and CSS Improvements
- Created responsive dashboard layout for better mobile compatibility
- Designed custom widget components for sensor data visualization
- Implemented a dark mode theme option
- Added accessibility features for improved user experience

### Source Code Modifications
- Fixed several bugs in the device discovery process.
- Integrated WiFiManager library for easier WiFi configuration and management.

## How to Test
1. Clone the repository
2. Follow setup instructions in the main README
3. Navigate to the dashboard to see UI improvements
4. Connect a supported sensor to verify the new functionality

## Future Improvements
- will add new features and more accessible functions
- Unit tests for the modified components