#include <WiFiUdp.h>
#include <DHT.h>

#include "wifitools.h"

// Used DHT-11 on GPIO13 (D7)
#define DHTPIN_DATA D7
#define DHTPIN_PWR D1   // Other GPIOs did not work well for me.
#define DHTTYPE DHT11

// Misc defines.
#define SERIAL_BAUD 115200
#define SERVER_IP 192, 168, 0, 150
#define SERVER_PORT 33666

#define SENSORD_ID "sens_test"
#define SLEEP_SEC 10
#define SEC_TO_STABILIZE 1.5

void setup()
{
  // Setup DHT power GPIO pin and turn power on.
  pinMode(DHTPIN_PWR, OUTPUT);
  digitalWrite(DHTPIN_PWR, HIGH);

  DHT dht(DHTPIN_DATA, DHTTYPE);
  Serial.begin(SERIAL_BAUD);

  double seconds_to_join;
  bool connected_wifi = connectToWifi(&seconds_to_join);
  double additional_wait = SEC_TO_STABILIZE - seconds_to_join;

  if (connected_wifi)
  {
    // Wait for sensor to stabilize.
    if (additional_wait > 0.0) {
      Serial.printf("Additionally waiting for %f seconds.\n", additional_wait);
      delay(additional_wait * 1000.0);
    }

    // Turn on and initialize sensor.
    dht.begin();

    // Read sensor data.
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    // Turn off power to sensor.
    digitalWrite(DHTPIN_PWR, LOW);

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

  digitalWrite(DHTPIN_PWR, LOW);

  // Turn off sensor and deep sleep.
  ESP.deepSleep(SLEEP_SEC * 1e6);
}

void loop()
{
}
