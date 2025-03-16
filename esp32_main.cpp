#include<WiFi.h>
#include<string.h>
IPAddress ip;
String str1;
String str2;
IPAddress ip_ap;
WiFiServer server(80);
IPAddress local_ip(192,168,4,1);
IPAddress subnet(255,255,255,0);
IPAddress brodcast_ip(192,168,4,255);
const char *ssid_ap = "esp32-iot";
const char *pass_ap = "Sai12345";
String ssid;
String password;
void setup(){
  Serial.begin(115200);

  WiFi.softAPConfig(local_ip,brodcast_ip,subnet);
  while(!WiFi.softAP(ssid_ap,pass_ap)){
    Serial.println("Bruh, Couldn't make the access point");
  }
  ip_ap = WiFi.softAPIP();
  Serial.print("For Access Point IP:- ");
  Serial.println(ip_ap);
  server.begin();
}


void loop() {
  String header;
  WiFiClient client = server.accept();
  if(client){
    String current_line="";
    while(client.connected()){
      if(client.available()){
        char c = client.read();
        header+=c;
        Serial.write(c);
        if(c == '\n'){
          if(current_line.length() == 0){
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            client.print("ESP32 - Currently located at :");
            if(WiFi.status() == WL_CONNECTED) client.println(ip);
            else client.println(ip_ap);
            client.print("<br>");
            if(header.indexOf("GET /wifi_connect") >= 0){
              Serial.println("Entered here");
              unsigned short int pos_start = header.indexOf("/ssid="); // 13
              unsigned short int pos_end = header.indexOf("/pass=");
              ssid = header.substring(pos_start+6,pos_end);
              pos_start = header.indexOf("/end");
              password = header.substring(pos_end+6, pos_start);
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
             if(WiFi.status() == WL_CONNECTED){
              ip = WiFi.localIP();
              client.println("Now you can connect to your ssid and use the esp32, Connect to the following IP <br>");
              client.print("WiFi Ip: ");
              client.println(ip);
              client.println("<br>");
             }
             else {
              client.println("connection Failed");
             }

            }
            if(header.indexOf("GET /digital/OUT") >= 0){
              unsigned short int pos_start = header.indexOf("/gpio_pin="); // 13
              unsigned short int pos_end = header.indexOf("/status=");
              String gpio_pin = header.substring(pos_start+10,pos_end);
              pos_start = header.indexOf("/end");
              String gpio_pin_status = header.substring(pos_end+8, pos_start); 
              int pin =  gpio_pin.toInt();
              int pin_status = gpio_pin_status.toInt();
              pinMode(pin,OUTPUT);
              digitalWrite(pin,pin_status);
            }
            else if(header.indexOf("GET /digital/IN") >= 0){
              unsigned short int pos_start = header.indexOf("/gpio_pin="); // 13
              unsigned short int pos_end = header.indexOf("/status=");
              String gpio_pin = header.substring(pos_start+10,pos_end);
              pos_start = header.indexOf("/end");
              String gpio_pin_status = header.substring(pos_end+8, pos_start); 
              int pin =  gpio_pin.toInt();
              if(gpio_pin_status == "INPUT_PULLUP"){
                pinMode(pin,INPUT_PULLUP);
              }
              else if(gpio_pin_status == "INPUT_PULLDOWN"){
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
            else if(header.indexOf("GET /digital/dac") >= 0){
              unsigned short int pos_start = header.indexOf("/gpio_pin="); // 13
              unsigned short int pos_end = header.indexOf("/status=");
              String gpio_pin = header.substring(pos_start+10,pos_end);
              pos_start = header.indexOf("/end");
              String gpio_pin_status = header.substring(pos_end+8, pos_start); 
              int pin =  gpio_pin.toInt();
              int pin_status = gpio_pin_status.toInt();
              dacWrite(pin, pin_status);
            }
            else if(header.indexOf("GET /digital/disable-dac") >= 0){
              unsigned short int pos_start = header.indexOf("/gpio_pin="); // 13
              unsigned short int pos_end = header.indexOf("/end");
              String gpio_pin = header.substring(pos_start+10,pos_end);
              int pin =  gpio_pin.toInt();
              dacDisable(pin);
            }
            else if(header.indexOf("GET /delay") >= 0){
              unsigned short int pos_start = header.indexOf("/delay="); // 13
              unsigned short int pos_end = header.indexOf("/end");
              String gpio_pin = header.substring(pos_start+7,pos_end);
              int delay_value =  gpio_pin.toInt();
              delay(delay_value);
            }
            client.println();
            break;
          }
          else {
            current_line = "";
          }
        }
        else if(c != '\r'){
          current_line+=c;
        }
      }

      }
      client.stop();
      Serial.println("Client Disconnected");
   }
}