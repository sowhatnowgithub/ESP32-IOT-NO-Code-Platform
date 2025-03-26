#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 4       // ESP32 GPIO connected to DHT11 Data Pin
#define DHTTYPE DHT11  // Define DHT type (DHT11, DHT22, DHT21)

DHT dht(DHTPIN, DHTTYPE);  // Initialize DHT sensor

void setup() {
  Serial.begin(115200);
  Serial.println("DHT11 Sensor with ESP32");
  
  dht.begin();  // Start DHT sensor
}

void loop() {
  delay(2000);  // Wait for sensor response

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
  Serial.print("Humidity: "); Serial.print(humidity); Serial.print("% | ");
  Serial.print("Temperature: "); Serial.print(temperatureC); Serial.print("°C / ");
  Serial.print(temperatureF); Serial.print("°F | ");
  Serial.print("Heat Index: "); Serial.print(heatIndexC); Serial.print("°C / ");
  Serial.print(heatIndexF); Serial.println("°F");
}
