#include <WiFi.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <SPI.h>
#include "Ucglib.h"
#include "Wire.h"
#include "MPU6050_6Axis_MotionApps20.h"
// from here i am declaring for MPU6500
MPU6050 mpu;
bool dmpReady = false;
uint8_t devStatus;
uint16_t packetSize;
uint8_t fifoBuffer[64];
#define ACCEL_SCALE (9.81 / 16384.0)
#define GYRO_SCALE (250.0 / 32768.0)

Quaternion q;
VectorFloat gravity;
float ypr[3];

int16_t ax, ay, az;
int16_t gx, gy, gz;

unsigned long prevMillis = 0;
const int sampleRate = 1000;
// from here i am declaring for DHT11

#define DHTTYPE_11 DHT11
#define T 4000
#define DLY() delay(2000)
// Access Point credentials
// // from here i am declaring for accesspoint

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

// from here i am declaring for ttf

// Display setup
Ucglib_ST7735_18x128x160_HWSPI ucg(/*cd=*/ 21, /*cs=*/ 5, /*reset=*/ 22); //set these connectioin pins cd ->A0


// Server setup
WiFiServer server(80);
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
void display_word(String font_size,String animation_type, String alignment, int r, int g, int b,String s) {
  // Set text color
   // Set text color
   ucg.setColor(r, g, b);

   // Set the font size
   if (font_size == "small") {
     ucg.setFont(ucg_font_ncenR12_tr); // Small font
   } else if (font_size == "medium") {
     ucg.setFont(ucg_font_ncenB18_tr); // Medium font
   } else if (font_size == "larger") {
     ucg.setFont(ucg_font_ncenB24_tr); // Larger font
   } else {
     ucg.setFont(ucg_font_ncenR12_tr); // Default small font
   }
 
   // Alignment handling (for non-animated text)
   int x = 0;
   if (alignment == "left") {
     x = 0;
   } else if (alignment == "right") {
     x = 128 - ucg.getStrWidth(s.c_str()); // Right alignment
   } else if (alignment == "middle") {
     x = (128 - ucg.getStrWidth(s.c_str())) / 2; // Center alignment
   }
 
   // Apply animation type
   if (animation_type == "scrolling") {
     int start_x = -ucg.getStrWidth(s.c_str());  // Start from off-screen (left)
     int end_x = 128;  // Move to off-screen right
     int y = 80;  // Fixed vertical position
 
     int prev_x = start_x; // Store previous position
 
     for (int i = start_x; i <= end_x; i++) {
       // Overwrite previous position with background color (black)
       ucg.setColor(0, 0, 0);
       ucg.setPrintPos(prev_x, y);
       ucg.print(s);
 
       // Draw text at new position
       ucg.setColor(r, g, b);
       ucg.setPrintPos(i, y);
       ucg.print(s);
 
       prev_x = i; // Update previous position
       delay(50); // Adjust speed
     }
   } 
   else if (animation_type == "blinking") {
     for (int i = 0; i < 10; i++) {
       ucg.setColor(i % 2 == 0 ? 0 : r, i % 2 == 0 ? 0 : g, i % 2 == 0 ? 0 : b);
       ucg.setPrintPos(x, 80);
       ucg.print(s);
       delay(500);
     }
   } 
   else {
     ucg.clearScreen();
     ucg.setColor(r, g, b);
     ucg.setPrintPos(x, 80);
     ucg.print(s);
   }
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
              else if(header.indexOf("GET /Display/clear/tft") >= 0)
              {
                  ucg.clearScreen();
              }
              else if(header.indexOf("GET /Display/word/format/")>=0)
              {
                unsigned short int pos_start = header.indexOf("/font=");
                unsigned short int pos_end = header.indexOf("/animation=");
                String font = header.substring(pos_start + 6,pos_end);
                pos_start = pos_end;
                pos_end = header.indexOf("/color_R=");
                String animation = header.substring(pos_start+11,pos_end);
                pos_start = pos_end;
                pos_end = header.indexOf("/color_G="); 
                String color_SR = header.substring(pos_start+8,pos_end);
                pos_start = pos_end;
                pos_end = header.indexOf("/color_B=");
                String color_SG = header.substring(pos_start+8,pos_end);
                pos_start = pos_end;
                pos_end= header.indexOf("/alignment=");
                String color_SB = header.substring(pos_start+8,pos_end);
                pos_start = pos_end;
                pos_end= header.indexOf("/string=");
                String alignment = header.substring(pos_start+10,pos_end);
                pos_start = pos_end;
                pos_end =header.indexOf("/end");
                String string = header.substring(pos_start+10,pos_end);
                int color_R = color_SR.toInt();
                int color_G = color_SG.toInt();
                int color_B = color_SB.toInt();
                display_word(font,animation,alignment,color_R,color_G,color_B,string);
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
              else if(header.indexOf("GET /sensor/mpu6500") >= 0){
                  Wire.begin(21, 22);

                     Serial.println("Initializing MPU6050...");
                     mpu.initialize();

                     devStatus = mpu.dmpInitialize();
                     if (devStatus == 0) {
                         Serial.println("DMP Initialization Successful!");
                         mpu.setDMPEnabled(true);
                         packetSize = mpu.dmpGetFIFOPacketSize();
                         dmpReady = true;
                     } else {
                         Serial.print("DMP Initialization Failed (Code ");
                         Serial.print(devStatus);
                         Serial.println(")");
                     }
                     unsigned long currentMillis = millis();

                     if (currentMillis - prevMillis >= sampleRate) { // Non-blocking delay
                         prevMillis = currentMillis;

                         mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
                         client.print("Accel (m/s^2): "); client.print(ax * ACCEL_SCALE ); client.print("\t");
                         client.print(ay * ACCEL_SCALE); client.print("\t");
                         client.println(az* ACCEL_SCALE );

                         client.print("Gyro (°/s): "); client.print(gx * GYRO_SCALE); client.print("\t");
                         client.print(gy* GYRO_SCALE ); client.print("\t");
                         client.println(gz * GYRO_SCALE);
                         if (dmpReady && mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {
                             mpu.dmpGetQuaternion(&q, fifoBuffer);
                             mpu.dmpGetGravity(&gravity, &q);
                             mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
                             client.print("Yaw: ");client.print(ypr[0] * 180 / M_PI);
                             client.print(" Pitch: "); client.print(ypr[1] * 180 / M_PI);
                             client.print(" Roll: "); client.println(ypr[2] * 180 / M_PI);
                         }
                     }

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
