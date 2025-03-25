#include<WiFi.h>
#include <WiFiManager.h>

void setup(){
	WiFi.mode(WIFI_STA);
	Serial.begin(115200);
	WiFiManager wm;
	wm.resetSettings();
  
	bool res;
	res = wm.autoConnect("esp32-iot","Sai12345");
  
	if(!res) {
	  Serial.println("Failed to Connect...:(");
	}
	else{
	  Serial.println("connected yeey...:)");
	}

void loop();