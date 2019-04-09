#include <WiFiUdp.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include "wifitools.h"

// Data pins for BME280.
#define D1 5
#define D2 4
#define D3 0
#define D4 2

#define BME_SCK D1
#define BME_MISO D4
#define BME_MOSI D2
#define BME_CS D3

#define SEALEVELPRESSURE_HPA (1013.25)

// Misc defines.
#define SERIAL_BAUD 115200
#define SERVER_IP 192, 168, 0, 150
#define SERVER_PORT 33666

#define SENSOR_ID "sens_test"
#define SLEEP_SEC 10
#define SEC_TO_STABILIZE 1.0

Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK);

void setup()
{
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
    bme.begin();
    
    float t = bme.readTemperature();
    float h = bme.readHumidity();

    /*
    Serial.print("Pressure = ");

    Serial.print(bme.readPressure() / 100.0F);
    Serial.println(" hPa");
    */

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t))
    {
      Serial.println(F("Failed to read from sensor!"));
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
