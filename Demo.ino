#include <Wire.h>
#include <SFE_BMP180.h>
#include <ESP8266WiFi.h>
#include "DHT.h"

// DHT11 Settings
#define DHTPIN D3     // Pin connected to the DHT11 sensor
#define DHTTYPE DHT11 // DHT11 type sensor

// Initialize sensors
DHT dht(DHTPIN, DHTTYPE);
SFE_BMP180 bmp;
WiFiClient client;

// Rain sensor connected to A0
int rainSensorPin = A0;

// ThingSpeak API Key and WiFi credentials
String apiKey = "HZ9CM2XWCJL0XZ1Y";
const char *ssid = "Airtel_MSKUSH";
const char *password = "monty@1980";
const char *server = "api.thingspeak.com";

double temperature, pressure;
char status;

void setup() {
  // Start serial communication
  Serial.begin(115200);
  delay(10);

  // Start the sensors
  Wire.begin();
  bmp.begin();
  dht.begin();

  // Connect to WiFi
  connectWiFi();
}

void loop() {
  // Read from the BMP180 sensor
  status = bmp.startTemperature();
  if (status != 0) {
    delay(status);
    status = bmp.getTemperature(temperature);
    status = bmp.startPressure(3); // Oversampling
    if (status != 0) {
      delay(status);
      status = bmp.getPressure(pressure, temperature);
    }
  }

  // Read from DHT11 sensor
  float humidity = dht.readHumidity();
  float temp = dht.readTemperature();

  // Check if any reads failed
  if (isnan(humidity) || isnan(temp)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Read from the rain sensor
  int rainLevel = analogRead(rainSensorPin);
  rainLevel = map(rainLevel, 0, 1023, 100, 60); // Convert to percentage

  // Post data to ThingSpeak
  if (client.connect(server, 80)) {
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(temp);
    postStr += "&field2=";
    postStr += String(humidity);
    postStr += "&field3=";
    postStr += String(pressure / 100.0, 2); // Convert to hPa
    postStr += "&field4=";
    postStr += String(rainLevel);
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);

    Serial.print("Temperature: ");
    Serial.println(temp);
    Serial.print("Humidity: ");
    Serial.println(humidity);
    Serial.print("Pressure: ");
    Serial.print(pressure / 100.0);
    Serial.println(" hPa");
    Serial.print("Rain Level: ");
    Serial.println(rainLevel);
  }

  client.stop();

  // Delay before sending data again
  delay(2000); // 20 seconds delay between posts
}

void connectWiFi() {
  // Connect to the specified WiFi network
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
}
