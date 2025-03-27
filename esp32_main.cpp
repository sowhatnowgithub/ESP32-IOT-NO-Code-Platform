#include <WiFi.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <SPI.h>
#include "Ucglib.h"

#define DHTTYPE_11 DHT11
#define T 4000
#define DLY() delay(2000)

// Access Point credentials
const char* ssid_ap = "esp32-iot";
const char* pass_ap = "Sai12345";

// Fixed IP configuration
IPAddress local_ip(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress gateway(192, 168, 4, 1);
IPAddress brodcast_ip(192, 168, 4, 255);

// Global variables
IPAddress ip;
IPAddress ip_ap;
String str1;
String str2;
String ssid;
String password;

// Display setup
Ucglib_ST7735_18x128x160_HWSPI ucg(/*cd=*/ 21, /*cs=*/ 5, /*reset=*/ 22); //set these connectioin pins cd ->A0

// Server setup
WiFiServer server(80);

void text_display(String s) {
  ucg.setFont(ucg_font_ncenR12_tr);
  ucg.setColor(255, 255, 255);
  //ucg.setColor(0, 255, 0);
  ucg.setColor(1, 255, 0, 0);

  ucg.setPrintPos(0, 25);
  ucg.print(s);
  delay(500);
}

void setup() {
  Serial.begin(115200);

  // Initialize display
  ucg.begin(UCG_FONT_MODE_TRANSPARENT);
  //ucg.begin(UCG_FONT_MODE_SOLID);
  ucg.clearScreen();

  // Configure AP with specific IP settings
  WiFi.softAPConfig(local_ip, brodcast_ip, subnet);

  // Start the Access Point
  while (!WiFi.softAP(ssid_ap, pass_ap)) {
    Serial.println("Bruh, Couldn't make the access point");
    delay(500);
  }

  // Get and display the AP IP address
  ip_ap = WiFi.softAPIP();
  Serial.print("For Access Point IP:- ");
  Serial.println(ip_ap);

  // Start the web server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  String header;
  WiFiClient client = server.accept();
  if (client) {
    String current_line = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        header += c;
        Serial.write(c);
        if (c == '\n') {
          if (current_line.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // Handle WiFi scanner page
            if (header.indexOf("GET /wifiscan HTTP") >= 0) {
              client.println("<html><body><h1>WiFi Scanner</h1>");
              client.println("<div style='margin-bottom: 10px;'>");
              client.println("<button onclick=\"scanNetworks()\" style='margin-right: 10px;'>Scan for WiFi Networks</button>");
              client.println("<button onclick=\"history.back()\">Back</button>");
              client.println("</div>");
              client.println("<pre id='results'></pre>");
              client.println("<script>");
              client.println("function scanNetworks() {");
              client.println("  fetch('/scan').then(response => response.text()).then(data => {");
              client.println("    document.getElementById('results').innerText = data;");
              client.println("  });");
              client.println("}");
              client.println("</script>");
              client.println("</body></html>");
            }

            // Handle scan request
            else if (header.indexOf("GET /scan") >= 0) {
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
              client.println(result);
            }

            // Original server functionality
            else {
              client.print("ESP32 - Currently located at :");
              if (WiFi.status() == WL_CONNECTED)
                client.println(ip);
              else
                client.println(ip_ap);
              client.print("<br>");

              if (header.indexOf("GET /wifi_connect") >= 0) {
                Serial.println("Entered here");
                unsigned short int pos_start = header.indexOf("/ssid="); // 13
                unsigned short int pos_end = header.indexOf("/pass=");
                ssid = header.substring(pos_start + 6, pos_end);
                pos_start = header.indexOf("/end");
                password = header.substring(pos_end + 6, pos_start);
                client.println("<br>");
                client.println(ssid);
                client.println("<br>");
                client.println(password);
                client.println("<br>");
                WiFi.begin(ssid, password);
                unsigned long startAttemptTime = millis();
                while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
                  delay(500);
                  Serial.print('.');
                }
                if (WiFi.status() == WL_CONNECTED) {
                  ip = WiFi.localIP();
                  client.println("Now you can connect to your ssid and use the esp32, Connect to the following IP <br>");
                  client.print("WiFi Ip: ");
                  client.println(ip);
                  client.println("<br>");
                } else {
                  client.println("connection Failed");
                }
              }

              if (header.indexOf("GET /digital/OUT") >= 0) {
                unsigned short int pos_start = header.indexOf("/gpio_pin="); // 13
                unsigned short int pos_end = header.indexOf("/status=");
                String gpio_pin = header.substring(pos_start + 10, pos_end);
                pos_start = header.indexOf("/end");
                String gpio_pin_status = header.substring(pos_end + 8, pos_start);
                int pin = gpio_pin.toInt();
                int pin_status = gpio_pin_status.toInt();
                pinMode(pin, OUTPUT);
                digitalWrite(pin, pin_status);
              }
              else if (header.indexOf("GET /digital/IN") >= 0) {
                unsigned short int pos_start = header.indexOf("/gpio_pin="); // 13
                unsigned short int pos_end = header.indexOf("/status=");
                String gpio_pin = header.substring(pos_start + 10, pos_end);
                pos_start = header.indexOf("/end");
                String gpio_pin_status = header.substring(pos_end + 8, pos_start);
                int pin = gpio_pin.toInt();
                if (gpio_pin_status == "INPUT_PULLUP") {
                  pinMode(pin, INPUT_PULLUP);
                }
                else if (gpio_pin_status == "INPUT_PULLDOWN") {
                  pinMode(pin, INPUT_PULLDOWN);
                }
                else {
                  pinMode(pin, INPUT);
                }
                client.print("The status of the pin_");
                client.print(pin);
                client.print(" is ");
                client.println(digitalRead(pin));
                client.print("<br>");
              }
              else if (header.indexOf("GET /digital/dac") >= 0) {
                unsigned short int pos_start = header.indexOf("/gpio_pin="); // 13
                unsigned short int pos_end = header.indexOf("/status=");
                String gpio_pin = header.substring(pos_start + 10, pos_end);
                pos_start = header.indexOf("/end");
                String gpio_pin_status = header.substring(pos_end + 8, pos_start);
                int pin = gpio_pin.toInt();
                int pin_status = gpio_pin_status.toInt();
                dacWrite(pin, pin_status);
              }
              else if (header.indexOf("GET /digital/disable-dac") >= 0) {
                unsigned short int pos_start = header.indexOf("/gpio_pin="); // 13
                unsigned short int pos_end = header.indexOf("/end");
                String gpio_pin = header.substring(pos_start + 10, pos_end);
                int pin = gpio_pin.toInt();
                dacDisable(pin);
              }
              else if (header.indexOf("GET /delay") >= 0) {
                unsigned short int pos_start = header.indexOf("/delay="); // 13
                unsigned short int pos_end = header.indexOf("/end");
                String gpio_pin = header.substring(pos_start + 7, pos_end);
                int delay_value = gpio_pin.toInt();
                delay(delay_value);
              }
              else if (header.indexOf("GET /analog/write/") >= 0) {
                unsigned short int pos_start = header.indexOf("/gpio_pin="); // 13
                unsigned short int pos_end = header.indexOf("/freq=");
                String gpio_pin = header.substring(pos_start + 10, pos_end);
                pos_start = header.indexOf("/res=");
                String attach_freq = header.substring(pos_end + 6, pos_start);
                pos_end = header.indexOf("/duty=");
                String write_res = header.substring(pos_start + 5, pos_end);
                pos_start = header.indexOf("/end");
                String write_duty = header.substring(pos_end + 6, pos_start);
                int duty = write_duty.toInt();
                int res = write_res.toInt();
                int pin = gpio_pin.toInt();
                int freq = attach_freq.toInt();
                analogWrite(pin, duty);
                analogWriteResolution(res);
                analogWriteFrequency(freq);
                Serial.print("GPIO Pin: ");
                Serial.println(pin);
                Serial.print("Frequency: ");
                Serial.println(freq);
                Serial.print("Resolution: ");
                Serial.println(res);
                Serial.print("Duty Cycle: ");
                Serial.println(duty);
              }
              else if (header.indexOf("GET /text/") >= 0) {
                unsigned short int pos_start = header.indexOf("/display="); //
                unsigned short int pos_end = header.indexOf("/end");
                String text = header.substring(pos_start + 9, pos_end);
                text_display(text);
              }
              else if (header.indexOf("GET /sensor/DHT11") >= 0) {
                unsigned short int pos_start = header.indexOf("/gpio_pin="); // 13
                unsigned short int pos_end = header.indexOf("/end");
                String pin = header.substring(pos_start + 10, pos_end);
                int pin_new = pin.toInt();

                DHT dht(pin_new, DHTTYPE_11);  // Initialize DHT sensor
                dht.begin();
                delay(500);  // Wait for sensor response

                float humidity = dht.readHumidity();      // Read humidity
                float temperatureC = dht.readTemperature(); // Read temperature (Celsius)
                float temperatureF = dht.readTemperature(true); // Read temperature (Fahrenheit)

                if (isnan(humidity) || isnan(temperatureC) || isnan(temperatureF)) {
                  Serial.println("⚠️ Failed to read from DHT sensor!");
                  return;
                }

                // Compute heat index
                float heatIndexC = dht.computeHeatIndex(temperatureC, humidity, false);
                float heatIndexF = dht.computeHeatIndex(temperatureF, humidity);

                // Print Data
                client.print("Humidity: "); client.print(humidity); client.print("% | ");
                client.print("Temperature: "); client.print(temperatureC); client.print("°C / ");
                client.print(temperatureF); client.print("°F | ");
                client.print("Heat Index: "); client.print(heatIndexC); client.print("°C / ");
                client.print(heatIndexF); client.println("°F");
              }
            }

            client.println();
            break;
          }
          else {
            current_line = "";
          }
        }
        else if (c != '\r') {
          current_line += c;
        }
      }
    }
    client.stop();
    Serial.println("Client Disconnected");
  }
}
