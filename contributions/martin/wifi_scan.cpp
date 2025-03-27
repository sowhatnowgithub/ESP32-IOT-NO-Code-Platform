// if you want to access th page, you need to type the IP address of the ESP32 in the browser
// http://192.168.4.1 for the AP mode
// from there you can scan for the available networks with their signal strength
// signal strength is measured in dBm, the higher the value the stronger the signal (0 is the strongest)
// This particular code snippet uses the WebServer library to handle routes and responses


#include <WiFi.h>
#include <WebServer.h>

// Access Point credentials
const char* ssid_ap = "esp32-iot";
const char* pass_ap = "Sai12345";

// Fixed IP configuration
IPAddress local_ip(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress gateway(192, 168, 4, 1);

// Global variables
IPAddress ip_ap;
String ssid;
String password;

// Use WebServer instead of WiFiServer for easier route handling
WebServer server(80);

void handleRoot() {
  String html = "<html><body><h1>WiFi Scanner</h1>";
  html += "<div style='margin-bottom: 10px;'>";
  html += "<button onclick=\"scanNetworks()\" style='margin-right: 10px;'>Scan for WiFi Networks</button>";
  html += "<button onclick=\"history.back()\">Back</button>";
  html += "</div>";
  html += "<pre id='results'></pre>";
  html += "<script>";
  html += "function scanNetworks() {";
  html += "  fetch('/scan').then(response => response.text()).then(data => {";
  html += "    document.getElementById('results').innerText = data;";
  html += "  });";
  html += "}";
  html += "</script>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleScan() {
  String result = "Scanning for networks...\n";
  int n = WiFi.scanNetworks();
  result += "Scan done\n";
  if (n == 0) {
    result += "No networks found\n";
  } else {
    result += String(n) + " networks found:\n";
    for (int i = 0; i < n; ++i) {
      result += String(i + 1) + ": " + WiFi.SSID(i) + " (" + WiFi.RSSI(i) + " dBm)\n";
    }
  }
  server.send(200, "text/plain", result);
}

void setup() {
  Serial.begin(115200);
  
  // Configure AP with specific IP settings
  WiFi.softAPConfig(local_ip, gateway, subnet);
  
  // Start the Access Point
  while (!WiFi.softAP(ssid_ap, pass_ap)) {
    Serial.println("Couldn't make the access point, retrying...");
    delay(500);
  }
  
  // Get and display the AP IP address
  ip_ap = WiFi.softAPIP();
  Serial.print("Access Point IP: ");
  Serial.println(ip_ap);

  // Set up web server routes
  server.on("/", handleRoot);
  server.on("/scan", handleScan);
  
  // Start the web server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}