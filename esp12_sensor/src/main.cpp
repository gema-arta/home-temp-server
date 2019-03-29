#include <WiFiUdp.h>
#include <DHT.h>

#include "wifitools.h"

// Used DHT-11 on GPIO13 (D7)
#define DHTPIN 13
#define DHTTYPE DHT11

// Misc defines.
#define SERIAL_BAUD 115200
#define SERVER_IP 192, 168, 0, 150
#define SERVER_PORT 33666
#define SENSOR_ID "sens_test"
#define SLEEP_SEC 5

void setup()
{
  DHT dht(DHTPIN, DHTTYPE);

  Serial.begin(SERIAL_BAUD);

  // Turn on and initialize sensor.
  dht.begin();

  bool connected_wifi = connectToWifi();

  if (connected_wifi)
  {
    // Read sensor data.
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t))
    {
      Serial.println(F("Failed to read from DHT sensor!"));
    }
    else
    {
      char buffer[32];
      int used_buf = sprintf(buffer, "%s;%.2f;%.2f", SENSOR_ID, t, h);

      Serial.printf("Data for sending to server is: '%s'\n", buffer);

      // Send da shit.
      WiFiUDP Udp;
      Udp.beginPacket(IPAddress(SERVER_IP), SERVER_PORT);
      Udp.write(buffer, used_buf);

      if (Udp.endPacket()) {
        Serial.println(F("Data sent to server successfully."));

        // Make sure the stuff is sent.
        yield();
        delay(100);
      }
      else {
        Serial.println(F("Error, NO data sent to server!"));
      }
    }
  }

  // Turn off sensor and deep sleep.
  ESP.deepSleep(SLEEP_SEC * 1e6);
}

void loop()
{
}
