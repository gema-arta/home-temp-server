#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <DHT.h>

// Wi-Fi credentials.
#ifndef STASSID
#define STASSID "u_pasovcu"
#define STAPSK  "pasovec1"
#endif

// Used DHT-11 on GPIO13 (D7)
#define DHTPIN 13
#define DHTTYPE DHT11

// Misc defines.
#define SERIAL_BAUD 115200
#define UDP_PORT 33666
#define SENSORD_ID "sens_test"
#define SLEEP_SEC 5

// Globals.
WiFiUDP Udp;
DHT dht(DHTPIN, DHTTYPE);
IPAddress server_ip(192, 168, 0, 150);

void setup() {
  Serial.begin(SERIAL_BAUD);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);

  // Wait while connecting to network.
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(50);
  }

  Serial.println("Connected to Wi-Fi!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  dht.begin();

  // Read sensor data.
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
  }
  else {
    char buffer [50];
    int used_buf = sprintf(buffer, "%s;%.2f;%.2f", SENSORD_ID, t, h);
  
    Serial.println(buffer);
    
    Udp.beginPacket(server_ip, 33666);
    Udp.write(buffer, used_buf);
    int res = Udp.endPacket();

    Serial.println(res);
  }

  ESP.deepSleep(SLEEP_SEC * 1e6);
}

void loop() {
}
